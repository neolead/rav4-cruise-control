import time
import keyboard
from threading import Thread
import threading
import re
from re import search
from time import sleep
import os
import sys
import telnetlib
th = ""
overspeed = 10#Speed which can be overvalued
lol = 0
iter = 0
restart = 0
lastdate = ""
lastdatex = ""
tn_ip = "192.168.4.1"
tn_port = "23"
data = ""
isactive = "#ARRIVES#"
up = "#UPOK#"
down = "#DOWNOK#"
cancel = "#CANCELOK#"
onoff = "#ONOFFOK#"
selfup = "#SELF_U#"
selfdown = "#SELF_D#"
selfcancel = "#SELF_C#"
selfonoff = "#SELF_SW#"

connected = False
active = False
diff = "0"
slow = ""
limited = ""
gps = ""
dosleep = False
s = 0

def senddatadown(diff):
    number = 1
    global dosleep
    d1 = "DOWN\r\n"
    while (number <= diff):
        tn.write(d1.encode())
        time.sleep(0.2)
        print (d1)
        number = number + 1
        print ("We just speed down")
    print ("We just speed down and SLEEP")
    #time.sleep (5)
    dosleep = True


def senddataup(diff):
    global dosleep
    number = 0
    dx = "DOWN\r\n"
    d1 = "UP\r\n"
    print ("senddataup")
    tn.write(dx.encode())
    time.sleep(0.2)
    while (number <= diff):
        tn.write(d1.encode())
        time.sleep(0.2)
        print (d1)
        number = number + 1
        print ("We just speed up")
    print ("We just speed UP and SLEEP")
    #time.sleep (5)
    dosleep = True


def getspeeds():
    global active
    global gps
    global limited
    global lastdate
    global lastdatex
    global iter
    global dosleep
    global restart
    if active is False and (restart >0 and restart < 3):
        try:
            os.popen("killall -9 ru.yandex.yandexnavi").read
            time.sleep(1)
            os.popen("am start -W --activity-brought-to-front --user 0 -n ru.yandex.yandexnavi/ru.yandex.yandexnavi.core.NavigatorActivity").read()
            time.sleep(5)
            os.popen("killall -9 org.prowl.torque").read
            time.sleep(1)
            os.popen("am start -W  --user 0 -n org.prowl.torque/org.prowl.torque.landing.FrontPage").read()
            time.sleep(5)
            restart = restart + 1
            active = True
        except:
            time.sleep(1)
            pass

    if active is True:
        if dosleep is True:
            time.sleep(5)
            dosleep = False
        try:
            gps = \
                os.popen(
                     "ps -A|grep -q -i org.prowl.torque||am start -W --activity-brought-to-front --user 0 -n org.prowl.torque/org.prowl.torque.landing.FrontPage&&cat  /sdcard/torqueLogs/* |tail -1").read()
            print ("We reqire obd2 speed")
            gps1 = re.split(r',', gps)
            lastdate=gps1[0] #date
            spd=gps1[1] #speed
            gps = spd.replace("\r", "")
            gps = gps.replace("\n", "")
            print ("This is obd2 speed")
            print (gps)
            if (lastdate in lastdatex):
                print ("last date of obd is same")
                print (lastdate)
                print (lastdatex)
                if (iter < 20):
                    iter = iter + 1
                    print("iter is lower than 10,iter value is")
                    print(iter)
                    time.sleep(2)
                else:
                    print("iter value is more than 10")
                    print(iter)
                    print("We set active to off")
                    active = False
                    restart = restart + 1
            else:
                print ("last date of obd is different")
                print (lastdate)
                print (lastdatex)
                lastdatex = lastdate
                restart = 0
                iter = 0

            print ("obd speed ")
            print (int(gps))
            if isinstance(int(gps),str):
                gps = 0
                print ("obd speed is not valid")
                print("we set gps 0")
                print (str(gps))
            else:
                if int(gps) >180 or int(gps) < 60:
                    gps = 0
                    print ("limit is not valid,(exceed speed >180 or <60")
                    print("we set gps 0")
                    print (str(gps))
        except:
            gps = 0
            time.sleep(5)
            print("we got exception")
            pass

        try:
            limited = \
                os.popen("if ps -A|grep -q -i ru.yandex.yandexnavi; then if /system/bin/screencap -p /data/data/com.termux/files/home/map.png && (/data/data/com.termux/files/usr/bin/mogrify -crop '125x100+910+140' -set units PixelsPerInch -density 300 -write  /data/data/com.termux/files/home/map_.png  /data/data/com.termux/files/home/map.png && convert /data/data/com.termux/files/home/map_.png -define histogram:unique-colors=true -format %c histogram:info:-|grep FF5858 -i -q ); then if [ $(convert /data/data/com.termux/files/home/map_.png -define histogram:unique-colors=true -format %c histogram:info:-|grep FF5858 |tr -d ' '|cut -f1 -d ':' ) -ge 500 ]; then /data/data/com.termux/files/usr/bin/tesseract --oem 0 -c tessedit_char_whitelist=0123456789  /data/data/com.termux/files/home/map_.png /data/data/com.termux/files/home/map1 > /dev/null 2>&1 && head  -1 /data/data/com.termux/files/home/map1.txt; 	else  echo 0;	fi else  echo 0;  fi fi ").read()
            print ("We reqire limit")
            limited = limited.replace("\r", "")
            limited = limited.replace("\n", "")
            print ("This is limit")
            print (limited)
            if isinstance(int(limited),str):
                limited = 0
                print ("limit is not valid")
                print (str(limited))
            else:
                if int(limited) >180 or int(limited) < 60:
                    limited = 0
                    print ("limit is not valid,(exceed speed >180 or <60)")
                    print (str(limited))
        except:
            limited = 0
            pass

def check():
    global overspeed
    global gps
    global limited
    getspeeds()
    number = 1
#//    if (gps or gps==0) or (limited or limited==0) :
    if gps==0 or limited==0 :
#        print("gps or limited is 0")
####    if (gps or gps==0) :
#    if (gps == 0) or (limited == 0):
        return None
####################
    limited = int(limited) + int(overspeed)
    print ("This is limit with overspeed")
    print (limited)

    if (int(limited) < int(gps)):
##        gps = (int(gps) )
        gps = (int(gps))
        limited = (int(limited))
        if (gps - limited) == 1:
            diff = ((gps - limited))
        else:
            if ((gps - limited) % 2 != 0):
                print ("not devided")
                diff = ((((gps - limited) + 1) / 2) * 2)
                print (diff)
            else:
                diff = (((gps - limited) / 2 )* 2 )
        print ("This is diff", int(diff))
#        print (int(diff))
        print ("We in limited lower gps")
        senddatadown(int(diff))
    elif (int(limited) > (int(gps)+5)):
        gps = (int(gps))
        limited = (int(limited))
        if (gps - limited) == 1:
            diff = ((gps - limited))
        else:
            if ((gps - limited) % 2 != 0):
                print ("not devided")
                diff = ((((gps - limited) + 1) / 2) * 2 )
                print (abs(diff))
            else:
                diff = (((gps - limited) / 2) * 2 )
#        print (abs(int(diff)))
        print ("we in limited more gps", abs(int(diff)))
        senddataup(abs(int(diff)))


def connect():
    global tn
    global lol
    global connected
    while not connected:
        try:
            tn = telnetlib.Telnet(tn_ip, tn_port, 15)
            connected = True
            print ("Connected")
            if (lol <1):
                print ("Starting thread")
                th = threading.Thread(target=run)
                th.start()
                lol = lol + 1
            if th.is_alive() is False:
                th = threading.Thread(target=run)
                th.start()
            checkme()
        except IOError:
            print ("Reconnecting")
            time.sleep (2)

def checkme():
    while True:
        check()

  
def run():
    global tn
    global connected
    global gps
    global limited
    global active
    global iter
    print ("We are in main loop")
    try:
        while True:
            try:  
                if keyboard.is_pressed('q'):
                    print('Exit!')
                    break 
            except:
                    pass    
            data=''
            while data.find('#'):  # == -1:
                data = tn.read_very_eager()
            if(isactive in data):
                print ("We found " + isactive + "\r\n")
                print("SWITCHED\n")
                print ("\r\nACTIVE IS " + str(active) + "\r\n")
                if active == True:
                    active = False
                    print ("\r\nACTIVE AFTER " + str(active) + "\r\n")
                elif active == False:
                    active = True
                    iter = 0
                    d1 = "DOWN\r\n"
                    tn.write(d1.encode())
                    print ("\r\nACTIVE AFTER " + str(active) + "\r\n")
                    print ("\r\nWe sended activate Cruise control ")
            elif (up in data):
                print ("We found " + up)
            elif (down in data):
                print ("We found " + down)
            elif (cancel in data):
                print ("We found " + cancel)
            elif (onoff in data):
                print ("We found " + onoff)
            elif (selfup in data):
                print ("We found " + selfup)
            elif (selfdown in data):
                print ("We found " + selfdown)
            elif (selfcancel in data):
                print ("We found " + selfcancel)
            elif (selfonoff in data):
                print ("We found " + selfonoff)
            else:
                print ("Try again")
    except IOError:
        print ("Connection Lost")
        connected = False

if s==0:
    gps = 0
    limited = 0
    os.popen("ip rule add prio 100 from all lookup 100").read
    os.popen("ip route add table 100 192.168.4.0/24 dev wlan0").read
    os.popen("ip route add table 100 192.168.1.0/24 dev wlan0").read
    os.popen("killall -9 org.prowl.torque").read    
    time.sleep(3)
    os.popen("am start -W --user 0 -n org.prowl.torque/org.prowl.torque.landing.FrontPage&").read()
    time.sleep(5)
    os.popen("am start --activity-brought-to-front --user 0 -n ru.yandex.yandexnavi/ru.yandex.yandexnavi.core.NavigatorActivity").read()
    time.sleep(5)
    s = 1
    dosleep = True

connect()

