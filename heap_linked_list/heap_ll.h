#ifndef HEAP_LL_HEADER
#define HEAP_LL_HEADER

#include "list.h"

#define SIZE_HEAP 128
#define FREE_BLOCK '\0'
#define FREE_ZONE -1

#define MEMORY_FULL -1

char heap[SIZE_HEAP];

void init_heap(list_t *libre);
void display_heap(int size, list_t *libre);

// Stratégies
int8_t (*strategie)(uint8_t, list_t *);
int8_t worst_fit(uint8_t size, list_t *libre);
int8_t first_fit(uint8_t size, list_t *libre);
int8_t best_fit(uint8_t size, list_t *libre);
char *heap_malloc(uint8_t size, list_t *libre, int8_t (*strategie)(uint8_t, list_t *));

void list_sort_crescent(list_t *libre);

void heap_free(char *ptr, list_t *libre);

// Deux zones libres à la suite
void search_two_free_zone(list_t *libre);

#endif