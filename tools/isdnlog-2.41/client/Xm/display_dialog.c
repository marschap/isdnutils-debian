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

static	void	message_call (Widget w, CALL *info, XmAnyCallbackStruct *reason)
{
 switch		(reason->reason) {
   case XmCR_OK:	printf ("OK\tMsg = %s\n", info->msg);
   			break;
   case XmCR_CANCEL:	printf ("CANCEL\tMsg = %s\n", info->msg);
   			break;
   case XmCR_HELP:	printf ("HELP\tMsg = %s\n", info->msg);
   			break;
 }
 XtFree			((char *) info);
 XtUnmanageChild	(w);
 XtDestroyWidget	(w);
}

void	display_dialog	(CALL *info)
{
 Widget	dialog = XmCreateWorkingDialog (toplevel, "Working", NULL, 0);

 XtAddCallback	(dialog, XmNokCallback, (XtCallbackProc) message_call, (XtPointer) info);
 XtAddCallback	(dialog, XmNcancelCallback, (XtCallbackProc) message_call, (XtPointer) info);
 XtAddCallback	(dialog, XmNhelpCallback, (XtCallbackProc) message_call, (XtPointer) info);

 XtManageChild	(dialog);
}

