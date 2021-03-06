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

# Set \n to newline
define \n


endef

LIBNAME=libprompt.so
CFLAGS=-Wall -Wextra -Werror -pedantic -O3 -fpic
CFLAGS+= -DBUFSIZE=$(BUFSIZE)
CFLAGS+= -DHISTSIZEMAX=$(HISTSIZEMAX)
CFLAGS+= -DKILLSIZEMAX=$(KILLSIZEMAX)
LDFLAGS=

LINT=cppcheck
LINTFLAGS=--enable=all

HEADERS=$(wildcard src/*.h)
SRCFILES=$(wildcard src/*.c)
OBJFILES=$(addsuffix .o,$(basename $(SRCFILES)))

TESTSRC=$(wildcard tests/test_*.c)
TESTOBJ=$(addsuffix .o,$(basename $(TESTSRC)))

$(LIBNAME): $(OBJFILES)
	$(CC) $(LDFLAGS) -shared -o $@ $^

install:$(LIBNAME)
	cp $(LIBNAME) $(PREFIX)/lib/$(LIBNAME)
	chmod 0755 $(PREFIX)/lib/$(LIBNAME)
	sed 's/BUFSIZE/$(BUFSIZE)/g' src/prompt.h > $(PREFIX)/include/prompt.h
	@echo
	@echo "Run 'make testfile' if you want to test-link the library"
	@echo "Note that you might need to run ldconfig first"
.PHONY: install

testfile: LDFLAGS+= -lprompt
testfile:	$(LIBNAME) tests/main.o
	$(CC) $(LDFLAGS) -o $@ $^

test: CFLAGS += -g -fsanitize=address -fno-omit-frame-pointer
test: LDFLAGS+= -fsanitize=address
test: $(LIBNAME) $(TESTOBJ)
	$(foreach test, $(TESTOBJ),\
	  $(CC) $(LDFLAGS) -o test $(test) $(OBJFILES) && ./test${\n})
	@$(RM) test
.PHONY: test

debug: CFLAGS+= -g -DDEBUG
debug: LDFLAGS+= -L. -lprompt
debug: $(LIBNAME) tests/main.o
	$(CC) $(LDFLAGS) -o debug $^
	LD_LIBRARY_PATH=. ./debug
.PHONY: debug

debugmem: CFLAGS+= -g -DDEBUG -fsanitize=address -fno-omit-frame-pointer
debugmem: LDFLAGS+= -fsanitize=address
debugmem: $(OBJFILES) tests/main.o
	$(CC) $(LDFLAGS) -o debug $^
	./debug
.PHONY: debugmem

debugheap: CFLAGS+= -g -DDEBUG
debugheap: $(OBJFILES) tests/main.o
	$(CC) $(LDFLAGS) -o debug $^
	valgrind ./debug
.PHONY: debugheap

lint:
	$(LINT) $(LINTFLAGS) $(SRCFILES) $(HEADERS)
.PHONY: lint

clean:
	$(RM) $(OBJFILES) $(TESTOBJ) $(LIBNAME) testfile debug tests/main.o
.PHONY: clean
