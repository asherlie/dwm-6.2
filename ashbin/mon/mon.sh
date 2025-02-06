#!/bin/bash
xrandr --output DP3 --auto --primary
xrandr --output LVDS1 --off
# this webcam is only in my upstate workspace
if test -h "/dev/snd/by-id/usb-046d_HD_Pro_Webcam_C920_8B16F02F-02"; then
    xrandr --output DP2 --auto --left-of DP3 --rotate left 
    echo "$(date) TURNING ON *upstate* MONITOR MODE" >> ~/.mon_hist
else
    xrandr --output DP2 --auto --left-of DP3 --rotate right
    echo "$(date) TURNING ON *nyc* MONITOR MODE" >> ~/.mon_hist
fi
