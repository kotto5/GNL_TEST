/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakiba <kotto555555@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 13:10:57 by kakiba            #+#    #+#             */
/*   Updated: 2022/08/28 20:16:28 by kakiba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
//realloc で行われるfree で、remain が生じるときのみinvalid freeになっている
//最後の行に勝手に改行を加えてしまっている（これ大きな問題じゃないか？）
char	*get_next_line(int fd)
{
	char		*buf;
	char		*new_line;
	size_t		allocate_size;
	static char	remain[OPEN_MAX][BUFFER_SIZE + 1UL];

	allocate_size = BUFFER_SIZE + 1UL;
	buf = malloc(allocate_size);
	if (!buf)
		return (NULL);
	if (remain[fd][0] != '\0')
	{
		allocate_size += ft_strlcpy_clear(buf, remain[fd], BUFFER_SIZE);
		ft_bzero(remain[fd], BUFFER_SIZE);
	}
	new_line = ft_strchr(buf, '\n', allocate_size - 1);
	if (new_line == NULL)
		buf = read_file(fd, buf, &new_line, &allocate_size);
	if (new_line)
	{
		if (new_line[1] != '\0')
			ft_strlcpy_clear(remain[fd], new_line + 1, BUFFER_SIZE);
		// ft_bzero(new_line + 1, allocate_size - 1);
		return (ft_realloc(buf, allocate_size, (new_line - buf + 2)));
	}
	return (buf);
}

char	*read_file(int fd, char *buf, char **new_line,
							size_t *allocate_size)
{
	ssize_t	read_length;

	read_length = BUFFER_SIZE;
	while (*new_line == NULL && read_length == BUFFER_SIZE)
	{
		buf = ft_realloc(buf, *allocate_size - BUFFER_SIZE, *allocate_size);
		if (!buf)
			return (NULL);
		read_length = read(fd, buf + *allocate_size - BUFFER_SIZE - 1, \
		BUFFER_SIZE);
		*new_line = ft_strchr(&buf[*allocate_size - BUFFER_SIZE - 1], \
			'\n', BUFFER_SIZE);
		if (*allocate_size == BUFFER_SIZE + 1UL && \
		read_length <= 0 && !(*new_line))
		{
			free (buf);
			buf = NULL;
			return (NULL);
		}
		*allocate_size += read_length;
	}
	if (read_length < BUFFER_SIZE && !(*new_line))
		buf = ft_realloc(buf, *allocate_size - BUFFER_SIZE, *allocate_size);
	return (buf);
}

// if remain has nothing, strchr() overflow so it must have length.
// Name is easily confused original strchr...
char	*ft_strchr(const char *s, int c, size_t length)
{
	size_t	i;

	if (s == NULL)
		return (NULL);
	i = 0;
	while (s[i] && i < length && s[i] != (char)c)
		i++;
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}

// to clear remain, fill src '\0'
size_t	ft_strlcpy_clear(char *dst, char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0' && i + 1 < dstsize)
	{
		dst[i] = src[i];
		// src[i] = '\0';
		i++;
	}
	if (dstsize != 0)
		dst[i] = '\0';
	//while (i < dstsize)
	//while (src[i] != '\0')
	while (src[i] != '\0' && i < dstsize)
	{
		// src[i] = '\0';
		i++;
	}
	return (i);
}

//after read, to stop Line112
void	*ft_realloc(void *src, size_t src_size, size_t new_size)
{
	void	*dst;

	// dst = ft_calloc(new_size, 1);
	dst = malloc(new_size);
	if (src_size != 0 && dst)
	{
		if (src_size < new_size)
		{
			((char *)dst)[new_size - 1] = '\0';
			ft_strlcpy_clear((char *)dst, (char *)src, src_size);
		}
		else
		{
			size_t	i;
			for (i = 0; i + 1 < new_size; ++i)
			{
				((char *)dst)[i] = ((char *)src)[i];
			}
			((char *)dst)[i] = '\0';
		}
	}
	free (src);
	src = NULL;
	return (dst);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*m;

	if (size == 0 || count == 0)
	{
		count = 1;
		size = 1;
	}
	if (count <= __SIZE_MAX__ / size)
	{
		m = malloc(count * size);
		if (m != NULL)
			ft_bzero(m, size * count);
	}
	else
		m = NULL;
	return (m);
}

void	*ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*m;

	m = (unsigned char *)s;
	i = 0;
	while (i < n)
		m[i++] = '\0';
	return (m);
}
