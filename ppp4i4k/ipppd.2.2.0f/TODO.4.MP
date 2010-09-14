ccp.c: all_rejected[..]
ccp.c: bisher kaum bearbeitet. Sollte abgeschaltet sein.
ipxcp.c: not tested at all

TODO: zur Umstellung auf multiple devices mit MP handling untereinander

folgende variablen muessen (zum teil) noch als
Felder ausgefuehrt werden: (rein in die link_struct)
  xmit_accm
  (kill_link, ist global)
  lcp_echos_pending
  lcp_echo_fails
  lcp_echo_number
  lcp_echo_interval
  idle_time_limit
  lcp_echo_timer_running
  hungup
  (baud_rate wird nicht benoetigt.)

[ tut eventuell, wegen Trennung von wantoptions,allowoptions
  von gotoptions und hisoptions. ]:
  um einen Link mit den richtigen Werten zu restarten sollte man 
  die beim Parameterauswerten gewonnenen Optionen sichern.

Dynamisch units vergeben. Eventuell auch erst bei
    Bedarf den benoetigten Speicher reservieren.
    (teilweise angefangen)



