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
	vector <double> incident;
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
		leftvert[edgevalue[i].left].incident.push_back(edgevalue[i].weight);
		leftvert[edgevalue[i].left].size++;
		rightvert[edgevalue[i].right].totweight+=edgevalue[i].weight;
		rightvert[edgevalue[i].right].incident.push_back(edgevalue[i].weight);
		rightvert[edgevalue[i].right].size++;
	}
	
	double maxsize= 0.000;
	int maxedge_incident= 0;
	for(int i=0;i<vertice;i++)
	{
		maxsize = max(maxsize,max(leftvert[i].totweight,rightvert[i].totweight));
		maxedge_incident = max(maxedge_incident,max(leftvert[i].size,rightvert[i].size));
	}
	int binno= ceil(2.000*maxsize);
	try
	{
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);
		
		//Create variables
		GRBVar leftbin[vertice][maxedge_incident][binno];
		GRBVar rightbin[vertice][maxedge_incident][binno];
		GRBVar y[binno];
		
		for(int i=0;i<vertice;i++)
		{
			for(int j=0;j<leftvert[i].size;j++)
			{
				for(int k=0;k<binno;k++)
				{
					leftbin[i][j][k]= model.addVar(0.0, GRB_INFINITY, 0.0, GRB_BINARY);
				}
			}
		}

		for(int i=0;i<vertice;i++)
		{
			for(int j=0;j<rightvert[i].size;j++)
			{
				for(int k=0;k<binno;k++)
				{
					rightbin[i][j][k]= model.addVar(0.0, GRB_INFINITY, 0.0, GRB_BINARY);
				}
			}
		}

		for(int j=0;j<binno;j++)
		{
			y[j]= model.addVar(0.0, GRB_INFINITY, 0.0, GRB_BINARY);
		}

		//Setting thw objective
		GRBLinExpr obj = 0;
		for(int i=0;i<binno;i++)
		{
			obj += y[i];
		}
		model.setObjective(obj,GRB_MINIMIZE);

		//Now to add Constaints
		// First for all objects to be convered by a bin
		
		for(int i=0;i<vertice;i++)
		{
			for(int j=0;j<leftvert[i].size;j++)
			{
				GRBLinExpr binpack=0;
				for(int k=0;k<binno;k++)
				{
					binpack += leftbin[i][j][k];
				}
				model.addConstr(binpack,GRB_EQUAL,1);
			}
		}
		
		for(int i=0;i<vertice;i++)
		{
			for(int j=0;j<rightvert[i].size;j++)
			{
				GRBLinExpr binpack=0;
				for(int k=0;k<binno;k++)
				{
					binpack += rightbin[i][j][k];
				}
				model.addConstr(binpack,GRB_EQUAL,1);
			}
		}

		// Now for the weight constraint
		
		for(int i=0;i<vertice;i++)
		{
			for(int j=0;j<binno;j++)
			{
				GRBLinExpr weightconstraint= 0;
				for(int k=0;k<leftvert[i].size;k++)
				{
					weightconstraint += (leftvert[i].incident[k]*leftbin[i][k][j]);
				}
				model.addConstr(weightconstraint<=1);
			}
		}

		for(int i=0;i<vertice;i++)
		{
			for(int j=0;j<binno;j++)
			{
				GRBLinExpr weightconstraint= 0;
				for(int k=0;k<rightvert[i].size;k++)
				{
					weightconstraint += (rightvert[i].incident[k]*rightbin[i][k][j]);
				}
				model.addConstr(weightconstraint<=1);
			}
		}

		// Final set of Constraints
		
		for(int i=0;i<vertice;i++)
		{
			for(int j=0;j<leftvert[i].size;j++)
			{
				for(int k=0;k<binno;k++)
				{
					GRBLinExpr existcolor = 0;
					existcolor += leftbin[i][j][k];
					existcolor -= y[k];
					model.addConstr(existcolor <= 0);		
				}
			}
		}

		for(int i=0;i<vertice;i++)
		{
			for(int j=0;j<rightvert[i].size;j++)
			{
				for(int k=0;k<binno;k++)
				{
					GRBLinExpr existcolor = 0;
					existcolor += rightbin[i][j][k];
					existcolor -= y[k];
					model.addConstr(existcolor <= 0);		
				}
			}
		}

		model.optimize();
		cout << "No of bins required for each vertex: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	}

	catch(...) 
	{
    	cout << "Exception during optimization" << endl;
  	} 
	return 0;
}