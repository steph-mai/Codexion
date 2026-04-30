NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread

SRCS        = main.c \
              src/parsing.c \
			  src/coder_routine.c\
			  src/monitor.c\
			  src/scheduler.c\
			  src/utils.c\
			  src/cleanup.c\
			  src/init.c\
			  src/simulation.c

OBJS        = $(SRCS:.c=.o)

INCLUDES    = -I.

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

-include $(SRCS:.c=.d)

clean:
	rm -f $(OBJS) $(SRCS:.c=.d)
	@echo "Object and dependency files cleaned."

fclean: clean
	rm -f $(NAME)
	@echo "Executable removed."

re: fclean all

.PHONY: all clean fclean re
