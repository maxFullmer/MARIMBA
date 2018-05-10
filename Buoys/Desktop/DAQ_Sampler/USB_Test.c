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

int main (int argc, char **argv) {
  libusb_device_handle *udev = NULL;
  libusb_device_handle *udev2 = NULL;  // second device
  struct tm calDate;
  float table_AIN[NGAINS_USB1608FS_PLUS][NCHAN_USB1608FS_PLUS][2];
  int i, j;
  uint8_t options;
  uint8_t ranges[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int channel= 0;
  int range=0;
  long count=5000;
  double frequency=10000;
  int ret;
  uint16_t  sdataIn[8*512]; // holds 16 bit unsigned analog input data
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
    loc_t data;
    gps_location(&data);
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

  usbBuildGainTable_USB1608FS_Plus(udev, table_AIN);
  for (i = 0; i < NGAINS_USB1608FS_PLUS; i++ ) {
    for (j = 0; j < NCHAN_USB1608FS_PLUS; j++) {
    }
  }

  usbCalDate_USB1608FS_Plus(udev, &calDate);
  usbAInScanStop_USB1608FS_Plus(udev);
  usbAInScanClearFIFO_USB1608FS_Plus(udev);
  usbAInScanConfig_USB1608FS_Plus(udev, ranges);

  options = (BLOCK_TRANSFER_MODE | INTERNAL_PACER_ON);
  memset(sdataIn, 0x0, sizeof(sdataIn));
  usbAInScanStart_USB1608FS_Plus(udev, count, frequency, (0x1<<channel), options);
  ret = usbAInScanRead_USB1608FS_Plus(udev, count, 1, sdataIn, options);

  FILE *fp;
  fp=fopen("/home/pi/Desktop/DAQ_test.txt","w");  //For prototyping convenience
  fprintf(fp, "%s %fl %fl\n", c_time_string, data.latitude, data.longitude);  //GPS Group Plz Do
  fprintf(fp, "--------------------------------\n");
  fprintf(fp, "Volts     |  1pps   \n");
  fprintf(fp, "--------------------------------\n");

  for (i = 0; i < count; i++) {
	 sdataIn[i] = rint(sdataIn[i]*table_AIN[range][channel][0] + table_AIN[range][channel][1]);
	 fprintf(fp, "%lf,  ", volts_USB1608FS_Plus(sdataIn[i], range));
	 fprintf(fp, "%lf\n", volts_USB1608FS_Plus(sdataIn[i], range));
	}

  fclose(fp);
  return 0;
}
