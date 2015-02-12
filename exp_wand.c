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

void			recurs(t_glitch *glitch, int line, int col, int size)
{
	unsigned char *data;

	data = glitch->copy;
	if (line < 0 || col < 0 || line > glitch->header.height - 1 || col > glitch->header.width - 1 || 
		!(data[3 * ((line * glitch->header.width) + col)] > 200 &&
			data[3 * ((line * glitch->header.width) + col) + 1] > 200 &&
			data[3 * ((line * glitch->header.width) + col) + 2] > 200))
	{
		return ;
	}
	data[3 * ((line * glitch->header.width) + col)] = 0;
	data[3 * ((line * glitch->header.width) + col) + 1] = 0;
	data[3 * ((line * glitch->header.width) + col) + 2] = 100;
	recurs(glitch, line + 1, col, size);
	recurs(glitch, line - 1, col, size);
	recurs(glitch, line, col + 1, size);
	recurs(glitch, line, col - 1, size);
}

void			ft_glitch(t_glitch glitch, int equiv_size, int equiv, int noise)
{
	int				size;

	size = glitch.header.width * glitch.header.height * 3;
	recurs(&glitch, 150, 150, size);
}
