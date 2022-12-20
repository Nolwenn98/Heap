#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "list.h"
#include "heap_ll.h"

static list_t libre = LIST_INITIALIZER;
static char heap[SIZE_HEAP];

void test_init_heap(void)
{
    init_heap(heap, &libre);

    CU_ASSERT(heap[0] == 127);
    CU_ASSERT(heap[1] == FREE_ZONE);
    CU_ASSERT(libre.start->data == &heap[0]);
    CU_ASSERT(libre.end->data == &heap[0]);

    // Remise à zero de libre
    list_append(&libre, (void *)4);
    init_heap(heap, &libre);

    CU_ASSERT(libre.start->data == &heap[0]);
    CU_ASSERT(libre.end->data == &heap[0]);
}

void test_get_index(void)
{
    int index;
    init_heap(heap, &libre);
    index = get_index(heap, libre.start);

    CU_ASSERT(index == 0);

    list_append(&libre, heap + 4);
    index = get_index(heap, libre.start->next);

    CU_ASSERT(index == 4);
}

void test_fit_memory_free(void)
{
    int size = 3;

    // Toute la mémoire libre
    init_heap(heap, &libre);

    CU_ASSERT(first_fit(heap, &libre, size) == 0);
    CU_ASSERT(worst_fit(heap, &libre, size) == 0);
    CU_ASSERT(best_fit(heap, &libre, size) == 0);
}

void test_fit_memory_full(void)
{
    int size = 3;

    // Toute la mémoire alloué
    init_heap(heap, &libre);

    heap[0] = 127;
    heap[127] = '\0';
    list_pop_first(&libre);

    CU_ASSERT(first_fit(heap, &libre, size) == MEMORY_FULL);
    CU_ASSERT(worst_fit(heap, &libre, size) == MEMORY_FULL);
    CU_ASSERT(best_fit(heap, &libre, size) == MEMORY_FULL);
}

void test_first_fit(void)
{
    int size = 3;

    // Une zone mémoire alloué
    heap[0] = 3;
    strcpy(&heap[1], "ab");
    list_pop_first(&libre);
    heap[4] = 123;
    heap[5] = -1;
    list_append(&libre, heap + 4);

    CU_ASSERT(first_fit(heap, &libre, size) == 4);

    // Première zone mémoire trop petite
    init_heap(heap, &libre);
    heap[0] = 2;
    heap[1] = FREE_BLOCK;
    heap[3] = 4;
    heap[7] = 115;
    heap[8] = -1;
    list_append(&libre, heap + 7);
    CU_ASSERT(first_fit(heap, &libre, size) == 7);
}

void test_worst_fit(void)
{
    int size = 3;

    // Une zone mémoire alloué
    heap[0] = 3;
    strcpy(&heap[1], "ab");
    list_pop_first(&libre);
    heap[4] = 123;
    heap[5] = -1;
    list_append(&libre, heap + 4);

    CU_ASSERT(worst_fit(heap, &libre, size) == 4);

    // Plusieurs zone mémoire
    init_heap(heap, &libre);
    heap[0] = 15;
    heap[1] = FREE_BLOCK;
    heap[16] = 4;
    heap[20] = 115;
    heap[21] = -1;
    list_append(&libre, heap + 20);
    CU_ASSERT(worst_fit(heap, &libre, size) == 20);
}

void test_best_fit(void)
{
    int size = 3;

    init_heap(heap, &libre);

    // Une zone mémoire alloué
    heap[0] = 3;
    strcpy(&heap[1], "ab");
    list_pop_first(&libre);
    heap[4] = 123;
    heap[5] = -1;
    list_append(&libre, heap + 4);

    CU_ASSERT(best_fit(heap, &libre, size) == 4);

    // Plusieurs zone mémoire
    init_heap(heap, &libre);
    heap[0] = 15;
    heap[1] = FREE_BLOCK;
    heap[16] = 4;
    heap[20] = 115;
    heap[21] = -1;
    list_append(&libre, heap + 20);
    CU_ASSERT(best_fit(heap, &libre, size) == 0);

    // Plusieurs zone mémoire
    init_heap(heap, &libre);
    heap[0] = 4;
    heap[1] = FREE_BLOCK;
    heap[5] = 4;
    heap[9] = 3;
    heap[10] = -1;
    list_append(&libre, heap + 9);
    CU_ASSERT(best_fit(heap, &libre, size) == 9);
}

void test_add_character(void)
{
    char *p1, *p2, *p3;
    strategie = &first_fit;

    // Toute la mémoire libre
    init_heap(heap, &libre);
    p1 = (char *)heap_malloc(heap, &libre, 3, strategie);
    strcpy(p1, "ab");

    CU_ASSERT(heap[0] == 3 && heap[1] == 'a' && heap[2] == 'b' && heap[3] == FREE_BLOCK)
    CU_ASSERT(heap[4] == 123 && heap[5] == FREE_ZONE);
    CU_ASSERT(libre.start->data == heap + 4);

    // Une zone mémoire alloué
    p2 = (char *)heap_malloc(heap, &libre, 123, strategie);
    strcpy(p2, "aaaaab");

    CU_ASSERT(heap[4] == 123 && heap[5] == 'a')
    CU_ASSERT(libre.start == NULL);

    // Toute la mémoire alloué
    p3 = (char *)heap_malloc(heap, &libre, 4, strategie);

    CU_ASSERT(p3 == NULL)
    CU_ASSERT(libre.start == NULL);
}

void test_add_to_empty_heap(void)
{
    char *p1, *p2;
    strategie = &first_fit;

    init_heap(heap, &libre);
    p1 = (char *)heap_malloc(heap, &libre, 127, strategie);
    strcpy(p1, "ab");

    p2 = (char *)heap_malloc(heap, &libre, 3, strategie);

    CU_ASSERT(p2 == NULL);
    CU_ASSERT(libre.start == NULL);
    CU_ASSERT(libre.end == NULL);
}

void test_add_to_almost_empty_heap(void)
{
    char *p1, *p2;
    strategie = &first_fit;

    init_heap(heap, &libre);
    p1 = (char *)heap_malloc(heap, &libre, 125, strategie);
    strcpy(p1, "ab");

    p2 = (char *)heap_malloc(heap, &libre, 3, strategie);

    CU_ASSERT(p2 == NULL);
    CU_ASSERT(libre.start->data == heap + 126);
}

void test_heap_free(void)
{
    char *p1, *p2, *p3;
    strategie = &first_fit;

    init_heap(heap, &libre);
    p1 = (char *)heap_malloc(heap, &libre, 3, strategie);
    strcpy(p1, "ab");
    p2 = (char *)heap_malloc(heap, &libre, 2, strategie);
    strcpy(p2, "a");
    p3 = (char *)heap_malloc(heap, &libre, 3, strategie);
    strcpy(p3, "ab");

    heap_free(heap, &libre, p2);

    display_heap(heap, &libre, 10);

    CU_ASSERT(heap[4] == 2 && heap[5] == FREE_ZONE);
    CU_ASSERT(libre.start->data == heap + 4);
}

void test_list_sort(void)
{
    init_heap(heap, &libre);

    list_append(&libre, heap + 9);
    list_append(&libre, heap + 4);
    list_append(&libre, heap + 2);
    list_append(&libre, heap + 7);
    list_append(&libre, heap + 1);

    element_t *ptr;

    list_sort(&libre);

    ptr = libre.start;

    CU_ASSERT(ptr->data == heap + 0);
    ptr = ptr->next;
    CU_ASSERT(ptr->data == heap + 1);
    ptr = ptr->next;
    CU_ASSERT(ptr->data == heap + 2);
    ptr = ptr->next;
    CU_ASSERT(ptr->data == heap + 4);
    ptr = ptr->next;
    CU_ASSERT(ptr->data == heap + 7);
    ptr = ptr->next;
    CU_ASSERT(ptr->data == heap + 9);
}

void test_search_two_free_zone(void)
{
    char *p1, *p2;
    strategie = &first_fit;

    init_heap(heap, &libre);
    p1 = (char *)heap_malloc(heap, &libre, 3, strategie);
    p2 = (char *)heap_malloc(heap, &libre, 2, strategie);
    (char *)heap_malloc(heap, &libre, 3, strategie);

    heap_free(heap, &libre, p1);
    heap_free(heap, &libre, p2);

    CU_ASSERT(heap[0] == 6 && heap[1] == FREE_ZONE);
    CU_ASSERT(heap[4] == FREE_BLOCK && heap[5] == FREE_BLOCK);
    CU_ASSERT(libre.start->data == heap + 0);
    CU_ASSERT(libre.start->next->data == heap + 11);
}

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (
        NULL == CU_add_test(pSuite, "test of test_init_heap()", test_init_heap) ||
        NULL == CU_add_test(pSuite, "test of test_get_index()", test_get_index) ||
        NULL == CU_add_test(pSuite, "test of test_fit_memory_free()", test_fit_memory_free) ||
        NULL == CU_add_test(pSuite, "test of test_fit_memory_full()", test_fit_memory_full) ||
        NULL == CU_add_test(pSuite, "test of test_first_fit()", test_first_fit) ||
        NULL == CU_add_test(pSuite, "test of test_worst_fit()", test_worst_fit) ||
        NULL == CU_add_test(pSuite, "test of test_best_fit()", test_best_fit) ||
        NULL == CU_add_test(pSuite, "test of test_add_character()", test_add_character) ||
        NULL == CU_add_test(pSuite, "test of test_add_to_empty_heap()", test_add_to_empty_heap) ||
        NULL == CU_add_test(pSuite, "test of test_add_to_almost_empty_heap()", test_add_to_almost_empty_heap) ||
        NULL == CU_add_test(pSuite, "test of test_heap_free()", test_heap_free) ||
        NULL == CU_add_test(pSuite, "test of test_list_sort()", test_list_sort) ||
        NULL == CU_add_test(pSuite, "test of test_search_two_free_zone()", test_search_two_free_zone)

    )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
