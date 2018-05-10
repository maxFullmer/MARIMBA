# Whale_Watchers

This is the breakdown of all code and background nessesary to duplicate what I put together for the senior design project. If you are reading this now, you have done more software than the entirity of my group this year. That being said, I challenge you to make this project what it was always meant to be. I am simply supplying the bare minimum to get you up and running... 

Here is the breakdown of each major componant I will be discussing:

1) /etc/rc.local
a) Network Card Configuration
2) Shutdown.py
3) DeploymentHandler.py
a) Mount USB Drive
b) rtp_send.sh
4) DAQ_Sampler
5) Transmit.sh

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

`sudo shutdown now`

**DeploymentHandler.py**

This is the central hub for all of the data collection. I set it up to act as a really crap daemon. Mostly because I wouldn't have shell access to the devices while they are deployed without a business account with Verizon. Food for thought when it isn't just one person "making things happen".

- First it mounts the physical storage media to a known point in the filesystem for local data backup 

`sudo mount /dev/sda1 /media/pi/usb -o uid=pi,gid=pi`

- Then it begins the rtp stream to the server's vlc player @ port :1234 by launching rtp_send.sh

`ffmpeg -ar 8000 -f alsa -i plughw:1,0 -acodec mp2 -b:a 128k -f rtp rtp://131.128.105.39:1234`

Please note that ffmpeg is not a default package, nor is in the apt-get repository. So you must compile from source. Link here :

https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu

Please note that you must compile all dependencies listed.

- Then it sets the [Ready] LED on the Pi High so the user can see when the live audio stream is running.

- Next the DAQ sampling script saves data locally

- Finally the Transmit.sh script is executed upon completion of the DAQ sampling

**DAQ_Sampler**

This driver was writen by using the template provided by MCC hosted on github at:

https://github.com/wjasper/Linux_Drivers

- First the program reads the GPS and appends the data to the header of the data file. Information on libGPS can be found here:

https://github.com/wdalmut/libgps

````//GPS Read
    time_t current_time;
    char* c_time_string;
    current_time = time(NULL);
````
````// Open
    gps_init();
    loc_t gps_data;
    gps_location(&gps_data);
    c_time_string = ctime(&current_time);
````

- Next it samples 600,000 samples at 10 kHz and saves them to a text file.

**Transmit.sh**

This shell script needed to be written to nest the python script controlling the transmission of data in order to have propper permissions nessesary to make changes to the filesystem and send the data.

- First it renames the data file with a Unix-Time string (seconds passed since Jannuary 1st 1970)

- Then it zips the file usign .bz2 for maximum compression

`"sudo bzip2 %s" % destination`

- Next it saves that file to the usb drive connected to the Pi

`"sudo cp %s.txt.bz2 /media/pi/usb/1/" % d_time`

- Finally it uses SCP to send the file to the server (plese note sshpass is used in order to log into the remote server, rsa keys were not functioning properly due to the dynamic ip set by the aircard)

`"sudo sshpass -p 'ramboat' scp /home/pi/Documents/Data/%s.txt.bz2 whale-srv@131.128.105.39:~/Whale_Srv/Incoming/1/" % d_time`
