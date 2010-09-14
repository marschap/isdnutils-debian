/* $Id: rmdcutheader.c,v 1.3 1996/06/09 13:10:56 root Exp $
 *
 * $Log: rmdcutheader.c,v $
 * Revision 1.3  1996/06/09  13:10:56  root
 * o Neues Versionsschema.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>

#include "voice.h"

/** Prototypes ***********************************************************/

static void Usage(void);

/** Variables ************************************************************/

static char *Basename;

static struct option Arguments[] =
{
	{ "help"    , no_argument, NULL, 'h' },
	{ NULL      , 0          , NULL,  0  }
};

/*************************************************************************
 ** The magic main...																	**
 *************************************************************************/

void main(int argc, char **argv)
{
	struct VoiceHeader Header;

	int	Opts;
	char	Byte;
	short	Compression;

	Basename = argv[0];

	while ((Opts = getopt_long(argc, argv, "h", Arguments, (int *)0)) != EOF)
	{
		switch (Opts)
		{
			default:		Usage();
							break;
		}
	}

	if (read(STDIN_FILENO, &Header, sizeof(struct VoiceHeader)) != sizeof(struct VoiceHeader))
	{
		fprintf(stderr, "%s: can't read voice header.\n", Basename);

		exit(20);
	}

	if (strncmp(Header.Magic, VOICE_MAGIC, 4) != 0)
	{
		fprintf(stderr, "%s: unknown voice header found.\n", Basename);
		
		exit(21);
	}

	if (strcmp(Header.Modem, VOICE_MODEM) != 0)
	{
		fprintf(stderr, "%s: unknown modem type found.\n", Basename);
		
		exit(22);
	}

	Compression = ntohs(Header.Compression);
	
	if ((Compression < 2) || (Compression > 6))
	{
		fprintf(stderr, "%s. unknown compressen mode found.\n", Basename);
		
		exit(23);
	}

	while (read(STDIN_FILENO, &Byte, 1) == 1)
	{
		if (write(STDOUT_FILENO, &Byte, 1) != 1)
		{
			fprintf(stderr, "%s: can't write one byte to stdout.\n", Basename);
			
			exit(25);
		}
	}
}

/*************************************************************************
 ** Usage():																				**
 *************************************************************************/

static void Usage(void)
{
	fprintf(stderr, "\nUsage: %s <INFILE >OUTFILE\n\n", Basename);

	exit(5);
}
