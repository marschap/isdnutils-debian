# example of /etc/isdn/isdn.conf for the Netherlands
#
# More information: /usr/share/doc/isdnlog/*


[GLOBAL]
COUNTRYPREFIX   = +
COUNTRYCODE     = 31
AREAPREFIX      = 0

# Lokale netnummer ZONDER 0 hieronder invullen! (20 = Amsterdam)
AREACODE        = 20

[VARIABLES]

[ISDNLOG]
LOGFILE         = /var/lib/isdn/calls
ILABEL          = %b %e %T %ICall to tei %t from %N2 on %n2
OLABEL          = %b %e %T %Itei %t calling %N2 with %n2
# alternatieven voor bovenstaande twee
# ILABEL        = %a %b %e %T inkomend %I%n2 <- %N2
# OLABEL        = %a %b %e %T uitgaand %I%n2 -> %N2
REPFMTWWW       = "%X %D %17.17H %T %-17.17F %-20.20l SI: %S %9u %U %I %O"
REPFMTSHORT     = "%X%D %8.8H %T %-14.14F%U%I %O"
REPFMT          = "  %X %D %15.15H %T %-15.15F %7u %U %I %O"
CHARGEMAX       = 50.00
CURRENCY        = 0.01,EUR

COUNTRYFILE = /usr/share/isdn/country.dat
RATECONF= /etc/isdn/rate.conf
RATEFILE= /usr/share/isdn/rate-nl.dat
HOLIDAYS= /usr/share/isdn/holiday-nl.dat
ZONEFILE= /usr/share/isdn/zone-nl-%s.cdb
DESTFILE= /usr/share/isdn/dest.cdb

# providerselect
VBN = 16:17
VBNLEN = 2 
# Onderstaande regel kiest KPN. Voor andere mogelijkheden, zie o.a. rate.conf
# Dit is bijna altijd OK, behalve als je het via 0800-1273 anders geregeld hebt.
PRESELECTED=55
