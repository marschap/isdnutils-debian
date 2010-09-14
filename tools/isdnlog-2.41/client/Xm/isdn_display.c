#include <isdn_client.h>
#include <Xm/MessageB.h>


static Widget widget = NULL;


void isdn_new_caller_display (socket_queue *socket)
{
}


void DisplayMessage (unsigned char dialog_type, String message)
{
 XmString	label = XmStringCreateSimple (message);
 String		title = dialog_type == XmDIALOG_ERROR ? "Error" : "Information";
 Cardinal	n = 0;
 Arg		arg[6];

 XtSetArg	(arg[n], XmNtitle, title);                                  n++;
 XtSetArg	(arg[n], XmNlabelString, label);                            n++;
 XtSetArg	(arg[n], XmNdialogType, dialog_type);                       n++;

 if		(!widget) {
   XtSetArg	(arg[n], XmNdefaultPosition, True);                         n++;
   XtSetArg	(arg[n], XmNautoUnmanage, True);                            n++;
   widget	= XmCreateMessageDialog (toplevel, "ErrorDialog", arg, n);  n=0;
   XtUnmanageChild (XmMessageBoxGetChild (widget, XmDIALOG_CANCEL_BUTTON));
   XtUnmanageChild (XmMessageBoxGetChild (widget, XmDIALOG_HELP_BUTTON));

 } else {
   XtSetValues	(widget, arg, n);                                           n=0;
 }

 XtManageChild	(widget);
 XmStringFree	(label);
}


void DisplayInformation (String message)
{
 DisplayMessage		(XmDIALOG_INFORMATION, message);
}


void DisplayError	(String message)
{
 DisplayMessage		(XmDIALOG_ERROR, message);
}

