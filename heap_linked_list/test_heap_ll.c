#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "list.h"
#include "heap_ll.h"

static list_t libre = LIST_INITIALIZER;

void test_init_heap(void)
{
    init_heap(&libre);

    CU_ASSERT(heap[0] == 127);
    CU_ASSERT(heap[1] == FREE_ZONE);
    CU_ASSERT((int)libre.start->data == 0);
    CU_ASSERT((int)libre.end->data == 0);

    // Remise à zero de libre
    list_append(&libre, (void *)4);
    init_heap(&libre);

    CU_ASSERT((int)libre.start->data == 0);
    CU_ASSERT((int)libre.end->data == 0);
}

void test_first_fit(void)
{
    int size = 3;
    int data1 = 4;

    // Toute la mémoire libre
    init_heap(&libre);

    CU_ASSERT(first_fit(size, &libre) == 0);

    // Une zone mémoire alloué
    heap[0] = 3;
    strcpy(&heap[1], "ab");
    list_pop_first(&libre);
    heap[4] = 123;
    heap[5] = -1;
    list_append(&libre, (void *)data1);

    CU_ASSERT(first_fit(size, &libre) == 4);

    // Toute la mémoire alloué
    init_heap(&libre);

    heap[0] = 127;
    heap[127] = '\0';
    list_pop_first(&libre);

    CU_ASSERT(first_fit(size, &libre) == MEMORY_FULL);
}

void test_add_character(void)
{
    char *p1, *p2, *p3;
    strategie = &first_fit;

    // Toute la mémoire libre
    init_heap(&libre);
    p1 = (char *)heap_malloc(3, &libre, strategie);
    strcpy(p1, "ab");

    CU_ASSERT(heap[0] == 3 && heap[1] == 'a' && heap[2] == 'b' && heap[3] == FREE_BLOCK)
    CU_ASSERT(heap[4] == 123 && heap[5] == FREE_ZONE);
    CU_ASSERT((int)libre.start->data == 4);

    // Une zone mémoire alloué
    p2 = (char *)heap_malloc(123, &libre, strategie);
    strcpy(p2, "aaaaab");

    CU_ASSERT(heap[4] == 123 && heap[5] == 'a')
    CU_ASSERT((int)libre.start == NULL);

    // Toute la mémoire alloué
    p3 = (char *)heap_malloc(4, &libre, strategie);

    CU_ASSERT(p3 == NULL)
    CU_ASSERT((int)libre.start == NULL);
}

void test_add_to_empty_heap(void)
{
    char *p1, *p2;

    init_heap(&libre);
    p1 = (char *)heap_malloc(127, &libre, strategie);
    strcpy(p1, "ab");

    p2 = (char *)heap_malloc(3, &libre, strategie);

    CU_ASSERT(p2 == NULL);
    CU_ASSERT(libre.start == NULL);
    CU_ASSERT(libre.end == NULL);
}

void test_add_to_almost_empty_heap(void)
{
    char *p1, *p2;

    init_heap(&libre);
    p1 = (char *)heap_malloc(125, &libre, strategie);
    strcpy(p1, "ab");

    p2 = (char *)heap_malloc(3, &libre, strategie);

    CU_ASSERT(p2 == NULL);
    CU_ASSERT((int)libre.start->data == 126);
}

void test_heap_free(void)
{
    init_heap(&libre);

    heap[0] = 3;
    strcpy(&heap[1], "ab");
    heap[4] = 2;
    strcpy(&heap[5], "a");
    heap[7] = 3;
    strcpy(&heap[8], "ab");
    heap[11] = 116;
    heap[12] = -1;
    libre = 11;

    heap_free(&heap[5]);

    CU_ASSERT(heap[4] == 2 && heap[5] == FREE_ZONE);
    CU_ASSERT((int)libre.start->data == 4);
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
        NULL == CU_add_test(pSuite, "test of test_first_fit()", test_first_fit) ||
        NULL == CU_add_test(pSuite, "test of test_add_character()", test_add_character) ||
        NULL == CU_add_test(pSuite, "test of test_add_to_empty_heap()", test_add_to_empty_heap) ||
        NULL == CU_add_test(pSuite, "test of test_add_to_almost_empty_heap()", test_add_to_almost_empty_heap) ||
        NULL == CU_add_test(pSuite, "test of test_heap_free()", test_heap_free)

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
