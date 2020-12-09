
Adaptive cruise control with mapcamifo
# Testing DIY Toyota adaptive rav4 cruise control with nodemcu

Project based on capturing part of warnings,OCR - it , and control over nodemcu + android rooted phone with termux and , for 



Cruise control switch circuit.jpg  Rav4 schematic cruise control regulator
resistor.jpg  How to connect cruise control regulator to nodemcu/arduino, does not matter


schematic.jpg principial schematic of rav4 - (2004 aca20) -testing - cruise control wiring

tester.ino  - arduino/nodemcu test project to get real values for cruise control regulator(different resistance)(with resistor 2kOm) 
(unknown resistor is a "cruise control regulator" ,from the car)

transistor1.jpg  - principial wiring for chip to make cruise control

```#install termux on android
install termuxapi on android
inside termux: 
#  install 
apt install tsu
apt install  tesseract
```


```
export termux_bin_path="/data/data/com.termux/files/usr/bin"; export owner="$(stat -c "%u" "$termux_bin_path")"; for f in tesseract; do if [ -f "$termux_bin_path/$f" ]; then su -c "chown $owner:$owner \"$termux_bin_path/$f\" && chmod 700 \"$termux_bin_path/$f\""; fi; done;
```


##  For current speed using : gplogger (added)
##  It is possibble to read speed from elm327 by bluetooth


|#######################________| Processing of project.....
