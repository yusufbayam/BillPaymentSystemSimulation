simulation: simulation.o
	g++ simulation.o -o simulation -lpthread

simulation.o: simulation.cpp
	g++ -c simulation.cpp -lpthread

clean:
	rm *.o simulation