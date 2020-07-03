start:
	g++ -o Graphgen generator.cpp
	./Graphgen > store.txt
	g++ -I/Library/gurobi902/mac64/include/ /Library/gurobi902/mac64/lib/libgurobi_c++.a -m64 -g -lgurobi90 -o Binpacking bin_packer.cpp
	./Binpacking
	g++ -I/Library/gurobi902/mac64/include/ /Library/gurobi902/mac64/lib/libgurobi_c++.a -m64 -g -lgurobi90 -o PropColour proper_colour.cpp
	./PropColour

clean:
	rm Propcolour Graphgen Binpacking store.txt