PROGNAME=prompt
CFLAGS=-Wall -Wextra -Werror -pedantic -O3 -g
LDFLAGS=

LINT=cppcheck
LINTFLAGS=--enable=all

HEADERS=$(wildcard src/*.h src/**/*.h)
SRCFILES=$(wildcard src/*.c src/**/*.c)
OBJFILES=$(addsuffix .o,$(basename $(SRCFILES)))

$(PROGNAME):	$(OBJFILES)
	$(CC) $(LDFLAGS) -o $@ $^

lint:
	$(LINT) $(LINTFLAGS) $(SRCFILES) $(HEADERS)

clean:
	$(RM) $(PROGNAME) $(OBJFILES)

.PHONY:	clean lint
