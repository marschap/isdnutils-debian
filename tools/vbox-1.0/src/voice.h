/* $Id: voice.h,v 1.3 1996/06/09 13:10:38 root Exp $
 */
 
#ifndef _VBOX_VOICE_H
#define _VBOX_VOICE_H 1

#include "defines.h"

/** RMD Header (vgetty) **************************************************/

struct VoiceHeader
{
	char	Magic[4];
	char	Modem[16];
	short	Compression;
	char	Reserved[10];
};

#define VOICE_MAGIC	"RMD1"
#define VOICE_MODEM	"ZyXEL 1496"

/** Defines **************************************************************/

#define ETX  (0x03)
#define NL   (0x0A)
#define CR   (0x0D)
#define DLE  (0x10)
#define XON  (0x11)
#define XOFF (0x13)
#define DC4  (0x14)
#define CAN  (0x18)

#define ST_NO_INPUT (0x00)
#define ST_GOT_DLE  (0x01)

#define VOICE_ACTION_OK			(0)											/* Ok	*/
#define VOICE_ACTION_STOP		(1)				/* Es soll gestoppt werden	*/
#define VOICE_ACTION_HANGUP	(2)				 /* Benutzer hat aufgelegt	*/
#define VOICE_ACTION_TONES		(3)			 /* Touchtone-Kennung erkannt	*/
#define VOICE_ACTION_TIMEOUT	(4)

#define TOUCHTONE_BUFFER_LEN	(32)

/** Prototypes ***********************************************************/

extern boolean	VoiceMode(void);

#endif /* _VBOX_VOICE_H */
