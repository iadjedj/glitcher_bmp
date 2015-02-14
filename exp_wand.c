/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wand.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 16:37:02 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/12 18:34:08 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

/* En cours de dev : algo de remplacement par zone et non lineaire					*/
/* Utilisation : A lancer avec une position de depart et la taille totale de data	*/
/* 	size = glitch.header.width * glitch.header.height * 3							*/
/*
d=sqrt((r2-r1)^2+(g2-g1)^2+(b2-b1)^2)
p= 100 * d/sqrt((255)^2+(255)^2+(255)^2)
*/

typedef struct				s_rgb
{
	unsigned char			R;
	unsigned char			G;
	unsigned char			B;
}							t_rgb;

int				get_diff(t_glitch *glitch, int x1, int y1, t_rgb *comp)
{
	unsigned char *data;
	int distance;
	int percent;

	data = glitch->copy;

	distance = sqrt(
		pow(comp->B - data[3 * ((y1 * glitch->header.width) + x1) + 0], 2) +
		pow(comp->G - data[3 * ((y1 * glitch->header.width) + x1) + 1], 2) +
		pow(comp->R - data[3 * ((y1 * glitch->header.width) + x1) + 2], 2)
		);
	percent = (100 * distance) / sqrt(195075);
	return (percent);
}

void			recurs(t_glitch *glitch, const int line, const int col, const int size, t_rgb *start, int *pos)
{
	unsigned char	*data;

	data = glitch->copy;
	(*pos)++;
	if (*pos > 1000)
	{
		*pos = 0;
		glitch->env.img = mlx_new_image(glitch->env.mlx, glitch->header.width, glitch->header.height);
		data_to_img(*glitch);
		mlx_put_image_to_window(glitch->env.mlx, glitch->env.win, glitch->env.img, 0, 0);
		usleep(20000);
	}
	if (line < 0 || col < 0 || line + 1 > glitch->header.height || col + 1 > glitch->header.width ||
		get_diff(glitch, col, line, start) > 40 ||
		(data[3 * ((line * glitch->header.width) + col)] == 255 &&
				data[3 * ((line * glitch->header.width) + col) + 1] == 15 &&
				data[3 * ((line * glitch->header.width) + col) + 2] == 0))
	{
			return ;
	}
	data[3 * ((line * glitch->header.width) + col)] = 255;
	data[3 * ((line * glitch->header.width) + col) + 1] = 15;
	data[3 * ((line * glitch->header.width) + col) + 2] = 0;
	recurs(glitch, line + 1, col, size, start, pos);
	recurs(glitch, line - 1, col, size, start, pos);
	recurs(glitch, line, col + 1, size, start, pos);
	recurs(glitch, line, col - 1, size, start, pos);
}


void			ft_glitch(t_glitch glitch, int equiv_size, int equiv, int noise)
{
	int				size;

	size = glitch.header.width * glitch.header.height * 3;
	(void)equiv_size;
	(void)equiv;
	(void)noise;

	int x;
	int y;
	int pos;
	unsigned char	*data;
	t_rgb			start;

	data = glitch.copy;
	x = 150;
	y = 100;
	pos = 0;
	start.B = data[3 * ((y * glitch.header.width) + x) + 0];
	start.G = data[3 * ((y * glitch.header.width) + x) + 1];
	start.R = data[3 * ((y * glitch.header.width) + x) + 2];
	recurs(&glitch, x, y, size, &start, &pos);
}
