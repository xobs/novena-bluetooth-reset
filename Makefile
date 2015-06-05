CFLAGS += -Wall -g $(shell dpkg-buildflags --get CFLAGS)
LDFLAGS += -g $(shell dpkg-buildflags --get LDFLAGS)

all:
	$(CC) $(CFLAGS) $(LDFLAGS) novena-bluetooth-reset.c -o novena-bluetooth-reset

clean:
	rm -f novena-bluetooth-reset

install:
	mkdir -p $(DESTDIR)/usr/sbin
	mkdir -p $(DESTDIR)/usr/share/man/man1
	cp novena-bluetooth-reset $(DESTDIR)/usr/sbin/
	cp novena-bluetooth-reset.1 $(DESTDIR)/usr/share/man/man1
