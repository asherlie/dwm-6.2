#!/bin/bash
# set timezone based on IP and update time
timedatectl set-timezone "$(curl --fail https://ipapi.co/timezone)"
ntpd -qg
sudo hwclock -w
echo "TIMESET!" >> ~/TIME_SET
