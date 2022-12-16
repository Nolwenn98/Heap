#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "heap_ll.h"

static list_t libre = LIST_INITIALIZER;

int main()
{
    init_heap(&libre);
    display_heap(30, &libre);

    char *p1, *p2, *p3, *p4, *p5;
    strategie = &first_fit;

    p1 = (char *)heap_malloc(5, &libre, strategie);
    p2 = (char *)heap_malloc(5, &libre, strategie);
    p3 = (char *)heap_malloc(5, &libre, strategie);
    p4 = (char *)heap_malloc(5, &libre, strategie);
    p5 = (char *)heap_malloc(3, &libre, strategie);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    strcpy(p4, "tp 4");
    strcpy(p5, "ab");

    heap_free(p2, &libre);
    heap_free(p4, &libre);
    heap_free(p5, &libre);
    display_heap(30, &libre);

    p4 = (char *)heap_malloc(8, &libre, strategie);
    printf("p4 : %d", *p4);
    strcpy(p4, "systeme");

    // char *p1, *p2, *p3, *p4;

    // p1 = (char *)heap_malloc(10, strategie);
    display_heap(30, &libre);
    // p2 = (char *)heap_malloc(9, strategie);
    // p3 = (char *)heap_malloc(5, strategie);

    // strcpy(p1, "tp 1");
    // strcpy(p2, "tp 2");
    // strcpy(p3, "tp 3");
    // display_heap(30);

    // heap_free(p2);
    // p4 = (char *)heap_malloc(5, strategie);
    // strcpy(p4, "abc");
    // display_heap(30);

    // heap_free(p1);
    // p4 = (char *)heap_malloc(5, strategie);
    // strcpy(p4, "def");
    // display_heap(30);

    // p4 = (char *)heap_malloc(3, strategie);
    // strcpy(p4, "ab");
    // display_heap(30);

    // return 0;
}