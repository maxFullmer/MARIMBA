import RPi.GPIO as GPIO
import time
import os

GPIO.setmode(GPIO.BOARD)
GPIO.setup(12, GPIO.IN, pull_up_down=GPIO.PUD_UP)

while True:
	button = GPIO.input(12)
	if (button == False):
		os.system("sudo shutdown now")
	time.sleep(.2)
