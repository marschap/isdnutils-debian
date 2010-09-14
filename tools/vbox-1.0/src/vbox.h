/* $Id: vbox.h,v 1.3 1996/06/09 13:10:37 root Exp $
 */
 
#ifndef _VBOX_H
#define _VBOX_H 1

#include <limits.h>
#include <unistd.h>

#define NORMALEXIT			(128)	 /* Alles unter diesem Exitcode stammt	*/
											 /* vom Signalhändler!						*/

#define STATE_WAITING		(0)		  /* Modem auf RING's warten lassen	*/
#define STATE_INITIALIZE	(1)			  /* Modem initialisieren lassen	*/
#define STATE_LEAVE			(2)					/* Hautpschleife verlassen	*/
#define STATE_CHECKMODEM	(3)					/* Modem überprüfen lassen	*/
#define STATE_RING			(4)			  /* Modem bekommt gerade RING's	*/
#define STATE_DIALOUT		(5)							/* Device ist belegt	*/

#define MAXCALLERIDLEN		(32)				 /* Max. Länge einer CallerID	*/
#define MAXUSERNAMELEN		(32)			  /* Max. Länge eines Usernamens	*/
#define MAXINITCOMMANDLEN	(128)

/** Variables ************************************************************/

extern char		VBoxPID[PATH_MAX + 1];
extern char		VBoxLockname[PATH_MAX + 1];
extern char		VBoxLogname[PATH_MAX + 1];
extern char		VBoxDevicetext[NAME_MAX + 1];
extern char		VBoxDevicename[PATH_MAX + 1];
extern char		CallerID[MAXCALLERIDLEN + 1];
extern char		Username[MAXUSERNAMELEN + 1];
extern char		UserHome[PATH_MAX + 1];
extern char		InitCommand[MAXINITCOMMANDLEN + 1];
extern uid_t	UID;
extern gid_t	GID;

/** Prototypes ***********************************************************/

extern unsigned long StrToNumber(char *, unsigned long);

#endif /* _VBOX_H */
