#include <isdn_client.h>
#include <Xmt/Layout.h>
#include <Xm/PushB.h>


static void destroy_call(Widget w, newCallerStruct new_caller, XmAnyCallbackStruct *reason)
{
 printf			("NewCaller ignoriert!\n");
 XtFree			((char *) new_caller);
}


static void ok_call	(Widget w, newCallerStruct new_caller, XmAnyCallbackStruct *reason)
{
 char	*adress;
 String	string;
 Address * data = XtNew (Address);

 data->NumAdr = 1; data->Adr = (Address *) XtCalloc (1, sizeof (Address));
 data->Birthday = NULL;

 string = XmtInputFieldGetString (XmtNameToWidget (new_caller->dialog, "phone"));
 if	((data->Adr->NumTel = string ? 1 : 0) == 1) {
	data->Adr->Tel = (PhoneNumber *) XtCalloc (1, sizeof (PhoneNumber));
	data->Adr->Tel->Number = XtNewString (string);
 } else
	data->Adr->Tel = NULL;

 string = XmtInputFieldGetString (XmtNameToWidget (new_caller->dialog, "faxnr"));
 if	((data->Adr->NumFax = string ? 1 : 0) == 1) {
	data->Adr->Fax = (PhoneNumber *) XtCalloc (1, sizeof (PhoneNumber));
	data->Adr->Fax->Number = XtNewString (string);
 } else
	data->Adr->Fax = NULL;

 string = XmtInputFieldGetString (XmtNameToWidget (new_caller->dialog, "email"));
 if	((data->Adr->NumEmail = string ? 1 : 0) == 1) {
	data->Adr->Email = (char **) XtCalloc (1, sizeof (char *));
	data->Adr->Email[0] = XtNewString (string);
 } else
	data->Adr->Email = NULL;

 string = XmtInputFieldGetString (XmtNameToWidget (new_caller->dialog, "lname"));
 data->NName = string ? XtNewString (string) : NULL;
 string = XmtInputFieldGetString (XmtNameToWidget (new_caller->dialog, "fname"));
 data->FName = string ? XtNewString (string) : NULL;
 string = XmtInputFieldGetString (XmtNameToWidget (new_caller->dialog, "street"));
 data->Adr->Street = string ? XtNewString (string) : NULL;
 string = XmtInputFieldGetString (XmtNameToWidget (new_caller->dialog, "city"));
 data->Adr->City = string ? XtNewString (string) : NULL;
 string = XmtInputFieldGetString (XmtNameToWidget (new_caller->dialog, "country"));
 data->Adr->Country = string ? XtNewString (string) : NULL;

 if		((adress = write_address (data)) != NULL) {
   msgcpy	(new_caller->socket, adress, strlen (adress) + 1);
   free_Address	(data);
   free		(adress);
   Write	(new_caller->socket);
 }
 XtUnmanageChild	(new_caller->dialog);
 XtDestroyWidget	(new_caller->dialog);
}


static void cancel_call	(Widget w, newCallerStruct new_caller, XmAnyCallbackStruct *reason)
{
 XtUnmanageChild	(new_caller->dialog);
 XtDestroyWidget	(new_caller->dialog);
}


static void help_call	(Widget w, newCallerStruct new_caller, XmAnyCallbackStruct *reason)
{
}


void isdn_new_caller_display (socket_queue *socket)
{
 newCallerStruct	new_caller = XtNew (NewCallerStruct);
 new_caller->socket  =	socket;
 new_caller->dialog  =	XmtCreateLayoutDialog (toplevel, "NewCaller", NULL, 0);
 XmtCreateChildren	(new_caller->dialog);
 XtAddCallback		(XmtNameToWidget (new_caller->dialog, "ok"),
 			XmNactivateCallback, (XtCallbackProc) ok_call,
 			(XtPointer) new_caller);
 XtAddCallback		(XmtNameToWidget (new_caller->dialog, "cancel"),
 			XmNactivateCallback, (XtCallbackProc) cancel_call,
 			(XtPointer) new_caller);
 XtAddCallback		(XmtNameToWidget (new_caller->dialog, "help"),
 			XmNactivateCallback, (XtCallbackProc) help_call,
 			(XtPointer) new_caller);
 XtAddCallback		(new_caller->dialog, XmNdestroyCallback,
			(XtCallbackProc) destroy_call, (XtPointer) new_caller);
 XmtInputFieldSetString (XmtNameToWidget (new_caller->dialog, "phone"),
 			(String) socket->msgbuf.buf);
 XtManageChild          (new_caller->dialog);
}
