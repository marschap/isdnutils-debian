/* $Id: vboxview.c,v 1.3 1996/06/09 13:10:57 root Exp $
 *
 * $Log: vboxview.c,v $
 * Revision 1.3  1996/06/09  13:10:57  root
 * o Neues Versionsschema.
 *
 */

#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pwd.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

#include "vboxview.h"
#include "settings.h"
#include "log.h"
#include "vboxrc.h"
#include "defines.h"
#include "version.h"

/** Prototypes ***********************************************************/

static char		*GetCallerList(void);
static void	 	 Error(int, char *, ...);
static void	 	 ExitNCurses(void);
static void	 	 PrintCallerLine(int, long, char);
static void	 	 PrintAllCallers(void);
static void	 	 Main(void);
static void	 	 StopFunc(int);
static chtype	 Color(int);
static void		 ListviewPoint(float, float, float);
static void		 ClearListviewPoint(void);
static void		 PrintVolume(void);
static void		 Play(struct CallerLine *);
static void		 Bottom(char *);
static char		 Quit(void);
static void		 Beep(void);
static void		 MachineStatus(char, char *);
static void		 Usage(void);

/** Variables ************************************************************/

static char		*CallerList	= NULL;
static time_t	 SaveMTime	= 0;
static time_t	 SaveCTime	= 0;
static char		*Basename	= NULL;
static char		 ShowList	= FALSE;
static char		 Mono			= FALSE;
static char		 Graph		= TRUE;
static char		 Stop			= FALSE;
static char		 ViewMode	= FALSE;
static long		 NumCallers	= 0;
static long		 ErrCallers = 0;
static FILE		*NCursesO	= NULL;
static FILE		*NCursesI	= NULL;
static SCREEN	*Init			= NULL;
static uid_t	 UID			= -1;
static gid_t	 GID			= -1;
static int		 Timeout		= 0;
static int		 CurPosY		= 4; 
static long		 CurLine		= 0;

static char MessageDir[PATH_MAX + 1];
static char Device[NAME_MAX + 1];
static char Title[82];

static float ListviewPointY = 0;
static float Volume			 = 10.0;

static struct option Arguments[] =
{
	{ "device"		, required_argument, NULL, 'd' },
	{ "username"	, required_argument, NULL, 'u' },
	{ "update"		, required_argument, NULL, 'r' },
	{ "mono"			, no_argument		 , NULL, 'm' },
	{ "noscroller"	, no_argument		 , NULL, 's' },
	{ "help"			, no_argument		 , NULL, 'h' },
	{ NULL			, 0					 , NULL,  0  }
};

/*************************************************************************
 ** The magic main...																	**
 *************************************************************************/

void main(int argc, char **argv)
{
	struct passwd *User;

	int Opts;

	Basename = argv[0];
	ViewMode	= FALSE;

	UID = getuid();
	GID = getgid();

	*Device		= 0;
	*MessageDir	= 0;
	*Title		= 0;

	while ((Opts = getopt_long(argc, argv, "u:d:r:hms", Arguments, (int *)0)) != EOF)
	{
		switch (Opts)
		{
			case 's':
				Graph = FALSE;
				break;

			case 'r':
				Timeout = atoi(optarg);
				break;

			case 'm':
				Mono = TRUE;
				break;

			case 'u':
				if ((UID == 0) && (GID == 0))
				{
					if ((User = getpwnam(optarg)))
					{
						UID = User->pw_uid;
						GID = User->pw_gid;

						ViewMode = TRUE;
					}
					else Error(20, "%s: can't switch to user %s.\n", Basename, optarg);
				}
				else Error(20, "%s: only root can switch to other users.\n", Basename);

				break;

			case 'd':
				strncpy(Device, optarg, NAME_MAX);
				break;

			case 'h':
			default:
				Usage();
				break;
		}
	}

	if ((User = getpwuid(UID)))
	{
		strcpy(MessageDir, SPOOLDIR);
		strcat(MessageDir, "/");
		strcat(MessageDir, User->pw_name);
		strcat(MessageDir, "/incoming");
	}
	else Error(21, "%s: can't get settings for uid %d.\n", Basename, UID);

	if (!*Device)
	{
		strcpy(Device, "/dev/tty");
	}

	NCursesI = fopen(Device, "r");
	NCursesO = fopen(Device, "w");
                  	
	if ((!NCursesI) || (!NCursesO))
	{
		if (NCursesI) fclose(NCursesI);
		if (NCursesO) fclose(NCursesO);

		Error(22, "%s: can't setup screen for \"%s\".\n", Basename, Device);
	}

	if ((Init = newterm(NULL, NCursesO, NCursesI)))
	{
		if (!set_term(Init))
		{
			delscreen(Init);
				
			Init = NULL;
		}
	}
	
	if (!Init) Error(23, "%s: can't initialize screen.\n");	

	if ((LINES < 24) || (COLS < 80))
	{
		ExitNCurses();

		Error(24, "%s: your terminal must minimal have 80 columns and 24 lines.\n", Basename);
	}

	setuid(UID);
	setgid(GID);
	seteuid(UID);
	setegid(GID);

	if ((start_color() != ERR) && (!Mono))
	{
		init_pair(1, COLOR_WHITE , COLOR_BLACK);			  			/* Titel	*/
		init_pair(2, COLOR_WHITE , COLOR_RED  );						/* Titel	*/
		init_pair(3, COLOR_WHITE , COLOR_BLUE );					  /* Border	*/
		init_pair(4, COLOR_YELLOW, COLOR_BLUE );						 /* Text	*/
		init_pair(5, COLOR_YELLOW, COLOR_RED  );  				  /* Cursor	*/
		init_pair(6, COLOR_CYAN  , COLOR_BLUE );
	}

	GetCallerList();

	if (strlen(MessageDir) > 76)
	{
		strcpy(Title, "...");
		strcat(Title, &MessageDir[strlen(MessageDir) - 73]);
	}
	else strcpy(Title, MessageDir);

	attrset(Color(3) | A_BOLD);

	box(stdscr, ACS_VLINE, ACS_HLINE);
	move(1, 0);
	hline(ACS_HLINE, COLS);
	mvaddch(1, 0, ACS_ULCORNER);
	mvaddch(1, COLS - 1, ACS_URCORNER);
	move(LINES - 2, 0);
	hline(ACS_HLINE, COLS);
	mvaddch(LINES - 2, 0, ACS_LLCORNER);
	mvaddch(LINES - 2, COLS - 1, ACS_LRCORNER);
	mvprintw(1, (COLS - strlen(Title)) / 2, " %s ", Title);

	if (ViewMode)
	{
		mvprintw(LINES - 2, COLS - 13, " View mode ");
	}

	attrset(Color(1) | A_REVERSE);

	move(0, 0);
	hline(32, COLS);
	move(LINES - 1, 0);
	hline(32, COLS);

	PrintAllCallers();
	Bottom(NULL);
	Main();

	ExitNCurses();
	Error(0, "\n");
}

static void Usage(void)
{
	fprintf(stderr, "\nVersion %s - %s\n\n", VERSION, COPYRIGHT);
	fprintf(stderr, "Usage: %s Option(s)\n\n", Basename);
	fprintf(stderr, "-d, --device NAME     Output & input to device NAME\n");
	fprintf(stderr, "-u, --username NAME   Runs in viewmode for user NAME\n");
	fprintf(stderr, "-r, --update SECS     Updates the display of messages all SECS seconds\n");
	fprintf(stderr, "-m, --mono            Switchs to mono display\n");
	fprintf(stderr, "-s, --noscroller      Disables the position scroller\n\n");

	exit(5);
}

/*************************************************************************
 ** LogLine(): 																 [Dummy] **
 *************************************************************************/

void LogLine(int Level, char *Fmt, ...)
{
}

/*************************************************************************
 ** SetLogLevel():															 [Dummy] **
 *************************************************************************/

void SetLogLevel(int Level)
{
}

/*************************************************************************
 ** GetLogLevel():															 [Dummy] **
 *************************************************************************/

int GetLogLevel(void)
{
	return(0);
}

/*************************************************************************
 ** Beep():																					**
 *************************************************************************/

static void Beep(void)
{
	beep();
	refresh();
}

/*************************************************************************
 ** Color():																				**
 *************************************************************************/

static chtype Color(int Pair)
{
	if (!Mono) return(COLOR_PAIR(Pair));

	return(COLOR_PAIR(0));
}

/*************************************************************************
 ** StopFunc():																			**
 *************************************************************************/

static void StopFunc(int Sig)
{
	if (Sig != -1)
	{
		ExitNCurses();

		Error(30, "%s: Got signal %d -- shutdown...\n", Basename, Sig);
	}
	else Stop = FALSE;

	alarm(0);
	signal(SIGALRM , SIG_IGN);
	signal(SIGINT	, SIG_IGN);
	signal(SIGQUIT	, StopFunc);
	signal(SIGHUP	, StopFunc);
	signal(SIGTERM	, StopFunc);
}

/*************************************************************************
 ** Main():	Hauptschleife																**
 *************************************************************************/

static void Main(void)
{
	struct CallerLine *Line;

	int Key;

	setscrreg(4, LINES - 3);
	scrollok(stdscr, TRUE);
	curs_set(0);
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	if (Timeout > 0) timeout(Timeout * 1000);

	StopFunc(-1);
	ListviewPoint(LINES - 6, 0, 0);
	MachineStatus(FALSE, "on");

	while (!Stop)
	{
		PrintAllCallers();
		ListviewPoint(LINES - 6, CurLine, NumCallers);

		Line = (struct CallerLine *)(CallerList + (sizeof(struct CallerLine) * CurLine));

		switch ((Key = wgetch(stdscr)))
		{
			case 'O':
				if (!ViewMode)
				{
					MachineStatus(TRUE, "");
				}
				else Beep();
				break;

			case 'R':
			case 'r':
			case ERR:
				GetCallerList();
				break;
			
			case 'Q':
			case 'q':
				if (!ViewMode)
				{
					if (Quit()) Stop = TRUE;
				}
				else Stop = TRUE;
				break;

			case 'D':
			case 'd':
				if (!ViewMode)
				{
					Line->Deleted = (Line->Deleted ? FALSE : TRUE);
					PrintCallerLine(CurPosY, CurLine, TRUE);
					ungetch(KEY_DOWN);
				}
				else Beep();
				break;

			case KEY_UP:
				if (CurLine > 0)
				{
					if (CurPosY > 4)
					{
						PrintCallerLine(CurPosY--, CurLine--, FALSE);
						PrintCallerLine(CurPosY  , CurLine  , TRUE );
					}
					else
					{
						PrintCallerLine(CurPosY, CurLine--, FALSE);
						ClearListviewPoint();
						Scroll(-1);
						PrintCallerLine(CurPosY, CurLine, TRUE);
					}
				}
				break;

			case KEY_DOWN:
				if (CurLine < (NumCallers - 1))
				{
					if (CurPosY < (LINES - 3))
					{
						PrintCallerLine(CurPosY++, CurLine++, FALSE);
						PrintCallerLine(CurPosY  , CurLine  , TRUE );
					}
					else
					{
						PrintCallerLine(CurPosY, CurLine++, FALSE);
						ClearListviewPoint();
						Scroll(1);
						PrintCallerLine(CurPosY, CurLine, TRUE);
					}
				}
				break;

			case '+':
				if ((Volume < 50.0) && (!ViewMode))
				{
					Volume += 0.1;
					PrintVolume();
				}
				else Beep();
				break;
			
			case '-':
				if ((Volume >= 0.1) && (!ViewMode))
				{
					Volume -= 0.1;
					PrintVolume();
				}
				else Beep();
				break;

			case 'P':
			case 'p':
			case '\r':
			case '\n':
				if (!ViewMode)
				{
					Play(Line);
				}
				else Beep();
				break;

			case 'N':
			case 'n':
				if (!ViewMode)
				{
					if (Line->New)
					{
						chmod(Line->Filename, S_IRUSR | S_IWUSR | S_IXUSR);
						chmod(Line->Filename, S_IRUSR | S_IWUSR | S_IXUSR);

						Line->New = FALSE;
					}
					else
					{
						chmod(Line->Filename, S_IRUSR | S_IWUSR);
						chmod(Line->Filename, S_IRUSR | S_IWUSR);

						Line->New = TRUE;
					}
					PrintCallerLine(CurPosY, CurLine, TRUE);
					ungetch(KEY_DOWN);
				}
				else Beep();
				break;
		}
	}
}

/*************************************************************************
 ** WaitYesNo():																			**
 *************************************************************************/

static char WaitYesNo(char Default)
{
	cbreak();
	noecho();
	
	switch (wgetch(stdscr))
	{
		case 'Y':
		case 'y':
			return(TRUE);
			break;
		
		case 'N':
		case 'n':
			return(FALSE);
			break;

		case '\n':
		case '\r':
			return(Default);
			break;
	}

	return(FALSE);
}

/*************************************************************************
 ** Quit():																					**
 *************************************************************************/

static char Quit(void)
{
	struct CallerLine *Line;

	char Go;
	char Ok;
	long i;
	long k;

	Bottom("Do you really want to quit (Y/n)?");

	Go = WaitYesNo(TRUE);

	Bottom(NULL);

	if (Go)
	{
		for (i = 0; i < NumCallers; i++)
		{
			Line = (struct CallerLine *)(CallerList + (sizeof(struct CallerLine) * i));

			if (Line->Deleted)
			{
				Bottom("Delete all marked entries (y/N)?");
				
				Ok = WaitYesNo(FALSE);
				
				Bottom(NULL);
				
				if (Ok)
				{
					for (k = 0; k < NumCallers; k++)
					{
						Line = (struct CallerLine *)(CallerList + (sizeof(struct CallerLine) * k));

						if (Line->Deleted) unlink(Line->Filename);
					}
				}
				
				return(Go);
			}
		}

		return(Go);
	}

	return(Go);
}

/*************************************************************************
 ** PrintVolume():																		**
 *************************************************************************/

static void PrintVolume(void)
{
	attrset(Color(1) | A_REVERSE);

	move(LINES - 1, COLS - 5);

	if (Volume < 10) printw(" ");
	
	printw("%2.1f", Volume);

	refresh();
}

/*************************************************************************
 ** PrintAllCallers():																	**
 *************************************************************************/

static void PrintAllCallers(void)
{
	int i;

	if (ShowList)
	{
		attrset(Color(1) | A_REVERSE);

		move(0, 0);
		hline(32, COLS);

		mvprintw(0, 1, "VBox Version %s", VERSION);
		mvprintw(0, COLS - 28, "Messages:%4ld | Errors:%4ld", NumCallers, ErrCallers);

		mvaddch(0, COLS - 14, ACS_VLINE);
		mvaddch(0, COLS - 30, ACS_VLINE);

		attrset(Color(6) | A_BOLD);

		for (i = 2; i < (LINES - 2); i++)
		{
			move(i, 1);
			hline(32, COLS - 2);
		}

		mvprintw(2, 2, "*  Date     Time           Size  Name");
		mvprintw(2, COLS - 10, "CallerID");
		move(3, 2);
		hline(ACS_HLINE, COLS - 4);
	}

	if ((CallerList) && (NumCallers > 0) && (ShowList))
	{
		for (i = 0; i < NumCallers; i++)
		{
			if ((i + 4) > (LINES - 3)) break;

			PrintCallerLine(i + 4, i, FALSE);
		}

		CurPosY = 4;
		CurLine = 0;

		PrintCallerLine(CurPosY, CurLine, TRUE);
	}

	ShowList = FALSE;
}

/*************************************************************************
 ** PrintCallerLine():																	**
 *************************************************************************/

static void PrintCallerLine(int Y, long E, char Select)
{
	struct CallerLine *Line;

	curs_set(0);

	attrset(Color(3)|A_BOLD);
	mvaddch(Y, 0, ACS_VLINE);

	if (Select)
	{
		mvaddch(Y,        1, (Mono ? '>' : ' '));
		mvaddch(Y, COLS - 2, (Mono ? '<' : ' '));
	}
	else
	{
		mvaddch(Y,        1, ' ');
		mvaddch(Y, COLS - 2, ' ');
	}

	if (!Graph) mvaddch(Y, COLS - 1, ACS_VLINE);

	Line = (struct CallerLine *)(CallerList + (sizeof(struct CallerLine) * E));

	if (Select)
		attrset(Color(5) | A_BOLD);
	else
		attrset(Color(4) | A_BOLD);

	move(Y, 2);
	hline(32, COLS - 4);

	move(Y, 2);

	printw("%c%c ", (Line->New ? 'N' : ' '), (Line->Deleted ? 'D' : ' '));

	printw("%2.2s-%2.2s-%2.2s ", &Line->Date[4], &Line->Date[2], &Line->Date[0]);
	printw("%2.2s:%2.2s:%2.2s ", &Line->Date[6], &Line->Date[8], &Line->Date[10]);
	printw("%10ld  ", Line->Size);
	printw("%-20.20s", Line->Name);
	mvprintw(Y, COLS - 22, "%20.20s", Line->CallerID);

	if ((Graph) && (NumCallers > (LINES - 6)))
	{
		attrset(Color(1) | A_REVERSE);
		mvaddch(Y, COLS - 1, ACS_BOARD);
	}

	refresh();
}

/*************************************************************************
 ** ExitNCurses(): Beendet NCurses-Initialisierung.							**
 *************************************************************************/

static void ExitNCurses(void)
{
	clear();
	refresh();
	endwin();
	delscreen(Init);
	fclose(NCursesI);
	fclose(NCursesO);
}

/*************************************************************************
 ** Error(): Gibt Meldungen nach stderr aus und beendet das Programm.	**
 *************************************************************************/

static void Error(int RC, char *Fmt, ...)
{
	va_list Arg;
	
	va_start(Arg, Fmt);
	vfprintf(stderr, Fmt, Arg);
	va_end(Arg);

	if (CallerList) free(CallerList);

	exit(RC);
}

/*************************************************************************
 ** SortCallerList():																	**
 *************************************************************************/

static int SortCallerList(const void *A, const void *B)
{
	struct CallerLine *LineA = (struct CallerLine *)A;
	struct CallerLine *LineB = (struct CallerLine *)B;

	return(strcmp(LineB->Date, LineA->Date));
}

/*************************************************************************
 ** GetCallerList():																		**
 *************************************************************************/

static char *GetCallerList(void)
{
	struct dirent *DirEnt;
	struct stat Status;
	struct CallerLine *Line;

	char	*List		= NULL;
	char	*Temp		= NULL;
	char	*Name		= NULL;
	DIR	*Dir		= NULL;
	long   Callers	= 0;
	long   Errors	= 0;

		/* Untersuchen, ob sich seit dem letzten Zugriff auf das	*/
		/* Verzeichniss etwas geändert hat.								*/

	if (stat(MessageDir, &Status) != 0) return(CallerList);

	if ((Status.st_mtime == SaveMTime) && (Status.st_ctime == SaveCTime))
	{
		return(CallerList);
	}

	SaveMTime = Status.st_mtime;
	SaveCTime = Status.st_ctime;

		/* In das Incoming-Verzeichnis wechseln */

	if (chdir(MessageDir) != 0) return(CallerList);

		/* Incoming-Verzeichnis durchsuchen und alle passenden Nach-	*/
		/* richten in die neue Liste eintragen.								*/

	if (!(Dir = opendir(MessageDir))) return(CallerList);

	while ((DirEnt = readdir(Dir)))
	{
		Name = DirEnt->d_name;

		if ((strcmp(Name, ".") != 0) && (strcmp(Name, "..") != 0))
		{
			if ((isdigit(*Name)) && (Name[12] == '-'))
			{
				if (stat(Name, &Status) == 0)
				{
					if (Status.st_mode & S_IRUSR)
					{
						if ((Temp = realloc(List, (sizeof(struct CallerLine) * (Callers + 1)))))
						{
							List = Temp;
							Line = (struct CallerLine *)(List + (sizeof(struct CallerLine) * Callers));

							strncpy(Line->Date, Name, 12);
							strncpy(Line->Name, "*** Unknown ***", MAXNAMELEN);
							strncpy(Line->CallerID, &Name[13], MAXCALLERIDLEN);
							strncpy(Line->Filename, Name, NAME_MAX);

							Line->New		= TRUE;
							Line->Deleted	= FALSE;
							Line->Size		= Status.st_size;
							
							if (Status.st_mode & S_IXUSR) Line->New = FALSE;

							GetNameFromCallerID(getuid(), Line->CallerID, Line->Name, MAXNAMELEN);

							Callers++;
						}
						else Errors++;
					}
					else
					{
						SaveMTime = 0;
						SaveCTime = 0;
					}
				}
			}
		}
	}

	closedir(Dir);

		/* Wenn die neue Liste erzeugt werden konnte, wird die alte	*/
		/* freigegeben und durch die neue ersetzt.						*/

	if (CallerList) free(CallerList);

	CallerList	= List;
	ErrCallers	= Errors;
	NumCallers	= Callers;
	ShowList		= TRUE;

	qsort(List, Callers, sizeof(struct CallerLine), SortCallerList);

	return(CallerList);
}

/*************************************************************************
 ** ListviewPoint():																		**
 *************************************************************************/

static void ListviewPoint(float H, float P, float E)
{
	int Y;

	if ((Graph) && (NumCallers > (LINES - 6)))
	{
		if (E > 0)
		{
			ClearListviewPoint();

			Y = (int)((P / (E / H)));

			attrset(Color(2) | A_BOLD | A_REVERSE);
			mvaddch(Y + 4, COLS - 1, ACS_BLOCK);

			ListviewPointY = Y;
		}
		else ListviewPointY = 0;
	}
}

/*************************************************************************
 ** ClearListviewPoint():																**
 *************************************************************************/

static void ClearListviewPoint(void)
{
	if (Graph)
	{
		attrset(Color(1) | A_REVERSE);
		mvaddch(ListviewPointY + 4, COLS - 1, ACS_BOARD);
	}
}

/*************************************************************************
 ** Bottom():																				**
 *************************************************************************/

static void Bottom(char *Text)
{
	attrset(Color(1) | A_REVERSE);

	move(LINES - 1, 0);
	hline(32, COLS);

	if (!Text)
	{
		mvprintw(LINES - 1, 0, " Quit | Play | (Un)Delete | New/Read | Reread | On/Off |");

		mvprintw(LINES - 1, COLS - 17, "+/- Volume: ");

		mvaddch(LINES - 1,  6, ACS_VLINE);
		mvaddch(LINES - 1, 13, ACS_VLINE);
		mvaddch(LINES - 1, 26, ACS_VLINE);
		mvaddch(LINES - 1, 37, ACS_VLINE);
		mvaddch(LINES - 1, 46, ACS_VLINE);
		mvaddch(LINES - 1, 55, ACS_VLINE);

		mvaddch(LINES - 1, COLS - 19, ACS_VLINE);

		attrset(Color(2) | A_REVERSE);

		mvaddch(LINES - 1,  1, 'Q');
		mvaddch(LINES - 1,  8, 'P');
		mvaddch(LINES - 1, 19, 'D');
		mvaddch(LINES - 1, 28, 'N');
		mvaddch(LINES - 1, 39, 'R');
		mvaddch(LINES - 1, 48, 'O');

		mvaddch(LINES - 1, COLS - 17, '+');
		mvaddch(LINES - 1, COLS - 15, '-');

		PrintVolume();
	}
	else mvprintw(LINES - 1, 0, "%s", Text);

	refresh();
}

/*************************************************************************
 ** Play():																					**
 *************************************************************************/

static void Play(struct CallerLine *Line)
{
	char	Command[MAXCOMMANDLEN + 1];

	sprintf(Command, "%s \"%s/%s\" \"%2.1f\"", VBOXPLAYCMD, MessageDir, Line->Filename, Volume);

	Bottom("Running (Ctrl-C to stop)...");

	signal(SIGINT , SIG_DFL );
	system(Command);

	StopFunc(-1);

	Line->New = FALSE;

	chmod(Line->Filename, S_IRUSR | S_IWUSR | S_IXUSR);
	chmod(Line->Filename, S_IRUSR | S_IWUSR | S_IXUSR);

	PrintCallerLine(CurPosY, CurLine, TRUE);
	Bottom(NULL);
}

/*************************************************************************
 ** MachineStatus():																		**
 *************************************************************************/

static void MachineStatus(char Set, char *Mode)
{
	struct passwd *Pass;

	char	File[PATH_MAX + 1];
	FILE *Open;

	if (ViewMode) return;

	attrset(Color(3) | A_BOLD);
	move(LINES - 2, 2);
	hline(ACS_HLINE, 30);

	if (!(Pass = getpwuid(getuid())))
	{
		mvprintw(LINES - 2, 2, " Status: Unknown ");
		refresh();
		
		return;
	}

	strcpy(File, Pass->pw_dir);
	strcat(File, "/.vboxstop");

	if (Set)
	{
		if (access(File, F_OK) == 0)
		{
			unlink(File);
			unlink(File);
		}
		else
		{
			if ((Open = fopen(File, "w"))) fclose(Open);
			if ((Open = fopen(File, "w"))) fclose(Open);
		}
	}

	if (access(File, F_OK) == 0)
		mvprintw(LINES - 2, 2, " Status: Off ");
	else
		mvprintw(LINES - 2, 2, " Status: On ");

	refresh();
}
