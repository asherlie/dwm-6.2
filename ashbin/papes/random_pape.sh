#!/bin/bash
export DISPLAY=:0
p=$(ls /home/asher/wallpaper | shuf -n 1)
echo "/home/asher/wallpaper/$p" > /home/asher/.bg_set
#feh --bg-scale "/home/asher/wallpaper/$p"
#feh --bg-max "/home/asher/wallpaper/$p"
feh --bg-fill "/home/asher/wallpaper/$p"
echo "set wallpaper $p at $(date)\nrunning 'feh --bg-scale /home/asher/wallpaper/$p'" >> /home/asher/FUFU
