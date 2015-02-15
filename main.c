/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 16:33:08 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/12 11:33:32 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void		ft_get_header(t_glitch *glitch)
{
	t_header		header;

	memset(&header, 0, sizeof(t_header));
	read(glitch->fd_in, &header, sizeof(t_header));
	if (header.signature != 0x4D42) // signature BMP
	{
		printf("Attention, l'input n\'est probablement pas un bmp\n");
		close(glitch->fd_in);
		exit(1);
	}
	glitch->negative_height = (header.height < 0) ? -1 : 1;
	header.height = abs(header.height);
	printf("Dimensions : %d*%d\n", header.width, header.height);
	glitch->header = header;
}

int 		main(int ac, char **av)
{
	t_glitch			glitch;

	if (ac != 2)
	{
		printf("Pas de fichier ou trop d'args\n");
		exit(1);
	}
	glitch.fd_in = open(av[1], O_RDONLY);
	if (glitch.fd_in == -1)
		put_error_and_exit(av[1]);
	glitch.fd_out = FD_OUT_INIT;
	ft_get_header(&glitch);
	glitch.source = ft_get_data(glitch.fd_in, glitch.header);
	glitch.env = start_mlx(glitch.header);
	glitch.copy = (unsigned char *)malloc(sizeof(unsigned char) * glitch.header.width * glitch.header.height * 10);
	memcpy(glitch.copy, glitch.source, glitch.header.width * glitch.header.height * 4);
	data_to_img(glitch);
	mlx_string_put(glitch.env.mlx, glitch.env.win, 20, 20, 0xC73920, "Propagande pour un monde meilleur (Collaboration 42 / Penninghen)");
	mlx_string_put(glitch.env.mlx, glitch.env.win, 40, 45, 0x999999, "Up/Down :       Glitch strength");
	mlx_string_put(glitch.env.mlx, glitch.env.win, 40, 65, 0x999999, "Left/Right :    Glitch size");
	mlx_string_put(glitch.env.mlx, glitch.env.win, 40, 85, 0x999999, "+/- :           Noise Level");
	mlx_string_put(glitch.env.mlx, glitch.env.win, 40, 105, 0x999999, "Space :         Save Image");
	mlx_string_put(glitch.env.mlx, glitch.env.win, 40, 125, 0x999999, "Echap :         Exit");
	sleep(2);

	/* pour gerer le repeat sur les touches*/
	// mlx_hook(glitch.env.win, 2, (1L << 0), key_hook, &glitch);
	mlx_key_hook(glitch.env.win, key_hook, &glitch);
	mlx_mouse_hook(glitch.env.win, mouse_hook, &glitch);
	mlx_expose_hook(glitch.env.win, expose_hook, &glitch);
	mlx_loop(glitch.env.mlx);
	free(glitch.copy);
	free(glitch.source);
	close(glitch.fd_in);
	close(glitch.fd_out);
	return (0);
}
