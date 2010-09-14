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
** Revision: 0.01	Date: 10.02.96		Author: os
*/

#include <isdn_client.h>
#include <Xm/List.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/ScrollBar.h>
#include <Xm/MenuShell.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeBG.h>
#include <Xm/ToggleBG.h>
#include <Xm/PanedW.h>

#define	LIST_COUNT 2
#define	RowColumnCount 1

static	Widget	list_wid[LIST_COUNT];

static	Widget	create_menus	(Widget main_wid, Widget cascade_button, String	menu_name)
/*
** Returns the MenuBar and creates recurrently all the PulldowMenus of the MenuBar.
*/

#if 0
{ 
 WidgetList	buttons;
 Cardinal	count = 0, n = 0, i;
 Boolean	radio, behavior;
 Widget		menu;
 Arg		args[10];
 char		name [256];

 if		(cascade_button) {	/* Creating PulldownMenus */
   XtSetArg	(args[n], XmNsimpleCallback, (XtCallbackProc) pb_call);	n++;
   menu =  XmCreateSimplePulldownMenu (main_wid, menu_name, args, n);	n=0;

 } else {	/* Creating MenuBar */
   XtSetArg	(args[n], XmNsimpleCallback, (XtCallbackProc) cb_call);	n++;
   menu =	XmCreateSimpleMenuBar (main_wid, menu_name, args, n);	n=0;
 }

 /* Read all of its children created automaticly from the Resource data base */
 XtSetArg	(args[n], XmNchildren, &buttons);			n++;
 XtSetArg	(args[n], XmNnumChildren, &count);			n++;
 XtSetArg	(args[n], XmNradioAlwaysOne, &radio);			n++;
 XtSetArg	(args[n], XmNradioBehavior, &behavior);			n++;
 XtGetValues	(menu, args, n);					n=0;

 /* Creating PulldownMenus for all CascadeButtons */
 for		(i = 0; i < count; i++)
   if		(XtIsSubclass (buttons[i], xmCascadeButtonGadgetClass)) {
     sprintf	(name, "%s_pd_%d", menu_name, i);
     XtSetArg	(args[n], XmNsubMenuId, create_menus
		(main_wid, buttons[i], name));				n++;
     XtSetValues(buttons[i], args, n);					n=0;
   }

 /* The last CascadeButton of the MenuBar is the HelpWidget */
 if		(!cascade_button && count > 0) {
   XtSetArg	(args[n], XmNmenuHelpWidget, buttons[count-1]);		n++;
   XtSetValues	(menu, args, n);					n=0;
 }

 return		menu;
}

#else
{
 WidgetList	buttons;
 Cardinal	count = 0, n = 0, i;
 Boolean	radio, radio_behavior;
 Widget		menu;
 Arg		args[10];
 char		name [256];

 if		(cascade_button) {	/* Creating PulldownMenus */
   XtSetArg	(args[n], XmNsimpleCallback, (XtCallbackProc) pb_call);	n++;
   menu =  XmCreateSimplePulldownMenu (main_wid, menu_name, args, n);	n=0;

 } else {	/* Creating MenuBar */
   XtSetArg	(args[n], XmNsimpleCallback, (XtCallbackProc) cb_call);	n++;
   menu =	XmCreateSimpleMenuBar (main_wid, menu_name, args, n);	n=0;
 }

 /* Read all of its children from the resource data base */
 XtSetArg	(args[n], XmNchildren, &buttons);			n++;
 XtSetArg	(args[n], XmNnumChildren, &count);			n++;
 XtSetArg	(args[n], XmNradioAlwaysOne, &radio);			n++;
 XtSetArg	(args[n], XmNradioBehavior, &radio_behavior);		n++;
 XtGetValues	(menu, args, n);					n=0;

 if		(radio_behavior) {	/* Radio- oder CheckBox */
   XmString *	labels = (XmString *) XtCalloc (count, sizeof (XmString));
   XmString *	actext = (XmString *) XtCalloc (count, sizeof (XmString));
   String *	accel  = (String *)   XtCalloc (count, sizeof (String));
   KeySym *	mnemo  = (KeySym *)   XtCalloc (count, sizeof (KeySym));
   for		(i = 0; i < count; i++) {
     XtSetArg	(args[n], XmNmnemonic, &(mnemo[i]));			n++;
     XtSetArg	(args[n], XmNaccelerator, &(accel[i]));			n++;
     XtSetArg	(args[n], XmNlabelString, &(labels[i]));		n++;
     XtSetArg	(args[n], XmNacceleratorText, &(actext[i]));		n++;
     XtGetValues(buttons[i], args, n);					n=0;
   }
   XtDestroyWidget (menu);

   XtSetArg	(args[n], XmNradioBehavior, radio);			n++;
   XtSetArg	(args[n], XmNradioAlwaysOne, radio);			n++;
   menu =	XmCreatePulldownMenu (main_wid, menu_name, args, n);    n=0;

   buttons =	(Widget *) XtCalloc (count, sizeof (Widget));
   for		(i = 0; i < count; i++) {
     sprintf	(name, "%s_button_%d", menu_name, i);
     if		(radio && i == 0) {
       XtSetArg	(args[n], XmNset, True);                                    n++;
     }
     XtSetArg	(args[n], XmNmnemonic, mnemo[i]);                           n++;
     XtSetArg	(args[n], XmNaccelerator, accel[i]);                        n++;
     XtSetArg	(args[n], XmNlabelString, labels[i]);                       n++;
     XtSetArg	(args[n], XmNacceleratorText, actext[i]);                   n++;
     XtSetArg	(args[n], XmNindicatorType,
		radio ? XmONE_OF_MANY : XmN_OF_MANY);                       n++;
     buttons[i] = XmCreateToggleButtonGadget (menu, name, args, n);         n=0;
     XtAddCallback (buttons[i], XmNvalueChangedCallback, (XtCallbackProc) radio_call, NULL);
     XmStringFree  (labels[i]);
     XmStringFree  (actext[i]);
     XtFree        ((char *) accel[i]);
   }

   XtManageChildren (buttons, i);
   XtFree           ((char *) buttons);
   XtFree           ((char *) labels);
   XtFree           ((char *) actext);
   XtFree           ((char *) accel);
   XtFree           ((char *) mnemo);

    /* Creating PulldownMenus for all CascadeButtons */
 } else	for	(i = 0; i < count; i++)
   if		(XtIsSubclass (buttons[i], xmCascadeButtonGadgetClass)) {
     sprintf	(name, "%s_pd_%d", menu_name, i);
     XtSetArg	(args[n], XmNsubMenuId, create_menus
		(main_wid, buttons[i], name));				n++;
     XtSetValues(buttons[i], args, n);					n=0;
   }

 /* The last CascadeButton of the MenuBar is the HelpWidget */
 if		(!cascade_button && count > 0) {
   XtSetArg	(args[n], XmNmenuHelpWidget, buttons[count-1]);		n++;
   XtSetValues	(menu, args, n);					n=0;
 }

 return		menu;
}

#endif

Widget	main_create ( Widget parent, String main_name)
/*
** Returns the MainWindow widget
*/
{
 Widget		main_window, form, rowcol, pwindow, child[RowColumnCount];
 char		name [256];
 Arg		args[10];
 Cardinal	n = 0, i;

 /* Creating MainWindow */
 main_window =	XmCreateMainWindow (parent, main_name, args, n);	n=0;

 /* Creating MenuBar and its all PulldownMenus */
 XtManageChild	(create_menus (main_window, NULL, "MenuBar"));

 /* Creating PanedWindow and its 2 children (ListWidgets) */
 form =		XmCreateForm (main_window, "MainForm", args, n); n=0;

 XtSetArg	(args[n], XmNtopAttachment, XmATTACH_FORM);		n++;
 XtSetArg	(args[n], XmNleftAttachment, XmATTACH_FORM);		n++;
 XtSetArg	(args[n], XmNrightAttachment, XmATTACH_FORM);		n++;
 XtSetArg	(args[n], XmNrightAttachment, XmATTACH_FORM);		n++;
 rowcol =	XmCreateRowColumn (form, "RowColumn", args, n);		n=0;
 for		(i = 0; i < RowColumnCount; i++) {
   sprintf	(name, "RowColumn_%d", i);
   switch	(i) {
     case 0:	child [i] = XmCreateLabel (rowcol, name, NULL, 0); break;
     default:	child [i] = XmCreatePushButton (rowcol, name, NULL, 0); break;
   }
 }
 XtManageChildren (child, i);

 XtSetArg	(args[n], XmNtopWidget, rowcol);			n++;
 XtSetArg	(args[n], XmNtopAttachment, XmATTACH_WIDGET);		n++;
 XtSetArg	(args[n], XmNleftAttachment, XmATTACH_FORM);		n++;
 XtSetArg	(args[n], XmNrightAttachment, XmATTACH_FORM);		n++;
 XtSetArg	(args[n], XmNbottomAttachment, XmATTACH_FORM);		n++;
 pwindow =	XmCreatePanedWindow (form, "PanedWindow", args, n);	n=0;
 XtSetArg	(args[n], XmNlistSizePolicy, XmCONSTANT);		n++;
 XtSetArg	(args[n], XmNselectionPolicy, XmEXTENDED_SELECT);	n++;
 list_wid[0] =	XmCreateScrolledList (pwindow, "CurrentList", args, n);
 list_wid[1] =	XmCreateScrolledList (pwindow, "ClosedList", args, n);	n=0;
 XtManageChild	(list_wid[0]);
 XtManageChild	(list_wid[1]);
 XtManageChild	(rowcol);
 XtManageChild	(pwindow);
 XtManageChild	(form);
 (void)		isdn_prot_wid_get ();
 return		main_window;
}

Widget isdn_current_list_get ()
/*
** Returns the ListWidget of current calls
*/
{
 return list_wid[0];
}

Widget isdn_closed_list_get ()
/*
** Returns the ListWidget of closed calls
*/
{
 return list_wid[1];
}
