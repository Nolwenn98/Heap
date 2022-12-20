CC ?= gcc
CFLAGS += -Wall 

CFILES = heap_ll.c main_heap_ll.c
LOCAL_LIB=./ListeChainee/dll
LOCAL_INC=./ListeChainee/dll


all:
	$(CC) $(LDFLAGS) -L$(LOCAL_LIB) -I$(LOCAL_INC) -g -o heap $(CFLAGS) $(CFILES) -lm -llinked_list

test:
	$(CC) $(LDFLAGS) -L$(LOCAL_LIB) -I$(LOCAL_INC) -o tests_heap $(CFLAGS) test_heap_ll.c heap_ll.c -lcunit -llinked_list

clean:
	rm -f *.o tests_heap heap

