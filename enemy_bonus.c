/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrollon- <jrollon-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:44:09 by jrollon-          #+#    #+#             */
/*   Updated: 2025/09/10 16:05:55 by jrollon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*just one enemy 'x' allowed, if more return 1, else 0*/
int	enemy_position(t_game *game)
{
	size_t	x;
	size_t	y;

	x = 0;
	y = 0;
	while (y < game->map->lines)
	{
		x = 0;
		while (x < game->map->columns)
		{
			if (game->map->map[y][x] == 'x')
			{
				game->enemy.num_enemies++;
				if (game->enemy.num_enemies > 1)
					return (1);
				game->enemy.e_x = x + 0.5;
				game->enemy.e_y = y + 0.5;
			}
			x++;
		}
		y++;
	}
	return (0);	
}

/*By pythagoras we have distance from dx and dy
I want only to load enemy position once for performance so that is for...
...enemy.loaded. Also once if enemy not moving calculate the distance...
...with updated_dist*/
void	enemy(t_game *game, int x)
{
	double	e_dx;
	double	e_dy;

	e_dx = game->enemy.e_x - game->map->p_x;
	e_dy = game->enemy.e_y - game->map->p_y;	
	if (!game->enemy.loaded && enemy_position(game))
	{
		perror("Error: This game don't allow more than one enemy");
		clean_up_memory(game, 0);
		exit (1);
	}
	game->enemy.loaded = 1;
	if (!game->enemy.updated_dist)
	{
		game->enemy.e_dist = sqrt(e_dx * e_dx + e_dy * e_dy);
		game->enemy.updated_dist = 1;
	}
	(void)x;
	if (game->enemy.num_enemies == 1)
		draw_enemy_on_canvas(game, game->win->sprite[18]);
}
