/* 
 * File:   matrix.h
 * Author: wagner
 *
 * Created on 25 de Maio de 2012, 16:59
 */

#ifndef MATRIX_H
#define	MATRIX_H

// header file for matrix template class
// NOTE:  all matrices handled here must be SQUARE 

#include <iostream>
#include <cmath>

using namespace std;

// generic object (class) definition of matrix:
template <typename D>
class Matrix
{
    
  // NOTE: maxsize determines available memory storage, but
  // actualsize determines the actual size of the stored matrix in use
  // at a particular time.
    
  int maxsize;          // max number of rows (same as max number of columns)
  int actualsize;       // actual size (rows, or columns) of the stored matrix
  D* data;              // where the data contents of the matrix are stored
  
  void allocate();
    
public:
    // CONSTRUCTORS 
    Matrix();
    Matrix(int newmaxsize);
    Matrix(int newmaxsize, int newactualsize);    
    ~Matrix();

    // SETS/GETS
    void setActualSize(int newactualsize);    
    int getActualSize();

    void getvalue(int row, int column, D& returnvalue, bool& success);    
    D getValue(int pos);  
    bool setvalue(int row, int column, D newvalue);

    // COMPLEX FUNCTIONS
    void invert();
    double determinant();

    // OTHERS
    void copy(Matrix&  source);
    void comparetoidentity();
    std::ostream& operator<<(std::ostream& o) const;        // print matrix
  
    // AUXILIARS
private:
    double _determinant(D **a, int n);
    

    
};

#endif	/* MATRIX_H */

