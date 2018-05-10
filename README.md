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

I'll do my best to go as in depth as possible but I do have to assume some level of knowlege about the hardware in use. Consult datasheets and the internet to fill in the blanks.

**rc.local**
- The rc.local script is the last thing executed before recieving login shell access. We can leverage this to auto-execute scripts we stash there as the Pi boots. Here we have added 3 programs to launch in order. 
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
