program = monte_carlo

obj = $(source:.cpp=.o)

headers = $(source:.cpp=.h)
source = Boundaries.cpp
source += Main.cpp
source += Fission.cpp
source += Material.cpp
source += Distributions.cpp
source += Tally.cpp
source += Neutron.cpp
source += Mesh.cpp
source += Monte_carlo.cpp
source += Plotter.cpp

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
