/* level.h */
#ifndef LEVEL_H
#define LEVEL_H

#include "Utils.h"
#include "LevelStruct.h"
#include "maps/Level1_maps.h"



static u8 map_buf[MAP_BUF_H][MAP_BUF_W]={0};
static SCB_REHV_PAL sprite_pool[MAX_ACTIVE_SPRITES];
static u8 sprite_used[MAX_ACTIVE_SPRITES];

static const u8 tile_palette[8] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
};





extern Level level;




/* Dichiarazioni pubbliche */
void level_init(void);
void level_load( u8 level_num);
void level_draw();


void level_init_camera();
void level_update_camera();
//u8 level_get_tile_abs(u16 world_x, u16 world_y);



static inline u8 level_get_tile_abs(u16 tile_x, u16 tile_y)
{
	s16 bx = tile_x - level.map_buf_origin_x;
	s16 by = tile_y - level.map_buf_origin_y;

	if ((u16)bx >= MAP_BUF_W || (u16)by >= MAP_BUF_H)
		return TILE_EMPTY;

	// applica circolarità
	bx += level.map_buf_col0;
	if (bx >= MAP_BUF_W) bx -= MAP_BUF_W;

	by += level.map_buf_row0;
	if (by >= MAP_BUF_H) by -= MAP_BUF_H;

	return map_buf[by][bx];
}


static inline s16 parallax_x(s16 cam_x, u8 shift)
{
    return cam_x >> shift;   // /2, /4, /8
}

static inline u8 decompress_rle_get_value(
		const u8 *data,
		u16 offset,
		u16 col_idx
) {
	const u8 *p = data + offset;
	u16 col = 0;

	for (;;) {
		u8 val = *p++;
		u8 cnt = *p++;

		col += cnt;
		if (col > col_idx)
			return val;
	}
	//return 0;
}


static inline void stream_scroll_right(void)
{
	u8 write_col;
	u8 y;
	u16 new_world_col;
	u16 world_row;
	u8 by;

	/* Avanza origine mondo */
	level.map_buf_origin_x++;

	/* Avanza colonna circolare */
	level.map_buf_col0 = (level.map_buf_col0 + 1) & MAP_BUF_MASK;

	/* Colonna fisica da scrivere */
	write_col = (level.map_buf_col0 + MAP_BUF_W - 1) & MAP_BUF_MASK;

	/* Colonna mondo da caricare */
	new_world_col = level.map_buf_origin_x + MAP_BUF_W - 1;

	if (new_world_col >= level.map_w)
		return;

	for (y = 0; y < MAP_BUF_H; y++) {

		world_row = level.map_buf_origin_y + y;
		if (world_row >= level.map_h) {
			by = (level.map_buf_row0 + y) & MAP_BUF_MASK;
			map_buf[by][write_col] = TILE_EMPTY;
			continue;
		}

		by = (level.map_buf_row0 + y) & MAP_BUF_MASK;

		map_buf[by][write_col] = decompress_rle_get_value(
				Z1L1_FG_MAP_RLE_DATA,
				Z1L1_FG_MAP_RLE_OFFS[world_row],
				new_world_col
		);
	}
}



static inline void stream_scroll_left(void)
{
	u8 write_col;
	u8 y;
	u16 new_world_col;

	/* Retrocede origine mondo */
	if (level.map_buf_origin_x == 0)
		return;

	level.map_buf_origin_x--;
	level.map_buf_col0 = (level.map_buf_col0 + MAP_BUF_W - 1) & MAP_BUF_MASK;

	/* Colonna fisica da scrivere (prima logica) */
	write_col = level.map_buf_col0;
	new_world_col = level.map_buf_origin_x;

	for (y = 0; y < MAP_BUF_H; y++) {

		u16 world_row = level.map_buf_origin_y + y;
		u8  phys_row  = (y + level.map_buf_row0) & MAP_BUF_MASK;

		if (world_row < level.map_h) {
			map_buf[phys_row][write_col] =
					decompress_rle_get_value(
							Z1L1_FG_MAP_RLE_DATA,
							Z1L1_FG_MAP_RLE_OFFS[world_row],
							new_world_col
					);
		} else {
			map_buf[phys_row][write_col] = TILE_EMPTY;
		}
	}
}




static inline void stream_scroll_up(void)
{
	u8 x, write_row, write_col;
	u16 world_row, world_col;

	if (level.map_buf_origin_y == 0)
		return;

	level.map_buf_origin_y--;
	level.map_buf_row0 = (level.map_buf_row0 + MAP_BUF_H - 1) & MAP_BUF_MASK;

	write_row = level.map_buf_row0;
	world_row = level.map_buf_origin_y;

	for (x = 0; x < MAP_BUF_W; x++) {
		world_col = level.map_buf_origin_x + x;
		write_col = (x + level.map_buf_col0) & MAP_BUF_MASK;

		if (world_row < level.map_h && world_col < level.map_w) {
			map_buf[write_row][write_col] =
					decompress_rle_get_value(
							Z1L1_FG_MAP_RLE_DATA,
							Z1L1_FG_MAP_RLE_OFFS[world_row],
							world_col
					);
		} else {
			map_buf[write_row][write_col] = TILE_EMPTY;
		}
	}
}




static inline void stream_scroll_down(void)
{
	u8 x, write_row, write_col;
	u16 world_row, world_col;

	level.map_buf_origin_y++;
	level.map_buf_row0 = (level.map_buf_row0 + 1) & MAP_BUF_MASK;

	write_row = (level.map_buf_row0 + MAP_BUF_H - 1) & MAP_BUF_MASK;
	world_row = level.map_buf_origin_y + MAP_BUF_H - 1;

	for (x = 0; x < MAP_BUF_W; x++) {
		world_col = level.map_buf_origin_x + x;
		write_col = (x + level.map_buf_col0) & MAP_BUF_MASK;

		if (world_row < level.map_h && world_col < level.map_w) {
			map_buf[write_row][write_col] =
					decompress_rle_get_value(
							Z1L1_FG_MAP_RLE_DATA,
							Z1L1_FG_MAP_RLE_OFFS[world_row],
							world_col
					);
		} else {
			map_buf[write_row][write_col] = TILE_EMPTY;
		}
	}
}








#endif /* LEVEL_H */
