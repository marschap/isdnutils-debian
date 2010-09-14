/***********************************************************************/
/*                                                                     */
/* Project      : iMON                                                 */
/* Current Rel. : 1.0                                                  */
/* Creator      : Michael Knigge                                       */
/* Creation Date: 08/18/95                                             */
/*                                                                     */
/***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <ncurses.h>

#include <linux/isdn.h>

#define KEY_Q                81
#define KEY_q               113

#define CURSOR_INVISIBLE      0   
#define CURSOR_NORMAL         1
#define CURSOR_BOLD           2

#define WHITE_ON_BLUE         1
#define YELLOW_ON_BLUE        2
#define RED_ON_BLUE           3
#define GREEN_ON_BLUE         4
#define WHITE_ON_BLACK        5

struct phone_entry {
   struct phone_entry *next;
   char phone[30];
   char name[30];
};

int imon_init_ncurses(int *);
int imon_draw_screen(int);
int imon_draw_status(int, time_t *);
static int wildmat(char *, char *);

static int  enable_quit = 1;
static int  show_names  = 0;
static FILE *isdninfo;
static int  color;
static char *phonebook;
static struct phone_entry *phones;

void readphonebook() {
  FILE *f;
  struct phone_entry *p;
  struct phone_entry *q;
  char line[255];
  char *s;

  if (!(f = fopen(phonebook, "r"))) {
    fprintf(stderr, "Can't open %s\n",phonebook);
  }
  p = phones;
  while (p) {
    q = p->next;
    free(p);
    p = q;
  }
  phones = NULL;
  while (fgets(line,sizeof(line),f)) {
    if ((s = strchr(line,'\n')))
      *s = '\0';
    if (line[0]=='#')
      continue;
    s = strchr(line,'\t');
    *s++ = '\0';
    if (strlen(line) && strlen(s)) {
      q = malloc(sizeof(struct phone_entry));
      q->next = phones;
      strcpy(q->phone,line);
      strcpy(q->name,s);
      phones = q;
    }
  }
  fclose(f);
  if (!phones)
    show_names = 0;
}

static int
Star(char *s, char *p) {
  while (wildmat(s, p) == 0)
    if (*++s == '\0')
      return(0);
  return(1);
}

static int
wildmat(char *s, char *p) {
  register int   last;
  register int   matched;
  register int   reverse;

  for ( ; *p; s++, p++)
    switch (*p) {
      case '\\':
        /* Literal match with following character; fall through. */
        p++;
      default:
        if (*s != *p)
          return(0);
        continue;
      case '?':
        /* Match anything. */
        if (*s == '\0')
          return(0);
        continue;
      case '*':
        /* Trailing star matches everything. */
        return(*++p ? Star(s, p) : 1);
      case '[':
        /* [^....] means inverse character class. */
        if ((reverse = (p[1] == '^')))
          p++;
        for (last = 0, matched = 0; *++p && (*p != ']'); last = *p)
          /* This next line requires a good C compiler. */
          if (*p == '-' ? *s <= *++p && *s >= last : *s == *p)
            matched = 1;
        if (matched == reverse)
          return(0);
        continue;
    }
  return(*s == '\0');
}

void find_name(char *num) {
  struct phone_entry *p = phones;

  while (p) {
    if (wildmat(num,p->phone)) {
      strcpy(num,p->name);
      break;
    }
    p = p->next;
  }
}

void
cleanup(int dummy) {
  /*
   * okay, all done. clear the screen and quit ncurses
   */
  fclose(isdninfo);
  clear();
  if (color == TRUE) {
    attron(COLOR_PAIR(WHITE_ON_BLACK));
    attroff(A_BOLD);
  }
  mvaddstr(0,0, "iMON                                                    (C) Michael Knigge, 1995");
  mvaddstr(1,0, "--------------------------------------------------------------------------------");
  mvaddstr(2,0, "I hope you like this little ISDN-Monitor. Send any bug-reports and suggestions");
  mvaddstr(3,0, "to knick@cove.han.de. I will do my very best ;-)  Thank you...");
  mvaddstr(5,0, "Have fun");
  mvaddstr(6,2, "Michael");
  curs_set(CURSOR_NORMAL);                               
  refresh();
  sleep(2);
  endwin();
  exit(0);
}

void
usage(void) {
   fprintf(stderr,"usage: imon [-q][-p PhoneBookFile]\n");
   exit(-1);
}

int
main(int argc, char **argv) {
  int   quit , i;
  fd_set fdset;
  struct timeval timeout;
  time_t current_time;

  /*
   * check parameters
   */
  phonebook = NULL;
  phones = NULL;
  for (i=1;i<argc;i++) {
    if (argv[i][0] == '-')
      switch (argv[i][1]) {
	case 'q':
          enable_quit = 0;
	  break;
	case 'p':
	  if (i<argc) {
            phonebook = argv[++i];
            show_names = 1;
          }
          else usage();
	  break;
        default:
          usage();
      } else
        usage();
  }

  if (phonebook)
    readphonebook();
  if (!(isdninfo = fopen("/dev/isdninfo", "r"))) {
    fprintf(stderr, "Can't open /dev/isdninfo\n");
    return(-1);
  }

  /*
   * initialize ncurses and draw the main screen
   */
  imon_init_ncurses(&color);
  imon_draw_screen(color);
  refresh();
  quit = 0;

  /*
   * Install Signal-Handlers
   */
  signal(SIGTERM,cleanup);
  if (enable_quit) {
    signal(SIGHUP,cleanup);
    signal(SIGINT,cleanup);
  } else {
    sigblock(SIGHUP);
    sigblock(SIGINT);
  }
  /*
   * loop until the user pressed  "Q"
   */
  do {
    FD_ZERO(&fdset);
    FD_SET(0,&fdset);
    FD_SET(fileno(isdninfo),&fdset);
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    switch (select(32,&fdset,(fd_set *)0,(fd_set *)0,NULL)) {
      case 0:
        fprintf(stderr,"timeout\n");
        break;
      case 1:
      case 2:
        if (FD_ISSET(0,&fdset))
          switch (getch()) {
 	   case KEY_Q:
 	   case KEY_q:
 	     quit = enable_quit;
 	     break;
           case 'R':
	   case 'r':
             if (phonebook)
               readphonebook();
           case 'S':
           case 's':
	     if (phones)
               show_names = (show_names)?0:1;
 	 }
        if (FD_ISSET(fileno(isdninfo),&fdset)) {
          time(&current_time);
 	  imon_draw_status(color, &current_time);
	  refresh();
        } 
        break;
      case -1:
        perror("select");
        sleep(5);
        exit(-1);
        break;
    }
  } while (!quit);
  cleanup(0);
  return 0;
}

int
imon_init_ncurses(int *color) {
  /*
   * init ncurses and check if colors can be used
   */
  initscr();
  noecho();
  nonl();
  refresh();
  cbreak();
  nodelay(stdscr,TRUE);
  curs_set(CURSOR_INVISIBLE);                               
  if ((*color = has_colors()) == TRUE) {
    start_color();
    init_pair(WHITE_ON_BLUE,   COLOR_WHITE,  COLOR_BLUE);
    init_pair(YELLOW_ON_BLUE,  COLOR_YELLOW, COLOR_BLUE);
    init_pair(RED_ON_BLUE,     COLOR_RED,    COLOR_BLUE);      
    init_pair(GREEN_ON_BLUE,   COLOR_GREEN,  COLOR_BLUE);
    init_pair(WHITE_ON_BLACK,  COLOR_WHITE,  COLOR_BLACK);
  }                        
  return(TRUE);
}

int
imon_draw_screen(int color) {
  int  line;
  int  col;
   
  if (color == TRUE) {
    attron(A_BOLD);
    attron(COLOR_PAIR(WHITE_ON_BLUE));
  }
  move(0,0);
  addch(ACS_ULCORNER);
  for(col=1;col<79;col++)
    addch(ACS_HLINE);
  addch(ACS_URCORNER);
  for(line=1; line<ISDN_MAX_CHANNELS+5; line++) {
    move(line,0);
    addch(ACS_VLINE);
    for(col=1; col<79; col++)
      addch(32);
    addch(ACS_VLINE);
  }
  move(21,0);
  addch(ACS_LLCORNER);
  for(col=1;col<79;col++)
    addch(ACS_HLINE);
  addch(ACS_LRCORNER);
  if (color == TRUE) {
    attroff(COLOR_PAIR(WHITE_ON_BLUE));
    attron(COLOR_PAIR(WHITE_ON_BLACK));
  }
  if (enable_quit) {
    move(ISDN_MAX_CHANNELS+7,5);
    addstr("Press Q to quit");
  }
  if (phonebook) {
    move(ISDN_MAX_CHANNELS+7,21);
    addstr("Press R - read Phonebook, S - Show ");
    addstr((show_names)?"phonenumbers":"names");
  }
  if(color == TRUE) {
    attroff(COLOR_PAIR(WHITE_ON_BLACK));
    attron(COLOR_PAIR(YELLOW_ON_BLUE));
  }
  mvaddstr(1,2,"iMON 1.1                                               Last Update: ");
  mvaddstr(3,2,"Channel      Status           Phone Number         Usage       Type");
  move(4,2);
  for(col=2; col<78; col++)
    addch(ACS_HLINE);
  if(color == TRUE)
    attroff(COLOR_PAIR(YELLOW_ON_BLUE));
  return(TRUE);
}

int
imon_draw_status(int color, time_t *current_time) {
  struct tm    *now;

  char idmap_line[4096];
  char chmap_line[4096];
  char drmap_line[4096];
  char usage_line[4096];
  char flags_line[4096];
  char phone_line[4096];

  char *ptr_idmap;
  char *ptr_chmap;
  char *ptr_drmap;
  char *ptr_usage;
  char *ptr_flags;
  char *ptr_phone;
   
  char channel[3];
  char status[10];  
  char phone[30];
  char usage[10];
  char type[10];  

  char temp[20];   

  int  local;
  int  chanum;
  int  line;
   
  /*
   * first, open the isdninfo device
   */
  local = 0;

  /*
   * read the five important lines
   */   
  fgets(idmap_line, 4095, isdninfo);
  fgets(chmap_line, 4095, isdninfo);
  fgets(drmap_line, 4095, isdninfo);
  fgets(usage_line, 4095, isdninfo);
  fgets(flags_line, 4095, isdninfo);
  fgets(phone_line, 4095, isdninfo);
  
  
#ifdef TEST_IMON
  sprintf(idmap_line, "idmap: TA250034 - - - - - - - - - - - - - - -");
  sprintf(chmap_line, "chmap: 0 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1");
  sprintf(drmap_line, "drmap: 0 0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1");   
  sprintf(usage_line, "usage: 1 2 0 3 4 0 5 0 129 0 130 131 132 0 0 133");
  sprintf(flags_line, "flags: 0 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?");
  sprintf(phone_line, "phone: 05114711 0815 ??? 5555 666666666 ??? 4711 ??? 875257 ??? 77848 7890 2345467 ??? ??? 88888");
#endif


  ptr_idmap = idmap_line + 7;
  ptr_chmap = chmap_line + 7;
  ptr_drmap = drmap_line + 7;
  ptr_usage = usage_line + 7;
  ptr_flags = flags_line + 7;
  ptr_phone = phone_line + 7;
   
  /*
   * now print info for every line
   */
  for(line=1; line<ISDN_MAX_CHANNELS+1; line++) {
    sscanf(ptr_idmap, "%s", channel);
    ptr_idmap = ptr_idmap + strlen(channel) + 1;
    sscanf(ptr_usage, "%d", &local);   
    sscanf(ptr_usage, "%s", temp);
    ptr_usage = ptr_usage + strlen(temp) + 1;
    sscanf(ptr_phone, "%s", phone);
    sscanf(ptr_chmap, "%d", &chanum);
    sscanf(ptr_chmap, "%s", temp);
    ptr_chmap = ptr_chmap + strlen(temp) + 1;

    /* A channel-number of -1 indicates an nonexistent channel */     
    if (chanum==-1)
      continue;    
    /*
     * if usage&7 is zero, there is no connection
     */
    if ((local&7) == 0) {
      if(color == TRUE)
	attron(COLOR_PAIR(GREEN_ON_BLUE));
      
      sprintf(status, "%s",(local&64)?"Excl.  ":"Offline");
      sprintf(phone, "                  ");
      sprintf(usage, "     ");
      sprintf(type, "         ");
      ptr_phone = ptr_phone + 4;
    } else {
      if(color == TRUE)
	attron(COLOR_PAIR(RED_ON_BLUE));
      sprintf(status, "Online ");
      sprintf(type, (local&ISDN_USAGE_OUTGOING)?"Outgoing":"Incoming");
      switch (local&7) {
	case ISDN_USAGE_RAW:
	  sprintf(usage, "Raw  ");
	  break;
	case ISDN_USAGE_MODEM:
	  sprintf(usage, "Modem");
	  break;
	case ISDN_USAGE_NET:
	  sprintf(usage, "Net  ");
	  break;
	case ISDN_USAGE_VOICE:
	  sprintf(usage, "Voice");
	  break;
	case ISDN_USAGE_FAX:
	  sprintf(usage, "Fax  ");
	  break;
	default:
	  sprintf(usage, "-----");
	  break;
      }
      ptr_phone = ptr_phone + strlen(phone) + 1;
      if (show_names)
        find_name(phone);
    }
    mvaddstr(line+4,3, channel);
    mvaddstr(line+4,15, status);
    mvaddstr(line+4,32, phone);
    mvaddstr(line+4,53, usage);
    mvaddstr(line+4,65, type);
  }
  now = localtime(current_time);
  sprintf(temp, "%02d:%02d:%02d", now->tm_hour,now->tm_min,now->tm_sec);
  if (color == TRUE)
    attron(COLOR_PAIR(YELLOW_ON_BLUE));
  mvaddstr(1,70, temp);
  return(TRUE);
}
