#!/bin/bash

device="/dev/video1"

#white balance
v4l2-ctl -d $device --set-ctrl=white_balance_temperature_auto=0
v4l2-ctl -d $device --set-ctrl=white_balance_temperature=3700

#exposure (auto)
v4l2-ctl -d $device --set-ctrl=exposure_auto=3
