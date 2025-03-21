#include <iostream>
#include <glpk.h>
#include<bits/stdc++.h>
  
using namespace std; 


int main() {

	//read and store the input
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




    // Create the LP problem
    glp_prob *lp = glp_create_prob();
    glp_set_prob_name(lp, "linear_program");
    glp_set_obj_dir(lp, GLP_MAX);

    // Add variables (mn variables in total)
	int noVar = m*n;
    glp_add_cols(lp, noVar);

    // Set variable bounds (variables: 1 to mn)
	for (int i=0; i<m; i++){
		for (int j=0; j<n; j++){
		    glp_set_col_bnds(lp, i*n+j+1, GLP_DB, lower[i][j], upper[i][j]);  
		}
	}
 
    // Set the objective function coefficients

    // Add constraints m+n one for each row, one for each column
    glp_add_rows(lp, m+n);

    // Set the constraint bounds
	for (int i=0; i<m; i++){
		glp_set_row_bnds(lp, i+1, GLP_DB, rowL[i], rowU[i]);  // row sums
	}
	for (int j=0; j<n; j++){
		glp_set_row_bnds(lp, m+j+1, GLP_DB, colL[j], colU[j]);  // column sums
	}

    // Add the coefficients for each constraint
	int ia[2*m*n+1];
	int ja[2*m*n+1];
	double ar[2*m*n+1];

	ia[0]=0;
	ja[0]=0;
	ar[0]=0;

	//adding constraints for each row
	for (int i=0; i<m; i++){
		for (int j=0; j<n; j++){
			ia[n*i+j+1]=i+1;
			ja[n*i+j+1]=n*i+j+1;
			ar[n*i+j+1]=1;
		}
	}
	//adding constraints for each column
	for (int j=0; j<n; j++){
		for (int i=0; i<m; i++){
			int index=m*n+ m*j+i+1;
			ia[index]= m+j+1;
			ja[index]= n*i+j+1;
			ar[index]= 1;
		}
	}



    glp_load_matrix(lp, 2*m*n, ia, ja, ar);
	
	double c = 1; // 1 for max, -1 for min;

	for (int i=0; i<noVar; i++){
	    glp_set_obj_coef(lp, i+1, c); 
	}


    // Solve the LP problem
    glp_simplex(lp, nullptr);

    // Get and print the results
      std::cout << "max value: " << glp_get_obj_val(lp) << std::endl;
  
	c = -1; // 1 for max, -1 for min;

	for (int i=0; i<noVar; i++){
	    glp_set_obj_coef(lp, i+1, c); 
	}


    // Solve the LP problem
    glp_simplex(lp, nullptr);

    // Get and print the results
      std::cout << "min value: " << -glp_get_obj_val(lp) << std::endl;
  


 // std::cout << "Optimal Solution:" << std::endl;
    //for (int i = 1; i <= 9; i++) {
      // std::cout << "Variable " << i << " = " << glp_get_col_prim(lp, i) << std::endl;
    //}

    // Free the problem


    glp_delete_prob(lp);

    return 0;
}

/*
int main() {
    // Create the LP problem
    glp_prob *lp = glp_create_prob();
    glp_set_prob_name(lp, "linear_program");
    glp_set_obj_dir(lp, GLP_MAX);

    // Add variables (9 variables in total)
    glp_add_cols(lp, 9);

    // Set column bounds (variables: a, b, c, d, e, f, g, h, i)
    glp_set_col_bnds(lp, 1, GLP_DB, 1800, 1900);  // a
    glp_set_col_bnds(lp, 2, GLP_DB, 1000, 1100);  // b
    glp_set_col_bnds(lp, 3, GLP_DB, 500, 600);    // c
    glp_set_col_bnds(lp, 4, GLP_DB, 1500, 1600);  // d
    glp_set_col_bnds(lp, 5, GLP_FX, 700, 700);    // e = 700
    glp_set_col_bnds(lp, 6, GLP_DB, 500, 600);    // f
    glp_set_col_bnds(lp, 7, GLP_DB, 1400, 1500);  // g
    glp_set_col_bnds(lp, 8, GLP_DB, 800, 900);    // h
    glp_set_col_bnds(lp, 9, GLP_DB, 600, 700);    // i

    // Set the objective function coefficients
	double temp = -1;
    glp_set_obj_coef(lp, 1, temp);  // a
    glp_set_obj_coef(lp, 2, temp);  // b
    glp_set_obj_coef(lp, 3, temp);  // c
    glp_set_obj_coef(lp, 4, temp);  // d
    glp_set_obj_coef(lp, 5, temp);  // e
    glp_set_obj_coef(lp, 6, temp);  // f
    glp_set_obj_coef(lp, 7, temp);  // g
    glp_set_obj_coef(lp, 8, temp);  // h
    glp_set_obj_coef(lp, 9, temp);  // i

    // Add constraints (rows)
    glp_add_rows(lp, 6);

    // Set the constraint bounds
    glp_set_row_bnds(lp, 1, GLP_DB, 3480, 3490);  // a + b + c
    glp_set_row_bnds(lp, 2, GLP_DB, 2835, 2840);  // d + e + f
    glp_set_row_bnds(lp, 3, GLP_DB, 2960, 3000);  // g + h + i
    glp_set_row_bnds(lp, 4, GLP_DB, 4870, 4895);  // a + d + g
    glp_set_row_bnds(lp, 5, GLP_DB, 2600, 2605);  // b + e + h
    glp_set_row_bnds(lp, 6, GLP_DB, 1805, 1815);  // c + f + i

    // Add the coefficients for each constraint
	int ia[19] = {0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6};
	int ja[19] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 4, 7, 2, 5, 8, 3, 6, 9};
	double ar[19] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    glp_load_matrix(lp, 18, ia, ja, ar);

    // Solve the LP problem
    glp_simplex(lp, nullptr);

    // Get and print the results
      std::cout << "optimal value: " << glp_get_obj_val(lp) << std::endl;
    std::cout << "Optimal Solution:" << std::endl;
    for (int i = 1; i <= 9; i++) {
       std::cout << "Variable " << i << " = " << glp_get_col_prim(lp, i) << std::endl;
    }

    // Free the problem
    glp_delete_prob(lp);

    return 0;
}*/
