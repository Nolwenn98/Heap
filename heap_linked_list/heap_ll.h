#ifndef HEAP_LL_HEADER
#define HEAP_LL_HEADER

#include "list_functions.h"

#define SIZE_HEAP 128
#define FREE_BLOCK '\0'
#define FREE_ZONE -1

#define MEMORY_FULL -1

Liste *heap;
Liste *libre;

Liste *init_heap();
void ll_print(Liste *list);

void display_heap(Liste *heap, int size);

Liste *get_ptr_next_block(Liste *ptr_heap);
void add_words(Liste *list, char word[], int size, Liste *heap);

// Stratégies
int (*strategie)(int);
int worst_fit(int size);
Liste *first_fit(int size);
int best_fit(int size);
char *heap_malloc(unsigned int taille, int (*strategie)(int));

void heap_free(char *ptr);

// Deux zones libres à la suite
int search_two_free_zone();
void add_two_zone_free(int first_zone);

#endif