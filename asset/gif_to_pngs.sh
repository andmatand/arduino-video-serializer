if [ $# -eq 0 ]
then
    echo "Specify a GIF filename as the first argument"
    exit 1
fi

mkdir -p frame
rm -f frame/*.png

convert \
    -coalesce "$1" \
    -resize 32x16^ \
    -gravity center \
    -extent 32x16+0+0 \
    frame/%03d.png
