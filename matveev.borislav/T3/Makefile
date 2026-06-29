CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -Weffc++

run: lab
	./lab

lab: main.o geometry.o polygonAlgorithms.o commands.o ioFormat.o
	$(CXX) -o $@ $^

main.o: main.cpp geometry.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

geometry.o: geometry.cpp geometry.hpp ioFormat.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

polygonAlgorithms.o: polygonAlgorithms.cpp polygonAlgorithms.hpp geometry.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

commands.o: commands.cpp commands.hpp polygonAlgorithms.hpp geometry.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

ioFormat.o: ioFormat.cpp ioFormat.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f lab *.o
