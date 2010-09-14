/* $Id: isdnlog.h,v 2.3.4 1996/04/05 11:30:43 root Exp $
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
 * $Log: isdnlog.h,v $
 * Revision 2.3.4  1996/04/05  11:30:43  akool
 * New ibytes/obytes ioctl() from Fritz implemented
 * USERFILE
 *
 * Revision 2.23  1996/03/14  18:07:43  akool
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

#ifndef _ISDNLOG_H_
#define _ISDNLOG_H_

/****************************************************************************/

#define PUBLIC extern

/****************************************************************************/

#include <tools.h>
#include "socket.h"

/****************************************************************************/

#define MAX_CALLS_IN_QUEUE    100
#define MAX_PRINTS_IN_QUEUE   500

/****************************************************************************/

#define ISDNCTRL    0
#define ISDNINFO    1
#define IN_PORT     2

#define FIRST_DESCR 3

/****************************************************************************/

#define PRT_ERR        	  	   1
#define PRT_WARN       	  	   2
#define PRT_INFO       	  	   4
#define PRT_PROG_OUT   	  	   4
#define PRT_NORMAL     	  	   4
#define PRT_LOG        	  	   8
#define PRT_SHOWNUMBERS	  	  16
#define PRT_SHOWAOCD   	  	  32
#define PRT_SHOWCONNECT	  	  64
#define PRT_SHOWHANGUP 	  	 128
#define PRT_SHOWCAUSE  	         256
#define PRT_SHOWTIME   	         512
#define PRT_SHOWBYTE   	        1024
#define	PRT_SHOWIMON		2048
#define PRT_DEBUG_GENERAL	4096
#define PRT_DEBUG_DIAG 	  	8192
#define PRT_DEBUG_INFO 	       16384
#define PRT_DEBUG_EXEC 	       32768
#define PRT_DEBUG_BUGS 	       65536
#define PRT_DEBUG_DECODE      131072
#define PRT_DEBUG_RING	      262144
#define PRT_DEBUG_CS	      524288
#define PRT_DEBUG_PROT	    0x100000
#define PRT_NOTHING   	    0x200000

#define IS_DEBUG(VALUE) (VALUE >= PRT_DEBUG_GENERAL)

/****************************************************************************/

#define AOC_INITIAL             0
#define AOCDCurrency            1
#define AOCDChargingUnit        2
#define AOCECurrency            3
#define AOCEChargingUnit        4
#define AOCDCurrencyInfo        5
#define AOCECurrencyInfo        6
#define AOCDChargingUnitInfo    7
#define AOCEChargingUnitInfo    8
#define RecordedCurrency       	9
#define TypeOfChargingInfo     10
#define Currency               11
#define Amount                 12
#define CurrencyAmount         13
#define Multiplier             14
#define RecordedUnitsList      15
#define RecordedUnits          16
#define NumberOfUnits          17

/****************************************************************************/

#ifndef USERFILE
#define USERFILE "isdnlog.users"
#endif

/****************************************************************************/

typedef struct _interval {
	int        event;
	int        chan;
	info_args *infoarg;
	time_t     next_start;
	struct _interval *next;
}interval;

/****************************************************************************/

CALL call[MAXCHAN];

/****************************************************************************/

#ifdef _ISDNLOG_C_
#define _EXTERN
socket_queue *sockets = NULL;
#else
#define _EXTERN extern
extern socket_queue *sockets;
#endif

_EXTERN void   Exit(int RetCode);
_EXTERN void   exit_on_signal(int Sign);
_EXTERN int    print_msg(int Level, const char *fmt, ...);

#undef _EXTERN

/****************************************************************************/

#ifdef _SERVER_C_
#define _EXTERN
#else
#define _EXTERN extern
#endif

_EXTERN int message_from_server(CALL *call, int chan);
_EXTERN int eval_message(int sock);
_EXTERN int start_server(int port);
_EXTERN int listening(int port);
_EXTERN int print_from_server(char *String);
_EXTERN int change_channel(int old_chan, int new_chan);
_EXTERN int disconnect_client(int sock);

#undef _EXTERN

/****************************************************************************/

#ifdef _START_PROG_C_
#define _EXTERN
#else
#define _EXTERN extern
#endif

_EXTERN int Ring(char *, char *[], int, int);
_EXTERN void  Alarm(void);
_EXTERN int   CheckTime(char *);
_EXTERN int   Print_Cmd_Output( int sock );
_EXTERN int Get_Sock_From_Call_And_Info_Args( int chan, info_args *Ptr, int Cnt );
_EXTERN int Get_Sock_From_Call( int chan, int Cnt );
_EXTERN int Get_Sock_From_Info_Args( info_args *Ptr, int Cnt );
_EXTERN int Condition_Changed( int condition, int flag );
_EXTERN const char *Set_Ringer_Flags( int condtion, int InOut );
_EXTERN int Start_Ring(int chan, info_args *infoarg, int event, int intervalflag);
_EXTERN int Start_Process(int chan, info_args *infoarg, int event);
_EXTERN int New_Interval(int chan, info_args *infoarg, int event);
_EXTERN int Del_Interval(int chan, info_args *infoarg);
_EXTERN struct timeval *Get_Interval(void);
_EXTERN int Change_Channel_Ring( int old_channel, int new_channel);
_EXTERN int Start_Interval(void);

#undef _EXTERN

/****************************************************************************/

#ifdef _USER_ACCESS_C_
#define _EXTERN
#else
#define _EXTERN extern
#endif

_EXTERN int read_user_access( void );
_EXTERN int write_user_access( void );
_EXTERN int user_has_access(char *User, char *Host);
_EXTERN int User_Get_Message(char *User, char *Host, char* mymsn, int Flag);
_EXTERN const char *userfile(void);

#undef _EXTERN

/****************************************************************************/

#endif /* _ISDNLOG_H_ */
