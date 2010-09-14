/* $Id: vbox.c,v 1.3 1996/06/09 13:10:57 root Exp $
 *
 * $Log: vbox.c,v $
 * Revision 1.3  1996/06/09  13:10:57  root
 * o Neues Versionsschema.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <errno.h>
#include <getopt.h>
              
#include "vbox.h"
#include "modem.h"
#include "log.h"
#include "settings.h"
#include "lock.h"
#include "voice.h"
#include "vboxrc.h"
#include "version.h"

/** Variables ************************************************************/

static int	 Loglevel;
static int	 RingsNeeded;
static char	*Basename;

char	VBoxPID[PATH_MAX + 1];
char	VBoxLockname[PATH_MAX + 1];
char	VBoxLogname[PATH_MAX + 1];
char	VBoxDevicetext[NAME_MAX + 1];
char	VBoxDevicename[PATH_MAX + 1];
char	InitCommand[MAXINITCOMMANDLEN + 1];
char	CallerID[MAXCALLERIDLEN + 1];
char	Username[MAXUSERNAMELEN + 1];
char	UserHome[PATH_MAX + 1];
uid_t	UID;
gid_t	GID;

static struct option Arguments[] =
{
	{ "device"  , required_argument, NULL, 'd' },
	{ "speed"	, required_argument, NULL, 's' },
	{ "username", required_argument, NULL, 'u' },
	{ "init"		, required_argument, NULL, 'i' },
	{ "help"		, no_argument      , NULL, 'h' },
	{ NULL		, 0                , NULL,  0  }
};

/** Prototypes ***********************************************************/

static boolean	InitVBox(int);
static void		ExitVBox(int);
static void		BlockSignals(void);
static void		Main(void);
static boolean	MkDir(char *, char *);
static void		Usage(void);
static boolean	FreeSpace(unsigned long);

/*************************************************************************
 ** The magic main...																	**
 *************************************************************************/

void main(int argc, char **argv)
{
	int Opts;

	Basename = argv[0];

	Loglevel 	= L_FATAL | L_ERROR | L_WARN;
	FlowControl	= MODEM_FLOW_HARD;
	Speed			= 38400;
	ToggleDTR	= TOGGLEDTR;
	TogglePause	= TOGGLEDTRTIME;
	RingsNeeded	= 10;
	UID			= -1;
	GID			= -1;

	*VBoxPID				= 0;
	*VBoxLockname		= 0;
	*VBoxLogname		= 0;
	*VBoxDevicetext	= 0;
	*VBoxDevicename	= 0;
	*CallerID			= 0;
	*Username			= 0;
	*InitCommand		= 0;

		/* Argumente eintragen. */

	while ((Opts = getopt_long(argc, argv, "d:s:u:i:h", Arguments, (int *)0)) != EOF)
	{
		switch (Opts)
		{
			case 's':
				Speed = (long)StrToNumber(optarg, 38400);
				break;
		
			case 'd':
				strncpy(VBoxDevicename, optarg, PATH_MAX);
				break;
			
			case 'u':
				strncpy(Username, optarg, MAXUSERNAMELEN);
				break;
			
			case 'i':
				strncpy(InitCommand, optarg, MAXINITCOMMANDLEN);
				break;

			case 'h':
			default:
				Usage();
				break;
		}
	}

	if (!*InitCommand)
	{
		strncpy(InitCommand, "ATZ", MAXINITCOMMANDLEN);
	}

		/* `vbox' initialisieren. */

	if (!InitVBox(Loglevel)) ExitVBox(NORMALEXIT + 20);

		/* Signalhändler installieren. */

	signal(SIGHUP	, ExitVBox);
	signal(SIGINT	, ExitVBox);
	signal(SIGTERM	, ExitVBox);
	signal(SIGHUP	, ExitVBox);

		/* Hauptschleife */

	while (TRUE)
	{
		if (ModemOpenDevice(VBoxDevicename))
		{
			Main();

			ModemCloseDevice();
		}
		else sleep(10);
	}

	ExitVBox(NORMALEXIT + 0);
}

/*************************************************************************
 ** Usage():																				**
 *************************************************************************/

static void Usage(void)
{
	fprintf(stderr, "\nVersion %s - (C) 1996 Michael Herold\n\n", VERSION);
	fprintf(stderr, "Usage: %s option(s)\n\n", Basename);
	fprintf(stderr, "-s, --speed SPEED    Sets modem speed to SPEED (default 38400)\n");
	fprintf(stderr, "-d, --device NAME    Device name to use (eg. /dev/ttyI5) [required]\n");
	fprintf(stderr, "-u, --username NAME  Name of the user to spool voice messages for [required]\n");
	fprintf(stderr, "-i, --init INIT      Sets modem initialize command to INIT (default \"ATZ\")\n");
	fprintf(stderr, "-h, --help           Displays this help message\n\n");

	UnlockPID();
	CloseLog();

	exit(NORMALEXIT + 5);
}

/*************************************************************************
 ** InitVBox():																			**
 *************************************************************************/

static boolean InitVBox(int Level)
{
	struct passwd *User;

	char *Temp;

	SetLogLevel(Level);

		/* Name des aktuellen Devices ermitteln (ohne den Pfad) und alle	*/
		/* Log- und Lockdateinamen erzeugen.										*/

	if (!*VBoxDevicename)
	{
		fprintf(stderr, "%s: can't spool voice mails without a device.\n", Basename);

		Usage();
	}

	if ((Temp = rindex(VBoxDevicename, '/')))
	{
		Temp++;

		strncpy(VBoxDevicetext, Temp, NAME_MAX);
	}
	else strncpy(VBoxDevicetext, VBoxDevicename, NAME_MAX);

	if (strncmp(VBoxDevicetext, "tty", 3) != 0)
	{
		fprintf(stderr, "%s: Device name must be a tty device (eg. ttyI5).\n", Basename);

		Usage();
	}

	sprintf(VBoxPID		, PIDFILE , VBoxDevicetext);
	sprintf(VBoxLogname	, LOGFILE , VBoxDevicetext);
	sprintf(VBoxLockname	, LOCKFILE, VBoxDevicetext);

		/* Programm locken. `vbox' erzeugt eine PID Datei für das	*/
		/* aktuelle Device und lockt diese 'exclusiv', sodaß ein		*/
		/* erneuter Start mit dem gleichen Device nicht möglich		*/
		/* ist.																		*/

	if (!LockPID())
	{
		fprintf(stderr, "%s: can't create \"%s\".\n", Basename, VBoxPID);
		
		return(FALSE);
	}

		/* Logdatei für das aktuelle Device öffnen. */

	OpenLog(VBoxLogname);

		/* UID und GID des aktuellen Benutzers aus /etc/passwd holen	*/
		/* und in die globalen Variablen eintragen. Alle Verzeichnisse	*/
		/* und Dateien werden mit diesen Permissions versehen!			*/

	if (!*Username)
	{
		fprintf(stderr, "%s: can't spool voice mails without a username.\n", Basename);

		Usage();
	}

	if (!(User = getpwnam(Username)))
	{
		fprintf(stderr, "%s: user %s doesn't exists.\n", Basename, Username);
		
		return(FALSE);
	}

	UID = User->pw_uid;
	GID = User->pw_gid;

	strncpy(UserHome, User->pw_dir, PATH_MAX);

		/* Verzeichnisse für den Benutzer erzeugen. MkDir() kehrt nur	*/
		/* mit einem Fehler zurück, wenn das Verzeichnis nicht exis-	*/
		/* tiert hat und nicht erzeugt werden konnte.						*/

	if (!MkDir(Username, 		  "")) return(FALSE);
	if (!MkDir(Username, "incoming")) return(FALSE);
	if (!MkDir(Username, "messages")) return(FALSE);

		/* Ok */

	SetLogLevel(GetDebugMode(UID));

	LogLine(L_INFO, "Voice spooling for user %s (uid %d)...\n", Username, UID);

	return(TRUE);
}

/*************************************************************************
 ** MkDir():																				**
 *************************************************************************/

static boolean MkDir(char *Name, char *Dir)
{
	char NewDir[PATH_MAX + 1];

	strcpy(NewDir, SPOOLDIR);
	strcat(NewDir, "/");
	strcat(NewDir, Name);

	if ((Dir) && (*Dir))
	{
		strcat(NewDir, "/");
		strcat(NewDir, Dir);
	}

	if (mkdir(NewDir, S_IRUSR | S_IWUSR | S_IXUSR) != 0)
	{
		if (errno != EEXIST)
		{
			LogLine(L_ERROR, "Can't create directory \"%s\" (%s)\n", NewDir, strerror(errno));

			return(FALSE);
		}
		else LogLine(L_DEBUG, "Directory \"%s\" exists.\n", NewDir);
	}
	else LogLine(L_DEBUG, "Directory \"%s\" created.\n", NewDir);

	chown(NewDir, UID, GID);
	chown(NewDir, UID, GID);

	return(TRUE);
}

/*************************************************************************
 ** ExitVBox():																			**
 *************************************************************************/

static void ExitVBox(int RC)
{
	BlockSignals();

	if (RC < NORMALEXIT) LogLine(L_FATAL, "Got signal %d -- shutdown...\n", RC);

	ModemCloseDevice();

	UnlockPID();
	UnlockDevice();

	LogLine(L_INFO, "---------------------- End session -----------------------\n");

	CloseLog();

	exit(RC);
}

/*************************************************************************
 ** BlockSignals():																		**
 *************************************************************************/

static void BlockSignals(void)
{
	signal(SIGHUP		, SIG_IGN);
	signal(SIGINT		, SIG_IGN);
	signal(SIGQUIT		, SIG_IGN);
	signal(SIGILL		, SIG_IGN);
	signal(SIGTRAP		, SIG_IGN);
	signal(SIGABRT		, SIG_IGN);
	signal(SIGUNUSED	, SIG_IGN);
	signal(SIGUSR1		, SIG_IGN);
	signal(SIGUSR2		, SIG_IGN);
	signal(SIGPIPE		, SIG_IGN);
	signal(SIGALRM		, SIG_IGN);
	signal(SIGTERM		, SIG_IGN);
	signal(SIGSTKFLT	, SIG_IGN);
	signal(SIGCHLD		, SIG_IGN);
	signal(SIGTSTP		, SIG_IGN);
	signal(SIGTTIN		, SIG_IGN);
	signal(SIGTTOU		, SIG_IGN);
	signal(SIGIO		, SIG_IGN);
	signal(SIGXCPU		, SIG_IGN);
	signal(SIGXFSZ		, SIG_IGN);
	signal(SIGVTALRM	, SIG_IGN);
	signal(SIGPROF		, SIG_IGN);
	signal(SIGWINCH	, SIG_IGN);
}

/*************************************************************************
 ** Main():																					**
 *************************************************************************/

void Main(void)
{
	int Status;

	Status = STATE_INITIALIZE;

	while (Status != STATE_LEAVE)
	{
		switch (Status)
		{
				/* Das benutzte Device ist von einem anderen Prozeß ge-	*/
				/* lockt (oder noch vom eigenen?).								*/

			case STATE_DIALOUT:
			
				LogLine(L_DEBUG, "Device ist busy (dialout mode) - try again...\n");

				SleepSecs(5);
				
				if (LockDevice()) Status = STATE_INITIALIZE;
				
				break;

				/* Das Modem wird mit dem Initstring initialisiert und da-	*/
				/* nach in den Voicemodus versetzs (+FCLASS=8).					*/

			case STATE_INITIALIZE:

				LogLine(L_INFO, "Initialize modem (voice mode)...\n");

				if (LockDevice())
				{					
					if (ModemCommand(InitCommand, "OK") > 0)
					{
						if (ModemCommand("AT+FCLASS=8", "VCON|OK") > 0)
						{
							Status = STATE_WAITING;
						}
						else SleepSecs(2);
					}
					else SleepSecs(2);
				}
				else Status = STATE_DIALOUT;

				break;

				/* Das Modem wartet auf Anrufe. Nach MODEM_WAIT_TIMEOUT	*/
				/* Sekunden wird überprüft, ob das Modem noch auf An-		*/
				/* fragen reagiert.													*/

			case STATE_WAITING:

				ModemFlushFull();
				UnlockDevice();

				if (ModemWait(MODEM_WAIT_TIMEOUT))
				{
					Status = STATE_RING;
				}
				else Status = STATE_CHECKMODEM;

				break;

				/* Ein eingehender Anruf wurde bemerkt. Wenn noch genug	*/
				/* Platz auf der Festplatte ist, wird der Anruf nach ge-	*/
				/* nügend gefundenen RING's entgegengenommen.				*/

			case STATE_RING:

				if (LockDevice())
				{
					Status = STATE_INITIALIZE;

					if ((RingsNeeded = GetRingsToWait(UID)) > 0)
					{
						if (ModemGetRings(RingsNeeded))
						{
							if (FreeSpace(NEED_FREE_DISK_SPACE))
							{
								if (ModemCommand("ATA", "VCON|CONNECT") > 0)
								{
									ModemSetNoCarrier(FALSE);
									VoiceMode();
							
									Status = STATE_LEAVE;
								}
							}
							else LogLine(L_FATAL, "Not enough free disk space!\n");
						}
					}
				}
				else Status = STATE_DIALOUT;

				break;

				/* Nachdem der MODEM_WAIT_TIMEOUT abgelaufen ist, wird	*/
				/* ein Kommando an das Modem geschickt um zu sehen ob es	*/
				/* noch reagiert.														*/

			case STATE_CHECKMODEM:

				LogLine(L_DEBUG, "Checking modem (timeout)...\n");

				Status = STATE_LEAVE;

				if (LockDevice())
				{
					if (ModemCommand("AT", "OK") > 0) Status = STATE_WAITING;
				}
				else Status = STATE_DIALOUT;

				break;
		}
	}

	UnlockDevice();
}

/*************************************************************************
 ** FreeSpace():																			**
 *************************************************************************/

static boolean FreeSpace(unsigned long Bytes)
{

	return(TRUE);
}

/*************************************************************************
 ** StrToNumber():																		**
 *************************************************************************/
 
unsigned long StrToNumber(char *Text, unsigned long Default)
{
	char				*Stop;
	unsigned long	 Number;

	Number = strtoul(Text, &Stop, 10);

	if (*Stop != 0)
	{
		LogLine(L_WARN, "StrToNumber() can't convert \"%s\".\n", Text);

		Number = Default;
	}

	return(Number);
}
