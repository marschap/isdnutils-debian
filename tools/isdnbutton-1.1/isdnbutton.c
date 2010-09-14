/*  ISDN button V1.1
 *  Copyright (C) 1996  M. Gutschke
 *
 *  At the time of writing my e-mail address is:
 *	Internet: gutschk@uni-muenster.de
 *  My snail mail address is:
 *	Markus Gutschke
 *	Schlage 5a
 *	48268 Greven-Gimbte
 *	Germany
 *  If you like this software, I would appreciate if you sent me a postcard
 *  from your hometown. Under the terms of the GNU general public license
 *  you are free to include this program into (commercial) software
 *  distributions (e.g. putting it onto CD-ROM); nonetheless, I would really
 *  appreciate if you dropped me a short note (sending me a sample copy of
 *  your distribution would be even more appreciated!)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Label.h>
#include <X11/xpm.h>
#include <fcntl.h>
#include <linux/isdn.h>
#include <linux/isdnif.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <unistd.h>

#include "red.xpm"
#include "yellow.xpm"
#include "green.xpm"

typedef struct {
  Pixmap        pixmap;
  Pixmap        mask;
  XpmAttributes attributes;
} XpmIcon;

static Widget	    toplevel,icon;
static Display	    *display;
static Colormap	    colormap;
static XtAppContext app_context;
static XpmIcon      red,yellow,green;
static XpmIcon      *icons[3] = {&red,&yellow,&green};
static enum status {isRed,isYellow,isGreen} curStatus = isRed;
static XtIntervalId timer;
static Atom wmDeleteWindow;
static int           rootUID,rootGID;

static void setPixmap(enum status status)
{
  XpmIcon    *pm = icons[status];

  XtVaSetValues(icon,XtNbitmap,pm->pixmap,NULL);
  return;
}

static void testStatus(XtPointer closure,XtIntervalId *id)
{
  char                 buf[8192],*ptr;
  enum status          status = isRed;
  isdn_net_ioctl_phone phone;
  int                  fd,len;
  FILE                 *fp;

  setreuid(rootUID,-1); setregid(rootGID,-1);
  /* Check if any of the isdn network devices currently has a outgoing
     telephone number assigned to its interface */
  if ((fp = fopen("/proc/net/dev","r")) != NULL) {
    while (!feof(fp) && fgets(buf,sizeof(buf),fp)) {
      if ((ptr = strchr(buf,':')) != NULL) {
	*ptr = '\000';
	for (ptr = buf; *ptr == ' '; ptr++);
	if (*ptr && (fd = open("/dev/isdnctrl",O_RDONLY)) >= 0) {
	  strcpy(phone.name,ptr);
	  phone.outgoing = 1;
	  if (ioctl(fd,IIOCNETGNM,&phone) >= 0 &&
	      *phone.name && strcmp(phone.name,ptr))
	    status = isYellow;
	  close(fd); } } }
    fclose(fp); }
  /* Check whether any of the isdn network devices is currently active */
  if ((fd = open("/dev/isdninfo",O_RDONLY|O_NDELAY)) >= 0) {
    if ((len = read(fd,buf,sizeof(buf)-1)) >= 0) {
      buf[len] = '\000';
      for (ptr = buf; ptr && *ptr; (ptr = strchr(ptr,'\n')) ? ptr++ : 0) {
	if (!strncmp(ptr,"usage:\t",7))
	  for (ptr += 7; *ptr && *ptr != '\n'; ptr++)
	    if (*ptr != '0' && *ptr != ' ') {
	      status = isGreen; break; } } }
    close(fd); }
  setreuid(getuid(),-1); setregid(getgid(),-1);
  /* Update the status icon and reschedule the next test in 3 seconds time */
  if (status != curStatus)
    setPixmap(curStatus = status);
  timer = XtAppAddTimeOut(app_context,3000,testStatus,0);
  return;
}

static int open_max(void)
{
  #ifdef OPEN_MAX
  static int openmax = OPEN_MAX;
  #else
  static int openmax = 0;
  #endif

  if (openmax == 0) {
    errno = 0;
    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
      if (errno == 0)
	openmax = 256;
      else {
	syslog(LOG_ERR,"Sysconf error for _SC_OPEN_MAX");
	return(-1); } } }
  return(openmax);
}

static void inputCb(Widget widget,XtPointer closure,XEvent *event,Boolean *c)
{
  static int maxfd = -1;

  if (event->type == ButtonPress) {
    char  buf[2048];
    int   pfd[2],stat;
    pid_t pid;
    FILE  *fp;

    /* Avoid using "system" or "popen" as they introduce possible security
       holes */
    if (maxfd <  0) maxfd = open_max();
    if (pipe(pfd) < 0 || (pid = fork()) < 0)
      return;
    else if (pid == 0) { /* child */
      char *s,*env[] = { NULL };
      int  i;
      /* Open /dev/null on stdin and connect both stdout and stderr to the
	 pipe */
      close(pfd[0]); i = open("/dev/null",O_RDONLY);
      if (i != 0) { dup2(i,0); close(i); }/* stdin  */
      if (pfd[1] != 1) dup2(pfd[1],1);    /* stdout */
      if (pfd[1] != 2) dup2(pfd[1],2);    /* stderr */
      if (pfd[1] != 1 && pfd[1] != 2) close(pfd[1]);
      /* Close all other file handles, including the syslog */
      closelog();
      for (i = 3; i < maxfd; i++)
	close(i);
      /* From here on, run as priviledged user */
      setreuid(rootUID,-1); setregid(rootGID,-1);
      /* Launch the shell script */
      s = curStatus == isRed ?
	"/usr/sbin/isdn/connect" : "/usr/sbin/isdn/disconnect";
      execle(s,s+15,NULL,env);
      _exit(127); }
    /* Connect to the child and read all of its output, piping it to syslogd */
    close(pfd[1]);
    if ((fp = fdopen(pfd[0],"r")) == NULL) return;
    while (!feof(fp) && fgets(buf,sizeof(buf)-1,fp) != NULL)
      syslog(LOG_WARNING,"%sconnect: %s",curStatus == isRed ? "" : "dis",buf);
    fclose(fp);
    /* Report exit status */
    while (waitpid(pid,&stat,0) < 0 && errno == EINTR);
    if ((WIFEXITED(stat) && WEXITSTATUS(stat)) ||
	WIFSIGNALED(stat))
      syslog(LOG_ERR,WIFSIGNALED(stat) ?
	     "/usr/sbin/isdn/%sconnect terminated by signal %d" :
	     WEXITSTATUS(stat) == 127 ?
	     "Could not execute /usr/sbin/isdn/%sconnect" :
	     "/usr/sbin/isdn/%sconnect returns %d",
	     curStatus == isRed ? "" : "dis",
	     WIFSIGNALED(stat) ? WTERMSIG(stat) : WEXITSTATUS(stat));
    /* Reschedule next timer event immediately */
    XtRemoveTimeOut(timer);
    timer = XtAppAddTimeOut(app_context,200,testStatus,0); }
  *c = True;
  return;
}

static void quit (Widget w,XEvent *event,String *params,Cardinal *num_params)
{
  if (event->type == ClientMessage &&
      event->xclient.data.l[0] == wmDeleteWindow) {
    XCloseDisplay(XtDisplay(w));
    exit (0); }
  return;
}

static void zapEnvironment(char *keep,...)
{
  extern char **environ;
  char        **env = calloc(sizeof(char *),1);
  char        *val;
  int         count = 0;
  va_list     arg;

  /* Zap all of the environment except for a few selected variables */
  if (!keep) {
    environ = (char *[]){ NULL };
    return; }
  for (va_start(arg,keep); keep != NULL; keep = va_arg(arg,char *))
    if ((val = getenv(keep)) != NULL) {
      env = realloc(env,sizeof(char *)*(count+2));
      env[count++] = strcat(strcat(strcpy(malloc(strlen(keep)+strlen(val)+2),
					  keep),"="),val);
      env[count]   = NULL; }
  va_end(arg);
  environ = env;
  return;
}

int main (int argc, char *argv[])
{
  static XtActionsRec actions[] = {{"quit",quit}};
  static String       fallback[] = {"*background: MidnightBlue",NULL};
  static char         rgb[20];
  XColor              background;

  /* Close some of the more obvious security holes related to running setuid-
     root programs */
  zapEnvironment("DISPLAY","HOME",NULL);
  setrlimit(RLIMIT_CORE,&((struct rlimit){0,0}));
  rootUID = geteuid(); rootGID = getegid();
  setreuid(getuid(),-1); setregid(getgid(),-1);
  openlog("isdnbutton",LOG_PID,LOG_USER);
  /* Initialize the X toolkit and create a toplevel shell */
  if (!(toplevel = XtVaAppInitialize(&app_context,"isdnbutton",NULL,0,
				     &argc,argv,fallback,
				     XtNmappedWhenManaged,False,
				     XtNwidth,1,
				     XtNheight,1,
				     XtNinput,True,NULL))) {
    perror ("Can't open display or init Xtoolkit");
    return(1); }
  XtRealizeWidget(toplevel);
  display  = XtDisplay(toplevel);
  colormap = XDefaultColormapOfScreen(XtScreen(toplevel));
  /* Add support for the WM_DELETE_WINDOW protocol */
  XtAppAddActions(app_context,actions,XtNumber(actions));
  XtOverrideTranslations(toplevel,
		  XtParseTranslationTable("<Message>WM_PROTOCOLS: quit()"));
  wmDeleteWindow = XInternAtom(display,"WM_DELETE_WINDOW",False);
  /* Set the background color of the pixmaps to the same value that is
     used for the toplevel shell; this defaults to MidnightBlue, but can
     be overridden on the command line */
  XtVaGetValues(toplevel,XtNbackground,&background.pixel,NULL);
  XQueryColor(display,colormap,&background);
  sprintf(red_xpm[1] = yellow_xpm[1] = green_xpm[1] = rgb,
	  " \tc #%04X%04X%04X",
	  background.red,background.green,background.blue);
  /* Initialize the pixmaps for our three icons */
  if (XpmCreatePixmapFromData(display,XtWindow(toplevel),red_xpm,
	       &red.pixmap,&red.mask,&red.attributes) != XpmSuccess ||
      XpmCreatePixmapFromData(display,XtWindow(toplevel),yellow_xpm,
	       &yellow.pixmap,&yellow.mask,&yellow.attributes) != XpmSuccess ||
      XpmCreatePixmapFromData(display,XtWindow(toplevel),green_xpm,
	       &green.pixmap,&green.mask,&green.attributes) != XpmSuccess) {
	perror("Creating pixmap");
	return(1); }
  /* Create the window that displays the icon */
  icon = XtVaCreateManagedWidget("icon",labelWidgetClass,toplevel,
				 XtNborderWidth,0,
				 XtNbitmap,red.pixmap,
				 NULL);
  /* Adjust the size of the toplevel window to exactly fit the size of the
     icon */
  XtVaSetValues(toplevel,XtNwidth,red.attributes.width,
		XtNheight,red.attributes.height,NULL);
  /* Handle ButtonPress events for toggling the demand-dial status */
  XtAddEventHandler(toplevel,ButtonPressMask,ButtonPressMask,inputCb,NULL);
  /* Now lets get started */
  XMapWindow(display,XtWindow(toplevel));
  timer = XtAppAddTimeOut(app_context,200,testStatus,0);
  XSetWMProtocols(display,XtWindow(toplevel),&wmDeleteWindow,1);
  XtAppMainLoop(app_context);
  return(0);
}
