APP:=ex01
SHELL:=sh
CXX:=g++ -std=c++0x -Wall -O2
LD:=g++
LIBS:=-lGL -lglut
MAKEDEP:=g++ -M

SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)
DEPS:=$(SOURCES:.cpp=.d)

.PHONY: all clean

all: $(APP)

$(APP).png: $(APP).plt $(APP).dat
	gnuplot ex01.plt

$(APP).dat: $(APP)
	./$(APP) 1 5000

$(APP): $(OBJECTS)
	$(LD) $^ -o $@ $(LIBS)

$(OBJECTS): %.o: %.cpp Makefile
	$(CXX) -c $< -o $@

$(DEPS): %.d: %.cpp Makefile
	$(SHELL) -ec '$(CXX) -M $< | sed "s/$*.o/& $@/g" > $@'

clean:
	@rm -rf $(APP) $(OBJECTS) $(DEPS) *~ $(APP).dat $(APP).png

-include $(SOURCES:.cpp=.d)

