/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glitch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 16:37:02 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/12 11:38:05 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void			ft_glitch(t_glitch glitch, int equiv_size, int equiv, int noise)
{
	unsigned char	*data;
	int				i;
	int				tmp;

	data = glitch.copy;
	/* Remplacement des pixels similaires */
	i = 0;
	while (i < glitch.header.width * glitch.header.height * 3)
	{
		tmp = 0;
		while (tmp + 5 <= equiv_size)
		{
			if (abs(data[tmp + i] - data[tmp + i + 3]) < equiv &&
			abs(data[tmp + i + 1] - data[tmp + i + 4]) < equiv &&
			abs(data[tmp + i + 2] - data[tmp + i + 5]) < equiv)
			{
				data[tmp + i] = data[i + 0];
				data[tmp + i + 1] = data[i + 1];
				data[tmp + i + 2] = data[i + 2];
			}
			tmp += 3;
		}
		i += tmp;
	}

	/* Generation de bruit aleatoire, la valeur de 150 est pour eviter les couleurs trop criardes */
	i = 0;
	while (i < glitch.header.width * glitch.header.height * 3)
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
