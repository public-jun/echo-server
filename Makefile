NAME      := echoserv

CXX       := clang++
# CXXFLAGS  := -Wall -Wextra -Werror -std=c++98
CXXFLAGS  := -std=c++98

INCDIR    :=    ./include
OBJDIR    :=    ./objs
DPSDIR    :=    ./dps

INCLUDE   := -I$(INCDIR)
VPATH     := src:

SRCS      := src/main2.cpp

OBJS      := $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))
DPS       := $(addprefix $(DPSDIR)/, $(notdir $(SRCS:.o=.d)))

RM        := rm -rf

all: makedir $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) $(LIBS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(DPSDIR)/$(notdir $(<:.cpp=.d)) -c $< -o $@

-include $(DPS)

makedir :
	mkdir -p $(OBJDIR)
	mkdir -p $(DPSDIR)

clean:
	rm -rf $(OBJDIR) $(DPSDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
