/* $Id: isdnlog.c,v 2.41 1996/06/23 10:59:46 root Exp $
 *
 * ISDN accounting for isdn4linux. (log-module)
 *
 * Copyright 1995, 1996 by Andreas Kool (akool@Kool.f.EUnet.de)
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
 * $Log: isdnlog.c,v $
 * Revision 2.41    1996/06/23  10:59:46  akool
 * -C Option fixed
 *
 * Revision 2.40    1996/06/20  19:12:46  akool
 *
 * Revision 2.3.28  1996/05/06  22:18:46  akool
 * "huptimeout" handling implemented (-hx)
 *
 * Revision 2.3.24  1996/05/04  23:03:46  akool
 * Kleiner Fix am ASN.1 Parser von Bernhard Kruepl
 * i/o byte Handing redesigned
 *
 * Revision 2.3.23  1996/04/28  12:44:46  akool
 * PRT_SHOWIMON eingefuehrt
 *
 * Revision 2.3.21  1996/04/26  11:43:46  akool
 * Faelschliche DM 0,12 Meldung an xisdn unterdrueckt
 *
 * Revision 2.3.19  1996/04/25  21:44:46  akool
 * -DSELECT_FIX, new Option "-M"
 * Optionen "-i" und "-c" entfernt
 *
 * Revision 2.3.17  1996/04/23  00:25:46  akool
 * isdn4kernel-1.3.93 voll implementiert
 *
 * Revision 2.3.16  1996/04/22  22:58:46  akool
 * Temp. Fix fuer isdn4kernel-1.3.91 implementiert
 *
 * Revision 2.3.15  1996/04/22  21:25:46  akool
 * general cleanup
 *
 * Revision 2.3.13  1996/04/18  20:36:46  akool
 * Fehlerhafte Meldung der Durchsatzrate auf unbenutztem Kanal unterdrueckt
 *
 * Revision 2.3.11  1996/04/14  21:26:46  akool
 *
 * Revision 2.3.4  1996/04/05  13:50:46  akool
 * NEWCPS-Handling implemented
 *
 * Revision 2.2.5  1996/03/25  19:47:46  akool
 * Fix in Exit() (sl)
 * 1TR6-Unterstuetzung fertiggestellt
 * Neuer Switch "-e" zum Unterdruecken der "tei" Angabe
 *
 * Revision 2.2.4  1996/03/24  12:17:46  akool
 * 1TR6 Causes implemented
 * 1TR6 / E-DSS1 Frames werden unterschieden
 * Pipe-Funktionalitaet reaktiviert 19-03-96 Bodo Bellut (lasagne@garfield.ping.de)
 * Alle Console-Ausgaben wieder mit \r
 * Gebuehrenauswertung fuer 1TR6 implementiert (Wim Bonis (bonis@kiss.de))
 *
 * Revision 2.23  1996/03/17  12:26:46  akool
 *
 * Revision 2.20  1996/03/11  21:15:46  akool
 * Calling/Called party decoding
 *
 * Revision 2.19  1996/03/10  19:46:46  akool
 * Alarm-Handling fuer /dev/isdnctrl0 funzt!
 *
 * Revision 2.17  1996/02/25  19:23:46  akool
 * Andy's Geburtstags-Release
 *
 * Revision 2.15  1996/02/21  20:38:46  akool
 * sl's Server-Verschmelzung
 * Gernot's Parken/Makeln
 *
 * Revision 2.15  1996/02/17  21:01:10  root
 * Nun geht auch Parken und Makeln
 *
 * Revision 2.14  1996/02/17  16:00:00  root
 * Zeitfehler weg
 *
 * Revision 2.15  1996/02/21  20:30:42  akool
 * sl's Serververschmelzung
 * Gernot's Makeln
 *
 * Revision 2.13  1996/02/15  21:03:42  akool
 * ein kleiner noch
 * Gernot's Erweiterungen implementiert
 * MSG_CALL_INFO enthaelt nun State
 *
 * Revision 2.12  1996/02/13  20:08:43  root
 * Nu geht's (oder?)
 *
 * Revision 1.4  1996/02/13  20:05:28  root
 * so nun gehts
 *
 * Revision 1.3  1996/02/13  18:08:45  root
 * Noch ein [ und ein ;
 *
 * Revision 1.2  1996/02/13  18:02:40  root
 * Haben wir's drin - erster Versuch!
 *
 * Revision 1.1  1996/02/13  14:28:14  root
 * Initial revision
 *
 * Revision 2.10  1996/02/12  20:38:16  akool
 * TEI-Handling von Gernot Zander
 *
 * Revision 2.06  1996/02/10  20:10:16  akool
 * Handling evtl. vorlaufender "0" bereinigt
 *
 * Revision 2.05  1996/02/05  21:42:16  akool
 * Signal-Handling eingebaut
 * AVON-Handling implementiert
 *
 * Revision 2.04  1996/01/31  18:30:16  akool
 * Bugfix im C/S
 * Neue Option "-R"
 *
 * Revision 2.03  1996/01/29  15:13:16  akool
 * Bugfix im C/S

 * Revision 2.02  1996/01/27  15:13:16  akool
 * Stefan Luethje's Client-/Server Anbindung implementiert
 * Bugfix bzgl. HANGUP ohne AOCE-Meldung
 *
 * Revision 2.01  1996/01/21  15:32:16  akool
 * Erweiterungen fuer Michael 'Ghandi' Herold implementiert
 * Syslog-Meldungen implementiert
 * Reread der isdnlog.conf bei SIGHUP implementiert
 * AOCD/AOCE Auswertungen fuer Oesterreich implementiert
 *
 * Revision 2.00  1996/01/10  20:10:16  akool
 * Vollstaendiges Redesign, basierend auf der "call reference"
 * WARNING: Requires Patch of 'q931.c'
 *
 * Revision 1.25  1995/11/18  14:38:16  akool
 * AOC von Anrufen auf 0130-xxx werden korrekt ausgewertet
 *
 * Revision 1.24  1995/11/12  11:08:16  akool
 * Auch die "call reference" wird (ansatzweise) ausgewertet
 * Neue Option "-x" aktiviert X11-Popup
 * Date/Time wird ausgewertet
 * AOC-D wird korrekt ausgewertet
 * Neue Option "-t" setzt Systemzeit auf die von der VSt gemeldete
 * Die "-m" Option kann nun auch mehrfach (additiv) angegeben werden
 *
 * Revision 1.23  1995/11/06  18:03:16  akool
 * "-m16" zeigt die Cause im Klartext an
 * Auch Gebuehreneinheiten > 255 werden korrekt ausgewertet
 *
 * Revision 1.22  1995/10/22  14:43:16  akool
 * General cleanup
 * "isdn.log" um 'I' == dialin / 'O' == dialout erweitert
 * Auch nicht zustande gekommene Verbindungen werden (mit cause)
 * protokolliert
 *
 * Revision 1.21  1995/10/18  21:25:16  akool
 * Option "-r" implementiert
 * Charging-Infos waehrend der Verbindung (FACILITY) werden ignoriert
 * "/etc/isdnlog.pid" wird erzeugt
 *
 * Revision 1.20  1995/10/15  17:23:16  akool
 * Volles D-Kanal Protokoll implementiert (fuer Teles-0.4d Treiber)
 *
 * Revision 1.13  1995/09/30  09:34:16  akool
 * Option "-m", Console-Meldung implementiert
 * Flush bei SIGTERM implementiert
 *
 * Revision 1.12  1995/09/29  17:21:13  akool
 * "isdn.log" um Zeiteintrag in UTC erweitert
 *
 * Revision 1.11  1995/09/28  18:51:17  akool
 * First public release
 *
 * Revision 1.1  1995/09/16  16:54:12  akool
 * Initial revision
 *
 */

 /* Letzte Exit-Nummer: 29 */


#define _ISDNLOG_C_
#include "isdnlog.h"


#define LOG_MAJOR_VERSION   2
#define LOG_MINOR_VERSION   0

static char    usage[]   = "%s: usage: %s [ -%s ] file\n";
static char    options[] = "avsx:m:l:rt:cC:w:SVTRDPeMh:";

static char    msg1[]    = "%s: Can't open %s (%s)\n";

static FILE   *flog      = (FILE *)NULL;   /* /var/adm/isdn.log          */
static FILE   *fcons 	 = (FILE *)NULL;   /* /dev/ttyX      (or stderr) */
static FILE   *fprot 	 = (FILE *)NULL;   /* /tmp/isdnctrl0 	 	 */

static int     chan;
static int     message = PRT_NOTHING, syslogmessage = PRT_NOTHING;
static int     xinfo = 0, sound = 0, trace = 0, daemon = 0, imon = 0;
static int     port = 0;
static int     wakeup = 1, fullhour = 0;
static int     tty_dv = 0, net_dv = 0, inf_dv = 0;
static clock_t tt, tto;
static char    st[64], stl[64];
static int     settime = 0, replay = 0, replaydev = 0;
static int     verbose = 0, synclog = 0, any = 1, stdoutput = 0, showtei = 1;
static int     allflags = 0, newcps = 0, chans = 2, huptimeofs = 0;
static IFO     ifo[ISDN_MAX_CHANNELS];
static IO      io[ISDN_MAX_CHANNELS];

extern double  cheap96(time_t when, int zone);


static int Change_Channel(int old_channel, int new_channel)
{
  change_channel(old_channel,new_channel);
  Change_Channel_Ring(old_channel,new_channel);

  return 0;
}

static void set_time_str()
{
  auto struct tm *tm_time = localtime(&cur_time);


  tm_time->tm_isdst = 0;

  strftime(stl, 64, "%a %b %d %X %Y", tm_time);
  strftime(st, 64, "%a %b %d %X", tm_time);

  fullhour = (!tm_time->tm_min && (tm_time->tm_sec <= (wakeup + 2)));
} /* set_time_str */


static void logger(int chan)
{
  register char  *p;
  auto 	   char   s[BUFSIZ];


  strcpy(s, ctime(&call[chan].connect));

  if ((p = strchr(s, '\n')))
    *p = 0;

  fprintf(flog, "%s|%-16s|%-16s|%5d|%10d|%10d|%5d|%c|%3d|%10ld|%10ld|%d.%d|%d|%d|%g|%s|\n",
    s + 4, call[chan].num[CALLING], call[chan].num[CALLED],
    (int)(call[chan].disconnect - call[chan].connect),
    (int)call[chan].duration, (int)call[chan].connect,
    call[chan].aoce, call[chan].dialin ? 'I' : 'O',
    call[chan].cause, call[chan].ibytes, call[chan].obytes,
    LOG_MAJOR_VERSION, LOG_MINOR_VERSION, call[chan].si1, call[chan].si11,
    currency_factor ? currency_factor : 0.12, currency ? currency : "DM");

  fflush(flog);

} /* logger */


static void diag(int cref, int tei, int dialin, int net, int type, int version)
{
  char String[LONG_STRING_SIZE];
  char TmpString[LONG_STRING_SIZE];


  if (dialin != -1) {
    sprintf(String,"  DIAG> %s: %3d/%3d %3d %s %s %s-> ",
      st + 4, cref, cref & 0x7f, tei,
      ((version == VERSION_1TR6) ? "1TR6" : "E-DSS1"),
      dialin ? " IN" : "OUT",
      net ? "NET" : "USR");

    if ((cref > 128) && (type == SETUP_ACKNOWLEDGE)) {
      sprintf(TmpString," *%d* ", cref);
      strcat(String, TmpString);
    } /* if */

  } /* if */

  switch (type) {
    case ALERTING            : sprintf(TmpString, " ALERTING\n");
    	 		       break;

    case CALL_PROCEEDING     : sprintf(TmpString, " CALL PROCEEDING\n");
    	 		       break;

    case SETUP               : sprintf(TmpString, " SETUP\n");
    	 		       break;

    case SETUP_ACKNOWLEDGE   : sprintf(TmpString, " SETUP ACKNOWLEDGE\n");
    	 		       break;

    case CONNECT             : sprintf(TmpString, " CONNECT\n");
    	 		       break;

    case CONNECT_ACKNOWLEDGE : sprintf(TmpString, " CONNECT ACKNOWLEDGE\n");
    	 		       break;

    case FACILITY 	     : sprintf(TmpString, " FACILITY\n");
    	 		       break;

    case NOTIFY 	     : sprintf(TmpString, " NOTIFY\n");
    	 		       break;

    case STATUS 	     : sprintf(TmpString, " STATUS\n");
    	 		       break;

    case DISCONNECT 	     : sprintf(TmpString, " DISCONNECT\n");
    	 		       break;

    case RELEASE 	     : sprintf(TmpString, " RELEASE\n");
    	 		       break;

    case RELEASE_COMPLETE    : sprintf(TmpString, " RELEASE COMPLETE\n");
    	 		       break;

    default 		     : sprintf(TmpString, " TYPE 0x%0x\n", type);
    			       break;
  } /* switch */

  strcat(String, TmpString);
  print_msg(PRT_DEBUG_GENERAL, "%s", String);
} /* diag */


static void info(int chan, int reason, int state, char *msg)
{
  auto char   s[BUFSIZ], *indent;


  if (allflags & PRT_DEBUG_GENERAL)
    if (allflags & PRT_DEBUG_INFO)
      print_msg(PRT_DEBUG_INFO, "%d INFO> ", chan);
    else
      return;

  if (!*msg)
    switch (call[chan].bearer) {
      case 0 : msg = "Digital"; break;
      case 1 : msg = "Analog";  break;
      case 7 : msg = "Digital"; break; /* evtl. _nur_ fuer 1TR6 gueltig? */
    } /* switch */

  switch (chan) {
     case 0 : indent = "";   break;
     case 1 : indent = "  "; break;
    default : indent = "* "; break;
  } /* switch */

  if (call[chan].dialin) {

    if (call[chan].tei && showtei)
      sprintf(s, "%s %sCall to tei %d from %s on %s  %s\n",
         st + 4, indent,
         call[chan].tei,
         call[chan].vnum[CALLING],
         call[chan].vnum[CALLED],
         msg);
    else
      sprintf(s, "%s %sCall from %s on %s  %s\n",
        st + 4, indent,
        call[chan].vnum[CALLING],
        call[chan].vnum[CALLED],
        msg);

  }
  else {
    if (showtei)
      sprintf(s, "%s %stei %d calling %s with %s  %s\n",
     	  st + 4, indent,
      	  call[chan].tei,
      	  call[chan].vnum[CALLED],
      	  call[chan].vnum[CALLING],
      	  msg);
    else
      sprintf(s, "%s %sCalling %s with %s  %s\n",
    	  st + 4, indent,
      	  call[chan].vnum[CALLED],
      	  call[chan].vnum[CALLING],
      	  msg);
  } /* else */

  print_msg(PRT_DEBUG_INFO, "%s", s);

  print_msg(reason, "%s", s);

  if (xinfo) {
    strcpy(call[chan].msg, msg);
    call[chan].stat = state;

    if (state == STATE_HANGUP)
      if (call[chan].dialin || (call[chan].aoce < 0))
        call[chan].aoce = 0;

    message_from_server(&(call[chan]), chan);

    print_msg(PRT_DEBUG_CS, "SOCKET> %s: MSG_CALL_INFO chan=%d\n", st + 4, chan);
  } /* if */
} /* info */


static int ringer(int chan, int event)
{
  register int i, j, c;
  int   ProcessStarted = 0;
  int   old_c = -1;
  info_args *infoarg = NULL;

  print_msg(PRT_DEBUG_EXEC, "Got ring event %d on channel %d, number of sockets: %d\n", event, chan, socket_size(sockets));

  /* chan; Der jeweilige B-Kanal (0 oder 1)

     event:
       RING_RING    = Nummern wurden uebertragen
       RING_CONNECT = Hoerer abgenommen
       RING_HANGUP  = Hoerer aufgelegt
       RING_AOCD    = Gebuehrenimpuls (wird noch nicht verwendet)
  */

  if (event != RING_HANGUP)
  {
    if (event == RING_RING || event == RING_CONNECT)
      call[chan].cur_event = event;
  }
  else
    call[chan].cur_event = 0;

  for (i = CALLING; i <= CALLED; i++) {   /* fuer beide beteilige Telefonnummern */
    c = call[chan].confentry[i];

    if (c != -1 && c != old_c) { /* wenn Eintrag in isdnlog.conf gefunden */
    	old_c = c;

      for (j = 0;known[c]->infoargs != NULL && (infoarg = known[c]->infoargs[j]) != NULL;j++)
        ProcessStarted += Start_Ring(chan,infoarg,event,0);
    }
  }

  if (ProcessStarted == 0)
  {
    for (j = 0;start_procs.infoargs != NULL && (infoarg = start_procs.infoargs[j]) != NULL;j++)
      ProcessStarted += Start_Ring(chan,infoarg,event,0);
  }

  return ProcessStarted;
} /* ringer */


static void hup_handler(int isig)
{
  print_msg(PRT_INFO, "re-reading %s\n", CONFFILE);

  discardconfig();
  readconfig(myname);

  signal(SIGHUP, hup_handler);
} /* hup_handler */


#ifdef OLD_AOC
static int AOC(int l, char *p)
{
  register int   i, c, x = 0;
  auto	   int	 ID = 0, OP = 0, EH = 0;
#if V2
  register char *pc;
  auto 	   char  CUR[16];
#endif


  c = strtol(p += 3, NIL, 16);              /* Ext/Spare/Profile */

  c = strtol(p += 3, NIL, 16);        	    /* Invoke Comp Type */
  c = strtol(p += 3, NIL, 16);        	    /* Invoke Comp Length */
  c = strtol(p += 3, NIL, 16);        	    /* Invoke ID Type */

  l = strtol(p += 3, NIL, 16);        	    /* Invoke ID Length */

  while (l--) {
    ID = ID << 8;
    ID += strtol(p += 3, NIL, 16);    	    /* Invoke ID Contents */
  } /* while */

  c = strtol(p += 3, NIL, 16); 	      	    /* Operation Type */

  l = strtol(p += 3, NIL, 16); 	      	    /* Operation Length */

  while (l--) {
    OP = OP << 8;
    OP += strtol(p += 3, NIL, 16);    	    /* Operation Content */
  } /* while */

  c = strtol(p += 3, NIL, 16); 	      	    /* Sequence Type */

  l = strtol(p += 3, NIL, 16);        	    /* Sequence Length */

  if (!l)      	     	  	      	    /* VSt Giessen meldet so: */
    return(AOC_130);                        /* "Free of charge" */

  if (OP == 0x22) {

#if V2
    c = strtol(p += 3, NIL, 16);      	    /* Recorded Currency Type */
    l = strtol(p += 3, NIL, 16);      	    /* Recorded Currency Length */

    c = strtol(p += 3, NIL, 16);      	    /* Currency Type */
    l = strtol(p += 3, NIL, 16);            /* Currency Length */

    pc = CUR;

    while (l--) {
      *pc++ = (char)strtol(p += 3, NIL, 16) /* Currency Contents */
    } /* while */

    c = strtol(p += 3, NIL, 16);       	    /* Amount Type */
    l = strtol(p += 3, NIL, 16);      	    /* Amount Length */

    while (l--) {
      EH = EH << 8;
      EH += strtol(p += 3, NIL, 16);        /* Amount */
    } /* while */

#else

    for (i = 0; i < 5; i++)            	    /* 5 Byte unknown */
      c = strtol(p += 3, NIL, 16); 	    /* unknown */

    l = strtol(p += 3, NIL, 16);      	    /* length */

    while (l--) {
      EH = EH << 8;
      EH += strtol(p += 3, NIL, 16);
    } /* while */

    c = strtol(p += 3, NIL, 16); 	    /* unknown */

    l = strtol(p += 3, NIL, 16);      	    /* length */

    while (l--) {
      x = x << 8;
      x += strtol(p += 3, NIL, 16);
    } /* while */

    if (x)       /* AOC-E */
      EH = EH;
    else         /* AOC-D */
      EH = -EH;
#endif

  }
  else if (OP == 0x24) {
    for (i = 0; i < 7; i++) {          	    /* 7 Byte unknown */
      c = strtol(p += 3, NIL, 16); 	    /* unknown */

      if ((i == 1) && !c)     		    /* so melden andere VSt */
        return(AOC_130);		    /* "Free of charge" */
    } /* for */

    l = strtol(p += 3, NIL, 16);      	    /* length */

    while (l--) {
      EH = EH << 8;
      EH += strtol(p += 3, NIL, 16);
    } /* while */

  }
  else if (OP == 0x21) {   		    /* AOCD Oesterreich */
    for (i = 0; i < 2; i++)                 /* 2 Byte unknown */
      c = strtol(p += 3, NIL, 16);          /* unknown */

    c = strtol(p += 3, NIL, 16);            /* Currency Type */
    l = strtol(p += 3, NIL, 16);            /* Currency Length */

    while (l--)
      c = strtol(p += 3, NIL, 16);          /* Currency Contents */

    for (i = 0; i < 3; i++)                 /* 3 Byte unknown */
      c = strtol(p += 3, NIL, 16);          /* unknown */

    l = strtol(p += 3, NIL, 16);            /* Amount Length */

    while (l--) {
      EH = EH << 8;
      EH += strtol(p += 3, NIL, 16);        /* Amount */
    } /* while */

    EH = -EH; /* AOC-D */

  }
  else if (OP == 0x23) {                    /* AOCE Oesterreich */
    for (i = 0; i < 4; i++)                 /* 4 Byte unknown */
      c = strtol(p += 3, NIL, 16);          /* unknown */

    c = strtol(p += 3, NIL, 16);            /* Currency Type */
    l = strtol(p += 3, NIL, 16);            /* Currency Length */

    while (l--)
      c = strtol(p += 3, NIL, 16);          /* Currency Contents */

    for (i = 0; i < 3; i++)                 /* 3 Byte unknown */
      c = strtol(p += 3, NIL, 16);          /* unknown */

    l = strtol(p += 3, NIL, 16);            /* Amount Length */

    while (l--) {
      EH = EH << 8;
      EH += strtol(p += 3, NIL, 16);        /* Amount */
    } /* while */

  } /* else */

  return(EH);
} /* AOC */

#else

static void aoc_debug(char *s)
{
  print_msg(PRT_DEBUG_DECODE, " DEBUG> %s: %s\n", st + 4, s);
} /* aoc_debug */


static int AOC(int l, char *p, int type)
{
  int   c, ret = 0;
  static int ID = 0, OP = 0, EH = 0, MP = 0;

  switch(type) {
    case AOC_INITIAL:
      aoc_debug("AOC_INITIAL");
      ID = OP = EH = MP = 0;
      c = strtol(p += 3, NIL, 16);              /* Ext/Spare/Profile */
      ret += 3;

      c = strtol(p += 3, NIL, 16);              /* Invoke Comp type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);              /* Invoke Comp length */
      ret += 3;

      c = strtol(p += 3, NIL, 16);              /* Invoke ID type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);              /* Invoke ID length */
      ret += 3;

      while (l--) {
        ID = ID << 8;
        ID += strtol(p += 3, NIL, 16);          /* Invoke ID Contents */
        ret += 3;
      } /* while */

      c = strtol(p += 3, NIL, 16);              /* OPERATION type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);              /* OPERATION length */
      ret += 3;

      while (l--) {
        OP = OP << 8;
        OP += strtol(p += 3, NIL, 16);          /* OPERATION contents */
        ret += 3;
      } /* while */

      switch (OP) {
        case 0x21:
          c = AOC(l, p, AOCDCurrency);
          p += c;
          EH = -EH; /* AOC-D */
          ret = EH;
          break;

        case 0x22:
          c = AOC(l, p, AOCDChargingUnit);
          p += c;
          ret = -EH;
          break;

        case 0x23:
          c = AOC(l, p, AOCECurrency);
          p += c;
          ret = EH;
          break;

        case 0x24:
          c = AOC(l, p, AOCEChargingUnit);
          p += c;
          ret = -EH;
          break;

      }
      break;

    case AOCDCurrency:
      aoc_debug("AOCDCurrency");
      c = AOC(l, p, AOCDCurrencyInfo);
      p += c;
      ret += c;
      break;

    case AOCDChargingUnit:
      aoc_debug("AOCDChargingUnit");
      c = AOC(l, p, AOCDChargingUnitInfo);
      p += c;
      ret += c;
      break;

    case AOCECurrency:
      aoc_debug("AOCECurrency");
      c = AOC(l, p, AOCECurrencyInfo);
      p += c;
      ret += c;
      break;

    case AOCEChargingUnit:
      aoc_debug("AOCEChargingUnit");
      c = AOC(l, p, AOCEChargingUnitInfo);
      p += c;
      ret += c;
      break;

    case AOCDChargingUnitInfo:
      aoc_debug("AOCDChargingUnitInfo");
      c = strtol(p += 3, NIL, 16);  /* specificChargingUnits SEQUENCE type / NULL */
      ret += 3;
      l = strtol(p += 3, NIL, 16);  /* specificChargingUnits SEQUENCE length / NULL */
      ret += 3;

      if (!l)                                 /* freeOfCharge */
        return(AOC_130);

      c = AOC(l, p, RecordedUnitsList);
      p += c;
      ret += c;
      c = AOC(l, p, TypeOfChargingInfo);
      p += c;
      ret += c;
      /* c = AOC(l, p, AOCDBillingId);
         p += c;
         ret += c; */
      break;

    case AOCEChargingUnitInfo:
      aoc_debug("AOCEChargingUnitInfo");
      c = strtol(p += 3, NIL, 16);            /* SEQUENCE type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* SEQUENCE length */
      ret += 3;

      c = strtol(p += 3, NIL, 16);  /* specificChargingUnits SEQUENCE type / NULL */
      ret += 3;
      l = strtol(p += 3, NIL, 16);  /* specificChargingUnits SEQUENCE length / NULL */
      ret += 3;

      if (!l)                                 /* freeOfCharge */
        return(AOC_130);

      c = AOC(l, p, RecordedUnitsList);
      p += c;
      ret += c;
      c = AOC(l, p, TypeOfChargingInfo);
      p += c;
      ret += c;
      /* c = AOC(l, p, AOCDBillingId);
         p += c;
         ret += c; */
      break;

    case AOCDCurrencyInfo:
      aoc_debug("AOCDCurrencyInfo");
      c = strtol(p += 3, NIL, 16);  /* specificCurrency SEQUENCE type / NULL */
      ret += 3;
      l = strtol(p += 3, NIL, 16);  /* specificCurrency SEQUENCE length / NULL */
      ret += 3;

      if (!l)                                 /* freeOfCharge */
        return(AOC_130);

      c = strtol(p += 3, NIL, 16);            /* SEQUENCE type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* SEQUENCE length */
      ret += 3;

      c = AOC(l, p, RecordedCurrency);
      p += c;
      ret += c;
      c = AOC(l, p, TypeOfChargingInfo);
      p += c;
      ret += c;
      /* c = AOC(l, p, AOCDBillingId);
         p += c;
         ret += c; */
      break;

    case AOCECurrencyInfo:
      aoc_debug("AOCECurrencyInfo");
      c = strtol(p += 3, NIL, 16);            /* SEQUENCE type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* SEQUENCE length */
      ret += 3;

      c = strtol(p += 3, NIL, 16);  /* specificCurrency SEQUENCE type / NULL */
      ret += 3;
      l = strtol(p += 3, NIL, 16);  /* specificCurrency SEQUENCE length / NULL */
      ret += 3;

      if (!l)                                 /* freeOfCharge */
        return(AOC_130);

      c = strtol(p += 3, NIL, 16);            /* SEQUENCE type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* SEQUENCE length */
      ret += 3;

      c = AOC(l, p, RecordedCurrency);
      p += c;
      ret += c;
      /* c = AOC(l, p, AOCDBillingId);
         p += c;
         ret += c; */
      /* c = AOC(l, p, ChargingAssociation);
         p += c;
         ret += c; */
      break;

    case RecordedCurrency:
      aoc_debug("RecordedCurrency");
      c = strtol(p += 3, NIL, 16);            /* IA5String type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* IA5String length */
      ret += 3;

      c = AOC(l, p, Currency);
      p += c;
      ret += c;

      c = strtol(p += 3, NIL, 16);            /* SEQUENCE type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* SEQUENCE length */
      ret += 3;

      c = AOC(l, p, Amount);
      p += c;
      ret += c;
      break;

    case Currency:
      aoc_debug("Currency");
      currency = (char *) malloc(l + 1);
      c = 0;
      while (l--) {
        currency[c++] = strtol(p += 3, NIL, 16);
        ret += 3;
      } /* while */
      currency[c] = 0;
      break;

    case Amount:
      aoc_debug("Amount");
      c = strtol(p += 3, NIL, 16);            /* INTEGER type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* INTEGER length */
      ret += 3;

      c = AOC(l, p, CurrencyAmount);
      p += c;
      ret += c;

      c = strtol(p += 3, NIL, 16);            /* ENUMERATED type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* ENUMERATED length */
      ret += 3;

      c = AOC(l, p, Multiplier);
      p += c;
      ret += c;
      break;

    case CurrencyAmount:
      aoc_debug("CurrencyAmount");
      while (l--) {
        EH = EH << 8;
        EH += strtol(p += 3, NIL, 16);        /* Amount */
        ret += 3;
      } /* while */
      break;

    case Multiplier:
      aoc_debug("Multiplier");
      while (l--) {
        MP = MP << 8;
        MP += strtol(p += 3, NIL, 16);        /* Multiplier */
        ret += 3;
      } /* while */
      switch (MP) {
        case 0:
          currency_factor = 0.001;
          break;
        case 1:
          currency_factor = 0.01;
          break;
        case 2:
          currency_factor = 0.1;
          break;
        case 3:
          currency_factor = 1.0;
          break;
        case 4:
          currency_factor = 10.0;
          break;
        case 5:
          currency_factor = 100.0;
          break;
        case 6:
          currency_factor = 1000.0;
          break;
      }
      break;

    case RecordedUnitsList:
      aoc_debug("RecordedUnitsList");
      c = strtol(p += 3, NIL, 16);            /* SEQUENCE SIZE type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* SEQUENCE SIZE length */
      ret += 3;

      c = AOC(l, p, RecordedUnits);
      p += c;
      ret += c;
      break;

    case RecordedUnits:
      aoc_debug("RecordedUnits");
      c = strtol(p += 3, NIL, 16);            /* SEQUENCE type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* SEQUENCE length */
      ret += 3;

      if (!l)                                 /* notAvailable */
        return(AOC_130);

      c = AOC(l, p, NumberOfUnits);
      p += c;
      ret += c;

      break;

   case NumberOfUnits:
      aoc_debug("NumberOfUnits");
      c = strtol(p += 3, NIL, 16);            /* INTEGER type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* INTEGER length */
      ret += 3;

      while (l--) {
        EH = EH << 8;
        EH += strtol(p += 3, NIL, 16);        /* NumberOfUnits */
        ret += 3;
      } /* while */

      break;

    case TypeOfChargingInfo:
      aoc_debug("TypeOfChargingInfo");
      c = strtol(p += 3, NIL, 16);            /* typeOfChargingInfo type */
      ret += 3;
      l = strtol(p += 3, NIL, 16);            /* typeOfChargingInfo length */
      ret += 3;

      c = strtol(p += 3, NIL, 16);            /* typeOfChargingInfo contents */
      ret += 3;
      break;
  }
  return(ret);
}
#endif


static int AOC_1TR6(int l, char *p)
{
  auto int EH = 0;
  auto int digit = 0;


  aoc_debug("AOC_INITIAL_1TR6");

  while (l--) {
    digit = strtol(p += 3, NIL, 16) ;

    if ((digit >= '0') && (digit <= '9')) {
      EH = EH * 10;
      EH += (digit - '0'); /* Einheiten sind in ASCII */
    } /* if */
  } /* while */

  return(EH);
} /* AOC_1TR6 */


static void buildnumber(char *num, int type, char *result, int version)
{
  strcpy(result, "");


  switch (type & 0x70) { /* Calling party number Information element, Octet 3 - Table 4-11/Q.931 */
    case 0x00 : if (*num && (*num != '0'))  /* 000 Unknown */
    	      	  strcpy(result, myprefix);
                break;

    case 0x10 : if (version != VERSION_1TR6)
    	          strcpy(result, "00");     /* 001 International */
    	      	break;

    case 0x20 : if (version != VERSION_1TR6)
    	      	  strcpy(result, "0");      /* 010 National */
    	      	break;

    case 0x30 : break;         		    /* 011 Network specific number */

    case 0x40 :	strcpy(result, myprefix);   /* 100 Subscriber number */
    	      	break;

    case 0x60 :	break;	       		    /* 110 Abbreviated number */

    case 0x70 :	break;	      		    /* 111 Reserved for extension */
  } /* switch */

  strcat(result, num);

  print_msg(PRT_DEBUG_DECODE, " DEBUG> %s: num=\"%s\", type=%s, result=\"%s\"\n",
    st + 4, num, i2a(type, 8, 2), result);
} /* buildnumber */


static void decode(int chan, register char *p, int type, int version)
{
  register char     *pd, *px;
  register int       i, element, l, c, oc, sxp = 0;
  auto	   char	     s[BUFSIZ];
  auto	   char	     sx[10][BUFSIZ];
  auto 	   struct tm tm;
  auto	   double    tx, err, tack;


  while (1) {

    if (!*(p + 2))
      break;

    element = strtol(p += 3, NIL, 16);

    if (element < 128) {

      l = strtol(p += 3, NIL, 16);

      switch (element) {
        case 0x08 : /* Cause */
                    if (version == VERSION_1TR6) {
                      switch (l)  {
                         case 0 : call[chan].cause = 0;
                               	  break;

                         case 1 : call[chan].cause = strtol(p += 3, NIL, 16);
                                  break;

                         case 2 : call[chan].cause = strtol(p += 3, NIL, 16);
                               	  c = strtol(p += 3, NIL, 16); /* Sometimes it 0xc4 or 0xc5 */
                         	  break;

                        default : p += (l * 3);
                         	  print_msg(PRT_ERR,"Wrong Cause (more than two bytes)");
                         	  break;
                      } /* switch l */

		      info(chan, PRT_SHOWCAUSE, STATE_CAUSE, tr6cause(call[chan].cause));

		      if (sound) {
                      	if (call[chan].cause == 0xbb) /* "User busy" */
                          ringer(chan, RING_BUSY);
                      	else
                          ringer(chan, RING_ERROR);
                      } /* if */
                    }
                    else { /* E-DSS1 */
                      c = strtol(p += 3, NIL, 16);

                      call[chan].cause = strtol(p += 3, NIL, 16) & 0x7f;

                      if ((call[chan].cause != 0x10) && /* "Normal call clearing" */
                          (call[chan].cause != 0x1a) && /* "non-selected user clearing" */
                          (call[chan].cause != 0x1f) && /* "Normal, unspecified" */
                          (call[chan].cause != 0x51)) { /* "Invalid call reference value" <- dies nur Aufgrund eines Bug im Teles-Treiber! */
		        info(chan, PRT_SHOWCAUSE, STATE_CAUSE, CauseValue[call[chan].cause]);

                    	if (sound) {
                      	  if (call[chan].cause == 0x11) /* "User busy" */
                            ringer(chan, RING_BUSY);
                      	  else
                            ringer(chan, RING_ERROR);
                    	} /* if */
                      } /* if */
                    } /* else */

                    break;

        case 0x28 : /* Makelweg (wie die Funktion richtig heisst, weiss ich nicht */
                    p += (l * 3);
                    break;

        case 0x2d : /* Parkweg */
                    p += (l * 3);
                    break;

        case 0x2e : /* Parkran */
                    p += (l * 3);
                    /* ggf. neuer Channel kommt gleich mit */
                    break;

        case 0x33 : /* Makelran */
                    p += (l * 3);
                    /* ggf. neuer Channel kommt gleich mit */
                    break;

        case 0x02 : /* Facility AOCE on 1TR6 */
        case 0x1c : /* Facility AOC-D/AOC-E on E-DSS1 */

        	    if (allflags & PRT_DEBUG_DECODE) {
          	      register char *p1 = p;
                      register int   i;
                      auto     char  String[LONG_STRING_SIZE];


                      sprintf(String, " DEBUG> %s: FACILITY:", st + 4);

                      for (i = 0; i < l; i++)
                        sprintf(String+strlen(String), " 0x%02x", (int)strtol(p1 += 3, NIL, 16));

                      print_msg(PRT_DEBUG_DECODE, "%s\n", String);

        	    } /* if */

#ifdef OLD_AOC
	            call[chan].aoce = AOC(l, p);
#else
                    {
                      int n;


                      if ((element == 0x02) && (version == VERSION_1TR6)) {
                        n = AOC_1TR6(l, p);           /* Wieviele Einheiten? */

                        if (type == AOCD_1TR6) {
                          n = -n;  /* Negativ: laufende Verbindung */
                                   /* ansonsten wars ein AOCE */
                          print_msg(PRT_DEBUG_DECODE, " DEBUG> %s: 1TR6 AOCD %i\n", st + 4, n);
                        } /* if */
                      }
                      else
                        n = AOC(l, p, AOC_INITIAL); /* E-DSS1 */

                      /* Dirty-Hack fuer VSt Giessen:
                      	 Diese meldet auch AOC-E als AOC-D */

                      if ((type != FACILITY) && (type != AOCD_1TR6) && (n < 0) ) {
		        aoc_debug("DIRTY-HACK: AOC-D -> AOC-E");
                        n = -n;
                      } /* if */

                      call[chan].aoce = n;

                      sprintf(s, "AOC=%d", n);
		      aoc_debug(s);
                    }
#endif

		    if (call[chan].aoce == AOC_130) {
                      info(chan, PRT_SHOWAOCD, STATE_AOCD, "Free of charge");
                      call[chan].aoce = 0;
		    }
		    else if (call[chan].aoce < 0) {
                      if (currency_factor)
                        sprintf(s, "%s %s (%d s)",
  	                  double2str(-call[chan].aoce * currency_factor, 6, 2, DEB),
                          currency,
                          (int)(cur_time - call[chan].connect));
                      else {

                        tx = cur_time - call[chan].connect;

                        if ((c = call[chan].confentry[OTHER]) > -1) {
                          tack = cheap96(cur_time, known[c]->zone);
                          err  = call[chan].tick - tx;
                          call[chan].tick += tack;

			  sprintf(s, "%d.EH DM %s (%s %d) C=%s",
			    -call[chan].aoce,
			    double2str(-call[chan].aoce * einheit, 6, 2, DEB),
			    tx ? double2clock(tx) : "", (int)err,
			    double2clock(call[chan].tick - tx) + 4);
                        }
                        else if (-call[chan].aoce > 1) { /* try to guess Gebuehrenzone */
                          tack = 0;
                          err = 0;
                          px = "";

                          for (c = 4; c > 0; c--) {
                            call[chan].tick = 0;

                            for (i = 0; i < -call[chan].aoce - 1; i++) {
                              tack = cheap96(cur_time, c);
                              call[chan].tick += tack;
                            } /* for */

                            err = call[chan].tick - tx;

			    if (err >= 0) {
                              switch (c) {
                                case 4 : px = "Fern";      break;
                                case 3 : px = "Regio 200"; break;
                                case 2 : px = "Regio 50";  break;
                                case 1 : px = "City"; 	   break;
                              } /* switch */

                              break;
			    } /* if */
                          } /* for */

			  sprintf(s, "%d.EH DM %s (%s %d %s?) C=%s",
			    -call[chan].aoce,
			    double2str(-call[chan].aoce * einheit, 6, 2, DEB),
			    tx ? double2clock(tx) : "", (int)err, px,
			    double2clock(call[chan].tick - tx) + 4);
                        }
                        else {
                          sprintf(s, "%d.EH DM %s",
                            -call[chan].aoce,
  	                    double2str(-call[chan].aoce * einheit, 6, 2, DEB));
                        } /* else */
                      } /* else */

                      info(chan, PRT_SHOWAOCD, STATE_AOCD, s);

                      if (sound)
                        ringer(chan, RING_AOCD);
		    } /* else */

                    p += (l * 3);
                    break;

        case 0x29 : /* Date/Time */
             	    tm.tm_year  = strtol(p += 3, NIL, 16);
	     	    tm.tm_mon   = strtol(p += 3, NIL, 16) - 1;
		    tm.tm_mday  = strtol(p += 3, NIL, 16);
		    tm.tm_hour  = strtol(p += 3, NIL, 16);
		    tm.tm_min   = strtol(p += 3, NIL, 16);

		    tm.tm_sec   = 0;
		    tm.tm_wday  = tm.tm_yday = 0;
		    tm.tm_isdst = -1;

                    call[chan].time = mktime(&tm);

                    if (settime) {
  		      auto time_t     tn;
  		      auto struct tms tms;

	      	      time(&tn);

                      if (labs(tn - call[chan].time) > 61) {
       			(void)stime(&call[chan].time);

                        /* Nicht gerade sauber, sollte aber all zu
                           grosse Spruenge verhindern! */

                        if (replay)
                	  cur_time = tt = tto = call[chan].time;
                        else {
      			  time(&cur_time);
      			  tt = tto = times(&tms);
                        } /* else */

              		set_time_str();

                      } /* if */

                      if (settime == 1)
                        settime--;
		    } /* if */

                    sprintf(s, "Time:%s", ctime(&call[chan].time));
                    if ((px = strchr(s, '\n')))
                      *px = 0;

                    info(chan, PRT_SHOWTIME, STATE_TIME, s);
                    break;

        case 0x6c : /* Calling party number */
             	    oc = strtol(p += 3, NIL, 16);

                    if (oc < 128) {
                      c = strtol(p += 3, NIL, 16);
                      l--;
                    } /* if */

                    pd = s;

                    while (--l)
                      *pd++ = strtol(p += 3, NIL, 16);

                    *pd = 0;

                    strcpy(call[chan].onum[CALLING], s);
                    buildnumber(s, oc, call[chan].num[CALLING], version);

		    strcpy(call[chan].vnum[CALLING], vnum(chan, CALLING));
              	    break;


        case 0x70 : /* Called party number */
             	    oc = strtol(p += 3, NIL, 16);

             	    pd = s;

                    while (--l)
                      *pd++ = strtol(p += 3, NIL, 16);

                    *pd = 0;

                    strcpy(call[chan].onum[CALLED], s);
                    buildnumber(s, oc, call[chan].num[CALLED], version);

		    strcpy(call[chan].vnum[CALLED], vnum(chan, CALLED));

                    /* This message comes before bearer capability */
                    /* So dont show it here, show it at Bearer capability */

		    if (version != VERSION_1TR6) {
                      sprintf(s, "Service:");

                      switch (call[chan].si1) {
                        case 1 : sprintf(s + strlen(s), "Speech, %s Network (%d)", (call[chan].si11 ? "Digital" : "Analog"), call[chan].si1);
                                 break;
                        case 4 : sprintf(s + strlen(s), "Video (%d)", call[chan].si1);
                                 break;
                        case 5 : sprintf(s + strlen(s), "BTX (%d)", call[chan].si1);
                                 break;
                        case 7 : sprintf(s + strlen(s), "Data (%d)", call[chan].si1);
                                 break;
                       default : sprintf(s + strlen(s), "Unknown (%d)", call[chan].si1);
                                 break;
                      } /* switch */

                      info(chan, PRT_SHOWNUMBERS, STATE_RING, s);

                      for (c = 0; c <= sxp; c++)
                        if (*sx[c])
                          info(chan, PRT_SHOWNUMBERS, STATE_RING, sx[c]);

                      if (sound)
                        ringer(chan, RING_RING);
                    } /* if */
              	    break;


        case 0x01 : /* Bearer capability 1TR6 */
                    switch (l)  {
                       case 0 : call[chan].bearer = 1; /* Analog */
                              	break;
                       case 1 : call[chan].bearer = strtol(p += 3, NIL, 16);
                              	break;
                       case 2 : call[chan].bearer = strtol(p += 3, NIL, 16);
                              	c = strtol(p += 3, NIL, 16);
                         	print_msg(PRT_DEBUG_DECODE, " DEBUG> %s: Bearer=%i\n",
                                                         st + 4, call[chan].bearer);
                                break;
                      default : p += (l * 3);
                              	print_msg(PRT_ERR,"Wrong Bearer (more than two bytes)");
                         	break;
                    } /* switch l */

                    info(chan, PRT_SHOWNUMBERS, STATE_RING, "");

                    if (sound)
                      ringer(chan, RING_RING);

                    break;


        case 0x04 : /* Bearer capability E-DSS1 */

             	    pd = p + (l * 3);
		    sxp = 0;

                    c = strtol(p += 3, NIL, 16);       /* Octet 3 */
                    px = sx[sxp];

		    /* Mapping from Octet 3 to i4l's Service Indicator:
		     *
		     *  0 = Unknown
		     *  1 = Speech
		     *  2 = Restricted digital information
		     *  3 = Unrestricted digital information
		     *      with tones/announcements
		     *  4 = Video
		     *  5 = BTX (?)
		     *  7 = Unrestricted digital information
		     */

                    switch (c & 0x1f) {
                      case 0x00 : px += sprintf(px, "Speech");
                                  call[chan].si1 = 1;
                                  call[chan].si11 = 1;
                             	  break;

                      case 0x08 : px += sprintf(px, "Unrestricted digital information");
                                  call[chan].si1 = 7;
                                  call[chan].si11 = 0;
                             	  break;

                      case 0x09 : px += sprintf(px, "Restricted digital information");
                                  call[chan].si1 = 2;
                                  call[chan].si11 = 0;
                             	  break;

                      case 0x10 : px += sprintf(px, "3.1 kHz audio");
                                  call[chan].si1 = 1;
                                  call[chan].si11 = 0;
                             	  break;

                      case 0x11 : px += sprintf(px, "Unrestricted digital information with tones/announcements");
                                  call[chan].si1 = 3;
                                  call[chan].si11 = 0;
                             	  break;

                      case 0x18 : px += sprintf(px, "Video");
                                  call[chan].si1 = 4;
                                  call[chan].si11 = 0;
                             	  break;
                    } /* switch */

                    switch (c & 0x60) {
                      case 0x00 : px += sprintf(px, ", CCITT standardized coding"); 	   break;
                      case 0x20 : px += sprintf(px, ", ISO/IEC"); 	 		   break;
                      case 0x40 : px += sprintf(px, ", National standard"); 		   break;
                      case 0x60 : px += sprintf(px, ", Standard defined for the network"); break;
                    } /* switch */

                    c = strtol(p += 3, NIL, 16);       /* Octet 4 */
                    px = sx[++sxp];

                    switch (c & 0x1f) {
                      case 0x10 : px += sprintf(px, "64 kbit/s");     break;
                      case 0x11 : px += sprintf(px, "2 * 64 kbit/s"); break;
                      case 0x13 : px += sprintf(px, "384 kbit/s");    break;
                      case 0x15 : px += sprintf(px, "1536 kbit/s");   break;
                      case 0x17 : px += sprintf(px, "1920 kbit/s");   break;

                      case 0x18 : oc = strtol(p += 3, NIL, 16); /* Octet 4.1 */
                                  px += sprintf(px, ", %d kbit/s", 64 * oc & 0x7f);
                                  break;
                    } /* switch */

                    switch (c & 0x60) {
                      case 0x00 : px += sprintf(px, ", Circuit mode"); break;
                      case 0x40 : px += sprintf(px, ", Packet mode");  break;
                    } /* switch */


                    c = strtol(p += 3, NIL, 16);

		    if ((c & 0x60) == 0x20) { /* User information layer 1 */
		      int ch = ' ';

                      do {
                        switch (ch) {
                          case ' ' : px = sx[++sxp]; /* Octet 5 */

                            	     switch (c & 0x1f) {
                    	      	       case 0x01 : px += sprintf(px, "CCITT standardized rate adaption V.110/X.30");               break;
                    	      	       case 0x02 : px += sprintf(px, "G.711 u-law"); 	      	       				   break;
                    	      	       case 0x03 : px += sprintf(px, "G.711 A-law"); 						   break;
                    	      	       case 0x04 : px += sprintf(px, "G.721 32 kbit/s ADPCM (I.460)"); 				   break;
                    	      	       case 0x05 : px += sprintf(px, "H.221/H.242");  	    					   break;
                    	      	       case 0x07 : px += sprintf(px, "Non-CCITT standardized rate adaption"); 			   break;
                    	      	       case 0x08 : px += sprintf(px, "CCITT standardized rate adaption V.120");                    break;
                    	      	       case 0x09 : px += sprintf(px, "CCITT standardized rate adaption X.31, HDLC flag stuffing"); break;
                            	     } /* switch */

                          	     break;

                          case 'a' : px = sx[++sxp]; /* Octet 5a */

                                     switch (c & 0x1f) {
                                       case 0x01 : px += sprintf(px, "0.6 kbit/s"); break;
                                       case 0x02 : px += sprintf(px, "1.2 kbit/s"); break;
                                       case 0x03 : px += sprintf(px, "2.4 kbit/s"); break;
                                       case 0x04 : px += sprintf(px, "3.6 kbit/s"); break;
                                       case 0x05 : px += sprintf(px, "4.8 kbit/s"); break;
                                       case 0x06 : px += sprintf(px, "7.2 kbit/s"); break;
                                       case 0x07 : px += sprintf(px, "8 kbit/s"); break;
                                       case 0x08 : px += sprintf(px, "9.6 kbit/s"); break;
                                       case 0x09 : px += sprintf(px, "14.4 kbit/s"); break;
                                       case 0x0a : px += sprintf(px, "16 kbit/s"); break;
                                       case 0x0b : px += sprintf(px, "19.2 kbit/s"); break;
                                       case 0x0c : px += sprintf(px, "32 kbit/s"); break;
				       case 0x0e : px += sprintf(px, "48 kbit/s"); break;
                                       case 0x0f : px += sprintf(px, "56 kbit/s"); break;
                                       case 0x15 : px += sprintf(px, "0.1345 kbit/s"); break;
                                       case 0x16 : px += sprintf(px, "0.100 kbit/s"); break;
                                       case 0x17 : px += sprintf(px, "0.075/1.2 kbit/s"); break;
                                       case 0x18 : px += sprintf(px, "1.2/0.075 kbit/s"); break;
                                       case 0x19 : px += sprintf(px, "0.050 kbit/s"); break;
                                       case 0x1a : px += sprintf(px, "0.075 kbit/s"); break;
                                       case 0x1b : px += sprintf(px, "0.110 kbit/s"); break;
                                       case 0x1c : px += sprintf(px, "0.150 kbit/s"); break;
                                       case 0x1d : px += sprintf(px, "0.200 kbit/s"); break;
                                       case 0x1e : px += sprintf(px, "0.300 kbit/s"); break;
                                       case 0x1f : px += sprintf(px, "12 kbit/s"); break;
                                     } /* switch */

                            	     switch (c & 0x40) {
				       case 0x00 : px += sprintf(px, ", Synchronous"); break;
                                       case 0x40 : px += sprintf(px, ", Asynchronous"); break;
                            	     } /* switch */

                                     switch (c & 0x20) {
                                       case 0x00 : px += sprintf(px, ", In-band negotiation not possible"); break;
                                       case 0x20 : px += sprintf(px, ", In-band negotiation possible"); break;
                                     } /* switch */

                                     break;

                          case 'b' : px = sx[++sxp]; /* Octet 5b */

                                     switch (c & 0x60) {
                                       case 0x20 : px += sprintf(px, "8 kbit/s");  break;
                                       case 0x40 : px += sprintf(px, "16 kbit/s"); break;
                                       case 0x60 : px += sprintf(px, "32 kbit/s"); break;
                                     } /* switch */

                                     break;
                        } /* switch */

                        ch = (ch == ' ') ? 'a' : ch + 1;
                      	c = strtol(p += 3, NIL, 16);
                      } while (!(c & 0x80));
                    } /* if */

		    if ((c & 0x60) == 0x40) { /* User information layer 2 */
		      px = sx[++sxp];

                      switch (c & 0x1f) {
                        case 0x02 : px += sprintf(px, "Q.931/I.441");        break;
                        case 0x06 : px += sprintf(px, "X.25, packet layer"); break;
                      } /* switch */

                      c = strtol(p += 3, NIL, 16);
		    } /* if */

		    if ((c & 0x60) == 0x60) { /* User information layer 3 */
		      px = sx[++sxp];

                      switch (c & 0x1f) {
                        case 0x02 : px += sprintf(px, "Q.931/I.451");        break;
                        case 0x06 : px += sprintf(px, "X.25, packet layer"); break;
                      } /* switch */

                    } /* if */

                    p = pd;

             	    break;


        case 0x18 : /* Channel identification */
             	    if (strtol(p + 3, NIL, 16) == 138) {
                      call[chan].channel = 2;
		      /* Bei mir kommt hier tatsaechlich der B-Kanal! Beim Selbstanruf
   		      	 werden natuerlich beide belegt... jetzt eine 1 fuer Kanal
   			 1 und 2 fuer die 2. 0 heisst unbekannt. chan muss dann spaeter
   			 auf channel - 1 gesetzt werden
   			 Beim Parken bleibt der Kanal belegt (bei mir jedenfalls)
   			 und neue Verbindungen kriegen vom Amt den anderen */
                      print_msg(PRT_DEBUG_DECODE, " DEBUG> %s: CHANNEL=Selbstanruf\n", st + 4);

                    }
                    else
                       call[chan].channel = 1; /* Vielleicht kann man das nutzen */
        	    p += (l * 3);
             	    break;


        case 0x1e : /* Progress indicator */
                    if (allflags & PRT_DEBUG_DECODE) {
                      register int a = strtol(p + 3, NIL, 16);
                      register int b = strtol(p + 6, NIL, 16);
                      register int x = 0;


                      if ((a == 130) && (b == 129))
                        x = 1;
                      else if ((a == 130) && (b == 131))
                        x = 2;
                      else if ((a == 130) && (b == 136))
		        x = 3;
                      else if ((a == 132) && (b == 130))
		        x = 4;
                      else if ((a == 132) && (b == 131))
		      	x = 5;
                      else if ((a == 133) && (b == 130))
		      	x = 6;
                      else if ((a == 138) && (b == 129))
		      	x = 7;
                      else if ((a == 138) && (b == 136))
		      	x = 8;

                      print_msg(PRT_DEBUG_DECODE, " DEBUG> %s: PROGRESS=%d\n", st + 4, x);
                    } /* if */
        	    p += (l * 3);
             	    break;


        case 0x7d : /* High layer compatibility */
                      print_msg(PRT_DEBUG_DECODE, " DEBUG> %s: HIGH LAYER=Phone\n", st + 4);
        	    p += (l * 3);
             	    break;


        default   : if (allflags & PRT_DEBUG_DECODE) {
          	      register char *p1 = p;
        	      register int   i;
                      auto     char  String[LONG_STRING_SIZE];


                      sprintf(String, " DEBUG> %s: ELEMENT=0x%02x :", st + 4, element);

                      for (i = 0; i < l; i++)
                        sprintf(String, " 0x%02x", (int)strtol(p1 += 3, NIL, 16));

                      print_msg(PRT_DEBUG_DECODE, "%s\n", String);
        	    } /* if */

        	    p += (l * 3);
              	    break;
      } /* switch */

    } /* if */
  } /* while */
} /* decode */


int print_msg(int Level, const char *fmt, ...)
{
  /* ACHTUNG IN DIESER FKT DARF KEIN print_msg() AUFGERUFEN WERDEN !!!!! */
  auto int     SLevel = PRT_NOTHING;
  auto char    String[LONG_STRING_SIZE];
  auto va_list ap;


  va_start(ap, fmt);
  (void)vsnprintf(String, LONG_STRING_SIZE, fmt, ap);
  va_end(ap);

/*
  if ((Level & PRT_NORMAL) || (Level & PRT_INFO) || (Level & PRT_LOG))
    SLevel = LOG_INFO;
  else if (Level & PRT_ERR)
    SLevel = LOG_ERR;
  else if (Level & PRT_WARN)
    SLevel = LOG_WARNING;
  else if (debug & Level)
    SLevel = LOG_DEBUG;
*/


  SLevel = IS_DEBUG(Level) ? LOG_DEBUG : LOG_INFO;

  if (Level & syslogmessage)
    syslog(SLevel, "%s", String);

  if (Level & stdoutput) {
    fprintf(stdout, "%s\n", String);
    fflush(stdout);
  } /* if */

  if (Level & message)
    if (fcons == NULL)
      fprintf(stderr, "%s", String);
    else
      fprintf(fcons, "%s\r", String);

  if (Level & xinfo)
    print_from_server(String);

  if (Level & PRT_LOG) {
    fprintf(fprot, "%s  %s", stl, String);

    if (synclog)
      fflush(fprot);
  } /* if */

  return(0);
} /* print_msg */


void Exit(int RetCode)
{
  print_msg(PRT_NORMAL, "exit now %d\n", RetCode);

  if (socket_size(sockets) >= 2) {
    close(sockets[ISDNCTRL].descriptor);
    close(sockets[ISDNINFO].descriptor);

    if (xinfo && sockets[IN_PORT].descriptor != -2)
      close(sockets[IN_PORT].descriptor);
  } /* if */

  closelog();
  exit(RetCode);
} /* Exit */


void exit_on_signal(int Sign)
{
  print_msg(PRT_NORMAL, "Got signal %d\n", Sign);

  Exit(7);
} /* exit_on_signal */


/* --------------------------------------------------------------------------
   call reference:


    1 ..  63 DIALIN's    - ist dabei 8. Bit gesetzt, meine Antwort an VSt
                           (cref=1 : VSt->User // cref=129 : User->VSt)

   64 .. 127 DIALOUT's   - ist dabei 8. Bit gesetzt, Antwort der VSt an mich
      	     		   (cref=64 : User->VSt // cref=192 : VSt->User)

   kommt ein SETUP ACKNOWLEDGE mit cref > 128, beginnt ein DIALOUT (!)
   _nicht_ mit der Teles-Karte

   kommt ein CALL PROCEEDING mit cref > 191, beginnt ein DIALOUT
   mit der 2. Teles-Karte

   folgt danach sofort ein SETUP, ist das ein Selbstanruf!


   DIALOUT's erhalten vom Teles-Treiber staendig eine um jeweils 1
   erhoehte call references
   War die letzte cref also < 127, und die naechste = 64, bedeutet dies
   einen Reload des Teles-Treibers!
-------------------------------------------------------------------------- */


static void dotrace()
{
  register int  i;
  auto 	   char s[BUFSIZ];


  print_msg(PRT_NORMAL, ">>>>>>> TRACE (CR=next, q=quit, d=dump, g=go):");
  gets(s);

  if (*s == 'q')
    exit(0);
  else if (*s == 'g')
    trace = 0;
  else if (*s == 'd') {

    print_msg(PRT_NORMAL, "chan=%d\n", chan);

    for (i = 0; i < MAXCHAN; i++) {
      if (call[i].state) {
        print_msg(PRT_NORMAL, "call[%d]:", i);
        print_msg(PRT_NORMAL, "state=%d, cref=%d, dialin=%d, cause=%d\n",
          call[i].state, call[i].cref, call[i].dialin, call[i].cause);
        print_msg(PRT_NORMAL, "\taoce=%d, channel=%d, dialog=%d, bearer=%d\n",
          call[i].aoce, call[i].channel, call[i].dialog, call[i].bearer);
        print_msg(PRT_NORMAL, "\tnum[0]=\"%s\", num[1]=\"%s\"\n",
          call[i].num[0], call[i].num[1]);
        print_msg(PRT_NORMAL, "\tvnum[0]=\"%s\", vnum[1]=\"%s\"\n",
          call[i].vnum[0], call[i].vnum[1]);
        print_msg(PRT_NORMAL, "\tconfentry[0]=%d,  confentry[1]=%d\n",
          call[i].confentry[0], call[i].confentry[1]);
        print_msg(PRT_NORMAL, "\ttime=%d, connect=%d, disconnect=%d, duration=%d\n",
          (int)call[i].time, (int)call[i].connect, (int)call[i].disconnect, (int)call[i].duration);
      } /* if */
    } /* for */

    dotrace();

  } /* if */
} /* dotrace */


static int b2c(register int b)
{
  register int i;


  for (i = 0; i < MAXCHAN; i++)
    if (call[i].bchan == b)
      return(i);

  return(-1);
} /* b2c */


static void huptime(int chan, int bchan)
{
  register int                c = call[chan].confentry[OTHER];
  auto 	   isdn_net_ioctl_cfg cfg;
  auto 	   int		      old, new;
  auto	   char	 	      sx[BUFSIZ];


  if (huptimeofs && (c > -1)) {
    if ((ifo[bchan].u & ISDN_USAGE_OUTGOING) && ((ifo[bchan].u & ISDN_USAGE_MASK) == ISDN_USAGE_NET)) {

      strcpy(cfg.name, known[c]->interface);

      if (ioctl(sockets[ISDNCTRL].descriptor, IIOCNETGCF, &cfg) >= 0) {
        old = cfg.onhtime;

        new = (int)cheap96(cur_time, known[c]->zone);

        if (old != (new - huptimeofs)) {
          cfg.onhtime = new - huptimeofs;

          if (ioctl(sockets[ISDNCTRL].descriptor, IIOCNETSCF, &cfg) >= 0) {
            sprintf(sx, "HUPTIMEOUT %s %d - %d = %d (was %d)",
              known[c]->interface, new, huptimeofs, new - huptimeofs, old);
            info(chan, PRT_INFO, STATE_HUPTIMEOUT, sx);
          } /* if */
        }
        else {
          sprintf(sx, "HUPTIMEOUT %s still %d", known[c]->interface, old);
          info(chan, PRT_INFO, STATE_HUPTIMEOUT, sx);
        } /* else */
      } /* if */
    } /* if */
  } /* if */
} /* huptime */


static void processbytes()
{
  register int j,  chan, change = 0;
  auto	   char	   sx[BUFSIZ], sy[BUFSIZ];
  auto	   time_t  DiffTime = (time_t)0;
  auto	   double  tack;
#if RATE_PER_SAMPLE
  auto	   double  DiffTime2;
#endif


  for (j = 0; j < chans; j++)
    if (ifo[j].u & ISDN_USAGE_MASK)
      if ((chan = b2c(j)) != -1) {

        if (io[j].i > call[chan].ibytes) {
      	  call[chan].libytes = call[chan].ibytes;
      	  call[chan].ibytes = io[j].i;
          change++;
        } /* if */

        if (io[j].o > call[chan].obytes) {
      	  call[chan].lobytes = call[chan].obytes;
      	  call[chan].obytes = io[j].o;
          change++;
        } /* if */

        if (fullhour) /* zu jeder vollen Stunde HANGUP-Timer neu setzen (aendern sich um: 9:00, 12:00, 18:00, 21:00, 2:00, 5:00 Uhr) */
	  huptime(chan, j);

        DiffTime = cur_time - call[chan].connect;

        if (DiffTime) {
          call[chan].ibps = (double)(call[chan].ibytes / (double)(DiffTime));
          call[chan].obps = (double)(call[chan].obytes / (double)(DiffTime));
        }
        else
          call[chan].ibps = call[chan].obps = 0.0;

        if (change && (call[chan].ibytes + call[chan].obytes)) {

          if ((tack = call[chan].tick - (double)(cur_time - call[chan].connect)) > 0.0)
            sprintf(sy, " C=%s", double2clock(tack) + 4);
          else
            *sy = 0;

          sprintf(sx, "I=%s %s/s  O=%s %s/s%s",
      	    double2byte((double)call[chan].ibytes),
      	    double2byte((double)call[chan].ibps),
      	    double2byte((double)call[chan].obytes),
      	    double2byte((double)call[chan].obps),
	    sy);


          info(chan, PRT_SHOWBYTE, STATE_BYTE, sx);

#if RATE_PER_SAMPLE
          if ((DiffTime2 = ((double)(tt - tto) / (double)CLK_TCK))) {
            auto long   ibytes = call[chan].ibytes - call[chan].libytes;
            auto long   obytes = call[chan].obytes - call[chan].lobytes;
            auto double ibps = (double)ibytes / (double)DiffTime2;
            auto double obps = (double)obytes / (double)DiffTime2;


            sprintf(sx, "I=%s %s/s  O=%s %s/s (%4.4gs)",
      	      double2byte(ibytes),
      	      double2byte(ibps),
      	      double2byte(obytes),
      	      double2byte(obps),
              (double)DiffTime2);

            info(chan, PRT_SHOWBYTE, STATE_BYTE, sx);
          } /* if */
#endif
        } /* if */
      } /* if */
} /* processbytes */


static void processinfo(char *s)
{
  register char	*p;
  register int	 j, k, chan, version;
  auto	   char	 sx[BUFSIZ];


  if (verbose)
    print_msg(PRT_LOG, "%s\n", s);


  if (!memcmp(s, "idmap:", 6)) {
    j = sscanf(s + 7, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
      ifo[ 0].id, ifo[ 1].id, ifo[ 2].id, ifo[ 3].id,
      ifo[ 4].id, ifo[ 5].id, ifo[ 6].id, ifo[ 7].id,
      ifo[ 8].id, ifo[ 9].id, ifo[10].id, ifo[11].id,
      ifo[12].id, ifo[13].id, ifo[14].id, ifo[15].id, ifo[16].id);

    if (!newcps && (j == 17)) {
      newcps = 1;

      for (chans = 0; chans < 17; chans++)
        if (!strcmp(ifo[chans].id, "-"))
          break;

      print_msg(PRT_NORMAL, "(ISDN subsystem with ISDN_MAX_CHANNELS > 16 detected - %d active channels, %d MSN/SI entries)\n", chans, mymsns);

      /*
       * Ab "ISDN subsystem Rev: 1.21/1.20/1.14/1.10/1.6" gibt's den ioctl(IIOCGETDVR)
       *
       * Letzte Version davor war "ISDN subsystem Rev: 1.18/1.18/1.13/1.9/1.6"
       */

      if (!replay)
        if ((version = ioctl(sockets[ISDNINFO].descriptor, IIOCGETDVR)) != -EINVAL) {

          tty_dv = version & 0xff;
          version = version >> 8;
          net_dv = version & 0xff;
          version = version >> 8;
          inf_dv = version & 0xff;

      	  print_msg(PRT_NORMAL, "(Data versions: iprofd=0x%02x  net_cfg=0x%02x  /dev/isdninfo=0x%02x)\n", tty_dv, net_dv, inf_dv);
        } /* if */

      if (chans != 2) /* coming soon ;-) */
        chans = 2;
    } /* if */
  }
  else if (!memcmp(s, "chmap:", 6))
    sscanf(s + 7, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
      &ifo[ 0].ch, &ifo[ 1].ch, &ifo[ 2].ch, &ifo[ 3].ch,
      &ifo[ 4].ch, &ifo[ 5].ch, &ifo[ 6].ch, &ifo[ 7].ch,
      &ifo[ 8].ch, &ifo[ 9].ch, &ifo[10].ch, &ifo[11].ch,
      &ifo[12].ch, &ifo[13].ch, &ifo[14].ch, &ifo[15].ch);
  else if (!memcmp(s, "drmap:", 6))
    sscanf(s + 7, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
      &ifo[ 0].dr, &ifo[ 1].dr, &ifo[ 2].dr, &ifo[ 3].dr,
      &ifo[ 4].dr, &ifo[ 5].dr, &ifo[ 6].dr, &ifo[ 7].dr,
      &ifo[ 8].dr, &ifo[ 9].dr, &ifo[10].dr, &ifo[11].dr,
      &ifo[12].dr, &ifo[13].dr, &ifo[14].dr, &ifo[15].dr);
  else if (!memcmp(s, "usage:", 6))
    sscanf(s + 7, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
      &ifo[ 0].u, &ifo[ 1].u, &ifo[ 2].u, &ifo[ 3].u,
      &ifo[ 4].u, &ifo[ 5].u, &ifo[ 6].u, &ifo[ 7].u,
      &ifo[ 8].u, &ifo[ 9].u, &ifo[10].u, &ifo[11].u,
      &ifo[12].u, &ifo[13].u, &ifo[14].u, &ifo[15].u);
  else if (!memcmp(s, "flags:", 6))
    sscanf(s + 7, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
      &ifo[ 0].f, &ifo[ 1].f, &ifo[ 2].f, &ifo[ 3].f,
      &ifo[ 4].f, &ifo[ 5].f, &ifo[ 6].f, &ifo[ 7].f,
      &ifo[ 8].f, &ifo[ 9].f, &ifo[10].f, &ifo[11].f,
      &ifo[12].f, &ifo[13].f, &ifo[14].f, &ifo[15].f);
  else if (!memcmp(s, "phone:", 6)) {
    sscanf(s + 7, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
      ifo[ 0].n, ifo[ 1].n, ifo[ 2].n, ifo[ 3].n,
      ifo[ 4].n, ifo[ 5].n, ifo[ 6].n, ifo[ 7].n,
      ifo[ 8].n, ifo[ 9].n, ifo[10].n, ifo[11].n,
      ifo[12].n, ifo[13].n, ifo[14].n, ifo[15].n);

    for (j = 0; j < chans; j++)
      if (ifo[j].u & ISDN_USAGE_MASK)
        for (chan = 0; chan < MAXCHAN; chan++)
          if (memcmp(ifo[j].n, "???", 3) && !strcmp(ifo[j].n, call[chan].onum[OTHER])) {
            call[chan].bchan = j;

	    strcpy(call[chan].id, ifo[j].id);

      	    if (!(ifo[j].u & ISDN_USAGE_MASK)) /* no connection */
              strcpy(call[chan].usage, (ifo[j].u & ISDN_USAGE_EXCLUSIVE) ? "Exclusive" : "Offline");
      	    else {
      	      switch (ifo[j].u & ISDN_USAGE_MASK) {
	        case ISDN_USAGE_RAW   : sprintf(call[chan].usage, "%s %s", (ifo[j].u & ISDN_USAGE_OUTGOING) ? "Outgoing" : "Incoming", "Raw");
	         		      	break;

	        case ISDN_USAGE_MODEM : sprintf(call[chan].usage, "%s %s", (ifo[j].u & ISDN_USAGE_OUTGOING) ? "Outgoing" : "Incoming", "Modem");
	       			      	break;

	        case ISDN_USAGE_NET   : sprintf(call[chan].usage, "%s %s", (ifo[j].u & ISDN_USAGE_OUTGOING) ? "Outgoing" : "Incoming", "Net");
	    	 		      	break;

	        case ISDN_USAGE_VOICE : sprintf(call[chan].usage, "%s %s", (ifo[j].u & ISDN_USAGE_OUTGOING) ? "Outgoing" : "Incoming", "Voice");
	       			      	break;

	        case ISDN_USAGE_FAX   : sprintf(call[chan].usage, "%s %s", (ifo[j].u & ISDN_USAGE_OUTGOING) ? "Outgoing" : "Incoming", "Fax");
	       			      	break;
      	      } /* switch */
      	    } /* else */

	    huptime(chan, j); /* bei Verbindungsbeginn HANGUP-Timer neu setzen */
          } /* if */

    if (imon) {
      print_msg(PRT_SHOWIMON, "\n+ %s -----------------------------------------\n", st + 4);

      for (j = 0; j < chans; j++) {

        p = sx;

        p += sprintf(p, "| %s#%d : ", ifo[j].id, j & 1);

        if (!(ifo[j].u & ISDN_USAGE_MASK)) /* no connection */
          p += sprintf(p, (ifo[j].u & ISDN_USAGE_EXCLUSIVE) ? "exclusive" : "free");
        else {
          p += sprintf(p, "%s\t", (ifo[j].u & ISDN_USAGE_OUTGOING) ? "outgoing" : "incoming");

      	  switch (ifo[j].u & ISDN_USAGE_MASK) {
	    case ISDN_USAGE_RAW   : p += sprintf(p, "raw device");
	         		    break;

	    case ISDN_USAGE_MODEM : p += sprintf(p, "tty emulation");
	       			    break;

	    case ISDN_USAGE_NET   : p += sprintf(p, "IP interface");
	    	 		    break;

	    case ISDN_USAGE_VOICE : p += sprintf(p, "Voice");
	       			    break;

	    case ISDN_USAGE_FAX   : p += sprintf(p, "Fax");
	       			    break;
      	  } /* switch */

      	  p += sprintf(p, "\t%s", ifo[j].n);

          if ((chan = b2c(j)) != -1) {
            k = call[chan].dialin ? CALLING : CALLED;

            p += sprintf(p, " (%s/%s, %s)",
                   call[chan].vorwahl[k],
            	   call[chan].rufnummer[k],
            	   call[chan].area[k]);

          } /* if */
        } /* else */

      	print_msg(PRT_SHOWIMON, "%s\n", sx);

      } /* for */
    } /* if */
  }
  else if (!memcmp(s, "ibytes:", 7))
    sscanf(s + 8, "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
      &io[ 0].i, &io[ 1].i, &io[ 2].i, &io[ 3].i,
      &io[ 4].i, &io[ 5].i, &io[ 6].i, &io[ 7].i,
      &io[ 8].i, &io[ 9].i, &io[10].i, &io[11].i,
      &io[12].i, &io[13].i, &io[14].i, &io[15].i);
  else if (!memcmp(s, "obytes:", 7)) {
    sscanf(s + 8, "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
      &io[ 0].o, &io[ 1].o, &io[ 2].o, &io[ 3].o,
      &io[ 4].o, &io[ 5].o, &io[ 6].o, &io[ 7].o,
      &io[ 8].o, &io[ 9].o, &io[10].o, &io[11].o,
      &io[12].o, &io[13].o, &io[14].o, &io[15].o);

    processbytes();
  } /* else */
} /* processinfo */


static void clearchan(int chan)
{
  memset((char *)&call[chan], 0, sizeof(CALL));
  strcpy(call[chan].vnum[0], "?");
  strcpy(call[chan].vnum[1], "?");
  call[chan].confentry[0] = call[chan].confentry[1] = -1;
  call[chan].bchan = -1;
} /* clearchan */


static void processctrl(char *s)
{
  register char       *ps = s;
  register int         i;
  register int         wegchan; /* fuer gemakelte */
  auto 	   int 	       ref, dialin, type, cref, version;
  static   int	       tei = 0, net = 1;
  auto 	   char	       sx[BUFSIZ], s2[BUFSIZ];


  if (verbose)
    print_msg(PRT_LOG, "%s\n", s);

  /* Tei wird gelesen und bleibt bis zum Ende des naechsten hex: stehen.
     Der naechste hex: -Durchlauf hat also die korrekte tei.*/

  if (!memcmp(ps, "Q.931 frame network->user tei ", 30)) {
    tei = strtol(ps += 30, NIL, 10);
    net = 1;
  }
  else if (!memcmp(ps, "Q.931 frame user->network tei ", 30)) {
    tei = strtol(ps += 30, NIL, 10);
    net = 0;
  }
  else if (!memcmp(ps, "Q.931 frame network->user with tei ", 35)) {
    tei = strtol(ps += 35, NIL, 10);
    net = 1;
  }
  else if (!memcmp(ps, "Q.931 frame network->user", 25))
    net = 1;
  else if (!memcmp(ps, "hex: ", 5)) {
    i = strtol(ps += 5, NIL, 16);

    switch (i) {
      case 0x40 :
      case 0x41 : version = VERSION_1TR6;    break;

      case 0x08 : version = VERSION_EDSS1;   break;

      default   : version = VERSION_UNKNOWN; break;
    } /* switch */

#ifdef SL
    if (version == VERSION_1TR6) {
      print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: OOPS! 1TR6 Frame? Ignored!\n", st + 4);
      goto endhex;
    } /* if */
#endif

    cref = strtol(ps += 6, NIL, 16);
    type = strtol(ps += 3, NIL, 16);


    cref = (net) ? cref : cref ^ 0x80; /* cref immer aus Sicht des Amts */
    ref = cref ^ 0x80;

    dialin = !tei; /* Keine tei -> dialin (Broadcast), alle anderen haben schon eine Tei! */

    if (allflags & PRT_DEBUG_DIAG)
      diag(cref, tei, dialin, net, type, version);

    /* leider laesst sich kein switch nehmen, da decode
       innerhalb von SETUP/A_ACK aufgerufen werden muss, sonst
       aber erst nach feststellen von chan
       Daher GOTO (urgs...) an das Ende vom if hex:.. */

    if (type == SETUP) { /* neuen Kanal, ev. dummy, wenn keiner da ist */
      chan = 5; /* den nehmen wir _nur_ dafuer! */
      clearchan(chan);
      call[chan].aoce = -1;
      call[chan].dialin = !tei;
      call[chan].tei = tei;
      decode(chan, ps, type, version);

      if (call[chan].channel) { /* Aha, Kanal war dabei, dann nehmen wir den gleich */
        chan = call[chan].channel - 1;

        print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: Chan auf %d gesetzt\n", st + 4, chan);

        /* nicht --channel, channel muss unveraendert bleiben! */
        memcpy((char *)&call[chan], (char *)&call[5], sizeof(CALL));
        Change_Channel(5, chan);
      	clearchan(5);
      } /* if */

      call[chan].cref = (dialin) ? cref : (cref | 0x80); /* immer die cref, die _vom_ Amt kommt/kommen sollte */
      call[chan].dialin = dialin;
      call[chan].tei = tei;
      call[chan].connect = cur_time;
      call[chan].duration = tt;
      call[chan].state = type;

      print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: START CHAN#%d tei %d cref %d %d %s %s->\n",
        st + 4, chan, tei, cref, call[chan].cref,
        call[chan].dialin ? " IN" : "OUT",
        net ? "NET" : "USR");

      goto endhex;
    } /* if SETUP */

    if ((type == SETUP_ACKNOWLEDGE) || (type == CALL_PROCEEDING)) {
      /* Kann sein, dass ein SETUP vorher kam, suchen wir mal, denkbar:
       	   a) SETUP in 5 (eig. rausruf): decode auf 5, dann copy nach channel
       	   b) nichts (rausruf fremd): decode auf 5, copy nach channel */

      chan = 5;

      if ((call[5].cref != cref) || (call[5].tei != tei)) {
        /* bei C_PROC/S_ACK ist cref _immer_ > 128 */
        /* keiner da, also leeren */
      	clearchan(chan);
        call[chan].aoce = -1;
        call[chan].dialin = !tei;
        call[chan].tei = tei;
        call[chan].cref = cref;
      } /* if */

      decode(chan, ps, type, version);

      if (call[chan].channel) { /* jetzt muesste einer da sein */

        chan = call[chan].channel - 1;

        /* nicht --channel, channel muss unveraendert bleiben! */
        memcpy((char *)&call[chan], (char *)&call[5], sizeof(CALL));
        Change_Channel(5, chan);
      	clearchan(5);
      }
      else
        print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: OOPS, C_PROC/S_ACK ohne channel? tei %d\n",
          st + 4, tei);

      call[chan].connect = cur_time;
      call[chan].duration = tt;
      call[chan].state = type;

      print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: START CHAN#%d tei %d %s %s->\n",
        st + 4, chan, tei,
        call[chan].dialin ? " IN" : "OUT",
        net ? "NET" : "USR");
      goto endhex;
    } /* if C_PROC || S_ACK */

    if (type == AOCD_1TR6) {
      decode(chan, ps, type, version);
      goto endhex;
    } /* if AOCD_1TR6 */

    /* Beim Makeln kommt Geb. Info nur mit Cref und Tei, die
       cref muessen wir dann in chan 2/3 suchen */

    /* Bei geparkten Gespr. kommen die waehrend des Parkens
       aufgelaufenen Gebuehren beim Wiederholen. */

    if ((cref != call[0].cref) && (cref != call[1].cref) &&
        (cref != call[2].cref) && (cref != call[3].cref))

      /* Mit falscher cref kommt hier keiner rein, koennte
      	 ein RELEASE auf bereits freiem Kanal sein */
      goto endhex;

    /* So, wenn wir hier ankommen, haben wir auf jeden Fall einen
       Kanal (0, 1, 2 oder 3) und eine cref. Die tei folgt evtl. erst beim
       Connect (Reinruf). Suchen wir den Kanal: */

    /* crefs absuchen. Gibt's die mehrmals, tei absuchen, dann haben wir
       ihn.
       Es kann aber sein, dass cref stimmt, aber noch keine tei da war
       (Reinruf). Dann ist aber die cref eindeutig (hoffentlich)!
       finden wir einen Kanal mit passender cref, der keine
       tei hat, haben wir ihn. Hat er eine, und sie stimmt,
       ebenso. Sonst weitersuchen. Geparkte Kanaele ignorieren
       bis zum RESUME, oder sie werden bei neuem SETUP_ACK. ueber-
       schrieben, wenn wir wen im Parken verhungen lassen haben */

    chan = -1;

    for (i = 0; ((i < 4) &&
      ((call[i].cref != cref) ||
        ((call[i].state == SUSPEND) && (type != RESUME_ACKNOWLEDGE)) ||
        ((call[i].tei) && (call[i].tei != tei)))); i++);
      chan = i;

    print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: Kanal %d\n", st + 4, chan);

    /* auch wenn hier schon eine tei bei ist, erst beim connect hat
       ein reingerufener Kanal eine gueltige tei*/

    decode(chan, ps, type, version);

    switch (type) {

      case CONNECT             :
      case CONNECT_ACKNOWLEDGE :

	/* Bei Rufen an die Teles kommt CONNECT und CONN.ACKN., eins reicht uns */
	if (call[chan].state == CONNECT)
	  goto doppelt;

	call[chan].state = CONNECT;
	call[chan].tei = tei;
	call[chan].dialog++; /* es hat connect gegeben */
        call[chan].connect = cur_time;
        call[chan].duration = tt;

        info(chan, PRT_SHOWCONNECT, STATE_CONNECT, "CONNECTED");

        if (sound)
          ringer(chan, RING_CONNECT);

doppelt:break;

      case SUSPEND_ACKNOWLEDGE :
        call[chan].state = SUSPEND;
        info(chan, PRT_SHOWHANGUP, STATE_HANGUP, "PARK");
        break;

      case RESUME_ACKNOWLEDGE :
        call[chan].state = CONNECT;
        info(chan, PRT_SHOWCONNECT, STATE_CONNECT, "RESUME");
        break;

      case MAKEL_ACKNOWLEDGE :
        wegchan = (call[2].state) ? 3 : 2;
        memcpy((char *)&call[wegchan], (char *)&call[chan], sizeof(CALL));
        Change_Channel(chan, wegchan);
      	clearchan(chan);
        call[wegchan].state = MAKEL_ACKNOWLEDGE;
        info(wegchan, PRT_SHOWHANGUP, STATE_HANGUP, "MAKEL");
        break;

      case MAKEL_RESUME_ACK :
        if (call[chan].channel) { /* muesste einer da sein */
          memcpy((char *)&call[call[chan].channel - 1], (char *)&call[chan], sizeof(CALL));
          call[call[chan].channel - 1].channel = chan; /* den alten merken */
          Change_Channel(chan, call[chan].channel - 1);
          chan = call[chan].channel - 1; /* chan setzen */
      	  clearchan(call[chan].channel);
          call[chan].channel = chan + 1; /* in Ordnung bringen */
          call[chan].state = CONNECT;
        } /* if */

        info(chan, PRT_SHOWCONNECT, STATE_CONNECT, "MAKELRESUME");
        break;


      case DISCONNECT          :

	if (!call[chan].state) /* Keine Infos -> Weg damit */
	  break;

	call[chan].disconnect = cur_time;

	if (replay)
          call[chan].duration = (tt - call[chan].duration) * 100;
	else
          call[chan].duration = tt - call[chan].duration;

        call[chan].state = DISCONNECT;

        break;


      case RELEASE             :
      case RELEASE_COMPLETE    :

        if (!net) /* wir nehmen nur RELEASE vom Amt */
          break;

        if (!call[chan].state) /* Keine Infos -> Weg damit */
          break;

        /* Statt der Konstrukte haben wir uns gemerkt, ob's ein CONNECT gab.
           Wenn nicht, hat's auch nichts gekostet. */
        /* Falls der RELEASE aber ein Rufablehnen war und der
           CONNECT noch folgt, wird dafuer jetzt chan auf
           4 gepackt, um die schoenen Daten in 0/1/ev.4 nicht
           zu zerstoeren. Wir erkennen das an fehlender tei. */

        if (!call[chan].tei) {
          memcpy((char *)&call[4], (char *)&call[chan], sizeof(CALL));
          Change_Channel(chan, 4);
          chan = 4;
          call[chan].tei = tei;
        } /* if */

	if (!call[chan].disconnect) {
  	  call[chan].disconnect = cur_time;

	  if (replay)
            call[chan].duration = (tt - call[chan].duration) * 100;
	  else
            call[chan].duration = tt - call[chan].duration;
        } /* if */

	if (!call[chan].dialog) {
          call[chan].duration = 0;
  	  call[chan].disconnect = call[chan].connect;

          print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: CHAN#%d genullt (dialin=%d, state=%d, tei=%d, cref=%d)\n",
            st + 4, chan, call[chan].dialin, call[chan].state, call[chan].tei,call[chan].cref);

          print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: OOPS! DURATION=0\n", st + 4);
        } /* if kein connect */

       	if (allflags & PRT_DEBUG_BUGS) {
          strcpy(sx, ctime(&call[chan].connect));
      	  sx[19] = 0;

          print_msg(PRT_DEBUG_BUGS, " DEBUG> %s: LOG CHAN#%d(%s : DIAL%s : %s -> %s : %d s (%d s) : %d EH):%s\n\n",
            st + 4, chan,
      	    sx + 4,
      	    call[chan].dialin ? "IN" : "OUT",
      	    call[chan].num[CALLING],
      	    call[chan].num[CALLED],
      	    (int)(call[chan].disconnect - call[chan].connect),
            (int)call[chan].duration,
      	    call[chan].aoce,
      	    CauseValue[call[chan].cause]);
       	} /* if */

        logger(chan);

	if (call[chan].dialog || any) { /* AK:14-Mar-96 ::
	   		      	      	     Unterdrueckung der HANGUP-Meldungen
                                             falls andere Geraete am Bus den Ruf
                                             abgelehnt haben */

          if (call[chan].ibytes + call[chan].obytes) {
            sprintf(s2, " I=%s O=%s",
      	      double2byte((double)call[chan].ibytes),
      	      double2byte((double)call[chan].obytes));
          }
          else
            *s2 = 0;

          if (call[chan].dialin)
            sprintf(sx, "HANGUP (%s %s)",
              double2clock((double)(call[chan].disconnect - call[chan].connect)), s2);
          else {
            if (call[chan].aoce > 0) {
              if (currency_factor)
                sprintf(sx, "HANGUP (%s %s %s %s)",
                  double2str(call[chan].aoce * currency_factor, 6, 2, DEB),
                  currency,
              	  double2clock((double)(call[chan].disconnect - call[chan].connect)), s2);
              else
                sprintf(sx, "HANGUP (%d EH DM %s %s %s)",
                  call[chan].aoce,
                  double2str(call[chan].aoce * einheit, 6, 2, DEB),
              	  double2clock((double)(call[chan].disconnect - call[chan].connect)), s2);
            }
            else {
              if (version == VERSION_1TR6)
                sprintf(sx, "HANGUP (%s %s) %s",
              	  double2clock((double)(call[chan].disconnect - call[chan].connect)), s2,
              	  tr6cause(call[chan].cause));
              else
                sprintf(sx, "HANGUP (%s %s) %s",
              	  double2clock((double)(call[chan].disconnect - call[chan].connect)), s2,
              	  CauseValue[call[chan].cause]);
            } /* else */
          } /* else */

          if (!memcmp(sx, "HANGUP (         )", 18))
            info(chan, PRT_SHOWHANGUP, STATE_HANGUP, "HANGUP");
          else
            info(chan, PRT_SHOWHANGUP, STATE_HANGUP, sx);

          if (sound)
            ringer(chan, RING_HANGUP);
        } /* if */

        call[chan].state = 0;
        call[chan].dialog = 0;
        call[chan].cref = 0;
        call[chan].tei = 0;

     	break;

    } /* switch */

endhex:
    tei = 0; /* Wenn nach einer tei-Zeile keine hex:-Zeile kommt, tei ungueltig machen! */
  } /* if */
} /* processctrl */


static void processrate()
{
  register char  *p;
  register int 	  j;
  auto	   char	  sx[BUFSIZ];
  auto	   double s;


  if (!ioctl(sockets[ISDNINFO].descriptor, IIOCGETCPS, &io)) {

    if (verbose) {
      p = sx;
      s = 0L;

      for (j = 0; j < chans; j++) {
	p += sprintf(p, "%ld ", io[j].i);
        s += io[j].i;
      } /* for */

      if (s > 0L)
        print_msg(PRT_LOG, "ibytes:\t%s\n", sx);

      p = sx;
      s = 0L;

      for (j = 0; j < chans; j++) {
	p += sprintf(p, "%ld ", io[j].o);
        s += io[j].o;
      } /* for */

      if (s > 0L)
        print_msg(PRT_LOG, "obytes:\t%s\n", sx);
    } /* if */

    processbytes();
  } /* if */
} /* processrate */


static void now()
{
  auto struct tms tms;


  time(&cur_time);
  tto = tt;
  tt = times(&tms);
  set_time_str();
} /* now */


#ifdef I4l_073
static void alarm_handler()
{
  signal(SIGALRM, alarm_handler);
} /* alarm_handler */
#endif


static int morectrl()
{
  register char      *p, *p1, *p2, *p3;
  static   char       s[BIGBUFSIZ];
  static   char      *ps = s;
  auto	   int        n = 0;
  auto 	   struct tm *tm;

	/* Der alarm() sollte eigentlich fuer das neue i4l ueberfluessig sein,
	   er muss aber weiterhin da sein. Vielleicht Autoerkennung */

#ifdef I4l_073
  signal(SIGALRM, alarm_handler);
  alarm(wakeup);
#endif

	/* Andreas, pass bloss mit alarm auf! Der _muss_ ausgeschaltet sein,
	   wenn der read vorbei ist! */
  if ((n = read(sockets[ISDNCTRL].descriptor, ps, BUFSIZ)) > 0) {

#ifdef I4l_073
    alarm(0);
#endif

    now();
    ps += n;

    *ps = 0;

    p1 = s;

    while ((p = p2 = strchr(p1, '\n'))) {
      *p = 0;

      while (*--p == ' ')
        *p = 0;

      if (replay) {

        if (replaydev)
          p3 = p1;
        else {
    	  cur_time = tt = atom(s + 4);

    	  set_time_str();

    	  tm = localtime(&cur_time);
    	  einheit = (tm->tm_year > 95) ? 0.12 : 0.23;

    	  p3 = p1 + 26;
        } /* if */

        if (!memcmp(p3, "idmap:", 6) ||
            !memcmp(p3, "chmap:", 6) ||
            !memcmp(p3, "drmap:", 6) ||
            !memcmp(p3, "usage:", 6) ||
            !memcmp(p3, "flags:", 6) ||
            !memcmp(p3, "phone:", 6) ||
            !memcmp(p3, "ibytes:", 7) ||
            !memcmp(p3, "obytes:", 7))
      	  processinfo(p3);
        else
          processctrl(p3);
      }
      else
        processctrl(p1);

      p1 = p2 + 1;
    } /* while */

    if (p1 < ps) {
      n = ps - p1;
      memmove(s, p1, n);
      ps = s + n;
    }
    else
      ps = s;

    return(1);
  }
  else
  {
    alarm(0);
    return(0);
  }
} /* morectrl */


static void moreinfo()
{
  register char *p, *p1, *p2;
  static   char  s[BIGBUFSIZ];
  static   char *ps = s;
  auto	   int   n;


  if ((n = read(sockets[ISDNINFO].descriptor, ps, BUFSIZ)) > 0) {
    now();
    ps += n;

    *ps = 0;

    p1 = s;

    while ((p = p2 = strchr(p1, '\n'))) {
      *p = 0;

      while (*--p == ' ')
        *p = 0;

      processinfo(p1);

      p1 = p2 + 1;
    } /* while */

    if (p1 < ps) {
      n = ps - p1;
      memmove(s, p1, n);
      ps = s + n;
    }
    else
      ps = s;
  } /* if */
} /* moreinfo */


static void loop()
{
  auto fd_set readmask;
  auto fd_set exceptmask;
  auto int    Cnt;
  auto int    len;
  auto int    queuenumber;
  auto int    NewSocket;
  auto int    NewClient = 0;
  auto struct sockaddr incoming;
  auto struct timeval timeout, *Interval;


  if (xinfo)
    if ((NewSocket = listening(port)) >= 0) {
      if (add_socket(&sockets,NewSocket))
	Exit(11);
    }
    else
      Exit(17);

  while (1) {
    if (replay) {

      if (trace)
        dotrace();

      if (!morectrl())
        break;
    }
    else {
      FD_ZERO(&readmask);
      FD_ZERO(&exceptmask);

      queuenumber = socket_size(sockets);

      if (NewClient) {
        /* Damit sich der neue Client anmelden kann, ohne
           das was anderes dazwischen funkt ... */
        FD_SET(sockets[NewClient].descriptor, &readmask);
      	NewClient = 0;
      }
      else
      {
        for (Cnt = 0; Cnt < queuenumber; Cnt++)
          FD_SET(sockets[Cnt].descriptor, &readmask);

        for (Cnt = FIRST_DESCR; Cnt < queuenumber; Cnt++)
          FD_SET(sockets[Cnt].descriptor, &exceptmask);
      }

      Interval = Get_Interval();

      if (newcps && ((ifo[0].u & ISDN_USAGE_MASK) + (ifo[1].u & ISDN_USAGE_MASK)) &&
          (Interval == (struct timeval *)NULL)) {
        timeout.tv_usec = 0;
  	timeout.tv_sec  = wakeup;

        Interval = &timeout;
      } /* if */

      if ((Cnt = select(FD_SETSIZE, &readmask, NULL, &exceptmask, Interval)) < 0) {
        print_msg(PRT_DEBUG_CS,"Error select: %s\n", strerror(errno));
        Exit(12);
      } /* if */

      processrate();

      if (!Cnt && (Interval == (struct timeval *)NULL))
        continue;

#ifdef I4l_073
      if (Cnt == 0 || FD_ISSET(sockets[ISDNCTRL].descriptor, &readmask)) /* Abbruch durch Timeout -> Programm starten */
#else
      if (Cnt == 0) /* Abbruch durch Timeout -> Programm starten */
#endif

        Start_Interval();
      now();

      for (Cnt = FIRST_DESCR; Cnt < socket_size(sockets); Cnt++)
      {
        if (FD_ISSET(sockets[Cnt].descriptor, &exceptmask))
        {
          if (sockets[Cnt].fp == NULL)
          {
          	disconnect_client(Cnt);
          	break;
          }
          else
          {
            int        event    = sockets[Cnt].call_event;
            int        cur_call = sockets[Cnt].chan;
            info_args *infoarg  = sockets[Cnt].info_arg;

          	Ring(NULL,NULL,Cnt,0);

               if (infoarg->flag & RING_LOOP && call[cur_call].cur_event == event)
              Start_Process(cur_call, infoarg, event);

          	break;
          }
        }

        else
        if (FD_ISSET(sockets[Cnt].descriptor, &readmask))
          if (sockets[Cnt].fp == NULL)
          {
            eval_message(Cnt);
            /* Arbeite immer nur ein Client ab, du weisst nicht, ob der
               naechste noch lebt */
            break;
          }
          else
            Print_Cmd_Output(Cnt);
      }

      if (xinfo && FD_ISSET(sockets[IN_PORT].descriptor, &readmask)) {
        len = sizeof(incoming);

        if ((NewSocket = accept(sockets[IN_PORT].descriptor,&incoming,&len)) == -1)
          print_msg(PRT_DEBUG_CS, "Error accept: %s\n", strerror(errno));
        else {
          if (add_socket(&sockets, NewSocket))
            Exit(13);

      	  queuenumber = socket_size(sockets);

          sockets[queuenumber -1 ].f_hostname = GetHostByAddr(&incoming);
          sockets[queuenumber - 1].waitstatus = WF_ACC;

          NewClient = queuenumber - 1;
        } /* else */
      }
      else if (FD_ISSET(sockets[ISDNINFO].descriptor, &readmask))
        moreinfo();
      else if (FD_ISSET(sockets[ISDNCTRL].descriptor, &readmask))
        (void)morectrl();
    } /* else */
  } /* while */
} /* loop */


int main(int argc, char *argv[], char *envp[])
{
  register char *p;
  register int	 i, c, res = 0;
  auto 	   char* ptty = NULL;
  auto 	   char  fn[BUFSIZ], logname[BUFSIZ];
  auto 	   FILE *fp;
  auto     int   defaultmsg = message = PRT_NORMAL | PRT_WARN | PRT_ERR | PRT_INFO;
  auto     int   newmessage = 0;
#ifdef TEST
  extern   void  test_center(void);
#endif


  sockets = NULL;
  allflags = 0;
  dbm = (DBM *)NULL;
  einheit = 0.12;
  known = (KNOWN **)NULL;

  myname = argv[0];
  myshortname = basename(myname);

  while ((c = getopt(argc, argv, options)) != EOF)
    switch (c) {
      case 'V' : printf("%s Version %s, Copyright (C) 1995, 1996 Andreas Kool\n", myshortname, VERSION);
      	       	 printf("%s comes with ABSOLUTELY NO WARRANTY; for details see COPYING.\n", myshortname);
                 printf("This is free software, and you are welcome to redistribute it\n");
                 printf("under certain conditions; see COPYING for details.\n");
      	       	 return(0);
  		 break;

      case 'v' : verbose++;
      	       	 break;

      case 's' : synclog++;
      	       	 break;

      case 'p' : port = strtol(optarg, NIL, 0);
      	       	 break;

      case 'm' : newmessage = strtol(optarg, NIL, 0);

      	       	 if (!newmessage) {
                   newmessage = defaultmsg;
                   printf("%s: WARNING: \"-m\" Option now requires numeric Argument\n", myshortname);
                 } /* if */
      	       	 break;

      case 'l' : syslogmessage = strtol(optarg, NIL, 0);

               	 if (!syslogmessage) {
                   syslogmessage = defaultmsg;
                   printf("%s: WARNING: \"-l\" Option requires numeric Argument\n", myshortname);
                 } /* if */
                 break;

      case 'x' : xinfo = strtol(optarg, NIL, 0);

               	 if (!xinfo)
                   xinfo = defaultmsg;
      	       	 break;

      case 'r' : replay++;
      	       	 break;

      case 'R' : replay++;
      	       	 replaydev++;
                 break;

      case 't' : settime = strtol(optarg, NIL, 0); /* 1=once, 2=ever */
      	       	 break;

      case 'C' : ptty = strdup(optarg);
                 break;

      case 'M' : imon++;
      	       	 break;

      case 'S' : sound++;
      	       	 break;

      case 'w' : wakeup = strtol(optarg, NIL, 0);
      	       	 break;

      case 'D' : daemon++;
      	       	 break;

      case 'T' : trace++;
      	       	 break;

      case 'a' : any = 0;
      	       	 break;

      case 'P' : stdoutput = PRT_LOG;
      	       	 break;

      case 'e' : showtei = 0;
      	       	 break;

      case 'h' : huptimeofs = strtol(optarg, NIL, 0);
      	       	 break;

      case '?' : printf(usage, myshortname, myshortname, options);
	         return(1);
    } /* switch */

    if (newmessage)
      message = newmessage;

    if (trace && daemon) {
      printf("%s","Can not trace and daemon together!\n");
      exit(20);
    } /* if */

    if (stdoutput && daemon) {
      printf("%s","Can not write to stdout as daemon!\n");
      exit(21);
    } /* if */

    if (daemon) {
      if (syslogmessage == -1)
        syslogmessage = defaultmsg;

      switch (fork()) {
        case -1 : print_msg(PRT_ERR,"%s","Can not start fork()!\n");
                  Exit(18);
                  break;
        case 0  : break;

        default : _exit(0);
      }

      /* message nicht explixit gesetzt wurde, dann gibt es beim daemon auch
         kein Output auf der Console/ttyx                                   */

      if (!newmessage && ptty == NULL)
        message = 0;
    } /* if */

  allflags = syslogmessage | message | xinfo | stdoutput;

  if (optind < argc) {

    if (add_socket(&sockets, -1) ||  /* reserviert fuer isdnctrl */
        add_socket(&sockets, -1)   ) /* reserviert fuer isdninfo */
      Exit(19);

    strcpy((char *)logname, LOGFILE);

    if (replay)
      strcat((char *)logname, ".rep");

    openlog(myshortname, LOG_NDELAY, LOG_DAEMON);

#ifdef TEST
    test_center();
#endif
    if (xinfo && read_user_access())
      Exit(22);

    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP,  hup_handler);
    signal(SIGINT,  exit_on_signal);
    signal(SIGTERM, exit_on_signal);
    signal(SIGALRM, exit_on_signal);
    signal(SIGQUIT, exit_on_signal);
    signal(SIGILL,  exit_on_signal);

    if (allflags == PRT_DEBUG_GENERAL)
      signal(SIGSEGV, exit_on_signal);

#ifdef SELECT_FIX
    sockets[ISDNCTRL].descriptor = !strcmp(argv[optind], "-") ? fileno(stdin) : open(argv[optind], O_RDONLY);
#else
    sockets[ISDNCTRL].descriptor = !strcmp(argv[optind], "-") ? fileno(stdin) : open(argv[optind], O_RDONLY | O_NONBLOCK);
#endif

    if (sockets[ISDNCTRL].descriptor >= 0) {

      if (ptty != NULL)
        fcons = fopen(ptty, "a");

      if (ptty == NULL || fcons != (FILE *)NULL) {
        if ((flog = fopen(logname, "a")) != (FILE *)NULL) {

          if (verbose) {
            if ((p = strrchr(argv[optind], '/')))
              p++;
            else
              p = argv[optind];

            sprintf(fn, "/var/log/isdn/%s", p);
          } /* if */

          if (!verbose || ((fprot = fopen(fn, "a")) != (FILE *)NULL)) {

            for (i = 0; i < MAXCHAN; i++)
	      clearchan(i);

            readconfig(myshortname);

            if (!replay)
              if ((fp = fopen(PIDFILE, "w")) != (FILE *)NULL) {
                fprintf(fp, "%d\n", (int)getpid());
                fclose(fp);
                (void)chmod(PIDFILE, 0644);
              } /* if */

            if (replay || ((sockets[ISDNINFO].descriptor = open(INFO, O_RDONLY | O_NONBLOCK)) >= 0)) {

              now();

    	      print_msg(PRT_NORMAL, "%s Version %s loaded\n", myshortname, VERSION);

              loop();

              if (sockets[ISDNINFO].descriptor >= 0)
                close(sockets[ISDNINFO].descriptor);
            }
            else {
              print_msg(PRT_ERR, msg1, myshortname, INFO, strerror(errno));
              res = 7;
            } /* else */

            if (verbose)
              fclose(fprot);
          }
          else {
            print_msg(PRT_ERR, msg1, myshortname, fn, strerror(errno));
            res = 5;
          } /* else */

          fclose(flog);
        }
    	else {
      	  print_msg(PRT_ERR, msg1, myshortname, logname, strerror(errno));
      	  res = 4;
    	} /* else */

        if (ptty != NULL)
          fclose(fcons);
      }
      else {
        print_msg(PRT_ERR, msg1, myshortname, fn, strerror(errno));
        res = 3;
      } /* else */

      close(sockets[ISDNCTRL].descriptor);
    }
    else {
      print_msg(PRT_ERR, msg1, myshortname, argv[optind], strerror(errno));
      res = 2;
    } /* else */

    print_msg(PRT_NORMAL, "%s Version %s exiting\n", myshortname, VERSION);
    closelog();

  }
  else {
    print_msg(PRT_ERR, usage, myshortname, myshortname, options);
    res = 1;
  } /* else */

  return(res);
} /* main */
