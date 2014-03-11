#include <stdio.h>
#include <iostream>
#include <list>
#include <queue>
#include <math.h>
#include <string.h>

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

/* Graph input */
bool graph_input(int *n_vertex, vertex *adjList,int *max_degree, int *size_domSet, int *size_minDomSet, int *domSet, int *minDomSet, int *n_dominated, bool certOut){
	std::list<vertex>::iterator it;
	*max_degree = 0;
	*n_vertex = 0;
	/* Read number of vertices */
	if(scanf("%d",n_vertex) == EOF)
		return true;
	if(certOut)
		printf("%d\n", *n_vertex);
//	printf("Read: %d\n",*n_vertex);
	for(int i=0;i<*n_vertex;i++){
		vertex tmp;
		int tmp_vertex;
		
		tmp.status = NOT_DECIDED;
		/* Read degree of the vertex */
		scanf("%d", &tmp.degree);
		if(certOut)
			printf("%d", tmp.degree);
		if(tmp.degree > *max_degree)
			*max_degree = tmp.degree;
		tmp.allred_dominated = tmp.degree + 1;
		tmp.times_dominated = 0;
		/* Create a vertex */
		adjList[i] = tmp;
		for(int j=0;j<tmp.degree;j++){
			scanf("%d", &tmp_vertex);
			if(certOut)
				printf(" %d", tmp_vertex);
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

/*void debug(int rec_level){
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
}*/

void renameVertices(int *n_vertex, vertex *adjList, int _vertex, int *visited){
	int count = 0;
	int n_visited = 0;
	queue<int> st;
	
	st.push(_vertex);
	visited[_vertex] = 1;
	while(!st.empty()){
		/* If the vertex was not visited */
		_vertex = st.front();
		st.pop();
		if(visited[_vertex] == 0){
			vertex tmp;
			
			tmp = adjList[count];
			adjList[count] = adjList[_vertex];
			adjList[_vertex] = adjList[count];
			visited[_vertex] = 1;
			for(list<int>::iterator it = adjList[_vertex].neighbors.begin(); it != adjList[_vertex].neighbors.end();++it){
				st.push(*it);
			}
		}
		count++;
	}
	
	return;
}

int find_minDomSet(int rec_level, int *n_vertex, vertex *adjList,int *max_degree, int *size_domSet, int *size_minDomSet, int *domSet, int *minDomSet, int *n_dominated, bool certOut){
//	if(DEBUG) debug(rec_level);
	
	/* If all whites are red and there is some vertex dominated 0 times, then backtrack */
//	for(int it = 0;it < *n_vertex;it++){
//		if(adjList[it].allred_dominated == 0)
//			return 1;
//	}

	/**************************************************************************************/
	if(*n_dominated == *n_vertex){
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

	/* If is in the last level (All vertices have an status) and All vertex are dominated */
	if(rec_level == *n_vertex){
		return 1;
	}
	
	if(*size_domSet+ceil((*n_vertex-*n_dominated)/(*max_degree+1)) >= *size_minDomSet){
		return 1;
	}

	/**************************************************/
	/* Try vertex as EN_DOMSET (Excluded from domSet) */
	/* Push */
	bool undom_neighbor = false;
//	adjList[rec_level].status = NE_DOMSET;
	adjList[rec_level].allred_dominated = adjList[rec_level].allred_dominated - 1;
	for(std::list<int>::iterator itt = adjList[rec_level].neighbors.begin();itt != adjList[rec_level].neighbors.end(); ++itt){
		adjList[*itt].allred_dominated = adjList[*itt].allred_dominated - 1;
		if(adjList[*itt].allred_dominated == 0)
			undom_neighbor = true;
	}
	/* Recursive call */
	if(!undom_neighbor && !find_minDomSet(rec_level+1, n_vertex, adjList, max_degree, size_domSet, size_minDomSet, domSet, minDomSet, n_dominated, certOut))
		return 0;
	/* Pop */
//	adjList[rec_level].status = NOT_DECIDED;
	adjList[rec_level].allred_dominated = adjList[rec_level].allred_dominated + 1;
	for(std::list<int>::iterator itt = adjList[rec_level].neighbors.begin();itt != adjList[rec_level].neighbors.end(); ++itt){
		adjList[*itt].allred_dominated = adjList[*itt].allred_dominated + 1;
	}
	
	/**************************************/
	/* Try vertex as E_DOMSET (In domSet) */
	/* Push */
	*size_domSet = *size_domSet + 1;
	domSet[*size_domSet - 1] = rec_level;
//	adjList[rec_level].status = E_DOMSET;
	if(adjList[rec_level].times_dominated == 0)
		*n_dominated = *n_dominated + 1;
	adjList[rec_level].times_dominated = adjList[rec_level].times_dominated + 1;
	for(std::list<int>::iterator itt = adjList[rec_level].neighbors.begin();itt != adjList[rec_level].neighbors.end(); ++itt){
		if(adjList[*itt].times_dominated == 0)
			*n_dominated = *n_dominated + 1;
		adjList[*itt].times_dominated = adjList[*itt].times_dominated + 1;
	}
	/* Recursive call */
	if(!find_minDomSet(rec_level+1, n_vertex, adjList, max_degree, size_domSet, size_minDomSet, domSet, minDomSet, n_dominated, certOut))
		return 0;
	/* Pop */
	*size_domSet = *size_domSet - 1;
//	adjList[rec_level].status = NOT_DECIDED;
	adjList[rec_level].times_dominated = adjList[rec_level].times_dominated - 1;
	if(adjList[rec_level].times_dominated == 0)
		*n_dominated = *n_dominated - 1;
	for(std::list<int>::iterator itt = adjList[rec_level].neighbors.begin();itt != adjList[rec_level].neighbors.end(); ++itt){
		adjList[*itt].times_dominated = adjList[*itt].times_dominated - 1;
		if(adjList[*itt].times_dominated == 0)
			*n_dominated = *n_dominated - 1;
	}
}

int main(int argc, char **argv){
	int n_vertex,n_graph = 1, max_degree = 0;
	vertex adjList[NMAX];
	/* Variables for finding the domSet */
	int size_domSet, size_minDomSet, n_dominated;
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
		endOfInput = graph_input(&n_vertex, adjList, &max_degree, &size_domSet, &size_minDomSet, domSet, minDomSet, &n_dominated, certOut);
		if(endOfInput == false){
//			printf("Graph %d:\n",n_graph++);
			fflush(stdout);

			int visitedVertices[NMAX];
			for(int k=0;k<n_vertex;k++){
				visitedVertices[k] = 0;
			}
			renameVertices(&n_vertex, adjList, 0, visitedVertices);
			find_minDomSet(0, &n_vertex, adjList, &max_degree, &size_domSet, &size_minDomSet, domSet, minDomSet, &n_dominated, certOut);
			if(certOut == true){
				printf("%d\n", size_minDomSet);
				for(int p=0;p<size_minDomSet;p++){
					printf(" %d", minDomSet[p]);
				}
				printf("\n");
			} else {
				printf("-1\n");
			}
		}
	}while(endOfInput == false);
}
