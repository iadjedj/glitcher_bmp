/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 16:33:08 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/11 17:34:32 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

t_header		ft_check_header(const int fd_in, const int fd_out)
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

void		put_error(char *str)
{
	perror(str);
	exit(1);
}

int 		main(int ac, char **av)
{
	int				fd_in;
	int				fd_out;
	t_mlx			mlx;

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
	mlx.fd_out = fd_out;
	mlx.header = ft_check_header(fd_in, fd_out);
	mlx.source = ft_get_data(fd_in, mlx.header);
	mlx.env = start_mlx(mlx.header);
	mlx.copy = (unsigned char *)malloc(sizeof(unsigned char) * mlx.header.width * mlx.header.height * 10);

	mlx.copy = memcpy(mlx.copy, mlx.source, mlx.header.width * mlx.header.height * 4);
	data_to_img(mlx);
	mlx_string_put(mlx.env.mlx, mlx.env.win, 20, 20, 0xC73920, "Propagande pour un monde meilleur (Collaboration 42 / Penninghen)");
	mlx_string_put(mlx.env.mlx, mlx.env.win, 40, 45, 0x999999, "Up/Down :       Glitch strength");
	mlx_string_put(mlx.env.mlx, mlx.env.win, 40, 65, 0x999999, "Left/Right :    Glitch size");
	mlx_string_put(mlx.env.mlx, mlx.env.win, 40, 85, 0x999999, "+/- :           Noise Level");
	mlx_string_put(mlx.env.mlx, mlx.env.win, 40, 105, 0x999999, "Space :         Save Image");
	mlx_string_put(mlx.env.mlx, mlx.env.win, 40, 125, 0x999999, "Echap :         Exit");
	sleep(2);
	// mlx_put_image_to_window(mlx.env.mlx, mlx.env.win, mlx.env.img, 0, 0);

	/* pour gerer le repeat sur les touches*/
	// mlx_hook(mlx.env.win, 2, (1L << 0), key_hook, &mlx);
	mlx_key_hook(mlx.env.win, key_hook, &mlx);
	mlx_expose_hook(mlx.env.win, expose_hook, &mlx);
	mlx_loop(mlx.env.mlx);
	free(mlx.copy);
	free(mlx.source);
	close(fd_in);
	close(fd_out);
	return (0);
}
