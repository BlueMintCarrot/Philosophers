/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joanda-s <joanda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:34:56 by joanda-s          #+#    #+#             */
/*   Updated: 2024/05/03 17:18:52 by joanda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>


typedef struct	s_philo
{
	int			seat_pos;
	int			still_alive;
	long int	time_til_death;
	pthread_t	id;
	t_info		*info;
}				t_philo;

typedef struct	s_info
{
	int				nbr_of_meals;
    int				nbr_of_philo;
    long int		time_til_death;
	long int		start_time;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		time_to_think;
	pthread_mutex_t	beep;
	pthread_mutex_t	*fork;
	pthread_mutex_t	lock;
	
}				t_info;


#endif