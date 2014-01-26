#include <stdio.h>
#include <iostream>
#include <list>
#include <math.h>

#define NOT_DECIDED 0
#define E_DOMSET 1
#define NE_DOMSET 2

#define NMAX 500

#define DEBUG 0

using namespace std;

/* Definition of an graph vertex in the adjList */
typedef struct vertex{
	int degree;
	int status;
	int times_dominated;
	int allred_dominated;
	list<int> neighbors;
};

int n_vertex,n_graph = 1, max_degree = 0;
/* Adjacent List */
vertex adjList[NMAX];
/* Variables for finding the domSet */
int size_domSet, size_minDomSet, n_dominated;
int domSet[NMAX], minDomSet[NMAX];
bool endOfInput = false;

/* Init graph */
void init_graph(){
	/* Set graph */
	n_vertex = 0;
}

/* Graph input */
bool graph_input(){
	std::list<vertex>::iterator it;
	max_degree = 0;
	/* Read number of vertices */
	if(scanf("%d",&n_vertex) == EOF)
		return true;
//	printf("Read: %d\n",*n_vertex);
	for(int i=0;i<n_vertex;i++){
		vertex tmp;
		int tmp_vertex;
		
		tmp.status = NOT_DECIDED;
		/* Read degree of the vertex */
		scanf("%d", &tmp.degree);
		if(tmp.degree > max_degree)
			max_degree = tmp.degree;
		tmp.allred_dominated = tmp.degree + 1;
		tmp.times_dominated = 0;
		/* Create a vertex */
		adjList[i] = tmp;
		for(int j=0;j<tmp.degree;j++){
			scanf("%d", &tmp_vertex);
			adjList[i].neighbors.push_back(tmp_vertex);
		}
	}
	/* Set the inital domSet as the entire graph */
	size_domSet = 0;
	size_minDomSet = n_vertex;
	n_dominated = 0;
	for(int i=0;i<n_vertex;i++){
		minDomSet[i] = i;
	}
	return false;
}

void debug(int rec_level){
	int i = 0;

	printf("Recursion Level: %d\n", rec_level);
	printf("   Number of vertices dominated: %d\n", n_dominated);	
	printf("   Vertex: Times Dominated - Times Dominated if Whites are Red - Status\n"); 
	for(int it=0;it < n_vertex;it++){
		printf("   %d: %d - %d",i,adjList[it].times_dominated,adjList[it].allred_dominated);
		if(adjList[it].status == NOT_DECIDED) printf(" - NOT DECIDED\n");
		else if(adjList[it].status == E_DOMSET) printf(" - E DOMSET\n");
		else if(adjList[it].status == NE_DOMSET) printf(" - NE DOMSET\n");
		i++;
	}
}

void find_minDomSet(int rec_level){
//	if(DEBUG) debug(rec_level);
	
	/* If all whites are red and there is some vertex dominated 0 times, then backtrack */
	for(int it = 0;it < n_vertex;it++){
		if(adjList[it].allred_dominated == 0)
			return;
	}

	/**************************************************************************************/
	if(n_dominated == n_vertex){
			if(size_domSet < size_minDomSet){
//				if(DEBUG){
//					printf("*** RECORD THIS SET AS THE CURRENT minDomSet ***\n");
//					printf("*** minDomSet:");
//				}
				printf("	BEST so far (Size %d):", size_domSet);
				size_minDomSet = size_domSet;
				for(int i=0;i<size_domSet;i++){
					minDomSet[i] = domSet[i];
					if(i != 0 && i != size_domSet) printf(" ");
					printf(" %d",minDomSet[i]);
				}
				printf("\n");
			}
		return;
	}

	/* If is in the last level (All vertices have an status) and All vertex are dominated */
	if(rec_level == n_vertex-1 || size_domSet+ceil((n_vertex-n_dominated)/(max_degree+1)) >= size_minDomSet){
		return;
	}

	/**************************************************/
	/* Try vertex as EN_DOMSET (Excluded from domSet) */
	/* Push */
	adjList[rec_level].status = NE_DOMSET;
	adjList[rec_level].allred_dominated = adjList[rec_level].allred_dominated - 1;
	for(std::list<int>::iterator itt = adjList[rec_level].neighbors.begin();itt != adjList[rec_level].neighbors.end(); ++itt){
		adjList[*itt].allred_dominated = adjList[*itt].allred_dominated - 1;
	}
	/* Recursive call */
	find_minDomSet(rec_level+1);
	/* Pop */
	adjList[rec_level].status = NOT_DECIDED;
	adjList[rec_level].allred_dominated = adjList[rec_level].allred_dominated + 1;
	for(std::list<int>::iterator itt = adjList[rec_level].neighbors.begin();itt != adjList[rec_level].neighbors.end(); ++itt){
		adjList[*itt].allred_dominated = adjList[*itt].allred_dominated + 1;
	}

	/**************************************/
	/* Try vertex as E_DOMSET (In domSet) */
	/* Push */
	size_domSet = size_domSet + 1;
	domSet[size_domSet - 1] = rec_level;
	adjList[rec_level].status = E_DOMSET;
	if(adjList[rec_level].times_dominated == 0)
		n_dominated++;
	adjList[rec_level].times_dominated = adjList[rec_level].times_dominated + 1;
	for(std::list<int>::iterator itt = adjList[rec_level].neighbors.begin();itt != adjList[rec_level].neighbors.end(); ++itt){
		if(adjList[*itt].times_dominated == 0)
			n_dominated++;
		adjList[*itt].times_dominated = adjList[*itt].times_dominated + 1;
	}
	/* Recursive call */
	find_minDomSet(rec_level+1);
	/* Pop */
	size_domSet = size_domSet - 1;
	adjList[rec_level].status = NOT_DECIDED;
	adjList[rec_level].times_dominated = adjList[rec_level].times_dominated - 1;
	if(adjList[rec_level].times_dominated == 0)
		n_dominated--;
	for(std::list<int>::iterator itt = adjList[rec_level].neighbors.begin();itt != adjList[rec_level].neighbors.end(); ++itt){
		adjList[*itt].times_dominated = adjList[*itt].times_dominated - 1;
		if(adjList[*itt].times_dominated == 0)
			n_dominated--;
	}
}

int main(int argc, char **argv){
	
	do{
		init_graph();
		endOfInput = graph_input();
		if(endOfInput == false){
			printf("Graph %d:\n",n_graph++);
			fflush(stdout);
			find_minDomSet(0);
		}
	}while(endOfInput == false);
}
