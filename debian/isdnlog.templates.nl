Template: isdnlog/intro
Type: text
Description: Entering isdnlog configuration
 For correct isdnlog functioning, isdnlog needs to be configured
 with a number of settings. These are some or all (depending on
 the level you have configured debconf with) of:
 .
  - country          this is used for setting a number of defaults,
                     e.g. what rate tables to use for calculating
                     the cost of a call.
 .
  - country prefix   what is used to indicate an international number
 .
  - country code     the phone number for your country
                     (e.g. 49 for Germany)
 .
  - areacode prefix  what is used to indicate an areacode,
                     if applicable in your country. Usually a 0 .
 .
  - areacode         what your local areacode is (if applicable)
 .
 After installation, you will may have to edit
 /etc/isdn/isdnlog.isdnctrl0 before isdnlog can work
 properly, although that file is set up with reasonable
 defaults for most cases.
 .
 If you previously had /etc/isdn/isdnlog.isdnctrl0, that
 will be preserved.
Description-nl: Ingave isdnlog configuratie
 Voor het juist functioneren moet isdnlog configureerd worden met een aantal
 instellingen. Dit is dan sommige of allen (afhankelijk van de debconf level)
 van de volgende:
 .
  - land             wordt gebruikt om een aantal standaard instellingen
                     te zetten, b.v. welke tabellen te gebruiken om de
                     kosten te berekenen.
 .
  - land prefix      wat gebruikt wordt om een internationaal
                     toegangsnummer aan te geven
 .
  - land code        toegangsnummer voor uw land (b.v. 31 voor Nederland)
 .
  - netnummer prefix wat gebruikt wordt om een netnummer aan te geven,
                     indien van toepassing in uw land. Meestal een 0 .
 .
  - netnummer        wat uw netnummer is (indien van toepassing).
 .
 Na de installatie kan het nog nodig zijn om /etc/isdn/isdnlog.isdnctrl0
 te bewerken voordat isdnlog correct werkt, alhoewel dat bestand zal
 voldoen voor de meeste gevallen.
 .
 Indien er voorheen een /etc/isdn/isdnlog.isdnctrl0 bestand was, dan zal
 die behouden blijven.

Template: isdnlog/country
Type: select
Choices: AT, CH, DE, ES, FR, LU, NL, NO, other
Default: DE
Description: What country is this system in?
 This is used for setting a number of defaults, e.g. what rate tables
 to use for calculating the cost of a call.
Choices-nl: AT, CH, DE, ES, FR, LU, NL, NO, anders
Default-nl: NL
Description-nl: In welk land staat dit systeem?
 Dit wordt gebruikt om een standaard instellingen te zetten,
 b.v. welke tabellen te gebruiken om de kosten te berekenen.

Template: isdnlog/country_manual
Type: string
Description: Enter the ISO two-letter code for your country
 This can't really be used for setting any defaults, but maybe some day...
 .
 Note that you will probably have to edit /etc/isdn/isdn.conf
 before isdnlog will be able to work.
Description-nl: Geef de ISO twee-letterige code voor uw land
 Dit kan nu nog niet echt gebruikt worden om standaard instellingen te
 zetten, maar wie weet ooit...
 .
 U zal hoogstwaarschijnlijk /etc/isdn/isdn.conf moeten editen voordat
 isdnlog iets nuttigs kan doen.

Template: isdnlog/countryprefix
Type: string
Default: +
Description: What is used to indicate an international number?
Description-nl: Wat wordt gebruikt om een internationaal nummer aan te geven?

Template: isdnlog/countrycode
Type: string
Description: What is the countrycode for your country?
 e.g. ${default_countrycode} for ${default_country}
Description-nl: Wat is het toegangsnummer voor uw land?
 b.v. ${default_countrycode} voor  ${default_country}

Template: isdnlog/areaprefix
Type: string
Default: 0
Description: What is used to indicate an areacode, if applicable?
Description-nl: Waarmee wordt een netnummer ingeleid (indien van toepassing)?
 Meestal, en zeker in Nederland, is dit een 0.

Template: isdnlog/areacode
Type: string
Description: What is your local areacode, if applicable?
Description-nl: Wat is uw eigen netnummer (indien van toepassing)?
