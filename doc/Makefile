MAN4FILES=ttyI.4
MAN8FILES=isdnctrl.8 icnctrl.8 telesctrl.8 iprofd.8 pcbitctl.8
MAN4PATH=$(MANDEST)/man4
MAN8PATH=$(MANDEST)/man8

all:

install:
	install -o root -g root -m 0644 $(MAN8FILES) $(MAN8PATH)
	install -o root -g root -m 0644 $(MAN4FILES) $(MAN4PATH)
	-@rm -f $(MAN4PATH)/cui.4*
	ln -s $(MAN4PATH)/ttyI.4 $(MAN4PATH)/cui.4

clean:
	-@rm -f *~

html:
	@for i in $(MAN4FILES) ; do groff -man -Tascii $$i | man2html > `basename $$i .4`.html ; done
	@for i in $(MAN8FILES) ; do groff -man -Tascii $$i | man2html > `basename $$i .8`.html ; done
