
all:
	#g++ parser.cpp ibd_segment.cpp -o parser
	g++ main.cpp parser.cpp genome_matrix.cpp ibd_segment.cpp ibd_map.cpp -o main 

