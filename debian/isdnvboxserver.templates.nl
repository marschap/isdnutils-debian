Template: isdnvboxserver/msn
Type: string
Default: quit
Description: What number should vbox answer?
 Vbox must know what number to listen to. Depending on your country's ISDN
 system, this may or may not include the areacode. It is usually without
 the leading 0.
 .
 Enter `quit' (or leave it blank) if you want to configure by hand,
 not via debconf. Also enter `quit' if you already have an existing
 (working) vbox configuration from the time when this was still part
 of isdnutils (unless you want to create a new configuration).
Description-nl: Op welk nummer moet vbox luisteren?
 Vbox moet weten op welk nummer geluisterd moet worden. Afhankelijk van het
 ISDN systeem waarop u aangesloten zit, bevat dit wel of niet het netnummer.
 Het is meestal zonder de eerste nul.
 .
 In Nederland moet voor KPN aansluitingen het complete nummer maar zonder
 de nul gegeven worden (9 cijfers dus).
 .
 Geef `quit' (of laat het veld leeg) indien u met de hand wilt configureren;
 doe dit ook indien u reeds een werkend vbox configuratie heeft (tenzij u
 de configuratie over wilt doen natuurlijk).

Template: isdnvboxserver/doityourself
Type: note
Description: You've chosen not to use debconf for isdnvboxserver
 If you change your mind, run "dpkg-reconfigure isdnvboxserver".
 To finalize isdnvboxserver, you need to ensure that vboxgetty is running
 from inittab, and that vboxd will run out of inetd.
Description-nl: U heeft gekozen debconf niet te gebruiken voor isdnvboxserver
 Indien u later van gedachten verandert, dan kunt u ""dpkg-reconfigure
 isdnvboxserver" uitvoeren. Om de installatie af te maken moet vboxgetty
 vanuit inittab draaien, en vboxd moet uit inetd draaien.

Template: isdnvboxserver/rings
Type: string
Default: 4
Description: After how many rings should vbox pick up the line?
 One ring is about 5 seconds. You can fine-tune the number of rings
 for certain phonenumbers manually, e.g. any salesmen you know the
 number of (or calls without caller-ID) can be dumped into the
 answering machine after one ring.  See `man vbox.conf' for more info.
Description-nl: Hoe vaak moet de bel overgaan voordat vbox antwoordt?
 Een bel duurt ongeveer 5 seconden. U kunt verder nog afstemmen dat
 b.v. bepaalde telefoonnummers eerder of later opgenomen worden; zo
 is het heel handig om oproepen zonder nummerweergave direkt door
 vbox af te laten handelen, aangezien die meestal verkopers zijn.
 Zie `man vbox.conf' voor meer info.

Template: isdnvboxserver/attachmsg
Type: select
Choices: yes, no
Default: yes
Description: Should the message be attached to the email?
 When a message is recorded, an email notification is sent.
 If that email should contain the message as an attachment,
 choose "yes" here. Note: the attachment can be large!
Choices-nl: ja, nee
Description-nl: moet het bericht meegestuurd worden met de email?
 Wanneer een bericht opgenomen wordt, dan wordt een email verstuurd om dit
 aan te geven. Indien die email het gesproken bericht als een attachment
 meegestuurd moet worden, kies hier dan "ja". N.B.: de attachment kan
 potentieel heel groot zijn!

Template: isdnvboxserver/daemonuser
Type: string
Default: vboxdaemonuser
Description: As what user should the answering machine run?
 vboxd runs with the privileges of a normal (non-root) user.
 This is typically your non-root login name. This user must
 be a member of the `dialout' group.
Description-nl: Als welke user moet het antwoordapparaat draaien?
 vboxd draait met de privileges van een gewone (niet-root) gebruiker.
 Dit zal meestal uw gewone login naam zijn. Deze gebruiker moet een
 lid zijn van de `dialout' group.

Template: isdnvboxserver/nosuchuser
Type: text
Description: The user ${Daemonuser} doesn't exist on the system.
 Enter a valid user name here that's known to the system.
Description-nl: De gebruiker ${Daemonuser} bestaat niet op het systeem.
 Geef een geldige gebruikersnaam op die bekend is op het systeem.

Template: isdnvboxserver/user
Type: string
Description: What username may connect to the vbox server?
 To listen to messages using vbox (from the isdnvboxclient package),
 you need a username and password. Enter the username here. This does
 not have to be a user on the system itself.
Description-nl: Welke gebruikersnaam mag verbinden met de vbox server?
 Om opgenomen gesproken berichten af te kunnen luisteren met vbox
 (uit de isdnvboxclient package) moet een gebruikersnaam en wachtwoord
 gebruikt worden. Geef de gebruikersnaam hier op. Deze hoeft geen
 gebruiker op het systeem zelf te zijn.

Template: isdnvboxserver/password
Type: password
Description: Enter the password for ${User} here.
 Please don't use ':' in it! The way it is stored cannot handle this.
Description-nl: Geef de wachtwoord for ${User} here.
 AUB geen ':' in het wachtwoord gebruiken! De manier waarop het opgeslagen
 wordt kan daar niet tegen.

Template: isdnvboxserver/spoolrename
Type: note
Description: spool directory renamed
 The spool directory `${OLDSPOOL}' has been renamed to `${NEWSPOOL}' for
 consistency's sake. This should have no other consequences.
Description-nl: spool directory onbenoemd
 De spool directory `${OLDSPOOL}' is ombenoemd tot `${NEWSPOOL}' om
 het enigzins standaard te hebben. Dit heeft als het goed is geen verdere
 gevolgen.

Template: isdnvboxserver/dup3spool
Type: note
Description: two obsolete spool directories detected
 The directories `${OLDSPOOL1}' and `${OLDSPOOL2}'
 (which aren't used anymore in the standard configuration), and the
 new spool directory `${NEWSPOOL}' all exist. The old
 directories will no longer be used; please manually move any messages from
 there to the new directory (if any), and delete the old directories.
Description-nl: twee overbodige spool directories gedetecteerd
 De directories `${OLDSPOOL1}' en `${OLDSPOOL2}'
 (welke niet meer gebruikt worden in de standaard configuratie) en de
 nieuwe spool directory `${NEWSPOOL}' bestaan allemaal. De oude
 directories zullen niet langer gebruikt worden; AUB berichten daarvandaan
 naar de nieuwe verplaatsen (indien van toepassing) en dan de oude
 directories verwijderen.

Template: isdnvboxserver/dupspool
Type: note
Description: two spool directories detected
 The old spool directory `${OLDSPOOL}' and the new spool
 directory `${NEWSPOOL}' both exist. The old directory
 will no longer be used; please manually move any messages from there to the
 new directory (if any), and delete the old directory.
Description-nl: twee spool directories gedetecteerd
 De oude spool directory `${OLDSPOOL}'
 en de nieuwe spool directory `${NEWSPOOL}' bestaan beide. De oude
 directory zal niet langer gebruikt worden; AUB berichten daarvandaan
 naar de nieuwe verplaatsen (indien van toepassing) en dan de oude
 directory verwijderen.

Template: isdnvboxserver/spoolcreate
Type: note
Description: spool directory created
 The spool directory `${NEWSPOOL}' has been created,
 which is necessary for proper working of vbox.
Description-nl: spool directory aangemaakt
 De spool directory `${NEWSPOOL}' is aangemaakt.
 Dit is nodig voor een correcte werking van vbox.

Template: isdnvboxserver/vboxmoved
Type: note
Description: vbox config file moved
 The configuration file `${OLD}' has been moved to
 `${DIR}/.vbox.conf'.  See `man vbox.conf' for information
 on changing this file.
Description-nl: vbox config bestand verplaatst
 De configuratie bestand `${OLD}' is verplaatst naar
 `${DIR}/.vbox.conf'. Zie `man vbox.conf' voor informatie
 over het aanpassen van dit bestand.

Template: isdnvboxserver/vboxcreate
Type: note
Description: vbox config file created
 The configuration file `${DIR}/.vbox.conf' has been created.
 See `man vbox.conf' for information on changing this file.
Description-nl: vbox config bestand aangemaakt
 Het configuratie bestand `${DIR}/.vbox.conf' is aangemaakt.
 Zie `man vbox.conf' voor informatie over het aanpassen van dit bestand.

Template: isdnvboxserver/vboxnodir
Type: note
Description: No home directory!
 The home directory `${DIR}' for user `${USER}' doesn't exist.
 This means that the file `${DIR}/.vbox.conf' cannot be created.
Description-nl: Geen home directory!
 De home directory `${DIR}' voor gebruiker `${USER}'
 bestaat niet. Dit betekent dat het bestand `${DIR}/.vbox.conf'
 niet aangemaakt kon worden.

Template: isdnvboxserver/vboxnouser
Type: note
Description: User doesn't exist!
 The user `${USER}' doesn't exist on the system!
 Please rerun the configuration with `dpkg-reconfigure isdnvboxserver'
 to enter another username, or after creating the user.
Description-nl: Gebruiker bestaat niet!
 De gebruiker `${USER}' bestaat niet op het systeem!
 AUB de configuratie opnieuw uitvoeren d.m.v. `dpkg-reconfigure
 isdnvboxserver' om een andere gebruikersnaam in te geven, óf nadat
 de gebruiker op het systeem toegevoegd is.

Template: isdnvboxserver/doinit
Type: select
Choices: yes, no
Default: yes
Description: Should vboxgetty be enabled?
 vboxgetty is in /etc/inittab, but not yet enabled.
 Answering `yes' here will enable it once this package is fully configured.
 Choose `no' if you want to tweak it manually.
Choices-nl: ja, nee
Description-nl: Moet vboxgetty geactiveerd worden?
 vboxgetty is ingevoerd in /etc/inittab, maar nog niet actief.
 Kies `ja' om het te activeren zodra deze package volledige geconfigureerd
 is. Kies `nee' indien u het met de hand wilt bijstellen.

Template: isdnvboxserver/adduserdone
Type: note
Description: user `vbox' created
 A user `vbox' was created, in group `dialout'. This is used as the ID
 that the vbox daemons run under.
Description-nl: gebruiker `vbox' aangemaakt
 Een gebruiker `vbox' is aangemaakt, in de group `dialout'. Deze wordt
 gebruikt als de ID waaronder de vbox daemons draaien.

Template: isdnvboxserver/adduserfailed
Type: note
Description: Failed to create user `vbox'
 For optimal operation of vbox, a user `vbox' should be created,
 in group `dialout'.
 However, creating that user failed with the following error(s):
 .
 ${MSG}
 .
 Please remedy this problem by hand, and then rerun the configuration with
 `dpkg-reconfigure isdnvboxserver'.
Description-nl: kon gebruiker `vbox' niet aanmaken op het systeem
 Voor optimaal gebruik van vbox moet een gebruiker `vbox' aangemaakt worden
 in group `dialout'.
 Echter, het aanmaken daarvan mislukte met de volgende melding(en):
 .
 ${MSG}
 .
 AUB het probleem handmatig oplossen en daarna de configuratie opnieuw
 uitvoeren met `dpkg-reconfigure isdnvboxserver'.

Template: isdnvboxserver/dummy
Type: note
Description: isdnvboxserver is being configured.
 Note to translators: no need to translate this, not shown to users.
