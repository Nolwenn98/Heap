#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_functions.h"
#include "heap_ll.h"

int main()
{
    Liste *heap;

    printf("Initialisation :\n");
    heap = init_heap();
    display_heap(heap, 10);

    char word[4] = "abc";
    add_words(heap->next, word, 4, heap);

    display_heap(heap, 10);
    // char *p1, *p2, *p3, *p4, *p5;

    // p1 = (char *)heap_malloc(5);
    // p2 = (char *)heap_malloc(5);
    // p3 = (char *)heap_malloc(5);
    // p4 = (char *)heap_malloc(5);
    // p5 = (char *)heap_malloc(3);

    // strcpy(p1, "tp 1");
    // strcpy(p2, "tp 2");
    // strcpy(p3, "tp 3");
    // strcpy(p4, "tp 4");
    // strcpy(p5, "ab");

    // heap_free(p2);
    // display_heap(30);
    // heap_free(p4);
    // heap_free(p5);
    // display_heap(30);

    // p4 = (char *)heap_malloc(8);
    // printf("p4 : %d", *p4);
    // strcpy(p4, "systeme");

    // char *p1, *p2, *p3, *p4;
    // strategie = &worst_fit;

    // p1 = (char *)heap_malloc(10, strategie);
    // display_heap(30);
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

    return 0;
}