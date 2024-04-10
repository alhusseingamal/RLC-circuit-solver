#include<iostream>
#include<cmath>
using namespace std;

#define EPSILON 1e-6

void deallocateMatrix(float **a, int n){
    for(int i = 0; i < n; i++) delete[] a[i];
    delete[] a;
}

void deallocateMatrix(int **a, int n){
    for(int i = 0; i < n; i++) delete[] a[i];
    delete[] a;
}

// assertMatrix is a function used to assert 0 values existing as very small (negative) values
void assertMatrix(float *a, int n, int m){
    for(int i = 0; i < n; i++){
        if(abs(a[i]) < EPSILON){
            a[i] = 0;
        } 
        else if(abs(a[i] - floor(a[i])) < EPSILON){
            a[i] = floor(a[i]);
        }
        else if(abs(a[i] - ceil(a[i])) < EPSILON){
            a[i] = ceil(a[i]);
        }
    }
    return;
}
void assertMatrix(float **a, int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(abs(a[i][j]) < EPSILON){
                a[i][j] = 0;
            } 
        }
    }
    return;
}
// Matrix Operations
void MatrixInverse(float **a, float **b, int n){
    int i, j, k;
    float t;
    for(i = 0; i < n; i++){ // copy matrix a to matrix b; a and b should have the same dimensions and types
        for(j = 0; j < n; j++){
            b[i][j] = a[i][j];
        }
    }
    for(i = 0; i < n ; i++){
        for(j = n; j < 2 * n; j++){
            if(i == j-n)
                b[i][j] = 1;
            else
                b[i][j] = 0;
        }
    }

    for (int i = n - 1; i > 0; i--) {
      if (b[i - 1][0] < b[i][0]) {
         float* temp = b[i];
         b[i] = b[i - 1];
         b[i - 1] = temp;
      }
   }
    for(i = 0; i < n; i++){
        t = b[i][i];
        for(j = i; j < 2 * n; j++)
            b[i][j] = b[i][j]/t;
        for(j = 0; j < n; j++){
            if(i != j){
                t = b[j][i];
                for(k = 0; k < 2 * n; k++)
                b[j][k] = b[j][k] - t * b[i][k];
            }
        }
    }
    for(i = 0; i < n; i++){
        for(j = n; j < 2 * n; j++){
            b[i][j-n] = b[i][j];
        }
    }
    assertMatrix(b, n, n);
	return;
}


int MatrixMultiplication(float **a, int r1, int c1,float *b, int r2, int c2, float *c){
    int i, j, k;
    if(c1 != r2){ // incorrect dimensions
        return -1;
    }
    for(i = 0; i < r1; i++){
        for(j = 0; j < c2; j++){
            for(k = 0; k < c1; k++){
                c[i] += a[i][k] * b[k];
            } 
        }
    }
    return 0;
}


void printMatrix(float *a, int n, int m){
    for(int i = 0; i < n; i++){
        cout << a[i] << "\t\t";  
    }
    cout << endl;
}

void printMatrix(float **a, int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << a[i][j] << "\t\t";  
        }
        cout << endl;
    }
}
void printMatrix(int *a, int n, int m){
    for(int i = 0; i < n; i++){
        cout << a[i] << "\t\t";  
    }
    cout << endl;
}

void printMatrix(int **a, int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << a[i][j] << "\t\t";
        }
        cout << endl;
    }
}

void initializeMatrix(int*a, int n, int m){
    for(int i = 0; i < n; i++)
        a[i] = 0;
}
void initializeMatrix(int**a, int n, int m){
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            a[i][j] = 0;
}
void initializeMatrix(float*a, int n, int m){
    for(int i = 0; i < n; i++)
        a[i] = 0.0;
}
void initializeMatrix(float**a, int n, int m){
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            a[i][j] = 0.0;
}