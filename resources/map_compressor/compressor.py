import re
import sys
from pathlib import Path

U8_TYPES = ("u8", "uint8_t")


# ---------- RLE helpers ----------

def rle_encode(seq):
    out = []
    prev = seq[0]
    cnt = 1

    for v in seq[1:]:
        if v == prev and cnt < 255:
            cnt += 1
        else:
            out.append((prev, cnt))
            prev = v
            cnt = 1

    out.append((prev, cnt))
    return out


# ---------- C parser ----------

def extract_u8_arrays(text):
    pattern = re.compile(
        rf'const\s+({"|".join(U8_TYPES)})\s+(\w+)\s*\[[^\]]+\]\s*\[[^\]]+\]\s*=\s*\{{',
        re.MULTILINE
    )

    arrays = []

    for m in pattern.finditer(text):
        name = m.group(2)
        start = m.end()

        brace = 1
        i = start
        while i < len(text) and brace > 0:
            if text[i] == '{':
                brace += 1
            elif text[i] == '}':
                brace -= 1
            i += 1

        body = text[start:i - 1]
        rows_raw = re.findall(r'\{([^{}]*)\}', body)

        matrix = []
        for row in rows_raw:
            values = [int(v.strip(), 0) for v in row.split(',') if v.strip()]
            matrix.append(values)

        if matrix:
            arrays.append((name, len(matrix), len(matrix[0]), matrix))

    return arrays


# ---------- Compression ----------

def compress_columns(rows, cols, matrix):
    rle = []
    for c in range(cols):
        col = [matrix[r][c] for r in range(rows)]
        rle.append(rle_encode(col))
    return rle


def compress_rows(rows, cols, matrix):
    rle = []
    for r in range(rows):
        rle.append(rle_encode(matrix[r]))
    return rle


def flatten_rle(rle_list):
    offs = []
    data = []
    offset = 0

    for rle in rle_list:
        offs.append(offset)
        for v, cnt in rle:
            data.append(v)
            data.append(cnt)
            offset += 2

    return offs, data


# ---------- C generation ----------

def generate_c(arrays):
    out = []
    out.append("// FILE GENERATO AUTOMATICAMENTE\n")
    out.append("// RLE colonne + righe\n")
    out.append("// Target: Atari Lynx (65SC02)\n\n")
    out.append("#include <stdint.h>\n\n")

    for name, rows, cols, matrix in arrays:
        col_rle = compress_columns(rows, cols, matrix)
        row_rle = compress_rows(rows, cols, matrix)

        col_offs, col_data = flatten_rle(col_rle)
        row_offs, row_data = flatten_rle(row_rle)

        orig_size = rows * cols

        out.append(f"// ===== {name} =====\n")
        out.append(f"// Size originale : {orig_size} byte\n")
        out.append(f"// Col RLE size   : {len(col_data)} byte\n")
        out.append(f"// Row RLE size   : {len(row_data)} byte\n\n")

        # ---- COLUMN ----
        out.append(f"const uint16_t {name}_RLE_COL_OFFS[{cols}] = {{\n    ")
        out.append(", ".join(map(str, col_offs)))
        out.append("\n};\n\n")

        out.append(f"const uint8_t {name}_RLE_COL_DATA[] = {{\n    ")
        for i, b in enumerate(col_data):
            out.append(f"{b}, ")
            if (i + 1) % 16 == 0:
                out.append("\n    ")
        out.append("\n};\n\n")

        # ---- ROW ----
        out.append(f"const uint16_t {name}_RLE_ROW_OFFS[{rows}] = {{\n    ")
        out.append(", ".join(map(str, row_offs)))
        out.append("\n};\n\n")

        out.append(f"const uint8_t {name}_RLE_ROW_DATA[] = {{\n    ")
        for i, b in enumerate(row_data):
            out.append(f"{b}, ")
            if (i + 1) % 16 == 0:
                out.append("\n    ")
        out.append("\n};\n\n")

    return "".join(out)


# ---------- main ----------

def main():
    if len(sys.argv) != 3:
        print("Uso: python rle_maps.py input.c output.c")
        sys.exit(1)

    text = Path(sys.argv[1]).read_text()
    arrays = extract_u8_arrays(text)

    if not arrays:
        print("Nessun array u8 bidimensionale trovato.")
        return

    Path(sys.argv[2]).write_text(generate_c(arrays))
    print(f"Generate {len(arrays)} mappe RLE.")


if __name__ == "__main__":
    main()

