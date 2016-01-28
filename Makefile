program = monte_carlo

source = \
boundaries.cpp

obj = $(source:.cpp=.o)

headers = $(source:.cpp=.h)
source += main.cpp
#source+= k_tester.cpp
source += fission.cpp
source += material.cpp
source += distributions.cpp
source += tally.cpp
source += neutron.cpp
source += mesh.cpp
source += monte_carlo.cpp

CC = g++

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

