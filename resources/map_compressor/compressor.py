import re
import sys
from pathlib import Path

U8_TYPES = ("u8", "uint8_t")


def rle_encode_column(col):
    out = []
    prev = col[0]
    count = 1

    for v in col[1:]:
        if v == prev and count < 255:
            count += 1
        else:
            out.append((prev, count))
            prev = v
            count = 1

    out.append((prev, count))
    return out


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

        body = text[start:i-1]
        rows_raw = re.findall(r'\{([^{}]*)\}', body)

        matrix = []
        for row in rows_raw:
            values = [int(v.strip(), 0) for v in row.split(',') if v.strip()]
            matrix.append(values)

        if not matrix:
            continue

        rows = len(matrix)
        cols = len(matrix[0])
        arrays.append((name, rows, cols, matrix))

    return arrays


def compress_array(rows, cols, matrix):
    cols_rle = []
    for c in range(cols):
        col = [matrix[r][c] for r in range(rows)]
        cols_rle.append(rle_encode_column(col))
    return cols_rle


def generate_c(arrays):
    out = []
    out.append("// FILE GENERATO AUTOMATICAMENTE (RLE verticale)\n")
    out.append("// Target: Atari Lynx - 65SC02\n\n")
    out.append("#include <stdint.h>\n\n")

    for name, rows, cols, matrix in arrays:
        cols_rle = compress_array(rows, cols, matrix)

        offs = []
        data = []

        offset = 0
        for col in cols_rle:
            offs.append(offset)
            for v, r in col:
                data.append(v)
                data.append(r)
                offset += 2

        original_size = rows * cols
        compressed_size = len(data)

        out.append(f"// ===== {name} =====\n")
        out.append(f"// Dimensioni: {rows} x {cols}\n")
        out.append(f"// Size originale: {original_size} byte\n")
        out.append(f"// Size compressa: {compressed_size} byte\n")
        out.append(f"// Rapporto: {compressed_size / original_size:.2f}\n")

        if compressed_size > original_size:
            out.append("// NOTE: compressione peggiorativa\n")

        # OFFSETS
        out.append(f"const uint16_t {name}_RLE_OFFS[{cols}] = {{\n    ")
        out.append(", ".join(str(o) for o in offs))
        out.append("\n};\n\n")

        # DATA
        out.append(f"const uint8_t {name}_RLE_DATA[] = {{\n    ")
        for i, b in enumerate(data):
            out.append(f"{b}, ")
            if (i + 1) % 16 == 0:
                out.append("\n    ")
        out.append("\n};\n\n")

    return "".join(out)


def main():
    if len(sys.argv) != 3:
        print("Uso: python rle_maps.py input.c output.c")
        sys.exit(1)

    text = Path(sys.argv[1]).read_text()
    arrays = extract_u8_arrays(text)

    if not arrays:
        print("Nessun array u8 bidimensionale trovato.")
        sys.exit(0)

    Path(sys.argv[2]).write_text(generate_c(arrays))
    print(f"Generati {len(arrays)} array compressi.")


if __name__ == "__main__":
    main()

