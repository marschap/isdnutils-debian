AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
DISTCLEANFILES=telesctrl.8 hisaxctrl.8
sbin_PROGRAMS=telesctrl hisaxctrl
man_MANS=telesctrl.8 hisaxctrl.8
hisaxctrl_SOURCES=telesctrl.c

telesctrl.8: telesctrl.man
	cp $< $@

hisaxctrl.8: telesctrl.man
	sed \
		-e 's/teles/hisax/g' \
		-e 's/Teles/HiSax/g' \
		-e 's/TELES/HISAX/g' \
		-e 's/hisaxctrl\\c/telesctrl\\c/g' \
                < $< > $@             
