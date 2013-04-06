jsme
====

Joystick Mouse Emulation

Description
===========

Daemon which translate stream of joystick events (read = output from /dev/js0) to
stream of MouseSystems events (read = input readable by gsm and/or X)

Installation
============
Because there are only two users of jsme as for this date,
no steps were taken to ease installation.

1. Edit jsme.c and set JOYSTICK_DEVICE to path to your 
    joystick device, if other than "/dev/js0"
2. Edit jsme.c and set PIPE_OUT to path where you emulated
    mouse device will be, if other than "/dev/jsme"
3. Create fifo for emulated mouse device. Like:
    $ mkfifo /dev/jsme
4. Compile:
    $ make
5. Run it with:
    $ nohup jsme &
   You can put this command into /etc/rc.boot or to other
   place from where it will be automatically started on boot time.

Now either:

6a. Edit /etc/gpm.conf to contain:
    device=/dev/jsme
    repeat_type=ms3
    type=msc
7a. Set up X server to use /dev/gpmdata

Or:

6b. Set up X server to use /dev/jsme

gpm-js, install and uninstall are simple helper scripts,
not very useful.

Note
====

This project is probably of mainly historical value only.
I'm moving my various old Open Source projects to github on an off chance someone finds them useful,
but it hasn't been maintained in very long time.
