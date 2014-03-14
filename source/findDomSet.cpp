#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <queue>
#include <math.h>
#include <string.h>
#include <algorithm>

#define NOT_DECIDED 0
#define E_DOMSET 1
#define NE_DOMSET 2

#define NMAX 500

#define DEBUG 0

using namespace std;

/* Definition of an graph vertex in the adjList */
struct vertex{
	int degree;
	int status;
	int times_dominated;
	int allred_dominated;
	list<int> neighbors;
};

/* Graph input */
bool graph_input(int *n_vertex, vertex *adjList,int *max_degree, int *size_domSet, int *size_minDomSet, int *domSet, int *minDomSet, int *n_dominated, bool certOut, int queue[]){
	std::list<vertex>::iterator it;
	int listed[NMAX], nListed;
	*max_degree = 0;
	*n_vertex = 0;
	/* Read number of vertices */
	if(scanf("%d",n_vertex) == EOF)
		return true;
	if(certOut)
		printf("%d\n", *n_vertex);
//	printf("Read: %d\n",*n_vertex);

	memset(listed,0,*n_vertex*sizeof(int));
	listed[0]++;
	queue[0]=0;
	nListed=1;

	for(int i=0;i<*n_vertex;i++){
		vertex tmp;
		int tmp_vertex;
		
		tmp.status = NOT_DECIDED;
		/* Read degree of the vertex */
		if(scanf("%d", &tmp.degree) == EOF)
			return false;
		if(certOut)
			printf("%d", tmp.degree);
		if(tmp.degree > *max_degree)
			*max_degree = tmp.degree;
		tmp.allred_dominated = tmp.degree + 1;
		tmp.times_dominated = 0;
		/* Create a vertex */
		adjList[i] = tmp;
		for(int j=0;j<tmp.degree;j++){
			if(scanf("%d", &tmp_vertex) == EOF)
				return false;
			if(certOut)
				printf(" %d", tmp_vertex);
			if(!listed[tmp_vertex]){
				queue[nListed++] = tmp_vertex;
				listed[tmp_vertex]++;
			}
			adjList[i].neighbors.push_back(tmp_vertex);
		}
		if(certOut)
			printf("\n");
	}
	/* Set the inital domSet as the entire graph */
	*size_domSet = 0;
	*size_minDomSet = *n_vertex;
	*n_dominated = 0;
	for(int i=0;i<*n_vertex;i++){
		minDomSet[i] = i;
	}
	return false;
}

/*void debug(int vertex){
	int i = 0;

	printf("Recursion Level: %d\n", vertex);
	printf("   Number of vertices dominated: %d\n", n_dominated);	
	printf("   Vertex: Times Dominated - Times Dominated if Whites are Red - Status\n"); 
	for(int it=0;it < n_vertex;it++){
		printf("   %d: %d - %d",i,adjList[it].times_dominated,adjList[it].allred_dominated);
		if(adjList[it].status == NOT_DECIDED) printf(" - NOT DECIDED\n");
		else if(adjList[it].status == E_DOMSET) printf(" - E DOMSET\n");
		else if(adjList[it].status == NE_DOMSET) printf(" - NE DOMSET\n");
		i++;
	}
}*/

int find_minDomSet(int rec_level, int *n_vertex, vertex *adjList,int *max_degree, int *size_domSet, int *size_minDomSet, int *domSet, int *minDomSet, int *n_dominated, bool certOut, int queue[]){
//	if(DEBUG) debug(rec_level);
	
	/* If all whites are red and there is some vertex dominated 0 times, then backtrack */
//	for(int it = 0;it < *n_vertex;it++){
//		if(adjList[it].allred_dominated == 0)
//			return 1;
//	}

	if(*size_domSet+ceil((double)((double)(*n_vertex-*n_dominated)/(double)(*max_degree+1))) >= *size_minDomSet){
		return 1;
	}

	/* If is in the last level (All vertices have an status) and All vertex are dominated */

	/**************************************************************************************/
	if(*n_dominated == *n_vertex || rec_level == *n_vertex){
			if(*size_domSet < *size_minDomSet){
//				if(DEBUG){
//					printf("*** RECORD THIS SET AS THE CURRENT minDomSet ***\n");
//					printf("*** minDomSet:");
//				}
				if(certOut == false)
					printf("%d", *size_domSet);
				*size_minDomSet = *size_domSet;
				for(int i=0;i<*size_domSet;i++){
					minDomSet[i] = domSet[i];
					if(certOut == false){
						if(i != 0 && i != *size_domSet)
							printf(" ");
						printf(" %d",minDomSet[i]);
						
					}
				}
				if(certOut == false)
					printf("\n");
			}
		return 1;
	}
	
	int vertex = queue[rec_level];

	/**************************************************/
	/* Try vertex as EN_DOMSET (Excluded from domSet) */
	/* Push */
	bool undom_neighbor = false;
//	adjList[vertex].status = NE_DOMSET;
	adjList[vertex].allred_dominated = adjList[vertex].allred_dominated - 1;
	if(adjList[vertex].allred_dominated != 0){
		for(std::list<int>::iterator itt = adjList[vertex].neighbors.begin();itt != adjList[vertex].neighbors.end(); ++itt){
			adjList[*itt].allred_dominated = adjList[*itt].allred_dominated - 1;
			if(adjList[*itt].allred_dominated == 0)
				undom_neighbor = true;
		}
		/* Recursive call */
		if(!undom_neighbor && !find_minDomSet(rec_level+1, n_vertex, adjList, max_degree, size_domSet, size_minDomSet, domSet, minDomSet, n_dominated, certOut, queue))
			return 0;
	}
	/* Pop */
//	adjList[vertex].status = NOT_DECIDED;
	adjList[vertex].allred_dominated = adjList[vertex].allred_dominated + 1;
	if(adjList[vertex].allred_dominated != 1){
		for(std::list<int>::iterator itt = adjList[vertex].neighbors.begin();itt != adjList[vertex].neighbors.end(); ++itt){
			adjList[*itt].allred_dominated = adjList[*itt].allred_dominated + 1;
		}
	}
	
	/**************************************/
	/* Try vertex as E_DOMSET (In domSet) */
	/* Push */
	*size_domSet = *size_domSet + 1;
	domSet[*size_domSet - 1] = vertex;
//	adjList[vertex].status = E_DOMSET;
	if(adjList[vertex].times_dominated == 0)
		*n_dominated = *n_dominated + 1;
	adjList[vertex].times_dominated = adjList[vertex].times_dominated + 1;
	for(std::list<int>::iterator itt = adjList[vertex].neighbors.begin();itt != adjList[vertex].neighbors.end(); ++itt){
		if(adjList[*itt].times_dominated == 0)
			*n_dominated = *n_dominated + 1;
		adjList[*itt].times_dominated = adjList[*itt].times_dominated + 1;
	}
	/* Recursive call */
	if(!find_minDomSet(rec_level+1, n_vertex, adjList, max_degree, size_domSet, size_minDomSet, domSet, minDomSet, n_dominated, certOut, queue))
		return 0;
		
	if(rec_level==0)
		return 1;
	/* Pop */
	*size_domSet = *size_domSet - 1;
//	adjList[vertex].status = NOT_DECIDED;
	adjList[vertex].times_dominated = adjList[vertex].times_dominated - 1;
	if(adjList[vertex].times_dominated == 0)
		*n_dominated = *n_dominated - 1;
	for(std::list<int>::iterator itt = adjList[vertex].neighbors.begin();itt != adjList[vertex].neighbors.end(); ++itt){
		adjList[*itt].times_dominated = adjList[*itt].times_dominated - 1;
		if(adjList[*itt].times_dominated == 0)
			*n_dominated = *n_dominated - 1;
	}
	return 1;
}

int main(int argc, char **argv){
	int n_vertex,n_graph = 1, max_degree = 0;
	vertex adjList[NMAX];
	/* Variables for finding the domSet */
	int size_domSet, size_minDomSet, n_dominated;
	int queue[NMAX];
	int domSet[NMAX], minDomSet[NMAX];
	bool endOfInput = false;
	bool certOut = false;
	
	if(argc > 1){
		if(!strcmp(argv[1], "0")){
			certOut = true;
		}
		if(!strcmp(argv[1], "1")){
			certOut = false;
		}
	}
	
	do{
		endOfInput = graph_input(&n_vertex, adjList, &max_degree, &size_domSet, &size_minDomSet, domSet, minDomSet, &n_dominated, certOut, queue);
		if(endOfInput == false){
		
//			printf("Graph %d:\n",n_graph++);
			fflush(stdout);

			find_minDomSet(0, &n_vertex, adjList, &max_degree, &size_domSet, &size_minDomSet, domSet, minDomSet, &n_dominated, certOut, queue);
			if(certOut == true){
				printf("%d\n", size_minDomSet);
				for(int p=0;p<size_minDomSet;p++){
					printf(" %d", minDomSet[p]);
				}
				printf("\n");
			} else {
				printf("-1\n");
			}
			for(int p=0;p<NMAX;p++){
				queue[p] = 0;
			}
		}
	}while(endOfInput == false);
}
