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

Widget		toplevel;
XtAppContext	context;


void	isdn_initialize (Cardinal *argc, String argv[])
{
 Cardinal       n = 0;
 toplevel =     XmtInitialize (&context, "Isdn", NULL, 0,
                argc, argv, NULL, NULL, n);
}


void    isdn_main_loop  ()
{
 XmtRegisterAll    ();
 XtManageChild     (main_create (toplevel, "MainWindow"));
 XmtCreateChildren (toplevel);
 XtRealizeWidget   (toplevel);
 XtAppMainLoop     (context);
}
