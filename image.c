/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/07 16:45:41 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/11 16:37:53 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

unsigned char	*ft_get_data(const int fd_in, const t_header header)
{
	unsigned char	buff[BUFF_SIZE];
	unsigned char	*data;
	int				pos;
	int				ret;

	data = (unsigned char *)malloc(sizeof(unsigned char) * header.width * header.height * 4);
	pos = 0;
	srand(time(NULL));
	while ((ret = read(fd_in, buff, BUFF_SIZE)))
	{
		memcpy(data + pos, buff, ret);
		pos += ret;
	}
	return (data);
}

void			data_to_img(t_mlx mlx)
{
	char			*img_data;
	int				bpx;
	int				linesize;
	int				endian;
	int				i;
	int				j;
	int				k;
	int				index;

	if (mlx.env.img)
	{
		img_data = mlx_get_data_addr(mlx.env.img, &bpx, &linesize, &endian);
		i = 0;
		k = 0;
		while ((int)i < mlx.header.height)
		{
			j = 0;
			k = (i) * (mlx.header.width * 3);
			while ((int)j < mlx.header.width)
			{
				index = i * linesize + ((j * bpx) >> 3);
				img_data[index] = mlx.copy[k++];
				img_data[++index] = mlx.copy[k++];
				img_data[++index] = mlx.copy[k++];
				j++;
			}
			i++;
		}
	}
}
