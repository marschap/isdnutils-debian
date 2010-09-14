/*
 * ISDN accounting for isdn4linux. 
 *
 * Copyright 1996 by Stefan Luethje (luethje@konstanz.netsurf.de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#define PUBLIC extern

#include <pwd.h>
#include <tools.h>
#include "socket.h"

#ifdef _CLIENT_C_
#define _EXTERN
socket_queue *sockets = NULL;
#else
#define _EXTERN extern
extern socket_queue *sockets;
#endif

#endif /* _CLIENT_H_ */
