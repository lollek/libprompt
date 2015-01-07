### Configurables:
# Prefix where to place library and header.
PREFIX=/usr/local

# How many characters there can be in the buffer. After BUFSIZE, you will only
# hear a bell when trying to type something
BUFSIZE=4096

# How many lines that will be saved in history. After HISTSIZEMAX lines, the
# oldest will be removed to make room for the newest
HISTSIZEMAX=100

# How many lines that will be saved in kill-ring. After KILLSIZEMAX lines, the
# oldest will be removed to make room for the newest
KILLSIZEMAX=10
###

LIBNAME=libprompt.so
CFLAGS=-Wall -Wextra -Werror -pedantic -O3 -fpic
CFLAGS+= -DBUFSIZE=$(BUFSIZE)
CFLAGS+= -DHISTSIZEMAX=$(HISTSIZEMAX)
CFLAGS+= -DKILLSIZEMAX=$(KILLSIZEMAX)
LDFLAGS=

LINT=cppcheck
LINTFLAGS=--enable=all

HEADERS=$(wildcard src/*.h src/**/*.h)
SRCFILES=$(wildcard src/*.c src/**/*.c)
OBJFILES=$(addsuffix .o,$(basename $(SRCFILES)))

$(LIBNAME):	$(OBJFILES)
	$(CC) $(LDFLAGS) -shared -o $@ $^

install: $(LIBNAME)
	cp $(LIBNAME) $(PREFIX)/lib
	chmod 0755 $(PREFIX)/lib/$(LIBNAME)
	cp src/prompt.h $(PREFIX)/include
	@echo
	@echo "Run 'make testfile' if you want to test-link the library"
	@echo "Note that you might need to run ldconfig first"

testfile: LDFLAGS+= -lprompt
testfile:	$(LIBNAME) test/main.o
	$(CC) $(LDFLAGS) -o $@ $^

debug: CFLAGS+= -g -DDEBUG
debug: LDFLAGS+= -L. -lprompt
debug: $(LIBNAME) test/main.o
	$(CC) $(LDFLAGS) -o debug $^
	LD_LIBRARY_PATH=. ./debug

debugmem: CFLAGS+= -g -DDEBUG -fsanitize=address -fno-omit-frame-pointer
debugmem: LDFLAGS+= -fsanitize=address
debugmem: $(OBJFILES) test/main.o
	$(CC) $(LDFLAGS) -o debug $^
	./debug

debugheap: CFLAGS+= -g -DDEBUG
debugheap: $(OBJFILES) test/main.o
	$(CC) $(LDFLAGS) -o debug $^
	valgrind ./debug

lint:
	$(LINT) $(LINTFLAGS) $(SRCFILES) $(HEADERS)

clean:
	$(RM) $(OBJFILES) $(LIBNAME) testfile debug test/main.o

.PHONY:	clean lint testfile debug debugmem install
