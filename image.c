/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/07 16:45:41 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/10 22:34:58 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void		put_error(char *str)
{
	perror(str);
	exit(1);
}

t_header	ft_check_header(const int fd_in, const int fd_out)
{
	t_header		header;

	memset(&header, 0, sizeof(t_header));
	read(fd_in, &header, sizeof(t_header));
	if (header.signature != 0x4D42) // signature BMP
	{
		printf("Attention, l'input n\'est probablement pas un bmp\n");
		close(fd_in);
		close(fd_out);
		exit(1);
	}
	write(fd_out, &header, sizeof(t_header)); /* Copie du Header dans le fichier output */
	header.height = abs(header.height);
	printf("Dimensions : %d*%d\n", header.width, header.height);
	return (header);
}

unsigned char	*ft_get_data(const int fd_in, const t_header header)
{
	unsigned char	buff[BUFF_SIZE];
	unsigned char	*data;
	int pos;
	int ret;

	data = (unsigned char *)malloc(sizeof(unsigned char) * header.width * header.height * 3);
	pos = 0;
	srand(time(NULL));
	while ((ret = read(fd_in, buff, BUFF_SIZE)))
	{
		memcpy(data + pos, buff, ret);
		pos += ret;
	}
	return (data);
}

void			ft_glitch(unsigned char **data, t_header header, int equiv_size, int equiv)
{
	unsigned char	*buff;
	int				i;
	int				tmp;
	
	buff = *data;

	/* Remplacement des pixels similaires */
	i = 0;
	while (i < header.width * header.height * 3)
	{
		tmp = 0;
		while (tmp + 5 <= equiv_size)
		{
			if (abs(buff[tmp + i] - buff[tmp + i + 3]) < equiv &&
			abs(buff[tmp + i + 1] - buff[tmp + i + 4]) < equiv &&
			abs(buff[tmp + i + 2] - buff[tmp + i + 5]) < equiv)
			{
				buff[tmp + i]  = buff[i + 0];
				buff[tmp + i + 1] = buff[i + 1];
				buff[tmp + i + 2] = buff[i + 2];
			}
			tmp += 3;
		}
		i += tmp;
	}

	/* Generation de bruit aleatoire, la valeur de 100 est pour eviter le cote carnaval */
	i = 0;
	while (i < header.width * header.height * 3)
	{
		if (rand() % 10 == 0)
		{
			buff[i] = rand() % 100;
			buff[i + 1] = rand() % 100;
			buff[i + 2] = rand() % 100;
		}
		i += 3;
	}
}

void		data_to_img(unsigned char *data, t_env env, t_header header)
{
	char			*img_data;
	int				bpx;
	int				linesize;
	int				endian;
	int				i;
	int				j;
	int				k;
	int				index;

	if (env.img)
	{
		img_data = mlx_get_data_addr(env.img, &bpx, &linesize, &endian);
		i = 0;
		k = 0;
		while ((int)i < header.height)
		{
			j = 0;
			k = (i) * (header.width * 3);
			while ((int)j < header.width)
			{
				index = i * linesize + ((j * bpx) >> 3);
				img_data[index] = data[k++];
				img_data[++index] = data[k++];
				img_data[++index] = data[k++];
				j++;
			}
			i++;
		}
	}
}

t_env		mlx_start(t_header header)
{
	t_env			env;

	if ((env.mlx = mlx_init()) == NULL ||
		(env.win = mlx_new_window(env.mlx,
			header.width, header.height, "Propagande")) == NULL ||
		(env.img = mlx_new_image(env.mlx, header.width, header.height)) == NULL)
	{
		printf("Probleme avec la libmlx\n");
		exit (1);
	}
	return (env);
}

int				expose_hook(t_mlx *mlx)
{
	mlx_put_image_to_window(mlx->env.mlx, mlx->env.win, mlx->env.img, 0, 0);
	return (0);
}

int				key_hook(int keycode, t_mlx *mlx)
{
	if (keycode == ECHAP)
		exit(0);
	// else if (keycode == DOWN)
	// else if (keycode == UP)
	// mlx_destroy_image(mlx->env.mlx, mlx->env.img);
	// mlx->env.img = mlx_new_image(mlx->env.mlx, mlx->header.width, mlx->header.height);
	expose_hook(mlx);
	return (0);
}

int 		main(int ac, char **av)
{
	int				fd_in;
	int				fd_out;
	unsigned char	*data;
	unsigned char	*copy;
	t_mlx			mlx;
	t_header		header;
	t_env			env;

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
	header = ft_check_header(fd_in, fd_out);
	data = ft_get_data(fd_in, header);
	env = mlx_start(header);
	mlx.env = env;
	mlx.header = header;


	copy = (unsigned char *)malloc(sizeof(unsigned char) * header.width * header.height * 3);

		copy = memcpy(copy, data, header.width * header.height * 3);
		ft_glitch(&copy, header, 200, 20);
		data_to_img(copy, env, header);
		
	write(fd_out, copy, header.width * header.height * 3);
	mlx_key_hook(env.win, key_hook, &mlx);
	mlx_expose_hook(env.win, expose_hook, &mlx);
	mlx_loop(env.mlx);
	free(copy);
	free(data);
	close(fd_in);
	close(fd_out);
	return 0;
}