#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "/Library/gurobi902/mac64/include/gurobi_c++.h"
using namespace std ;


struct edge
{
	int left;
	int right;
	double weight;	
};

struct vertex
{
	int size =0;
	double totweight =0.000;
	vector <int> incident;
};

int main()
{
	//Bin Packing
	freopen("store.txt", "r", stdin); // redirects standard output
	int vertice =3;
	int edges=15;
	struct edge edgevalue[edges];
	struct vertex leftvert[vertice];
	struct vertex rightvert[vertice];	
	for(int i=0;i<edges;i++)
	{
		cin>>edgevalue[i].left>>edgevalue[i].right>>edgevalue[i].weight;
		leftvert[edgevalue[i].left].totweight+=edgevalue[i].weight;
		leftvert[edgevalue[i].left].incident.push_back(i);
		leftvert[edgevalue[i].left].size++;
		rightvert[edgevalue[i].right].totweight+=edgevalue[i].weight;
		rightvert[edgevalue[i].right].incident.push_back(i);
		rightvert[edgevalue[i].right].size++;
	}
	double maxsize= 0.000;
	maxsize=max(maxsize,max(leftvert[0].totweight,leftvert[1].totweight));
	maxsize=max(maxsize,max(rightvert[0].totweight,rightvert[1].totweight));	
	int maxcolors= ceil(4.000*maxsize);

	try
	{
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		// Create Variables
		GRBVar edgecolor[edges][maxcolors];
		GRBVar colorused[maxcolors];

		//Initialising the variables
		for(int i=0;i<edges;i++)
		{
			for (int j=0; j<maxcolors; j++)
			{
				edgecolor[i][j] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_BINARY);
			}
		}

		for (int i=0; i<maxcolors; i++)
		{
			colorused[i] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_BINARY);
		}

		GRBLinExpr obj = 0;
		for(int i=0;i<maxcolors;i++)
		{
			obj += colorused[i];
		}
		model.setObjective(obj,GRB_MINIMIZE);

		//Setting the constriants
		
		for(int i=0;i<edges;i++)
		{
			GRBLinExpr packcolor = 0;
			for (int j=0;j<maxcolors;j++)
			{
				packcolor = edgecolor[i][j] - colorused[j];
				model.addConstr(packcolor<=0);
			}
		}

		
		//Each edge must be packed
		for(int i=0;i<edges;i++)
		{
			GRBLinExpr packcolor = 0;
			for (int j=0;j<maxcolors;j++)
			{
				packcolor += edgecolor[i][j] ;
			}
			model.addConstr(packcolor,GRB_EQUAL,1);
		}

		//Weight for each edge at each vertex
		
		for(int i=0;i<maxcolors;i++)
		{
			for(int j=0;j<vertice;j++)
			{
				int size = leftvert[j].size;
				GRBLinExpr edgepack = 0;
				for(int k=0;k<size;k++)
				{
					edgepack += (edgevalue[leftvert[j].incident[k]].weight * edgecolor[leftvert[j].incident[k]][i]);
				}
				model.addConstr(edgepack<=1);
			}
		}
		
		for(int i=0;i<maxcolors;i++)
		{
			for(int j=0;j<vertice;j++)
			{
				int size = rightvert[j].size;
				GRBLinExpr edgepack = 0;
				for(int k=0;k<size;k++)
				{
					edgepack += (edgevalue[rightvert[j].incident[k]].weight * edgecolor[rightvert[j].incident[k]][i]);
				}
				model.addConstr(edgepack<=1);
			}
		}

		model.optimize();
		cout << "No of colors required for offline coloring: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	}

	catch(...) 
	{
    	cout << "Exception during optimization" << endl;
  	} 
	return 0;
}



