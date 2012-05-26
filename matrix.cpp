
#include "matrix.h"
template <typename D>
void Matrix<D>::allocate(){
    delete[] data;
    data = new D [maxsize*maxsize];
}

//__________________________________________________ CONSTRUCTORS
template <typename D>
Matrix<D>::Matrix(){};

template <typename D>
Matrix<D>::Matrix(int newmaxsize){
    Matrix(newmaxsize,newmaxsize);
}

template <typename D>
Matrix<D>::Matrix(int newmaxsize, int newactualsize){
    if (newmaxsize <= 0) 
        newmaxsize = 5;
    
    maxsize = newmaxsize;
    
    if ((newactualsize <= newmaxsize)&&(newactualsize>0))
      actualsize = newactualsize;
    else 
      actualsize = newmaxsize;
    
    // since allocate() will first call delete[] on data:
    data = NULL;
    allocate();
}

template <typename D>
Matrix<D>::~Matrix(){
    delete[] data;
}

//________________________________________________ SETS/GETS METHODS
template <typename D>
void Matrix<D>::setActualSize(int newactualsize) {
    if ( newactualsize > maxsize ){
        maxsize = newactualsize ; 
        allocate();
    }
    if (newactualsize >= 0){
        actualsize = newactualsize;
    }
}

template <typename D>
int Matrix<D>::getActualSize() { return actualsize; };

template <typename D>
void Matrix<D>::getvalue(int row, int column, D& returnvalue, bool& success)   {
    if ( (row>=maxsize) || (column>=maxsize) || (row<0) || (column<0) ){
        success = false;
        return;
    }

    returnvalue = data[row*maxsize + column];
    success = true;
};

template <typename D>
D Matrix<D>::getValue(int pos){
    if ((pos>=maxsize*maxsize) || (pos<maxsize)){
        std::cerr << "[WARNING] Matrix access outise the limits." << std::endl;
        return 0;
    }
    return data[pos];
}

template <typename D>
bool Matrix<D>::setvalue(int row, int column, D newvalue)  {
    if ( (row >= maxsize) || (column >= maxsize) || (row<0) || (column<0) ) 
        return false;
    
    data[row*maxsize+column] = newvalue;
    return true;
};
  







//________________________________________________ COMPLEX COMPUTATION

/* This function calculates the inverse of a square matrix
*  Notes:
*  - the matrix must be invertible
*  - there's no pivoting of rows or columns, hence,
*        accuracy might not be adequate for your needs.
*
*  Code is rewritten from c++ template code Mike Dinolfo
*/

template <typename D>
void Matrix<D>::invert() {

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


template <typename D>
double Matrix<D>::determinant(){
    D **a = NULL ;
    a = new D*[actualsize];
    //a = (D **) malloc(actualsize* sizeof(D *)) ;

    for (int i = 0 ; i < actualsize ; i++)
        a[i] = new D[actualsize];
        //a[i] = (D *) malloc(actualsize* sizeof(D)) ;

    D value;
    bool success;
    for (int i = 0 ; i < actualsize ; i++){
        for (int j = 0; j < actualsize; j++){
            getvalue(i,j,value,success);
            a[i][j] = value;
        }
    }

    double det = _determinant(a,actualsize);

    for (int i = 0 ; i < actualsize ; i++) delete[] a[i];
    delete[] a;
    // compute determinant recursively
    return det;
}

template <typename D>
double Matrix<D>::_determinant(D **a, int n)
{
    int i,j,j1,j2 ;                    // general loop and matrix subscripts
    double det = 0 ;                   // init determinant
    D **m = NULL ;                // pointer to pointers to implement 2d

    // square array


    if (n < 1)    {   }                // error condition, should never get here

    else if (n == 1) {                 // should not get here
        det = a[0][0] ;
        }

    else if (n == 2)  {                // basic 2X2 sub-matrix determinate
                                    // definition. When n==2, this ends the
        det = a[0][0] * a[1][1] - a[1][0] * a[0][1] ;// the recursion series
        }


                                    // recursion continues, solve next sub-matrix
    else {                             // solve the next minor by building a
                                    // sub matrix
        det = 0 ;                      // initialize determinant of sub-matrix

                                        // for each column in sub-matrix
        for (j1 = 0 ; j1 < n ; j1++) {
                                        // get space for the pointer list
            m = new D*[n-1];

            for (i = 0 ; i < n-1 ; i++)
                m = new D[n-1];
                    //     i[0][1][2][3]  first malloc
                    //  m -> +  +  +  +   space for 4 pointers
                    //       |  |  |  |          j  second malloc
                    //       |  |  |  +-> _ _ _ [0] pointers to
                    //       |  |  +----> _ _ _ [1] and memory for
                    //       |  +-------> _ a _ [2] 4 doubles
                    //       +----------> _ _ _ [3]
                    //
                    //                   a[1][2]
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
            delete[] m ;                       // free the storage for the original
                                            // pointer to pointer
        }
    }
    return(det) ;
}





//_________________________________________________ OTHERS
template <typename D>
std::ostream& Matrix<D> ::operator<<(std::ostream& o) const
{
    o << "[";
    int totalsize = actualsize*actualsize;
    for (int i=0; i<totalsize; ++i){
        if ( (i!=0) && (i!=actualsize-1) && (i%(actualsize-1) == 0)) o << "; ";
        o << data[i] << " ";
    }
    o << "]" << std::endl;
        
    return o;
}

template <typename D>
void Matrix<D>::copy(Matrix&  source){
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



/*
void Matrix::comparetoidentity()  {
    int worstdiagonal = 0;
    D maxunitydeviation = 0.0;
    D currentunitydeviation;
    for ( int i = 0; i < actualsize; i++ )  {
      currentunitydeviation = data[i*maxsize+i] - 1.;
      if ( currentunitydeviation < 0.0) currentunitydeviation *= -1.;
      if ( currentunitydeviation > maxunitydeviation )  {
           maxunitydeviation = currentunitydeviation;
           worstdiagonal = i;
           }
      }
    int worstoffdiagonalrow = 0;
    int worstoffdiagonalcolumn = 0;
    D maxzerodeviation = 0.0;
    D currentzerodeviation ;
    for ( int i = 0; i < actualsize; i++ )  {
      for ( int j = 0; j < actualsize; j++ )  {
        if ( i == j ) continue;  // we look only at non-diagonal terms
        currentzerodeviation = data[i*maxsize+j];
        if ( currentzerodeviation < 0.0) currentzerodeviation *= -1.0;
        if ( currentzerodeviation > maxzerodeviation )  {
          maxzerodeviation = currentzerodeviation;
          worstoffdiagonalrow = i;
          worstoffdiagonalcolumn = j;
          }

        }
      }
    cout << "Worst diagonal value deviation from unity: " 
       << maxunitydeviation << " at row/column " << worstdiagonal << endl;
    cout << "Worst off-diagonal value deviation from zero: " 
       << maxzerodeviation << " at row = " << worstoffdiagonalrow 
       << ", column = " << worstoffdiagonalcolumn << endl;
}
 */

  /*
  void settoproduct(Matrix& left, Matrix& right)  {
    actualsize = left.getactualsize();
    if ( maxsize < left.getactualsize() )   {
      maxsize = left.getactualsize();
      allocate();
      }
    for ( int i = 0; i < actualsize; i++ )
      for ( int j = 0; j < actualsize; j++ )  {
        D sum = 0.0;
        D leftvalue, rightvalue;
        bool success;
        for (int c = 0; c < actualsize; c++)  {
            left.getvalue(i,c,leftvalue,success);
            right.getvalue(c,j,rightvalue,success);
            sum += leftvalue * rightvalue;
            }
        setvalue(i,j,sum);
        }
  }
   */


