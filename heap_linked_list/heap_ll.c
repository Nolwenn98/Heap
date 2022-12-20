#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "list.h"
#include "heap_ll.h"

void get_local_time() {}

void log_malloc(int size, char *adresse, bool is_malloc)
{
    // Get time
    char format_date[128];
    char str_log[150];
    time_t temps;
    struct tm date;

    time(&temps);
    date = *localtime(&temps);
    // strftime(format_date, 128, "[%d/%m/%Y]", &date);
    strftime(format_date, 128, "[%d/%m/%Y %H:%M:%S]", &date);

    if (is_malloc)
        sprintf(str_log, "%s malloced %d byte at address %p \n", format_date, size, adresse);
    // else
    //     sprintf(str_log, "%s freed %d byte at address %p \n", format_date, size, adresse);

    // Write log
    FILE *file_ptr;
    file_ptr = fopen("log.txt", "a");
    if (file_ptr != NULL)
    {
        fputs(str_log, file_ptr);
        fclose(file_ptr);
    }
}

void read_logger(char *logger_file)
{
    int size;
    char *adresse;
    char is_malloc[12];

    FILE *file_ptr;
    file_ptr = fopen("log.txt", "r");

    if (file_ptr != NULL)
    {
        while (!feof(file_ptr))
        {
            fscanf(file_ptr, "%*s %*s %s %d byte at address %p \n",
                   is_malloc,
                   &size,
                   &adresse);

            if (is_malloc[0] == 'm')
                printf("%d - %p\n", size, adresse);
        }

        fclose(file_ptr);
    }
}

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

    list_append(libre, heap); // mettre l'adresse de la première case de heap
}

int get_index(char heap[SIZE_HEAP], element_t *elem)
{
    // printf("%d\n", *(char *)elem->data);
    // printf("%p - %p = %ld\n", heap, elem->data, (char *)elem->data - heap);
    return (char *)elem->data - heap;
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
        printf("%d\n", get_index(heap, elem));
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
        int8_t size_free = *(char *)element->data;
        if (size_free >= size)
            return get_index(heap, element);

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
        int8_t size_free = *(char *)element->data;
        if (size_free >= max)
        {
            max = size_free;
            max_index = get_index(heap, element);
        }

        element = element->next;
    }

    return max_index;
}

int8_t best_fit(char heap[SIZE_HEAP], list_t *libre, uint8_t size)
{
    uint8_t nearest = SIZE_HEAP;
    int8_t near_index = MEMORY_FULL;

    element_t *element;
    element = libre->start;

    while (element != NULL)
    {
        int8_t size_free = *(char *)element->data;

        if (size_free >= size && size_free - size < nearest)
        {
            nearest = size_free - size;
            near_index = get_index(heap, element);
        }
        element = element->next;
    }
    return near_index;
}

void list_sort(list_t *libre) // trier de l'adresse la plus petite à la plus grande
{
    uint8_t index = 0;
    element_t *ptr;
    ptr = libre->start;

    while (ptr->next != NULL)
    {
        uint32_t index_move = index + 1;
        void *data_to_move;
        if (ptr->previous == NULL && (char *)ptr->data > (char *)ptr->next->data)
        {
            data_to_move = list_remove_at(libre, index_move);
            list_insert_at(libre, data_to_move, index_move - 1);
        }
        else if ((char *)ptr->data > (char *)ptr->next->data)
        {
            data_to_move = list_remove_at(libre, index_move);
            list_insert_at(libre, data_to_move, index_move - 1);
            index -= 2;
            ptr = ptr->previous->previous;
        }
        else
        {
            ptr = ptr->next;
        }
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

    int32_t index_rm = list_index(libre, heap + free_index);
    list_remove_at(libre, index_rm);

    // Définition zone libre derrière (si besoin)
    if (size_free_zone >= size + 2 && free_index + size + 2 < SIZE_HEAP)
    {
        heap[free_index + size + 1] = size_free_zone - size - 1;
        heap[free_index + size + 2] = FREE_ZONE;

        // Ajouter un élément à libre
        int index_new_free = free_index + size + 1;
        list_append(libre, heap + index_new_free);
        list_sort(libre);
    }

    log_malloc(size, heap + free_index, true);

    return &heap[free_index + 1];
}

void heap_free(char heap[SIZE_HEAP], list_t *libre, char *ptr)
{
    int8_t index_ptr;

    index_ptr = ptr - heap - 1;
    // log_malloc(*(heap + index_ptr), heap + index_ptr, false);

    *ptr = FREE_ZONE;
    for (int i = 1; i < *(ptr - 1); ++i)
    {
        *(ptr + i) = FREE_BLOCK;
    }

    // Ajout de l'index dans free

    list_append(libre, heap + index_ptr);
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
        index = get_index(heap, element);
        index_next = get_index(heap, element->next);
        if (index + heap[index] + 1 == index_next)
        {
            heap[index] += heap[index_next] + 1;
            heap[index_next] = FREE_BLOCK;
            heap[index_next + 1] = FREE_BLOCK;

            ind_remove = list_index(libre, heap + index_next);
            list_remove_at(libre, ind_remove);
        }
        else
            element = element->next;
    }
}