/* $Id: iprofd.c,v 1.3 1996/04/30 12:45:48 fritz Exp fritz $
 *
 * Daemon for saving ttyIx-profiles to a file.
 *
 * Copyright 1994,95 by Fritz Elfert (fritz@wuemaus.franken.de)
 * Copyright 1995 Thinking Objects Software GmbH Wuerzburg
 *
 * This file is part of Isdn4Linux.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 * $Log: iprofd.c,v $
 * Revision 1.3  1996/04/30 12:45:48  fritz
 * Changed ioctl-names according to kernel-version.
 *
 * Revision 1.2  1996/01/04 02:43:44  fritz
 * Changed copying policy to GPL.
 *
 * Revision 1.1  1995/12/18  18:22:24  fritz
 * Initial revision
 *
 */

#define DATA_VERSION 1
#define SIGNATURE "iprofd%02x"
#define SIGLEN 9

#include <sys/types.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <linux/isdn.h>

typedef unsigned char uchar;

int  isdnctrl_fd;
char *modemsettings;

#define BUFSZ ((ISDN_MODEM_ANZREG+ISDN_MSNLEN)*ISDN_MAX_CHANNELS)

void
dumpModem(int dummy) {
  int fd;
  int len;
  char buffer[BUFSZ];
  char signature[SIGLEN];

  if ((len = ioctl(isdnctrl_fd,IIOCGETPRF,&buffer))<0) {
    perror("ioctl IIOCGETPRF");
    exit(-1);
  }
  fd = open(modemsettings,O_WRONLY|O_CREAT|O_TRUNC,0644);
  if (fd<0) {
    perror(modemsettings);
    exit(-1);
  }
  sprintf(signature,SIGNATURE,DATA_VERSION);
  write(fd,signature,sizeof(signature));
  write(fd,buffer,len);
  close(fd);
  signal(SIGIO,dumpModem);
}

void
readModem(void) {
  int len;
  int fd;
  char buffer[BUFSZ];
  char signature[SIGLEN];

  sprintf(signature,SIGNATURE,DATA_VERSION);
  fd = open(modemsettings,O_RDONLY);
  if (fd<0)
    return;
  len = read(fd,buffer,sizeof(signature));
  if (len<0) {
    perror(modemsettings);
    exit(-1);
  }
  if (strcmp(buffer,signature)) {
    fprintf(stderr,"Version of iprofd (%d) does NOT match\n",DATA_VERSION);
    fprintf(stderr,"signature of saved data!\n");
    fprintf(stderr,"Profiles NOT restored, use AT&W0 to update data.\n");
    close(fd);
    return;
  }
  len = read(fd,buffer,BUFSZ);
  if (len<0) {
    perror(modemsettings);
    exit(-1);
  }
  close(fd);
  if (ioctl(isdnctrl_fd,IIOCSETPRF,&buffer)<0) {
    perror("ioctl IIOCSETPRF");
    exit(-1);
  }
}

void
usage(void) {
  fprintf(stderr,"usage: iprofd <IsdnModemProfile>\n");
  exit(-1);
}

void
main(int argc, char **argv) {

  int data_version;

  if (argc != 2)
    usage();
  modemsettings = argv[1];
  isdnctrl_fd = open("/dev/isdninfo",O_RDWR);
  if (isdnctrl_fd<0) {
    perror("/dev/isdninfo");
    exit(-1);
  }
  data_version = ioctl(isdnctrl_fd,IIOCGETDVR,0);
  if (data_version<0) {
    fprintf(stderr,"Could not get version info of ISDN modem-profile!\n");
    fprintf(stderr,"Make sure, you are using the correct version.\n");
    exit(-1);
  }
  close(isdnctrl_fd);
  data_version &= 0xff;
  if (data_version != DATA_VERSION) {
    fprintf(stderr,"Version of kernel modem-profile (%d) does NOT match\n",
            data_version);
    fprintf(stderr,"version of iprofd (%d)!\n",DATA_VERSION);
    fprintf(stderr,"Make sure, you are using the correct version.\n");
    exit(-1);
  }
  isdnctrl_fd = open("/dev/isdnctrl",O_RDONLY);
  if (isdnctrl_fd<0) {
    perror("/dev/isdninfo");
    exit(-1);
  }
  readModem();
  switch (fork()) {
    case -1:
      perror("fork");
      exit(-1);
      break;
    case 0:      
      dumpModem(0);
      if (ioctl(isdnctrl_fd,IIOCSIGPRF,0)) {
	perror("ioctl IIOCSIGPRF");
	exit(-1);
      }
      while (1) sleep(1000);
      break;
    default:
      break;
  }
}