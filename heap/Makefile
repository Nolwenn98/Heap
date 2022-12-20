CC ?= gcc
CFLAGS += -Wall --pedantic -Werror -Wno-error=unused-but-set-variable

all:
	$(CC) $(LDFLAGS) -o tas $(CFLAGS) tas_functions.c main.c -lm 

test:
	$(CC) $(LDFLAGS)  -o tests_tas $(CFLAGS) test_tas.c tas_functions.c -lcunit

clean:
	rm -f *.o tests_tas tas


