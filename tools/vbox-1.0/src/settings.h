
/*************************************************************************
 ** Konfigurationsdatei für `vbox'. 							 (Tabulator 3)	**
 *************************************************************************/

#ifndef _VBOX_SETTINGS_H
#define _VBOX_SETTINGS_H 1

/*************************************************************************
 **																							**
 **											VBOX											**
 **																							**
 *************************************************************************/

/*************************************************************************
 ** LOGFILE																					**
 *************************************************************************
 ** Name der Logdatei in die `vbox' seine Meldungen schreibt. Der Pla-	**
 ** tzhalter %s wird durch den Namen des jeweiligen tty-Devices ersetzt	**
 ** (z.B. ttyI5).																			**
 *************************************************************************/

#define LOGFILE "/var/log/isdn/vbox.%s"

/*************************************************************************
 ** LOCKFILE																				**
 *************************************************************************
 ** Name der Datei mit der `vbox' die einzelnen Devices lockt. Der Pla-	**
 ** tzhalter %s wird durch den Namen des jeweiligen tty-Devices ersetzt	**
 ** (z.B. ttyI5).																			**
 *************************************************************************/

#define LOCKFILE "/var/lock/LCK..%s"

/*************************************************************************
 ** PIDFILE																					**
 *************************************************************************
 ** Name der Datei in der `vbox' seine Prozeß-ID vermerkt. Der Platz-	**
 ** halter %s wird durch den Namen des jeweiligen tty-Devices ersetzt	**
 ** (z.B. ttyI5).																			**
 *************************************************************************/

#define PIDFILE "/var/run/vbox-pid.%s"

/*************************************************************************
 ** VBOXRC																					**
 *************************************************************************
 ** Name der Konfiguration, die sich im Homeverzeichnis des Benutzers	**
 ** befinden kann.																		**
 *************************************************************************/

#define VBOXRC ".vboxrc"

/*************************************************************************
 ** Einstellungen für das emulierte "Modem".										**
 *************************************************************************/

#define MODEM_RING_TIMEOUT			5
#define MODEM_COMMAND_TIMEOUT		2
#define MODEM_ECHOCMD_TIMEOUT		2
#define MODEM_WAIT_TIMEOUT			1800
#define TOGGLEDTR						TRUE
#define TOGGLEDTRTIME				800

/*************************************************************************
 ** DEFAULT_RINGS																			**
 *************************************************************************
 ** Anzahl der RING's bevor `vbox' abhebt. Bei mir persönlich werden 5	**
 ** RING's gemeldet, bevor das Telefon 1x klingelt (Europa 10). Die An-	**
 ** zahl der RING's ist also *nicht* identisch mit der Anzahl der Klin-	**
 ** gelsignale vom Telefon!															**
 *************************************************************************/

#define DEFAULT_RINGS 20

/*************************************************************************
 ** DEFAULT_RECORD_TIME																	**
 *************************************************************************
 ** Zeit in Sekunden die ein Anrufer sprechen darf, der nicht in der		**
 ** Datei '.vboxrc' des Benutzers für den `vbox' läuft gefunden wurde.	**
 *************************************************************************/

#define DEFAULT_RECORD_TIME 60

/*************************************************************************
 ** VOICE_COMPRESSION_MODE																**
 *************************************************************************
 ** Kompressionsmodus für die Audio Daten. Möglich sind:						**
 **																							**
 ** 2 - Modus ADPCM-2																	**
 ** 3 - Modus ADPCM-3																	**
 ** 4 - Modus ADPCM-4																	**
 ** 5 - Modus aLAW																		**
 ** 6 - Modus uLAW																		**
 *************************************************************************/ 

#define VOICE_COMPRESSION_MODE 4

/*************************************************************************
 ** NEED_FREE_DISK_SPACE																**
 *************************************************************************
 ** Anzahl der Byte's, die auf der Partition auf der sich das Spoolver- **
 ** zeichnis befindet, frei sein müssen, damit ein Anruf entgegenge-    **
 ** nommen wird.																			**
 *************************************************************************/

#define NEED_FREE_DISK_SPACE 5000000

/*************************************************************************
 **																							**
 **      								VBOXVIEW											**
 **																							**
 *************************************************************************/

/*************************************************************************
 ** VBOXPLAYCMD																			**
 *************************************************************************
 ** Kommando zum abspielen einer Nachricht. Dem Kommando werden folgen-	**
 ** de Argumente übergeben:															**
 **																							**
 ** 1. Name der ausgewählten Datei													**
 ** 2. Eingestellte Lautstärke														**
 *************************************************************************/

#define VBOXPLAYCMD "/usr/bin/vboxplay"

/** ENDE *****************************************************************/

#endif /* _VBOX_SETTINGS_H */
