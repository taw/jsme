#include <linux/joystick.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>

#define JOYSTICK_DEVICE "/dev/js0"
#define PIPE_OUT "/dev/jsme"
#define MIN_FREQ 20 /* Hz */
#define MAX_DELAY (1000*1000/MIN_FREQ) /* usec */

int main () {
  int dx=0, dy=0;
  int but=0;
  int md, jd;
  fd_set set;
  struct timeval tv;
  int axiss=0,buttons=0;
  struct js_event jev;
  signed char mev[5];

  jd = open (JOYSTICK_DEVICE, O_RDONLY);
  if (jd==-1) {
      perror ("jsme: couldn't open /dev/js0 for reading");
      exit (1);
  }
  md = open (PIPE_OUT, O_WRONLY); /* O_NONBLOCK ??? */
  if (md==-1) {
      perror ("jsme: couldn't open /dev/jsme for writing");
      exit (1);
  }

  do {
      read (jd, &jev, sizeof(jev));
      if (jev.type == (JS_EVENT_INIT | JS_EVENT_AXIS)) axiss ++;
      if (jev.type == (JS_EVENT_INIT | JS_EVENT_BUTTON)) buttons ++;
  } while (jev.type & JS_EVENT_INIT);

  memset (mev, 0, 5);
  
  while (1) {
      if (jev.type == JS_EVENT_BUTTON) {
            if (jev.value)
	            but |= 1 << jev.number;
	    else
	            but &=~ (1 << jev.number);
      }      
      else if (jev.type == JS_EVENT_AXIS) {

	    if (jev.number) {
                    dy = jev.value / 4000;
	            mev[4] = -dy - (mev[2] = -dy/2);
            } else {
                    dx = jev.value / 4000;
                    mev[3] =  dx - (mev[1] =  dx/2);
	    }
      } else
	    exit (1);
     /****************************
      * out 1 = r	in 1 = l
      * out 2 = m	in 2 = r
      * out 4 = l	in 4 = m
      ****************************/
      mev [0] = 0x87 - (((but&1)<<2) + ((but&2)>>1) + ((but&4)>>1) );
      write (md, mev, 5);

     /*
      * State of set and tv is unspecified after select(2), isn't it ?
      */
      FD_ZERO (&set);
      FD_SET (jd, &set);
      tv.tv_sec  = 0;
      tv.tv_usec = MAX_DELAY;
      if (select (jd+1, &set, NULL, NULL, &tv))
            read  (jd, &jev, sizeof(jev));
  }
}
