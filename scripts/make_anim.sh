#!/usr/bin/env bash
# needs perl, imagemagic, ffmpeg
# this takes a long time to run

freeze_time=50
slideback_time=200
maxFrame=$(ls mar1d_recording_* | sort -t _ -k3n | tail -n1)
maxFrame=${maxFrame##*_}
maxFrame=${maxFrame%.png}
width=$(( $maxFrame + 31 ))
doubleWidth=$(( ($width - 15) * 2 ))

smooth_interp=$(cat <<'EOF'
$y0 = $ARGV[0];
$y1 = $ARGV[1];

$x0 = $ARGV[2];
$x1 = $ARGV[3];

sub _interp {
    return ((1 - cos($_[0]*3.141592653589793238462643373)) / 2);
}

sub interp {
    return $y0 + _interp(($_[0] - $x0) / ($x1 - $x0)) * ($y1 - $y0);
}

print int(interp($ARGV[4]));
EOF
)

rm -rf anim
mkdir anim

for i in $(seq 0 $maxFrame); do
    in="mar1d_recording_$i.png"
    out="anim/$i.png"
    convert \
        $in \
        \( -background black -gravity east -extent $width \) \
        \( -background black -gravity west -extent $doubleWidth \) \
        $out
done

for i in $(seq 1 $freeze_time); do
    in="anim/$maxFrame.png"
    out="anim/$(( $maxFrame + $i )).png"
    cp $in $out
done

for i in $(seq 1 $slideback_time); do
    pos=$(perl -e "$smooth_interp" 0 $(( ($width - 30) / 2 )) 0 $slideback_time $i)
    in="anim/$maxFrame.png"
    out="anim/$(( $maxFrame + $freeze_time + $i )).png"
    convert \
        $in \
        \( -background black -gravity west -extent $(( $doubleWidth - $pos )) \) \
        \( -background black -gravity east -extent $doubleWidth \) \
        $out
done

for i in $(seq 1 $freeze_time); do
    in="anim/$(( $maxFrame + $freeze_time + $slideback_time )).png"
    out="anim/$(( $maxFrame + $freeze_time  + $slideback_time + $i )).png"
    cp $in $out
done

for f in anim/*; do
    convert \
        $f \
        -background black -gravity center -extent $(( $width / 2 * 2 )) \
        $f
done

ffmpeg -framerate 60 -i anim/%d.png -c:v libx264 -profile:v high -crf 20 -pix_fmt yuv420p anim.mp4
