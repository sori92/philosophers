#include "philo.h"

void    show_manager(t_manager manager)
{
	printf("MANAGER dead var: %d\n", manager.dead);
	printf("MANAGER n_philos var: %d\n", manager.n_philos);
	printf("MANAGER time_die: %d\n", manager.time_die);
	printf("MANAGER time_eat: %d\n", manager.time_eat);
	printf("MANAGER time_sleep var: %d\n", manager.time_sleep);
	printf("MANAGER goal_lunchs var: %d\n", manager.goal_lunchs);
	printf("MANAGER threads pointer: %p\n", manager.threads);
	printf("MANAGER forks pointer: %p\n", manager.forks);
	printf("MANAGER printer pointer: %p\n", &manager.printer);
}

void    show_philo(t_philo philo)
{
	printf("%d l_hand var: %d\n", philo.name, philo.l_hand);
	printf("%d r_hand var: %d\n", philo.name, philo.r_hand);
	//printf("%d manager pointer: %p\n", philo.name, philo.manager);
}
