#include "libft.h"

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void    *new_ptr;

    if (!ptr)
        return (malloc(new_size));
    if (new_size == 0)
    {
        free(ptr);
        return (NULL);
    }
    new_ptr = malloc(new_size);
    if (!new_ptr)
    {
        free(ptr);
        return (NULL);
    }
    if (old_size > new_size)
        ft_memcpy(new_ptr, ptr, new_size);
    else
        ft_memcpy(new_ptr, ptr, old_size);
    free(ptr);
    return (new_ptr);
}
