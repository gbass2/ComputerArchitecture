CXX=g++
INCDIR=inc
SRCDIR=src
OBJDIR=obj
CPPFLAGS:=-I$(INCDIR) --std=c++14 -Wall
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(SRCS:%.cpp=$(ONJDIR)/%.o)
TARGET=simulation_demo

.PHONY : clean	run all

all : clean $(Target) run

$(OBJS) : $(OBJDIR)/%.o : %.cpp
	$(CXX) -c -o $@ $< $(CPPFLAGS)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $^ $(CPPFLAGS)

clean :
	rm -rf $(TARGET)
	rm -rf $(OBJDIR)/$(SRCDIR)/*.o

run :
	./$(TARGET)
