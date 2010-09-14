/* $Id: log.h,v 1.3 1996/06/09 13:10:36 root Exp $
 */
 
#ifndef _VBOX_LOG_H
#define _VBOX_LOG_H 1

#include <stdio.h>

#include "defines.h"

/** Loglevels ************************************************************/

#define L_FATAL			(0)
#define L_ERROR			(1)
#define L_WARN				(2)
#define L_INFO				(4)
#define L_DEBUG			(8)
#define L_JUNK				(16)

/** Prototypes ***********************************************************/

extern void	 SetLogLevel(int);
extern int	 GetLogLevel(void);
extern FILE	*OpenLog(char *);
extern void	 CloseLog(void);
extern void	 LogChar(int, char, int);
extern void	 LogLine(int, char *, ...);
extern void	 LogText(int, char *, ...);

#endif /* _VBOX_LOG_H */
