/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 20:12:16 by vdelsie           #+#    #+#             */
/*   Updated: 2019/09/11 17:29:46 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mlx.h>
#include <libft.h>
#include "fdf.h"

static void		setup_env(t_envars *env)
{
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, WIN_W, WIN_H, "FdF 21");
	init_img(env);
	init_mouse(env);
	env->x_shift = 0;
	env->y_shift = 0;
	env->z_shift = 0;
	env->x_rot = 0;
	env->y_rot = 0;
	env->z_rot = 0;
	env->x_sca = 25;
	env->y_sca = 25;
	env->z_sca = 25;
	env->color_style = 0;
	env->c_max = 0xFFFFFF;
	env->c_min = 0xFFFFFF;
	env->point_mode = 0;
	env->no_zbuf_mode = 0;
}

static void		disp_usage(void)
{
	ft_putstr("\n						Меню Fdf 21!!\n\
				Щелкните мышью и перетащите: поверните оси X / Y  \n \
				Прокрутка мышью вверх / вниз: увеличение / уменьшение  \n \
				Клавиши со стрелками: Перевести оси X / Y  \n \
				Клавиши 'z / x': вращение оси z  \n \
				c: изменить цветовые режимы  \n \
				p: включить / выключить только режим точки  \n \
				q: включить / выключить Z-буферизацию  \n \
				r: сбросить переводы, повороты и масштабирование карты  \n \
				esc: Quit: ( \n  \n ");
}

int				main(int argc, char *argv[])
{	
	void	*mlx_ptr;
	void	*win_mlx;
	t_envars	env;
	int			fd;

	mlx_ptr = mlx_init();
	win_mlx = mlx_new_window(mlx_ptr, 500, 500, "21");

	if (argc != 2)
	{
		write(2, "usage: ./fdf [target_file.fdf]\n", 31);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0 || read_file(fd, &(env.map)) != 0)
	{
		write(2, "An error occurred reading the file... Is it valid?\n", 51);
		return (1);
	}



	close(fd);
	setup_env(&env);
	disp_usage();
	render(&env);
	mlx_key_hook(env.win, keydown_hook, &env);
	mlx_hook(env.win, 2, 0, key_pressed_hook, &env);
	mlx_hook(env.win, 4, 0, mouse_pressed_hook, &env);
	mlx_hook(env.win, 5, 0, mouse_released_hook, &env);
	mlx_hook(env.win, 6, 0, mouse_moved_hook, &env);
	mlx_loop(env.mlx);
	return (0);
}
