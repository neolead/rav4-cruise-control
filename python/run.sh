#!/bin/bash
killall -9 tail
echo Strating Gps Assistant > l
tail -n +1 -f l  | awk -F# '{system(" termux-toast -s " $1)}'&
sudo python2 gps/gps_y_t_d.py
