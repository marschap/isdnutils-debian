/* $Id: icnctrl.c,v 1.6 1995/12/18 18:25:24 fritz Exp fritz $
 *
 * ICN-ISDN driver for Linux. (Control-Utility)
 *
 * Copyright 1994,95 by Fritz Elfert (fritz@wuemaus.franken.de)
 * Copyright 1995 Thinking Objects Software GmbH Wuerzburg
 *
 * This file is part of Isdn4Linux.
 * 
 * Isdn4Linux is distributed with NO WARRANTY OF ANY KIND.  No author
 * or distributor accepts any responsibility for the consequences of using it,
 * or for whether it serves any particular purpose or works at all, unless he
 * or she says so in writing.  Refer to the Isdn4Linux Free Public
 * License (the "License") for full details.
 * 
 * Every copy of Isdn4Linux must include a copy of the License,
 * normally in a plain ASCII text file named LICENSE.  The License grants you
 * the right to copy, modify and redistribute Isdn4Linux, but only
 * under certain conditions described in the License.  Among other things, the
 * License requires that the copyright notice and this notice be preserved on
 * all copies.
 *
 * $Log: icnctrl.c,v $
 * Revision 1.6  1995/12/18  18:25:24  fritz
 * Support for ICN-4B Cards.
 *
 * Revision 1.5  1995/10/29  21:44:02  fritz
 * Changed to support DriverId's.
 * Added support for leased lines.
 *
 * Revision 1.4  1995/04/23  13:43:02  fritz
 * Changed Copyright.
 *
 * Revision 1.3  1995/02/01  11:03:26  fritz
 * Added Messages, when loading bootcode and protocol.
 *
 * Revision 1.2  1995/01/09  07:41:59  fritz
 * Added GPL-Notice
 *
 *
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <linux/isdn.h>
#include "icn.h"

char *cmd;
char *ctrldev;
int  arg_ofs;

void usage() {
  fprintf(stderr,"usage: %s [-d driverId] io [mmio port]             (get/set hw-io)\n",cmd);
  fprintf(stderr,"   or: %s [-d driverId] add <portbase> [id1 [id2]] (Add a new card\n",cmd);
  fprintf(stderr,"   or: %s [-d driverId] load <bootcode> <protocol> (load firmware)\n",cmd);
  fprintf(stderr,"   or: %s [-d driverId] leased <on|off>            (Switch interface\n",cmd);
  fprintf(stderr,"                                                    into Leased-Line-Mode)\n");
  exit(-1);
}

int main(int argc, char **argv) {
  int fd;
  FILE *code;
  int mmio,port;
  int ac;
  int a;
  u_char buf[0x20000];
  isdn_ioctl_struct ioctl_s;
  icn_cdef newcard;

  cmd = strrchr(argv[0],'/');
  cmd = (cmd == NULL)?argv[0]:++cmd;
  if (argc>1) {
    if (!strcmp(argv[1],"-d")) {
      strcpy(ioctl_s.drvid,argv[2]);
      arg_ofs = 3;
    } else {
      ioctl_s.drvid[0] = '\0';
      arg_ofs = 1;
    }
  } else
    usage();
  ac = argc - (arg_ofs-1);
  fd = open("/dev/isdnctrl",O_RDWR);
  if (fd < 0) {
    perror(ctrldev);
    exit(-1);
  }
  if (!strcmp(argv[arg_ofs],"leased")) {
    if (ac == 3) {
      ioctl_s.arg = 0;
      if ((!strcmp(argv[arg_ofs+1],"on")) ||
          (!strcmp(argv[arg_ofs+1],"yes"))  )
	ioctl_s.arg = 1;
      if ((ioctl(fd,ICN_IOCTL_LEASEDCFG+IIOCDRVCTL,&ioctl_s))<0) {
	perror("ioctl LEASEDCFG");
	exit(-1);
      }
      close(fd);
      return 0;
    }
  }
  if (!strcmp(argv[arg_ofs],"add")) {
    if (ac >= 3) {
      sscanf(argv[arg_ofs+1],"%i",&a);
      if (a == 0x300 || a == 0x310 || a == 0x320 || a == 0x330
	  || a == 0x340 || a == 0x350 || a == 0x360 ||
	  a == 0x308 || a == 0x318 || a == 0x328 || a == 0x338
	  || a == 0x348 || a == 0x358 || a == 0x368) {
	ioctl_s.arg = (unsigned long)&newcard;
	newcard.port = a;
	newcard.id1[0] = 0;
	newcard.id2[0] = 0;
	if (ac >= 4)
	  strncpy(newcard.id1,argv[arg_ofs+2],sizeof(newcard.id1)-1);
	if (ac == 5)
	  strncpy(newcard.id2,argv[arg_ofs+3],sizeof(newcard.id2)-1);
	if ((ioctl(fd,ICN_IOCTL_ADDCARD+IIOCDRVCTL,&ioctl_s))<0) {
	  perror("ioctl ADDCARD");
	  exit(-1);
	}
	close(fd);
	return 0;
      }
    }
  }
  if (!strcmp(argv[arg_ofs],"io")) {
    if (ac == 4) {
      if (sscanf(argv[arg_ofs+1],"%i",&mmio)!=1)
	usage();
      if (sscanf(argv[arg_ofs+2],"%i",&port)!=1)
	usage();
      ioctl_s.arg = mmio;
      if ((mmio = ioctl(fd,ICN_IOCTL_SETMMIO+IIOCDRVCTL,&ioctl_s))<0) {
	perror("ioctl SETMMIO");
	exit(-1);
      }
      ioctl_s.arg = port;
      if ((port = ioctl(fd,ICN_IOCTL_SETPORT+IIOCDRVCTL,&ioctl_s))<0) {
	perror("ioctl SETPORT");
	exit(-1);
      }
    }
    if ((mmio = ioctl(fd,ICN_IOCTL_GETMMIO+IIOCDRVCTL,&ioctl_s))<0) {
      perror("ioctl GETMMIO");
      exit(-1);
    }
    if ((port = ioctl(fd,ICN_IOCTL_GETPORT+IIOCDRVCTL,&ioctl_s))<0) {
      perror("ioctl GETPORT");
      exit(-1);
    }
    printf("Memory-mapped io at 0x%08lx, port 0x%03x\n",
	   (unsigned long)mmio,(unsigned short)port);
    close(fd);
    return 0;
  }
  if (!strcmp(argv[arg_ofs],"load")) {
    int needed;
    switch (ioctl(fd,ICN_IOCTL_GETDOUBLE+IIOCDRVCTL,&ioctl_s)) {
      case 0:
	needed = 4;
	break;
      case 1:
	needed = 5;
	break;
      default:
	perror("ioctl GETDOUBLE");
	exit(-1);
    }
    if (ac == needed) {
      if (!(code=fopen(argv[arg_ofs+1],"r"))) {
	perror(argv[arg_ofs+1]);
	exit(-1);
      }
      if (fread(buf,4096,1,code)<1) {
	fprintf(stderr,"Read error on %s\n",argv[arg_ofs+1]);
	exit(-1);
      }
      printf("Loading Bootcode %s ... ",argv[arg_ofs+1]);
      fflush(stdout);
      ioctl_s.arg = (ulong)buf;
      if (ioctl(fd,ICN_IOCTL_LOADBOOT+IIOCDRVCTL,&ioctl_s)<0) {
	perror("\nioctl LOADBOOT");
	exit(-1);
      }
      fclose(code);
      printf("done\n");
      if (!(code=fopen(argv[arg_ofs+2],"r"))) {
	perror(argv[arg_ofs+2]);
	exit(-1);
      }
      if (fread(buf,65536,1,code)<1) {
	fprintf(stderr,"Read error on %s\n",argv[arg_ofs+2]);
	exit(-1);
      }
      fclose(code);
      if (needed == 5) {
	if (!(code=fopen(argv[arg_ofs+3],"r"))) {
	  perror(argv[arg_ofs+3]);
	  exit(-1);
	}
	if (fread(buf+65536,65536,1,code)<1) {
	  fprintf(stderr,"Read error on %s\n",argv[arg_ofs+3]);
	  exit(-1);
	}
	fclose(code);
	printf("Loading Protocols %s\n",argv[arg_ofs+2]);
	printf("              and %s ... ",argv[arg_ofs+3]);
      } else
	printf("Loading Protocol %s ... ",argv[arg_ofs+2]);
      fflush(stdout);
      ioctl_s.arg = (ulong)buf;
      if (ioctl(fd,ICN_IOCTL_LOADPROTO+IIOCDRVCTL,&ioctl_s)<0) {
	perror("\nioctl LOADPROTO");
	exit(-1);
      }
      printf("done\n");
      close(fd);
      return 0;
    }
    usage();
  }
  usage();
  return 0;
}



