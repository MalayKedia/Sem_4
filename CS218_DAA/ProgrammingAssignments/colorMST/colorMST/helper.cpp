#include<bits/stdc++.h>
  
using namespace std; 

// Creating shortcut for an integer pair 
typedef pair<int, int> iPair;
 
// Structure to represent a graph 
struct Graph 
{ 
	int V, E;
 	vector< pair<int, iPair> > edges; 

	// Constructor 
	Graph(int V, int E) 
	{ 
		this->V = V; 
		this->E = E; 
 	} 

	// Utility function to add an edge  // red=1 means red
	void addEdge(int u, int v, int w, int red) 
	{ 
		edges.push_back({w, {u, v}});
	} 

}; 

 int main() 
{ 
	int V, E; 
	int threshold;  

	cin >> V;
	cin >> E;
	cin >> threshold;
	Graph g(V, E); 
 

	int u, v, w, r;

	for (int i=0; i< E; i++){
		cin >> u;
		cin >> v;
		cin >> w;
		cin >> r;
		g.addEdge(u, v, w, r); 
 	}

 	cout << 0 << endl;
	cout << 0 << endl;

 

	return 0; 
} 

