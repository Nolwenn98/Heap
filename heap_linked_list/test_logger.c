#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "list.h"
#include "heap_ll.h"

static list_t libre = LIST_INITIALIZER;
static char heap[SIZE_HEAP];

void test_log_write(void)
{
    log_write(12, "12", true);

    // Remise à zero de libre
    CU_ASSERT(libre.start->data == &heap[0]);
    CU_ASSERT(libre.end->data == &heap[0]);
}

void test_log_read(void)
{
    init_heap(heap, &libre);

    // Remise à zero de libre
    list_append(&libre, (void *)4);
    init_heap(heap, &libre);

    CU_ASSERT(libre.start->data == &heap[0]);
    CU_ASSERT(libre.end->data == &heap[0]);
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

        NULL == CU_add_test(pSuite, "test of test_list_sort()", test_list_sort) //||

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
