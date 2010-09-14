/*
** X11/Motif interface for ISDN accounting for isdn4linux.
**
** Copyright 1996 by Oleg von Styp Rekowski (os@Kool.f.EUnet.de)
**
** This revision is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2, or (at your option)
** any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Revision: 0.01       Date: 10.02.96          Author: os
*/

#include <isdn_client.h>
#include <Xm/List.h>
/*                     Date    Duration    Money Currcy  MSN   Dir  Call    City     Datenrate  */
#define CURRENT_FORMAT	"%s %02d:%02d:%02d %6.6s %-3.3s %-10.10s %s %-15.15s %-13.13s %s %s %s %s"
/*                   Date    Duration  Money Currcy     MSN  Dir  Call     City    Status  Datenrate  */
#define CLOSED_FORMAT	"%s %02d:%02d:%02d %6.6s %-3.3s %-10.10s %s %-15.15s %-13.13s %-25.25s %s %s"
#define UNKNOWN "UNKNOWN"

static int *stack = NULL;
static int count = 0;

static void set_byte_string(char* String, double Bytes)
{
	if (!Bytes)
		strcpy(String,"       ");
	else
	if (Bytes >= 1073741824)
		sprintf(String,"%5.1fGB",Bytes/1073741824);
	else
	if (Bytes >= 1048576)
		sprintf(String,"%5.1fMB",Bytes/1048576);
	else
	if (Bytes >= 1024)
		sprintf(String,"%5.1fkB",Bytes/1024);
	else
	if (Bytes < 1024)
		sprintf(String,"%5ld B",(long int) Bytes);
}

static void set_bps_string(char* String, long int Bytes)
{
	if (!Bytes)
		strcpy(String,"       ");
	else
	if (Bytes >= 1073741824)
		sprintf(String,"%5.1fGB/s",((double)Bytes)/1073741824);
	else
	if (Bytes >= 1048576)
		sprintf(String,"%5.1fMB/s",((double)Bytes)/1048576);
	else
	if (Bytes >= 1024)
		sprintf(String,"%5.1fkB/s",((double)Bytes)/1024);
	else
	if (Bytes < 1024)
		sprintf(String,"%5ld B/s",Bytes);
}

void isdn_current_list_item_add    (int channel, CALL *info)
{
 char		*ptr1 = info->num[_ME(info)][0] ? info->num[_ME(info)] : UNKNOWN,
        *ptr0 = info->alias[_OTHER(info)][0] ? info->alias[_OTHER(info)] : info->num[_OTHER(info)][0] ? info->num[_OTHER(info)] : UNKNOWN,
        *ptr  = info->dialin ? "<-" : "->",
        Bytes[4][32],
		buf[256], time_string[256];
 struct tm	*lt = localtime (&info->t_duration), dur;

 Widget		list = isdn_current_list_get ();
 XmString	string =  NULL;

 dur.tm_hour =	lt->tm_hour - 1,
 dur.tm_min =	lt->tm_min,
 dur.tm_sec =	lt->tm_sec;
 lt = localtime (&info->connect);

 set_byte_string(Bytes[0],info->ibytes);
 set_byte_string(Bytes[1],info->obytes);
 set_bps_string(Bytes[2],info->ibps);
 set_bps_string(Bytes[3],info->obps);

 strftime	(time_string, 256, "%b %d %X", lt);
 sprintf	(buf, CURRENT_FORMAT, time_string,
    dur.tm_hour, dur.tm_min, dur.tm_sec, info->money, info->currency,
 		ptr1, ptr, ptr0, info->area[0], Bytes[0], Bytes[1], Bytes[2], Bytes[3]);
 string =	XmStringCreateSimple (buf);
 XmListAddItem	(list, string, 0);
 XmStringFree	(string);

 stack =	(int *) XtRealloc ((char *) stack,
		    (Cardinal) (count + 1) * sizeof (int));

 stack[count++] = channel;
}


void isdn_closed_list_delete     ()
{
 int	*items, count;
 Widget list = isdn_closed_list_get ();

 XmListGetSelectedPos	(list, &items, &count);
 XmListDeletePositions	(list, items, count);
 XtFree			((char *) items);
}


void isdn_closed_list_item_add     (int channel, CALL *info)
{
 int		pos, i;
 char		*ptr1 = info->num[_ME(info)][0] ? info->num[_ME(info)] : UNKNOWN,
        *ptr0 = info->alias[_OTHER(info)][0] ? info->alias[_OTHER(info)] : info->num[_OTHER(info)][0] ? info->num[_OTHER(info)] : UNKNOWN,
        *ptr  = info->dialin ? "<-" : "->",
        Bytes[4][32],
 		buf[256], time_string[256];
 Widget		list;
 XmString	string;
 struct tm	*lt = localtime (&info->t_duration), dur;

 dur.tm_hour =	lt->tm_hour - 1,
 dur.tm_min =	lt->tm_min,
 dur.tm_sec =	lt->tm_sec;

 lt = localtime (&info->connect);

 set_byte_string(Bytes[0],info->ibytes);
 set_byte_string(Bytes[1],info->obytes);
 set_bps_string(Bytes[2],info->ibps);
 set_bps_string(Bytes[3],info->obps);

 strftime	(time_string, 256, "%b %d %X", lt);
 sprintf	(buf, CLOSED_FORMAT, time_string,
  dur.tm_hour, dur.tm_min, dur.tm_sec,
  info->money, info->currency, ptr1, ptr, ptr0,
	info->area[0],info->msg,Bytes[0],Bytes[1]/*,Bytes[2],Bytes[3]*/);
 string =	XmStringCreateSimple (buf);
 XmListAddItem	(list = isdn_closed_list_get (), string, 0);
 XmListSetBottomPos (list, 0);
 XmStringFree	(string);

 if		(count < 1)
   return;

 for		(pos = 0; pos < count; pos++)
   if		(channel == stack[pos])
     break;

 if (pos == count)
 	return;

 for		(count--, i = pos; i < count; i++)
   stack[i] =	stack[i+1];

 list =		isdn_current_list_get ();
 if		(count > 0) {
   stack =	(int *) XtRealloc ((char *) stack, (Cardinal) count * sizeof (int));
   XmListDeletePos (list, pos + 1);

 } else {
   XtFree	((char *) stack); stack = NULL;
   XmListDeletePos (list, 0);
 }
}


void isdn_current_list_item_change (int channel, CALL *info)
{
 int		pos;

 for	(pos = 0; pos < count; pos++)
   if	(channel == stack[pos])
	break;

 if				(pos == count)
   isdn_current_list_item_add	(channel, info);

 else {
 char		*ptr1 = info->num[_ME(info)][0] ? info->num[_ME(info)] : UNKNOWN,
        *ptr0 = info->alias[_OTHER(info)][0] ? info->alias[_OTHER(info)] : info->num[_OTHER(info)][0] ? info->num[_OTHER(info)] : UNKNOWN,
        *ptr  = info->dialin ? "<-" : "->",
        Bytes[4][32],
		buf[256], time_string[256];
   Widget	list = isdn_current_list_get ();
   XmString	string;
   struct tm	*lt = localtime (&info->t_duration), dur;

   dur.tm_hour	= lt->tm_hour - 1,
   dur.tm_min	= lt->tm_min,
   dur.tm_sec	= lt->tm_sec;

   set_byte_string(Bytes[0],info->ibytes);
   set_byte_string(Bytes[1],info->obytes);
   set_bps_string(Bytes[2],info->ibps);
   set_bps_string(Bytes[3],info->obps);

   lt =		localtime (&info->connect);
   strftime	(time_string, 256, "%b %d %X", lt);
   sprintf	(buf, CURRENT_FORMAT, time_string,
    lt->tm_hour, lt->tm_min, lt->tm_sec,
    info->money, info->currency, ptr1, ptr, ptr0,
 		info->area[0],Bytes[0],Bytes[1],Bytes[2],Bytes[3]);
   string =		XmStringCreateSimple (buf); pos += 1;
   XmListDeletePos	(list, pos);
   XmListAddItem	(list, string, pos);
   XmStringFree		(string);
 }
}

void isdn_current_list_channel_change (socket_queue *sock)
{
 int  pos = 0;
 int  old_channel = 0;
 int  new_channel = 0;
 char FormatString[SHORT_STRING_SIZE];

 sprintf(FormatString,"%%d%c%%d",C_DELIMITER);

 if (sscanf(sock->msgbuf.buf,FormatString,&old_channel,&new_channel) != 2)
   isdn_error(6);

 for   (pos = 0; pos < count; pos++)
   if  (old_channel == stack[pos])
   {
     stack[pos] = new_channel;
     break;
   }
}
