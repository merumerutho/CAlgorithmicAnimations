#!/bin/bash
gcc main.c -lm -o main
echo "Generating pictures..."
mkdir $HOME/Frames &> /dev/null;
parallel ./main {} "$1" :::: <(seq 0 "$1")
cd $HOME/Frames;
echo "Creating video..."
ffmpeg -i %01d.ppm -framerate 60 -pattern_type sequence -y out.mp4 &> /dev/null
rm *.ppm >& /dev/null
echo "Creating palette..."
ffmpeg -i out.mp4 -vf "fps=30,scale=160:-1:flags=lanczos,palettegen=stats_mode=diff" -y 'palette.png' >& /dev/null
echo "Creating gif..."
ffmpeg -i out.mp4 -i palette.png -lavfi "fps=30,scale=160:-1:flags=lanczos,paletteuse=dither=bayer:bayer_scale=2:diff_mode=rectangle" -y out.gif &> /dev/null
rm palette.png
