#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "heap_ll.h"

void init_heap(list_t *libre)
{
    heap[0] = SIZE_HEAP - 1;
    heap[1] = FREE_ZONE;
    for (int i = 2; i < SIZE_HEAP; i++)
    {
        heap[i] = FREE_BLOCK;
    }

    // uint8_t first_free_index = 0;
    while (libre->start != NULL)
    {
        list_pop_first(libre);
    }

    list_append(libre, (void *)0);
}

void display_heap(int size, list_t *libre)
{
    uint8_t i = 0;
    uint8_t count = heap[0];

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

    element_t *elem = libre->start;
    printf("Libre : \n");
    while (elem != NULL)
    {
        printf("%d\n", (int)elem->data);
        elem = elem->next;
    }
}

// int get_index_next_block(int index)
// {
//     return heap[index] + index + 1;
// }

int8_t first_fit(uint8_t size, list_t *libre)
{
    if (libre->start == NULL)
        return MEMORY_FULL;

    // sinon parcourir les espaces libres et voir si on a assez de place pour allouer
    element_t *element;
    element = libre->start;

    while (element != NULL)
    {
        int8_t index = (int)element->data;
        if (heap[index] >= size)
            return (int)element->data;

        element = element->next;
    }

    return MEMORY_FULL;
}

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

// void update_libre(int free_index)
// {
//     // free_index vient d'être allouer
//     Liste *tmp, *tmp_prev;
//     int index_free;
//     index_free = first_fit(1);
//     printf("index_free %d\n", index_free);
//     tmp = libre;
//     if (libre->number >= index_free)
//     {
//         tmp->number = index_free;
//         tmp->next = libre;
//         libre = tmp;
//         return;
//     }

//     tmp_prev = libre;
//     tmp = tmp->next;
//     while (tmp->number < index_free || tmp->next != NULL)
//     {
//         tmp = tmp->next;
//         tmp_prev = tmp_prev->next;
//     }
//     tmp->number = index_free;
//     tmp_prev->next = tmp;
// }

char *heap_malloc(uint8_t size, list_t *libre, int8_t (*strategie)(uint8_t, list_t *))
{

    int8_t free_index = strategie(size, libre);

    printf("return strategy : %d", free_index);
    if (free_index == MEMORY_FULL)
        return NULL;

    uint8_t size_free_zone = heap[free_index];

    // Allocation zone prise
    if (size_free_zone == size + 1) // S'il y a une miette derrière
        size++;

    heap[free_index] = size;
    heap[free_index + 1] = FREE_BLOCK;

    int32_t index_rm = list_index(libre, (void *)free_index);
    list_remove_at(libre, index_rm);

    // Définition zone libre derrière (si besoin)
    if (size_free_zone >= size + 2 && free_index + size + 2 < SIZE_HEAP)
    {
        heap[free_index + size + 1] = size_free_zone - size - 1;
        heap[free_index + size + 2] = FREE_ZONE;

        // Ajouter un élément à libre
        int index_new_free = free_index + size + 1;
        list_append(libre, (void *)index_new_free); // a trier
    }

    return &heap[free_index + 1];
}

void heap_free(char *ptr, list_t *libre)
{
    int8_t index_ptr;

    *ptr = FREE_ZONE;
    for (int i = 1; i < *(ptr - 1); ++i)
    {
        *(ptr + i) = FREE_BLOCK;
    }

    index_ptr = ptr - heap - 1;

    // ajouter l'index dans free // A trier

    list_append(libre, (void *)index_ptr);

    // int bool = 1;

    // while (bool)
    // {
    //     bool = search_two_free_zone();
    // }
}

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