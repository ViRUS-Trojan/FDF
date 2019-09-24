/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 18:28:37 by vdelsie           #+#    #+#             */
/*   Updated: 2019/09/08 18:30:17 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
**	idx () - Полезная функция для получения строки, значения col из сплющенного
**	2D матрица. Dim - это ширина матрицы ...
*/

int		idx(int row, int col, int dim)
{
	return (row * dim + col);
}

void	init_vert(t_vert *new_vert)
{
	new_vert->x = 0;
	new_vert->y = 0;
	new_vert->z = 0;
	new_vert->w = 0;
	new_vert->color = 0;
}

/*
** Вычислить скалярное произведение между матрицей преобразования 4 x 4 *
** (который мы представляем как двойной элемент из 16 элементов) и
** 4 x 1 * vector *, который мы представляем в виде структуры t_vert.
**
** Много циклов раскрутки для (обнадеживающих) скоростных оптимизаций ...
**
** Эта матричная функция является гиперспециализированной для этого проекта, поэтому ...
** НЕ ИСПОЛЬЗУЙТЕ ДЛЯ МУЛЬТИПЛИКАЦИИ ОБЩЕГО НАЗНАЧЕНИЯ MATRIX-VECTOR
** У тебя будет плохое время ...
*/

t_vert	m_v_mult(t_trmat m, t_vert vec)
{
	int		i;
	t_vert	new_vert;
	double	*new_vert_ind[4];

	init_vert(&new_vert);
	new_vert_ind[0] = &new_vert.x;
	new_vert_ind[1] = &new_vert.y;
	new_vert_ind[2] = &new_vert.z;
	new_vert_ind[3] = &new_vert.w;
	i = 0;
	while (i <= 2)
	{
		*new_vert_ind[i] += m.m[idx(i, 0, 4)] * vec.x;
		*new_vert_ind[i] += m.m[idx(i, 1, 4)] * vec.y;
		*new_vert_ind[i] += m.m[idx(i, 2, 4)] * vec.z;
		*new_vert_ind[i] += m.m[idx(i, 3, 4)] * vec.w;
		*new_vert_ind[i + 1] += m.m[idx(i + 1, 0, 4)] * vec.x;
		*new_vert_ind[i + 1] += m.m[idx(i + 1, 1, 4)] * vec.y;
		*new_vert_ind[i + 1] += m.m[idx(i + 1, 2, 4)] * vec.z;
		*new_vert_ind[i + 1] += m.m[idx(i + 1, 3, 4)] * vec.w;
		i += 2;
	}
	return (new_vert);
}
