/* $Id: modem.c,v 1.9 1996/06/14 20:01:52 root Exp $
 *
 * $Log: modem.c,v $
 * Revision 1.9  1996/06/14  20:01:52  root
 * o Einige kleine Änderungen.
 *
 * Revision 1.8  1996/06/09  12:45:04  root
 * o Lowlevel-Funktion zum lesen von Daten vom Modem eingebaut. Die einge-
 *   lesenen Daten werden gleichzeitig auf "NO CARRIER" untersucht.
 *
 * o Funktionen eingebaut, die den NoCarrier-Status zurückliefern oder setz-
 *   en.
 *
 * Revision 1.7  1996/06/08  08:56:25  root
 * o Funktion ModemCheckInput() eingebaut, die zurückgibt ob am Modem Daten
 *   zum lesen anliegen.
 *
 * o Allgemeine Funktion zum lesen von Daten vom Modem eingebaut. Der ein-
 *   gelesene Buffer wird gleich auf "NO CARRIER" überprüft.
 *
 * Revision 1.6  1996/06/08  08:55:56  root
 * o Versionsschema geändert.
 *
 */

#include <termio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "vbox.h"
#include "modem.h"
#include "log.h"
#include "settings.h"
#include "lock.h"
#include "defines.h"

/** Variables ************************************************************/

static int	 ModemFD			= -1;
static int	 TimeoutFlag	= FALSE;
static int	 NoCarrier		= FALSE;
static int	 NoCarrierPos	= 0;
static char	*NoCarrierTxt	= "NO CARRIER";

/** Extern Variables *****************************************************/

int	FlowControl	= MODEM_FLOW_HARD;
bool	ToggleDTR	= TRUE;
long	TogglePause	= 50;
long	Speed			= 38400;

/** Supported modem speeds ***********************************************/

static struct ModemOneSpeed ModemSpeeds[] =
{
	{ 50		, B50		}, { 75		, B75		}, { 110		, B110	},
	{ 134		, B134	}, { 150		, B150	}, { 200		, B200	},
	{ 300		, B300	}, { 600		, B600	}, { 1200	, B1200	},
	{ 1800	, B1800	}, { 2400	, B2400	}, { 4800	, B4800	},
	{ 9600	, B9600	}, { 19200	, B19200	}, { 38400	, B38400	},
	{ 57600	, B57600	}, { 115200	, B115200}, { 230400	, B230400},
	{ 460800	, B460800}, { 0		, 0		}
};

/** Prototypes ***********************************************************/

static boolean ModemWrite(char *);
static boolean ModemRead(char *, int);
static speed_t	ModemGetSpeedFlag(long, speed_t);
static void		TimeoutFunction(int);
static void		CheckNoCarrier(char);

/*************************************************************************
 ** CheckNoCarrier():	Überwacht die eingehenden Daten auf NO CARRIER.	**
 *************************************************************************
 ** C							Zeichen vom Modem.										**
 *************************************************************************/

void CheckNoCarrier(char C)
{
	if (C == NoCarrierTxt[NoCarrierPos])
	{
		NoCarrierPos++;

		if (NoCarrierPos >= strlen(NoCarrierTxt))
		{
			LogLine(L_JUNK, "*** NO CARRIER ***\n");

			NoCarrier		= TRUE;
			NoCarrierPos	= 0;
		}
	}
	else
	{
		NoCarrierPos = 0;
		
		if (C == NoCarrierTxt[0]) NoCarrierPos++;
	}
}

/*************************************************************************
 ** ModemGetNoCarrier():																**
 *************************************************************************/

boolean ModemGetNoCarrier(void)
{
	return(NoCarrier);
}

/*************************************************************************
 ** ModemSetNoCarrier():																**
 *************************************************************************/

void ModemSetNoCarrier(bool Flag)
{
	NoCarrier = Flag;
}

/*************************************************************************
 ** ModemGetTermIO():																	**
 *************************************************************************/

boolean ModemGetTermIO(TIO *TermIO)
{
	if (LockDevice())
	{
		if (tcgetattr(ModemFD, TermIO) >= 0) True();
	}
    
	False();
}

/*************************************************************************
 ** ModemSetTermIO():																	**
 *************************************************************************
 ** TermIO				 Zeiger auf termios-Struktur.								**
 *************************************************************************/

boolean ModemSetTermIO(TIO *TermIO)
{
	if (LockDevice())
	{
		if (tcsetattr(ModemFD, TCSANOW, TermIO) >= 0) True();
	}
    
	False();
}

/*************************************************************************
 ** ModemPause():																			**
 *************************************************************************
 ** Pause			Anzahl der Millisekunden zum warten.						**
 *************************************************************************/

void ModemPause(unsigned long Pause)
{
	usleep(Pause * 1000);										 /* Millisekunden	*/
}

/*************************************************************************
 ** ModemToggleDTR():																	**
 *************************************************************************
 ** Pause				 Anzahl der Millisekunden die DTR gehalten werden	**
 **						 soll.															**
 *************************************************************************/

boolean ModemToggleDTR(unsigned long Pause)
{
	TIO TermIO;
	TIO SaveIO;

	LogLine(L_DEBUG, "Lowering DTR to reset modem (%d ms)...\n", Pause);

	if (!ModemGetTermIO(&TermIO)) False();

	SaveIO = TermIO;

	cfsetospeed(&TermIO, B0);
	cfsetispeed(&TermIO, B0);

	ModemSetTermIO(&TermIO);
	ModemPause(Pause);

	return(ModemSetTermIO(&SaveIO));
}

/*************************************************************************
 ** ModemSetSaneMode():																	**
 *************************************************************************/

void ModemSetSaneMode(TIO *TermIO, bool Local)
{
	TermIO->c_iflag  =  (BRKINT | IGNPAR | IXON | IXANY);
	TermIO->c_oflag  =  (OPOST | TAB3);
	TermIO->c_cflag &= ~(CSIZE | CSTOPB | PARENB | PARODD | CLOCAL);
	TermIO->c_cflag |=  (CS8 | CREAD | HUPCL | (Local ? CLOCAL : 0));
	TermIO->c_lflag  =  (ECHOK | ECHOE | ECHO | ISIG | ICANON);
}

/*************************************************************************
 ** ModemSetSpeed():																		**
 *************************************************************************/

void ModemSetSpeed(TIO *TermIO, long Speed)
{
	speed_t UseSpeed;

	UseSpeed = ModemGetSpeedFlag(Speed, B38400);

	LogLine(L_DEBUG, "Setting modem speed to %d (%03o).\n", Speed, UseSpeed);

	cfsetospeed(TermIO, UseSpeed);
	cfsetispeed(TermIO, UseSpeed);
}

/*************************************************************************
 ** ModemGetSpeedFlag():																**
 *************************************************************************/

static speed_t ModemGetSpeedFlag(long Speed, speed_t Default)
{
	int i = 0;

	while (ModemSpeeds[i].Flag != B0)
	{
		if (ModemSpeeds[i].Speed == Speed) return(ModemSpeeds[i].Flag);

		i++;
	}

	LogLine(L_WARN, "Needed speed %d not found -- use default (%03o).\n", Default);

	return(Default);
}

/*************************************************************************
 ** ModemSetRawMode():																	**
 *************************************************************************/
                    
void ModemSetRawMode(TIO *TermIO)
{
	TermIO->c_iflag &= (IXON | IXOFF | IXANY);
	TermIO->c_oflag  = 0;
	TermIO->c_lflag  = 0;

	TermIO->c_cc[VMIN]  = 1;
	TermIO->c_cc[VTIME] = 0;
}

/*************************************************************************
 ** ModemSetFlowControl():																**
 *************************************************************************/
                                      
void ModemSetFlowControl(TIO *TermIO, int Type)
{
	LogLine(L_DEBUG, "Setting modem flow control (");

	if (Type & MODEM_FLOW_HARD		) LogText(L_DEBUG, " HARD"		);
	if (Type & MODEM_FLOW_XON_IN	) LogText(L_DEBUG, " XON-IN"	);
	if (Type & MODEM_FLOW_XON_OUT	) LogText(L_DEBUG, " XON-OUT"	);

	LogText(L_DEBUG, " )...\n");

	TermIO->c_cflag &= ~(CRTSCTS);
	TermIO->c_iflag &= ~(IXON | IXOFF | IXANY);
        
	if (Type & MODEM_FLOW_HARD		) TermIO->c_cflag |= (CRTSCTS);
	if (Type & MODEM_FLOW_XON_IN	) TermIO->c_iflag |= (IXOFF);
	if (Type & MODEM_FLOW_XON_OUT	) TermIO->c_iflag |= (IXON);
}

/*************************************************************************
 ** ModemInitDevice():																	**
 *************************************************************************/

boolean ModemInitDevice(void)
{
	TIO TermIO;

	if (ToggleDTR)
	{
		if (!ModemToggleDTR(TogglePause))
		{
			LogLine(L_WARN, "Can't lowering DTR to reset modem.\n");
		}
	}

	if (!ModemGetTermIO(&TermIO))
	{
		LogLine(L_ERROR, "Can't get terminal I/O settings.\n");

		False();
	}

	ModemSetSaneMode(&TermIO, TRUE);
	ModemSetSpeed(&TermIO, Speed);
	ModemSetRawMode(&TermIO);
	ModemSetFlowControl(&TermIO, FlowControl);

	if (!ModemSetTermIO(&TermIO))
	{
		LogLine(L_ERROR, "Can't set new terminal I/O settings.\n");

		False();
	}

	True();
}

/*************************************************************************
 ** ModemFlush():																			**
 *************************************************************************/

void ModemFlush(int Timeout)
{
	TIO	TermIO;
	TIO	SaveIO;
	long	AllJunk		= 0;
	char	OneByte		= 0;

	LogLine(L_DEBUG, "Flushing modem%s...\n", (Timeout ? " (Timeout)" : ""));

	if (ModemGetTermIO(&TermIO))
	{
		SaveIO = TermIO;

		TermIO.c_lflag			&= ~ICANON;
		TermIO.c_cc[VMIN]		 = 0;
		TermIO.c_cc[VTIME]	 = Timeout;

		if (ModemSetTermIO(&TermIO))
		{
			while (ModemRawRead(&OneByte, 1) == 1)
			{
				if (AllJunk++ < 20)
				{
					LogLine(L_JUNK, "Junk: ");
					LogChar(L_JUNK, OneByte, TRUE);
				}
			}

			if (AllJunk > 20)
			{
				LogLine(L_DEBUG, "Flush has junked %d bytes.\n", AllJunk);
			}

			ModemSetTermIO(&SaveIO);
		}
	}
	else
	{
		LogLine(L_WARN, "Internal flush failed -- try tcflush()...\n");

		tcflush(ModemFD, TCIFLUSH);
	}
}

/*************************************************************************
 ** ModemFlushFull():																	**
 *************************************************************************/

void ModemFlushFull(void)
{
	tcflush(ModemFD, TCIOFLUSH);
	tcflush(ModemFD, TCIOFLUSH);
	
	ModemFlush(0);
}

/*************************************************************************
 ** ModemWrite():																			**
 *************************************************************************/

static boolean ModemWrite(char *Command)
{
	if (LockDevice())
	{
		if (ModemRawWrite(Command, strlen(Command)) == strlen(Command)) True();
	}

	False();
}

/*************************************************************************
 ** ModemRawWrite():																		**
 *************************************************************************/

size_t ModemRawWrite(char *Line, int Len)
{
	return(write(ModemFD, Line, Len));
}

/*************************************************************************
 ** ModemRead():	Liest eine komplette Kommandozeile vom Modem ein.		**
 *************************************************************************
 ** Line				Buffer in dem das Ergebnis gesichert werden soll; muß	**
 **					mindestens MODEM_BUFFER_LEN Byte's groß sein.			**
 ** Timeout			Timeout in Sekunden.												**
 *************************************************************************/

static boolean ModemRead(char *Line, int Timeout)
{
	char	OneByte	= 0;
	int	LineLen	= 0;
	int	Back;
	bool	Stop;

	LogLine(L_JUNK, "Reading line from modem (%d secs timeout)...\n", Timeout);

	if (!LockDevice()) False();

	ModemSetTimeout(Timeout);

	while (((OneByte != '\n') || (LineLen == 0)) && (LineLen < (MODEM_BUFFER_LEN - 1)))
	{
		Back = ModemRawRead(&OneByte, 1);
		Stop = ModemGetTimeout();

		if ((Stop) || (Back != 1))
		{
			ModemSetTimeout(0);

			LogLine(L_JUNK, "Can't read from modem [%d]%s.\n", Back, (Stop ? " (timeout)" : ""));

			False();
		}

		if ((OneByte != '\n') && (OneByte != '\r'))
		{
			*Line++ = OneByte;

			LineLen++;
		}
	}

	*Line = 0;

	ModemSetTimeout(0);
	True();
}

/*************************************************************************
 ** ModemRawRead():	Liest Daten vom Modem ein und überprüft diese nach	**
 **						"NO CARRIER".													**
 *************************************************************************
 ** Line					Buffer in den die Daten geschrieben werden sollen.	**
 ** Len					Anzahl der Daten die gelesen werden sollen.			**
 **																							**
 ** [Return]			Anzahl der Daten die gelesen wurden.					**
 *************************************************************************/

int ModemRawRead(char *Line, int Len)
{
	int r;
	int i;

	if ((r = read(ModemFD, Line, Len)) > 0)
	{
		for (i = 0; i < r; i++) CheckNoCarrier(Line[i]);
	}

	return(r);
}

/*************************************************************************
 ** ModemCheckResult():																	**
 *************************************************************************/

int ModemCheckResult(char *Have, char *Need)
{
	char	Line[MODEM_BUFFER_LEN + 1];
	char *Word;
	char *More;
	int	Nr;
	int	i;

	LogLine(L_DEBUG, "Waiting for \"");

	for (i = 0; i < strlen(Need); i++) LogChar(L_DEBUG, Need[i], FALSE);
	
	LogText(L_DEBUG, "\"... ");

		/* Text sichern und die einzelnen benötigten Antworten mit	*/
		/* dem Modembuffer vergleichen.										*/

	strncpy(Line, Need, MODEM_BUFFER_LEN);

	More	= strchr(Line, '|');
	Word	= strtok(Line, "|");
	Nr		= 0;

	while (Word)
	{
		Nr++;

		if (strncmp(Have, Word, strlen(Word)) == 0)
		{
			if (More)
			{
				LogText(L_DEBUG, "Got \"");

				for (i = 0; i < strlen(Word); i++) LogChar(L_DEBUG, Word[i], FALSE);
				
				LogText(L_DEBUG, "\" (%d).\n", Nr);
			}
			else LogText(L_DEBUG, "Got it.\n");
			
			return(Nr);
		}
	
		Word = strtok(NULL, "|");
	}

	LogText(L_DEBUG, "Oops!\n");

	return(0);
}

/*************************************************************************
 ** ModemCommand():																		**
 *************************************************************************/

boolean ModemCommand(char *Command, char *Result)
{
	char	Line[MODEM_BUFFER_LEN + 1];
	int	i;

	if (!LockDevice()) False();

	if ((Command) && (*Command))
	{
			/* Kommando an das Modem schicken. */

		ModemFlush(0);

		LogLine(L_DEBUG, "Sending \"");
		
		for (i = 0; i < strlen(Command); i++) LogChar(L_DEBUG, Command[i], FALSE);

		LogText(L_DEBUG, "\"...\n");		

		if (strcmp(Command, "\r") != 0)
		{
			if ((!ModemWrite(Command)) || (!ModemWrite(MODEM_COMMAND_SUFFIX)))
			{
				LogLine(L_ERROR, "Can't send modem command.\n");

				ModemFlush(1);
				False();
			}

			if (!ModemRead(Line, MODEM_ECHOCMD_TIMEOUT))
			{
				LogLine(L_ERROR, "Can't read modem command echo.\n");

				ModemFlush(1);
				False();
			}

			if (ModemCheckResult(Line, Command) == 0)
			{
				LogLine(L_ERROR, "Modem doesn't echo the command.\n");

				ModemFlush(1);
				False();
			}
		}
		else
		{
			if (!ModemWrite(Command))
			{
				LogLine(L_ERROR, "Can't send modem command.\n");

				ModemFlush(1);
				False();
			}
		}
	}

	if ((Result) && (*Result))
	{
		if (!ModemRead(Line, MODEM_COMMAND_TIMEOUT))
		{
			if ((Command) && (*Command))
			{
				LogLine(L_ERROR, "Can't read modem command result.\n");
			}

			ModemFlush(1);
			False();
		}

		if (ModemCheckResult(Line, Result) == 0)
		{
			LogLine(L_ERROR, "Modem returns unneeded command \"");

			for (i = 0; i < strlen(Line); i++) LogChar(L_ERROR, Line[i], FALSE);

			LogText(L_ERROR, "\".\n");

			ModemFlush(1);
			False();
		}
	}

	True();
}

/*************************************************************************
 ** ModemOpenDevice():																	**
 *************************************************************************/

boolean ModemOpenDevice(char *Name)
{
	if (ModemFD != -1) ModemCloseDevice();

	LogLine(L_DEBUG, "Open device \"%s\".\n", Name);

	if ((ModemFD = open(Name, O_RDWR | O_NDELAY)) != -1)
	{
		if (fcntl(ModemFD, F_SETFL, O_RDWR) != -1)
		{
			if (ModemInitDevice()) True();
		}
		else LogLine(L_ERROR, "Can't call fcntl() to set device flags.\n");

		ModemCloseDevice();
	}

	False();
}

/*************************************************************************
 ** ModemCloseDevice():																	**
 *************************************************************************/

void ModemCloseDevice(void)
{
	if (ModemFD != -1)
	{
		LogLine(L_DEBUG, "Flush modem for shutdown...\n");

		ModemFlushFull();
		ModemToggleDTR(TogglePause);

		LogLine(L_DEBUG, "Close device \"%s\" (%d)...\n", VBoxDevicename, ModemFD);

		close(ModemFD);

		ModemFD = -1;
	}
}

/*************************************************************************
 ** ModemWait():																			**
 *************************************************************************/

boolean ModemWait(long Secs)
{
	struct timeval  Timeout;
	struct timeval *UseTimeout;

	fd_set	FD;	
	int		Back;

	UnlockDevice();

	LogLine(L_INFO, "Waiting...\n");

	FD_ZERO(&FD);
	FD_SET(ModemFD, &FD);
        	
	if (Secs > 0)
	{
		Timeout.tv_sec		= Secs;
		Timeout.tv_usec	= Secs * 1000;

		UseTimeout = &Timeout;
	}
	else UseTimeout = NULL;

	Back = select(FD_SETSIZE, &FD, NULL, NULL, UseTimeout);
	
	if (Back <= 0)
	{
		if (Back < 0)
		{
			LogLine(L_ERROR, "Select returns with error (%d).\n", Back);
		}
		else LogLine(L_DEBUG, "Select returns with timeout.\n");

		False();
	}

	True();
}

/*************************************************************************
 ** TimeoutFunction():																	**
 *************************************************************************/

static void TimeoutFunction(int s)
{
	alarm(0);
	signal(SIGALRM, SIG_IGN);

	LogLine(L_JUNK, "Timeout function called.\n");

	TimeoutFlag = TRUE;
}

/*************************************************************************
 ** ModemSetTimeout():	Setzt den Timeout für das Modem. Ob ein Timeout	**
 **							eingetreten ist, kann mit ModemGetTimeout() ab-	**
 **							gefragt werden.											**
 *************************************************************************
 ** Timeout					Timeout in Sekunden.										**
 *************************************************************************/

void ModemSetTimeout(int Timeout)
{
	if (Timeout != 0)
	{
		TimeoutFlag = FALSE;

		signal(SIGALRM, TimeoutFunction);
		alarm(Timeout);
	}
	else
	{
		signal(SIGALRM, SIG_IGN);
		alarm(0);
	}
}

/*************************************************************************
 ** ModemGetTimeout():	Gibt zurück, ob das letzte Modemkommando durch	**
 **							den Timeout abgebrochen wurde.						**
 *************************************************************************/

boolean ModemGetTimeout(void)
{
	return(TimeoutFlag);
}

/*************************************************************************
 ** ModemGetRings():																		**
 *************************************************************************/

boolean ModemGetRings(int Need)
{
	char	Line[MODEM_BUFFER_LEN + 1];
	char	File[PATH_MAX + 1];
	int	Have;
	bool	Answer;

	CallerID[0] = 0;
	Have			= 0;
	Answer		= TRUE;

	strncpy(File, UserHome, PATH_MAX);
	strncat(File, "/.vboxstop", PATH_MAX - strlen(File));

	while (ModemRead(Line, MODEM_RING_TIMEOUT))
	{
			/* Wenn im Homeverzeichnis des Benutzers die Datei `.vboxstop'	*/
			/* existiert, wird auf den Anruf nicht geantwortet.				*/

		if (access(File, F_OK) == 0)
		{
			Answer = FALSE;
		}
		else Answer = TRUE;

			/* CallerID herausfuseln und in die globale Variable ein-	*/
			/* tragen.																	*/

		if (strncmp(Line, "CALLER NUMBER: ", 15) == 0)
		{
			if (!*CallerID)
			{
				strncpy(CallerID, &Line[15], MAXCALLERIDLEN);

				LogLine(L_INFO, "Found CallerID \"%s\".\n", CallerID);
			}

			continue;
		}

			/* RING's zählen! Wenn die Anzahl der benötigten RING's	*/
			/* gefunden wurde und die Datei `.vboxstop' nicht exist-	*/
			/* iert, wird der Anruf beantwortet.							*/

		if (strcmp(Line, "RING") == 0)
		{
			Have++;

			LogLine(L_DEBUG, "Got \"RING\" [%2d/%2d] (%s).\n", Have, Need, (Answer ? "answer" : "don't answer"));
		}
		else LogLine(L_JUNK, "Got junk line \"%s\"...\n", Line);

		if ((Have >= Need) && (Answer)) True();
	}

	False();
}

/*************************************************************************
 *************************************************************************/

boolean ModemCheckInput(void)
{
	struct timeval Timeout;

	fd_set	FD;
	int		Result;
        
	FD_ZERO(&FD);
	FD_SET(ModemFD, &FD);

	Timeout.tv_sec		= 0;
	Timeout.tv_usec	= 0;

	Result = select(FD_SETSIZE , &FD, NULL, NULL, &Timeout);
                
	if (Result > 0) True();

	False();
}
