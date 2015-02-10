/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/07 16:45:41 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/10 16:00:35 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <mlx.h>
#define BUFF_SIZE 1024

#pragma pack(push, 1)
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
#pragma pack(pop)


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
	int				pos;
	unsigned char	buff[BUFF_SIZE];
	unsigned char	*copy;
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
	if (header.signature != 0x4D42)
	{
		printf("%d\nAttention, l'input n\'est probablement pas un bmp\n", header.signature);
		exit(1);
	}
	write(fd_out, &header, sizeof(t_bitmap));
	header.height = abs(header.height);
	printf("Lignes : %d \nColonnes : %d\n", header.width, header.height);
	copy = (unsigned char *)malloc(sizeof(unsigned char) * header.width * header.height * 3);
	pos = 0;
	while ((ret = read(fd_in, buff, BUFF_SIZE)))
	{
		/*Remplacement des pixels similaires, la valeur est modifiable ci-dessous*/
		#define SIMIL 10
		i = 0;
		while (i + 5 <= 300)
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
			if (rand() % 10 == 0)
			{
				buff[i] = rand() % 100;
				buff[i + 1] = rand() % 100;
				buff[i + 2] = rand() % 100;
			}
			i += 3;
		}
		memcpy(copy + pos, buff, ret);
		pos += ret;
	}
	void	*mlx;
	void	*win;
	void	*img;
	char	*img_ptr;
	// int		depth, size_line, endian;

	mlx = mlx_init();
	win = mlx_new_window(mlx, header.width, header.height, "Propagande");
	img = mlx_new_image(mlx, header.width, header.height);
	if (img)
	{
		char			*idatas;
		int				bpx;
		int				linesize;
		int				indian;
		int i;
		int j;
		int k;
		int index;
		idatas =  mlx_get_data_addr(img, &bpx, &linesize, &indian);
		i = 0;
		k = 0;
		while ((int)i < header.height)
		{
			j = 0;
			k = (i) * (header.width * 3);
			while ((int)j < header.width)
			{
					index = i * linesize + ((j * bpx) >> 3);
					idatas[index] = copy[k++];
					idatas[++index] = copy[k++];
					idatas[++index] = copy[k++];
					j++;
			}
			i++;
		}
	}
	mlx_put_image_to_window(mlx, win, img, 0, 0);
	write(fd_out, copy, header.width * header.height * 3);
	mlx_loop(mlx);
	close(fd_in);
	close(fd_out);
	return 0;
}