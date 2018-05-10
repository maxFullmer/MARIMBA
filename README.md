# Whale_Watchers

This is the breakdown of all code and background nessesary to duplicate what I put together for the senior design project. If you are reading this now, you have done more software than the entirity of my group this year. That being said, I challenge you to make this project what it was always meant to be. I am simply supplying the bare minimum to get you up and running... 

Here is the breakdown of each major componant I will be discussing:

1) /etc/rc.local
  a) Network Card Configuration
2) Shutdown.py
3) DeploymentHandler.py
  a) Mount USB Drive
  b) rtp_send.sh
  c) Transmit.sh
4) DAQ_Sampler

I'll do my best to go as in depth as possible but I do have to assume some level of knowlege about the hardware in use. Consult datasheets and the internet to fill in the blanks. Best to have the code up as well so you can compare syntax to the discriptions.

**rc.local**

The rc.local script is the last thing executed before recieving login shell access. We can leverage this to auto-execute scripts we stash there as the Pi boots. Here we have added 3 programs to launch in order. 

- First it prints the IP address of the device. 
- Then it sleeps for 5 seconds in order to ensure things go through. 
- Then it runs usb_modeswitch in order to tell the network card what it is doing. Further documentation on this can be found in the user manual for the Aircard 602L here:

https://www.verizonwireless.com/dam/support/pdf/user_guide/u620-linux-integration-guide-7-17-15.pdf

- Sleeps again to let the aircard report it is working.
- Then it starts a background code called Shutdown.py. 
- Finally it launches the DeploymentHandler.py script

**Shutdown.sh**

The purpose of Shutdown.py is to tie the pushbutton input on the PCB to the shutdown command on the Pi as to not corrupt the filesystem.

**DeploymentHandler.py**

This is the central hub for all of the data collection. I set it up to act as a really crap daemon. Mostly because I wouldn't have shell access to the devices while they are deployed without a business account with Verizon. Food for thought when it isn't just one person "making things happen".

- First it mounts the physical storage media to a known point in the filesystem for local data backup 
'''
sudo mount /dev/sda1 /media/pi/usb -o uid=pi,gid=pi
'''
- Then it begins the rtp stream to the server's vlc player @ port :1234 by launching rtp_send.sh
'''
ffmpeg -ar 8000 -f alsa -i plughw:1,0 -acodec mp2 -b:a 128k -f rtp rtp://131.128.105.39:1234
'''
Please note that ffmpeg is not a default package, nor is in the apt-get repository. So you must compile from source. Link here :

https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu

Please note that you must compile all dependencies listed

- Then it sets the [Ready] LED on the Pi High so the user can see when the live audio stream is running.

- 
