APP:=ex06
SHELL:=sh
CXX:=g++ -Wall -O2 -fno-strict-aliasing -I /usr/include/bullet -I OpenGL
LD:=g++ -lglut -lGLU -lGL $(shell pkg-config --libs bullet)
MAKEDEP:=g++ -M

SOURCES:=$(shell find . -name '*.cpp')
OBJECTS:=$(SOURCES:.cpp=.o)
DEPS:=$(SOURCES:.cpp=.d)

.PHONY: all clean

all: $(APP)

$(APP): $(OBJECTS)
	$(LD) $^ -o $@

$(OBJECTS): %.o: %.cpp Makefile
	$(CXX) -c $< -o $@

$(DEPS): %.d: %.cpp Makefile
	@$(SHELL) -ec '$(CXX) -M $< | sed "s/$*.o/& $@/g" > $@'

clean:
	@rm -rf $(APP) $(OBJECTS) $(DEPS) *~ $(APP).dat $(APP).png

-include $(SOURCES:.cpp=.d)

