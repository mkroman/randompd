ifndef PREFIX
	PREFIX=/usr
endif

CFLAGS+=-Wall -Werror -pedantic -g
LDFLAGS+=-lmpdclient

REVISION=$(shell git rev-list --count HEAD)
OS:=$(shell uname)

ifeq ($(OS),Linux)
CFLAGS+=-DLINUX
endif

STATIC ?= 0
ifeq ($(STATIC),1)
# Build a static binary
LDFLAGS+=-static
endif

V ?= 0
ifeq ($(V),0)
# Don't print command lines which are run
.SILENT:
endif

CFLAGS+="-DRANDOMPD_VERSION=\"mpd-r${REVISION}\""

OBJS:=$(wildcard src/*.c)
OBJS:=$(OBJS:.c=.o)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC $<"

randompd: ${OBJS}
	@echo " LD $@"
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f src/*.o

distclean: clean
	rm -f randompd

install:
	install -m755 randompd $(PREFIX)/bin/randompd
