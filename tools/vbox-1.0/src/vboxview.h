/* $Id: vboxview.h,v 1.3 1996/06/09 13:10:37 root Exp $
 */
 
#ifndef _VBOXVIEW_H
#define _VBOXVIEW_H 1

#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

#define Scroll scrl

/*

# Messages 
###############################################################################
# N  27-12-69 15:20:00 0000123456  Jason                            939699926 #
# N  28-12-69 15:22:00 0000001234  Unknown caller                           - #
###############################################################################
*/

#define MAXNAMELEN		(20)
#define MAXCALLERIDLEN	(20)
#define MAXUSERNAMELEN	(32)
#define MAXCOMMANDLEN	(10240)

struct CallerLine
{
	char		New;
	char		Deleted;
	size_t	Size;
	char		Date[14];
	char		Name[MAXNAMELEN + 1];
	char		CallerID[MAXCALLERIDLEN + 1];
	char		Filename[NAME_MAX + 1];
};


#endif /* _VBOXVIEW_H */
