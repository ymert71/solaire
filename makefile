CC = g++

CFLAGS = -Wall -g

LIBS = -lGL -lGLU -lglut

TARGET = main

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
	rm -f $(OBJS) $(TARGET)
