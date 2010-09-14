PROGRAMS=telesctrl

all:	$(PROGRAMS)

telesctrl: telesctrl.c
	$(CC) $(PCFLAGS) -o telesctrl telesctrl.c

install: all
	install -o root -g bin -m 0750 telesctrl /sbin

clean:
	rm -f *.[iso] $(PROGRAMS) *~ core
