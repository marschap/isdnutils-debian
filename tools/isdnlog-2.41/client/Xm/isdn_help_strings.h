/*
** X11/Motif interface for ISDN accounting for isdn4linux.
**
** Copyright 1996 by Oleg von Styp Rekowski (oleg.styp@t-online.de)
**
** This revision is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2, or (at your option)
** any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Revision: 0.01       Date: 10.02.96          Author: os
*/

static char *isdn_help_strings[] = {

/* Help */
/* 0 */ "xisdn Version",
"@f[BIG]ISDN X11-Client\n\n@f[I]\
Copyright \251 1996 by Oleg von Styp Rekowski.\n\n\n@f[BIG]\
ISDNLOG Version "VERSION"\n\n@f[B]\
Copyright \251 1995, 1996 by:\n\n@f[I]\
Wim Bonis (bonis@@kiss.de) -- 1TR6\n\
Michael 'Ghandi' Herold (michael@@abadonna.franken.de) -- Sounderweiterung\n\
Andreas Kool (akool@@Kool.f.EUnet.de) (The Boss) -- isdnlog, isdnrep\n\
Stefan L�thje (luethje@@konstanz.netsurf.de) -- CS-Architektur, isdnrep\n\
Bernhard Kruepl (b.kruepl@@ieee.org) -- Geb�hrenauswertung\n\
Frank Strauss (strauss@@escape.de) -- Durchsatz-Ermittlung\n\
Oleg von Styp Rekowski (os@@Kool.f.EUnet.de) -- X11-Client\n\
Gernot Zander (hifi@@scorpio.in-berlin.de) -- isdnlog\n\
unz�hlige Mitglieder der isdn4linux-Mailing-Liste als\n\
Beta-Tester/Ideen-Lieferanten",

/* 1 */ "xisdn Online-Hilfe",
"Die Oberfl�che ist in 3 Bereiche unterteilt: Ein Men�bar mit Funktionen und Hilfe,\n\
und zwei scrollbaren Listen: die obere mit aktuell bestehenden und die untere\n\
mit bereits abgeschlossenen (bzw. abgebrochenen) Verbindungen. Die Zeilen in\n\
der letzten Liste k�nnen ausgew�hlt und dann mit 'Funktionen->L�schen' entfernt\n\
werden. Die Spalten tragen die folgenden Informationen (von links her): Monat,\n\
Tag, Verbindungsanfang, Dauer der Verbindung, Geb�hren, W�hrung, MSN, Richtung,\n\
Gegenstelle, deren Ort, Status. Mit 'Funktionen->Protokoll' k�nnen alle Meldungen\n\
vom Server im separaten Dialog ausgegeben werden."
/* 2 */ "", "",
/* 3 */ "", "",
/* 4 */ "", "",

/* Error */
/* 1 */ "Fehlernummer: MSG_ANN_REJ\nNicht autorisierter Zugriff auf Server.",
/* 2 */ "Fehlernummer: WF_ACC, Messagetyp = %d: Unbekannter Messagetyp.",
/* 3 */ "Fehler: Verbindung konnte nicht initialisiert werden.\n",
/* 4 */ "Nichts zu speichern.",
/* 5 */ "Datei\n%s\nkonnte nicht ge�ffnet werden!.",
/* 6 */ "Interner Fehler: MSG_CHANGE_CHAN\nsscanf liefert falsches Ergebnis!.",
/* 7 */ "Fehlernummer: WF_NOTHING, Messagetyp = %d: Unbekannter Messagetyp.",
};
