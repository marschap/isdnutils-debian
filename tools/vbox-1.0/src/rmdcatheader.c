/* $Id: rmdcatheader.c,v 1.3 1996/06/09 13:10:56 root Exp $
 *
 * $Log: rmdcatheader.c,v $
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
	{ "adpcm2"	, no_argument, NULL, '2' },
	{ "adpcm3"	, no_argument, NULL, '3' },
	{ "adpcm4"	, no_argument, NULL, '4' },
	{ "ulaw"    , no_argument, NULL, 'u' },
	{ "alaw"    , no_argument, NULL, 'a' },
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
	short	Compression;

	Compression = 4;
	Basename		= argv[0];

	while ((Opts = getopt_long(argc, argv, "234uah", Arguments, (int *)0)) != EOF)
	{
		switch (Opts)
		{
			case '2':	Compression = 2;
							break;
			case '3':	Compression = 3;
							break;
			case '4':	Compression = 4;
							break;
			case 'a':	Compression = 5;
							break;
			case 'u':	Compression = 6;
							break;
			default:		Usage();
							break;
		}
	}

	if ((Compression < 2) || (Compression > 6))
	{
		fprintf(stderr, "%s: unknown compression.\n", Basename);
		
		Usage();
	}

	memset(&Header, 0, sizeof(struct VoiceHeader));
   
	strcpy(Header.Magic, VOICE_MAGIC);
	strcpy(Header.Modem, VOICE_MODEM);

	Header.Compression = htons(Compression);
            
	if (write(STDOUT_FILENO, &Header, sizeof(struct VoiceHeader)) != sizeof(struct VoiceHeader))
	{
		fprintf(stderr, "can't write voice header to stdout.\n");
	}
}

/*************************************************************************
 ** Usage():																				**
 *************************************************************************/

static void Usage(void)
{
	fprintf(stderr, "\nUsage: %s [compression] >OUTFILE\n\n", Basename);

	fprintf(stderr, "-2, --adpcm2   ADPCM-2 compression\n");
	fprintf(stderr, "-3, --adpcm3   ADPCM-3 compression\n");
	fprintf(stderr, "-4, --adpcm4   ADPCM-4 compression\n");
	fprintf(stderr, "-a, --alaw     ALAW compression\n");
	fprintf(stderr, "-u, --ulaw     ULAW compression\n\n");

	exit(5);
}
