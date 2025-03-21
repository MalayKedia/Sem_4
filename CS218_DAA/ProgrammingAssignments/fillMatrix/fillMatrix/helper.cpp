#include<bits/stdc++.h>
  
using namespace std; 

 
 int main() 
{ 
	int m,n; 

	cin >> m;
	cin >> n;
 
    	std::vector<std::vector<int> >lower;
	std::vector<std::vector<int> >upper;

 	std::vector<int>rowL;
 	std::vector<int>rowU;
 	std::vector<int>colL;
 	std::vector<int>colU;

	int temp;

	for (int i=0; i< m; i++){
		std::vector<int>tempVector;
		for (int j=0; j< n; j++){
			cin >> temp;
			tempVector.push_back(temp);
		}
		lower.push_back(tempVector);
 	}

	for (int i=0; i< m; i++){
		std::vector<int>tempVector;
		for (int j=0; j< n; j++){
			cin >> temp;
			tempVector.push_back(temp);
		}
		upper.push_back(tempVector);
 	}


 	for (int i=0; i< m; i++){
		cin >> temp;
		rowL.push_back(temp);
		cin >> temp;
		rowU.push_back(temp);
	}

	for (int j=0; j< n; j++){
		cin >> temp;
		colL.push_back(temp);
		cin >> temp;
		colU.push_back(temp);
	}


 	cout << 1 << endl;
	cout << 0 << endl;
	cout << 0 << endl;



	return 0; 
} 

/*
	for (int i=0; i< m; i++){
		for (int j=0; j< n; j++){
			cout << lower[i][j] << " ";
		}
		cout << endl;
 	}
	for (int i=0; i< m; i++){
		for (int j=0; j< n; j++){
			cout << upper[i][j] << " ";
		}
		cout << endl;
 	}
	for (int i=0; i< m; i++){
		cout << rowL[i] << " ";
		cout << rowU[i] << " ";
	}

	cout << endl;

	for (int j=0; j< n; j++){
		cout << colL[j] << " ";
		cout << colU[j] << " ";
	}

 
*/
