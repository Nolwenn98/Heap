#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tas_functions.h"

void display_heap(int size)
{
    int i = 0;
    int count = heap[0];

    printf("\n### heap ###\n");
    printf("%d => %d\n", 0, count);

    while (i < size)
    {

        i++;

        if (heap[i] == FREE_BLOCK)
        {
            printf("%d =>  \n", i);
            count--;
        }

        else if (heap[i] == FREE_ZONE)
        {
            printf("%d => -1\n", i);
            count--;
        }

        else if (count != 0)
        {
            printf("%d => %c \n", i, heap[i]);
            count--;
        }
        else
        {
            count = heap[i];
            printf("%d => %d\n", i, count);
        }
    }

    printf("Libre = %d\n", libre);
}

void init_heap()
{
    heap[0] = SIZE_HEAP - 1;
    heap[1] = FREE_ZONE;
    for (int i = 2; i < SIZE_HEAP; i++)
    {
        heap[i] = FREE_BLOCK;
    }
    libre = 0;
}

int first_fit(int size)
{
    if (libre == MEMORY_FULL)
        return MEMORY_FULL;

    // sinon trouver -1 et vérifier qu'on a la place
    int jump;
    int index;
    index = libre;

    while (index < SIZE_HEAP)
    {
        if (heap[index + 1] == FREE_ZONE && heap[index] >= size)
            return index;

        jump = heap[index];
        index = index + jump + 1;
    }
    return MEMORY_FULL;
}

int worst_fit(int size)
{
    int max = 0;
    int max_index = MEMORY_FULL;

    int jump;
    int index;
    index = libre;

    while (index < SIZE_HEAP) // Sauter de block en block
    {
        if (heap[index + 1] == FREE_ZONE && heap[index] >= size && heap[index] > max)
        {
            max = heap[index];
            max_index = index;
        }
        jump = heap[index];
        index = index + jump + 1;
    }
    return max_index;
}

int best_fit(int size)
{
    int nearest = SIZE_HEAP;
    int near_index = MEMORY_FULL;

    int jump;
    int index;
    index = libre;

    while (index < SIZE_HEAP)
    {
        if (heap[index + 1] == FREE_ZONE && heap[index] >= size && heap[index] - size < nearest)
        {
            nearest = heap[index] - size;
            near_index = index;
        }
        jump = heap[index];
        index = index + jump + 1;
    }
    return near_index;
}

int strategie(int size)
{
    if (libre == MEMORY_FULL)
        return MEMORY_FULL;

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

    if (free_index == MEMORY_FULL)
        return NULL;

    int size_free_zone = heap[free_index];

    // Allocation zone prise
    if (size_free_zone == size + 1) // S'il y a une miette derrière
        size++;

    heap[free_index] = size;
    heap[free_index + 1] = FREE_BLOCK;

    // Définition zone libre derrière (si besoin)
    if (size_free_zone >= size + 2 && free_index + size + 2 < SIZE_HEAP)
    {
        heap[free_index + size + 1] = size_free_zone - size - 1;
        heap[free_index + size + 2] = FREE_ZONE;
    }

    libre = first_fit(1);
    return &heap[free_index + 1];
}

void heap_free(char *ptr)
{
    int index_ptr;

    *ptr = FREE_ZONE;
    for (int i = 1; i < *(ptr - 1); ++i)
    {
        *(ptr + i) = FREE_BLOCK;
    }

    index_ptr = ptr - heap;

    if (libre > index_ptr)
        libre = index_ptr - 1;

    int bool = 1;

    while (bool)
    {
        bool = search_two_free_zone();
    }
}

void add_two_zone_free(int first_zone)
{
    int len_first_zone = heap[first_zone];
    int index_second_zone = len_first_zone + first_zone + 1;

    heap[first_zone] = len_first_zone + heap[index_second_zone] + 1;
    heap[index_second_zone] = FREE_BLOCK;
    heap[index_second_zone + 1] = FREE_BLOCK;
}

int search_two_free_zone()
{

    int jump;
    int index;
    index = libre;

    while (index < SIZE_HEAP)
    {
        if (heap[index + 1] == FREE_ZONE && heap[heap[index] + index + 2] == FREE_ZONE)
        {
            add_two_zone_free(index);
            return 1;
        }

        jump = heap[index];
        index = index + jump + 1;
    }

    return 0;
}