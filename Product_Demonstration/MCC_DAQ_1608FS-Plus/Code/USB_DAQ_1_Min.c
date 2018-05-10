#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include "pmd.h"
#include "usb-1608FS-Plus.h"

#include <gps.h>

#define MAX_COUNT     (0xffff)
#define FALSE 0
#define TRUE 1

/* Test Program */
int toContinue()
{
  int answer;
  answer = 0; //answer = getchar();
  printf("Continue [yY]? ");
  while((answer = getchar()) == '\0' ||
    answer == '\n');
  return ( answer == 'y' || answer == 'Y');
}

int main (int argc, char **argv)
{
  libusb_device_handle *udev = NULL;
  libusb_device_handle *udev2 = NULL;  // second device
  struct tm calDate;

  float table_AIN[NGAINS_USB1608FS_PLUS][NCHAN_USB1608FS_PLUS][2];

//  int ch;
  int i, j, k, m;
//  uint8_t input;
//  int temp;
//  uint8_t options;
//  char serial[9];
  uint8_t /*channel,*/ channels;
//  uint8_t range;
  uint8_t ranges[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//  uint16_t value;
  uint32_t count = 500;
  double frequency = 10000;
  int ret;
  uint16_t sdataIn[8*512]; // holds 16 bit unsigned analog input data
//  uint16_t dataC[256][8];  // corrected data
  uint16_t data1;
  uint16_t data2;
  int nchan = 2;
  int repeats = 200;//, scan;
//  int flag;

//start:
  udev = NULL;
  udev2 = NULL;

  ret = libusb_init(NULL);
  if (ret < 0) {
    perror("usb_device_find_USB_MCC: Failed to initialize libusb");
    exit(1);
  }

  if ((udev = usb_device_find_USB_MCC(USB1608FS_PLUS_PID, NULL))) {
//    printf("Success, found a USB 1608FS-Plus!\n");
  } else {
    printf("Failure, did not find a USB 1608FS-Plus!\n");
    return 0;
  }

//GPS Read
    time_t current_time;
    char* c_time_string;
    current_time = time(NULL);

    // Open
    gps_init();
    loc_t gps_data;
    gps_location(&gps_data);
    c_time_string = ctime(&current_time);

//

  /******************************** Finding a second device has issues on the Raspberry Pi **************/
  // See if there is a second device:
#if defined(LIBUSB_API_VERSION) && (LIBUSB_API_VERSION >= 0x01000103)
  if ((udev2 = usb_device_find_USB_MCC(USB1608FS_PLUS_PID, NULL))) {
    printf("Success, found a second USB 1608FS-Plus!\n");
  } else {
//    printf("Did not find a second device.\n");
  }
#endif

  // some initialization
  //print out the wMaxPacketSize.  Should be 64.
//  printf("wMaxPacketSize = %d\n", usb_get_max_packet_size(udev,0));

  usbBuildGainTable_USB1608FS_Plus(udev, table_AIN);
  for (i = 0; i < NGAINS_USB1608FS_PLUS; i++ ) {
    for (j = 0; j < NCHAN_USB1608FS_PLUS; j++) {
//      printf("Calibration Table: Range = %d Channel = %d Slope = %f   Offset = %f\n", 
//	     i, j, table_AIN[i][j][0], table_AIN[i][j][1]);
    }
  }

  usbCalDate_USB1608FS_Plus(udev, &calDate);
//  printf("\n");
//  printf("MFG Calibration date = %s\n", asctime(&calDate));


//
//	printf("Testing USB-1608FS_Plus Mult-Channel Analog Input Scan.\n");
//        usbAInScanStop_USB1608FS_Plus(udev);
//        printf("enter number of channels (1-8) :");
//        scanf("%d", &nchan);
//        printf("Enter number of scans (less than 512): ");
//        scanf("%d", &count);
//        printf("Enter number of repeats: ");
//        scanf("%d", &repeats);
//        printf("Enter sampling frequency: ");
//        scanf("%lf", &frequency);
        // Build bitmap for the first nchan in channels.
        channels = 0;
        for (i = 0; i < nchan; i++) {
	  channels |= (1 << i);
	}

	  FILE *fp;
          fp=fopen("/home/pi/Desktop/Data.txt","w");  //For prototyping convenience
	  fprintf(fp, "%s %fl %fl\n", c_time_string, gps_data.latitude, gps_data.longitude);
          fprintf(fp, "--------------------------------\n");
          fprintf(fp, "Volts     |  1pps   \n");
          fprintf(fp, "--------------------------------\n");

        //printf ("channels: %02X   count:%d\n", channels, count);
        // Always use BP_10V to make it easy (BP_10V is 0...)
        memset(ranges, 0, sizeof(ranges));
        usbAInScanConfig_USB1608FS_Plus(udev, ranges);
        // Run a loop for the specified number of repeats and
        // show the results...
        for (m = 0; m < repeats; m++) {
//	  printf("\n\n---------------------------------------");
//	  printf("\nrepeat: %d\n", m);
	  usbAInScanStart_USB1608FS_Plus(udev, count, frequency, channels, 0);
	  ret = usbAInScanRead_USB1608FS_Plus(udev, count, nchan, sdataIn, 0);
//	  printf("Number samples read = %d\n", ret/2);
	  if (ret != count * nchan * 2) {
	    printf("***ERROR***  ret = %d   count = %d  nchan = %d\n", ret, count, nchan);
	    continue;
	  } /* if (ret != count * nchan * 2) */
	  for (i = 0; i < count; i++) {
	    //printf("%6d", i);
//	    for (j = 0; j < nchan; j++)	{
	      k = i*nchan + 0;
	      data1 = rint(sdataIn[k]*table_AIN[0][0][0] + table_AIN[0][0][1]);
	      fprintf(fp, "%8.4f", volts_USB1608FS_Plus(data1, 0));
	      fprintf(fp, ", ");
	      k = i*nchan + 1;
	      data2 = rint(sdataIn[k]*table_AIN[0][1][0] + table_AIN[0][1][1]);
              fprintf(fp, "%8.4f", volts_USB1608FS_Plus(data2, 0));
              fprintf(fp, " \n");
//	    } /* for (j - 0; j < 8, j++) */
//	    printf("\n");
	  } /* for (i = 0; i < count; i++) */
	} /* for (m = 0; m < repeats; m++) */
//	printf("\n\n---------------------------------------");
	fclose(fp);
//	system("sudo sshpass -p 'ramboat'  scp /home/pi/Desktop/Data.txt jack@192.168.1.186:~/Desktop/1");
return 0;
}

