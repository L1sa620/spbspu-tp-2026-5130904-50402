CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -Weffc++

run: lab
	./lab

lab: main.o dataStruct.o dataStructIO.o ioFormat.o
	$(CXX) -o $@ $^

main.o: main.cpp dataStruct.hpp dataStructIO.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dataStruct.o: dataStruct.cpp dataStruct.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dataStructIO.o: dataStructIO.cpp dataStructIO.hpp dataStruct.hpp ioFormat.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

ioFormat.o: ioFormat.cpp ioFormat.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f lab *.o
