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


static int isdn_disconnect (socket_queue *source)
{
  XtRemoveInput	((XtInputId) source->input_id);
  close		(source->descriptor);

  if		(del_socket (&source,0))
    return -1;

  return 0;
}


static	void	input_proc (socket_queue *source, int *fid, XtInputId *id)
{
 int	size;

 do

   if		((size = Read (source)) < 0) {
     XmtDisplayError (toplevel, NULL, "@f[BIG]Verbindung zum Server abgebrochen!", "Fataler Fehler");
     isdn_disconnect (source);

   } else {
     source->eval (source);
   }

 while	(source->status == NEXT_MSG);

 isdn_prot_update ();
}


void	isdn_add_connection	(socket_queue * source)
{
 XtInputId	input;

 input	= XtAppAddInput (context, source->descriptor,
			(XtPointer)		XtInputReadMask,
			(XtInputCallbackProc)	input_proc,
			(XtPointer)		source);

 source->input_id = (int) input;
}

extern int  compare_version (char *Version)
{
	char Warning[SHORT_STRING_SIZE];


	if (strcmp(PROT_VERSION,Version))
	{
/* Oleg: ueberarbeite mal die naechste Zeile */
		sprintf(Warning,"@f[BIG]Server und Client haben\nunterschiedliche Protkollversionen!\nServer: %s\nClient: %s\n",Version,PROT_VERSION);
		XmtDisplayError (toplevel, NULL, Warning, "Warnung");
		return -1;
	}

	return 0;
}
