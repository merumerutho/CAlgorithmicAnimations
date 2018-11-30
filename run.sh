#!/bin/bash

WILL_ABORT=FALSE;

if hash gcc 2>/dev/null; then
	echo "gcc.. OK";
else
	echo "gcc.. Not Present";
	WILL_ABORT=TRUE;
fi

if hash parallel 2>/dev/null; then
	echo "parallel.. OK";
else
	echo "parallel.. Not Present";
	WILL_ABORT=TRUE;
fi

if hash ffmpeg 2>/dev/null; then
	echo "ffmpeg.. OK";
else
	echo "ffmpeg.. Not Present";
	WILL_ABORT=TRUE;
fi


if [ "$WILL_ABORT" = TRUE ] ; then
    echo -e "\t\tPlease install missing stuff and run again!"
	exit 0
fi

gcc main.c -lm -o CAlgorithmicAnimations
echo "Generating pictures..."

if [ -n "$1" ]; then
	parallel ./CAlgorithmicAnimations {} "$1" :::: <(seq 0 "$1")
else
	echo "N' of frames not supplied, i will use 240"
	parallel ./CAlgorithmicAnimations {} 240 :::: <(seq 0 240)
fi
mkdir Frames &> /dev/null;
cd Frames;
echo "Creating video..."
ffmpeg -i %01d.ppm  -pattern_type sequence -r 60 -y out.mp4 &> /dev/null
rm *.ppm >& /dev/null
echo "Creating palette..."
ffmpeg -i out.mp4 -vf "fps=30,scale=160:-1:flags=lanczos,palettegen=stats_mode=diff" -y 'palette.png' >& /dev/null
echo "Creating gif..."
ffmpeg -i out.mp4 -i palette.png -lavfi "fps=30,scale=160:-1:flags=lanczos,paletteuse=dither=bayer:bayer_scale=2:diff_mode=rectangle" -y out.gif &> /dev/null
echo "creating webm... (this can take some time, you can CTRL+C twice to stop if you don't need the webm file)"
ffmpeg -i out.mp4 -c:v libvpx-vp9 -b:v 0 -y output.webm  &> /dev/null

rm palette.png
