/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 13:21:41 by smishos           #+#    #+#             */
/*   Updated: 2025/03/10 17:57:23 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest + i);
}

size_t	ft_strlen(const char *str)
{
	size_t	n;

	n = 0;
	if (!str)
		return (0);
	while (str[n])
		n++;
	return (n);
}

char	*ft_strdup(const char *src)
{
	char	*new;

	new = (char *) malloc((ft_strlen(src) + 1) * sizeof(char));
	if (!new)
		return (0);
	*(ft_strncpy(new, src, ft_strlen(src))) = 0;
	return (new);
}

char	*ft_join(char *s1, char *s2)
{
	char	*res;
	char	*temp;
	size_t	len1;
	size_t	len2;

	if (!s1)
	{
		s1 = (char *) malloc(1);
		if (!s1)
			return (NULL);
		s1[0] = 0;
	}
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = (char *) malloc((len1 + len2 + 1) * sizeof(char));
	if (!res)
		return (ft_free_null(&s1));
	temp = res;
	res = ft_strncpy(res, s1, len1);
	res = ft_strncpy(res, s2, len2);
	*res = '\0';
	free(s1);
	return (temp);
}

char	*ft_strchr(const char *s, int c)
{
	char	cc;

	cc = (char) c;
	while (1)
	{
		if (*s == cc)
			return ((char *)s);
		if (*s == 0)
			return (NULL);
		s++;
	}
}
