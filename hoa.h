#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <err.h>
#include <ctype.h>
#include <errno.h>
#include <usbhid.h>
#include <dev/usb/usb.h>
#include <dev/usb/usbhid.h>
#include <termios.h>
#include <signal.h>
#include <event.h>

#include "denon.h"
#include "mplayer.h"
#include "ultrax.h"
#include "pwrmate.h"

#define ULTRAX1_DEV	"/dev/uhid2"
#define ULTRAX2_DEV	"/dev/uhid3"
#define PWRMATE_DEV	"/dev/uhid4"
#define AVR_DEV		"/dev/ttyU0"

#define PFUNC	printf("%s\n", __FUNCTION__);
