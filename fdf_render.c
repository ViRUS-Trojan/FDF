/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 19:27:02 by vdelsie           #+#    #+#             */
/*   Updated: 2019/09/11 17:43:24 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include "fdf.h"

/*
** xform () - где происходит волшебство ...
** Дана карта с набором вершин (x, y, z, w)
** 1) переведите карту так, чтобы * CENTER * карты был @ (0,0).
** 2) Равномерно масштабируйте x, y и z (для эффектов увеличения и уменьшения)
** 3-5) применить повороты в порядке осей x, y, затем по оси z
** 4) Теперь сместим центр нашей карты @ (0,0) к середине
** окно рисования, т.е. (WIN_W / 2, WIN_H / 2)
** 5) Если мы хотим перевести карту вокруг, начиная с середины
** нашего окна, затем примените эти преобразования.
** 6) Рассчитайте цвет нашей результирующей вершины
**
** Примечание: вычисление цвета вершины не обязательно должно происходить в этом
** функция, но может быть полезна, если я решу реализовать цвет на основе
** преобразованные значения Z вместо текущей реализации, где цвет
** на основе нетрансформированных значений Z.
*/

static t_vert	xform(t_envars *env, t_map *map, int row, int col)
{
	t_vert	res;
	t_vert	curr_vert;
	double	mid_map_w;
	double	mid_map_h;
	double	mid_map_z;

	mid_map_w = (double)(map->width - 1) / 2;
	mid_map_h = (double)(map->height - 1) / 2;
	mid_map_z = (double)(map->max_z + map->min_z) / 2;
	curr_vert = map->verts[idx(row, col, map->width)];
	res = m_v_mult(trans_mat(-mid_map_w, -mid_map_h, -mid_map_z), curr_vert);
	res = m_v_mult(scale_mat(env->x_sca, env->y_sca, env->z_sca), res);
	res = m_v_mult(x_rot_mat(env->x_rot, 0), res);
	res = m_v_mult(y_rot_mat(env->y_rot, 0), res);
	res = m_v_mult(z_rot_mat(env->z_rot, 0), res);
	res = m_v_mult(trans_mat((WIN_W / 2), (WIN_H / 2), 0), res);
	res = m_v_mult(trans_mat(env->x_shift, env->y_shift, env->z_shift), res);
	res.color = calc_vert_color(env, map, curr_vert);
	return (res);
}

/*
** init_line () - инициализировать переменные, необходимые для алгоритма рисования линий Брезенхэма
** Потому что p1 -> p2 может быть линией со многими различными положительными или отрицательными
** Склоны нам нужно учитывать все 8 возможностей.
**
** 1) Возьмите абсолютное значение dx и dy в случае, когда x1 / y1
** меньше чем х0 / у0
** 2) Если x0 <x1, то мы движемся вправо, поэтому сумма, которую мы добавляем к x, равна +1
** Если x0> x1, то мы двигаемся влево, поэтому сумма, которую мы добавляем к x, равна -1
** 3) Если y0 <y1, то мы движемся вниз, поэтому сумма, которую мы добавляем к y, равна +1
** Если y0> y1, тогда мы движемся вверх, поэтому сумма, которую мы добавляем к y, равна -1
** 4) Если dx> dy, то у нас есть более плоская линия, поэтому dx является ведущей осью.
** 5) Если dy> dx, то у нас более крутая линия, поэтому сделайте dy ведущей осью.
**
** Примечание: вы можете подумать, что 3) ошибается в направлениях, но помните, что в
** графическая система координат 0,0 вверху слева и 0, WIN_H внизу
**	оставил.
*/

static t_line	init_line(t_vert p1, t_vert p2)
{
	t_line	nl;

	nl.x0 = (int)p1.x;
	nl.y0 = (int)p1.y;
	nl.x1 = (int)p2.x;
	nl.y1 = (int)p2.y;
	nl.dx = ft_abs(nl.x1 - nl.x0);
	nl.sx = (nl.x0 < nl.x1) ? 1 : -1;
	nl.dy = ft_abs(nl.y1 - nl.y0);
	nl.sy = (nl.y0 < nl.y1) ? 1 : -1;
	nl.err = ((nl.dx > nl.dy) ? nl.dx : -nl.dy) / 2;
	nl.dec_pcnt = 0.0;
	return (nl);
}

/*
** draw_line () - int ver. алгоритма рисования линий Брезенхема между p1 и p2
** 1) Инициализация переменных для рисования линий (см. Выше)
** 2) определить для нашего преобразованного х0, у0, как далеко мы до нашей цели, чтобы получить
** процент строки.
** 3) Используйте процент строк, определенный в 2) для линейной интерполяции цвета
** 4) Проверьте, не перекрыт ли наш новый пиксель, и нарисуйте его, если нет.
** 5) Если наш термин ошибки> -dx, мы переместились (вправо или влево) и нам нужно
** обновить нашу позицию х0 соответственно.
** 6) Если наш термин ошибки <dy, мы переместились (вверх или вниз) и должны обновить
** наша позиция у0 соответственно.
*/

static void		draw_line(t_envars *env, t_vert p1, t_vert p2)
{
	t_line	l;

	l = init_line(p1, p2);
	while (1)
	{
		l.dec_pcnt = (l.dx > l.dy) ? get_line_pcnt(p1.x, p2.x, l.x0)
					: get_line_pcnt(p1.y, p2.y, l.y0);
		l.color = color_lint(p1.color, p2.color, l.dec_pcnt);
		if (is_occluded(l, env, p1, p2) == 0)
			img_pixel_put(&env->image, l.x0, l.y0, l.color);
		if (l.x0 == l.x1 && l.y0 == l.y1)
			break ;
		l.err2 = l.err;
		if (l.err2 > -l.dx)
		{
			l.err -= l.dy;
			l.x0 += l.sx;
		}
		if (l.err2 < l.dy)
		{
			l.err += l.dx;
			l.y0 += l.sy;
		}
	}
}

/*
** position_verts () - учитывая карту и набор преобразований, которые мы хотим
** чтобы сделать это, эта функция будет применять нужные преобразования.
** преобразования будут применяться, начиная с нижнего правого угла
**	карта.
**
** Будущее улучшение будет заключаться в параллельном позиционировании
** вершины, так как задача смущающе параллельна ... (хотя только если
** мы действительно очень нуждаемся в производительности)
*/

static void		position_verts(t_envars *env)
{
	t_vert	res;
	int		row;
	int		col;
	int		map_w;

	map_w = env->map.width;
	row = env->map.height;
	while (--row > -1)
	{
		col = map_w;
		while (--col > -1)
		{
			res = xform(env, &env->map, row, col);
			env->map.xverts[idx(row, col, map_w)] = res;
			img_pixel_put(&env->image, res.x, res.y, res.color);
		}
	}
}

/*
** render () - мы будем называть это много
** 1) Очистить существующее изображение
** 2) Очистить наши значения z-буфера (только если мы на самом деле Z-буферизация)
** 3) Применить преобразования ко всем нашим вершинам
** 4) Перебрать все наши преобразованные вершины, начиная с нижнего правого
** угол. Если мы можем, нарисуйте линию между нашей текущей xformed вершины и
** вершины справа и снизу в нашей преобразованной сетке вершин.
** 5) Теперь поместите полученное изображение в окно.
*/

void			render(t_envars *env)
{
	t_vert	res;
	int		row;
	int		col;
	int		map_w;

	clear_img(&env->image);
	if (env->no_zbuf_mode == 0)
		clear_zbuf(&env->map);
	position_verts(env);
	map_w = env->map.width;
	row = env->map.height;
	while (--row > -1)
	{
		col = map_w;
		while (--col > -1)
		{
			res = env->map.xverts[idx(row, col, map_w)];
			if (row + 1 < env->map.height)
				draw_line(env, res, env->map.xverts[idx(row + 1, col, map_w)]);
			if (col + 1 < map_w)
				draw_line(env, res, env->map.xverts[idx(row, col + 1, map_w)]);
		}
	}
	mlx_put_image_to_window(env->mlx, env->win, env->image.image, 0, 0);
}
