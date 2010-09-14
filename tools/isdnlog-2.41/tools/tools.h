/* $Id: tools.h,v 2.40 1996/06/19 17:45:43 root Exp $
 *
 * ISDN accounting for isdn4linux.
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
 * $Log: tools.h,v $
 * Revision 2.40    1996/06/19  17:45:43  akool
 * double2byte(), time2str() added
 *
 * Revision 2.3.26  1996/05/05  12:07:43  akool
 * known.interface added
 *
 * Revision 2.3.23  1996/04/28  12:16:43  akool
 * confdir()
 *
 * Revision 2.2.5  1996/03/25  19:17:43  akool
 * 1TR6 causes implemented
 *
 * Revision 2.23  1996/03/24  12:11:43  akool
 * Explicit decl. of basename() - new "unistd.h" dont have one
 *
 * Revision 2.15  1996/02/21  20:14:43  akool
 *
 * Revision 2.12  1996/02/13  20:08:43  root
 * Nu geht's (oder?)
 *
 * Revision 2.12  1996/02/13  20:08:43  root
 * Nu geht's (oder?)
 *
 * Revision 1.2  1996/02/13  20:05:28  root
 * so nun gehts
 *
 * Revision 1.1  1996/02/13  14:28:14  root
 * Initial revision
 *
 * Revision 2.05  1995/02/11  17:10:16  akool
 *
 */

/****************************************************************************/

#ifndef _TOOLS_H_
#define _TOOLS_H_

/****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <math.h>
#include <syslog.h>
#include <sys/ioctl.h>
#ifdef linux
#include <ndbm.h>
#include <sys/kd.h>
extern char *basename __P((__const char *__name));
#ifndef I4L_073
#include <linux/isdn.h>
#endif
#else
#include "/usr/ucbinclude/ndbm.h"
#include <libgen.h>
#endif

/****************************************************************************/

#define CONFDIR_VAR "ISDNLOG"
#define C_SLASH '/'
/* #define C_SLASH '\\' */

#define C_COMMENT '#'
#define S_COMMENT "#"

#ifndef TMPDIR
#	define TMPDIR P_tmpdir
#endif

#ifndef CONFFILE
#	define CONFFILE "isdnlog.conf"
#endif

/****************************************************************************/

#undef  min
#undef  max
#define max(a,b)        (((a) > (b)) ? (a) : (b))
#define min(a,b)        (((a) < (b)) ? (a) : (b))

/****************************************************************************/

#define NIL  (char **)NULL

/****************************************************************************/

#define NUMSIZE      20
#define RETSIZE      32
#define MAXRET	      5
#define MAXZONES      6
#define MAXCHAN       6

#define DIGITS 	     17
#define DEB           1

#define MAXUNKNOWN   50
#define MAXCONNECTS  50

/****************************************************************************/

#define CALLING       0
#define CALLED        1
#define DATETIME      2

/****************************************************************************/

#define DIALOUT	      0
#define DIALIN	      1

/****************************************************************************/

#define ALERTING               0x01
#define CALL_PROCEEDING	       0x02
#define SETUP                  0x05
#define SETUP_ACKNOWLEDGE      0x0d
#define SUSPEND                0x25
#define SUSPEND_ACKNOWLEDGE    0x2d
#define RESUME                 0x26
#define RESUME_ACKNOWLEDGE     0x2e
#define CONNECT                0x07
#define CONNECT_ACKNOWLEDGE    0x0f
#define FACILITY               0x62
#define NOTIFY                 0x6e
#define STATUS                 0x7d
#define MAKEL_ACKNOWLEDGE      0x28
#define MAKEL_RESUME_ACK       0x33
#define DISCONNECT             0x45
#define RELEASE                0x4d
#define RELEASE_COMPLETE       0x5a
#define AOCD_1TR6              0x6d

/****************************************************************************/

#define	RING_INCOMING    1
#define RING_OUTGOING    2
#define	RING_RING        4
#define RING_CONNECT     8
#define RING_BUSY       16
#define RING_AOCD       32
#define RING_ERROR      64
#define RING_HANGUP    128
#define	RING_KILL      256
#define RING_SPEAK     512
#define RING_PROVIDER 1024
#define RING_LOOP     2048
#define RING_UNIQUE   4096
#define RING_INTERVAL 8192

/****************************************************************************/

#define STATE_RING		  1 /* "Telefonklingeln" ... jemand ruft an, oder man selbst ruft raus */
#define STATE_CONNECT	  	  2 /* Verbindung */
#define STATE_HANGUP	  	  3 /* Verbindung beendet */
#define STATE_AOCD		100 /* Gebuehrenimpuls _waehrend_ der Verbindung */
#define	STATE_CAUSE		101 /* Aussergewoehnliche Cause-Meldungen von der VSt */
#define STATE_TIME		102 /* Uhrzeit-Meldung von der VSt */
#define STATE_BYTE		103 /* Durchsatz-Meldung von Frank (Byte/s/B-Kanal) */
#define	STATE_HUPTIMEOUT	104 /* Wechsel des hangup-Timer's  */

/****************************************************************************/

#define AOC_130	           -999990L

/****************************************************************************/

#define QCMASK  	       0377
#define QUOTE   	       0200
#define QMASK      (QCMASK &~QUOTE)
#define NOT                     '!'

#define	AVON	             "avon"
#define INFO	    "/dev/isdninfo"

#define	BIGBUFSIZ              2048

/****************************************************************************/

#define VAR_START    "START"
#define VAR_MYMSNS   "MYMSNS"
#define VAR_MYPREFIX "MYPREFIX"
#define VAR_CURRENCY "CURRENCY"

/****************************************************************************/

#define VERSION_UNKNOWN       0
#define	VERSION_EDSS1	      1
#define	VERSION_1TR6	      2

/****************************************************************************/

#define  OTHER (call[chan].dialin ? CALLING : CALLED)
#define  ME    (call[chan].dialin ? CALLED : CALLING)
#define  _OTHER(call) (call->dialin ? CALLING : CALLED)
#define  _ME(call)    (call->dialin ? CALLED : CALLING)

/****************************************************************************/

#define SHORT_STRING_SIZE      256
#define LONG_STRING_SIZE      1024
#define BUF_SIZE              4096

/****************************************************************************/

#define QUOTE_IGNORE 0
#define QUOTE_DELETE 1

#define S_ALL_QUOTE "\\$@;,#"


/****************************************************************************/

typedef struct {
  int	  state;
  int     cref;
  int     tei;
  int	  dialin;
  int	  cause;
  int	  aoce;
  int	  channel;
  int	  dialog;
  int     bearer;
  int	  si1;     /* Service Indicator entsprechend i4l convention */
  int	  si11;	   /* if (si1 == 1) :: 0 = Telefon analog / 1 = Telefon digital */
  char    onum[2][NUMSIZE];
  char    num[2][NUMSIZE];
  char    vnum[2][256];
  char    id[32];
  char	  usage[16];
  int	  confentry[2];
  time_t  time;
  time_t  connect;
  time_t  t_duration;
  time_t  disconnect;
  clock_t duration;
  int     cur_event;
  long	  ibytes;
  long	  obytes;
  long	  libytes;
  long	  lobytes;
  double  ibps;
  double  obps;
  char	  vorwahl[2][NUMSIZE];
  char	  rufnummer[2][NUMSIZE];
  char	  alias[2][NUMSIZE];
  char	  area[2][128];
  char	  money[64];
  char	  currency[32];
  char    msg[128];
  int     stat;
  int	  version;
  int	  bchan;
  double  tick;
} CALL;

/****************************************************************************/

typedef struct {
  int	  flag;
  char *time;
  char *infoarg;
  int   interval;
} info_args;

/****************************************************************************/

typedef struct {
  char   *num;
  char	 *who;
  int	  zone;
  int	  flags;
  int	  si;
  char	 *interface;
  info_args **infoargs;
  /* above from "isdnlog.conf" */
  int	  usage[2];
  double  dur[2];
  int     eh;
  double  dm;
} KNOWN;

/****************************************************************************/

typedef struct {
  int    in;
  int    out;
  int    eh;
  int    err;
  double din;
  double dout;
  double dm;
  long	 ibytes;
  long	 obytes;
} sum_calls;

/****************************************************************************/

typedef struct {
  int    eh;
  int    cause;
  time_t t;
  int    dir;
  double duration;
  double dm;
  char   num[2][NUMSIZE];
  long	 ibytes;
  long	 obytes;
  double version;
  int	 si;
  int	 si2;
} one_call;

/****************************************************************************/

typedef struct {
  unsigned long i;
  unsigned long o;
} IO;

/****************************************************************************/

typedef struct {
  char id[20];
  int  ch;
  int  dr;
  int  u;
  int  f;
  char n[20];
} IFO;

/****************************************************************************/

PUBLIC KNOWN   start_procs;
PUBLIC KNOWN **known;
PUBLIC int     mymsns;
PUBLIC char   *myprefix;
PUBLIC int     knowns;
PUBLIC double  currency_factor;
PUBLIC char   *currency;
PUBLIC int     day;
PUBLIC int     month;
PUBLIC int     retnum;
PUBLIC int     ln;
PUBLIC char    retstr[MAXRET][RETSIZE];
PUBLIC char   *myname, *myshortname;
PUBLIC char   *Months[];
PUBLIC char   *CauseValue[];
PUBLIC datum   key, data;
PUBLIC DBM    *dbm;
PUBLIC time_t  cur_time;
PUBLIC double  einheit;

/****************************************************************************/

extern int   optind, errno;
extern char *optarg;

/****************************************************************************/

#ifdef _TOOLS_C_
#define _EXTERN
#else
#define _EXTERN extern
#endif

_EXTERN time_t atom(register char *p);
_EXTERN void   readconfig(char *myname);
_EXTERN void   discardconfig(void);
_EXTERN int    match(register char *s, register char *p);
_EXTERN char  *num2nam(char *num, int si);
_EXTERN char  *double2str(double n, int l, int d, int flags);
_EXTERN char  *double2byte(double bytes);
_EXTERN char  *time2str(time_t sec);
_EXTERN char  *double2clock(double n);
_EXTERN void   del_Array(char **Ptr);
_EXTERN char **String_to_Array(char* String, char Trenn);
_EXTERN char  *FGets(char *String, int StringLen, FILE *fp, int *Line);
_EXTERN char  *Check_Quote(char *String, char *Quote, int Flag);
_EXTERN char  *vnum(int chan, int who);
_EXTERN char  *i2a(int n, int l, int base);
_EXTERN char  *tr6cause(int cause);
_EXTERN char  *confdir(void);

#undef _EXTERN

#endif /* _TOOLS_H_ */
