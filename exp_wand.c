/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_wand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 16:37:02 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/16 18:00:32 by iadjedj          ###   ########.fr       */
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

void			recurs_wand(t_glitch *glitch, const int x, const int y, const int size, t_rgb *start)
{
	if (y < 0 || x < 0 || y + 1 > glitch->header.height || x + 1 > glitch->header.width ||
		get_diff(glitch, x, y, start) > 10 ||
		(glitch->copy[3 * ((y * glitch->header.width) + x)] == start->B &&
		glitch->copy[3 * ((y * glitch->header.width) + x) + 1] == start->G &&
		glitch->copy[3 * ((y * glitch->header.width) + x) + 2] == start->R))
	{
			return ;
	}
	glitch->copy[3 * ((y * glitch->header.width) + x) + 0] = start->B;
	glitch->copy[3 * ((y * glitch->header.width) + x) + 1] = start->G;
	glitch->copy[3 * ((y * glitch->header.width) + x) + 2] = start->R;
	recurs_wand(glitch, x, y + 1, size, start);
	recurs_wand(glitch, x, y - 1, size, start);
	recurs_wand(glitch, x + 1, y, size, start);
	recurs_wand(glitch, x - 1, y, size, start);
}

void		*launch_recurs(void *input)
{
	t_glitch *glitch;
	int x;
	int y;
	int size;
	t_rgb *start;
	t_thread *data;

	data = (t_thread*)input;
	glitch = data->glitch;
	x = data->x;
	y = data->y;
	size = data->size;
	start = data->start;
	recurs_wand(glitch, x, y, size, start);
	pthread_exit(NULL);
}

void			ft_wand(t_glitch glitch, int x, int y)
{
	int				size;
	unsigned char	*data;
	t_rgb			start;
	pthread_t		threads[1];
	pthread_attr_t attr;
	int				err;
	t_thread		p_data;

	size = glitch.header.width * glitch.header.height * 3;
	data = glitch.copy;
	y = (glitch.negative_height == -1) ? y : glitch.header.height - 1 - y;
	/*Le random permet a l'algo de remplacer le pixel de départ qui est du coup different de la couleur de départ*/
	start.B = (data[3 * ((y * glitch.header.width) + x) + 0] + (rand() % 2)) % 255;
	start.G = (data[3 * ((y * glitch.header.width) + x) + 1] + (rand() % 2)) % 255;
	start.R = (data[3 * ((y * glitch.header.width) + x) + 2] + (rand() % 2)) % 255;
	p_data.glitch = &glitch;
	p_data.x = x;
	p_data.y = y;
	p_data.size = size;
	p_data.start = &start;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	err = pthread_attr_setstacksize(&attr, 4096 * 10000);
	if (err != 0)
		printf("Error setting stacksize\n", err);
	err = pthread_create(&threads[0], &attr, launch_recurs, &p_data);
	if (err != 0)
		printf("Error creating thread\n");
	err = pthread_join(threads[0], NULL);
	if (err != 0)
		printf("Error joining thread\n");
	pthread_attr_destroy(&attr);
	printf("Pixel %dx%d replaced by #%02X%02X%02X (%d, %d, %d) \n", x, y, start.R, start.G, start.B, start.R, start.G, start.B);
}
