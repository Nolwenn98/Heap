#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "heap_ll.h"

static list_t libre = LIST_INITIALIZER;
static char heap[SIZE_HEAP];

int main()
{
    init_heap(heap, &libre);
    display_heap(heap, &libre, 30);

    char *p1, *p2, *p3, *p4, *p5;
    strategie = &first_fit;

    p1 = (char *)heap_malloc(heap, &libre, 5, strategie);
    p2 = (char *)heap_malloc(heap, &libre, 5, strategie);
    p3 = (char *)heap_malloc(heap, &libre, 5, strategie);
    p4 = (char *)heap_malloc(heap, &libre, 5, strategie);
    p5 = (char *)heap_malloc(heap, &libre, 3, strategie);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    strcpy(p4, "tp 4");
    strcpy(p5, "ab");

    heap_free(heap, &libre, p2);
    heap_free(heap, &libre, p4);
    heap_free(heap, &libre, p5);
    display_heap(heap, &libre, 30);

    p4 = (char *)heap_malloc(heap, &libre, 8, strategie);
    strcpy(p4, "systeme");

    display_heap(heap, &libre, 30);
}