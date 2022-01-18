SRC = main.c utils.c first_cmd.c ft_split.c

CC = gcc

HEADER = pipex.h

OBJ = $(SRC:%.c=./.build/%.o)

NAME = pipex

RM = rm -rf

CFLAGS = -Wall -Wextra -Werror

# COLORS
NONE			= \033[0m
CL_LINE			= \033[2K
B_RED			= \033[1;31m
B_GREEN			= \033[1;32m
B_MAGENTA 		= \033[1;35m
B_CYAN 			= \033[1;36m

./.build/%.o : %.c
	@$(CC) ${CFLAGS} -I. -o $@ -c $?
	@printf "${B_MAGENTA}Compilling $? ...\n${NONE}"

all :	
	@mkdir -p .build
	@make ${NAME}

${NAME} : $(OBJ)
	@printf "${B_CYAN}${NAME} is linking ...${NONE}\n"
	@${CC} ${CFLAGS} -o ${NAME} ${OBJ} -I
	@printf "${B_GREEN}==>{${NAME}} LINKED SUCCESFULLY<==${NONE}\n"

clean :
	@${RM} .build
	@printf "${B_RED}Deleting ./.build folder ...${NONE}\n"

fclean :	clean
	@${RM} ${NAME}
	@printf "${B_RED}Deleting ${NAME} ...${NONE}\n"

re :	fclean all