/* $Id: log.c,v 1.3 1996/06/09 13:10:56 root Exp $
 *
 * $Log: log.c,v $
 * Revision 1.3  1996/06/09  13:10:56  root
 * o Neues Versionsschema.
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
                     
#include "vbox.h"
#include "log.h"
#include "voice.h"
#include "defines.h"

static FILE *LogFile		= NULL;
static int	 LogLevel	= L_FATAL;

static void FlushLog(void)
{
	if (LogFile) fflush(LogFile);
}


void SetLogLevel(int Level)
{
	if (LogFile) LogLine(L_DEBUG, "Setting debug level to %d...\n", Level);

	LogLevel = Level;
}

int GetLogLevel(void)
{
	return(LogLevel);
}

FILE *OpenLog(char *Name)
{
	if ((LogFile = fopen(Name, "a")))
	{
		chmod(Name, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		chown(Name, 0, 0);

		LogLine(L_INFO, "------------------- Start new session --------------------\n");
	}

	return(LogFile);
}

void CloseLog(void)
{
	if (LogFile)
	{
		fclose(LogFile);
		
		LogFile = NULL;
	}
}

void LogChar(int Level, char C, int NextLine)
{
	if ((LogLevel & Level) || (Level == L_FATAL))
	{
		if (!LogFile) LogFile = fopen("/dev/console", "a");

		if (LogFile)
		{
			if (isprint(C))
			{
				fputc(C, LogFile);
			}
			else
			{
				switch (C)
				{
					case DLE:
						fprintf(LogFile, "<DLE>");
						break;

					case DC4:
						fprintf(LogFile, "<DC4>");
						break;
						
					case ETX:
						fprintf(LogFile, "<ETX>");
						break;
					
					default:
						fprintf(LogFile, "[0x%02X]", (unsigned char)C);
						break;
				}
			}

			if (NextLine) fputc('\n', LogFile);

			FlushLog();
		}
	}
}

void LogLine(int Level, char *Fmt, ...)
{
	struct tm   *Localtime;
	va_list      Arg;
	time_t       Time;
	char			 LogCode;
	char         TimeLine[20];

	if ((LogLevel & Level) || (Level == L_FATAL))
	{
		if (!LogFile) LogFile = fopen("/dev/console", "a");

		if (LogFile)
		{
			Time = time(NULL);
                        
			if ((Localtime = localtime(&Time)))
			{
				if (strftime(TimeLine, 20, "%d-%b %H:%M:%S", Localtime) != 15)
				{
					strcpy(TimeLine, "??-??? ??:??:??");
				}
			}

			switch (Level)
			{
				case L_FATAL:	LogCode = 'F';
									break;
				case L_ERROR:	LogCode = 'E';
									break;
				case L_WARN:	LogCode = 'W';
									break;
				case L_INFO:	LogCode = 'I';
									break;
				case L_DEBUG:	LogCode = 'D';
									break;
				case L_JUNK:	LogCode = 'J';
									break;
				default:			LogCode = '?';
									break;
			}

			fprintf(LogFile, "%s (%c) ", TimeLine, LogCode);

			va_start(Arg, Fmt);
			vfprintf(LogFile, Fmt, Arg);
			va_end(Arg);

			FlushLog();
		}
	}
}

void LogText(int Level, char *Fmt, ...)
{
	va_list      Arg;

	if ((LogLevel & Level) || (Level == L_FATAL))
	{
		if (!LogFile) OpenLog("/dev/console");

		if (LogFile)
		{
			va_start(Arg, Fmt);
			vfprintf(LogFile, Fmt, Arg);
			va_end(Arg);

			FlushLog();
		}
	}
}








