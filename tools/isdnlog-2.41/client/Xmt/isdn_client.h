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

#ifndef _ISDN_CLIENT_H
#define _ISDN_CLIENT_H

#include "../client.h"
#include <Xmt/Xmt.h>
#include <Xmt/InputField.h>
#include <Xmt/Create.h>
#include <X11/Intrinsic.h>
#include <Xm/MessageB.h>

typedef struct {
  Widget	dialog;
  String	phone, faxnr, fname, lname, email, street, city, country;
  socket_queue	*socket;
} NewCallerStruct, *newCallerStruct;

extern void isdn_main_loop	(void);
extern void isdn_initialize	(Cardinal*, String []);
extern void isdn_add_connection (socket_queue *);
extern void isdn_list_update	(int, CALL *);
extern void isdn_current_list_item_add		(int, CALL *);
extern void isdn_closed_list_delete		(void);
extern void isdn_closed_list_item_add		(int, CALL *);
extern void isdn_current_list_item_change	(int, CALL *);
extern void isdn_current_list_channel_change (socket_queue *);
extern void isdn_new_caller_display		(socket_queue *);
extern Widget isdn_current_list_get		( void );
extern Widget isdn_closed_list_get		( void );
extern Widget isdn_prot_wid_get			( void );
extern void isdn_prot_clear			( void );
extern void isdn_prot_update			( void );
extern void isdn_prot				( socket_queue * );
extern void isdn_error				( int );
extern int  compare_version (char *Version);
extern char *isdn_error_get			( int );
extern Widget main_create	(Widget, String);

extern  void cb_call    (Widget, XtPointer, XmAnyCallbackStruct *);
extern  void pb_call    (Widget, XtPointer, XmPushButtonCallbackStruct *);
extern  void radio_call (Widget, XtPointer, XmToggleButtonCallbackStruct *);
extern  void check_call (Widget, XtPointer, XmToggleButtonCallbackStruct *);

extern XtAppContext	context;
extern Widget		toplevel;

#endif
