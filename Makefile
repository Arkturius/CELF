NAME		=	celf
LIBNAME		=	lib$(NAME).so
STATICLIB	=	lib$(NAME).a

SRC_DIR		:=	src
INC_DIR		:=	include
OBJ_DIR		:=	build

include			sources.mk
SRCS		:=	$(addprefix $(SRC_DIR)/, $(SRCS))

OBJS		:=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
STATIC_OBJS	:=	$(addprefix $(OBJ_DIR)/static/, $(SRCS:%.c=%.o))

CC			:=	clang
AR			:=	ar

CFLAGS		:=	-Wall -Wextra -Werror -MMD -MP -O0
IFLAGS_DIRS	:=	$(INC_DIR)
IFLAGS		+=	$(foreach dir, $(IFLAGS_DIRS), -I$(dir))

ifneq ($(DEBUG), 0)
ifeq ($(DEBUG), 2)
	CFLAGS	+= -gdwarf-2
else
	CFLAGS	+= -g3
endif
endif

ifeq ($(SAVE_TEMPS), 1)
	CFLAGS	+=	--save-temps=obj
endif

VERBOSE		?=	0
ifeq ($(VERBOSE), 1)
	CFLAGS	+=	-DVERBOSE=1
endif

RM			:=	rm -rf
MKDIR		:=	mkdir -p
MAKE		+=	--no-print-directory

#
# Rules
#

all:		static shared

static:		$(STATICLIB)

shared:		$(LIBNAME)

$(LIBNAME):			$(OBJS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  linking	$(GREEN)$(BOLD)$(ITALIC)$(LIBNAME)$(RESET)"
	@$(CC) -shared $(CFLAGS) -o $@ $^ $(IFLAGS)

$(STATICLIB):		$(STATIC_OBJS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  archiving	$(GREEN)$(BOLD)$(ITALIC)$(STATICLIB)$(RESET)"
	@$(AR) -rcs $@ $^

$(OBJ_DIR)/%.o:		%.c
	@$(MKDIR) $(@D)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$(notdir $@)$(RESET) from $(GRAY)$(BOLD)$(ITALIC)$(notdir $^)$(RESET)"
	@$(CC) $(CFLAGS) -fPIC -o $@ -c $< $(IFLAGS)

$(OBJ_DIR)/static/%.o:	%.c
	@$(MKDIR) $(@D)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$(notdir $@)$(RESET) from $(GRAY)$(BOLD)$(ITALIC)$(notdir $^)$(RESET) (static)"
	@$(CC) $(CFLAGS) -o $@ -c $< $(IFLAGS)

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(NAME)/$(OBJ_DIR)$(RESET)"; \
		$(RM) $(OBJ_DIR); \
	fi

fclean:		clean
	@if [ -f "$(LIBNAME)" ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(LIBNAME)$(RESET)"; \
		$(RM) $(LIBNAME); \
	fi
	@if [ -f "$(STATICLIB)" ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(STATICLIB)$(RESET)"; \
		$(RM) $(STATICLIB); \
	fi
	@$(RM) *.d

re:			fclean all

-include	$(OBJS:.o=.d)

.PHONY:		all clean fclean re
.SILENT:	all clean fclean re

#
# COLORS
# 

BOLD			=	\033[1m
ITALIC			=	\033[3m

RED				=	\033[31m
GREEN			=	\033[32m
YELLOW			=	\033[33m
CYAN			=	\033[36m
GRAY			=	\033[90m

RESET			=	\033[0m

LINE_CLR		=	\33[2K\r


