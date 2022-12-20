#ifndef HEAP_LL_HEADER
#define HEAP_LL_HEADER

#include <stdbool.h>
#include "list.h"

#define SIZE_HEAP (128)
#define FREE_BLOCK ('\0')
#define FREE_ZONE (-1)

#define MEMORY_FULL (-1)

#define NUMBER_THREADS (5)

#define UTC_1 (1)

void init_heap(char heap[SIZE_HEAP], list_t *libre);
void display_heap(char heap[SIZE_HEAP], list_t *libre, int size);
int get_index(char heap[SIZE_HEAP], element_t *elem);

// Stratégies
int8_t (*strategie)(char *, list_t *, uint8_t);
int8_t worst_fit(char heap[SIZE_HEAP], list_t *libre, uint8_t size);
int8_t first_fit(char heap[SIZE_HEAP], list_t *libre, uint8_t size);
int8_t best_fit(char heap[SIZE_HEAP], list_t *libre, uint8_t size);
char *heap_malloc(char heap[SIZE_HEAP], list_t *libre, uint8_t size,
                  int8_t (*strategie)(char *, list_t *, uint8_t));

void list_sort(list_t *libre);

void heap_free(char heap[SIZE_HEAP], list_t *libre, char *ptr);

// Deux zones libres à la suite
void search_two_free_zone(char heap[SIZE_HEAP], list_t *libre);

// Log
void log_malloc(int size, char *adresse, bool is_malloc);
void read_logger(char *logger_file);

#endif