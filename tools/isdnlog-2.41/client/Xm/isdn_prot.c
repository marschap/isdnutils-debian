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
#include <Xm/TextF.h>
#include <Xm/Text.h>
#include <Xm/LabelG.h>
#include <Xm/SelectioB.h>
#include <Xm/FileSB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/List.h>
#include <Xm/ToggleBG.h>

#define MAX_UPDATE_COUNT 10000


static unsigned int	max_lines = 1000;
static unsigned int	line_count = 0;
static Widget		dialog = NULL, list_widget = NULL, fsb_dialog = NULL;

static XmStringTable	update_list = NULL;
static int		update_count = 0;


void isdn_prot_update	()
{
 register int i;

 if		(!update_count)
   return;

 if		(max_lines == 0 || (line_count + update_count) < max_lines)
   line_count += update_count;

 else
   XmListDeleteItemsPos	(list_widget, line_count + update_count - max_lines, 1);

 XmListAddItems		(list_widget, update_list, update_count, 0);
 XmListSetBottomPos	(list_widget, 0);
 for			(i = 0; i < update_count; i++)
   XmStringFree		(update_list[i]);
 XtFree			((char *) update_list);
 update_list = NULL;
 update_count = 0;
}


void isdn_prot	(socket_queue *socket)
{
 int      max_update_count = max_lines ? max_lines : MAX_UPDATE_COUNT;
 XmString item = XmStringCreateSimple (socket->msgbuf.buf);

 if			(!update_list)
   update_list =	(XmStringTable) XtCalloc (max_update_count, sizeof (XmString));
 
 if			(update_count < max_update_count)
   update_list [update_count++] = item;

 else {
   register int i;
   XmStringFree		(update_list [0]);
   for			(i = 0; i < max_update_count; i++)
     update_list [i] =	update_list[i+1];
 }
}


void isdn_prot_clear	()
{
 line_count = 0;
 XmListDeleteAllItems	(list_widget);
}


static	void isdn_prot_max_set	(Widget text, XtPointer client_data,
			 XmAnyCallbackStruct *reason)
{
 char	*inhalt =	XmTextGetString (text);

 if	(!inhalt)
 	return;

 if	(isdigit (inhalt[0])) {
   int	new_max_lines	= atoi (inhalt);
   int	count		= line_count - new_max_lines;

   if		(new_max_lines < max_lines && count > 0) {
     int i, *pos = (int *) XtCalloc ((Cardinal) count, sizeof (int));
     for		(i = 1; i <= count; i++)
        pos[i] = i;
     XmListDeletePositions (list_widget, pos, count);
     XtFree	((char *) pos);
     line_count = new_max_lines;
   }
   max_lines = new_max_lines;
 }

 XtFree	(inhalt);
}

static Boolean isdn_prot_overwrite ()
{
 Widget tb = XtNameToWidget (fsb_dialog, "*button_1");
 return	(tb ? XmToggleButtonGadgetGetState (tb) : False);
}


static void fsb_call	(Widget fsbw, XtPointer client_data,
			XmFileSelectionBoxCallbackStruct *reason)
{
 Arg		arg[3];
 int		*positions, count, i;
 FILE		*fp;
 char *		filename = NULL, *line;
 char *		mode = isdn_prot_overwrite () ? "w" : "a";
 XmStringTable	items;

 switch		(reason->reason) {
   case XmCR_OK:
	if		(!XmStringGetLtoR (reason->value, "", &filename))
	  break;

	if		(!(fp = fopen (filename, mode))) {
	  char		message [512];
	  sprintf	(message, isdn_error_get (5), filename);
	  DisplayError	(message);
	  XtFree	(filename);
	  break;
	}
	XtFree		(filename);

	XtSetArg	(arg[0], XmNitems, &items);
	XtSetArg	(arg[1], XmNselectedPositions, &positions);
	XtSetArg	(arg[2], XmNselectedPositionCount, &count);
	XtGetValues	(list_widget, arg, 3);
	if		(count) {
	  for		(i = 0; i < count; i++)
	    if		(XmStringGetLtoR (items[positions[i]-1], "", &line)) {
	      fprintf	(fp, "%s\n", line);
	      XtFree	(line);
	    }
	  XmListDeselectAllItems (list_widget);

	} else for	(i = 0; i < line_count; i++)
	    if		(XmStringGetLtoR (items[i], "", &line)) {
	      fprintf	(fp, "%s\n", line);
	      XtFree	(line);
	    }
	fclose		(fp);
	XtUnmanageChild	(fsbw);
   	break;

   case XmCR_CANCEL:
   	XtUnmanageChild (fsbw);
   	break;

   case XmCR_HELP:
   	break;
 }
}


static	void	isdn_fsb_init ()
{
 Arg		arg[5];
 XtSetArg	(arg[0], XmNautoUnmanage,	False);
 XtSetArg	(arg[1], XmNresizePolicy,	XmRESIZE_NONE);
 fsb_dialog =	XmCreateFileSelectionDialog (toplevel, "IsdnProtSave", arg, 2);
 XtAddCallback	(fsb_dialog, XmNokCallback, (XtCallbackProc) fsb_call, NULL);
 XtAddCallback	(fsb_dialog, XmNcancelCallback, (XtCallbackProc) fsb_call, NULL);
 XtManageChild	(XmCreateSimpleRadioBox	(fsb_dialog, "IsdnProtRB", arg, 0));
}

static void sel_call	(Widget w, XtPointer client_data,
		XmSelectionBoxCallbackStruct *reason)
{
 switch		(reason->reason) {
   case XmCR_OK:
	if		(!line_count) {
	  isdn_error	(4);
	  break;
	}

	if		(!fsb_dialog)
   	  isdn_fsb_init	();

	XtManageChild	(fsb_dialog);

   	break;

   case XmCR_CANCEL:
   	XtUnmanageChild (w);
   	break;

   case XmCR_APPLY:
   	isdn_prot_clear ();
   	break;

   case XmCR_HELP:
   	break;
 }
}


Widget	isdn_prot_wid_get ()
{
 if		(!dialog) {
   Arg		arg[5];
   Widget	form, child[2];
   XtSetArg	(arg[0], XmNautoUnmanage,	False);
   XtSetArg	(arg[1], XmNresizePolicy,	XmRESIZE_NONE);
   dialog =	XmCreateSelectionDialog (toplevel, "IsdnProt", arg, 2);
   XtAddCallback(dialog, XmNapplyCallback, (XtCallbackProc) sel_call, NULL);
   XtAddCallback(dialog, XmNokCallback, (XtCallbackProc) sel_call, NULL);
   XtAddCallback(dialog, XmNcancelCallback, (XtCallbackProc) sel_call, NULL);
   XtUnmanageChild	(XmSelectionBoxGetChild (dialog, XmDIALOG_LIST_LABEL));
   XtUnmanageChild	(XmSelectionBoxGetChild (dialog, XmDIALOG_SELECTION_LABEL));
   XtUnmanageChild	(XmSelectionBoxGetChild (dialog, XmDIALOG_TEXT));
   list_widget	= XmSelectionBoxGetChild (dialog, XmDIALOG_LIST);
   XtSetArg	(arg[0], XmNselectionPolicy,	XmEXTENDED_SELECT);
   XtSetValues	(list_widget, arg, 1);
   form =	XmCreateForm (dialog, "IsdnProtForm", arg, 0);
   XtSetArg	(arg[0], XmNtopAttachment,	XmATTACH_FORM);
   XtSetArg	(arg[1], XmNleftAttachment,	XmATTACH_FORM);
   child[0] =	XmCreateLabelGadget (form, "IsdnProtLabel", arg, 2);
   XtSetArg	(arg[0], XmNtopAttachment,	XmATTACH_FORM);
   XtSetArg	(arg[1], XmNleftAttachment,	XmATTACH_WIDGET);
   XtSetArg	(arg[2], XmNleftWidget,		child[0]);
   XtSetArg	(arg[3], XmNmaxLength,		5);
   XtSetArg	(arg[4], XmNcolumns,		5);
   child[1] =	XmCreateTextField (form, "IsdnProtText", arg, 5);
   XtAddCallback(child[1], XmNactivateCallback,
		(XtCallbackProc) isdn_prot_max_set, NULL);
   XtManageChildren	(child, 2);
   XtManageChild	(form);
 }

 return dialog;
}


