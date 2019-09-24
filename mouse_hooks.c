/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/17 14:16:38 by nmei              #+#    #+#             */
/*   Updated: 2019/09/11 17:43:21 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_mouse(t_envars *env)
{
	t_mouse		*new_mouse;

	new_mouse = &env->mouse;
	new_mouse->is_pressed = 0;
	new_mouse->prev_x = 0;
	new_mouse->prev_y = 0;
	new_mouse->x = 0;
	new_mouse->y = 0;
}

/*
** mouse_pressed_hook - ловушка для события нажатия мыши, которое передается
** mlx_hook (). mlx_hook () передаст "int button, int x, int y", а затем
** любые параметры также передаются в mlx_hook () функции, когда мышь
** нажата.
**
** Кнопка может принимать несколько значений !!!
** 1 = щелчок левой кнопкой мыши
** 2 = щелчок правой кнопкой мыши
** 3 = ????? (может быть, нажав колесо прокрутки ???)
** 4 = колесо прокрутки мыши вниз
** 5 = колесо прокрутки мыши вверх
** 6 = колесо прокрутки мыши вправо
** 7 = колесо прокрутки мыши влево
**
** ПРИМЕЧАНИЕ: все координаты x и y, сообщаемые функциям подключения мыши, будут
** иметь (0, 0) как нижний левый угол окна. (WIN_W, WIN_H) будет
** таким образом, будет верхний правый угол окна ... что совершенно другое
** из изображения (0,0), которое является верхним левым углом ... перейти к рисунку ...
**
** ПРИМЕЧАНИЕ: НЕ ПЫТАЙТЕСЬ ИЗМЕНИТЬ ТИПЫ ПАРАМЕТРОВ X & Y ОТ INT ДЛЯ ДВОЙНОГО !!
** Это повредит адресацию ваших параметров и даст вам segfaults
** это займет у вас, возможно, несколько часов для отладки (по крайней мере для меня ...)
*/

int		mouse_pressed_hook(int button, int x, int y, t_envars *env)
{
	if (x >= 0 && x <= WIN_W && y >= 0 && y <= WIN_H)
	{
		if (button == 1 || button == 2)
		{
			(&env->mouse)->is_pressed = 1;
			(&env->mouse)->x = x;
			(&env->mouse)->y = y;
		}
		if (button == 4)
		{
			env->x_sca -= 1;
			env->y_sca -= 1;
			env->z_sca -= 1;
		}
		if (button == 5)
		{
			env->x_sca += 1;
			env->y_sca += 1;
			env->z_sca += 1;
		}
		if (button == 4 || button == 5)
			render(env);
	}
	return (0);
}

/*
** ПРИМЕЧАНИЕ: НЕ ПЫТАЙТЕСЬ ИЗМЕНИТЬ ТИПЫ ПАРАМЕТРОВ X & Y ОТ INT ДЛЯ ДВОЙНОГО !!
*/

int		mouse_released_hook(int button, int x, int y, t_envars *env)
{
	t_mouse		*mouse;

	mouse = &env->mouse;
	(void)button;
	(void)x;
	(void)y;
	mouse->is_pressed = 0;
	return (0);
}

/*
** ПРИМЕЧАНИЕ: НЕ ПЫТАЙТЕСЬ ИЗМЕНИТЬ ТИПЫ ПАРАМЕТРОВ X & Y ОТ INT ДЛЯ ДВОЙНОГО !!
*/

int		mouse_moved_hook(int x, int y, t_envars *env)
{
	t_mouse		*mouse;

	mouse = &env->mouse;
	if (x >= 0 && x <= WIN_W && y >= 0 && y <= WIN_H)
	{
		mouse->prev_x = mouse->x;
		mouse->prev_y = mouse->y;
		mouse->x = x;
		mouse->y = y;
		if (mouse->is_pressed)
		{
			env->y_rot -= (mouse->prev_x - x) / 150;
			env->x_rot += (mouse->prev_y - y) / 150;
		}
		if (mouse->is_pressed)
			render(env);
	}
	return (0);
}
