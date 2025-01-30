#makefile for specAlign

EXECUTABLE_PREPRO = ./bin/specAlign-prepro
EXECUTABLE_PEAKALIGN = ./bin/specAlign-peakalign
EXECUTABLE_PAFFT = ./bin/specAlign-PAFFT
EXECUTABLE_RAFFT = ./bin/specAlign-RAFFT
LINKCC = $(CXX)

#CXXFLAGS denotes flags for the C++ compiler

CXX = g++
CXXFLAGS = -O3
LDFLAGS = -lpthread -L/usr/lib -lm

SRCS := $(wildcard ./src/common/*.cpp) $(wildcard ./src/*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(wildcard ./src/common/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard ./src/*.cpp))
OBJS_PREPRO := $(patsubst %.cpp,%.o,$(wildcard ./src/common/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard ./src/specAlign-prepro_main.cpp))
OBJS_PEAKALIGN := $(patsubst %.cpp,%.o,$(wildcard ./src/common/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard ./src/specAlign-peakalign_main.cpp))
OBJS_PAFFT := $(patsubst %.cpp,%.o,$(wildcard ./src/common/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard ./src/specAlign-PAFFT_main.cpp))
OBJS_RAFFT := $(patsubst %.cpp,%.o,$(wildcard ./src/common/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard ./src/specAlign-RAFFT_main.cpp))
#DEPS := $(patsubst %.o,%.d,$(OBJS))


all: 	$(EXECUTABLE_PREPRO) $(EXECUTABLE_PEAKALIGN) $(EXECUTABLE_PAFFT) $(EXECUTABLE_RAFFT)
	@echo ""
	@echo "By using specAlign you agree to the terms and conditions set out in the"
	@echo "LICENSE file included with this package. If you have not done so, please"
	@echo "refer to the LICENSE file before use of this application."
	@echo ""


#define the components of the program, and how to link them
#these components are defined as dependencies; that is they must be up-to-date before the code is linked

$(EXECUTABLE_PREPRO): $(OBJS)
	$(LINKCC) $(LDFLAGS) $(CXXFLAGS) -o $(EXECUTABLE_PREPRO) $(OBJS_PREPRO)

$(EXECUTABLE_PEAKALIGN): $(OBJS)
	$(LINKCC) $(LDFLAGS) $(CXXFLAGS) -o $(EXECUTABLE_PEAKALIGN) $(OBJS_PEAKALIGN)

$(EXECUTABLE_PAFFT): $(OBJS)
	$(LINKCC) $(LDFLAGS) $(CXXFLAGS) -o $(EXECUTABLE_PAFFT) $(OBJS_PAFFT)

$(EXECUTABLE_RAFFT): $(OBJS)
	$(LINKCC) $(LDFLAGS) $(CXXFLAGS) -o $(EXECUTABLE_RAFFT) $(OBJS_RAFFT)

#specify the dep files depend on the cpp files

%.d: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@
	$(CXX) -M $(CXXFLAGS) $< | sed s/\\.o/.d/ > $@
	


clean:
	-rm $(OBJS)

explain:
	@echo "The following info represents the program:"
	@echo "Final exec name: $(EXECUTABLE)"
	@echo "Source files:       $(SRCS)"
	@echo "Object files:       $(OBJS)"