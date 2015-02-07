/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/07 16:45:41 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/07 18:01:17 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

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
	unsigned char	buff[255];
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
	write(fd_out, &header, sizeof(t_bitmap));
	while ((ret = read(fd_in, buff, 255)))
	{
		if (rand() % 5)
		{
			i = 0;
			while (i < ret)
			{
				buff[i] %= (rand() % 10) + 1;
				buff[i] -= 2;
				i++;
			}
		}
		write(fd_out, buff, ret);
	}
	close(fd_in);
	close(fd_out);
	return 0;
}