import RPi.GPIO as GPIO
import time
import os

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(16, GPIO.OUT)
GPIO.output(16, 0)

source = "/home/pi/Documents/Data/"
drive = "/media/pi/usb/1/"
d_time = 0

time.sleep(30)

## Mount USB Drive
os.system("sudo mount /dev/sda1 /media/pi/usb -o uid=pi,gid=pi")

## Begin audio stream
os.system("/home/pi/Desktop/rtp_send.sh")

GPIO.output(16, 1)

def sample():
	d_time = time.time()
	print("Running Sample...")
	os.system("sudo /home/pi/Desktop/DAQ_Sampler/USB_Final")
	os.system("sudo /home/pi/Desktop/Transmit.sh")

while True:
	sample()
	time.sleep(.1)
