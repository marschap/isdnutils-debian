Template: isdnutils/no_ipppd_selected
Type: note
Description: ISDNUTILS is now split up
 The old isdnutils package is now split up into smaller parts:
 isdnutils ipppd isdnutils-xtools isdnutils-doc isdnvboxserver isdnvboxclient
 isdnlog isdnlog-data libcapi20 isdnactivecards pppdcapiplugin isdneurofile
 .
 This allows you to better select what parts you want installed, almost no
 one needs everything. Unfortunately there's no way of automatically
 selecting those parts you need.
 .
 You have ipppX interfaces configured, but you have not selected  ipppd
 to be installed. This can be dangerous (leaving you with no way of
 obtaining ipppd), so aborting the installation of this new isdnutils.
 .
 If you really mean to not have ipppd, please remove the ipppd.* files
 from /etc/isdn/, stop any ipppX interfaces, and reinstall isdnutils.
 .
 If you used dselect, please select (+) ipppd, and any of the other
 parts you might need (e.g. isdnlog, isdnvbox, isdnutils-xtools),
 although those should have been selected automatically.
 If you used apt-get, use 'apt-get install isdnutils ipppd isdnlog ...';
 you may need to run 'apt-get -f install' first to configure any other
 packages that were unpacked but not yet configured.
Description-nl: ISDNUTILS is opgesplitst
 De oude isdnutils package is in kleinere onderdelen opgesplitst:
 isdnutils ipppd isdnutils-xtools isdnutils-doc isdnvboxserver isdnvboxclient
 isdnlog isdnlog-data libcapi20 isdnactivecards pppdcapiplugin isdneurofile
 .
 Deze opsplitsing maakt het mogelijk beter te selecteren wat u geinstalleerd
 wilt hebben; bijna niemand heeft alles nodig. Helaas is het echter niet
 mogelijk om automatisch te bepalen welke onderdelen nodig zijn.
 .
 U heeft ipppX interfaces geconfigureerd, maar ipppd is niet gekozen om
 geinstalleerd te worden. Dit kan gevaarlijk zijn (u heeft na de upgrade
 geen mogelijkheid meer om op het internet te komen), dus wordt nu de
 installatie van deze nieuwe isdnutils afgebroken.
 .
 Indien u ipppd echt niet nodig heeft, verwijder dan AUB eerst de ipppd.*
 bestanden in de directory /etc/isdn, stop alle ipppX interfaces, en
 installeer opnieuw isdnutils.
 .
 Indien u dselect gebruikt, kies (+) dan ipppd en alle andere nodige
 packages (b.v isdnlog, isdnvbox, isdnutils-xtools), alhoewel dit
 automatisch gebeurd zou moeten zijn. Indien u apt-get gebruikt, voer dan
 'apt-get install isdnutils ipppd isdnlog ...' uit; wellicht moet u eerst
 'apt-get -f install' doen om reeds uitgepakte maar nog niet geconfigureerde
 packages nu te configureren.

Template: isdnutils/firmware
Type: select
Choices: none, ISAR.BIN
Choices-nl: geen, ISAR.BIN
Default: none
Description: Does firmware need to be loaded?
 Some ISDN cards can't function properly until firmware has been downloaded.
 Notable example is the Sedlbauer SpeedFax+ PCI or Siemens I-Surf, which
 need ISAR.BIN to be loaded. (I don't know of any others at this time.)
 Choose "ISAR.BIN" if this is necessary.
 If you don't know what I'm talking about, choose "none".
Description-nl: Moet firmware geladen worden?
 Sommige ISDN kaarten werken pas wanneer firmware in die kaarten geladen is.
 De Sedlbauer SpeedFax+ PCI of de Siemens I-Surf zijn voorbeelden hiervan.
 Deze kaarten hebben ISAR.BIN nodig (andere kaarten zijn momenteel nog niet
 bekend). Kies "ISAR.BIN" als dit moet gebeuren. Als u geen idee heeft waar
 dit over gaat, dan kunt u waarschijnlijk veilig "geen" kiezen.

Template: isdnutils/firmwarecards
Type: string
Description: Which ISDN card numbers have to be loaded with the firmware?
 If you have more than one card, enter the number of the card(s) that
 have to be loaded with the firmware, starting with 1, separated by commas.
 If you have only one card, the obvious answer here is "1".
 If you have two cards which both have to be loaded with firmware, enter "1,2".
Description-nl: Welke ISDN kaarten moeten geladen worden met de firmware?
 Indien u meer dan een ISDN kaart heeft waarvoor dit geldt, geef dan hier
 de nummers, beginnend met 1, gescheiden door komma's, waarop firmware
 geladen moet worden. Indien u maar één kaart heeft, geef dan "1". Indien u
 twee kaarten hebt waarop firmware geladen moet worden, geef dan "1,2".

Template: isdnutils/makedev
Type: select
Choices: yes, no
Choices-nl: ja, nee
Default: yes
Description: May I create ISDN device entries?
 Everybody should basically choose "yes" here.
 .
 Your system may be missing ISDN-specific entries in /dev .
 Before isdnutils can be of any use to you, these entries must be made.
 Select "no" if you don't want this, e.g. because you want to do it yourself
 by hand. Note that the answer here is used for all ISDN-related packages that
 may need to create devices. Debian policy demands that this question be asked.
Description-nl: Mogen de ISDN device files aangemaakt worden?
 Iedereen moet hier in principe "ja" kiezen.
 .
 Wellicht ontbreken ISDN-specifieke files in /dev .
 Voordat isdnutils gebruikt kan worden, moeten deze files aangemaakt worden.
 Kies "nee" indien u dit niet wilt, b.v. omdat u dit soort dingen zelf
 handmatig wilt doen. Het antwoord hier kan ook gebruikt worden in andere
 ISDN-gerelateerde packages die ook device files in /dev nodig hebben.
 Deze vraag wordt gesteld vanwege eisen in Debian Policy.

Template: isdnutils/isdnvbox-cronweekly
Type: note
Description: You have an (old) isdnvbox cron.weekly script
 In early versions of isdnutils 3.1pre1b, when upgrading from 3.0 the
 /etc/cron.weekly/isdnutils script was renamed to isdnvbox. Unfortunately,
 when isdnvbox was split up into isdnvboxserver and isdnvboxclient, the
 existing cron script was not renamed. This can lead to two scripts existing.
 .
 Normally, at this point the old script could just be deleted, but it looks
 like you have edited the script manually. Please look at
 /etc/cron.weekly/isdnvbox to see if any changes need to be preserved (e.g. in
 the /etc/cron.weekly/isdnvboxserver script), and then delete the old script
 by hand.
 .
 If you can't see anything that needs preserving, it is probably safe to delete
 it.
Description-nl: U heeft een (oude) isdnvbox cron.weekly script
 In de eerste versies van isdnutils 3.1pre1b, toen de upgrade van 3.0 naar
 3.1pre1b plaatsvond, werd de /etc/cron.weekly/isdnutils script naar isdnvbox
 omgenoemd. Toen daarna isdnvbox opgesplitst werd in isdnvboxserver en
 isdnvboxclient, werd helaas de bestaande script niet omgenoemd, waardoor er
 twee scripts zijn.
 .
 Normaal gesproken zou nu de oude script eenvoudig verwijderd kunnen worden,
 maar zo te zien is de script ooit met de hand aangepast. AUB de script
 /etc/cron.weekly/isdnvbox controleren of er wijzigingen zijn die bewaard
 moeten worden (die kunnen dan b.v. in de /etc/cron.weekly/isdnvboxserver
 script gezet worden), en verwijder daarna handmatig de oude script.
 .
 Als u niets kan vinden dat bewaard moet worden, dan is het
 hoogstwaarschijnlijk veilig om de script te verwijderen.

Template: isdnutils/isdnutils-cronweekly
Type: note
Description: /etc/cron.weekly/isdnutils renamed to isdnvboxserver
 The existing /etc/cron.weekly/isdnutils script has been renamed to
 isdnvboxserver to fit the names of the splitted packages (in the old
 version, the script only handled isdnvbox things).
 .
 When the packaging system asks whether to replace the
 /etc/cron.weekly/isdnvboxserver, it is recommended to do so. If you
 have made any changes to the script in the past, please put those changes
 into the new script.
Description-nl: /etc/cron.weekly/isdnutils ombenoemd naar isdnvboxserver
 De bestaande /etc/cron.weekly/isdnutils script is ombenoemd naar
 isdnvboxserver wat overeenkomt met de namen van de opgesplitste packages
 (in de oude versie, zorgde de script alleen voor isdnvbox zaken).
 .
 Wanneer het packaging systeem vraagt of /etc/cron.weekly/isdnvboxserver
 vervangen mag worden, dan is het aanbevolen om dat inderdaad te laten doen.
 Indien u ooit iets aangepast heeft in die script, dan moet u die wijzigingen
 in de nieuwe script zetten.
