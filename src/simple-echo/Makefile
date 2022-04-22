SERVER_NAME := simple_echo_server

CXX       := clang++
CXXFLAGS  := -std=c++98

SERVER_SRCS := ./main.cpp

SERVER_OBJS := $(SERVER_SRCS:.cpp=.o)

RM := rm -rf

all: $(SERVER_NAME)

$(SERVER_NAME) : $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS)

clean:
	$(RM) $(SERVER_OBJS)

fclean: clean
	$(RM) $(SERVER_NAME)

re: fclean all
