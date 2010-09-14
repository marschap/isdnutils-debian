/* $Id: vboxrc.h,v 1.3 1996/06/09 13:10:37 root Exp $
 */
 
#ifndef _VBOX_RC_H
#define _VBOX_RC_H 1

#include "defines.h"

#include <unistd.h>

/** Defines **************************************************************/

#define MAXVBOXRCLINELEN	(128)

/** Variables ************************************************************/

struct DebugModes
{
	char *Name;
	int	Mode;
};

/** Prototypes ***********************************************************/

extern FILE		*OpenVBoxRC(uid_t);
extern void		 CloseVBoxRC(void);
extern long		 GetRingsToWait(uid_t);
extern void		 GetNameFromCallerID(uid_t, char *, char *, int);
extern long		 GetOwnerSettings(uid_t, char *, char *, int);
extern int		 GetDebugMode(uid_t);

#endif /* _VBOX_RC_H */
