#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <list>

using namespace std;

/* Definition of an graph vertex in the adjList */
struct vertex{
	long long int degree;
	list<long long int> neighbors;
};

/* Init graph */
void init_graph(long long int *n_vertex, std::list<vertex> *adjList, long long int *size_domSet, std::list<long long int> *domSet){
	/* Set graph */
	*n_vertex = 0;
	adjList->clear();
	/* Set dominating set */
	*size_domSet = 0;
	domSet->clear();
}

/* Graph input */
int graph_input(long long int *n_vertex, std::list<vertex> *adjList, long long int *size_domSet, std::list<long long int> *domSet, long long int i){
	std::list<vertex>::iterator it;
	int return_value;
	return_value = 1;
	
	/* Read number of vertices */
	if(scanf("%lli",n_vertex) == EOF)
		return 0;
//	printf("Graph %lli: \n", i);
	if(*n_vertex < 0){
		printf("Invalid number of vertices!\n");
		return_value = 2;
	}
	for(long long int i=0;i<*n_vertex;i++){
		vertex tmp;
		long long int tmp_vertex;
		
		/* Read degree of each vertex */
		if(scanf("%lli", &tmp.degree) == EOF)
			return 0;
		if(tmp.degree < 0 || tmp.degree > *n_vertex-1){
			printf("Invalid degree of vertex: %lli\n", i);
			return_value = 2;
		}
		/* Create a vertex */
		it = adjList->end();
		it = adjList->insert(it, tmp);
		for(long long int j=0;j<tmp.degree;j++){
			if(scanf("%lli", &tmp_vertex) == EOF)
				return 0;
			if(tmp_vertex < 0 || tmp_vertex > *n_vertex-1){
				printf("Invalid neighbor for vertex: %lli\n", i);
				return_value = 2;
			}
			it->neighbors.push_back(tmp_vertex);
		}
	}
	/* Read Dominating Set Candidate */
	if(scanf("%lli",size_domSet) == EOF)
		return 0;
	if(*size_domSet < 0 || *size_domSet > *n_vertex){
		printf("Invalid number of vertices for dominating set!\n");
		return_value = 2;
	}
	for(long long int i=0;i<*size_domSet;i++){
		long long int tmp;
		
		if(scanf("%lli", &tmp) == EOF)
			return 0;
		if(tmp < 0 || tmp > *n_vertex-1){
			printf("Invalid vertex in dominating set\n");
			return_value = 2;
		}
		domSet->push_back(tmp);
	}
	return return_value;
}

/* Graph output */
void graph_output(long long int *n_vertex, std::list<vertex> *adjList, long long int *size_domSet, std::list<long long int> *domSet){
	long long int i = 0;

	printf("%lli\n", *n_vertex);
	for(std::list<vertex>::iterator it = adjList->begin();it != adjList->end();++it){
		printf("%lli",it->degree);
		for(std::list<long long int>::iterator itt = it->neighbors.begin();itt != it->neighbors.end();++itt){
			printf(" %lli", *itt);
		}
		printf("\n");
		i++;
	}
	printf("Given dominating Set:");
	for(std::list<long long int>::iterator it = domSet->begin();it != domSet->end();++it){
		printf("% lli", *it);
	}
	printf("\n");
}

bool is_domSet(long long int *n_vertex, std::list<vertex> *adjList, long long int *size_domSet, std::list<long long int> *domSet){
	std::list<long long int> domVertices;
	long long int count = 0;
	
	for(std::list<long long int>::iterator it = domSet->begin();it != domSet->end();++it){
		count = 0;
		for(std::list<vertex>::iterator itt = adjList->begin();itt != adjList->end();++itt){
			if(count == *it){
//				printf("FOR %lli ADDING:\n", *it);
				for(std::list<long long int>::iterator ittt = itt->neighbors.begin();ittt != itt->neighbors.end();++ittt){
//					printf(" %lli", *ittt);
					domVertices.push_back(*ittt);
				}
//				printf("\n");
				break;
			}
			count++;
		}
	}
	for(std::list<long long int>::iterator it = domSet->begin();it != domSet->end();++it){
		domVertices.push_back(*it);
	}
	domVertices.sort();
	domVertices.unique();
	
//	printf("Given set:   ");
//	for(std::list<long long int>::iterator it = domVertices.begin();it != domVertices.end();++it){
//		printf(" %lli", *it);
//	}
//	printf("\n");

	if (domVertices.size() == *n_vertex){
//		printf("IS a dominating set\n\n");
		return true;
	} else {
//		printf("NOT a dominating set.\n# of vertices dominated: %lli.\n# of vertices: %lli.\n\n", (long long int)domVertices.size(), (long long int)*n_vertex);
		return false;
	}
}

int main(int argc, char** argv) {
	long long int n_vertex, size_domSet, i=0;
	std::list<vertex> adjList;
	std::list<long long> domSet;
	int tmp;
	
	init_graph(&n_vertex, &adjList, &size_domSet, &domSet);
	while((tmp = graph_input(&n_vertex, &adjList, &size_domSet, &domSet, i))){
//		graph_output(&n_vertex, &adjList, &size_domSet, &domSet);
		if(tmp == 1){
			if(is_domSet(&n_vertex, &adjList, &size_domSet, &domSet)){
				printf ("\nCORRECT dominating sets.\n");
				return 0;
			} else {
				printf ("\nWRONG dominating sets.\n");
				return 1;
			}
			init_graph(&n_vertex, &adjList, &size_domSet, &domSet);
		}
		if(tmp == 2)
			printf("\n");
		i++;
	}
}
