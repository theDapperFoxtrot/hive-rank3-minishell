/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 13:21:56 by smishos           #+#    #+#             */
/*   Updated: 2025/03/10 16:23:29 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "../libft.h"

char	*get_next_line(int fd);
char	*ft_substr(char const *s, unsigned int start, size_t size);
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *src);
char	*ft_join(char *s1, char *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_free_null(char **str);

#endif