/* Default Values. Change them in config.h.in and re-run configure */

/* Isdnlogs server port for x11 client communication */
#define SERV_PORT 20011

/* user config file */
#define USERFILE "isdnlog.users"

/* charge data file */
#define CHARGEFILE "charge.dat"

/* config file */
#define CONFIGFILE "isdn.conf"

/* calleridfile */
#define CALLERIDFILE "callerid.conf"

/* logfile */
#define LOGFILE "/var/log/isdn.log"

/* reload command */
#define RELOADCMD "reload"

/* stop command */
#define STOPCMD "stop"

/* reboot command */
#define REBOOTCMD "/sbin/reboot"

/* old config directory */
#define OLDCONFDIR "/etc/isdnlog"

/* old config file */
#define OLDCONFFILE "isdnlog.conf"

@TOP@

#undef ISDN_CH
#undef ISDN_NL
#undef PACKAGE
#undef POSTGRES
#undef VERSION
#define HAVE_LIBDBM
#define HAVE_LIBGDBM


