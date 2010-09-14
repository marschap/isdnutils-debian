/* $Id: vboxrc.c,v 1.3 1996/06/09 13:10:57 root Exp $
 *
 * $Log: vboxrc.c,v $
 * Revision 1.3  1996/06/09  13:10:57  root
 * o Neues Versionsschema.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <fnmatch.h>

#include "vboxrc.h"
#include "vbox.h"
#include "log.h"
#include "settings.h"

/** Prototypes ***********************************************************/

static boolean ReturnLine(char *, int);
static boolean GotoSection(char *);
static boolean ParseTime(char *);

/** Variables ************************************************************/

static FILE *VBoxRC	= NULL;
static long	 LineRC	= -1;

static struct DebugModes Debugging[] =
{
	{ "FATAL"	, L_FATAL	},
	{ "ERROR"	, L_ERROR	},
	{ "ERRORS"	, L_ERROR	},
	{ "WARN"		, L_WARN		},
	{ "WARNING"	, L_WARN		},
	{ "WARNINGS", L_WARN		},
	{ "INFO"		, L_INFO		},
	{ "INFOS"	, L_INFO		},
	{ "DEBUG"	, L_DEBUG	},
	{ "JUNK"		, L_JUNK		},
	{ NULL		, -1			}
};

/*************************************************************************
 ** OpenVBoxRC():																			**
 *************************************************************************/

FILE *OpenVBoxRC(uid_t UID)
{
	struct passwd *Pass;

	char File[PATH_MAX + 1];

	if (!VBoxRC)
	{
		if ((Pass = getpwuid(UID)))
		{
			strncpy(File, Pass->pw_dir, PATH_MAX);
			strncat(File, "/", PATH_MAX - strlen(File));
			strncat(File, VBOXRC, PATH_MAX - strlen(File));

			if ((VBoxRC = fopen(File, "r")))
			{
				LineRC = 0;

				LogLine(L_DEBUG, "User's vboxrc \"%s\" opened.\n", File);
			}
			else LogLine(L_WARN, "Can't open \"%s\".\n", File);
		}
		else LogLine(L_ERROR, "Can't get passwd entry for uid %ld.\n", UID);
	}
	else
	{
		LogLine(L_DEBUG, "Rewinding \"%s\".\n", File);

		rewind(VBoxRC);

		LineRC = 0;
	}

	return(VBoxRC);
}

/*************************************************************************
 ** CloseVBoxRC():																		**
 *************************************************************************/

void CloseVBoxRC(void)
{
	if (VBoxRC)
	{
		fclose(VBoxRC);
		
		VBoxRC = NULL;
		LineRC = -1;
	}
}

/*************************************************************************
 ** ReturnLine():	Gibt eine Zeile aus dem vboxrc zurück. Leere Zeilen	**
 **					oder Kommentarzeilen werden überlesen. Kommentare am	**
 **					Ende der Zeile werden entfernt. Rückgabe ist TRUE,		**
 **					wenn sich eine gültige Zeile im Buffer befindet, oder	**
 **					FALSE.																**
 *************************************************************************/

static boolean ReturnLine(char *Line, int Len)
{
	char *Comment;
	int	i;

	if (VBoxRC)
	{
		while (fgets(Line, Len, VBoxRC))
		{
			LineRC++;

			Line[strlen(Line) - 1] = 0;

			if ((*Line == 0) || (*Line == '#')) continue;

			if ((Comment = strchr(Line, '#'))) *Comment = 0;

			if (strlen(Line) > 0)
			{
				for (i = (strlen(Line) - 1); i >= 0; i--)
				{
					if (!isspace(Line[i])) break;

					Line[i] = 0;
				}
			}

			True();
		}
	}

	False();
}

/*************************************************************************
 ** GotoSection():	Spoolt das vboxrc bis zu einer bestimmten Sektion.	**
 *************************************************************************/

static boolean GotoSection(char *Need)
{
	char Line[MAXVBOXRCLINELEN + 1];

	while (ReturnLine(Line, MAXVBOXRCLINELEN))
	{
		if (strcasecmp(Line, Need) == 0) True();
	}

	False();
}

/*************************************************************************
 ** ParseTime():	Zerlegt einen String mit einzelnen Zeitangaben und		**
 **					untersuche, ob die aktuelle Stunde im angegebenen		**
 **					Rahmen liegt. Rückgabe ist TRUE oder FALSE.				**
 *************************************************************************/

static boolean ParseTime(char *Time)
{
	struct tm *Localtime;

	char		 TimeFlags[24];
	int	 	 i;
	int		 b;
	int		 e;
	char		*Beg;
	char		*End;
	time_t	 Currenttime;

	if (strcmp(Time, "*") == 0) True();
	if (strcmp(Time, "-") == 0) False();

	for (i = 0; i < 24; i++) TimeFlags[i] = 0;

	Currenttime = time(NULL);

	if (!(Localtime = localtime(&Currenttime)))
	{
		LogLine(L_ERROR, "Can't get localtime()...\n");
		
		False();
	}

	for (i = 0; i < strlen(Time); i++)
	{
		if ((!isdigit(Time[i])) && (Time[i] != ',') && (Time[i] != '-') && (Time[i] != ';'))
		{
			LogLine(L_ERROR, "Error in time string \"%s\" in line #%ld.\n", Time, LineRC);

			False();
		}
	}

	Beg = strtok(Time, ",;");

	while (Beg)
	{
		if ((End = strchr(Beg, '-')))
		{
			*End++ = 0;

			if ((*End == 0) || (*Beg == 0))
			{
				LogLine(L_ERROR, "Bad time range in line #%ld (%s-%s).\n", LineRC, Beg, End);

				False();
			}
		}
		else End = Beg;

		b = atoi(Beg);
		e = atoi(End);

		if ((b < 0) || (b > 23) || (e < 0) || (e > 23))
		{
			LogLine(L_ERROR, "Bad time range in line #%ld (%s-%s).\n", LineRC, Beg, End);

			False();
		}

		if (b <= e)
		{
			LogLine(L_JUNK, "Time range %d-%d (%s-%s).\n", b, e, Beg, End);

			for (i = b; i <= e; i++) TimeFlags[i] = 1;
		}
		else
		{
			LogLine(L_JUNK, "1th time range %d-%d (%s-%s).\n", b, 23, Beg, End);
			LogLine(L_JUNK, "2nd time range %d-%d (%s-%s).\n", 0,  e, Beg, End);

			for (i = b; i < 24; i++) TimeFlags[i] = 1;
			for (i = 0; i <= e; i++) TimeFlags[i] = 1;
		}

		Beg = strtok(NULL, ",:");
	}

	if (TimeFlags[Localtime->tm_hour] == 1) True();

	False();
}

/*************************************************************************
 ** GetRingsToWait():	Liefert die Anzahl der RING's zurück, die be-	**
 **							nötigt werden, damit der Anruf beantwortet		**
 **							wird. Bei einem Fehler wird 0 zurückgegeben.		**
 *************************************************************************/

long GetRingsToWait(uid_t UID)
{
	char	Line[MAXVBOXRCLINELEN + 1];
	long	Need;
	char *Time;
	char *Ring;
	char *Stop;

	Need = 0;

	if (OpenVBoxRC(UID))
	{
		LogLine(L_DEBUG, "Searching for number of rings to answer phone...\n");

		if (GotoSection("[RINGS]"))
		{
			while (ReturnLine(Line, MAXVBOXRCLINELEN))
			{
				if ((*Line == '[') || (*Line == ']')) break;

				Time = strtok(Line, "\t ");
				Ring = strtok(NULL, "\t ");

				if ((!Time) || (!Ring))
				{
					LogLine(L_ERROR, "Error in vboxrc line #%ld.\n", LineRC);
					
					continue;
				}

				LogLine(L_JUNK, "Test time \"%s\" (%s rings)...\n", Time, Ring);

				if (ParseTime(Time))
				{
					Need = (long)strtoul(Ring, &Stop, 10);

					if (*Stop != 0)
					{
						LogLine(L_ERROR, "Bad number of rings in line #%ld.\n", LineRC);

						Need = 0;
					}
					else break;
				}
			}
		}
		else LogLine(L_WARN, "Section \"[RINGS]\" not found.\n");

		CloseVBoxRC();
	}

	if (Need <= 0)
	{
		LogLine(L_WARN, "No rings -- Call will not answered.\n");
	}
	else LogLine(L_DEBUG, "Call will be answered after %ld rings...\n", Need);

	return(Need);
}

/*************************************************************************
 ** GetNameFromCallerID():	Trägt den Besitzernamen einer CallerID oder	**
 **								"*** Unknown ***" in den Buffer ein.			**
 *************************************************************************/

void GetNameFromCallerID(uid_t UID, char *ID, char *Name, int Len)
{
	char	Line[MAXVBOXRCLINELEN + 1];
	char *Owner;
	char *OwnID;
	int	i;

	strncpy(Name, "*** Unknown ***", Len);

	if (OpenVBoxRC(UID))
	{
		LogLine(L_DEBUG, "Searching owner for CallerID \"%s\"...\n", ID);

		if (GotoSection("[CALLERID]"))
		{
			while (ReturnLine(Line, MAXVBOXRCLINELEN))
			{
				if ((*Line == '[') || (*Line == ']')) break;

				Owner = Line;
				OwnID = strsep(&Owner, "\t ");

				if ((!Owner) || (!OwnID))
				{
					LogLine(L_ERROR, "Error in vboxrc line #%ld.\n", LineRC);

					continue;
				}

				for (i = 0; i < strlen(Owner); i++)
				{
					if (isspace(*Owner)) Owner++;
				}

				if (fnmatch(OwnID, ID, FNM_NOESCAPE | FNM_PERIOD) == 0)
				{
					strncpy(Name, Owner, Len);
					
					break;
				}
			}
		}
		else LogLine(L_WARN, "Section \"[RINGS]\" not found.\n");

		CloseVBoxRC();
	}

	LogLine(L_DEBUG, "Found owner \"%s\".\n", Name);
}

/*************************************************************************
 ** GetOwnerSettings():	Gibt die Aufnahmezeit und den Ansagetext eines	**
 **							Benutzers zurück. Wenn die Rückgabe -1 ist,		**
 **							konnte die Sektion des Benutzers nicht gefunden	**
 **							werden. Bei Rückgabe >= 0 (Aufnahmesekunden)		**
 **							befindet sich der Name des Ansagetextes im Buf-	**
 **							fer.															**
 *************************************************************************/

long GetOwnerSettings(uid_t UID, char *Owner, char *Message, int Len)
{
	char	Temp[MAXVBOXRCLINELEN + 4];
	char	Line[MAXVBOXRCLINELEN + 1];
	long	Secs;
	char *Time;
	char *Text;
	char *Strg;
	char *Stop;

	strncpy(Message, "standard.msg", Len);

	Secs = -1;

	if (OpenVBoxRC(UID))
	{
		LogLine(L_DEBUG, "Parsing settings for \"%s\"...\n", Owner);

		sprintf(Temp, "[%s]", Owner);

		if (GotoSection(Temp))
		{
			while (ReturnLine(Line, MAXVBOXRCLINELEN))
			{
				if ((*Line == '[') || (*Line == ']')) break;

				Time = strtok(Line, "\t ");
				Text = strtok(NULL, "\t ");
				Strg = strtok(NULL, "\t ");

				if ((!Time) || (!Text) || (!Strg))
				{
					LogLine(L_ERROR, "Error in vboxrc line #%ld.\n", LineRC);

					continue;
				}

				LogLine(L_JUNK, "Test time \"%s\" (%s; %s secs)...\n", Time, Text, Strg);

				if (ParseTime(Time))
				{
					Secs = (long)strtoul(Strg, &Stop, 10);

					if (*Stop != 0)
					{
						LogLine(L_ERROR, "Bad record time in line #%ld.\n", LineRC);

						Secs = -1;
					}
					else strncpy(Message, Text, Len);

					break;
				}
			}
		}
		else LogLine(L_WARN, "Section \"[%s]\" not found.\n", Owner);

		CloseVBoxRC();
	}

	if (Secs == -1)
	{
		LogLine(L_WARN, "Owner \"%s\" not found (or error).\n", Owner);
	}
	else LogLine(L_DEBUG, "Use %ld secs record time for owner \"%s\".\n", Secs);

	return(Secs);
}

/*************************************************************************
 ** GetDebugMode():																		**
 *************************************************************************/

int GetDebugMode(uid_t UID)
{
	char	Line[MAXVBOXRCLINELEN + 1];
	char *Word;
	int	Level;
	int	i;

	Level = GetLogLevel();

	if (OpenVBoxRC(UID))
	{
		LogLine(L_DEBUG, "Searching debug levels...\n");

		if (GotoSection("[DEBUG]"))
		{
			Level = L_FATAL;

			while (ReturnLine(Line, MAXVBOXRCLINELEN))
			{
				if ((*Line == '[') || (*Line == ']')) break;

				Word = strtok(Line, "\t +|");
				
				while (Word)
				{
					i = 0;
					
					while (Debugging[i].Name)
					{
						if (strcasecmp(Debugging[i].Name, Word) == 0)
						{
							LogLine(L_JUNK, "Found debug level \"%s\" (%d).\n", Debugging[i].Name, Debugging[i].Mode);

							Level |= Debugging[i].Mode;

							break;
						}

						i++;
					}

					Word = strtok(NULL, "\t +|");
				}
			}
		}
		else LogLine(L_WARN, "Section \"[DEBUG]\" not found.\n");

		CloseVBoxRC();
	}

	return(Level);
}
