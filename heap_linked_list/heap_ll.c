#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "heap_ll.h"

void init_heap(char heap[SIZE_HEAP], list_t *libre)
{
    heap[0] = SIZE_HEAP - 1;
    heap[1] = FREE_ZONE;
    for (int i = 2; i < SIZE_HEAP; i++)
    {
        heap[i] = FREE_BLOCK;
    }

    while (libre->start != NULL)
    {
        list_pop_first(libre);
    }

    list_append(libre, (void *)0);
}

void display_heap(char heap[SIZE_HEAP], list_t *libre, int size)
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

int8_t first_fit(char heap[SIZE_HEAP], list_t *libre, uint8_t size)
{
    if (libre->start == NULL)
        return MEMORY_FULL;

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

int8_t worst_fit(char heap[SIZE_HEAP], list_t *libre, uint8_t size)
{
    int8_t max = 0;
    int8_t max_index = MEMORY_FULL;

    element_t *element;
    element = libre->start;

    while (element != NULL)
    {
        int8_t index = (int)element->data;
        if (heap[index] >= max)
        {
            max = heap[index];
            max_index = index;
        }

        element = element->next;
    }

    return max_index;
}

int8_t best_fit(char heap[SIZE_HEAP], list_t *libre, uint8_t size)
{
    int8_t nearest = SIZE_HEAP;
    int8_t near_index = MEMORY_FULL;

    element_t *element;
    element = libre->start;

    while (element != NULL)
    {
        int8_t index = (int)element->data;
        if (heap[index] >= size && heap[index] - size < nearest)
        {
            nearest = heap[index] - size;
            near_index = index;
        }
        element = element->next;
    }
    return near_index;
}

void list_sort(list_t *libre)
{
    uint8_t index = 0;
    element_t *ptr, *element_to_move;
    ptr = libre->start;

    while (ptr->next != NULL)
    {
        if (ptr->data > ptr->next->data)
        {
            element_to_move = list_remove_at(libre, index + 1);
            list_insert_at(libre, element_to_move, index);
        }

        else
            ptr = ptr->next;

        index++;
    }
}

char *heap_malloc(char heap[SIZE_HEAP], list_t *libre, uint8_t size, int8_t (*strategie)(char *, list_t *, uint8_t))
{

    int8_t free_index = strategie(heap, libre, size);

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
        list_append(libre, (void *)index_new_free);
        list_sort(libre);
    }

    return &heap[free_index + 1];
}

void heap_free(char heap[SIZE_HEAP], list_t *libre, char *ptr)
{
    int8_t index_ptr;

    *ptr = FREE_ZONE;
    for (int i = 1; i < *(ptr - 1); ++i)
    {
        *(ptr + i) = FREE_BLOCK;
    }

    index_ptr = ptr - heap - 1;

    // Ajout de l'index dans free
    list_append(libre, (void *)index_ptr);
    list_sort(libre);

    search_two_free_zone(heap, libre);
}

void search_two_free_zone(char heap[SIZE_HEAP], list_t *libre)
{
    element_t *element;
    element = libre->start;

    while (element->next != NULL)
    {
        uint8_t index, index_next;
        uint32_t ind_remove;
        index = (int)element->data;
        index_next = (int)element->next->data;
        if (index + heap[index] + 1 == index_next)
        {
            heap[index] += heap[index_next] + 1;
            heap[index_next] = FREE_BLOCK;
            heap[index_next + 1] = FREE_BLOCK;

            ind_remove = list_index(libre, (void *)index_next);
            list_remove_at(libre, ind_remove);
        }
        else
            element = element->next;
    }
}