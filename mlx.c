/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 16:34:02 by iadjedj           #+#    #+#             */
/*   Updated: 2015/02/11 17:33:12 by iadjedj          ###   ########.fr       */
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
	static int	equiv_size = 5;
	static int	equiv = 15;
	static int	noise = 0;
	int			tmp;

	if (keycode == ECHAP)
		exit(0);
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
		equiv += (keycode == DOWN) ? 5 : -5;
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
		tmp = mlx->header.width * mlx->header.height * 3;
		lseek(mlx->fd_out, mlx->header.dataoffSet, SEEK_SET);
		if (write(mlx->fd_out, mlx->copy, tmp) == tmp)
		{
			mlx_string_put(mlx->env.mlx, mlx->env.win, 15, 15, 0xFFFFFF, "File Saved");
			printf("File Saved\n");
		}
		else
		{
			mlx_string_put(mlx->env.mlx, mlx->env.win, 15, 15, 0xFF0000, "Error saving");
			printf("Error saving\n");
		}
		return (0);
	}
	mlx->copy = memcpy(mlx->copy, mlx->source, mlx->header.width * mlx->header.height * 4);
	ft_glitch(*mlx, equiv_size, equiv, noise);
	mlx_destroy_image(mlx->env.mlx, mlx->env.img);
	mlx->env.img = mlx_new_image(mlx->env.mlx, mlx->header.width, mlx->header.height);
	data_to_img(*mlx);
	mlx_put_image_to_window(mlx->env.mlx, mlx->env.win, mlx->env.img, 0, 0);
	printf("equiv_size : %d\t\tequiv : %d\tnoise : %d\n", equiv_size, equiv, noise);
	return (0);
}