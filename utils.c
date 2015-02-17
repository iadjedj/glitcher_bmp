/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 13:36:29 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/17 12:51:01 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

char			*ft_strnew(size_t size)
{
	char			*new;
	unsigned int	i;

	i = 0;
	if ((new = (char *)malloc(size + 1 * sizeof(new))) == NULL)
		return (NULL);
	while (i <= size + 1)
	{
		new[i] = '\0';
		i++;
	}
	return (new);
}

static int		ft_itoa_size(int n)
{
	int		size;

	size = 0;
	while (n != 0)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

char			*ft_itoa(int n)
{
	char	*str;
	int		i;
	int		len;

	str = ft_strnew(11);
	i = 0;
	len = ft_itoa_size(n);
	if (n == -2147483648)
		str = strcpy(str, "-2147483648");
	if (n < 0 && n != -2147483648)
	{
		str[i] = '-';
		n = -n;
		len++;
	}
	while (n >= 10)
	{
		str[len - i - 1] = n % 10 + '0';
		n /= 10;
		i++;
	}
	if (n < 10 && n != -2147483648)
		(str[0] != '-') ? (str[0] = n + '0') :
		(str[1] = n + '0');
	return (str);
}

void			put_error_and_exit(char *str)
{
	perror(str);
	exit(1);
}

void			clean_exit(t_glitch *glitch)
{
	free(glitch->source);
	free(glitch->copy);
	close(glitch->fd_in);
	close(glitch->fd_out);
	exit(0);
}

char			*get_file_to_write(t_glitch *glitch)
{
	int		i;
	char	*file;

	i = 1;
	file = ft_strnew(100);
	snprintf(file, 100, "%s_glitch%02d.bmp", glitch->file_name, 0);
	while (access(file, F_OK) != -1)
	{
		snprintf(file, 100, "%s_glitch%02d.bmp", glitch->file_name, i);
		i++;
	}
	return(file);
}
