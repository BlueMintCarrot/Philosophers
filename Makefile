CC = cc
CFLAGS = -g -pthread -Wall -Wextra -Werror
NAME = philo

SOURCES = actions.c init_and_input.c libft.c main.c stuff.c

OBJS_DIR = obj
OBJECTS = $(addprefix $(OBJS_DIR)/, $(SOURCES:%.c=%.o))

all: $(NAME)

$(NAME): $(OBJECTS)
		$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJS_DIR)/%.o:%.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf $(OBJS_DIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re