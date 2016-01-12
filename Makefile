#=============================================================================
#
#=============================================================================


program = monte_carlo

source = \
boundaries.cpp

obj = $(source:.cpp=.o)

headers = $(source:.cpp=.h)
source += main.cpp
source += fission.cpp
source += material.cpp
source += global_constants.h
source += distributions.cpp
source += tally.cpp
source += neutron.cpp
source += mesh.cpp
#=============================================================================
#
#=============================================================================

CC = g++

#=============================================================================
#
#=============================================================================

$(program): $(obj) $(headers)
	$(CC) $(obj) -o $@ -lm

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -rf $(program) $(obj)

edit:
	vim -p $(source) $(headers)

run:
	./$(program)

