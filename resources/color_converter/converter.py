def html_to_gbr(input_file, output_file):
    with open(input_file, "r") as fin, open(output_file, "w") as fout:
        for line in fin:
            line = line.strip()
            if not line or not line.startswith("#"):
                continue

            # Estrai i componenti RGB (8 bit)
            r = int(line[1:3], 16)
            g = int(line[3:5], 16)
            b = int(line[5:7], 16)

            # Riduzione a 4 bit per canale
            r4 = r >> 4
            g4 = g >> 4
            b4 = b >> 4

            # Composizione GBR: G(11–8), B(7–4), R(3–0)
            gbr = (g4 << 8) | (b4 << 4) | r4

            fout.write(f"0x{gbr:04X};\n")


# Esempio di utilizzo
html_to_gbr("palette_html.txt", "palette_gbr.txt")

