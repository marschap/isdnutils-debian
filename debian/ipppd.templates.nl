Template: ipppd/makedev
Type: select
Choices: yes, no
Default: yes
Description: May I create ISDN device entries?
 Everybody should basically choose "yes" here.
 .
 Your system may be missing ISDN-specific entries in /dev .
 Before ipppd can be of any use to you, these entries must be made.
 Select "no" if you don't want this, e.g. because you want to do it yourself
 by hand. Note that the answer here is used for all ISDN-related packages that
 may need to create devices. Debian policy demands that this question be asked.
Choices-nl: ja, nee
Description-nl: Mogen de ISDN device files aangemaakt worden?
 Iedereen moet hier in principe "ja" kiezen.
 .
 Wellicht ontbreken ISDN-specifieke files in /dev .
 Voordat ipppd gebruikt kan worden, moeten deze files aangemaakt worden.
 Kies "nee" indien u dit niet wilt, b.v. omdat u dit soort dingen zelf
 handmatig wilt doen. Het antwoord hier kan ook gebruikt worden in andere
 ISDN-gerelateerde packages die ook device files in /dev nodig hebben.
 Deze vraag wordt gesteld vanwege eisen in Debian Policy.

Template: ipppd/ispwontdoit
Type: note
Description: ISP dialup config exists already
 The files device.${IPPP0} and ipppd.${IPPP0} already exist. Therefore the ipppd
 configuration phase won't touch anything there, as it looks like it's
 already been configured.
 .
 If it doesn't work yet, and you want to try the automatic configuration,
 stop all ISDN processes (use "/etc/init.d/isdnutils stop"), remove the
 files mentioned above, and rerun the configuration with
 "dpkg-reconfigure ipppd".  After that, restart the ISDN processes:
 "/etc/init.d/isdnutils start".
Description-nl: ISP configuratie bestaat al
 De bestanden device.${IPPP0} en ipppd.${IPPP0} bestaan al. Hierdoor zal de ipppd
 configuratie niets daaraan veranderen, aangezien het blijkbaar al in orde is.
 .
 Als het echter nog niet werkt, en u wilt de automatische configuratie
 proberen, stop dan alle ISDN processen (met "/etc/init.d/isdnutils stop"),
 verwijder de bovengenoemde bestanden, en voer de configuratie opnieuw uit
 met "dpkg-reconfigure ipppd".  Daarna de ISDN processen opnieuw opstarten
 met "/etc/init.d/isdnutils start".

Template: ipppd/whichif
Type: string
Default: ippp0
Description: Which interface should be configured?
 Which interface should debconf configure?
 .
 Most people will say "ippp0" here, as that is the default for a connection
 to an ISP. The default gateway will be ippp0.
 .
 However, some people have special requirements, and they may choose another
 interface that debconf will configure (e.g. because they don't want this
 connection to be the default gateway).
 .
 Note that debconf will currently only configure at most one interface.
 Enter "none" if you do not want debconf to configure anything for ipppd.
Description-nl: Welk interface moet geconfigureerd worden?
 Welk interface moet debconf configureren?
 .
 De meeste mensen zullen hier "ippp0" zeggen, aangezien dat de default is
 voor een verbinding met het internet. De default gateway zal ippp0 zijn.
 .
 Echter, sommige mensen hebben bijzondere situaties, en die kunnen een
 ander interface opgeven dat door debconf geconfigureerd moet worden
 (bijvoorbeeld omdat ze deze verbinding niet als default gateway willen
 hebben).
 .
 Let wel, debconf zal ten hoogste één interface configureren.
 Geef "none" indien u niet wilt dat debconf iets voor ipppd configureert.

Template: ipppd/wrongif
Type: note
Description: wrong interface name
 You can only give names starting with "ippp" followed by a number between
 0 and 63.
Description-nl: verkeerde interface naam
 U kunt alleen namen geven die beginnen met "ippp" gevolgd door een getal
 tussen 0 en 63.

Template: ipppd/ispphone
Type: string
Default: manual
Description: What is your ISP's phone number?
 What number must be dialed for connecting to the internet?
 .
 This is the number complete with areacode, any necessary leading zeroes
 etc. that may be needed to get an outside line.
 .
 It is possible to enter more than one number here if there are alternatives
 available, simply enter the numbers with a space in between.
 .
 Leave this blank (or "manual") if you want to manually configure the
 ISP connection.
Description-nl: Wat is het telefoonnummer van uw ISP?
 Welk nummer moet gebeld worden voor een verbinding met het internet?
 .
 Dit is het nummer compleet met netnummer en mogelijke extra voorloop nullen
 o.i.d. voor een buitenlijn vanwege een huiscentrale.
 .
 Laat dit veld leeg (of "manual") indien u de configuratie met de hand
 wilt maken.

Template: ipppd/eaz
Type: string
Default: 0
Description: What is your local MSN?
 When making a call with ISDN, the MSN (phone number) that is originating the
 call must be given in the call-setup message. While usually a wrong MSN will
 be replaced by the main MSN for the ISDN line, especially on PABXes a wrong
 MSN (often the extension number in this case) will cause the call-setup to
 fail. So, it is best to enter the correct local MSN here.
 .
 This may also be necessary if you want the costs to be registered to one
 particular MSN, in case you have more than one MSN and this is supported by
 your telco.
Description-nl: Wat is uw eigen MSN?
 Wanneer een ISDN verbinding gemaakt wordt, dan moet het eigen MSN
 (telefoonnummer) waar de verbinding vandaan komt meegegeven worden door het
 systeem. Normaliter zal een foutief MSN vervangen worden door het juiste door
 de centrale, maar met name bij huiscentrales kan het nodig zijn om een juiste
 MSN (meestal het toestelnummer) in te stellen, omdat anders de huiscentrale
 er niet mee overweg kan. In zulke gevallen geeft u hier het eigen MSN in.
 .
 Dit is ook nodig als u de kosten op een bepaalde MSN wilt laten vastleggen
 i.p.v. op het hoofdnummer (dit geldt tenminste voor KPN lijnen in Nederland).
 Geef (tenminste in Nederland) het nummer inclusief netnummer maar zonder de
 eerste nul.

Template: ipppd/isplogin
Type: string
Description: What is the user name for logging into your ISP?
 Most (all?) ISPs need a user name and password for authenticating you as a
 valid user before allowing access. Enter the user name here. The password
 will be asked for next.
Description-nl: Wat is de usernaam bij uw ISP?
 De meeste (alle) ISP's hebben een usernaam en wachtwoord nodig voordat u
 toegelaten wordt. Geef hier de usernaam; het wachtwoord wordt hierna
 gevraagd.

Template: ipppd/isppasswd
Type: password
Description: What is the password for logging into your ISP?
 The password you give here will be entered into /etc/ppp/pap-secrets
 and /etc/ppp/chap-secrets together with the username.
Description-nl: Wat is het wachtwoord bij uw ISP?
 Dit wachtwoord zal in /etc/ppp/pap-secrets en /etc/ppp/chap-secrets
 samen met de usernaam toegevoegd worden.

Template: ipppd/isploginpapalreadythere
Type: note
Description: ISP user name already in pap-secrets
 The user name you entered for logging into your ISP is already listed in
 the /etc/ppp/pap-secrets file. That's not a problem; the existing entry
 will be commented out, and a new entry with the data you just entered will
 be inserted.
Description-nl: ISP usernaam bestaat al in pap-secrets
 De usernaam die u ingegeven heeft voor toegang tot uw ISP wordt al
 genoemd in het /etc/ppp/pap-secrets bestand. Dat is op zich geen probleem;
 de bestaande regel wordt uitgeschakeld en een nieuwe met de zojuist
 ingegeven data wordt toegevoegd.

Template: ipppd/isploginchapalreadythere
Type: note
Description: ISP user name already in chap-secrets
 The user name you entered for logging into your ISP is already listed in
 the /etc/ppp/chap-secrets file. That's not a problem; the existing entry
 will be commented out, and a new entry with the data you just entered will
 be inserted.
Description-nl: ISP usernaam bestaat al in chap-secrets
 De usernaam die u ingegeven heeft voor toegang tot uw ISP wordt al
 genoemd in het /etc/ppp/chap-secrets bestand. Dat is op zich geen probleem;
 de bestaande regel wordt uitgeschakeld en een nieuwe met de zojuist
 ingegeven data wordt toegevoegd.

Template: ipppd/isploginpapchapalreadythere
Type: note
Description: ISP user name already in chap-secrets and pap-secrets
 The user name you entered for logging into your ISP is already listed in
 the chap-secrets and pap-secrets files in /etc/ppp/. That's not a problem;
 the existing entries will be commented out, and new entries with the data you
 just entered will be inserted.
Description-nl: ISP usernaam bestaat al in chap-secrets en pap-secrets
 De usernaam die u ingegeven heeft voor toegang tot uw ISP wordt al
 genoemd in de /etc/ppp/chap-secrets en pap-secrets bestanden.
 Dat is op zich geen probleem; de bestaande regels worden uitgeschakeld en
 nieuwe met de zojuist ingegeven data worden toegevoegd.

Template: ipppd/oldipup
Type: note
Description: /etc/ppp/ip-up.d/00-isdnutils still exists
 The /etc/ppp/ip-up.d/00-isdnutils file from the old isdnutils package still
 exists. If you changed that file at some point, you may need to redo those
 changes in the 00-ipppd file (which is the new name). After that, please
 delete the old 00-isdnutils file.
 .
 Until it is deleted, it will still be used! This may cause conflicts.
Description-nl: /etc/ppp/ip-up.d/00-isdnutils bestaat nog steeds
 Het /etc/ppp/ip-up.d/00-isdnutils bestand van de oude isdnutils package
 bestaat nog steeds. Indien u ooit dat bestand aangepast heeft, dan moet u
 wellicht die wijzigingen opnieuw toepassen op het 00-ipppd bestand (dat is
 de nieuwe naam voor dit bestand).  Daarna AUB het oude 00-isdnutils bestand
 verwijderen.
 .
 Totdat het verwijderd is, zal het nog steeds gebruikt worden! Dat kan
 mogelijk tot conflicten met de nieuwe versie leiden.

Template: ipppd/oldipdown
Type: note
Description: /etc/ppp/ip-down.d/99-isdnutils still exists
 The /etc/ppp/ip-down.d/99-isdnutils file from the old isdnutils package still
 exists. If you changed that file at some point, you may need to redo those
 changes in the 99-ipppd file (which is the new name). After that, please
 delete the old 99-isdnutils file.
 .
 Until it is deleted, it will still be used! This may cause conflicts.
Description-nl: /etc/ppp/ip-down.d/99-isdnutils bestaat nog steeds
 Het /etc/ppp/ip-down.d/99-isdnutils bestand van de oude isdnutils package
 bestaat nog steeds. Indien u ooit dat bestand aangepast heeft, dan moet u
 wellicht die wijzigingen opnieuw toepassen op het 99-ipppd bestand (dat is
 de nieuwe naam voor dit bestand).  Daarna AUB het oude 99-isdnutils bestand
 verwijderen.
 .
 Totdat het verwijderd is, zal het nog steeds gebruikt worden! Dat kan
 mogelijk tot conflicten met de nieuwe versie leiden.

Template: ipppd/oldipupdown
Type: note
Description: old /etc/ppp/ip-up.d and ip-down.d scripts still exist
 The /etc/ppp/ip-up.d/00-isdnutils and /etc/ppp/ip-down.d/99-isdnutils files
 from the old isdnutils package still exist. If you changed those files at some
 point, you may need to redo those changes in the 00-ipppd and 99-ipppd files
 (which are the new names). After that, please delete the old 00-isdnutils
 and 99-isdnutils files.
 .
 Until they are deleted, they will still be used! This may cause conflicts.
Description-nl: oude /etc/ppp/ip-up.d en /etc/ppp/ip-down.d scripts bestaan nog steeds
 De /etc/ppp/ip-up.d/00-isdnutils en /etc/ppp/ip-down.d/99-isdnutils bestanden
 van de oude isdnutils package bestaat nog steeds. Indien u ooit die bestanden
 aangepast heeft, dan moet u wellicht die wijzigingen opnieuw toepassen op de
 00-ipppd en/of 99-ipppd bestanden (dat zijn de nieuwe namen voor deze
 bestanden).  Daarna AUB de oude 00-isdnutils en 99-isdnutils bestanden
 verwijderen.
 .
 Totdat ze verwijderd zijn, zullen ze nog steeds gebruikt worden! Dat kan
 mogelijk tot conflicten met de nieuwe versie leiden.

Template: ipppd/noisdnutilsinit
Type: note
Description: (re)start ipppd by hand
 There is no /etc/init.d/isdnutils on your system;
 hence you will have to stop and start any ipppd daemons by hand.
Description-nl: (her)start ipppd handmatig
 Er is geen /etc/init.d/isdnutils op uw systeem;
 daardoor zal u handmatig de nodige ipppd daemons moeten stoppen en starten.
