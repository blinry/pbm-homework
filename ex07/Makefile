APP:=ex07
SHELL:=sh
CXX:=g++ -Wall -O2
LD:=g++
LIBS:=-lglfw -lGLU -lGL
MAKEDEP:=g++ -M

CPPSOURCES:=$(wildcard *.cpp)
CPPOBJECTS:=$(CPPSOURCES:.cpp=.o)
CPPDEPS:=$(CPPSOURCES:.cpp=.d)

CUSOURCES:=$(wildcard *.cu)
CUOBJECTS:=$(CUSOURCES:.cu=.o)
CUDEPS:=$(CUSOURCES:.cu=.d)

.PHONY: all clean

all: $(APP)

$(APP): $(CPPOBJECTS) $(CUOBJECTS)
	$(LD) $^ -o $@ $(LIBS)

$(CPPOBJECTS): %.o: %.cpp Makefile
	$(CXX) -c $< -o $@

$(CPPDEPS): %.d: %.cpp Makefile
	@$(SHELL) -ec '$(CXX) -M $< | sed "s/$*.o/& $@/g" > $@'

clean:
	@rm -rf $(APP) $(CPPOBJECTS) $(CUOBJECTS) $(CPPDEPS) $(CUDEPS) *~ $(APP).dat $(APP).png

-include $(CPPSOURCES:.cpp=.d)
-include $(CUSOURCES:.cu=.d)

