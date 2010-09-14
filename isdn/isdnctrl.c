/* $Id: isdnctrl.c,v 1.14 1996/06/06 22:08:46 fritz Exp $
 *
 * ISDN driver for Linux. (Control-Utility)
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
 * $Log: isdnctrl.c,v $
 * Revision 1.14  1996/06/06 22:08:46  fritz
 * Bugfix: verbose and getconf checked wrong number of parameters.
 *         thanks to Andreas Jaeger <aj@arthur.pfalz.de>
 *
 * Revision 1.13  1996/04/30 12:48:35  fritz
 * Added Michael's ippp-bind patch.
 *
 * Revision 1.12  1996/04/30 12:43:18  fritz
 * Changed ioctl-names according to kernel-version
 *
 * Revision 1.11  1996/01/04 02:44:52  fritz
 * Changed copying policy to GPL
 * Added addslave, dial, sdelay and mapping.
 *
 * Revision 1.10  1995/12/18  18:03:19  fritz
 * New License, minor cleanups.
 *
 * Revision 1.9  1995/10/29  21:38:51  fritz
 * Changed all references to driver-numbers to new DriverId's
 *
 * Revision 1.8  1995/07/15  20:39:56  fritz
 * Added support for cisco_h Encapsulation.
 * Added suppurt for pre-binding an interface to a channel.
 *
 * Revision 1.7  1995/04/29  13:13:44  fritz
 * Added new command verbose.
 *
 * Revision 1.6  1995/04/23  13:38:34  fritz
 * Adapted addphone and delphone to support changes in isdn.c
 *
 * Revision 1.5  1995/03/25  23:35:35  fritz
 * Added ihup-Feature.
 *
 * Revision 1.4  1995/03/15  12:44:15  fritz
 * Added generic conversion-routines for keyword<->value conversion.
 * Added display of phone-numbers in list-routine
 * Corrected some typos
 *
 * Revision 1.3  1995/02/20  03:38:59  fritz
 * Added getmax and rmax for performance-tests of tty-driver.
 *
 * Revision 1.2  1995/01/29  23:27:52  fritz
 * Added keywords: list, l2_proto, l3_proto, huptimeout, chargehup and
 * encap.
 *
 * Revision 1.1  1995/01/09  07:35:35  fritz
 * Initial revision
 *
 *
 */

#undef  ISDN_DEBUG_MODEM_SENDOPT

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <linux/isdn.h>
#include <linux/isdnif.h>

char *l2protostr[] = {"x75i", "x75ui", "x75bui", "hdlc", "\0"};
int  l2protoval[]  = {ISDN_PROTO_L2_X75I, ISDN_PROTO_L2_X75UI,
		      ISDN_PROTO_L2_X75BUI, ISDN_PROTO_L2_HDLC, -1};

char *l3protostr[] = {"trans", "\0"};
int  l3protoval[]  = {ISDN_PROTO_L3_TRANS, -1};

char *pencapstr[]  = {"ethernet", "rawip", "ip", "cisco-h", "syncppp" ,
                      "uihdlc", "\0"};
int  pencapval[]   = {ISDN_NET_ENCAP_ETHER, ISDN_NET_ENCAP_RAWIP,
		      ISDN_NET_ENCAP_IPTYP, ISDN_NET_ENCAP_CISCOHDLC, 
                      ISDN_NET_ENCAP_SYNCPPP , ISDN_NET_ENCAP_UIHDLC, -1};

char *num2callb[]  = {"off","in","out"};

char *cmd;

char nextlistif[10];

void
usage(void) {
  fprintf(stderr,"usage: %s <command> <options>\n",cmd);
  fprintf(stderr,"\n");
  fprintf(stderr,"where <command> is one of the following:\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"    addif [name]               (add net-interface)\n");
  fprintf(stderr,"    delif name                 (remove net-interface)\n");
  fprintf(stderr,"    addphone name in|out num   (add phone-number to interface)\n");
  fprintf(stderr,"    delphone name in|out num   (remove phone-number from interface)\n");
  fprintf(stderr,"    eaz name [eaz|msn]         (get/set eaz for interface)\n");
  fprintf(stderr,"    huptimeout name [seconds]  (get/set hangup-timeout for interface)\n");
  fprintf(stderr,"    ihup name [on|off]         (get/set incoming-hangup for interface)\n");
  fprintf(stderr,"    chargehup name [on|off]    (get/set charge-hangup for interface)\n");
  fprintf(stderr,"    secure name [on|off]       (get/set secure-feature for interface)\n");
  fprintf(stderr,"    callback name [in|outon|off]\n");
  fprintf(stderr,"                               (get/set active callback-feature for interface)\n");
  fprintf(stderr,"    cbhup name [on|off]        (get/set reject-before-callback for interface)\n");
  fprintf(stderr,"    cbdelay name [seconds]     (get/set delay before callback for interface)\n");
  fprintf(stderr,"    dialmax name [num]         (get/set number of dial-atempts for interface)\n");
  fprintf(stderr,"    encap name [encapname]     (get/set packet-encapsulation for interface)\n");
  fprintf(stderr,"    l2_prot name [protocol]    (get/set layer-2-protocol for interface)\n");
  fprintf(stderr,"    l3_prot name [protocol]    (get/set layer-3-protocol for interface)\n");
  fprintf(stderr,"    bind name [drvId,channel [exclusive]]\n");
  fprintf(stderr,"                               (pre-bind interface to a channel)\n");
  fprintf(stderr,"    unbind name                (delete pre-binding)\n");
  fprintf(stderr,"    list name|all              (show current setup of interface(s))\n");
  fprintf(stderr,"    verbose num                (set verbose-level)\n");
  fprintf(stderr,"    hangup name                (force hangup of interface)\n");
  fprintf(stderr,"    busreject drvId on|off     (set bus-reject-mode)\n");
  fprintf(stderr,"    mapping drvId [MSN,MSN...] (set MSN<->EAZ-Mapping\n");
  fprintf(stderr,"    addslave name slavename    (add slave-interface\n");
  fprintf(stderr,"    sdelay mastername delay    (set slave-activation delay\n");
  fprintf(stderr,"    dial name                  (force dialing of interface\n");
  fprintf(stderr,"    system on|off              (switch isdn-system on or off)\n");
  fprintf(stderr,"    addlink name               (MPPP, increase number of links)\n");
  fprintf(stderr,"    removelink name            (MPPP, decrease number of links)\n");
  fprintf(stderr,"    pppbind name [devicenum]   (PPP, bind interface to ippp-device (always exclusive)\n");
  fprintf(stderr,"    pppunbind name             (PPP, remove ippp-device binding\n");
  exit(-2);
}

static int
key2num(char *key, char **keytable, int *numtable) {
  int i = -1;
  while (strlen(keytable[++i]))
    if (!strcmp(keytable[i],key))
      return numtable[i];
  return -1;
}

static char*
num2key(int num, char **keytable, int *numtable) {
  int i = -1;
  while (numtable[++i]>=0)
    if (numtable[i]==num)
      return keytable[i];
  return "???";
}

static void
listbind(char *s, int e) {
  if (strlen(s)) {
    char *p = strchr(s,',');
    int ch;
    sscanf(p+1,"%d",&ch);
    *p = '\0';
    printf("%s, Channel %d%s\n",s,ch,(e>0)?", exclusive":"");
  } else
    printf("Nothing\n");
}

static void
listif(int isdnctrl, char *name, int errexit) {
  isdn_net_ioctl_cfg   cfg;
  union p {
    isdn_net_ioctl_phone phone;
    char                 n[1024];
  } ph;
  char nn[1024];

  strcpy(cfg.name,name);
  if (ioctl(isdnctrl,IIOCNETGCF,&cfg)<0) {
    if (errexit) {
      perror(name);
      exit(-1);
    } else
      return;
  }
  strcpy(ph.phone.name,name);
  ph.phone.outgoing = 0;
  if (ioctl(isdnctrl,IIOCNETGNM,&ph.phone)<0) {
    if (errexit) {
      perror(name);
      exit(-1);
    } else
      return;
  }
  strcpy(nn,ph.n);
  strcpy(ph.phone.name,name);
  ph.phone.outgoing = 1;
  if (ioctl(isdnctrl,IIOCNETGNM,&ph.phone)<0) {
    if (errexit) {
      perror(name);
      exit(-1);
    } else
      return;
  }
  printf("\nCurrent setup of interface '%s':\n\n",cfg.name);
  printf("EAZ/MSN:                %s\n",cfg.eaz);
  printf("Phone number(s):\n");
  printf("  Outgoing:             %s\n",ph.n);
  printf("  Incoming:             %s\n",nn);
  printf("Secure:                 %s\n",cfg.secure?"on":"off");
  printf("Callback:               %s\n",num2callb[cfg.callback]);
  if (cfg.callback == 2)
    printf("Hangup after Dial       %s\n",cfg.cbdelay?"on":"off");
  else
    printf("Reject before Callback: %s\n",cfg.cbhup?"on":"off");
  printf("Callback-delay:         %d\n",cfg.cbdelay/5);
  printf("Dialmax:                %d\n",cfg.dialmax);
  printf("Hangup-Timeout:         %d\n",cfg.onhtime);
  printf("Incoming-Hangup:        %s\n",cfg.ihup?"on":"off");
  printf("ChargeHangup:           %s\n",cfg.chargehup?"on":"off");
  printf("Charge-Units:           %d\n",cfg.charge);
  printf("Layer-2-Protocol:       %s\n",num2key(cfg.l2_proto,l2protostr,l2protoval));
  printf("Layer-3-Protocol:       %s\n",num2key(cfg.l3_proto,l3protostr,l3protoval));
  printf("Encapsulation:          %s\n",num2key(cfg.p_encap,pencapstr,pencapval));
  printf("Slave Interface:        %s\n",strlen(cfg.slave)?cfg.slave:"None");
  printf("Slave delay:            %d\n",cfg.slavedelay);
  printf("Master Interface:       %s\n",strlen(cfg.master)?cfg.master:"None");
  printf("Pre-Bound to:           ");
  listbind(cfg.drvid,cfg.exclusive);
  if (strlen(cfg.slave))
    strcpy(nextlistif,cfg.slave);
  else
    nextlistif[0] = '\0';
}

static void
get_setup(int isdnctrl, char *name) {
  isdn_net_ioctl_cfg cfg;
  char buffer[0x1000];
  char *p;
  FILE *f;

  if (ioctl(isdnctrl,IIOCGETSET,&buffer)<0) {
    perror("ioctl GET_SETUP");
    exit(-1);
  }
  f = fopen(name,"w");
  if (!f) {
    perror(name);
    exit(-1);
  }
  p = buffer;
  while (strlen(p)) {
    fprintf(f,"IF=%s\n",p);
    p += 10;
    memcpy((char *)&cfg,p,sizeof(cfg));
    fprintf(f," E=%s\n",cfg.eaz);
    fprintf(f," F=%d %d %d %d %d %d %d %d\n",cfg.secure,cfg.callback,
	    cfg.onhtime,cfg.ihup,cfg.chargehup,cfg.l2_proto,cfg.l3_proto,
	    cfg.p_encap);
    fprintf(f," P=%d %s\n",cfg.exclusive,cfg.drvid);
    p += sizeof(cfg);
    fprintf(f," O=%s\n",p);
    p += (strlen(p)+1);
    fprintf(f," I=%s\n",p);
    p += (strlen(p)+1);
  }
  fclose(f);
}

void
main(int argc, char **argv) {
  int fd;
  int i;
  int result;
  FILE *iflst;
  char *p;
  char s[255];
  isdn_net_ioctl_phone phone;
  isdn_net_ioctl_cfg   cfg;
  isdn_ioctl_struct    iocts;

  cmd = argv[0];
  if (argc>=2) {
    fd = open("/dev/isdnctrl",O_RDWR);
    if (fd < 0) {
      perror("/dev/isdnctrl");
      exit(-1);
    }
    if (!strcmp(argv[1],"addif")) {
      if (argc==2)
	strcpy(s,"  ");
      else {
	if (strlen(argv[2])>8) {
	  fprintf(stderr,"name must not exceed 8 characters\n");
	  exit(-1);
	}
	strcpy(s,argv[2]);
      }
      if ((result=ioctl(fd,IIOCNETAIF,s))<0) {
	perror("addif");
	exit(-1);
      }
      printf("%s added\n",s);
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"addslave")) {
      if (argc<4)
	usage();
      else {
	if (strlen(argv[2])>8) {
	  fprintf(stderr,"name must not exceed 8 characters\n");
	  exit(-1);
	}
	if (strlen(argv[3])>8) {
	  fprintf(stderr,"slavename must not exceed 8 characters\n");
	  exit(-1);
	}
	sprintf(s,"%s,%s",argv[2],argv[3]);
      }
      if ((result=ioctl(fd,IIOCNETASL,s))<0) {
	perror("addslave");
	exit(-1);
      }
      printf("%s added as slave to %s\n",s,argv[2]);
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"delif")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      if ((result=ioctl(fd,IIOCNETDIF,argv[2]))<0) {
	perror(argv[2]);
	exit(-1);
      }
      printf("%s deleted\n",argv[2]);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"dial")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      if ((result=ioctl(fd,IIOCNETDIL,argv[2]))<0) {
	perror(argv[2]);
	exit(-1);
      }
      printf("Dialing of %s triggered\n",argv[2]);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"bind")) {
      if ((argc<4) || (argc>5))
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      if (argc==5)
	if (strncmp(argv[4],"excl",4))
	  usage();
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      sscanf(argv[3],"%s",cfg.drvid);
      cfg.exclusive = (argc == 5);
      if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	sprintf(s,"%s or %s",argv[2],argv[3]);
	perror(s);
	exit(-1);
      }
      printf("%s bound to ",argv[2]);
      listbind(cfg.drvid,cfg.exclusive);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"unbind")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (!strlen(cfg.drvid)) {
	printf("%s was not bound to anything\n",argv[2]);
	exit(0);
      }
      cfg.drvid[0] = '\0';
      cfg.exclusive = -1;
      if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      printf("%s unbound successfully\n",argv[2]);
      close(fd);
      return;
    }  

    if (!strcmp(argv[1],"pppbind")) {
      if (argc<4)
        usage();
      if (strlen(argv[2])>8) {
        fprintf(stderr,"name must not exceed 8 characters\n");
        exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
        perror(argv[2]);
        exit(-1);
      }
      sscanf(argv[3],"%d",&cfg.pppbind);
      if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
        sprintf(s,"%s or %s",argv[2],argv[3]);
        perror(s);
        exit(-1);
      }
      printf("%s bound to %s",argv[2],argv[3]);
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"pppunbind")) {
      if (argc<3)
        usage();
      if (strlen(argv[2])>8) {
        fprintf(stderr,"name must not exceed 8 characters\n");
        exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
        perror(argv[2]);
        exit(-1);
      }
      if (cfg.pppbind < 0) {
        printf("%s was not bound to anything\n",argv[2]);
        exit(0);
      }
      cfg.pppbind = -1;
      if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
        perror(argv[2]);
        exit(-1);
      }
      printf("%s unbound successfully\n",argv[2]);
      close(fd);
      return;
    }

    if (!strcmp(argv[1],"busreject")) {
      if (argc<4)
	usage();
      strcpy(iocts.drvid,argv[2]);
      if (strcmp(argv[3],"on") && strcmp(argv[3],"off")) {
	fprintf(stderr,"Bus-Reject must be 'on' or 'off'\n");
	exit(-1);
      }
      iocts.arg = strcmp(argv[3],"off");
      if ((result=ioctl(fd,IIOCSETBRJ,&iocts))<0) {
	perror(argv[2]);
	exit(-1);
      }
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"mapping")) {
      char nstring[255];
      if (argc<3)
	usage();
      strcpy(iocts.drvid,argv[2]);
      if (argc<4) {
	iocts.arg = (unsigned long)&nstring;
	if ((result=ioctl(fd,IIOCGETMAP,&iocts))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
	printf("MSN/EAZ-mapping for %s:\n%s\n",argv[2],nstring);
      } else {
	iocts.arg = (unsigned long)argv[3];
	if ((result=ioctl(fd,IIOCSETMAP,&iocts))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"system")) {
      unsigned long i;

      if (argc<3)
	usage();
      if (strcmp(argv[2],"on") && strcmp(argv[2],"off")) {
	fprintf(stderr,"System-Mode must be 'on' or 'off'\n");
	exit(-1);
      }
      i = strcmp(argv[2],"on");
      if ((result=ioctl(fd,IIOCSETGST,i))<0) {
	perror(argv[2]);
	exit(-1);
      }
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"hangup")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      if ((result=ioctl(fd,IIOCNETHUP,argv[2]))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (result)
	printf("%s not connected\n",argv[2]);
      else
	printf("%s hung up\n",argv[2]);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"addphone")) {
      if (argc<5)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      if (strcmp(argv[3],"in") && strcmp(argv[3],"out")) {
	fprintf(stderr,"Direction must be \"in\" or \"out\"\n");
	exit(-1);
      }
      phone.outgoing = strcmp(argv[3],"out")?0:1;
      if (strlen(argv[4])>20) {
	fprintf(stderr,"phone-number must not exceed 20 characters\n");
	exit(-1);
      }
      strcpy(phone.name,argv[2]);
      strcpy(phone.phone,argv[4]);
      if ((result=ioctl(fd,IIOCNETANM,&phone))<0) {
	perror(argv[2]);
	exit(-1);
      }
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"delphone")) {
      if (argc<5)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      if (strcmp(argv[3],"in") && strcmp(argv[3],"out")) {
	fprintf(stderr,"Direction must be \"in\" or \"out\"\n");
	exit(-1);
      }
      phone.outgoing = strcmp(argv[3],"out")?0:1;
      if (strlen(argv[4])>20) {
	fprintf(stderr,"phone-number must not exceed 20 characters\n");
	exit(-1);
      }
      strcpy(phone.name,argv[2]);
      strcpy(phone.phone,argv[4]);
      if ((result=ioctl(fd,IIOCNETDNM,&phone))<0) {
	perror(argv[2]);
	exit(-1);
      }
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"list")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      if (!strcmp(argv[2],"all")) {
	char name[10];
	if ((iflst = fopen("/proc/net/dev","r"))==NULL) {
	  perror("/proc/net/dev");
	  exit(-1);
	}
	while (!feof(iflst)) {
	  fgets(s,sizeof(s),iflst);
	  if ((p = strchr(s,':'))) {
	    *p = 0;
	    sscanf(s,"%s",name);
	    listif(fd,name,0);
	    while (strlen(nextlistif))
	      listif(fd,nextlistif,0);
	  }
	}
	fclose(iflst);
      } else
	listif(fd,argv[2],1);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"eaz")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	strncpy(cfg.eaz,argv[3],sizeof(cfg.eaz)-1);
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("EAZ/MSN for %s is %s\n",cfg.name,cfg.eaz);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"verbose")) {
      if (argc<3)
	usage();
      i = -1;
      sscanf(argv[2],"%d",&i);
      if (i<0) {
	fprintf(stderr,"Verbose-level must be >= 0\n");
	exit(-1);
      }
      if ((result=ioctl(fd,IIOCSETVER,i))<0) {
	perror("IIOCSETVER");
	exit(-1);
      }
      printf("Verbose-level set to %d.\n",i);
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"getconf")) {
      if (argc<3)
	usage();
      get_setup(fd,argv[2]);
      printf("Configuration written to %s.\n",argv[2]);
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"huptimeout")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	sscanf(argv[3],"%d",&i);
	if (i<0) {
	  fprintf(stderr,"Hangup-Timeout must be >= 0\n");
	  exit(-1);
	}
	cfg.onhtime = i;
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Hangup-Timeout for %s is %d sec.\n",cfg.name,cfg.onhtime);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"cbdelay")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	sscanf(argv[3],"%d",&i);
	if (i<0) {
	  fprintf(stderr,"Callback delay must be >= 0\n");
	  exit(-1);
	}
	cfg.cbdelay = i*5;
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Callback delay for %s is %d sec.\n",cfg.name,cfg.cbdelay/5);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"dialmax")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	sscanf(argv[3],"%d",&i);
	if (i<1) {
	  fprintf(stderr,"Dialmax must be > 0\n");
	  exit(-1);
	}
	cfg.dialmax = i;
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Dialmax for %s is %d times.\n",cfg.name,cfg.dialmax);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"sdelay")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	sscanf(argv[3],"%d",&i);
	if (i<1) {
	  fprintf(stderr,"Slave-activation delay must be >= 1\n");
	  exit(-1);
	}
	cfg.slavedelay = i;
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Slave-activation delay for %s is %d sec.\n",cfg.name,
	     cfg.slavedelay);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"chargehup")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	if (strcmp(argv[3],"on") && strcmp(argv[3],"off")) {
	  fprintf(stderr,"Charge-Hangup must be 'on' or 'off'\n");
	  exit(-1);
	}
	cfg.chargehup = strcmp(argv[3],"off");
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Charge-Hangup for %s is %s\n",cfg.name,cfg.chargehup?"on":"off");
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"cbhup")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	if (strcmp(argv[3],"on") && strcmp(argv[3],"off")) {
	  fprintf(stderr,"Callback-Hangup must be 'on' or 'off'\n");
	  exit(-1);
	}
	cfg.cbhup = strcmp(argv[3],"off");
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Reject before Callback for %s is %s\n",cfg.name,cfg.cbhup?"on":"off");
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"ihup")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	if (strcmp(argv[3],"on") && strcmp(argv[3],"off")) {
	  fprintf(stderr,"Incoming-Hangup must be 'on' or 'off'\n");
	  exit(-1);
	}
	cfg.ihup = strcmp(argv[3],"off");
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Incoming-Hangup for %s is %s\n",cfg.name,cfg.ihup?"on":"off");
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"secure")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	if (strcmp(argv[3],"on") && strcmp(argv[3],"off")) {
	  fprintf(stderr,"Secure-parameter must be 'on' or 'off'\n");
	  exit(-1);
	}
	cfg.secure = strcmp(argv[3],"off");
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Security for %s is %s\n",cfg.name,cfg.secure?"on":"off");
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"callback")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = -1;
	if (strcmp(argv[3],"on") && strcmp(argv[3],"off") &&
	    strcmp(argv[3],"in") && strcmp(argv[3],"out")) {
	  fprintf(stderr,"Callback-parameter must be 'on', 'in', 'out' or 'off'\n");
	  exit(-1);
	}
	cfg.callback = strcmp(argv[3],"off")?1:0;
	if (!strcmp(argv[3],"out")) cfg.callback = 2;
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Callback for %s is %s\n",cfg.name,num2callb[cfg.callback]);
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"l2_prot")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = key2num(argv[3],l2protostr,l2protoval);
	if (i<0) {
	  fprintf(stderr,"Layer-2-Protocol must be one of the following:\n");
	  i = 0;
	  while (strlen(l2protostr[i]))
	    fprintf(stderr,"\t\"%s\"\n",l2protostr[i++]);
	  exit(-1);
	}
	cfg.l2_proto = i;
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Layer-2-Protocol for %s is %s\n",cfg.name,
	     num2key(cfg.l2_proto,l2protostr,l2protoval));
      close(fd);
      return;
    }  
    if (!strcmp(argv[1],"l3_prot")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = key2num(argv[3],l3protostr,l3protoval);
	if (i<0) {
	  fprintf(stderr,"Layer-3-Protocol must be one of the following:\n");
	  i = 0;
	  while (strlen(l3protostr[i]))
	    fprintf(stderr,"\t\"%s\"\n",l3protostr[i++]);
	  exit(-1);
	}
	cfg.l3_proto = i;
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Layer-3-Protocol for %s is %s\n",cfg.name,
	     num2key(cfg.l3_proto,l3protostr,l3protoval));
      close(fd);
      return;
    } 
    if(!strcmp(argv[1],"addlink")) {
      if(argc!=3)
        usage();
      if(strlen(argv[2])>8) {
        fprintf(stderr,"name must not exceed 8 characters\n");
        exit(-1);
      }
      if((result=ioctl(fd,IIOCNETALN,argv[2]))<0)
      {
        perror(argv[2]);
        exit(-1);
      }
      if(result)
        printf("Can't increase number of links: %d\n",result);
      else
        printf("Ok, added a new link. (dialing)\n");
      close(fd);
      return; 
    }
    if(!strcmp(argv[1],"removelink")) {
      if(argc!=3)
        usage();
      if(strlen(argv[2])>8) {
        fprintf(stderr,"name must not exceed 8 characters\n");
        exit(-1);
      }
      if((result=ioctl(fd,IIOCNETDLN,argv[2]))<0)
      {
        perror(argv[2]);
        exit(-1);
      }
      if(result)
        printf("Can't decrease number of links: %d\n",result);
      else
        printf("Ok, removed a link. (hangup)\n");
      close(fd);
      return;
    }
    if (!strcmp(argv[1],"encap")) {
      if (argc<3)
	usage();
      if (strlen(argv[2])>8) {
	fprintf(stderr,"name must not exceed 8 characters\n");
	exit(-1);
      }
      strcpy(cfg.name,argv[2]);
      if ((result=ioctl(fd,IIOCNETGCF,&cfg))<0) {
	perror(argv[2]);
	exit(-1);
      }
      if (argc>3) {
	i = key2num(argv[3],pencapstr,pencapval);
	if (i<0) {
	  fprintf(stderr,"Encapsulation must be one of the following:\n");
	  i = 0;
	  while (strlen(pencapstr[i]))
	    fprintf(stderr,"\t\"%s\"\n",pencapstr[i++]);
	  exit(-1);
	}
	cfg.p_encap = i;
	if ((result=ioctl(fd,IIOCNETSCF,&cfg))<0) {
	  perror(argv[2]);
	  exit(-1);
	}
      }
      printf("Encapsulation for %s is %s\n",cfg.name,
	     num2key(cfg.p_encap,pencapstr,pencapval));
      close(fd);
      return;
    }  
    close(fd);
  }
  usage();
}



