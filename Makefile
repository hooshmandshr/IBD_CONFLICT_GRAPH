
all:	
	#g++ main.cpp parser.cpp genome_matrix.cpp ibd_segment.cpp ibd_map.cpp conflict_graph.cpp node.cpp -o main
	g++ main.cpp genome_matrix.cpp ibd_map.cpp parser.cpp conflict_graph.cpp graph.cpp -o main 

