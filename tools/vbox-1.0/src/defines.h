/* $Id: defines.h,v 1.3 1996/06/09 13:10:36 root Exp $
 */

#ifndef _VBOX_DEFINES
#define _VBOX_DEFINES 1

/** Defines **************************************************************/

#define CloseStream(S)		{ fclose(S); S = NULL; }
#define CloseFD(F)			{ close(F); F = -1; }
#define SleepSecs(S)			{ sleep(S); }

#define False()				return(0)
#define True()					return(1)
#define Cancel()				return(-1)
#define Delete					unlink

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef __NCURSES_H
typedef char bool;
typedef char boolean;
#endif /* __NCURSES_H */

#endif /* _VBOX_DEFINES_H */
