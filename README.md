
Adaptive cruise control with yandex navigator
# Testing DIY Toyota adaptive rav4 cruise control with nodemcu

Project based on capturing part of warnings,OCR - it , and control over nodemcu + android rooted phone with termux 

```
WIRING:
КР293КП4Б (5П14.2Б) * 2 
(analog: PRAB30S: KAQY214S; AQY214S; CPC1025N; CPC1030N; PS7241-1A-A;PS7200A-1A; M211; M221)



(https://www.chipdip.ru/product/kr293kp4b) optoelectronic current 10 mA , then 
390Ohm * 4 into input of КР293КП4Б




378 Ohm
722 Ohm
994 Ohm



```


```FILES\Cruise control switch circuit.jpg```  Rav4 schematic cruise control regulator



```FILES\termux.apk``` Termux installer for android



```FILES\termux_zip\com.termux-20201222-204146.TiBkp.zip``` Titanium Backup of termux, possible with tesseract :)))



```FILES\com.termux.api-20201222-204040.TiBkp``` Titanium Backup of termux API



```FILES\resistor.jpg```  How to connect cruise control regulator to nodemcu/arduino, does not matter



```FILES\schematic.jpg``` principial schematic of rav4 - (2004 aca20) -testing - cruise control wiring



```FILES\tester.ino```  - arduino/nodemcu test project to get real values for cruise control regulator(different resistance)(with resistor 2kOm) 
(unknown resistor is a "cruise control regulator" ,from the car)



```FILES\transistor1.jpg```  - principial wiring for chip to make cruise control



```FILES\org.prowl.torque-20201222-200422.TiBkp``` Titanium backup of Torque - for speed receiving from car




```FILES\obd2-simulator.zip``` OBD2 Simulator for PC+bluetooth



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
IMPORTANT!!!
ANDROID - enable in dev options enable connection to wifi for fast switching

Using python 2.7
```termux -> apt install python2 or pkg install python2```
```termux -> apt install python2-tools```
```pip2 install time```
```pip2 install keyboard```
```pip2 install threading```
```pip2 install re```
```pip2 install import```
```pip2 install time```
```pip2 install os```
```pip2 install sys```
```pip2 install telnetlib```



|###########################____| Processing of project.....
