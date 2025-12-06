#!/bin/bash

echo "=== Conversione immagini PNG in BMP con palette personalizzata e ridimensionamento a 16x16 ==="
echo "Paletta: 16 colori specificati"

# Crea la palette dai tuoi colori
echo "Creazione palette..."
convert -size 1x16 xc:"#97bceb" xc:"#000000" xc:"#451001" xc:"#004402" \
                  xc:"#135397" xc:"#ac4200" xc:"#5354ff" xc:"#dc4453" \
                  xc:"#358833" xc:"#37abff" xc:"#11ee14" xc:"#ffaa00" \
                  xc:"#febb73" xc:"#ffbaa7" xc:"#daff00" xc:"#ffffff" \
        +append palette_custom.png

echo "Palette creata: palette_custom.png"

# Converti tutte le PNG in BMP usando la palette e ridimensiona a 16x16
echo -e "\nConversione PNG -> BMP 16x16 con palette..."
for f in *.png; do
    if [ "$f" = "palette_custom.png" ]; then
        continue
    fi
    
    if [ -f "$f" ]; then
        basename="${f%.*}"
        # Converti usando la palette e ridimensiona a 16x16
        convert "$f" -resize 16x16 -remap palette_custom.png -type palette -depth 4 "${basename}.bmp"
        echo "  Convertito e ridimensionato: $f -> ${basename}.bmp (16x16)"
    fi
done

# Rinomina i file BMP numerici
echo -e "\nRinomina file BMP..."
count=0
for f in [0-9]*.bmp; do
    if [ -f "$f" ]; then
        # Estrai il numero
        num="${f%.bmp}"
        
        # Formatta con 3 cifre
        if [[ "$num" =~ ^[0-9]+$ ]]; then
            newnum=$(printf "%03d" "$num")
            mv "$f" "bck_${newnum}.bmp"
            echo "  Rinominato: $f -> bck_${newnum}.bmp"
            ((count++))
        fi
    fi
done

echo -e "\n=== Operazione completata ==="
echo "- Palette creata: palette_custom.png"
echo "- File BMP 16x16 rinominati: $count"
echo "- Formato finale: bck_000.bmp, bck_001.bmp, ..."

# Opzionale: mostra i primi 5 file per verifica
echo -e "\nPrimi 5 file BMP:"
ls -1 bck_*.bmp 2>/dev/null | head -5
