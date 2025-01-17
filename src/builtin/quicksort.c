#include "../../include/minishell.h"

// Function to swap two elements
void    swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for quicksort
int partition(char **arr, int low, int high)
{
    char *pivot;
    int i;
    int j;

    pivot = arr[high];
    i = low - 1;

    j = low;
    while (j < high)
    {
        if (ft_strncmp(arr[j], pivot, ft_strlen(pivot)) < 0)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
        j++;
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Quicksort function
void quicksort(char **arr, int low, int high)
{
    int pi;

    if (low < high) {
        pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}
