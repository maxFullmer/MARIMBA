import RPi.GPIO as GPIO
import time
import os

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(18, GPIO.OUT)
GPIO.output(18, 0)


source = "/home/pi/Documents/Data/"
drive = "/media/pi/usb/1/"
d_time = 0

def transmit():
	d_time = time.time()
	GPIO.output(18, 1)
	destination = "/home/pi/Documents/Data/%s.txt" % d_time
	os.chdir(source)
       	os.rename("Data.txt", destination)
	zip = "sudo bzip2 %s" % destination
	os.system(zip)
	print("File compressed")
	cp = "sudo cp %s.txt.bz2 /media/pi/usb/1/" % d_time
	os.system(cp)
	print("Saved to USB storage device")
	scp = "sudo sshpass -p '******' scp /home/pi/Documents/Data/%s.txt.bz2 whale-srv@******:~/Whale_Srv/Incoming/1/" % d_time
	os.system(scp)
	GPIO.output(18, 0)
	print("Done! %s" % d_time)

def threshold():
    y = scipy.fftpack.fft():
    for i in y:
        if i > 5000:
            return 'Possible Whale Detection'
            transmit()
        else:
            return 'No Whale Detected'
            d_time = time.time()
            GPIO.output(18, 1)
            destination = "/home/pi/Documents/Data/%s.txt" % d_time
            os.chdir(source)
                os.rename("Data.txt", destination)
            zip = "sudo bzip2 %s" % destination
            os.system(zip)
            print("File compressed")
            cp = "sudo cp %s.txt.bz2 /media/pi/usb/1/" % d_time
            os.system(cp)
            print("Saved to USB storage device")

threshold()
