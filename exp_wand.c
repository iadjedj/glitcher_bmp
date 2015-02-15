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

int				get_diff(t_glitch *glitch, int x, int y, t_rgb *comp)
{
	unsigned char *data;
	int distance;
	int percent;

	data = glitch->copy;

	distance = sqrt(
		pow(comp->B - data[3 * ((y * glitch->header.width) + x) + 0], 2) +
		pow(comp->G - data[3 * ((y * glitch->header.width) + x) + 1], 2) +
		pow(comp->R - data[3 * ((y * glitch->header.width) + x) + 2], 2)
		);
	percent = (100 * distance) / sqrt(195075); /*195075 = 3 * 255^2 */
	return (percent);
}

void			resurs_wand(t_glitch *glitch, const int x, const int y, const int size, t_rgb *start)
{
	unsigned char	*data;

	data = glitch->copy;
	if (y < 0 || x < 0 || y + 1 > glitch->header.height || x + 1 > glitch->header.width ||
		get_diff(glitch, x, y, start) > 10 ||
		(data[3 * ((y * glitch->header.width) + x)] == start->B &&
		data[3 * ((y * glitch->header.width) + x) + 1] == start->G &&
		data[3 * ((y * glitch->header.width) + x) + 2] == start->R))
	{
			return ;
	}
	data[3 * ((y * glitch->header.width) + x) + 0] = start->B;
	data[3 * ((y * glitch->header.width) + x) + 1] = start->G;
	data[3 * ((y * glitch->header.width) + x) + 2] = start->R;
	resurs_wand(glitch, x, y + 1, size, start);
	resurs_wand(glitch, x, y - 1, size, start);
	resurs_wand(glitch, x + 1, y, size, start);
	resurs_wand(glitch, x - 1, y, size, start);
}


void			ft_wand(t_glitch glitch, int x, int y)
{
	int				size;
	unsigned char	*data;
	t_rgb			start;

	size = glitch.header.width * glitch.header.height * 3;
	data = glitch.copy;
	y = (glitch.negative_height == -1) ? y : glitch.header.height - 1 - y;
	/*Le random permet a l'algo de remplacer le pixel de départ qui est du coup different de la couleur de départ*/
	start.B = (data[3 * ((y * glitch.header.width) + x) + 0] + (rand() % 5)) % 255;
	start.G = (data[3 * ((y * glitch.header.width) + x) + 1] + (rand() % 5)) % 255;
	start.R = (data[3 * ((y * glitch.header.width) + x) + 2] + (rand() % 5)) % 255;
	printf("Pixel %dx%d replaced by #%02X%02X%02X (%d, %d, %d) \n", x, y, start.R, start.G, start.B, start.R, start.G, start.B);
	resurs_wand(&glitch, x, y, size, &start);
}
