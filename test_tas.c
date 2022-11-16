#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "tas_functions.h"

void test_init_heap(void)
{

    init_heap();

    CU_ASSERT(HEAP[0] == 127);
    CU_ASSERT(HEAP[1] == FREE_ZONE);
    CU_ASSERT(FREE == 0);
}

void test_first_fit(void)
{

    int size = 3;

    init_heap();
    CU_ASSERT(first_fit(size) == 0);

    HEAP[0] = 3;
    HEAP[1] = 'a';
    HEAP[2] = 'b';
    HEAP[3] = '\0';
    HEAP[4] = 123;
    HEAP[5] = -1;
    FREE = 4;
    CU_ASSERT(first_fit(size) == 4);

    HEAP[0] = 3;
    HEAP[1] = 'a';
    HEAP[2] = 'b';
    HEAP[3] = '\0';
    HEAP[4] = 2;
    HEAP[5] = -1;
    HEAP[6] = 3;
    HEAP[7] = 'a';
    HEAP[8] = 'b';
    HEAP[9] = '\0';
    HEAP[10] = 118;
    HEAP[11] = -1;
    FREE = 4;
    CU_ASSERT(first_fit(size) == 10);

    HEAP[0] = 127;
    HEAP[127] = '\0';
    FREE = -1;
    CU_ASSERT(first_fit(size) == FREE_ZONE);
}

void test_add_character(void)
{
    char *p1;

    init_heap();
    p1 = (char *)heap_malloc(3);
    strcpy(p1, "ab");

    CU_ASSERT(HEAP[0] == 3 && HEAP[1] == 'a' && HEAP[2] == 'b' && HEAP[3] == FREE_BLOCK)
    CU_ASSERT(HEAP[4] == 123 && HEAP[5] == FREE_ZONE);
    CU_ASSERT(FREE == 4);
}

void test_empty_heap(void)
{

    char *p1;

    init_heap();
    p1 = (char *)heap_malloc(127);
    strcpy(p1, "ab");

    CU_ASSERT(HEAP[0] == 127 && HEAP[127] == FREE_BLOCK);
    CU_ASSERT(FREE == FREE_ZONE);
}

void test_add_to_empty_heap(void)
{

    char *p1, *p2;

    init_heap();
    p1 = (char *)heap_malloc(127);
    strcpy(p1, "ab");

    p2 = (char *)heap_malloc(3);
    display_heap(127);
    CU_ASSERT(p2 == NULL);
    CU_ASSERT(FREE == FREE_ZONE);
}

void test_add_to_almost_empty_heap(void)
{

    char *p1, *p2;

    init_heap();
    p1 = (char *)heap_malloc(125);
    strcpy(p1, "ab");

    p2 = (char *)heap_malloc(3);

    CU_ASSERT(p2 == NULL);
    CU_ASSERT(FREE == 126);
}

void test_heap_free(void)
{

    init_heap();
    HEAP[0] = 3;
    HEAP[1] = 'a';
    HEAP[2] = 'b';
    HEAP[3] = '\0';
    HEAP[4] = 2;
    HEAP[5] = 'a';
    HEAP[6] = '\0';
    HEAP[7] = 3;
    HEAP[8] = 'a';
    HEAP[9] = 'b';
    HEAP[10] = '\0';
    HEAP[11] = 116;
    HEAP[12] = -1;
    FREE = 11;

    heap_free(&HEAP[5]);

    CU_ASSERT(HEAP[4] == 2 && HEAP[5] == FREE_ZONE);
    CU_ASSERT(FREE == 4);
}

void test_heap_malloc_example()
{

    init_heap();

    char *p1 = heap_malloc(10);

    CU_ASSERT(p1 - 1 == HEAP);
    CU_ASSERT(*(p1 + 1) != FREE_ZONE);

    CU_ASSERT(FREE == 10 + 1);
    CU_ASSERT(*(HEAP + FREE) == 116);
    CU_ASSERT(*(HEAP + FREE + 1) == FREE_ZONE);

    char *p2 = heap_malloc(9);

    CU_ASSERT(p2 == HEAP + 12);
    CU_ASSERT(*(p2 - 1) == 9);
    CU_ASSERT(FREE == 21);

    char *p3 = heap_malloc(5);

    CU_ASSERT(p3 == HEAP + 22);
    CU_ASSERT(*(p3 - 1) == 5);
    CU_ASSERT(FREE == 27);

    char *p4 = heap_malloc(104);

    CU_ASSERT(p4 == NULL);
}

void test_heap_free_several()
{

    init_heap();

    char *p1 = heap_malloc(10);
    char *p2 = heap_malloc(10);
    char *p3 = heap_malloc(10);
    char *p4 = heap_malloc(10);

    strcpy(p1, "tp1");
    strcpy(p2, "tp2");
    strcpy(p3, "tp3");
    strcpy(p4, "tp4");

    heap_free(p2); // simple free

    CU_ASSERT(*(p2 - 1) == 10);
    CU_ASSERT(*(p2) == FREE_ZONE);
    CU_ASSERT(FREE == p2 - 1 - HEAP);

    heap_free(p3); // testing merge left

    CU_ASSERT(*(p3) == FREE_BLOCK);
    CU_ASSERT(*(p3 - 1) == FREE_BLOCK);

    CU_ASSERT(*(p2 - 1) == 21);
    CU_ASSERT(*(p2) == FREE_ZONE);
    CU_ASSERT(FREE == p2 - 1 - HEAP);

    heap_free(p1); // testing merge right

    CU_ASSERT(*(p1 - 1) == 32);
    CU_ASSERT(*(p1) == FREE_ZONE);
    CU_ASSERT(FREE == 0);
}

void test_full_example()
{
    init_heap();

    char *p1 = heap_malloc(10);
    char *p2 = heap_malloc(9);
    char *p3 = heap_malloc(5);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");

    heap_free(p2);

    // That verifies that in case we have 1 byte left we actually malloc size+1
    char *p4 = heap_malloc(8);
    strcpy(p4, "systeme");

    // display_heap(127);

    CU_ASSERT(*(p1 - 1) == 10);
    CU_ASSERT(*(p4 - 1) == 9);
    CU_ASSERT(*(p3 - 1) == 5);
    CU_ASSERT(FREE == 27);
    CU_ASSERT(HEAP[27] == 100);
    CU_ASSERT(HEAP[28] == FREE_ZONE);

    CU_ASSERT(strcmp(p1, "tp 1") == 0);
    CU_ASSERT(strcmp(p3, "tp 3") == 0);
    CU_ASSERT(strcmp(p4, "systeme") == 0);
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
        NULL == CU_add_test(pSuite, "test of test_empty_heap()", test_empty_heap) ||
        NULL == CU_add_test(pSuite, "test of test_add_to_empty_heap()", test_add_to_empty_heap) ||
        NULL == CU_add_test(pSuite, "test of test_add_to_almost_empty_heap()", test_add_to_almost_empty_heap) ||
        NULL == CU_add_test(pSuite, "test of test_heap_free()", test_heap_free) ||
        NULL == CU_add_test(pSuite, "test of test_heap_malloc_example()", test_heap_malloc_example) ||
        NULL == CU_add_test(pSuite, "test of test_heap_free_several()", test_heap_free_several) ||
        NULL == CU_add_test(pSuite, "test of test_full_example()", test_full_example)

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
