#define _CLIENT_C_
#include <isdn_client.h>



static char options[] = "p:";
static char    usage[]   = "%s: usage: %s [\"X11 options\"] [ -%s ] [host]\n";

/****************************************************************************
 *
 * In der Fkt. eval_message findet das EventHandling der einkommenden
 * Messages statt. Von Hier aus muessen die einzelnen Fenster bedient
 * werden.
 */

static	int eval_message (socket_queue *sock)
{
	int    Msg = sock->msg, kanal;
	CALL* Info = NULL;

	switch(sock->waitstatus)
	{
		case WF_ACC:
			switch (Msg)
			{
				case MSG_ANN_ACC:
					sock->servtyp = stoi(sock->msgbuf.buf,_MSG_2B);
					sock->waitstatus = WF_NOTHING;
					break;
				
				case MSG_VERSION:
					/* Meldung: Protokoll-Versionsnummer vom Server */
					compare_version (sock->msgbuf.buf);
					break;

				case MSG_ANN_REJ:
					/* Meldung: nicht authorisierter Zugriff auf Server */
          sock->msg = MSG_CLOSE;
          sock->msgbuf.used = 0;
          Write(sock);

					isdn_error (1);
					/* Die naechste Zeilen sollte bald realisert werden 
					close(sock->descriptor);
					return -1; */
					break;

				default:
					/* Meldung: Unbekannter Message-Typ Msg */
					{
						char message[256];
						sprintf (message, isdn_error_get (2), Msg);
						fprintf (stderr, message);
					}
					break;
			}
			break;

		default:
			switch (Msg)
			{
				case MSG_CALLER:
					/* Eingabe eines Neuen Benutzers sock->msgbuf */
					/*if (sock->servtyp & T_ADDRESSBOOK)*/
					break;
				
				case MSG_WHO_IS:
					/* Eingabe eines Neuen Benutzers sock->msgbuf */
					if (sock->servtyp & T_ADDRESSBOOK)
						isdn_new_caller_display (sock);
					break;
				
				case MSG_SERVER:
					/* Eingabe eines Neuen Benutzers sock->msgbuf */
					isdn_prot (sock);
					break;

                               case MSG_CHANGE_CHAN:
                                       /* Wechsel der aktuellen Kanalnummer des aktuellen Gespraeches */
                                       isdn_current_list_channel_change (sock);
                                       break;
				
				case MSG_CALL_INFO:
					switch (kanal = Set_Info_Struct(&Info,sock->msgbuf.buf)){
					  case	NO_MEMORY:
					  	return NO_MEMORY;
					  default:
					  	isdn_list_update (kanal, Info); break;
					}
					break;

				default:
					/* Meldung: Unbekannter Message-Typ Msg */
					{
						char message[256];
						sprintf (message, isdn_error_get (7), Msg);
						fprintf (stderr, message);
					}
					break;
			}
			break;
	}

	return 0;
}

/****************************************************************************/


static int start_connection (char *server, int port)
{
	int sock;
        socket_queue *socket = NULL;
	struct passwd *pw;

	if ((sock = client_connect(server,port)) < 0)
		return sock;

	if ((pw = getpwuid(getuid())) == NULL)
		return -1;

	if (add_socket (&socket, sock))
		return NO_MEMORY;

	socket->msg = MSG_ANNOUNCE;
	msgcpy (socket, pw->pw_name, strlen (pw->pw_name) + 1);

	Write (socket);

	socket->waitstatus = WF_ACC;
	socket->eval = eval_message;
	isdn_add_connection  (socket);
	return 0;
}


int main (int argc, char *argv[])
{
 int		fehler;
 int    c;
 int    port = 0;
 char   hostname[SHORT_STRING_SIZE] = "localhost";
 char  *myshortname = basename(argv[0]);

 isdn_initialize (&argc, argv);

 while ((c = getopt(argc, argv, options)) != EOF)
   switch (c)
   {
     case 'p' : port = strtol(optarg, NIL, 0);
                break;
     case '?' : printf(usage, myshortname, myshortname, options);
                return 1;
   }

 if (optind < argc)
   strcpy(hostname,argv[optind]);

 if (optind + 1 < argc)
 {
   printf(usage, myshortname, myshortname, options);
   return 1;
 }

 switch          (fehler = start_connection (hostname,port))
 {
   case 0:
     break;

   default:     /* Fehler */
     fprintf    (stderr, isdn_error_get (3));
     return     (fehler);
 }
 isdn_main_loop	();
 return 0;
}
