#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "heap_ll.h"

int main()
{
    t_strategie strategie;
    init_heap();
    display_heap(30);

    char *p1, *p2, *p3, *p4, *p5;
    strategie = &first_fit;

    p1 = (char *)heap_malloc(5, strategie);
    p2 = (char *)heap_malloc(5, strategie);
    p3 = (char *)heap_malloc(5, strategie);
    p4 = (char *)heap_malloc(5, strategie);
    p5 = (char *)heap_malloc(3, strategie);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    strcpy(p4, "tp 4");
    strcpy(p5, "ab");

    heap_free(p2);
    heap_free(p4);
    heap_free(p5);
    display_heap(30);

    p4 = (char *)heap_malloc(8, strategie);
    strcpy(p4, "systeme");

    display_heap(30);

    heap_free(p1);
    heap_free(p3);

    log_read("log.txt");

    // char test = "ajs";

    // log_malloc(2, &test);
}