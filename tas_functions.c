#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tas_functions.h"

void display_heap(int size)
{
    int i = 0;
    int count = HEAP[0];

    printf("\n### HEAP ###\n");
    printf("%d => %d\n", 0, count);

    while (i < size)
    {

        i++;

        if (HEAP[i] == FREE_BLOCK)
        {
            printf("%d =>  \n", i);
            count--;
        }

        else if (HEAP[i] == FREE_ZONE)
        {
            printf("%d => -1\n", i);
            count--;
        }

        else if (count != 0)
        {
            printf("%d => %c \n", i, HEAP[i]);
            count--;
        }
        else
        {
            count = HEAP[i];
            printf("%d => %d\n", i, count);
        }
    }

    printf("Libre = %d\n", FREE);
}

void init_heap()
{
    HEAP[0] = 127;
    HEAP[1] = FREE_ZONE;
    for (int i = 2; i < SIZE_HEAP; i++)
    {
        HEAP[i] = FREE_BLOCK;
    }
    FREE = 0;
}

int first_fit(int size)
{
    if (FREE == -1)
        return -1;

    if (HEAP[FREE + 1] == FREE_ZONE && HEAP[FREE] >= size)
        return FREE;

    // sinon trouver -1 et vérifier qu'on a la place
    int index = FREE + 1;
    while (index < SIZE_HEAP)
    {
        if (HEAP[index + 1] == FREE_ZONE && HEAP[index] >= size)
            return index;
        index++;
    }
    return -1;
}

int worst_fit(int size)
{
    int max = 0;
    int max_index = -1;

    for (int i = FREE; i < SIZE_HEAP; i++)
    {
        if (HEAP[i + 1] == FREE_ZONE && HEAP[i] >= size && HEAP[i] > max)
        {
            max = HEAP[i];
            max_index = i;
        }
    }
    return max_index;
}

int best_fit(int size)
{
    int nearest = 1000;
    int near_index = -1;

    for (int i = FREE; i < SIZE_HEAP; i++)
    {
        if (HEAP[i + 1] == FREE_ZONE && HEAP[i] >= size && HEAP[i] - size < nearest)
        {
            nearest = HEAP[i] - size;
            near_index = i;
        }
    }
    return near_index;
}

int strategie(int size)
{
    if (FREE == -1)
        return -1;

    if (STRATEGIE == 0) // best_fit
        return (best_fit(size));

    else if (STRATEGIE == 1) // worst_fit
        return (worst_fit(size));

    else // first_fit
        return (first_fit(size));
}

char *heap_malloc(unsigned int size)
{
    int free_index = strategie(size);

    if (free_index == -1) // Si plus de place !
        return NULL;

    int size_free_zone = HEAP[free_index];

    // Allocation zone prise
    if (size_free_zone == size + 1) // S'il y a une miette derrière
        size++;

    HEAP[free_index] = size;
    HEAP[free_index + 1] = 0;

    // Définition zone libre derrière (si besoin)
    if (size_free_zone >= size + 2 && free_index + size + 2 < SIZE_HEAP)
    {
        HEAP[free_index + size + 1] = size_free_zone - size - 1;
        HEAP[free_index + size + 2] = FREE_ZONE;
    }

    FREE = first_fit(1);
    return &HEAP[free_index + 1];
}

void heap_free(char *ptr)
{
    int index_ptr;

    *ptr = FREE_ZONE;
    for (int i = 1; i < *(ptr - 1); ++i)
    {
        *(ptr + i) = FREE_BLOCK;
    }

    index_ptr = ptr - HEAP;

    if (FREE > index_ptr)
        FREE = index_ptr - 1;

    int bool = 1;

    while (bool)
    {
        bool = search_two_free_zone();
    }
}

void add_two_zone_free(int first_zone)
{
    int len_first_zone = HEAP[first_zone];
    int index_second_zone = len_first_zone + first_zone + 1;

    HEAP[first_zone] = len_first_zone + HEAP[index_second_zone] + 1;
    HEAP[index_second_zone] = FREE_BLOCK;
    HEAP[index_second_zone + 1] = FREE_BLOCK;
}

int search_two_free_zone()
{
    int free_index = FREE;

    while (free_index < SIZE_HEAP)
    {
        if (HEAP[free_index + 1] == FREE_ZONE && HEAP[HEAP[free_index] + free_index + 2] == FREE_ZONE)
        {
            add_two_zone_free(free_index);
            return 1;
        }
        free_index++;
    }

    return 0;
}