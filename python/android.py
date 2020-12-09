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
tn_ip = "127.0.0.1"
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
    #print ("We in getspeeds")
    if active is True:
        try:
            gps = \
                os.popen(
                    "tail -1  /data/data/com.termux/files/home/storage/shared/Android/data/com.mendhak.gpslogger/files/20201207.csv |awk -F, '{print $7}'|awk -F. '{print $1}'").read()
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
	    #tn.write(str(gps))
            time.sleep(5)
            pass

        try:
            limited = \
                os.popen(
                    "ps -A|grep -q -i ru.yandex.yandexnavi && (/system/bin/screencap -p /data/data/com.termux/files/home/map.png && (/data/data/com.termux/files/usr/bin/mogrify -crop '125x100+910+140' -set units PixelsPerInch -density 300 -write  /data/data/com.termux/files/home/map_.png  /data/data/com.termux/files/home/map.png && (/data/data/com.termux/files/usr/bin/tesseract --oem 0 -c tessedit_char_whitelist=0123456789  /data/data/com.termux/files/home/map_.png /data/data/com.termux/files/home/map1 > /dev/null 2>&1 &&head  -1 /data/data/com.termux/files/home/map1.txt)) )"
                ).read()
            print ("We req limit")
#            print (str(limited))
            limited = limited.replace("\r", "")
            limited = limited.replace("\n", "")
            print (limited)
	    if not (limited==int(limited)):
                limited = 0
                print (str(limited))
        except:
            limited = 0
            pass
#    if active == False:
#        print("Active if off")

def check():
    global gps
    global limited
#    global diff
    getspeeds()
    #tn.write(str(gps))
    #tn.write(str(limited))
    number = 1
#    if (gps == 0) or (limited == 0):
#        while (number <= 3):
#            print("Retring get speed limits" + str(number))
#            getspeeds()
#            number = number + 1
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
#        time.sleep(5)
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
#        time.sleep(5)
        senddataup(int(diff))
#    else:
#        diff = 0
#        print (int(diff))
#        print ("we in else")
#        time.sleep(5)


def connect():
    global tn
    global lol
    global connected
    while not connected:
        try:
            tn = telnetlib.Telnet(tn_ip, tn_port, 15)
            connected = True
            print ("Connected")
#            run()
            if (lol <1):
                print ("Starting thread")
            	th = threading.Thread(target=run)
            	th.start()
                lol = lol + 1
            if th.is_alive() is False:
                th = threading.Thread(target=run)
                th.start()
#
#            check()
            checkme()
        except IOError:
            print ("Reconnecting")
            time.sleep (2)

def checkme():
    while True:
        #tn.write('\r\ncheckme\r\n')
        check()

  
def run():
    global tn
    global connected
    global gps
    global limited
    global active
    print ("We are in main loop")
#    th.join()
    try:
        while True:
            data=''
            while data.find('#'):  # == -1:
                data = tn.read_very_eager()
#                tn.write('ftest\r\n')
#                check()
            #tn.write(data + ' \r\n')
            if(isactive in data):
                print ("We found " + isactive + "\r\n")
                tn.write("SWITCHED\n")
                tn.write ("\r\nACTIVE IS " + str(active) + "\r\n")
                if active == True:
                    active = False
                    tn.write ("\r\nACTIVE AFTER " + str(active) + "\r\n")
                elif active == False:
                    active = True
                    tn.write ("\r\nACTIVE AFTER " + str(active) + "\r\n")
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
                #tn.write("Try again" + "\n")
    except IOError:
        print ("Connection Lost")
        connected = False
#        connect()




connect()
###    gps = (int(100))  # debug
###    limited = (int(80))  # debug
#    tn.write("gps is " + str(gps) + ' \r\n')
#    tn.write("limited is " + str(limited) + ' \r\n')
#    time.sleep (5)
