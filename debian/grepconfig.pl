#!/usr/bin/perl

$section = 'VARIABLES';

if ($#ARGV == 1) {
	$section = $ARGV[0];
	$findvar = $ARGV[1];
}
else {
	$findvar = $ARGV[0];
}

open(IN, "/etc/isdn/isdn.conf") || die "Cannot open /etc/isdn/isdn.conf";

while (<IN>) {
	last if /^\s*\[$section\]/i;
}

while (<IN>) {
	last if /^\s*\[/;
	next if /^\s+#/;
	s/^([^=]+?)\s*=\s*(.*)\s*(#?)/$1=$2/;
	next if /^$/;
	$var = $1;
	$val = $2;
	if ($var eq $findvar) {
		print "$val\n";
		exit 0;
	}
}

exit 1;
