/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcoga2 <marcoga2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:35:04 by jrollon-          #+#    #+#             */
/*   Updated: 2025/09/24 13:16:54 by marcoga2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "get_next_line.h"

void	check_internal_lines(char *line, t_map *map, size_t columns, size_t ln)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (i == 0 && line[0] == '1' && ft_strlen(line) == columns)
			map->num_walls++;
		if (i == (columns - 1) && line[columns - 1] == '1'
			&& ft_strlen(line) == columns)
			map->num_walls++;
		if (line[i] == 'N' || line[i] == 'S' || line[i] == 'E'\
			|| line[i] == 'W')
		{
			map->num_p++;
			map->p_x = i + 0.5;
			map->p_y = (ln - 1) + 0.5;
			init_looking_direction(map, line[i]);
		}
		i++;
	}
}

void	check_line(char *line, char *next_line, t_map *map, size_t columns)
{
	size_t	i;

	i = 0;
	map->lines++;
	if (ft_strlen(line) != columns)
		map->no_rectangle = 1;
	while (line[i])
	{
		if (!ft_strchr(VALID_BONUSMAP_CHARS, line[i]))
			map->no_valid_char = 1;
		i++;
	}
	i = 0;
	if (map->lines == 1 || next_line == NULL)
	{
		while (line[i])
		{
			if (line[i] == '1')
				map->num_walls++;
			i++;
		}
		return ;
	}
	check_internal_lines(line, map, columns, map->lines);
}


int	free_and_get_line(int *is_first_char, char **line, int fd)
{
	free(*line);
	*line = get_next_line(fd);
	*is_first_char = 1;
	return (0);
}

char	*jump_to_map(int fd, char *line, t_map *map)
{
	int	i;
	int	is_first_char;

	i = -1;
	is_first_char = 1;
	while (line != NULL && contains_invalid_char(line, VALID_BONUSMAP_CHARS))
	{
		if (line[++i] == '\0')
			i = free_and_get_line(&is_first_char, &line, fd);
		if (line[i] == 'N' && line[i + 1] == 'O' && is_first_char)
			save_texture_in(&(line[i + 2]), &(map->NO_tex), &i);
		else if (line[i] == 'S' && line[i + 1] == 'O' && is_first_char)
			save_texture_in(&(line[i + 2]), &(map->SO_tex), &i);
		else if (line[i] == 'W' && line[i + 1] == 'E' && is_first_char)
			save_texture_in(&(line[i + 2]), &(map->WE_tex), &i);
		else if (line[i] == 'E' && line[i + 1] == 'A' && is_first_char)
			save_texture_in(&(line[i + 2]), &(map->EA_tex), &i);
		else if (line[i] == 'F' && is_first_char)
			save_color_in(&(line[i + 1]), &(map->floor_color), &i);
		else if (line[i] == 'C' && is_first_char)
			save_color_in(&(line[i + 1]), &(map->sky_color), &i);
		else if (!ft_isspace(line[i]))
			is_first_char = 0;
	}
	return (line);
}

void	check_map(t_map *map)
{
	size_t	i;
	size_t	len;

	i = 0;
	map->lines = 0;
	map->columns = 0;
	while (map->map[i])
	{
		len = ft_strlen(map->map[i]);
		if (map->columns < len)
			map->columns = len;
		check_line(map->map[i], map->map[i + 1], map, map->columns);
		i++;
	}
}

// void print_map(char **map)
// {
//     int i = 0;
//     while (map[i])
//     {
//         printf("%s", map[i]);
//         i++;
//     }
// }

t_map	*process_map(char *map_dir)
{
	t_map	*map;

	map = (t_map *)ft_calloc(1, sizeof(t_map));
	if (!map)
		return (NULL);
	check_map(map, map_dir);
	if (check_map_errors(map) || map->lines == 0)
		return (free(map), NULL);
	load_map(map, map_dir);
	if (!map->map)
		return (free(map), NULL);
	zerify_map(map);
	squarify_map(map->lines, map);
	// printf("----------------------------\n");
	// print_map(map->map);
	// printf("NO_tex: %s\n", map->NO_tex);
	// printf("WE_tex: %s\n", map->WE_tex);
	// printf("EA_tex: %s\n", map->EA_tex);
	// printf("SO_tex: %s\n", map->SO_tex);
	return (map);
}
