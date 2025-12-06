#!/bin/bash
# converti_completo.sh

PALETTE_COLORS=(
    "#97bceb" "#000000" "#451001" "#004402" 
    "#135397" "#ac4200" "#5354ff" "#dc4453" 
    "#358833" "#37abff" "#11ee14" "#ffaa00" 
    "#febb73" "#ffbaa7" "#daff00" "#ffffff"
)

# Crea palette
echo "Creazione palette da ${#PALETTE_COLORS[@]} colori..."
args=()
for color in "${PALETTE_COLORS[@]}"; do
    args+=("xc:\"$color\"")
done

convert_cmd="convert -size 1x16 ${args[@]} +append palette_custom.png"
eval $convert_cmd

# Converti PNG
echo "Conversione in corso..."
for png in *.png; do
    [ "$png" = "palette_custom.png" ] && continue
    [ ! -f "$png" ] && continue
    
    bmp="${png%.png}.bmp"
    convert "$png" -remap palette_custom.png -type palette -depth 4 "$bmp"
    
    # Mostra informazioni sulla conversione
    if [ -f "$bmp" ]; then
        png_size=$(stat -c%s "$png" 2>/dev/null || stat -f%z "$png")
        bmp_size=$(stat -c%s "$bmp" 2>/dev/null || stat -f%z "$bmp")
        ratio=$(echo "scale=1; $bmp_size * 100 / $png_size" | bc)
        echo "✓ $png → $bmp (${ratio}% del originale)"
    fi
done

# Rinomina BMP numerici
echo "Rinomina file..."
renamed=0
for bmp in [0-9]*.bmp; do
    [ ! -f "$bmp" ] && continue
    
    num="${bmp%.bmp}"
    if [[ "$num" =~ ^[0-9]+$ ]]; then
        newname=$(printf "bck_%03d.bmp" "$num")
        
        # Evita sovrascritture accidentali
        if [ -f "$newname" ]; then
            echo "  ATTENZIONE: $newname esiste già! Salto $bmp"
        else
            mv "$bmp" "$newname"
            echo "  → $bmp → $newname"
            ((renamed++))
        fi
    fi
done

echo ""
echo "RIEPILOGO:"
echo "----------"
echo "Palette:    palette_custom.png"
echo "Colori:     16 colori specificati"
echo "Convertiti: $(ls *.png 2>/dev/null | grep -v palette_custom.png | wc -l) file PNG"
echo "Rinominati: $renamed file BMP"
echo "Formato:    bck_XXX.bmp (3 cifre)"
