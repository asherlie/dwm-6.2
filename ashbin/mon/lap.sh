#!/bin/bash
xrandr --output DP3 --auto --off
xrandr --output LVDS1 --auto --primary
xrandr --output DP2 --auto --off
echo "TURNING OFF MONITOR MODE" >> ~/.mon_hist
