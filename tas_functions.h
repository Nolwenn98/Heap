#ifndef HEAP_HEADER
#define HEAP_HEADER

#define SIZE_HEAP 128
#define FREE_BLOCK '\0'
#define FREE_ZONE -1
#define STRATEGIE 0 // 0 best_fit - 1 worst_fit - autre first_fit

char HEAP[SIZE_HEAP];
int FREE;

void init_heap();
void display_heap(int size);

// Stratégies

int worst_fit(int size);
int first_fit(int size);
int best_fit(int size);
char *heap_malloc(unsigned int taille);

void heap_free(char *ptr);

// Deux zones libres à la suite
int search_two_free_zone();
void add_two_zone_free(int first_zone);

#endif