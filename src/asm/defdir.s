;
; Karri Kaksonen, 2011
;
; A default directory with just the main executable.
;
        .include "lynx.inc"
        .import         __STARTOFDIRECTORY__
        .import         __MAIN_START__
        .import         __STARTUP_LOAD__, __BSS_LOAD__
        .import         __BANK0BLOCKSIZE__
        .import		__VIDEO__
        .import		__STACKSIZE__
        .import		__MAPS_SIZE__
        .import		__MAPS_START__
        .export         __DEFDIR__: absolute = 1


; ------------------------------------------------------------------------
; Lynx directory
        .segment "DIRECTORY"

__DIRECTORY_START__:

.macro entry old_off, old_len, new_off, new_block, new_len, new_size, new_addr
new_off=old_off+old_len
new_block=new_off/__BANK0BLOCKSIZE__
new_len=new_size
	.byte	<new_block
	.word	(new_off & (__BANK0BLOCKSIZE__ - 1))
	.byte	$88
	.word	new_addr
	.word	new_len
.endmacro




off0 = __STARTOFDIRECTORY__ + (__DIRECTORY_END__ - __DIRECTORY_START__)
blocka = off0 / __BANK0BLOCKSIZE__
; Entry 0 - first executable
block0 = off0 / __BANK0BLOCKSIZE__
len0 = __BSS_LOAD__ - __STARTUP_LOAD__

        .byte   <block0
        .word   off0 & (__BANK0BLOCKSIZE__ - 1)
        .byte   $88
        .word   __MAIN_START__
        .word   len0



entry off0, len0, off1, block1, len1,__MAPS_SIZE__, __MAPS_START__



__DIRECTORY_END__:
