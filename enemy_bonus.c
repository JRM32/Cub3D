/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcoga2 <marcoga2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:44:09 by jrollon-          #+#    #+#             */
/*   Updated: 2025/09/22 16:45:35 by marcoga2         ###   ########.fr       */
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
/*We want to transform the sprite vector in the world (e_dx, e_dy) to camera...
...coordinates (trans_x, trans_y), and that is a matrix formula:

⎡trans_x⎤ = M-¹ * ⎡e_dx⎤
⎣trans_y⎦	      ⎣e_dy⎦

M-¹ = 1 / det(M) ⎡plane_y  -plane_x⎤
                 ⎣-dir_y   -dir_x  ⎦

det(M) =  dir_x * plane_y - dir_y * plane_x 

inv_det = 1 / det(M)*/
void	calculate_screen_position_size(t_game *game, double dx, double dy)
{
	t_map	*map;
	double	inv_det;
	double	trans_x;
	double	trans_y;

	map = game->map;
	inv_det = 1.0 / (map->dir_x * map->plane_y - map->dir_y * map->plane_x);
	trans_x = inv_det * (map->dir_y * dx - map->dir_x * dy);
	trans_y = inv_det * (-map->plane_y * dx + map->plane_x * dy);

	game->enemy.sprite_size = abs((int)(WIN_H / trans_y));

	game->enemy.screen_x = (int)((WIN_W / 2) * (1 + trans_x / trans_y)) - game->enemy.sprite_size / 2;
	game->enemy.screen_y = -game->enemy.sprite_size / 2 + WIN_H / 2;
	if (game->enemy.screen_x < 0)
		game->enemy.screen_x = 0;
	if (game->enemy.screen_y < 0)
		game->enemy.screen_y = 0;	
}


/*By pythagoras we have distance from dx and dy
I want only to load enemy position once for performance so that is for...
...enemy.loaded. Also once if enemy not moving calculate the distance...
...with updated_dist*/
void	enemy(t_game *game, int x)
{
	double	e_dx;
	double	e_dy;
	// t_enemy	*foe;

	// foe = &game->enemy;
	e_dx = game->enemy.e_x - game->map->p_x;
	e_dy = game->enemy.e_y - game->map->p_y;	
	if (!game->enemy.loaded && enemy_position(game))
	{
		perror("Error: This game doesn't allow more than one enemy");
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
	{
		calculate_screen_position_size(game, e_dx, e_dy);
		//draw_enemy_on_canvas(game, game->win->sprite[18], foe->screen_x, foe->screen_y);
	}	
}
