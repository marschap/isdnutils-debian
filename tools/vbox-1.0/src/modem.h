/* $Id: modem.h,v 1.3 1996/06/09 13:10:37 root Exp $
 */

#ifndef _VBOX_MODEM_H
#define _VBOX_MODEM_H 1

#include <termio.h>
#include <sys/types.h>
#include <unistd.h>

#include "defines.h"

/** Defines **************************************************************/
              
struct ModemOneSpeed
{
	long		Speed;
	speed_t	Flag;
};

typedef struct termios TIO;

#define MODEM_FLOW_NONE			(0)
#define MODEM_FLOW_HARD			(1)
#define MODEM_FLOW_XON_IN		(2)
#define MODEM_FLOW_XON_OUT		(4)
#define MODEM_FLOW_SOFT			(MODEM_FLOW_XON_IN | MODEM_FLOW_XON_OUT)
#define MODEM_FLOW_BOTH			(MODEM_FLOW_HARD | MODEM_FLOW_SOFT )

#define MODEM_COMMAND_SUFFIX	"\r"
#define MODEM_BUFFER_LEN		(256)

/** Variables ************************************************************/

extern int	FlowControl;
extern bool	ToggleDTR;
extern long	TogglePause;
extern long	Speed;

/** Prototypes ***********************************************************/

extern boolean ModemGetTermIO(TIO *);
extern boolean ModemSetTermIO(TIO *);
extern boolean ModemToggleDTR(unsigned long);
extern void		ModemPause(unsigned long);
extern void		ModemSetSaneMode(TIO *, bool);
extern void		ModemSetSpeed(TIO *, long);
extern void		ModemSetRawMode(TIO *);
extern void		ModemSetFlowControl(TIO *, int);
extern boolean	ModemInitDevice(void);
extern boolean ModemCommand(char *, char *);
extern void		ModemFlush(int);
extern void		ModemFlushFull(void);
extern boolean	ModemOpenDevice(char *);
extern void		ModemCloseDevice(void);            
extern boolean	ModemWait(long);
extern boolean ModemGetRings(int);
extern int		ModemRawRead(char *, int);
extern size_t	ModemRawWrite(char *, int);
extern int		ModemCheckResult(char *, char *);
extern void		ModemSetTimeout(int);
extern boolean ModemGetTimeout(void);
extern boolean ModemCheckInput(void);
extern boolean ModemGetNoCarrier(void);
extern void		ModemSetNoCarrier(bool);
extern void		ModemNoCarrierTest(char);

#endif /* _VBOX_MODEM_H */
