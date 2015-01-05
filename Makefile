### Configurables:
# How many characters there can be in the buffer. After BUFSIZE, you will only
# hear a bell when trying to type something
BUFSIZE=4096
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

debug: CFLAGS+= -g -DDEBUG
debug: $(LIBNAME) test/main.o
	$(CC) -L. -lprompt -o $@ $^
	LD_LIBRARY_PATH=. ./$@

lint:
	$(LINT) $(LINTFLAGS) $(SRCFILES) $(HEADERS)

clean:
	$(RM) $(OBJFILES) $(LIBNAME) debug test/main.o

.PHONY:	clean lint
