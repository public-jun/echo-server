CLIENT_NAME := client
SERVER_NAME := server

CXX       := clang++
CXXFLAGS  := -std=c++98

CLIENT_SRCS := ./client.cpp
SERVER_SRCS := ./server.cpp

CLIENT_OBJS := $(CLIENT_SRCS:.cpp=.o)
SERVER_OBJS := $(SERVER_SRCS:.cpp=.o)

RM := rm -rf

all: $(CLIENT_NAME) $(SERVER_NAME)

$(CLIENT_NAME) : $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_NAME) $(CLIENT_OBJS)

$(SERVER_NAME) : $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS)

clean:
	$(RM) $(CLIENT_OBJS) $(SERVER_OBJS)

fclean: clean
	$(RM) $(CLIENT_NAME) $(SERVER_NAME)

re: fclean all
