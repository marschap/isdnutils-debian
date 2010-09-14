#include <isdn_client.h>

void isdn_list_update (int channel, CALL* info)
{
 if		(info) {
   switch	(info->stat) {
     case STATE_RING   :
     case STATE_CONNECT:
     case STATE_AOCD   :
     case STATE_CAUSE  :
     case STATE_TIME   :
     case STATE_BYTE   : isdn_current_list_item_change	(channel, info); break;
     case STATE_HANGUP : isdn_closed_list_item_add	(channel, info); break;
   }
   free ((void *) info);
 }
}
