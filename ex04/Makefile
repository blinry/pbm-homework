APP:=ex04
SHELL:=sh
CXX:=g++ -Wall -O2 -fno-strict-aliasing
LD:=g++
LIBS:=-lglut -lGLU -lGL
MAKEDEP:=g++ -M

SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)
DEPS:=$(SOURCES:.cpp=.d)

.PHONY: all clean

all: $(APP)

$(APP): $(OBJECTS)
	$(LD) $^ -o $@ $(LIBS)

$(OBJECTS): %.o: %.cpp Makefile
	$(CXX) -c $< -o $@

$(DEPS): %.d: %.cpp Makefile
	@$(SHELL) -ec '$(CXX) -M $< | sed "s/$*.o/& $@/g" > $@'

clean:
	@rm -rf $(APP) $(OBJECTS) $(DEPS) *~ $(APP).dat $(APP).png

-include $(SOURCES:.cpp=.d)

