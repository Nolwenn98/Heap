CC ?= gcc
CFLAGS += -Wall --pedantic -Werror -Wno-error=unused-but-set-variable

CFILES = heap_ll.c main_heap_ll.c
LOCAL_LIB=./ListeChainee/linked_list
LOCAL_INC=./ListeChainee/linked_list


all:
	$(CC) $(LDFLAGS) -L$(LOCAL_LIB) -I$(LOCAL_INC) -o heap $(CFLAGS) $(CFILES) -lm -llinked_list

test:
	$(CC) $(LDFLAGS) -L$(LOCAL_LIB) -I$(LOCAL_INC) -o tests_heap $(CFLAGS) test_heap_ll.c heap_ll.c -lcunit -llinked_list

clean:
	rm -f *.o tests_heap heap

