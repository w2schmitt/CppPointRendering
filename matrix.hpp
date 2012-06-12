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
#include <math.h>
#include <string.h>
#include "vector.hpp"
#include <cassert>

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
  
public:
  /*
  void allocate();
    
public:
    // CONSTRUCTORS 
    Matrix();
    Matrix(int newmaxsize);
    Matrix(int newmaxsize, int newactualsize);
    Matrix(int newmaxsize, D* values);
    ~Matrix();

    // SETS/GETS
    void setActualSize(int newactualsize);    
    int getActualSize() const;

    void getvalue(int row, int column, D& returnvalue, bool& success);    
    D getValue(int pos) const;  
    bool setValue(int row, int column, D newvalue);
    bool setValue(int index, D newvalue);

    // COMPLEX FUNCTIONS
    void transpose();
    D trace();
    void invert();
    double determinant();
    void eigenvalues(double* eig);
    bool isSymmetric();

    // OTHERS
    void copy(Matrix&  source);
    void comparetoidentity();
    Matrix<D> operator+(const Matrix<D> &m) const;
    Matrix<D> operator-(const Matrix<D> &m) const;
    Matrix<D> operator*(const Matrix<D> &m) const;
    Matrix<D> operator*(double scalar) const;
    D &operator()(int row, int col);
    D operator()(int row, int col) const;
    
    // AUXILIARS
    
private:
    double _determinant(D **a, int n);
    void _eig3Symmetric(double *eig);
    
public:
    // FRIENDS
    //template <typename T>
    friend std::ostream& operator<< <D>(std::ostream &o, const Matrix<D> &m);        // print matrix
    //friend Matrix<D> identity <D>(int msize);
    */
    

void allocate(){
    if (data) delete[] data;
    data = new D [maxsize*maxsize];
}

//__________________________________________________ CONSTRUCTORS
Matrix(): data(NULL){}

//template<typename U>
Matrix(const Matrix<D>& m){
    data=NULL;
    //std::cout << m.getActualSize() << std::endl;
    _createMatrix(m.getActualSize(), m.getData());
}


Matrix(int newmaxsize){
    data=NULL;
    _createMatrix(newmaxsize,NULL);
}

/*
Matrix(int newmaxsize, int newactualsize){

}
*/

Matrix(int newmaxsize, D* values){
    data=NULL;
    _createMatrix(newmaxsize,values);
}


~Matrix(){
    if (data)
        delete[] data;
}

//________________________________________________ SETS/GETS METHODS

void setActualSize(int newactualsize) {
    if ( newactualsize > maxsize ){
        maxsize = newactualsize ; 
        allocate();
    }
    if (newactualsize >= 0){
        actualsize = newactualsize;
    }
}

const D* getData() const{
    return data;
}

void setData(const D* d){
    if (d==NULL){
        data = NULL;
        return;
    }
    memcpy(data, d, sizeof(D)*(getActualSize()*getActualSize()));
}

int getActualSize() const { return actualsize; };


void getvalue(int row, int column, D& returnvalue, bool& success)   {
    if ( (row>=maxsize) || (column>=maxsize) || (row<0) || (column<0) ){
        success = false;
        return;
    }

    returnvalue = data[row*maxsize + column];
    success = true;
};


D getValue(int pos) const{
    if ((pos>=maxsize*maxsize) || (pos<0)){
        std::cerr << "[WARNING] Matrix access outise the limits." << std::endl;
        return 0;
    }
    return data[pos];
}


bool setValue(int row, int column, D newvalue)  {
    if ( (row >= maxsize) || (column >= maxsize) || (row<0) || (column<0) ) 
        return false;
    
    data[row*maxsize+column] = newvalue;
    return true;
}


bool setValue(int index, D newvalue)  {
    if ( (index >= maxsize*maxsize) || (index<0)) 
        return false;
    
    data[index] = newvalue;
    return true;
}


  



//________________________________________________ COMPLEX COMPUTATION


D trace(){
    D sum=0;
    for (int i=0; i<maxsize; ++i)
        sum+= (*this)(i,i);
    
    return sum;
}


void eigenvalues(double* eigval, double* eigvec){
    int msize = getActualSize();
    
    if (msize==3 && isSymmetric()){
        //std::cout << "teste" << std::endl;
        _eig3Symmetric(eigval);
        double b[3] = {0,0,0};
        
        //(*this - Matrix<D>::identity(3)*eigval[0])._solve(b.eig)
        double max = 100000;
        for (int i=0; i<3; i++){
            if (eigval[i] < max){
                max = eigval[i];
            }           
        }
        //std::cout << max << std::endl;
        (*this - Matrix<D>::identity(3)*max)._solve(b,eigvec);
        //std::cout << eigvec[0] << std::endl;
        return;
    }
    //eigenvalues
    
}


bool isSymmetric(){
    for (int i=0; i<getActualSize(); ++i){
        for (int j=0; j<getActualSize(); j++){
            //std::cout << (*this)(i,j) << " " << (*this)(j,i) << std::endl;
            if (i==j) continue;
            double value = (*this)(i,j)-(*this)(j,i);
            if ( fabs(value) > EPS) return false;
        }
    }
    
    return true;
}


/* This function calculates the inverse of a square matrix
*  Notes:
*  - the matrix must be invertible
*  - there's no pivoting of rows or columns, hence,
*        accuracy might not be adequate for your needs.
*
*  Code is rewritten from c++ template code Mike Dinolfo
*/


void invert() {

    /* Loop variables */
    int i, j, k;
    /* Sum variables */
    double sum,x;
       
    /* Add small value to diagonal if diagonal is zero */
    for(i=0; i<actualsize; i++)
    { 
        j=i*actualsize+i;
        if((data[j]<1e-12)&&(data[j]>-1e-12)){ data[j]=1e-12; }
    }
    
    /* Matrix size must be larger than one */
    if (actualsize <= 1) return;
    
    for (i=1; i < actualsize; i++) {
        data[i] /= data[0]; /* normalize row 0 */
    }
    
    for (i=1; i < actualsize; i++)  {
        for (j=i; j < actualsize; j++)  { /* do a column of L */
            sum = 0.0;
            for (k = 0; k < i; k++) {
                sum += data[j*actualsize+k] * data[k*actualsize+i];
            }
            data[j*actualsize+i] -= sum;
        }
        if (i == actualsize-1) continue;
        for (j=i+1; j < actualsize; j++)  {  /* do a row of U */
            sum = 0.0;
            for (k = 0; k < i; k++) {
                sum += data[i*actualsize+k]*data[k*actualsize+j];
            }
            data[i*actualsize+j] = (data[i*actualsize+j]-sum) / data[i*actualsize+i];
        }
    }
    for ( i = 0; i < actualsize; i++ )  /* invert L */ {
        for ( j = i; j < actualsize; j++ )  {
            x = 1.0;
            if ( i != j ) {
                x = 0.0;
                for ( k = i; k < j; k++ ) {
                    x -= data[j*actualsize+k]*data[k*actualsize+i];
                }
            }
            data[j*actualsize+i] = x / data[j*actualsize+j];
        }
    }
    for ( i = 0; i < actualsize; i++ ) /* invert U */ {
        for ( j = i; j < actualsize; j++ )  {
            if ( i == j ) continue;
            sum = 0.0;
            for ( k = i; k < j; k++ ) {
                sum += data[k*actualsize+j]*( (i==k) ? 1.0 : data[i*actualsize+k] );
            }
            data[i*actualsize+j] = -sum;
        }
    }
    for ( i = 0; i < actualsize; i++ ) /* final inversion */ {
        for ( j = 0; j < actualsize; j++ )  {
            sum = 0.0;
            for ( k = ((i>j)?i:j); k < actualsize; k++ ) {
                sum += ((j==k)?1.0:data[j*actualsize+k])*data[k*actualsize+i];
            }
            data[j*actualsize+i] = sum;
        }
    }
}



double determinant(){
    int n = getActualSize();
    D **a = new D*[n];
    assert(a != NULL);
    
    for (int i = 0 ; i < n ; i++){
        a[i] = new D[n];
        assert(a[i] != NULL);
    }
        //a[i] = (D*)malloc(n*sizeof(D));        
        //a[i] = (D *) malloc(actualsize* sizeof(D)) ;

    double value;
    bool success;
    for (int i = 0 ; i < n ; i++){
        for (int j = 0; j < n; j++){
            //getvalue(i,j,value,success);
            a[i][j] = (*this)(i,j);
        }
    }

    double det = _determinant(a,n);
    //double det = 10.;
    //std::cout << det << std::endl;

    for (int i = 0 ; i < n ; i++) delete[] a[i];
    delete[] a;
    // compute determinant recursively
    return det;
}

//_________________________________________________ OTHERS



friend std::ostream& operator<<(std::ostream& o, const Matrix<D> &m)
{
    o << "[";
    int size = m.getActualSize();
    int totalsize = size*size;
    for (int i=0; i<totalsize; ++i){
        if ( (i!=0) && (i%(size) == 0)) o << "; ";
        o << m.getValue(i) << " ";
    }
    o << "]";
        
    return o;
}


static Matrix<D> identity(int msize){
    D* values = new D[msize*msize];
    for (int i=0; i<msize; i++){
        for (int j=0; j<msize; j++){
            if (i==j) values[i*msize+j] = (D)1;
            else values[i+msize+j] = (D)0;
        }
    }
    
    Matrix<D> result(msize,values);
    delete[] values;
    
    return result;
}


Matrix<D> operator+(const Matrix<D> &m) const{
    Matrix<D> result = *this;
    int msize = m.getActualSize();

    if (msize == result.getActualSize()){
        for (int i=0; i<msize*msize; ++i){
            result.setValue(i, result.getValue(i) + m.getValue(i));
        }
    }
    else {
        std::cout << "[ERROR] Operation of incompatible Matrix size!" << std::endl;
    }
    return result;
}


Matrix<D> operator-(const Matrix<D> &m) const{
    Matrix<D> result = *this;
    int msize = m.getActualSize();
    
    if (msize == getActualSize()){
        for (int i=0; i<msize*msize; ++i){
            result.setValue(i, result.getValue(i) - m.getValue(i));
        }
    }
    else {
        std::cout << "[ERROR] Operation of incompatible Matrix size!" << std::endl;
    }
    return result;
}



Matrix<D> operator*(const Matrix<D> &m) const{

    int msize = m.getActualSize();
    Matrix<D> result(msize);
    
    if (msize == getActualSize()){
        for(int i=0; i<msize; ++i){
            for(int j=0; j<msize; ++j){
                for(int k=0; k<msize; ++k){
                    result(i,j) = result(i,j) + (*this)(i,k)*m(k,j);
                }
            }
        }
    }
    else {
        std::cout << "[ERROR] Operation of incompatible Matrix size!" << std::endl;
    }
    
    return result;
}


friend Matrix<D> operator*(double scalar, const Matrix<D> &m){
    Matrix<D> result(m);
    for (int i=0; i< m.getActualSize()*m.getActualSize(); ++i){
        result.setValue(i, scalar*result.getValue(i));
    }
    
    return result;
}

friend Matrix<D> operator*(const Matrix<D> &m, double scalar){
    Matrix<D> result(m);
    for (int i=0; i< m.getActualSize()*m.getActualSize(); ++i){
        result.setValue(i, scalar*result.getValue(i));
    }
    
    return result;
}


D operator()(int row, int col) const{
    int index = row*getActualSize() + col;
    return getValue(index);
} 


D &operator()(int row, int col){
    int index = row*getActualSize() + col;
    return this->data[index];
}

Matrix<D> &operator=(const Matrix<D> &m){
    const D* d = m.getData();
    setActualSize(m.getActualSize());
    setData(d);
    return *this;
}


void copy(Matrix&  source){
    actualsize = source.getActualSize();
    
    if ( maxsize < source.getActualSize() ){
        maxsize = source.getActualSize();
        allocate();
    }
    
    for ( int i = 0; i < actualsize; i++ ){
        for ( int j = 0; j < actualsize; j++ ){
            D value;
            bool success;
            source.getvalue(i,j,value,success);
            data[i*maxsize+j] = value;
        }
    }
}

private:
    
void _createMatrix(int n, const D* d){
    if (n<0)
        std::cout << "[ERROR]: Creating matrix with negative size!" << std::endl;
    
    actualsize = maxsize = n;
    allocate();
    
    if (!d){
        for (int i=0; i<n*n; ++i){ data[i]=0; }
    }
    else{    
        memcpy(data, d, sizeof(D)*n*n);
    }
}

 
    
void swapLine(int _i, int _j){
    int nsize = this->getActualSize();
    D aux = 0;
    //std::cout << "before: "<< *this << std::endl;
    for (int k=0; k<nsize; ++k){
        aux = (*this)(_i,k);
        (*this)(_i,k) = (*this)(_j,k);
        (*this)(_j,k) = aux;
        
    }
    
    //std::cout << "after: "<< *this << std::endl;
}

void swap(D* vec, int _i, int _j){
    
    D aux = 0;
    aux = vec[_i];
    vec[_i] = vec[_j];
    vec[_j] = aux;
}

Matrix<D> _solve(D* b, D* sol){
    //std::cout << EPS << std::endl;
    Matrix<D> A = *this;
    int n = A.getActualSize();
    
    for (int i=0; i<n-1; i++){
        for (int j=i+1; j<n; j++){
            // find another pivot
            if (fabs(A(i,i))<1.E-4){
                A(i,i)=0;
                bool found = false;
                for (int k=j; k<n; k++){
                    if (fabs(A(k,i))>1.E-4){
                        A.swapLine(i,k);
                        swap(b,i,k);
                        found = true;
                        break;
                    }                     
                }     
                if (!found) continue;
            }

            double mult = -A(j,i)/A(i,i);
            for (int k=i; k<n; k++){
                A(j,k) = mult*A(i,k)  + A(j,k);
            }
            b[j] = mult*b[i] + b[j];  
        }
    }
    
    //std::cout << A << std::endl;
    // RETRO SUBSTITUITION with free var    
    for (int i=n-1; i>=0; --i){
        for (int j=i+1; j<n+1; ++j){
            if (fabs(A(i,j-1))<1.E-4){     
                A(i,j-1)=0;
                sol[j-1] = 1;
                if ((j-1)>=n-1) break;
                continue;
            }
            for (int k=j; k<n; k++)
                b[i] = b[i] - A(i,k)*sol[k];
            sol[j-1] = b[i]/A(i,j-1);
            break;
        }
    }
    
    return A;    
}

//Given symmetric 3x3 matrix M, compute the eigenvalues
void _eig3Symmetric(double *eig){  
    //std::cout << "-" << std::endl;
    //std::cout << "fuck\n";
    
    double m = trace()/3.0;    
    Matrix<D> K = (*this) - Matrix<D>::identity(3)*m;
    double q = K.determinant()/2;
    
    double p = 0;
    for (int i=0; i<3; ++i){
        for (int j=0; j<3; ++j){
            p += K(i,j)*K(i,j);
        }
    }
    p = p/6.0;
    assert(p>=0);
    //std::cout << p << std::endl;
    double phi = (1./3.)*acos(q/pow( p, 3./2. ));
    
    if(fabs(q) >= abs(pow(p, 3./2.))){
        phi = 0;
    }

    if(phi<0){
        phi+=M_PI/3.;
    }    
    
    eig[0] = m + 2*sqrt(p)*cos(phi);
    eig[1] = m - sqrt(p)*(cos(phi) + sqrt(3)*sin(phi));
    eig[2] = m - sqrt(p)*(cos(phi) - sqrt(3)*sin(phi));
}


double _determinant(double **a, int n)
{
    int i,j,j1,j2 ;                    // general loop and matrix subscripts
    double det = 0 ;                   // init determinant

    if (n < 1)    {   }                // error condition, should never get here

    else if (n == 1) {                 // should not get here
        det = a[0][0] ;
        }
    else if (n == 2)  {                // basic 2X2 sub-matrix determinate
                                    // definition. When n==2, this ends the
        det = a[0][0] * a[1][1] - a[1][0] * a[0][1] ;// the recursion series
        }                                    // recursion continues, solve next sub-matrix
    else {                             // solve the next minor by building a
                                    // sub matrix
        det = 0 ;                      // initialize determinant of sub-matrix
                                        // for each column in sub-matrix
        for (j1 = 0 ; j1 < n ; j1++) {// get space for the pointer list
            D **m =  new D*[n-1] ;

            //double **m = (double**) malloc((n-1)*sizeof(double*));
            for (i = 0 ; i < n-1 ; i++)
                //m[i] = (double*) malloc((n-1)*sizeof(double));
                m[i] = new D[n-1];

                    // build sub-matrix with minor elements excluded
            for (i = 1 ; i < n ; i++) {
                j2 = 0 ;               // start at first sum-matrix column position
                                    // loop to copy source matrix less one column
                for (j = 0 ; j < n ; j++) {
                    if (j == j1) continue ; // don't copy the minor column element

                    m[i-1][j2] = a[i][j] ;  // copy source element into new sub-matrix
                                            // i-1 because new sub-matrix is one row
                                            // (and column) smaller with excluded minors
                    j2++ ;                  // move to next sub-matrix column position
                    }
                }

            det += pow(-1.0,1.0 + j1 + 1.0) * a[0][j1] * _determinant(m,n-1) ;
                                            // sum x raised to y power
                                            // recursively get determinant of next
                                            // sub-matrix which is now one
                                            // row & column smaller

            for (i = 0 ; i < n-1 ; i++) {delete[] m[i];}// free the storage allocated to
                                            // to this minor's set of pointers
            delete [] m;                       // free the storage for the original
                                            // pointer to pointer
        }
    }
    return(det) ;
}


};




#endif	/* MATRIX_H */

