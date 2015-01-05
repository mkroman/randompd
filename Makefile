ifndef PREFIX
	PREFIX=/usr
endif

CFLAGS+=-Wall -Werror
#CFLAGS+=-g
CFLAGS+=-O3
CFLAGS+=-pedantic
CFLAGS+=-std=c99

VERSION=$(shell git describe --tags --abbrev=0)
GIT_VERSION:="$(shell git describe --tags --always) ($(shell git log --pretty=format:%cd --date=short -n1))"
OS:=$(shell uname)

LIBS+=-lmpdclient

ifeq ($(OS),Linux)
CFLAGS+=-DLINUX
endif

V ?= 0
ifeq ($(V),0)
# Don't print command lines which are run
.SILENT:
endif

CFLAGS+=-DVERSION=\"${GIT_VERSION}\"

OBJS:=$(wildcard src/*.c)
OBJS:=$(OBJS:.c=.o)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC $<"

randompd: ${OBJS}
	@echo " LD $@"
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

static: ${OBJS}
	@echo " LD $@"
	$(CC) $(LDFLAGS) -static -o randompd $^ $(LIBS)

clean:
	rm -f src/*.o

distclean: clean
	rm -f randompd

install:
	install -m644 randompd $(PREFIX)/bin/randompd
