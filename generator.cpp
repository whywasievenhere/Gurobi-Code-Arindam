#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int vertice= 3; // left and right vertice are 2 in number

struct edge
{
	int left;
	int right;
	double weight;	
};


int main()
{
	double edgeweights[5]= {0.3,0.4,0.5,0.6,0.7};
	int edges=15;
	struct edge graphedge[edges];
	int storenum[3*edges];
	srand(static_cast<unsigned int>(time(nullptr))); // set initial seed value to system clock

	for(int i=0;i<3*edges;i++)
	{
		storenum[i]=rand();
	}

	int temp;
	for(int i=0;i<edges;i++)
	{
		graphedge[i].left= (storenum[3*i])%vertice;
		graphedge[i].right= (storenum[3*i+1])%vertice;		
		temp = (storenum[3*i+2])%5;
		graphedge[i].weight  = edgeweights[temp];
	}

	for(int i=0;i<edges;i++)
	{
		cout<<graphedge[i].left<<" "<<graphedge[i].right<<" "<<graphedge[i].weight<<endl;
	}
	return 0;
}
