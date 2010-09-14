/*
 * Copyright 1989 - 1994, John F. Haugh II
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *     This product includes software developed by John F. Haugh, II
 *      and other contributors.
 * 4. Neither the name of John F. Haugh, II nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY JOHN HAUGH AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL JOHN HAUGH OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Extracted from age.c and made part of libshadow.a - may be useful
 * in other shadow-aware programs.  --marekm
 */

#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#ifndef BSD
#include <sys/wait.h>
#endif

#include <shadow.h>

#define DAY (24L*3600L)
#define SCALE DAY

extern time_t  time ();

/*
 * isexpired - determine if account is expired yet
 *
 *     isexpired calculates the expiration date based on the
 *     password expiration criteria.
 */

/*ARGSUSED*/

int
isexpired (pw, sp)
       const struct passwd *pw;
       const struct spwd *sp;
{
       long    now;
       now = time ((time_t *) 0) / SCALE;

       /*
        * Quick and easy - there is an expired account field
        * along with an inactive account field.  Do the expired
        * one first since it is worse.
        */

       if (sp->sp_expire > 0 && sp->sp_expire < now)
               return 3;

       /*
        * Hack: last changed date 1970-01-01 (not very likely) means that
        * the password must be changed as soon as possible.  This is used
        * by passwd -e (SunOS 4.x has a similar feature).  --marekm
        */
       if (sp->sp_lstchg == 0)
               return 1;

       if (sp->sp_inact > 0 && sp->sp_lstchg > 0 && sp->sp_max > 0 &&
                       sp->sp_inact + sp->sp_lstchg + sp->sp_max < now)
               return 2;
       /*
        * The last and max fields must be present for an account
        * to have an expired password.  A maximum of >10000 days
        * is considered to be infinite.
        */

       if (sp->sp_lstchg == -1 ||
                       sp->sp_max == -1 || sp->sp_max >= (10000L*DAY/SCALE))
               return 0;
       /*
        * Calculate today's day and the day on which the password
        * is going to expire.  If that date has already passed,
        * the password has expired.
        */

       if (sp->sp_lstchg + sp->sp_max < now)
               return 1;
       return 0;
}


