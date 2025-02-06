#!/bin/bash 
bstr=$(acpi -b)
status=$(echo $bstr | cut -d " " -f 3 | sed s/,//)
bat=$(echo $bstr | cut -d " " -f 4 | sed s/%,//)

if [ "$status" == "Discharging" -a $bat -le 12 ]
then
     echo "$(date): $bat% battery and discharging - HIBERNATING!" >> /home/asher/hibernation_log
     systemctl hybrid-sleep -i
     #systemd-run --user systemctl hybrid-sleep
fi
