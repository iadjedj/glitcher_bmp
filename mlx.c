/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 16:34:02 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/12 11:36:56 by iadjedj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

t_env			start_mlx(t_header header)
{
	t_env			env;

	if ((env.mlx = mlx_init()) == NULL ||
		(env.win = mlx_new_window(env.mlx,
			header.width, header.height, "Propagande")) == NULL ||
		(env.img = mlx_new_image(env.mlx, header.width, header.height)) == NULL)
	{
		printf("Probleme avec la minilibx\n");
		exit (1);
	}
	return (env);
}

int				expose_hook(t_glitch *glitch)
{
	mlx_put_image_to_window(glitch->env.mlx, glitch->env.win, glitch->env.img, 0, 0);
	return (0);
}

int				mouse_hook(int button, int x, int y, t_glitch *glitch)
{
	if (button != 1)
		return (1);
	// glitch->copy = memcpy(glitch->copy, glitch->source, glitch->header.width * glitch->header.height * 4);
	ft_wand(*glitch, x, y);
	// mlx_destroy_image(glitch->env.mlx, glitch->env.img);
	glitch->env.img = mlx_new_image(glitch->env.mlx, glitch->header.width, glitch->header.height);
	data_to_img(*glitch);
	mlx_put_image_to_window(glitch->env.mlx, glitch->env.win, glitch->env.img, 0, 0);
	return (0);
}

int				key_hook(int keycode, t_glitch *glitch)
{
	static int	equiv_size = 5;
	static int	equiv = 15;
	static int	noise = 0;
	int			tmp;

	if (keycode == ECHAP)
		clean_exit(glitch);
	else if (keycode != DOWN && keycode != UP && keycode != LEFT && keycode != RIGHT &&
			keycode != PLUS && keycode != MINUS && keycode != SPACE)
		return (1);
	else if (keycode == LEFT || keycode == RIGHT)
	{
		equiv_size += (keycode == RIGHT) ? 5 : -5;
		if (equiv_size < 5)
			equiv_size = 5;
	}
	else if (keycode == DOWN || keycode == UP)
	{
		equiv += (keycode == DOWN) ? 3 : -3;
		if (equiv < 0)
			equiv = 0;
		else if (equiv > 170)
			equiv = 170;
	}
	else if (keycode == PLUS || keycode == MINUS)
	{
		noise += (keycode == PLUS) ? 5 : -5;
		if (noise > 100)
			noise = 100;
		else if (noise <= 0)
			noise = 0;
	}
	else if (keycode == SPACE)
	{
		if (glitch->fd_out == FD_OUT_INIT)
		{
			glitch->fd_out = open("out.bmp", O_CREAT | O_WRONLY | O_TRUNC, 0755);
			if (glitch->fd_out == -1)
				put_error_and_exit("out.bmp");
			/* Si dans le header d'origine height etait negatif, le remettre a sa valeur originale avant la copie*/
			glitch->header.height *= glitch->negative_height;
			/* Copie du Header dans le fichier output */
			write(glitch->fd_out, &(glitch->header), sizeof(t_header));
			glitch->header.height *= glitch->negative_height;
		}
		lseek(glitch->fd_out, glitch->header.dataoffSet, SEEK_SET);
		tmp = glitch->header.width * glitch->header.height * 3;
		if (write(glitch->fd_out, glitch->copy, tmp) == tmp)
		{
			mlx_string_put(glitch->env.mlx, glitch->env.win, 15, 15, 0xFFFFFF, "File Saved");
			printf("File Saved\n");
		}
		return (0);
	}
	glitch->copy = memcpy(glitch->copy, glitch->source, glitch->header.width * glitch->header.height * 4);
	ft_glitch(*glitch, equiv_size, equiv, noise);
	// mlx_destroy_image(glitch->env.mlx, glitch->env.img);
	glitch->env.img = mlx_new_image(glitch->env.mlx, glitch->header.width, glitch->header.height);
	data_to_img(*glitch);
	mlx_put_image_to_window(glitch->env.mlx, glitch->env.win, glitch->env.img, 0, 0);
	printf("equiv_size : %d\t\tequiv : %d\tnoise : %d\n", equiv_size, equiv, noise);
	return (0);
}