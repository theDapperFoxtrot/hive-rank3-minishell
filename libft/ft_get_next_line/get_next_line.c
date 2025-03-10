/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 13:21:24 by smishos           #+#    #+#             */
/*   Updated: 2025/03/10 16:27:16 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_to_stash(char *curr_stash, char *buffer, int fd);
char	*ret_line_clear_stash(char **curr_stash);
char	*ft_free_null(char **str);
char	*ft_substr(char const *s, unsigned int start, size_t size);

char	*get_next_line(int fd)
{
	char			*buffer;
	static char		*stash[1024];

	if (fd < 0 || 1024 < fd || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *) malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer || read(fd, 0, 0) < 0)
	{
		ft_free_null(&stash[fd]);
		return (ft_free_null(&buffer));
	}
	buffer[0] = 0;
	stash[fd] = read_to_stash(stash[fd], buffer, fd);
	if (!stash[fd])
		return (NULL);
	return (ret_line_clear_stash(&stash[fd]));
}

char	*read_to_stash(char *curr_stash, char *buffer, int fd)
{
	ssize_t	bytes;

	bytes = 1;
	while ((!curr_stash || (curr_stash && !ft_strchr(curr_stash, '\n')))
		&& bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0)
		{
			if (bytes == -1)
				ft_free_null(&curr_stash);
			break ;
		}
		buffer[bytes] = 0;
		curr_stash = ft_join(curr_stash, buffer);
	}
	free(buffer);
	return (curr_stash);
}

char	*ret_line_clear_stash(char **curr_stash)
{
	char			*tmp;
	char			*line;
	char			*pt_to_n;
	size_t			len;

	pt_to_n = ft_strchr(*curr_stash, '\n');
	len = pt_to_n - *curr_stash + 1;
	line = ft_substr(*curr_stash, 0, len);
	if (!line)
		return (ft_free_null(curr_stash));
	if (!pt_to_n || !curr_stash[0][len])
	{
		ft_free_null(curr_stash);
		return (line);
	}
	tmp = *curr_stash;
	*curr_stash = ft_substr(*curr_stash, len, ft_strlen(*curr_stash) - len);
	if (!*curr_stash)
		*curr_stash = NULL;
	ft_free_null(&tmp);
	return (line);
}

char	*ft_free_null(char **str)
{
	free(*str);
	*str = NULL;
	return (*str);
}

char	*ft_substr(char const *s, unsigned int start, size_t size)
{
	char	*new;
	size_t	chr_left;

	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	chr_left = ft_strlen(s + start);
	if (size > chr_left)
		size = chr_left;
	new = (char *) malloc((size + 1) * sizeof(char));
	if (!new)
		return (NULL);
	*(ft_strncpy(new, s + start, size)) = 0;
	return (new);
}
