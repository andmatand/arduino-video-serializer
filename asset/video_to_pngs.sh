if [ $# -eq 0 ]
then
    echo "Specify a video (preferably .mp4) filename as the first argument"
    exit 1
fi

mkdir -p frame
rm -f frame/*.png

ffmpeg -i "$1" -filter 'fps=28.125, scale=32:16, crop=32:16' -start_number 0 frame/%03d.png
