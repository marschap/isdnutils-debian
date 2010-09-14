/* $Id: start_prog.c,v 2.3.27 1996/05/05 20:35:46 root Exp $
 *
 * ISDN accounting for isdn4linux.
 *
 * Copyright 1996 by Michael 'Ghandi' Herold,
 *                   Stefan Luethje (luethje@konstanz.netsurf.de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Log: start_prog.c,v $
 * Revision 2.3.27  1996/05/05  20:35:46  akool
 *
 * Revision 2.21  1996/03/13  11:58:46  akool
 */


#define _START_PROG_C_
#include "isdnlog.h"

/*************************************************************************/

static interval *RootIntervall = NULL;

/** Prototypes ***********************************************************/

static void		KillCommand(int);
static int		GetArgs(char *, char *[], char *[], int);
static interval *Next_Interval(void);

/*************************************************************************
 ** Ring(-) - Externes Kommando ausfuehren oder loeschen. Bei einem Fehl-	    **
 **			  er wird -1 zurueckgegeben; ansonsten die PID des neuen    **
 **			  Prozesses. Wenn das Kommando mit system() aufgerufen wur- **
 **			  de (Async), wird -1 zurueckgegeben, da die PID nicht ge-  **
 **			  sichert werden muss.					    **
 *************************************************************************
 ** Die  : <= 0   Kommando ausfuehren.						    **
 **        >  0   PID killen.							    **
 ** Async: == 1   Kommando mit system() starten.				    **
 **        == 0   Kommando mit eigenem Prozess starten.				    **
 *************************************************************************/

int Ring(char *Cmd, char *Opts[], int Die, int Async)
{
	char  Command[SHORT_STRING_SIZE + 1];
	char  String[LONG_STRING_SIZE];
	int   filedes[2];
	char *Args[64];
	pid_t pid;
	FILE *fp;

	print_msg(PRT_DEBUG_EXEC, "Ring: Cmd: '%s' Die: %d Async: %d\n", Cmd, Die, Async);

	if (Die <= 0)
	{
		sprintf(Command,"%.*s",SHORT_STRING_SIZE-6,Cmd);


			/* Kommando soll gestartet werden - 'Async' gibt an, ob dieses	*/
			/* mit system() oder mit einem eigenen Proze� aufgerufen wer-	*/
			/* den soll.																	*/

		if (GetArgs(Command,Args,Opts,64) == -1)
		{
			print_msg(PRT_ERR, "Can't start \"%s\" with execvp().\n", Args[0]);
			return -1;
		}

		pipe(filedes);
		switch (pid = fork())
		{
			case -1: print_msg(PRT_ERR, "%s\n", "Can't start fork()!");
			         return 0;
			         break;
			case  0: dup2(filedes[1],STDOUT_FILENO);
			         dup2(filedes[1],STDERR_FILENO);
			         execvp(Args[0], Args);
			         print_msg(PRT_ERR, "Can't start \"%s\" with execvp().\n", Args[0]);
			         /* Alarm(); */
			         exit(-1);
			         break;
		}

		fp = fdopen(filedes[0],"r");
		close(filedes[1]);

		print_msg(PRT_DEBUG_INFO, "Program \"%s\" \"%s\" started %ssynchronous.\n", Args[0], Args[1], (Async?"a":""));

		if (Async == 1)
		{
			while(fgets(String,LONG_STRING_SIZE,fp) != NULL)
				print_msg(PRT_PROG_OUT,"%s\n",String);

			waitpid(pid,NULL,0);
			fclose(fp);
			return(0);
		}
		else
		{
			int sock;

			if (add_socket(&sockets,filedes[0]))
				return NO_MEMORY;

			sock = socket_size(sockets)-1;
			sockets[sock].fp  = fp;
			sockets[sock].pid = pid;

			return sock;
		}
	}
	else
	{
		KillCommand(Die);
		return 0;
	}

	return(-1);
}

/*************************************************************************
 ** GetArgs(-) - Zerlegt eine Kommandozeile in einzelne Argumente.		**
 *************************************************************************/

static int GetArgs(char *Line, char *Args[], char *Opts[], int MaxArgs)
{
	char	*Arg	= NULL;
	char	*Use	= Line;
	char  *Ptr  = NULL;
	char  *Ptr2 = NULL;
	char  *Org_Arg;
	int	 MaxOpts= 0;
	int	 i		= 0;
	int	 j		= 0;
	int	 Num  = 0;
	char HelpString[SHORT_STRING_SIZE];
	static char **MemPtr = NULL;

	if (MemPtr != NULL)
	{
		while(MemPtr[j] != NULL)
			free(MemPtr[j++]);

		free(MemPtr);
		MemPtr = NULL;
		j = 0;
	}

	while (Opts[MaxOpts] != NULL)
		MaxOpts++;

	while ((Org_Arg = Arg = strtok(Use, " \t")))
	{
		Use = NULL;

		if (*Arg == '@')
		{
			FILE *fp = fopen(Arg+1,"r");

			if (fp != NULL)
			{
				fgets(HelpString,SHORT_STRING_SIZE,fp);

				if (*HelpString != '\0')
					HelpString[strlen(HelpString)-1] = '\0';

				Arg = strdup(HelpString);

				MemPtr = (char**) realloc(MemPtr,sizeof(char*)*(j+2));
				MemPtr[j++] = Arg;
				MemPtr[j] = NULL;
				fclose(fp);
			}
			else
				Arg = NULL;
		}
		else
		if (*Arg == '$')
		{
			if (Opts != NULL && (Num = atoi(Arg+1)) > 0 && Num <= MaxOpts)
				Arg = Opts[Num-1];
			else
			if (strlen(Arg) > 3 && Arg[1] == '{' && (Ptr = strrchr(Arg,'}')) != NULL)
			{
				*Ptr = '\0';

				if ((Ptr2 = getenv(Arg+2)) != NULL)
				{
					strcpy(HelpString,Ptr2);
					strcat(HelpString,Ptr+1);
					Arg = strdup(HelpString);

					MemPtr = (char**) realloc(MemPtr,sizeof(char*)*(j+2));
					MemPtr[j++] = Arg;
					MemPtr[j] = NULL;
				}
				else
					Arg = NULL;
			}
			else
				Arg = getenv(Arg+1);
		}

		if (Arg == NULL || *Arg == '\0')
		{
			if (Arg == NULL)
				print_msg(PRT_WARN,"Invalid argument `%s' for program start!\n",Org_Arg);

			Arg = "?";
		}

               Ptr = Arg;
         while((Ptr = Check_Quote(Ptr, S_ALL_QUOTE, QUOTE_DELETE)) != NULL);

		if (i < MaxArgs) Args[i++] = Arg;
	}

	Args[i] = NULL;

	return(i);
}

/*************************************************************************
 ** KillCommand(-) - Beendet ein Programm anhand seiner PID.				**
 *************************************************************************/

static void KillCommand(int sock)
{
	char String[LONG_STRING_SIZE] = "";
		/* Kein Erbarmen - Alles was uns zwischen die Finger kommt wird	*/
		/* gel�scht :-)																	*/

	if (sock > 0)
	{
		while (fgets(String,LONG_STRING_SIZE,sockets[sock].fp))
			print_msg(PRT_PROG_OUT,"%s\n",String);

		kill(sockets[sock].pid, SIGTERM);
		kill(sockets[sock].pid, SIGKILL);

    /* ACHTUNG: Die naechste Zeile kann schwierigkeiten machen!!!
       Der Prozess kann eventuell hier haengen bleiben. Dann Zeile
       auskommentieren. Ich weiss nicht, ob kill den Prozess auch
       sauber beendet. Damit keine Zombies rumfahren vorsichtshaber
       der waitpid.
       Alternativ: waitpid(sockets[sock].pid,NULL,WNOHANG) */

		waitpid(sockets[sock].pid,NULL,0);
		fclose(sockets[sock].fp);
		del_socket(&sockets,sock);
	}
}

/*************************************************************************
 ** Alarm(-) - Gibt ein Alarmsignal �ber den internen Lautsprecher aus.	**
 *************************************************************************/

void Alarm(void)
{
#ifdef ALARM

	int FD;
	int i;

	if ((FD = open("/dev/console", O_WRONLY)) == -1) FD = 0;

	for (i = 0; i < 30; i++)
	{
		ioctl(FD, KIOCSOUND, (3000 - (i * 10)));

		usleep((1 * 1000));
	}

	ioctl(FD, KIOCSOUND, 0);

#endif
}

/*************************************************************************
 ** CheckTime(-) - Pr�ft ob die Zeitangabe in den lokalen Zeitrahmen		**
 **					 f�llt. R�ckgabe ist TRUE/FALSE.								**
 *************************************************************************/

int CheckTime(char *Line)
{
	char	 Temp[SHORT_STRING_SIZE + 1];
	char	 Time[24];
	char	*Use;
	struct tm	*tm_Time;
	char	*Arg;
	char	*Minus;
	int	 i;
	int	 r;
	int	 Beg;
	int	 End;
	time_t Local;

	if (Line == NULL || *Line == '\0')
		return 1;

	strncpy(Temp, Line, SHORT_STRING_SIZE);

	for (i = 0; i < 24; i++) Time[i] = 0;

	Use = Temp;

		/* Zeile in die einzelnen Komponenten trennen, die durch ',',	*/
		/* ' ' oder ';' voneinander getrennt sein k�nnen.					*/

	while ((Arg = strtok(Use, ", ;")))
	{
		Use = NULL;

		if (*Arg == '*')
			return 1;

		if (!isdigit(*Arg))
		{
			print_msg(PRT_WARN, " Wrong time in `%s`: \"%s\"\n", CONFFILE, Arg);
			continue;
		}

		if ((Minus = strchr(Arg, '-')))
		{
			*Minus++ = 0;

			Beg = atoi(Arg);
			End = atoi(Minus);
		}
		else
		{
			Beg = atoi(Arg);
			End = Beg+1;
		}

    if ((Beg < 0) || (End < 0) || (Beg > 24) || (End > 25))
		{
			print_msg(PRT_WARN, "Time range not correct in `%s`: (%d-%d)\n", CONFFILE, Beg, End);
		}
		else
		{
			if (End <= Beg)
				End += 24;

			for (r = Beg; r < End; r++) Time[r%24] = 1;
		}
	}

		/* Lokale Zeit errechnen und mit den Stunden im Zeitarray ver-	*/
		/* gleichen.																	*/

	Local = time(NULL);

	if ((tm_Time = localtime(&Local)) != NULL)
	{
			if (Time[tm_Time->tm_hour] == 1)
				return(1);
	}
	else print_msg(PRT_ERR, "Can't get local time.\n");

	return(0);
}

/****************************************************************************/

int Print_Cmd_Output( int sock )
{
	char String[LONG_STRING_SIZE] = "";


	if (feof(sockets[sock].fp))
	{
		KillCommand(sock);
		return -1;
	}

	fgets(String,LONG_STRING_SIZE,sockets[sock].fp);

	print_msg(PRT_PROG_OUT,"%s\n",String);

	return 0;
}

/****************************************************************************/

int Get_Sock_From_Info_Args( info_args *Ptr, int Cnt )
{
	if (socket_size(sockets) > Cnt || Cnt < 0)
		while (sockets[Cnt].descriptor != -2)
			if (sockets[Cnt].info_arg == Ptr)
				return Cnt;
			else
				Cnt++;

	return -1;
}

/****************************************************************************/

int Get_Sock_From_Call( int chan, int Cnt )
{
	if (socket_size(sockets) > Cnt || Cnt < 0)
		while (sockets[Cnt].descriptor != -2)
			if (sockets[Cnt].chan == chan)
				return Cnt;
			else
				Cnt++;

	return -1;
}

/****************************************************************************/

int Get_Sock_From_Call_And_Info_Args( int chan, info_args *Ptr, int Cnt )
{
	if (socket_size(sockets) > Cnt || Cnt < 0)
		while (sockets[Cnt].descriptor != -2)
			if (sockets[Cnt].chan == chan && sockets[Cnt].info_arg == Ptr)
				return Cnt;
			else
				Cnt++;

	return -1;
}

/****************************************************************************/

int Condition_Changed( int condition, int flag )
{
	if ((flag & RING_CONNECT) || (flag & RING_RING))
	{
		if ((flag & RING_CONNECT) && condition == RING_CONNECT)
			return 0;

		if ((flag & RING_CONNECT) && condition == RING_AOCD)
			return 0;

		if ((flag & RING_CONNECT) && condition == RING_ERROR)
			return 0;

		if ((flag & RING_RING) && condition == RING_RING)
			return 0;

		if ((flag & RING_RING) && condition == RING_ERROR)
			return 0;

		return 1;
	}
	return 0;
}

/****************************************************************************/

const char *Set_Ringer_Flags( int condtion, int InOut )
{
	char Char = 0;
	static char RetCode[10];

	if (InOut & RING_INCOMING) Char = 'I';
	else
	if (InOut & RING_OUTGOING) Char = 'O';
	else
	{
		print_msg(PRT_ERR, "Error: Expected flag `I' or `O'!\n");
		return NULL;
	}

	RetCode[0] = Char;

	if (condtion & RING_RING    ) Char = 'R';
	else
	if (condtion & RING_CONNECT ) Char = 'C';
	else
	if (condtion & RING_BUSY    ) Char = 'B';
	else
	if (condtion & RING_AOCD    ) Char = 'A';
	else
	if (condtion & RING_ERROR   ) Char = 'E';
	else
	if (condtion & RING_HANGUP  ) Char = 'H';
	else
	if (condtion & RING_KILL    ) Char = 'K';
	else
	if (condtion & RING_SPEAK   ) Char = 'S';
	else
	if (condtion & RING_PROVIDER) Char = 'P';
	else
	{
		print_msg(PRT_ERR, "Internal error: Unknown flag %d for flag -S!\n",condtion);
		return NULL;
	}

	RetCode[1] = Char;
	RetCode[2] = '\0';

	return RetCode;
}

/****************************************************************************/

int Start_Interval(void)
{
	interval *Ptr   = RootIntervall;
	time_t cur_time = time(NULL);
	int RetCode     = 0;

	while (Ptr != NULL)
	{
		if (Ptr->next_start <= cur_time)
		{
			RetCode += Start_Ring(Ptr->chan, Ptr->infoarg, Ptr->event, RING_INTERVAL);
			Ptr->next_start = cur_time + Ptr->infoarg->interval;
		}

		Ptr = Ptr->next;
	}

	return RetCode;
}

/****************************************************************************/

static interval *Next_Interval(void)
{
	interval *Ptr     = RootIntervall;
	interval *RetCode = NULL;
	time_t next_time  = 0;

	while (Ptr != NULL)
	{
		if (next_time == 0 || Ptr->next_start < next_time)
		{
			next_time = Ptr->next_start;
			RetCode = Ptr;
		}

		Ptr = Ptr->next;
	}

	return RetCode;
}

/****************************************************************************/

struct timeval *Get_Interval(void)
{
	static struct timeval timeout;
	interval *Ptr   = NULL;;


	timeout.tv_usec = 0;

	if ((Ptr = Next_Interval()) == NULL)
		return NULL;

	if ((timeout.tv_sec = (int) (Ptr->next_start - time(NULL))) < 0)
		timeout.tv_sec = 0;

	return &timeout;
}

/****************************************************************************/

int Del_Interval(int chan, info_args *infoarg)
{
	interval **Ptr = &RootIntervall;
	interval *Ptr2;

	while (*Ptr != NULL)
	{
		if ((*Ptr)->infoarg == infoarg && (*Ptr)->chan == chan)
		{
			Ptr2 = (*Ptr)->next;
			free(*Ptr);
			*Ptr = Ptr2;
			return 0;
		}

		Ptr = &((*Ptr)->next);
	}

	return -1;
}

/****************************************************************************/

int New_Interval(int chan, info_args *infoarg, int event)
{
	interval **Ptr = &RootIntervall;

	if (infoarg->interval == 0)
		return -1;

	while (*Ptr != NULL)
		Ptr = &((*Ptr)->next);

	if ((*Ptr = (interval*) calloc(1,sizeof(interval))) == NULL)
		return -1;

	(*Ptr)->event      = event;
	(*Ptr)->infoarg    = infoarg;
	(*Ptr)->chan       = chan;
	(*Ptr)->next_start = infoarg->interval + time(NULL);

	return 0;
}

/****************************************************************************/

int Start_Process(int chan, info_args *infoarg, int event)
{
	char *Opts[4];
	int   InOut = call[chan].dialin?RING_INCOMING:RING_OUTGOING;
	int   sock = -1;

	Opts[0] = (char*) Set_Ringer_Flags(event,InOut);
	Opts[1] = call[chan].num[CALLING];
	Opts[2] = call[chan].num[CALLED];
	Opts[3] = NULL;

	if ((infoarg->flag & event) && (infoarg->flag & InOut)                       &&
			CheckTime(infoarg->time)                          &&     /* wenn die angegebene Zeit passt */
			(sock = Ring(infoarg->infoarg, Opts, 0, 0)) != -1   )
	{
		sockets[sock].info_arg   = infoarg;
		sockets[sock].chan       = chan;
		sockets[sock].call_event = event;
	}

	return sock<0?-1:0;
}

/****************************************************************************/

int Start_Ring(int chan, info_args *infoarg, int event, int intervalflag)
{
	int   ProcessStarted = 0;
	char *Opts[4];
	int   InOut = call[chan].dialin?RING_INCOMING:RING_OUTGOING;
	int f    = infoarg->flag; /* die Flags zu diesem Eintrag */
	int sock = 0;

	Opts[0] = (char*) Set_Ringer_Flags(event,InOut);
	Opts[1] = call[chan].num[CALLING];
	Opts[2] = call[chan].num[CALLED];
	Opts[3] = NULL;

	if (intervalflag & RING_INTERVAL)
	{
		if (f & RING_KILL)
			while ((sock = Get_Sock_From_Call_And_Info_Args(chan,infoarg,sock)) != -1)
				if (sockets[sock].call_event == event)
					Ring(NULL, NULL, sock++, 0);
				else
					sock++;
	}
	else
	{
		if (infoarg->interval != 0 && (event == RING_RING || event == RING_CONNECT))
			New_Interval(chan, infoarg, event);
	}

	if (Condition_Changed(event,f))
	{
		while ((sock = Get_Sock_From_Call_And_Info_Args(chan,infoarg,sock)) != -1)
			Ring(NULL, NULL, sock++, 0);

		Del_Interval(chan,infoarg);
	}

	/* Wenn Event (siehe oben) passt, und INCOMING/OUTGOING passen */

	if (!((f & RING_UNIQUE)                             &&
	    ((event == RING_RING) || (event == RING_CONNECT)  )) ||
	    Get_Sock_From_Call_And_Info_Args(chan,infoarg,0) == -1               )
	{
		if ((f & event) && (f & InOut)                       &&
				CheckTime(infoarg->time)                         &&     /* wenn die angegebene Zeit passt */
				(sock = Ring(infoarg->infoarg, Opts, 0, 0)) != -1  )
		{
			sockets[sock].info_arg   = infoarg;
			sockets[sock].chan       = chan;
			sockets[sock].call_event = event;
			ProcessStarted++;
		}
	}

	return ProcessStarted;
}

/****************************************************************************/

int Change_Channel_Ring( int old_channel, int new_channel)
{
	interval *Ptr = RootIntervall;
	int sock = 0;

	while ((sock = Get_Sock_From_Call( old_channel, sock )) >= 0)
	{
		sockets[sock].chan = new_channel;
	}

	while (Ptr != NULL)
	{
		if (Ptr->chan == old_channel)
			Ptr->chan = new_channel;

		Ptr = Ptr->next;
	}

	return 0;
}

/****************************************************************************/

