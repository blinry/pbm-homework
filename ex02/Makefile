APP:=ex02
SHELL:=sh
CXX:=g++ -Wall -O2 -std=c++0x
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

task3: 
	g++ -std=c++0x task3.cpp AdaptiveEulerSolver.cpp EulerSolver.cpp Particle.cpp GravitationalSystem.cpp RungeKuttaSolver.cpp -o task3
