#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_functions.h"
#include "heap_ll.h"

Liste *init_heap()
{
    Liste *first_element = malloc(sizeof(*first_element));
    Liste *second_element = malloc(sizeof(*second_element));
    if (first_element == NULL || second_element == NULL)
    {
        exit(1);
    }

    second_element->number = FREE_ZONE;
    second_element->next = NULL;

    first_element->number = SIZE_HEAP - 1;
    first_element->next = second_element;

    Liste *ptr = second_element;
    for (int i = 0; i < SIZE_HEAP - 2; i++)
    {
        Liste *next_element = malloc(sizeof(*next_element));
        ptr->next = next_element;
        ptr = ptr->next;
        ptr->number = FREE_BLOCK;
    }

    libre = first_element;

    return first_element;
}

void ll_print(Liste *list)
{
    Liste *ptr = list;
    int i = 0;

    while (ptr != NULL && i < 10)
    {
        printf("%d => %d \n", i, ptr->number);
        ptr = ptr->next;
        i++;
    }
    printf("\n");
}

void add_words(Liste *list, char word[], int size, Liste *heap)
{
    for (int i = 0; i < size; i++)
    {
        list->number = word[i];
        list = list->next;
    }
}

void display_heap(Liste *heap, int size)
{
    ll_print(heap);
    int i = 0;
    int count = heap->number;
    Liste *ptr = heap;

    printf("\n### heap ###\n");
    printf("%d => %d\n", 0, count);

    while (i < size)
    {
        i++;
        ptr = ptr->next;

        if (heap->number == FREE_BLOCK)
        {
            printf("%d =>  \n", i);
            count--;
        }

        else if (heap->number == FREE_ZONE)
        {
            printf("%d => -1\n", i);
            count--;
        }

        else if (count != 0)
        {
            printf("%d => %c \n", i, heap->number);
            count--;
        }
        else
        {
            count = heap->number;
            printf("%d => %d\n", i, count);
        }
    }

    // printf("Libre = %d\n", libre);
}

Liste *get_ptr_next_block(Liste *ptr_heap)
{
    if (ptr_heap->next->number == -1)
        return ptr_heap;

    return get_ptr_next_block(ptr_heap->next);
}

// Liste *first_fit(int size)
// {
//     if (libre == MEMORY_FULL) // Comment gère t on la mémoire pleine ?
//         return MEMORY_FULL;

//     // sinon trouver -1 et vérifier qu'on a la place
//     Liste *index = libre;

//     while (index->next != NULL)
//     {
//         if (heap->next->number == FREE_ZONE && heap->number >= size)
//             return index;

//         index = get_ptr_next_block(index);
//     }
//     return MEMORY_FULL;
// }

// int worst_fit(int size)
// {
//     int max = 0;
//     int max_index = MEMORY_FULL;

//     int index = libre;

//     while (index < SIZE_HEAP) // Sauter de block en block
//     {
//         if (heap[index + 1] == FREE_ZONE && heap[index] >= size && heap[index] > max)
//         {
//             max = heap[index];
//             max_index = index;
//         }
//         index = get_index_next_block(index);
//     }
//     return max_index;
// }

// int best_fit(int size)
// {
//     int nearest = SIZE_HEAP;
//     int near_index = MEMORY_FULL;

//     int index = libre;

//     while (index < SIZE_HEAP)
//     {
//         if (heap[index + 1] == FREE_ZONE && heap[index] >= size && heap[index] - size < nearest)
//         {
//             nearest = heap[index] - size;
//             near_index = index;
//         }
//         index = get_index_next_block(index);
//     }
//     return near_index;
// }

// char *heap_malloc(unsigned int size, int (*strategie)(int))
// {

//     int free_index = strategie(size);

//     if (free_index == MEMORY_FULL)
//         return NULL;

//     int size_free_zone = heap[free_index];

//     // Allocation zone prise
//     if (size_free_zone == size + 1) // S'il y a une miette derrière
//         size++;

//     heap[free_index] = size;
//     heap[free_index + 1] = FREE_BLOCK;

//     // Définition zone libre derrière (si besoin)
//     if (size_free_zone >= size + 2 && free_index + size + 2 < SIZE_HEAP)
//     {
//         heap[free_index + size + 1] = size_free_zone - size - 1;
//         heap[free_index + size + 2] = FREE_ZONE;
//     }

//     libre = first_fit(1);
//     return &heap[free_index + 1];
// }

// void heap_free(char *ptr)
// {
//     int index_ptr;

//     *ptr = FREE_ZONE;
//     for (int i = 1; i < *(ptr - 1); ++i)
//     {
//         *(ptr + i) = FREE_BLOCK;
//     }

//     index_ptr = ptr - heap;

//     if (libre > index_ptr)
//         libre = index_ptr - 1;

//     int bool = 1;

//     while (bool)
//     {
//         bool = search_two_free_zone();
//     }
// }

// void add_two_zone_free(int first_zone)
// {
//     int len_first_zone = heap[first_zone];
//     int index_second_zone = len_first_zone + first_zone + 1;

//     heap[first_zone] = len_first_zone + heap[index_second_zone] + 1;
//     heap[index_second_zone] = FREE_BLOCK;
//     heap[index_second_zone + 1] = FREE_BLOCK;
// }

// int search_two_free_zone()
// {
//     int index = libre;

//     while (index < SIZE_HEAP)
//     {
//         if (heap[index + 1] == FREE_ZONE && heap[heap[index] + index + 2] == FREE_ZONE)
//         {
//             add_two_zone_free(index);
//             return 1;
//         }
//         index = get_index_next_block(index);
//     }

//     return 0;
// }