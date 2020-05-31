TARGET = main

CC = g++
SRC = main.cpp blocking_queue.cpp
OBJ = $(patsubst %.cpp,%.o,$(SRC))

CXXFLAGS += -g -Wall -I./ -std=c++11 -o0
LDFLAGS += -lpthread -Wl,-rpath -Wl,--no-as-needed

all: $(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $^

%.o: %.c
	$(CC) $(CXXFLAGS) -c -o $@ $<

.PHONY: all clean

clean:
	rm -f $(TARGET) $(OBJ)
