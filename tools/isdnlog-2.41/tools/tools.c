/* $Id: tools.c,v 2.40 1996/06/16 10:06:16 akool Exp akool $
 *
 * ISDN accounting for isdn4linux. (Utilities)
 *
 * Copyright 1995, 1996 by Andreas Kool (akool@Kool.f.EUnet.de)
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
 * $Log: tools.c,v $
 * Revision 2.40    1996/06/16  10:06:43  akool
 * double2byte(), time2str() added
 *
 * Revision 2.3.26  1996/05/05  12:09:16  akool
 * known.interface added
 *
 * Revision 2.3.15  1996/04/22  21:10:16  akool
 *
 * Revision 2.3.4  1996/04/05  11:12:16  sl
 * confdir()
 *
 * Revision 2.2.5  1996/03/25  19:41:16  akool
 * 1TR6 causes implemented
 *
 * Revision 2.23  1996/03/14  20:29:16  akool
 * Neue Routine i2a()
 *
 * Revision 2.17  1996/02/25  19:14:16  akool
 * Soft-Error in atom() abgefangen
 *
 * Revision 2.06  1996/02/07  18:49:16  akool
 * AVON-Handling implementiert
 *
 * Revision 2.01  1996/01/20  12:11:16  akool
 * Um Einlesen der neuen isdnlog.conf Felder erweitert
 * discardconfig() implementiert
 *
 * Revision 2.00  1996/01/10  20:11:16  akool
 *
 */


#define  PUBLIC /**/
#define  _TOOLS_C_

#include "tools.h"
#undef PUBLIC                    /*OBACHT!*/
#define PUBLIC extern
#include "../isdnlog/isdnlog.h"


char *CauseValue[] = { /* Cause Values According to Q.850 - shamelessly stolen from q931.c */
  "",
  "Unallocated (unassigned) number",
  "No route to specified transit network",
  "No route to destination",
  "Send special information tone",
  "Misdialled trunk prefix",
  "Channel unacceptable",
  "Channel awarded and being delivered in an established channel",
  "Preemption",
  "Preemption - circuit reserved for reuse",
  "",
  "",
  "",
  "",
  "",
  "",
  "Normal call clearing",
  "User busy",
  "No user responding",
  "No answer from user (user alerted)",
  "Subscriber absent",
  "Call rejected",
  "Number changed",
  "",
  "",
  "",
  "non-selected user clearing",
  "Destination out of order",
  "Invalid number format (address incomplete)",
  "Facility rejected",
  "Response to Status enquiry",
  "Normal, unspecified",
  "",
  "",
  "No circuit/channel available",
  "",
  "",
  "",
  "Network out of order",
  "Permanent frame mode connection out-of-service",
  "Permanent frame mode connection operational",
  "Temporary failure",
  "Switching equipment congestion",
  "Access information discarded",
  "Requested circuit/channel not available",
  "",
  "Precedence call blocked",
  "Resource unavailable, unspecified",
  "",
  "Quality of service unavailable",
  "Requested facility not subscribed",
  "",
  "",
  "Outgoing calls barred within CUG",
  "",
  "Incoming calls barred within CUG",
  "",
  "Bearer capability not auhorized",
  "Bearer capability not presently available",
  "",
  "",
  "",
  "Inconsistency in designated outgoing access information and subscriber class",
  "Service or option not available, unspecified",
  "",
  "Bearer capability not implemented",
  "Channel type not implemented",
  "Requested facility not implemented",
  "Only restricted digital information bearer capability is available",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "Service or option not implemented",
  "",
  "Invalid call reference value",
  "Identified channel does not exist",
  "A suspended call exists, but this call identity does not",
  "Call identity in use",
  "No call suspended",
  "Call having the requested call identity has been cleared",
  "User not member of CUG",
  "Incompatible destination",
  "",
  "Non-existent CUG",
  "Invalid transit network selection",
  "",
  "",
  "",
  "Invalid message, unspecified",
  "Mandatory information element is missing",
  "Message type non-existent or not implemented",
  "Message not compatible with call state or message type non-existent or not implemented",
  "Information element/parameter non-existent or not implemented",
  "Invalid information element contents",
  "Message not compatible with call state",
  "Recovery on timer expiry",
  "Parameter non-existent or not implemented - passed on",
  "",
  "",
  "",
  "",
  "",
  "",
  "Message with unrecognized parameter discarded",
  "Protocol error, unspecified",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "Interworking, unspecified",
};


char *Months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
       	    	     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


static char proto[] = "                   0,000000000";
static int  cnf;


time_t atom(register char *p)
{
  register char     *p1 = p;
  auto 	   struct tm tm;


#ifdef DEBUG_1
  if (strlen(p) < 20) {
    print_msg(PRT_DEBUG_GENERAL, " DEBUG> Huch? atom(``%s'')\n", p);
    return((time_t)0);
  } /* if */
#endif

  tm.tm_mon = 0;

  while ((tm.tm_mon < 12) && memcmp(p1, Months[tm.tm_mon], 3)) tm.tm_mon++;

  p1 += 4;
  p = p1 + 2;

  *p = 0;

  day = tm.tm_mday = atoi(p1);

  p1 += 3;
  p = p1 + 2;
  *p = 0;
  tm.tm_hour = atoi(p1);

  p1 = ++p;
  p += 2;
  *p = 0;
  tm.tm_min = atoi(p1);

  p1 = ++p;
  p += 2;
  *p = 0;
  tm.tm_sec = atoi(p1);

  p1 = ++p;
  p += 4;
  *p = 0;

  tm.tm_year = atoi(p1 + 2);

#ifdef DEBUG_1
  if (tm.tm_year < 1995) {
    print_msg(PRT_DEBUG_GENERAL, " DEBUG> Huch? atom(): year=%d\n", tm.tm_year);
    return((time_t)0);
  } /* if */
#endif

  tm.tm_wday = tm.tm_yday;
  tm.tm_isdst = -1;

  return(mktime(&tm));
} /* atom */


static int createDB(char *fn, int force)
{
  register int         f;
  auto 	   char        s[BUFSIZ];
  auto 	   struct stat statbuf;


  sprintf(s, "%s%c%s.dir", confdir(), C_SLASH, fn);

  if (force || stat(s, &statbuf)) {
    if ((f = open(s, O_CREAT | O_TRUNC | O_RDWR, 0666)) >= 0) {
      close(f);

      sprintf(s, "%s%c%s.pag", confdir(), C_SLASH, fn);

      if ((f = open(s, O_CREAT | O_TRUNC | O_RDWR, 0666)) >= 0) {
        close(f);

      	return(1);
      } /* if */
    } /* if */
  } /* if */

  return(0);
} /* createDB */


static int openDB(char *fn)
{
  auto char s[BUFSIZ];


  sprintf(s, "%s%c%s", confdir(), C_SLASH, fn);
  dbm = dbm_open(s, O_RDWR | O_CREAT, 0666);
  return((dbm == (DBM *)NULL) ? -1 : 0);
} /* dbminit */


static void closeDB()
{
  if (dbm != (DBM *)NULL) {
    dbm_close(dbm);
    dbm = (DBM *)NULL;
  } /* if */
} /* dbmclose */


static void insertDB(char *num, char *area)
{
  key.dsize = strlen(num);
  key.dptr  = num;

  data.dsize = strlen(area) + 1;
  data.dptr = area;

  if (dbm_store(dbm, key, data, DBM_INSERT)) {
    fprintf(stderr, "\n%s: WARNING: Duplicate Entry ``%s'' in AVON in Line %d, ignored\n", myname, num, ln);

    data = dbm_fetch(dbm, key);

    fprintf(stderr, "%s: WARNING: Can't be ``%s'' and ``%s''!\n", myname, area, data.dptr);
    fprintf(stderr, "        <- Reading AVON, please wait ...");
  } /* if */
} /* insertDB */


#ifdef DEVEL
static void writeAVON()
{
  auto 	   char  s[BUFSIZ];
  auto 	   FILE *f;


  sprintf(s, "%s%c%s.new", confdir(), C_SLASH, AVON);

  if ((f = fopen(s, "w")) != (FILE *)NULL) {
    for (key = dbm_firstkey(dbm); key.dptr != NULL; key = dbm_nextkey(dbm)) {
      strcpy(s, key.dptr);
      *(s + key.dsize) = 0;
      data = dbm_fetch(dbm, key);
      fprintf(f, "%s:%s\n", s, data.dptr);
    } /* for */

    fclose(f);
  } /* if */
} /* writeAVON */
#endif


static void readAVON(char *fn)
{
  register char *p, *p1;
  auto 	   char  s[BUFSIZ];
  auto 	   FILE *f;


  ln = 0;

  if ((f = fopen(fn, "r")) != (FILE *)NULL) {

    fprintf(stderr, "        <- Reading AVON, please wait ...");

    while (fgets(s, BUFSIZ, f) != NULL) {

      ln++;

      if ((*s != '#') && (*s != '\n')) {

        if ((p = strchr(s, '\n'))) {
          *p = 0;

          if (*(p - 1) == '\r') /* I *hate* DOS! */
            *(--p) = 0;

          while (*--p == ' ')
            *p = 0;
        } /* if */

        if ((p = strchr(s, ':'))) { /* "avon.txt" Format */
          *p = 0;

          if (!(ln % 50))
            fprintf(stderr, "\r%s", s);

          insertDB(s, p + 1);
        }
        else if (*s == 'D') { /* "avon.uisdn" Format */
          if ((p = p1 = strchr(s, '+'))) {
            p -= 7;
            *p = 0;

            if ((p = strchr(p1, '-')))
              *p = 0;

            if (!(ln % 50))
              fprintf(stderr, "\r%s", p1 + 4);

            while ((p = strchr(s + 6, '_')))
              *p = ' ';

  	    insertDB(p1 + 4, s + 6);
          } /* if */
        }
        else
          fprintf(stderr, "%s: WARNING: Syntax error in AVON at Line %d, ignored\n", myname, ln);
      } /* if */
    } /* while */

    fclose(f);
    fprintf(stderr, "\n");
  } /* if */
} /* readAVON */


static char *NextItem(char *Line, int komma)
{
  register char *p;
  register char *Ptr = Line;


  while (*Line && (*Line != ' ') && (*Line != '\t'))
    Line++;

  p = Line;

  while ((*Line == ' ') || (*Line == '\t'))
    Line++;

  *p = 0;

       p = Line;

       while((Ptr = Check_Quote(Ptr, komma ? "\\$@;#" : S_ALL_QUOTE, QUOTE_DELETE)) != NULL);

  return(Line);
} /* NextItem */

static char*NextOption(char** Options, char* Delim)
{
	char *RetCode = NULL;
	char *Ptr = *Options;
	int Len;

	*Delim = '\0';

	if (Ptr == NULL)
		return NULL;

	if ((Ptr = Check_Quote(Ptr,";,",QUOTE_DELETE)) == NULL)
		Ptr = "";

	if (*Ptr == ',' || *Ptr == ';' || *Ptr == '\0')
	{
		*Delim = *Ptr;

		if (Ptr != *Options)
		{
			Len = Ptr-(*Options);
			RetCode = (char*) calloc(Len+1, sizeof(char));
			strncpy(RetCode,*Options,Len);
		}

		if (*Ptr == ',')
		{
			*Options = Ptr+1;
		}
		else
		{
			while(!isalpha(*Ptr) && *Ptr != '\0')
				Ptr++;

			if (*Ptr == '\0')
			  *Options = NULL;
			else
			  *Options = Ptr;
		}
	}

	return RetCode;
}

static void Append(char **Target, char*Source)
{
	if (*Target == NULL)
	{
		*Target = Source;
		return;
	}

	if (Source == NULL)
		return;

	*Target = (char*) realloc(*Target, sizeof(char)*(strlen(*Target)+strlen(Source)+2));
	strcat(*Target,",");
	strcat(*Target,Source);
}

static int SetFlags(KNOWN *FlagPtr, char *flags)
{
	auto char Delim = '\0';
	auto char *Ptr;
  auto int  NumArgs = 0;
	auto int  flag = 0;

	while (*flags == ' ' || *flags == '\t')
		flags++;

  FlagPtr->infoargs = NULL;

  while (flags && *flags) {
  	flag = 0;

    while(isalpha(*flags) || *flags == ' ' || *flags == '\t')
    {
      switch (*flags++) {
        case 'I' : flag |= RING_INCOMING; break;
        case 'O' : flag |= RING_OUTGOING; break;
        case 'R' : flag |= RING_RING    ; break;
        case 'C' : flag |= RING_CONNECT ; break;
        case 'B' : flag |= RING_BUSY    ; break;
        case 'A' : flag |= RING_AOCD    ; break;
        case 'E' : flag |= RING_ERROR   ; break;
        case 'H' : flag |= RING_HANGUP  ; break;
        case 'K' : flag |= RING_KILL    ; break;
        case 'L' : flag |= RING_LOOP    ; break;
        case 'S' : flag |= RING_SPEAK   ; break;
        case 'P' : flag |= RING_PROVIDER; break;
        case 'U' : flag |= RING_UNIQUE  ; break;
        case ' ' : break;
        case '\t': break;

        default : fprintf(stderr, "%s: WARNING: Unknown flag ``%c'' in file \"%s\" line %d, ignored\n", myname, *(flags-1), CONFFILE, ln);
                  break;
      } /* switch */
    }

    if (flag != 0)
    {
    	if (*flags == '=')
    	{
    	  flags++;
    	  FlagPtr->flags |= flag;
    	  FlagPtr->infoargs = (info_args**) realloc(FlagPtr->infoargs, sizeof(info_args*) * (NumArgs+2));
    	  FlagPtr->infoargs[NumArgs] = (info_args*) calloc(1, sizeof(info_args));

   		  FlagPtr->infoargs[NumArgs]->flag    = flag;
   		  FlagPtr->infoargs[NumArgs]->infoarg = NextOption(&flags,&Delim);

   		  if (Delim == ',')
   		  {
                         if ((Ptr = NextOption(&flags,&Delim)) != NULL)
                         {
                           FlagPtr->infoargs[NumArgs]->interval= atoi(Ptr);
                           free(Ptr);
                         }
   	 		}

    		  if (Delim == ',')
     		    do
     		    	Append(&(FlagPtr->infoargs[NumArgs]->time),
     		    	       NextOption(&flags,&Delim));
     		    while(Delim == ',');

        if (Delim != ';')
        {
          fprintf(stderr, "%s: WARNING: Syntax-Error in file \"%s\" line %d, ignored (\";\" expected, but found \"%c\")\n", myname, CONFFILE, ln, Delim);
          flags = NULL;
        }

    	  FlagPtr->infoargs[++NumArgs] = NULL;
    	}
    	else
    	{
        fprintf(stderr, "%s: WARNING: Syntax-Error in file \"%s\" line %d, ignored (\"=\" expected, but found \"%c\")\n", myname, CONFFILE, ln, *flags);
        flags = NULL;
      }
    }
  } /* while */

  return 0;
}

static int IsVariable(char *string)
{
  char s1[SHORT_STRING_SIZE] = "",
       s2[LONG_STRING_SIZE] = "";

  if (sscanf(string,"%[a-zA-Z0-9] = %[^\n]",s1,s2) == 2)
  {
    if (setenv(s1,s2,1) == 0)
      return 1;

    fprintf(stderr, "%s: WARNING: Error in file \"%s\" line %d: Can not set variable (%s)!\n", myname, CONFFILE, ln, strerror(errno));
  }

  return 0;
}

void readconfig(char *myname)
{
  register char *p;
  register int	 i;
  auto     int   start_ln = 0;
  auto     FILE *f;
  auto     char  s[BUFSIZ];
  auto 	   char *num, *who, *zone, *flags, *interface;
  auto     char *Ptr;

	ln = 0;

  mymsns   = 3;
  myprefix = NULL;
  currency = NULL;
  knowns   = retnum = 0;
  known = (KNOWN **)NULL;
  start_procs.infoargs = NULL;
  start_procs.flags    = 0;


  sprintf(s, "%s%c%s", confdir(), C_SLASH, CONFFILE);

  if ((f = fopen(s, "r")) != (FILE *)NULL) {

    while (FGets(s, BUFSIZ, f, &ln) != NULL) {
      if ((*s != '\0') && (*s != '\n')) {

        if ((p = strchr(s, '\n'))) {
          *p = 0;

          while (*--p == ' ')
            *p = 0;
        } /* if */

        if (!IsVariable(s))
        {
          num       = s;
          who	    = NextItem(num, 1);
          zone      = NextItem(who, 0);
          interface = NextItem(zone, 0);
          flags     = NextItem(interface, 0);

          if (*num && *who) {
            known = realloc(known, sizeof(KNOWN *) * (knowns + 1));

            if ((known[knowns] = (KNOWN *)malloc(sizeof(KNOWN)))) {

              if ((p = strchr(num, ','))) { /* MSN,SI */
      	        known[knowns]->si      = atoi(p + 1);
		*p = 0;
              }
              else
      	        known[knowns]->si      = 0;

      	      known[knowns]->num       = strdup(num);
      	      known[knowns]->who       = strdup(who);
      	      known[knowns]->zone      = atoi(zone);
	      known[knowns]->interface = strdup(interface);
              known[knowns]->flags     = 0;

              SetFlags(known[knowns], flags);

  	          known[knowns]->dur[CALLING] = known[knowns]->dur[CALLED] = 0.0;
  	          known[knowns]->eh = 0;
  	          known[knowns]->usage[DIALOUT] = known[knowns]->usage[DIALIN] = 0;
  	          known[knowns]->dm = 0.0;

              knowns++;
            }
            else {
              fprintf(stderr, "%s: WARNING: Out of memory in Line %d\n", myname, ln);
              break;
            } /* else */
          }
          else {
  	        fprintf(stderr, "%s: WARNING: Syntax error in Line %d, ignored\n", myname, ln);
            /* break; */
          } /* else */
        } /* else */
        else
          if (start_ln == 0 && getenv(VAR_START) != NULL)
            start_ln = ln;

      } /* if */

    } /* while */

    fclose(f);

    if ((Ptr = getenv(VAR_MYMSNS)) == NULL)
    {
	    fprintf(stderr, "%s: WARNING: Variable `%s' is not set, now is 3!\n", myname, VAR_MYMSNS);
	    mymsns = 3;
	  }
	  else
	  	mymsns = atoi(Ptr);

    if ((myprefix = getenv(VAR_MYPREFIX)) == NULL)
    {
	    fprintf(stderr, "%s: WARNING: Variable `%s' is not set!\n", myname, VAR_MYPREFIX);
	  }

    if ((Ptr = getenv(VAR_CURRENCY)) != NULL)
	  {
	    currency_factor = atof(Ptr);

	    if ((currency = strchr(Ptr, ',')) == NULL)
	      fprintf(stderr, "%s: WARNING: Syntax error in `CURRENCY' in Line %d, ignored\n", myname, ln);
	  }

	  if ((Ptr = getenv(VAR_START)) != NULL)
	  {
      ln = start_ln;
      SetFlags(&start_procs, Ptr);
    }

/* Wenn eine unbekannte Varible kommt, stuerzt isdnlog einfach ab  ;-) !!!!*/
    if (myprefix != NULL) {
      for (i = 0; i < mymsns; i++) {
        if (known != NULL && known[i]->num != NULL) {
          sprintf(s, "%s%s", myprefix, known[i]->num);
          free(known[i]->num);
          known[i]->num  = strdup(s);
        }
        else
        {
        	fprintf(stderr, "%s: ERROR: There are only %d MSN's, expected %d!\n",myname, i, mymsns);
          exit(1);
        }
      } /* for */
    }
    else
      fprintf(stderr, "%s: WARNING: Missing entry ``MYPREFIX''\n", myname);

  }
  else
    fprintf(stderr, "%s: WARNING: Can't open ``%s''\n", myname, s);


  sprintf(s, "%s%c%s", confdir(), C_SLASH, AVON);

  if (!access(s, R_OK)) {
    if (createDB(AVON, 0) && !openDB(AVON))
      readAVON(s);
    else
      (void)openDB(AVON);
  } /* if */
} /* readconfig */


void discardconfig(void)
{
  register int i,j;


  for (i = 0; i < knowns; i++) {
    free(known[i]->num);
    free(known[i]->who);
    free(known[i]->interface);

  	for (j = 0; known[i]->infoargs != NULL && known[i]->infoargs[j] != NULL; j++) {
    	free(known[i]->infoargs[j]->time);
    	free(known[i]->infoargs[j]->infoarg);
    	free(known[i]->infoargs[j]);
  	}

		free(known[i]->infoargs);
    free(known[i]);
  } /* for */

  free(known);

  currency = NULL;
  myprefix = NULL;

  if (start_procs.infoargs != NULL)
  {
  	int j;
    for (j = 0; start_procs.infoargs != NULL && start_procs.infoargs[j] != NULL; j++) {
      free(start_procs.infoargs[j]->time);
      free(start_procs.infoargs[j]->infoarg);
      free(start_procs.infoargs[j]);
    }
	  free(start_procs.infoargs);
	}

  closeDB();
} /* discardconfig */


static char *cclass(register char *p, register int sub)
{
  register int c, d, not, found;


  if ((not = (*p == NOT)))
    p++;

  found = not;

  do {
    if (!*p)
      return(NULL);

    c = *p & QCMASK;

    if (p[1] == '-' && p[2] != ']') {
      d = p[2] & QCMASK;
      p++;
    }
    else
      d = c;

    if ((c == sub) || ((c <= sub) && (sub <= d)))
      found = !not;
  } while (*++p != ']');

  return(found ? p + 1 : NULL);
} /* cclass */


int match(register char *s, register char *p)
{
  register int sc, pcc;


  if (!*s || !*p)
    return(0);

  while ((pcc = *p++ & QCMASK)) {
    sc = *s++ & QMASK;

    switch (pcc) {
      case '[' : if (!(p = cclass(p, sc)))
	           return(0);
	         break;

      case '?' : if (!sc)
	           return(0);
	         break;

      case '*' : s--;
	         do {
	           if (!*p || match(s, p))
	             return(1);
	         } while (*s++);
	         return(0);

      default  : if (sc != (pcc &~QUOTE))
	           return(0);
    } /* switch */
  } /* while */

  return(!*s);
} /* match */


char *num2nam(char *num, int si)
{
  register int i, n;


  if (*num) {
    for (n = 0; n < 2; n++) {
      for (i = 0; i < knowns; i++) {
        if (((known[i]->si == si) || n) && match(num, known[i]->num)) {
          if (++retnum == MAXRET)
            retnum = 0;

          cnf = i;
          return(strcpy(retstr[retnum], known[i]->who));
        } /* if */
      } /* for */
    } /* for */
  } /* if */

  cnf = -1;
  return("");
} /* num2nam */


char *double2str(double n, int l, int d, int flags)
{
  register char *p, *ps, *pd, *px;
  auto     int   decpt, sign, dec, dp;
  auto     char  buf[BUFSIZ];


  if (++retnum == MAXRET)
    retnum = 0;

  p = retstr[retnum] + l + 1;
  *p = 0;

  dec = d ? d : -1;
  dp = l - dec;

  *buf = '0';
  memcpy(buf + 1, ecvt(n, DIGITS, &decpt, &sign), DIGITS);

  ps = buf;
  px = ps + decpt + d;

  if (px >= buf) {
    int rfound = 0;
    pd = px + 1;

    if (*pd > '4') {
      pd++;
      rfound++;
    } /* if */

    if (rfound) {
      while (pd > px)
	if (*pd >= '5') {
	  pd--;
	  while (*pd == '9')
	    *pd-- = '0';
	  *pd += 1;
	}
	else
	  pd--;
    } /* if */

    if (*buf == '1')
      decpt++;
    else
      ps++;

    if ((dp < 2 + sign) || ((decpt ? decpt : 1) + sign) >= dp) {
      memset(retstr[retnum] + 1, '*', *retstr[retnum] = l);
      return(retstr[retnum] + 1);
    } /* if */

  } /* if */

  memcpy(retstr[retnum] + 1, proto + 21 - l + dec, *retstr[retnum] = l);

  if (!((decpt < 0) && ((dec + decpt) <= 0))) {
    pd = retstr[retnum] + dp - decpt;

    if (sign) {
      if (decpt > 0)
	*(pd - 1) = '-';
      else
	*(retstr[retnum] + dp - 2) = '-';
    } /* if */

    while (decpt-- > 0)
      *pd++ = *ps++;

    pd++; /* skip comma */

    while (d-- > 0)
      *pd++ = *ps++;
  } /* if */

  if (flags & DEB) {
    p = retstr[retnum] + 1;

    while (*p == ' ')
      p++;

    return(p);
  } /* if */

  return(retstr[retnum] + 1);

} /* double2str */


char *double2byte(double bytes)
{
  static   char   mode[4] = " KMG";
  register int    m = 0;


  if (++retnum == MAXRET)
    retnum = 0;

  while (bytes > 999.9) {
    bytes /= 1024.0;
    m++;
  } /* while */

  sprintf(retstr[retnum], "%s%cb", double2str(bytes, 5, 1, 0), mode[m]);

  return(retstr[retnum]);
} /* double2byte */


char *time2str(time_t sec)
{
  static   char   mode[3] = "smh";
  register int    m = 0;
  auto     double s = (double)sec;


  if (++retnum == MAXRET)
    retnum = 0;

  while (s > 59.9) {
    s /= 60.0;
    m++;
  } /* while */

  sprintf(retstr[retnum], "%s%c", double2str(s, 4, 1, 0), mode[m]);

  return(retstr[retnum]);
} /* time2str */


char *double2clock(double n)
{
  auto int x, h, m, s;


  if (++retnum == MAXRET)
    retnum = 0;


  if (n <= 0.0)
    sprintf(retstr[retnum], "        ");
  else {
#if 0
    x = floor(n);
#else
    x = (int)n;
#endif

    h = (int)(x / 60 / 60);
    x %= 60 * 60;
    m = (int)(x / 60);
    s = (int)(x % 60);

#if 0
    sprintf(retstr[retnum], "%2d:%02d:%02d.%02d", h, m, s,
                                                  (int)((n - x) * 100));
#else
    sprintf(retstr[retnum], "%2d:%02d:%02d", h, m, s);
#endif
  } /* else */

  return(retstr[retnum]);
} /* double2clock */


char *vnum(int chan, int who)
{
  register int  l = strlen(call[chan].num[who]);


  if (++retnum == MAXRET)
    retnum = 0;

  *call[chan].vorwahl[who] =
  *call[chan].rufnummer[who] =
  *call[chan].alias[who] =
  *call[chan].area[who] = 0;
  call[chan].confentry[who] = -1;

  if (!l) {       /* keine Meldung von der Vst (Calling party number fehlt) */
    sprintf(retstr[retnum], "?");
    return(retstr[retnum]);
  } /* if */

  strcpy(call[chan].alias[who], num2nam(call[chan].num[who], call[chan].si1));

  if (cnf > -1) {                    /* Alias gefunden! */
    call[chan].confentry[who] = cnf;
    strcpy(retstr[retnum], call[chan].alias[who]);
    return(retstr[retnum]);
  } /* if */

  if (!memcmp(call[chan].num[who], "0130", 4)) {      /* Well known's prefix? */
    strcpy(call[chan].area[who], "Toll-free");
    l = 4;
  }
  else if (!memcmp(call[chan].num[who], "0190", 4)) {
    strcpy(call[chan].area[who], "Service 190");
    l = 4;
  }
  else if (!memcmp(call[chan].num[who], "019", 3)) {
    strcpy(call[chan].area[who], "Service 19");
    l = 3;
  }
  else if (!memcmp(call[chan].num[who], "0161", 4)) {
    strcpy(call[chan].area[who], "Mobilfunknetz C");
    l = 4;
  }
  else if (!memcmp(call[chan].num[who], "0171", 4)) {
    strcpy(call[chan].area[who], "Mobilfunknetz D1");
    l = 4;
  }
  else if (!memcmp(call[chan].num[who], "0172", 4)) {
    strcpy(call[chan].area[who], "Mobilfunknetz D2");
    l = 4;
  }
  else if (!memcmp(call[chan].num[who], "0177", 4)) {
    strcpy(call[chan].area[who], "Mobilfunknetz E-Plus");
    l = 4;
  }
  else if (!memcmp(call[chan].num[who], "01188", 5)) {
    strcpy(call[chan].area[who], "Auskunft Inland");
    l = 5;
  } /* else */

  if (!*call[chan].area[who] && (dbm != NULL) && (l > 4)) {
    key.dptr = call[chan].num[who];

    l -= 3;

    while (l > 1) {
      key.dsize = l;
      data = dbm_fetch(dbm, key);

      if (data.dptr != NULL) {
        strcpy(call[chan].area[who], data.dptr);
        break;
      } /* if */

      l--;
    } /* while */
  }
  else
    l = 0;

  if (l > 1) {
    strncpy(call[chan].vorwahl[who], call[chan].num[who], l);
    strcpy(call[chan].rufnummer[who], call[chan].num[who] + l);
  }
  else {
    strcpy(call[chan].rufnummer[who], call[chan].num[who]);
    l = 0;
  } /* else */

  if (l)
    sprintf(retstr[retnum], "%s/%s, %s", call[chan].vorwahl[who],
    			    	    	 call[chan].rufnummer[who],
    			    	    	 call[chan].area[who]);
  else if (*call[chan].area[who])
    sprintf(retstr[retnum], "%s, %s", call[chan].rufnummer[who],
    			    	      call[chan].area[who]);
  else
    strcpy(retstr[retnum], call[chan].rufnummer[who]);

  return(retstr[retnum]);
} /* vnum */


char *i2a(int n, int l, int base)
{
  static   char  Digits[] = "0123456789abcdef";
  register char *p;
  register int	 dot = 0;


  if (++retnum == MAXRET)
    retnum = 0;

  p = retstr[retnum] + RETSIZE - 1;
  *p = 0;

  while (n || (l > 0)) {
    if (n) {
      *--p = Digits[n % base];
      n /= base;
    }
    else
      *--p = '0';

    l--;

    dot++;

    if (!(dot % 8))
      *--p = ' ';
    else if (!(dot % 4))
      *--p = '.';
  } /* while */

  return(((*p == ' ') || (*p == '.')) ? p + 1 : p);
} /* i2a */


char *tr6cause(int cause)
{
  register int  i;
  static   char s[BUFSIZ];

  typedef struct {
    int   nr;
    char *descr;
  } Cause1TR6;

  static Cause1TR6 cause_1tr6[] = {
    {0x00, "Normal Call Clearing"},
    {0x80, "Normal Call Clearing"},
    {0x81, "Invalid Call Reference"},
    {0x83, "Bearer Service Not Implemented"},
    {0x87, "Caller Identity unknown"},
    {0x88, "Caller Identity in Use"},
    {0x89, "No Channels available"},
    {0x8a, "No Channels available"},
    {0x90, "Facility Not Implemented"},
    {0x91, "Facility Not Subscribed"},
    {0xa0, "Outgoing calls barred"},
    {0xa1, "User Access Busy"},
    {0xa2, "Negative GBG"},
    {0xa3, "Unknown  GBG"},
    {0xa5, "No SPC known"},
    {0xb5, "Destination not obtainable"},
    {0xb8, "Number changed"},
    {0xb9, "Out Of Order"},
    {0xba, "No User Response"},
    {0xbb, "User Busy"},
    {0xbd, "Incoming Barred"},
    {0xbe, "Call Rejected"},
    {0xd8, "Invalid destination address"},
    {0xd9, "Network Congestion"},
    {0xda, "Remote User initiated"},
    {0xf0, "Local Procedure Error"},
    {0xf1, "Remote Procedure Error"},
    {0xf2, "Remote User Suspend"},
    {0xf3, "Remote User Resumed"},
    {0xff, "User Info Discarded"},
    {-1, ""}
  };

  for (i = 0; cause_1tr6[i].nr != -1 ; i++)
    if (cause_1tr6[i].nr == cause) {
      sprintf(s, "%s", cause_1tr6[i].descr);
      return(s);
    } /* if */

  sprintf(s, "Unknown cause type : 0x%02x", cause);
  return(s);
} /* 1tr6cause */

/******************************************************************************/

char *confdir(void)
{
  static char *confdirvar = NULL;

	if (confdirvar == NULL && (confdirvar = getenv(CONFDIR_VAR)) == NULL)
    confdirvar = CONFDIR;

  return(confdirvar);
} /* confdir */

/****************************************************************************/

char **String_to_Array(char* String, char Trenn)
{
	char  *Ptr     = String;
	char **RetCode = NULL;
	char  *Ptr1    = NULL;
	char  *Ptr2    = NULL;
	int Cnt = 2;

	if (*String == '\0')
		return NULL;

	while((Ptr = strchr(Ptr,Trenn)) != NULL)
	{
		Cnt++;
		Ptr++;
	}

	RetCode = (char**) calloc(Cnt,sizeof(char*));
	Ptr     = (char*) calloc(strlen(String)+1,sizeof(char));

	if (RetCode == NULL || Ptr == NULL)
		return NULL;

	strcpy(Ptr,String);

	Cnt = 0;
	Ptr1 = Ptr;

	do
	{
		Ptr2 = strchr(Ptr1,Trenn);

		if (Ptr2)
			*Ptr2++ = '\0';

		RetCode[Cnt] = (char*) calloc(strlen(Ptr1)+1,sizeof(char));

		if (RetCode == NULL)
			return RetCode;

		strcpy(RetCode[Cnt++],Ptr1);
		Ptr1 = Ptr2;
	}
	while (Ptr1 != NULL);

	free(Ptr);
	return RetCode;
}

/****************************************************************************/

void del_Array(char **Ptr)
{
	int Cnt = 0;

	if (Ptr)
	{
		while (Ptr[Cnt])
			free(Ptr[Cnt++]);

		free (Ptr);
	}
}

/****************************************************************************/

char *FGets(char *String, int StringLen, FILE *fp, int *Line)
{
	char *RetCode = NULL;
	char *Ptr = NULL;
	char *Help = NULL;
	int Len;

	*String ='\0';

	if ((Help = (char*) calloc(StringLen,sizeof(char))) == NULL)
		return NULL;

	while ((RetCode = fgets(Help, StringLen - strlen(String), fp)) != NULL)
	{
		(*Line)++;

		if ((Len = strlen(Help)) > 0 && Help[Len-1]  == '\n' )
			Help[Len-1] = '\0';

		if ((Ptr = Check_Quote(Help,S_COMMENT,QUOTE_DELETE)) != NULL)
			*Ptr = '\0';

		strcat(String,Help);

		if ((Len = strlen(String)) > 0 && String[Len-1]  == '\\' )
			String[Len-1] = '\0';
		else
			break;
	}

	Ptr = String;
	while (*Ptr == ' ' || *Ptr == '\t')
		Ptr++;

	strcpy(Help,Ptr);
	strcpy(String,Help);

	free(Help);
	return RetCode;
}

/****************************************************************************/

char *Check_Quote(char *String, char *Quote, int Flag)
{
  char *Ptr = String;
  char *Ptr2;

  if (Ptr == NULL)
    return NULL;

  while ((Ptr = strpbrk(Ptr,Quote)) != NULL)
  {
  	if (Ptr != String && *(Ptr - 1) == '\\')
  	{
      if (Flag == QUOTE_IGNORE)
      {
      	Ptr++;
      }
      else
      if (Flag == QUOTE_DELETE)
      {
      	Ptr2 = Ptr - 1;

        do
          *Ptr2 = *(Ptr2 + 1);
        while(*Ptr2++ != '\0');
      }
      else
        return NULL;
    }
    else
    	break;
  }

  return Ptr;
}

/****************************************************************************/

