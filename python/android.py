import time
from threading import Thread
import threading
from re import search
from time import sleep
import os
import sys
import telnetlib
th = ""
lol = 0
tn_ip = "192.168.4.1"
tn_port = "23"
data = ""
isactive = "#ARRIVES#"
up = "#UPOK#"
down = "#DOWNOK#"
cancel = "#CANCELOK#"
onoff = "#ONOFFOK#"
connected = False
active = True
diff = "0"
slow = ""
limited = ""
gps = ""


def senddatadown(diff):
    number = 1
    d1 = "DOWN\r\n"
    while (number <= diff):
        tn.write(d1.encode())
        time.sleep(0.4)
        print (d1)
        number = number + 1
        print ("We just down")


def senddataup(diff):
    number = 1
    d1 = "UP\r\n"
    print ("senddataup")
    while (number <= diff):
        tn.write(d1.encode())
        time.sleep(0.4)
        print (d1)
        number = number + 1
        print ("We just down")


def getspeeds():
    global active
    global gps
    global limited
    if active is True:
        try:
            gps = \
                os.popen(
            #        "tail -1  /data/data/com.termux/files/home/storage/shared/Android/data/com.mendhak.gpslogger/files/gpslogger.csv |awk -F, '{print $7}'|awk -F. '{print $1}'").read()
                     "ps -A|grep -q -i com.mendhak.gpslogger || am start --user 0 -n com.mendhak.gpslogger/com.mendhak.gpslogger.GpsMainActivity && tail -1  /data/data/com.termux/files/home/storage/shared/Android/data/com.mendhak.gpslogger/files/gpslogger.csv |awk -F, '{print $7}'|awk -F. '{print $1}'").read()
            # run gps "am start --user 0 -n com.mendhak.gpslogger/com.mendhak.gpslogger.GpsMainActivity"
            # "ps -A|grep -q -i com.mendhak.gpslogger || am start --user 0 -n com.mendhak.gpslogger/com.mendhak.gpslogger.GpsMainActivity && tail -1  /data/data/com.termux/files/home/storage/shared/Android/data/com.mendhak.gpslogger/files/gpslogger.csv |awk -F, '{print $7}'|awk -F. '{print $1}'").read()
            print ("We req gps")
            gps = gps.replace("\r", "")
            gps = gps.replace("\n", "")
            #print (gps)
	    if not (gps==int(gps)):
                gps = 0
	    else:
                print(str(gps))
        except:
            gps = 0
            time.sleep(5)
            pass

        try:
            limited = \
                os.popen(
                   # "ps -A|grep -q -i ru.yandex.yandexnavi && (/system/bin/screencap -p /data/data/com.termux/files/home/map.png && (/data/data/com.termux/files/usr/bin/mogrify -crop '125x100+910+140' -set units PixelsPerInch -density 300 -write  /data/data/com.termux/files/home/map_.png  /data/data/com.termux/files/home/map.png && (/data/data/com.termux/files/usr/bin/tesseract --oem 0 -c tessedit_char_whitelist=0123456789  /data/data/com.termux/files/home/map_.png /data/data/com.termux/files/home/map1 > /dev/null 2>&1 &&head  -1 /data/data/com.termux/files/home/map1.txt)) )"
                   #Yandex speed limits
                    "ps -A|grep -q -i info.mapcam.droid && (/system/bin/screencap -p /data/data/com.termux/files/home/map.png && (/data/data/com.termux/files/usr/bin/mogrify -crop '200x140+340+700' -set units PixelsPerInch -density 300 -write  /data/data/com.termux/files/home/map_.png  /data/data/com.termux/files/home/map.png && (/data/data/com.termux/files/usr/bin/tesseract --oem 0 -c tessedit_char_whitelist=0123456789  /data/data/com.termux/files/home/map_.png /data/data/com.termux/files/home/map1 > /dev/null 2>&1 &&head  -1 /data/data/com.termux/files/home/map1.txt)) )"
		   #Mapcaminfo speed limits
                ).read()
            print ("We req limit")
            limited = limited.replace("\r", "")
            limited = limited.replace("\n", "")
            print (limited)
	    if not (limited==int(limited)):
                limited = 0
                print (str(limited))
        except:
            limited = 0
            pass

def check():
    global gps
    global limited
    getspeeds()
    number = 1
    if (gps == 0) or (limited == 0):
        return None
    if (int(limited) < int(gps)):
        gps = (int(gps))
        limited = (int(limited))
        if (gps - limited) == 1:
            diff = ((gps - limited))
        else:
            diff = ((gps - limited) / 2)
        print (int(diff))
        print ("we in limited lower gps")
        senddatadown(int(diff))
    elif (int(limited) > int(gps)):
        gps = (int(gps))
        limited = (int(limited))
        if (gps - limited) == 1:
            diff = ((gps - limited))
        else:
            diff = ((gps - limited) / 2)
        print (int(diff))
        print ("we in limited more gps")
        senddataup(int(diff))


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
    print ("We are in main loop")
    try:
        while True:
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
                    print ("\r\nACTIVE AFTER " + str(active) + "\r\n")
            elif (up in data):
                print ("We found " + up)
            elif (down in data):
                print ("We found " + down)
            elif (cancel in data):
                print ("We found " + cancel)
            elif (onoff in data):
                print ("We found " + onoff)
            else:
                print ("Try again")
    except IOError:
        print ("Connection Lost")
        connected = False


connect()
