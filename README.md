
Adaptive cruise control with mapcamifo
# Testing DIY Toyota adaptive rav4 cruise control with nodemcu

Project based on capturing part of warnings,OCR - it , and control over nodemcu + android rooted phone with termux and , for 



```Cruise control switch circuit.jpg```  Rav4 schematic cruise control regulator
```termux.apk``` Termux installer for android
```com.termux.api-20201222-204040.TiBkp``` Titanium Backup of termux API
```resistor.jpg```  How to connect cruise control regulator to nodemcu/arduino, does not matter
```schematic.jpg``` principial schematic of rav4 - (2004 aca20) -testing - cruise control wiring
```tester.ino```  - arduino/nodemcu test project to get real values for cruise control regulator(different resistance)(with resistor 2kOm) 
(unknown resistor is a "cruise control regulator" ,from the car)
```transistor1.jpg```  - principial wiring for chip to make cruise control
```org.prowl.torque-20201222-200422.TiBkp``` Titanium backup of Torque - for speed receiving from car
```obd2-simulator.zip``` OBD2 Simulator for PC+bluetooth
Pair your android device with PC over bluetooth
Check which bluetooth com-port are using, for me is COM16..
run:obdsim.exe -w COM16
Start torque application and connect to bluetooth elm , which is your pc.
Play with "speed", in torque speed must changes!


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



|#######################________| Processing of project.....
