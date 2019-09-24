/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 17:36:48 by vdelsie           #+#    #+#             */
/*   Updated: 2019/09/11 17:37:58 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include <mlx.h>
#include "fdf.h"

static void	cleanup_env(t_envars *env)
{
	free((&env->map)->verts);
	free((&env->map)->xverts);
	mlx_destroy_image(env->mlx, (&env->image)->image);
}

static void	reset_env(t_envars *env)
{
	env->x_shift = 0;
	env->y_shift = 0;
	env->z_shift = 0;
	env->x_rot = 0;
	env->y_rot = 0;
	env->z_rot = 0;
	env->x_sca = 25;
	env->y_sca = 25;
	env->z_sca = 25;
}

/*

** keydown_hook - аналогичен key_pressed_hook, но вызывается только когда
** клавиша была нажата И отпущена.
**
** Следующий ключ сопоставлен:
** 'r' (15) = вернуть карту к исходному масштабированию, поворотам и сдвигам
** 'esc' (53) = выйти из программы
** 'p' (35) = точечный режим (включает или выключает рисование линий)
** 'q' (12) = нет режима zbuffer (включает или выключает zbuffer)
** 'c' (8) = переключение режимов цвета
*/

int			keydown_hook(int key, t_envars *env)
{
	if (key == 53)
	{
		cleanup_env(env);
		exit(0);
	}
	if (key == 35)
		env->point_mode = (env->point_mode) ? 0 : 1;
	if (key == 12)
		env->no_zbuf_mode = (env->no_zbuf_mode) ? 0 : 1;
	if (key == 8)
	{
		env->color_style = (env->color_style + 1) % 5;
		toggle_colors(env);
	}
	if (key == 15)
	{
		reset_env(env);
	}
	render(env);
	return (0);
}

/*
** key_pressed_hook - когда любая из клавиш со стрелками на клавиатуре
** удерживая, сдвиньте карту в этом направлении.
**
** ← (123) = перевести х осталось на 5 единиц
** → (124) = перевести х вправо на 5 единиц
** ↓ (125) = перевести y вниз на 5 единиц
** ↑ (126) = перевести y вверх на 5 единиц
**
** z (6) = поворачивает ось z против часовой стрелки на 1 градус
** x (7) = поворачивает ось z по часовой стрелке на 1 градус
*/

int			key_pressed_hook(int key, t_envars *env)
{
	if (key == 124)
		env->x_shift += 5;
	if (key == 123)
		env->x_shift -= 5;
	if (key == 126)
		env->y_shift -= 5;
	if (key == 125)
		env->y_shift += 5;
	if (key == 6)
	{
		env->z_rot += (M_PI / 360);
		env->z_rot = fmod(env->z_rot, 360);
	}
	if (key == 7)
	{
		env->z_rot -= (M_PI / 360);
		env->z_rot = fmod(env->z_rot, 360);
	}
	render(env);
	return (0);
}
