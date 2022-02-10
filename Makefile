all: MainProcessor MapProcessor ReduceProcessor 

MainProcessor: MainProcessor.cpp
	g++ MainProcessor.cpp -o MainProcessor.out

MapProcessor: MapProcessor.cpp
	g++ MapProcessor.cpp -o MapProcessor.out

ReduceProcessor: ReduceProcessor.cpp
	g++ ReduceProcessor.cpp -o ReduceProcessor.out

.PHONY: clean
clean:
	rm -rf *.out 
