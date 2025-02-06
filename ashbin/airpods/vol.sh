#!/bin/bash
echo Setting volume to $1
for dev in $(./list_airpods.sh); do dbus-send --print-reply --system --dest=org.bluez "$dev" org.freedesktop.DBus.Properties.Set string:org.bluez.MediaTransport1 string:Volume variant:uint16:$1; done
