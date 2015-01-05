### Configurables:
# How many characters there can be in the buffer. After BUFSIZE, you will only
# hear a bell when trying to type something
BUFSIZE=4096
# Prefix where to place library and header.
PREFIX=/usr/local
###

LIBNAME=libprompt.so
CFLAGS=-Wall -Wextra -Werror -pedantic -O3 -fpic
CFLAGS+= -DBUFSIZE=$(BUFSIZE)

LINT=cppcheck
LINTFLAGS=--enable=all

HEADERS=$(wildcard src/*.h src/**/*.h)
SRCFILES=$(wildcard src/*.c src/**/*.c)
OBJFILES=$(addsuffix .o,$(basename $(SRCFILES)))

$(LIBNAME):	$(OBJFILES)
	$(CC) -shared -o $@ $^

install: $(LIBNAME)
	cp $(LIBNAME) $(PREFIX)/lib
	chmod 0755 $(PREFIX)/lib/$(LIBNAME)
	cp src/prompt.h $(PREFIX)/include
	@echo
	@echo "Run 'make testfile' if you want to test-link the library"
	@echo "Note that you might need to run ldconfig first"

testfile:	$(LIBNAME) test/main.o
	$(CC) -lprompt -o $@ $^

debug: CFLAGS+= -g -DDEBUG
debug: $(LIBNAME) test/main.o
	$(CC) -L. -lprompt -o $@ $^
	LD_LIBRARY_PATH=. ./$@

lint:
	$(LINT) $(LINTFLAGS) $(SRCFILES) $(HEADERS)

clean:
	$(RM) $(OBJFILES) $(LIBNAME) testfile debug test/main.o

.PHONY:	clean lint testfile debug install
