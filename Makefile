CC:= g++
override CFLAGS += -O3 -Wall -g -I./include

TARGET = greedy_snake


$(TARGET) : Snake.o greedy_snake.o
	$(CC) $(CFLAGS) $^ -o $@ -lcurses

Snake.o : Snake.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -lcurses

greedy_snake.o : greedy_snake.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -lcurses

clean:
	rm *.o