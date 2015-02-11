/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glitch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 16:37:02 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/11 18:38:51 by jlelez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void			ft_glitch(t_mlx mlx, int equiv_size, int equiv, int noise)
{
	unsigned char	*data;
	int				i;
	int				tmp;

	data = mlx.copy;
	/* Remplacement des pixels similaires */
	i = 0;
	while (i < mlx.header.width * mlx.header.height * 3)
	{
		tmp = 0;
		while (tmp + 5 <= equiv_size)
		{
			if (abs(data[tmp + i] - data[tmp + i + 3]) < equiv &&
			abs(data[tmp + i + 1] - data[tmp + i + 4]) < equiv &&
			abs(data[tmp + i + 2] - data[tmp + i + 5]) < equiv)
			{
				data[tmp + i]  = data[i + 0] + (rand() % 20);
				data[tmp + i + 1] = data[i + 1] + (rand() % 20);
				data[tmp + i + 2] = data[i + 2] + (rand() % 20);
			}
			tmp += 3;
		}
		i += tmp;
	}

	/* Generation de bruit aleatoire, la valeur de 150 est pour eviter le cote carnaval */
	i = 0;
	while (i < mlx.header.width * mlx.header.height * 3)
	{
		if (rand() % 100 < noise)
		{
			data[i] = rand() % 150;
			data[i + 1] = rand() % 150;
			data[i + 2] = rand() % 150;
		}
		i += 3;
	}
}
