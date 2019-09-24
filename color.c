/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 17:44:29 by vdelsie           #+#    #+#             */
/*   Updated: 2019/09/11 17:45:49 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
** ft_lint - Линейная интерполяция между двумя точками с учетом десятичного процента
**
** Мы не делаем: (start + decimal_percent * (end - start)), потому что если
** начало и конец существенно различаются по величине, тогда мы теряем точность.
**
** Тест с: ft_lint (-16.0e30, 16.0, 1.0)
*/

double		ft_lint(double start, double end, double decimal_percent)
{
	if (start == end)
		return (start);
	return (start * (1.0 - decimal_percent) + (end * decimal_percent));
}

/*
** color_lint - генерирует промежуточный цвет между c1 и c2.
**
** Цвета хранятся в следующем шестнадцатеричном формате:
** 0 х | F F | F F | F F |
** | красный | зеленый | синий |
**
** Итак, что мы можем сделать, так это замаскировать и изолировать каждый цветовой канал, используя сдвиг битов
** и выполнить линейную интерполяцию на каждом канале перед рекомбинацией.
**
** Каждый канал 8 бит.
*/

int			color_lint(int c1, int c2, double decimal_percent)
{
	int	r;
	int	g;
	int	b;

	if (c1 == c2)
		return (c1);
	if (decimal_percent == 0.0)
		return (c1);
	if (decimal_percent == 1.0)
		return (c2);
	r = (int)ft_lint(((c1 >> 16) & 0xFF), ((c2 >> 16) & 0xFF), decimal_percent);
	g = (int)ft_lint(((c1 >> 8) & 0xFF), ((c2 >> 8) & 0xFF), decimal_percent);
	b = (int)ft_lint((c1 & 0xFF), (c2 & 0xFF), decimal_percent);
	return (r << 16 | g << 8 | b);
}

double		get_line_pcnt(double start, double end, double curr)
{
	if (curr == start)
		return (0.0);
	if (curr == end)
		return (1.0);
	if (start == end)
		return (0.0);
	return ((curr - start) / (end - start));
}

int			calc_vert_color(t_envars *env, t_map *map, t_vert curr_vert)
{
	int		color;
	double	divisor;
	double	dec_pcnt;

	divisor = map->max_z - map->min_z;
	if (divisor != 0)
		dec_pcnt = (curr_vert.z - map->min_z) / divisor;
	else
		dec_pcnt = 0.0;
	color = color_lint(env->c_min, env->c_max, dec_pcnt);
	return (color);
}

/*
** toggle_colors
** стиль 0: ваниль
** стиль 1: красный -> белый
** стиль 2: зеленый -> белый
** стиль 3: синий -> белый
** стиль 4: Рождество
*/

void		toggle_colors(t_envars *env)
{
	if (env->color_style == 0)
	{
		env->c_max = 0xFFFFFF;
		env->c_min = 0xFFFFFF;
	}
	if (env->color_style == 1)
	{
		env->c_max = 0xFFFFFF;
		env->c_min = 0xFF0000;
	}
	if (env->color_style == 2)
	{
		env->c_max = 0xFFFFFF;
		env->c_min = 0x00FF00;
	}
	if (env->color_style == 3)
	{
		env->c_max = 0xFFFFFF;
		env->c_min = 0x0000FF;
	}
	if (env->color_style == 4)
	{
		env->c_max = 0xFF0000;
		env->c_min = 0x00FF00;
	}
}
