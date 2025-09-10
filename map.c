/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcoga2 <marcoga2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:02:09 by jrollon-          #+#    #+#             */
/*   Updated: 2025/09/10 11:53:42 by marcoga2         ###   ########.fr       */
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
	while (line[i])
	{
		if (!ft_strchr(VALID_MAP_CHARS, line[i]))
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

void	check_map(t_map *map, char *map_dir)
{
	char	*line;
	char	*next_line;
	int		fd;

	fd = open(map_dir, O_RDONLY);
	if (fd == -1)
	{
		write(1, "Error\n", 6);
		write(1, "Error loading map\n", 18);
		return ;
	}
	next_line = NULL;
	line = get_next_line(fd);
	while (line)
	{
		next_line = get_next_line(fd);
		if (map->lines == 0)
			map->columns = ft_strlen(line);
		check_line(line, next_line, map, map->columns);
		free(line);
		line = next_line;
	}
	close (fd);
}

int	check_map_errors(t_map *map)
{
	int	error;
	int	closed_walls;

	error = 0;
	closed_walls = map->columns * 2 + (map->lines - 2) * 2;
	if (closed_walls != map->num_walls
		|| map->num_p != 1 || map->no_valid_char == 1)
	{
		write(1, "Error\n", 6);
		error = 1;
	}
	if (closed_walls != map->num_walls)
		write(1, "Map not fully closed by walls\n", 30);
	if (map->num_p != 1)
		write(1, "There is no char start position, or more than one\n", 50);
	if (map->no_valid_char)
		write(1, "There is at least one invalid char in the map\n", 46);
	return (error);
}

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
	return (map);
}
