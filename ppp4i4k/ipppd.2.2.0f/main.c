/*
 * main.c - Point-to-Point Protocol main module
 *
 * Copyright (c) 1989 Carnegie Mellon University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Carnegie Mellon University.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * massive patches for MP/isdn4linux
 * copyright of all patches by Michael Hipp
 * still no warranties (see disclaimer above)
 */

#if 0
#ifndef lint
static char rcsid[] = "$Id: main.c,v 1.27 1995/08/16 01:39:08 paulus Exp $";
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <netdb.h>
#include <utmp.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <net/if.h>

#include "fsm.h"
#include "pppd.h"
#include "magic.h"
#include "lcp.h"
#include "ipcp.h"

#ifdef IPX_CHANGE
#include "ipxcp.h"
#endif /* IPX_CHANGE */

#include "upap.h"
#include "chap.h"
#include "ccp.h"
#include "pathnames.h"
#include "patchlevel.h"

/*
 * If REQ_SYSOPTIONS is defined to 1, pppd will not run unless
 * /etc/ppp/options exists.
 */
#ifndef	REQ_SYSOPTIONS
#define REQ_SYSOPTIONS	1
#endif

/* interface vars */

static char pidfilename[MAXPATHLEN];	/* name of pid file */
char *progname;			/* Name of this program */
char hostname[MAXNAMELEN];	/* Our hostname */
static pid_t	pid;		/* Our pid */

#if 0
static char default_devnam[MAXPATHLEN];	/* name of default device */
static pid_t	pgrpid;		/* Process Group ID */
#endif

static uid_t uid;		/* Our real user-id */

struct link_struct lns[NUM_PPP];
int numdev = 0;

int kill_link;

u_char outpacket_buf[PPP_MRU+PPP_HDRLEN]; /* buffer for outgoing packet */
static u_char inpacket_buf[PPP_MRU+PPP_HDRLEN]; /* buffer for incoming packet */

static int n_children;		/* # child processes still running */
int baud_rate;
char *no_ppp_msg = "Sorry - this system lacks PPP kernel support.\n"
	"Check whether you configured at least the ippp0 device!\n";

/* prototypes */
static void hup __P((int));
static void term __P((int));
static void chld __P((int));
static void toggle_debug __P((int));
static void open_ccp __P((int));
static void bad_signal __P((int));
static void get_input __P((int));
static int init_unit(int);
static int exit_unit(int);

#ifdef IPX_CHANGE
void remote_sys_options __P((void));
#endif /* IPX_CHANGE */

extern	char	*ttyname __P((int));
extern	char	*getlogin __P((void));
extern int daemon(int,int);

#ifdef ultrix
#undef	O_NONBLOCK
#define	O_NONBLOCK	O_NDELAY
#endif

/*
 * PPP Data Link Layer "protocol" table.
 * One entry per supported protocol.
 */
static struct protent {
    u_short protocol;
    void (*init)();
    void (*input)();
    void (*protrej)();
    int  (*printpkt)();
    void (*datainput)();
    char *name;
} prottbl[] = {
    { PPP_LCP, lcp_init, lcp_input, lcp_protrej,
	  lcp_printpkt, NULL, "LCP" },
    { PPP_IPCP, ipcp_init, ipcp_input, ipcp_protrej,
	  ipcp_printpkt, NULL, "IPCP" },
#ifdef IPX_CHANGE
    { PPP_IPXCP, ipxcp_init, ipxcp_input, ipxcp_protrej,
	  ipxcp_printpkt, NULL, "IPXCP" },
#endif /* IPX_CHANGE */
    { PPP_PAP, upap_init, upap_input, upap_protrej,
	  upap_printpkt, NULL, "PAP" },
    { PPP_CHAP, ChapInit, ChapInput, ChapProtocolReject,
	  ChapPrintPkt, NULL, "CHAP" },
    { PPP_CCP, ccp_init, ccp_input, ccp_protrej,
	  ccp_printpkt, ccp_datainput, "CCP" },
};

#define N_PROTO		(sizeof(prottbl) / sizeof(prottbl[0]))

void main(int argc,char **argv)
{
    int i,j;
    struct sigaction sa;
#if 0
    struct cmd *cmdp;
#endif
    FILE *pidfile;
    char *p;
    struct passwd *pw;
    struct timeval timo;
    sigset_t mask;

    for(i=0;i<NUM_PPP;i++)
    {
      lns[i].openfails = 0;
      lns[i].initfdflags = lns[i].initdisc = -1;
      lns[i].hungup = 1;
      lns[i].bundle_next = &lns[i];
      lns[i].ifname[0] = 0;
      lns[i].ifunit = -1;
      lns[i].open_ccp_flag = 0;
      lns[i].phase = PHASE_WAIT;
      lns[i].fd = -1;
      lns[i].logged_in = 0;
      lns[i].lcp_unit = lns[i].ipcp_unit = lns[i].ccp_unit = -1;
#ifdef IPX_CHANGE
      lns[i].ipxcp_unit = -1;
#endif
      lns[i].unit = i;
      lns[i].chap_unit = lns[i].upap_unit = -1;
    }

    /* write pid to file */
    (void) sprintf(pidfilename, "%s%s.pid", _PATH_VARRUN, "ppp??" );
    if ((pidfile = fopen(pidfilename, "w")) != NULL) {
	fprintf(pidfile, "%d\n", pid);
	(void) fclose(pidfile);
     } else {
	syslog(LOG_ERR, "Failed to create pid file %s: %m", pidfilename);
	pidfilename[0] = 0;
    }

#if 0
    p = ttyname(0);
    if (p)
	strcpy(devnam[0], p);
    strcpy(default_devnam, devnam);
#endif

    if (gethostname(hostname, MAXNAMELEN) < 0 ) {
	perror("couldn't get hostname");
	die(1);
    }
    hostname[MAXNAMELEN-1] = 0;

    uid = getuid();

    /*
     * Initialize to the standard option set, then parse, in order,
     * the system options file, the user's options file, and the command
     * line arguments.
     */
    progname = *argv;

    for (i = 0; i < N_PROTO; i++)
      for(j=0;j<NUM_PPP;j++)
	(*prottbl[i].init)(j); /* modifiziert unsere options .. !!!! */

    if (!options_from_file(_PATH_SYSOPTIONS, REQ_SYSOPTIONS, 0) ||
	!options_from_user() ||
	!parse_args(argc-1, argv+1) ||
	!options_for_tty())
	die(1);

    if (!ppp_available()) {
	fprintf(stderr, no_ppp_msg);
	exit(1);
    }

#ifdef IPX_CHANGE
    remove_sys_options();
#endif /* IPX_CHANGE */
    check_auth_options();
    setipdefault();

    if(!numdev)
    {
      fprintf(stderr,"ipppd: No devices found.\n");
      die(1);
    }
    else
    {
      char devstr[1024];
      sprintf(devstr,"Found %d devices: ",numdev);
      for(i=0;i<numdev;i++)
      {
        strcat(devstr,lns[i].devnam);
        strcat(devstr,", ");
      }
      syslog(LOG_NOTICE,devstr);
    }

    /*
     * If the user has specified the default device name explicitly,
     * pretend they hadn't.
     */
#if 0
    if (!default_device && strcmp(devnam, default_devnam) == 0)
	default_device = 1;
#endif

    /*
     * Initialize system-dependent stuff and magic number package.
     */
    sys_init();
    magic_init();

    /*
     * Detach ourselves from the terminal, if required,
     * and identify who is running us.
     */
    if (!default_device && !nodetach && daemon(0, 0) < 0) {
	perror("Couldn't detach from controlling terminal");
	exit(1);
    }
    pid = getpid();
    p = getlogin();
    if (p == NULL) {
	pw = getpwuid(uid);
	if (pw != NULL && pw->pw_name != NULL)
	    p = pw->pw_name;
	else
	    p = "(unknown)";
    }
    syslog(LOG_NOTICE, "pppd %s.%d started by %s, uid %d",
	   VERSION, PATCHLEVEL, p, uid);
  
    /*
     * Compute mask of all interesting signals and install signal handlers
     * for each.  Only one signal handler may be active at a time.  Therefore,
     * all other signals should be masked when any handler is executing.
     */
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGCHLD);

#define SIGNAL(s, handler)	{ \
	sa.sa_handler = handler; \
	if (sigaction(s, &sa, NULL) < 0) { \
	    syslog(LOG_ERR, "Couldn't establish signal handler (%d): %m", s); \
	    die(1); \
	} \
    }

    sa.sa_mask = mask;
    sa.sa_flags = 0;
    SIGNAL(SIGHUP, hup);		/* Hangup */
    SIGNAL(SIGINT, term);		/* Interrupt */
    SIGNAL(SIGTERM, term);		/* Terminate */
    SIGNAL(SIGCHLD, chld);

    SIGNAL(SIGUSR1, toggle_debug);	/* Toggle debug flag */
    SIGNAL(SIGUSR2, open_ccp);		/* Reopen CCP */

    /*
     * Install a handler for other signals which would otherwise
     * cause pppd to exit without cleaning up.
     */
    SIGNAL(SIGABRT, bad_signal);
    SIGNAL(SIGALRM, bad_signal);
    SIGNAL(SIGFPE, bad_signal);
    SIGNAL(SIGILL, bad_signal);
    SIGNAL(SIGPIPE, bad_signal);
    SIGNAL(SIGQUIT, bad_signal);
    SIGNAL(SIGSEGV, bad_signal);
#ifdef SIGBUS
    SIGNAL(SIGBUS, bad_signal);
#endif
#ifdef SIGEMT
    SIGNAL(SIGEMT, bad_signal);
#endif
#ifdef SIGPOLL
    SIGNAL(SIGPOLL, bad_signal);
#endif
#ifdef SIGPROF
    SIGNAL(SIGPROF, bad_signal);
#endif
#ifdef SIGSYS
    SIGNAL(SIGSYS, bad_signal);
#endif
#ifdef SIGTRAP
    SIGNAL(SIGTRAP, bad_signal);
#endif
#ifdef SIGVTALRM
    SIGNAL(SIGVTALRM, bad_signal);
#endif
#ifdef SIGXCPU
    SIGNAL(SIGXCPU, bad_signal);
#endif
#ifdef SIGXFSZ
    SIGNAL(SIGXFSZ, bad_signal);
#endif

        for(i=0;i<numdev;i++)
        {
          if(lns[i].fd == -1)
          {
            syslog(LOG_NOTICE,"init_unit: %d\n",i);
            if(!init_unit(i)) /* start this unit */
            {
              lns[i].hungup = 1;
              lns[i].lcp_unit = lcp_getunit(i);
/* give the same unit number to chap,upap */
              lns[i].upap_unit = lns[i].chap_unit = lns[i].lcp_unit; 
              upap[lns[i].upap_unit].us_unit = chap[lns[i].chap_unit].unit = i; 
              lcp_lowerup(lns[i].lcp_unit);
            }
          }
        }

        for(;;)
        {
          fd_set ready;
          int n,i,max=-1;
    
          FD_ZERO(&ready);
          for(i=0;i<numdev;i++)
            if(lns[i].fd >= 0)
            {
              if(lns[i].fd > max)
                max = lns[i].fd;
              FD_SET(lns[i].fd, &ready);
            }

          n = select(max+1, &ready, NULL, NULL, timeleft(&timo) );
          if (n < 0 && errno != EINTR)
          {
	    syslog(LOG_ERR, "select: %m");
	    die(1);
          }

          calltimeout();
          
          for(i=0;i<numdev && n>0;i++)
          {
            if(lns[i].fd >= 0 && FD_ISSET(lns[i].fd,&ready))
            {
            /*  n--; */
              if(lns[i].phase == PHASE_WAIT)
              {
                /* ok, now we (usually) have a unit */
                lns[i].hungup = 0;
                establish_ppp(i);
                syslog(LOG_NOTICE,"PHASE_WAIT -> PHASE_ESTABLISHED, ifunit: %d, linkunit: %d, fd: %d",lns[i].ifunit,i,lns[i].fd);
                lcp_open(lns[i].lcp_unit);
                lns[i].phase = PHASE_ESTABLISH;
              }
              get_input(i);
            }
            if (lns[i].open_ccp_flag) /* ugly: set by SIGUSR2 signal for all units */
            {
              if (lns[i].phase == PHASE_NETWORK) 
              {
                ccp_fsm[lns[i].ccp_unit].flags = OPT_RESTART; /* clears OPT_SILENT */
                ccp_open(lns[i].ccp_unit);
              }
	      lns[i].open_ccp_flag = 0;
	    }
          }
	  reap_kids();	/* Don't leave dead kids lying around */
          for(i=0;i<numdev;i++)
            if(lns[i].phase == PHASE_DEAD)
            {
              syslog(LOG_NOTICE,"taking down PHASE_DEAD link %d, linkunit: %d",i,lns[i].unit);
              lcp_close(lns[i].lcp_unit);
              lcp_lowerdown(lns[i].lcp_unit);
              lcp_freeunit(lns[i].lcp_unit);
              exit_unit(i);
              syslog(LOG_NOTICE,"link %d closed , linkunit: %d",i,lns[i].unit);
              lns[i].phase = PHASE_WAIT;
            }
          for(i=0;i<numdev;i++)
            if(lns[i].fd == -1)
            {
              syslog(LOG_NOTICE,"reinit_unit: %d\n",i);
              if(!init_unit(i)) /* protokolle hier neu initialisieren?? */
              {
                lns[i].lcp_unit = lcp_getunit(i);
                upap[i].us_unit = chap[i].unit = lns[i].lcp_unit; /* test */
                lcp_lowerup(lns[i].lcp_unit);
              }
            }
        }


  if (unlink(pidfilename) < 0 && errno != ENOENT) 
    syslog(LOG_WARNING, "unable to delete pid file: %m");
  pidfilename[0] = 0;
  die(0);
}

/*
 * initialize unit
 */

static int init_unit(int the_unit)
{
  if ((lns[the_unit].fd = open(lns[the_unit].devnam, O_NONBLOCK | O_RDWR, 0)) < 0) {
    if(errno == ENOENT)
    {
      syslog(LOG_ERR, "[%d] Failed to open %s: %m -> Disabling this unit",the_unit,lns[the_unit].devnam);
      syslog(LOG_NOTICE, "[%d] Failed to open %s: %m -> Disabling this unit",the_unit,lns[the_unit].devnam);
      lns[the_unit].fd = -2;
    }
    else 
    {
      syslog(LOG_ERR, "[%d] Failed to open %s: %m",the_unit,lns[the_unit].devnam);
      syslog(LOG_NOTICE, "[%d] Failed to open %s: %m",the_unit,lns[the_unit].devnam);
      lns[the_unit].fd = -1;
    }
    /* die(1); */
/* maybe we should start a callout here to retry the open after a few seconds */
    lns[the_unit].openfails++;
    if(lns[the_unit].openfails >= 16)
    {
      syslog(LOG_ERR, "Too much open fails on %s: %m", lns[the_unit].devnam);
      die(1);
    }
    return -1;
  }
  lns[the_unit].openfails = 0;
  if ((lns[the_unit].initfdflags = fcntl(lns[the_unit].fd, F_GETFL)) == -1) {
    syslog(LOG_ERR, "Couldn't get device fd flags: %m");
    die(1);
  }

#if 0
        lns[the_unit].initfdflags &= ~O_NONBLOCK;
        fcntl(lns[the_unit].fd, F_SETFL, lns[the_unit].initfdflags);

	/* set up the serial device as a ppp interface */
	establish_ppp(the_unit);
#endif

	/*
	 * Set device for non-blocking reads.
	 */
	if (fcntl(lns[the_unit].fd, F_SETFL, lns[the_unit].initfdflags | O_NONBLOCK) == -1) {
	    syslog(LOG_ERR, "Couldn't set device to non-blocking mode: %m");
	    die(1);
	}
	syslog(LOG_NOTICE, "Connect[%d]: %s, fd: %d",the_unit, lns[the_unit].devnam,lns[the_unit].fd);
  return 0;
}


static int exit_unit(int the_unit)
{
	if (lns[the_unit].initfdflags != -1)
        { 
          lns[the_unit].initfdflags &= ~O_NONBLOCK;
          if(fcntl(lns[the_unit].fd, F_SETFL, lns[the_unit].initfdflags) < 0)
            syslog(LOG_ERR,"Failed to set %s back to blocking mode %m",lns[the_unit].devnam);
          lns[the_unit].initfdflags = -1;
        }
	disestablish_ppp(the_unit);

#if 0
	if (disconnector) {
	    set_up_tty(the_unit, 1);
	    if (device_script(disconnector, lns[the_unit].fd, lns[the_unit].fd) < 0) {
		syslog(LOG_WARNING, "disconnect script failed");
	    } else {
		syslog(LOG_INFO, "Serial link disconnected.");
	    }
	}
#endif

	close_fd(the_unit);
  return 0;
}

/*
 * get_input - called when incoming data is available.
 */
static void get_input(int linkunit)
{
    int len, i;
    u_char *p;
    u_short protocol;

    p = inpacket_buf;	/* point to beginning of packet buffer */

    len = read_packet(inpacket_buf,linkunit);
    if (len < 0)
	return;

    if (len == 0) {
	syslog(LOG_NOTICE, "Modem hangup");
	lns[linkunit].hungup = 1;
#if 0
	lcp_lowerdown(lns[linkunit].lcp_unit);	/* serial link is no longer available */
#endif
	link_terminated(linkunit);
	return;
    }

    if (debug /*&& (debugflags & DBG_INPACKET)*/)
	log_packet(p, len, "rcvd ",linkunit);

    if (len < PPP_HDRLEN) {
	MAINDEBUG((LOG_INFO, "io(): Received short packet."));
	return;
    }

    p += 2;				/* Skip address and control */
    GETSHORT(protocol, p);
    len -= PPP_HDRLEN;

    /*
     * Toss all non-LCP packets unless LCP is OPEN.
     */
    if (protocol != PPP_LCP && lcp_fsm[lns[linkunit].lcp_unit].state != OPENED) {
	MAINDEBUG((LOG_INFO,
		   "io(): Received non-LCP packet when LCP not open."));
	return;
    }

    /*
     * Upcall the proper protocol input routine.
     */
    for (i = 0; i < sizeof (prottbl) / sizeof (struct protent); i++) {
	if (prottbl[i].protocol == protocol) {
	    (*prottbl[i].input)(linkunit, p, len);
	    return;
	}
        if (protocol == (prottbl[i].protocol & ~0x8000)
	    && prottbl[i].datainput != NULL) {
	    (*prottbl[i].datainput)(linkunit, p, len);
	    return;
	}
    }

    if (debug)
    	syslog(LOG_WARNING, "Unknown protocol (0x%x) received", protocol);
    lcp_sprotrej(linkunit, p - PPP_HDRLEN, len + PPP_HDRLEN);
}


/*
 * demuxprotrej - Demultiplex a Protocol-Reject.
 */
void demuxprotrej(int linkunit,u_short protocol)
{
    int i;

    /*
     * Upcall the proper Protocol-Reject routine.
     */
    for (i = 0; i < sizeof (prottbl) / sizeof (struct protent); i++)
	if (prottbl[i].protocol == protocol) {
	    (*prottbl[i].protrej)(linkunit);
	    return;
	}

    syslog(LOG_WARNING,
	   "demuxprotrej: Unrecognized Protocol-Reject for protocol 0x%x",
	   protocol);
}


/*
 * bad_signal - We've caught a fatal signal.  Clean up state and exit.
 */
static void
bad_signal(sig)
    int sig;
{
    syslog(LOG_ERR, "Fatal signal %d", sig);
    die(1);
}

/*
 * quit - Clean up state and exit (with an error indication).
 */
void 
quit()
{
    die(1);
}

/*
 * die - like quit, except we can specify an exit status.
 */
void die(int status)
{
    int i;
    for(i=0;i<numdev;i++)
      cleanup(0, NULL,i);
    syslog(LOG_INFO, "Exit.");
    exit(status);
}

/*
 * cleanup - restore anything which needs to be restored before we exit
 */
/* ARGSUSED */
void cleanup(int status,caddr_t arg,int tu)
{
    if (lns[tu].fd >= 0)
	close_fd(tu);

    if (pidfilename[0] != 0 && unlink(pidfilename) < 0 && errno != ENOENT) 
	syslog(LOG_WARNING, "unable to delete pid file: %m");
    pidfilename[0] = 0;

#if 0
    if (lockflag && !default_device)
	unlock();
#endif

}

/*
 * close_fd - restore the terminal device and close it.
 */
void close_fd(int tu)
{
    disestablish_ppp(tu);

    /* drop dtr to hang up */
    if (modem)
	setdtr(lns[tu].fd, FALSE);

    if (lns[tu].initfdflags != -1 && fcntl(lns[tu].fd, F_SETFL, lns[tu].initfdflags) < 0)
	syslog(LOG_WARNING, "Couldn't restore device fd flags: %m");
    lns[tu].initfdflags = -1;

    restore_tty(lns[tu].fd);

    close(lns[tu].fd);
    lns[tu].fd = -1;
}


struct	callout {
    struct timeval	c_time;		/* time at which to call routine */
    caddr_t		c_arg;		/* argument to routine */
    void		(*c_func)();	/* routine */
    struct		callout *c_next;
};

static struct callout *callout = NULL;	/* Callout list */
static struct timeval timenow;		/* Current time */

/*
 * timeout - Schedule a timeout.
 *
 * Note that this timeout takes the number of seconds, NOT hz (as in
 * the kernel).
 */
void
timeout(func, arg, time)
    void (*func)();
    caddr_t arg;
    int time;
{
    struct callout *newp, *p, **pp;
  
    MAINDEBUG((LOG_DEBUG, "Timeout %lx:%lx in %d seconds.",
	       (long) func, (long) arg, time));
  
    /*
     * Allocate timeout.
     */
    if ((newp = (struct callout *) malloc(sizeof(struct callout))) == NULL) {
	syslog(LOG_ERR, "Out of memory in timeout()!");
	die(1);
    }
    newp->c_arg = arg;
    newp->c_func = func;
    gettimeofday(&timenow, NULL);
    newp->c_time.tv_sec = timenow.tv_sec + time;
    newp->c_time.tv_usec = timenow.tv_usec;
  
    /*
     * Find correct place and link it in.
     */
    for (pp = &callout; (p = *pp); pp = &p->c_next)
	if (newp->c_time.tv_sec < p->c_time.tv_sec
	    || (newp->c_time.tv_sec == p->c_time.tv_sec
		&& newp->c_time.tv_usec < p->c_time.tv_sec))
	    break;
    newp->c_next = p;
    *pp = newp;
}


/*
 * untimeout - Unschedule a timeout.
 */
void
untimeout(func, arg)
    void (*func)();
    caddr_t arg;
{
#if 0
    struct itimerval itv;
    int reschedule = 0;
#endif
    struct callout **copp, *freep;
  
    MAINDEBUG((LOG_DEBUG, "Untimeout %lx:%lx.", (long) func, (long) arg));
  
    /*
     * Find first matching timeout and remove it from the list.
     */
    for (copp = &callout; (freep = *copp); copp = &freep->c_next)
	if (freep->c_func == func && freep->c_arg == arg) {
	    *copp = freep->c_next;
	    (void) free((char *) freep);
	    break;
	}
}


/*
 * calltimeout - Call any timeout routines which are now due.
 */
void
calltimeout()
{
    struct callout *p;

    while (callout != NULL) {
	p = callout;

	if (gettimeofday(&timenow, NULL) < 0) {
	    syslog(LOG_ERR, "Failed to get time of day: %m");
	    die(1);
	}
	if (!(p->c_time.tv_sec < timenow.tv_sec
	      || (p->c_time.tv_sec == timenow.tv_sec
		  && p->c_time.tv_usec <= timenow.tv_usec)))
	    break;		/* no, it's not time yet */

	callout = p->c_next;
	(*p->c_func)(p->c_arg);

	free((char *) p);
    }
}


/*
 * timeleft - return the length of time until the next timeout is due.
 */
struct timeval *
timeleft(tvp)
    struct timeval *tvp;
{
    if (callout == NULL)
	return NULL;

    gettimeofday(&timenow, NULL);
    tvp->tv_sec = callout->c_time.tv_sec - timenow.tv_sec;
    tvp->tv_usec = callout->c_time.tv_usec - timenow.tv_usec;
    if (tvp->tv_usec < 0) {
	tvp->tv_usec += 1000000;
	tvp->tv_sec -= 1;
    }
    if (tvp->tv_sec < 0)
	tvp->tv_sec = tvp->tv_usec = 0;

    return tvp;
}
    

/*
 * hup - Catch SIGHUP signal.
 *
 * Indicates that the physical layer has been disconnected.
 * We don't rely on this indication; if the user has sent this
 * signal, we just take the link down.
 */
static void hup(int sig)
{
    syslog(LOG_INFO, "Hangup (SIGHUP)");
/*    kill_link = 1; */
}


/*
 * term - Catch SIGTERM signal and SIGINT signal (^C/del).
 *
 * Indicates that we should initiate a graceful disconnect and exit.
 */
/*ARGSUSED*/
static void term(int sig)
{
    syslog(LOG_INFO, "Terminating on signal %d.", sig);
    persist = 0;		/* don't try to restart */
    kill_link = 1;
}


/*
 * chld - Catch SIGCHLD signal.
 * Calls reap_kids to get status for any dead kids.
 */
static void
chld(sig)
    int sig;
{
    reap_kids();
}


/*
 * toggle_debug - Catch SIGUSR1 signal.
 *
 * Toggle debug flag.
 */
/*ARGSUSED*/
static void toggle_debug(int sig)
{
    debug = !debug;
    note_debug_level();
}


/*
 * open_ccp - Catch SIGUSR2 signal.
 *
 * Try to (re)negotiate compression on all links (ugly).
 */
/*ARGSUSED*/
static void open_ccp(int sig)
{
  int i;
  for(i=0;i<NUM_PPP;i++)
    lns[i].open_ccp_flag = 1;
}


/*
 * device_script - run a program to connect or disconnect the
 * serial device.
 */
int device_script(char *program,int in,int out)
{
    int pid;
    int status;
    int errfd;

    pid = fork();

    if (pid < 0) {
	syslog(LOG_ERR, "Failed to create child process: %m");
	die(1);
    }

    if (pid == 0) {
	dup2(in, 0);
	dup2(out, 1);
	errfd = open(_PATH_CONNERRS, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (errfd >= 0)
	    dup2(errfd, 2);
	setuid(getuid());
	setgid(getgid());
	execl("/bin/sh", "sh", "-c", program, (char *)0);
	syslog(LOG_ERR, "could not exec /bin/sh: %m");
	_exit(99);
	/* NOTREACHED */
    }

    while (waitpid(pid, &status, 0) < 0) {
	if (errno == EINTR)
	    continue;
	syslog(LOG_ERR, "error waiting for (dis)connection process: %m");
	die(1);
    }

    return (status == 0 ? 0 : -1);
}


/*
 * run-program - execute a program with given arguments,
 * but don't wait for it.
 * If the program can't be executed, logs an error unless
 * must_exist is 0 and the program file doesn't exist.
 */
int run_program(char *prog,char **args,int must_exist,int unit)
{
    int pid;
    char *nullenv[1];

    pid = fork();
    if (pid == -1) {
	syslog(LOG_ERR, "Failed to create child process for %s: %m", prog);
	return -1;
    }
    if (pid == 0) {
	int new_fd;

	/* Leave the current location */
	(void) setsid();    /* No controlling tty. */
	(void) umask (S_IRWXG|S_IRWXO);
	(void) chdir ("/"); /* no current directory. */
	setuid(geteuid());
	setgid(getegid());

	/* Ensure that nothing of our device environment is inherited. */
	close (0);
	close (1);
	close (2);
	close (lns[unit].fd);  /* tty interface to the ppp device */
	/* XXX should call sysdep cleanup procedure here */

        /* Don't pass handles to the PPP device, even by accident. */
	new_fd = open (_PATH_DEVNULL, O_RDWR);
	if (new_fd >= 0) {
	    if (new_fd != 0) {
	        dup2  (new_fd, 0); /* stdin <- /dev/null */
		close (new_fd);
	    }
	    dup2 (0, 1); /* stdout -> /dev/null */
	    dup2 (0, 2); /* stderr -> /dev/null */
	}

#ifdef BSD
	/* Force the priority back to zero if pppd is running higher. */
	if (setpriority (PRIO_PROCESS, 0, 0) < 0)
	    syslog (LOG_WARNING, "can't reset priority to 0: %m"); 
#endif

	/* SysV recommends a second fork at this point. */

	/* run the program; give it a null environment */
	nullenv[0] = NULL;
	execve(prog, args, nullenv);
	if (must_exist || errno != ENOENT)
	    syslog(LOG_WARNING, "Can't execute %s: %m", prog);
	_exit(-1);
    }
    MAINDEBUG((LOG_DEBUG, "Script %s started; pid = %d", prog, pid));
    ++n_children;
    return 0;
}


/*
 * reap_kids - get status from any dead child processes,
 * and log a message for abnormal terminations.
 */
void
reap_kids()
{
    int pid, status;

    if (n_children == 0)
	return;
    if ((pid = waitpid(-1, &status, WNOHANG)) == -1) {
	if (errno != ECHILD)
	    syslog(LOG_ERR, "Error waiting for child process: %m");
	return;
    }
    if (pid > 0) {
	--n_children;
	if (WIFSIGNALED(status)) {
	    syslog(LOG_WARNING, "Child process %d terminated with signal %d",
		   pid, WTERMSIG(status));
	}
    }
}


/*
 * log_packet - format a packet and log it.
 */

char line[256];			/* line to be logged accumulated here */
char *linep;

void
log_packet(u_char *p,int len,char *prefix,int linkunit)
{
    strcpy(line, prefix);
    linep = line + strlen(line);
    format_packet(p, len, pr_log, NULL,linkunit);
    if (linep != line)
	syslog(LOG_DEBUG, "%s", line);
}

/*
 * format_packet - make a readable representation of a packet,
 * calling `printer(arg, format, ...)' to output it.
 */
void
format_packet(u_char *p,int len,void (*printer)(void*,char*,...),void *arg,int linkunit)
{
    int i, n;
    u_short proto;
    u_char x;

    if (len >= PPP_HDRLEN && p[0] == PPP_ALLSTATIONS && p[1] == PPP_UI) {
	p += 2;
	GETSHORT(proto, p);
	len -= PPP_HDRLEN;
	for (i = 0; i < N_PROTO; ++i)
	    if (proto == prottbl[i].protocol)
		break;
        printer(arg,"[%d]",linkunit);
	if (i < N_PROTO) {
	    printer(arg, "[%s", prottbl[i].name);
	    n = (*prottbl[i].printpkt)(p, len, printer, arg);
	    printer(arg, "]");
	    p += n;
	    len -= n;
	} else {
	    printer(arg, "[proto=0x%x]", proto);
	}
    }

    for (; len > 0; --len) {
	GETCHAR(x, p);
	printer(arg, " %.2x", x);
    }
}

#ifdef __STDC__
#include <stdarg.h>

void
pr_log(void *arg, char *fmt, ...)
{
    int n;
    va_list pvar;
    char buf[256];

    va_start(pvar, fmt);
    vsprintf(buf, fmt, pvar);
    va_end(pvar);

    n = strlen(buf);
    if (linep + n + 1 > line + sizeof(line)) {
	syslog(LOG_DEBUG, "%s", line);
	linep = line;
    }
    strcpy(linep, buf);
    linep += n;
}

#else /* __STDC__ */
#include <varargs.h>

void
pr_log(arg, fmt, va_alist)
void *arg;
char *fmt;
va_dcl
{
    int n;
    va_list pvar;
    char buf[256];

    va_start(pvar);
    vsprintf(buf, fmt, pvar);
    va_end(pvar);

    n = strlen(buf);
    if (linep + n + 1 > line + sizeof(line)) {
	syslog(LOG_DEBUG, "%s", line);
	linep = line;
    }
    strcpy(linep, buf);
    linep += n;
}
#endif

/*
 * print_string - print a readable representation of a string using
 * printer.
 */
void
print_string(p, len, printer, arg)
    char *p;
    int len;
    void (*printer) __P((void *, char *, ...));
    void *arg;
{
    int c;

    printer(arg, "\"");
    for (; len > 0; --len) {
	c = *p++;
	if (' ' <= c && c <= '~')
	    printer(arg, "%c", c);
	else
	    printer(arg, "\\%.3o", c);
    }
    printer(arg, "\"");
}

/*
 * novm - log an error message saying we ran out of memory, and die.
 */
void
novm(msg)
    char *msg;
{
    syslog(LOG_ERR, "Virtual memory exhausted allocating %s\n", msg);
    die(1);
}
