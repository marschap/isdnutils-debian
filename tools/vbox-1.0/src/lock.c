/* $Id: lock.c,v 1.3 1996/06/09 13:10:56 root Exp $
 *
 * $Log: lock.c,v $
 * Revision 1.3  1996/06/09  13:10:56  root
 * o Neues Versionsschema.
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "vbox.h"
#include "lock.h"
#include "log.h"
#include "defines.h"

/** Variables ************************************************************/

static int PIDFD		= -1;
static int DeviceFD	= -1;

/*************************************************************************
 ** LockPID():																				**
 *************************************************************************/

boolean LockPID(void)
{
	char Temp[32];

	if (PIDFD != -1) True();

	PIDFD = open(VBoxPID, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (PIDFD != -1)
	{
		if (Lock(PIDFD, 5))
		{
			sprintf(Temp, "%d\n", getpid());
			write(PIDFD, Temp, strlen(Temp));

			True();
		}
		else UnlockPID();
	}

	False();
}

/*************************************************************************
 ** LockDevice():																			**
 *************************************************************************/

boolean LockDevice(void)
{
	char Temp[32];

	if (DeviceFD != -1) True();

	LogLine(L_DEBUG, "Locking \"%s\"...\n", VBoxDevicename);

	DeviceFD = open(VBoxLockname, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (DeviceFD != -1)
	{
		if (Lock(PIDFD, 5))
		{
			sprintf(Temp, "%10d\n", getpid());
			write(DeviceFD, Temp, strlen(Temp));

			True();
		}
		else UnlockDevice();
	}

	LogLine(L_ERROR, "Can't lock!\n");

	False();
}

/*************************************************************************
 ** UnlockPID():																			**
 *************************************************************************/

void UnlockPID(void)
{
	if (PIDFD != -1)
	{
		Unlock(PIDFD);
		
		close(PIDFD);
		unlink(VBoxPID);
	}
}

/*************************************************************************
 ** UnlockDevice():																		**
 *************************************************************************/

void UnlockDevice(void)
{
	if (DeviceFD != -1)
	{
		Unlock(DeviceFD);
		
		close(DeviceFD);
		unlink(VBoxLockname);
	}
}

/*************************************************************************
 ** Lock(): Lockt eine Datei exclusiv.												**
 *************************************************************************
 ** FD		File-Descriptor der Datei die gelockt werden soll.				**
 ** Trys		Anzahl der Versuche. Zwischen jedem Versuch wird eine Sek-	**
 **			unde Pause gemacht.														**
 *************************************************************************/

boolean Lock(int FD, int Trys)
{
	while (Trys > 0)
	{
		if (flock(FD, LOCK_EX | LOCK_NB) == 0) True();

		sleep(1);

		Trys--;
	}

	False();
}

/*************************************************************************
 ** Unlock(): Hebt den Lock einer Datei wieder auf.							**
 *************************************************************************
 ** FD		  File-Descriptor der Datei.											**
 *************************************************************************/

void Unlock(int FD)
{
	flock(FD, LOCK_UN);
}	
