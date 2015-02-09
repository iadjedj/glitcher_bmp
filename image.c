/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/07 16:45:41 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/09 14:37:50 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#define BUFF_SIZE 300

typedef struct					s_bitmap
{
	short						signature;
	int							size;
	short						reserved1;
	short						reserved2;
	int							dataoffSet;
	int							bitmapheadersize;
	int							width;
	int							height;
	short						planes;
	short						bitsperpixel;
	int							compression;
	int							sizeImage;
	int							xpixelspremeter;
	int							ypixelspremeter;
	int							colorsused;
	int							colorsimportant;
}								t_bitmap;

void	put_error(char *str)
{
	perror(str);
	exit(1);
}

int main(int ac, char **av)
{
	int				fd_in;
	int				fd_out;
	int				i;
	int				ret;
	unsigned char	buff[BUFF_SIZE];
	t_bitmap		header;

	srand(time(NULL));
	if (ac != 2)
	{
		printf("Pas de fichier ou trop d'args\n");
		exit(1);
	}
	fd_in = open(av[1], O_RDONLY);
	if (fd_in == -1)
		put_error(av[1]);
	fd_out = open("out.bmp", O_CREAT | O_WRONLY | O_TRUNC, 0755);
	if (fd_out == -1)
		put_error("out.bmp");
	memset(&header, 0, sizeof(t_bitmap));
	ret = read(fd_in, &header, sizeof(t_bitmap));
	if (header.signature != 19778)
	{
		printf("Attention, l'input n\'est probablement pas un bmp\n");
		exit(1);
	}
	write(fd_out, &header, sizeof(t_bitmap));
	while ((ret = read(fd_in, buff, BUFF_SIZE)))
	{
		/*Remplacement des pixels similaires, la valeur est modifiable ci-dessous*/
		#define SIMIL 50
		i = 0;
		while (i + 5 <= ret)
		{
			if (abs(buff[i] - buff[i + 3]) < SIMIL &&
			abs(buff[i + 1] - buff[i + 4]) < SIMIL &&
			abs(buff[i + 2] - buff[i + 5]) < SIMIL)
			{
				buff[i]  = buff[0];
				buff[i + 1] = buff[1];
				buff[i + 2] = buff[2];
			}
			i += 3;
		}
		/*Generation de bruit aleatoire, la valeur de 100 est pour eviter le cote carnaval*/
		i = 0;
		while (i + 2 <= ret)
		{
			if (rand() % 5 == 0)
			{
				buff[i] = rand() % 100;
				buff[i + 1] = rand() % 100;
				buff[i + 2] = rand() % 100;
			}
			i += 3;
		}
		write(fd_out, buff, ret);
	}
	close(fd_in);
	close(fd_out);
	return 0;
}