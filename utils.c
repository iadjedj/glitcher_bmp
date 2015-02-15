/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/07 16:45:41 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/12 11:19:24 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

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

unsigned char	*ft_get_data(const int fd_in, const t_header header)
{
	unsigned char	buff[BUFF_SIZE];
	unsigned char	*data;
	int				pos;
	int				ret;

	data = (unsigned char *)malloc(sizeof(unsigned char) * header.width * header.height * 4);
	pos = 0;
	while ((ret = read(fd_in, buff, BUFF_SIZE)))
	{
		memcpy(data + pos, buff, ret);
		pos += ret;
	}
	return (data);
}

void			data_to_img(t_glitch glitch)
{
	char			*img_data;
	int				bpx;
	int				linesize;
	int				endian;
	int				i;
	int				j;
	int				k;
	int				index;

	if (glitch.env.img)
	{
		img_data = mlx_get_data_addr(glitch.env.img, &bpx, &linesize, &endian);
		i = 0;
		k = 0;
		while ((int)i < glitch.header.height)
		{
			j = 0;
			k = ((glitch.negative_height == -1) ? i : (glitch.header.height - 1 - i)) * (glitch.header.width * 3);
			while ((int)j < glitch.header.width)
			{
				index = i * linesize + ((j * bpx) >> 3);
				img_data[index] = glitch.copy[k++];
				img_data[++index] = glitch.copy[k++];
				img_data[++index] = glitch.copy[k++];
				j++;
			}
			i++;
		}
	}
}
