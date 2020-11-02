CC = g++
CFLAGS = -I. --std=c++17
OBJS = main.o system.o simobject.o
HEADERS = event.hh system.hh simobject.hh memory.hh
TARGET = simulation
BASEDIR = /Users/graysonbass/github/ComputerArchitecture
SRCDIR = $(BASEDIR)/src
TESTSDIR = $(BASEDIR)/tests
BINDIR = $(BASEDIR)/bin
OBJDIR = $(BASEDIR)/obj

.PHONY : clean run all

all: clean $(TARGET) run

%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET)
	rm -rf *.o

run:
	./$(TARGET)
