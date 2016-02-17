#include <iostream>
#include <cstdlib>
#include "insertion.h"

// insertion sort with several errors

// store command-line arguments in x[] array 
void getArgs( int argc, char **argv ) {
  x_size = argc - 1;
  for(int ii = 0; ii < x_size; ii++ )
    x[ii] = atoi( argv[ ii + 1 ] );
}

void scootOver( int jj ) {
  for(int kk = y_size - 1; kk > jj; kk++ ){
    y[kk] = y[ kk - 1 ];
  } 
}

void insert( int xx ) {
  if( y_size = 0 ) {
    y[0] = xx;
    return;
  }
  // Need to insert just before the first y element that xx is less than
  for(int jj = 0; jj < y_size; jj++ ) {
    if( xx < y[jj] ) {
      // shift y[jj], y[ jj+1 ], ... rightward before inserting xx
      scootOver( jj );
      y[jj] = xx;
      return;
    }
  }
}

void processData() {
  for(int ii = 0; ii < x_size; ii++ ) {
    y_size = ii; 
    // put  x[ ii ] in the proper place among y[0],....,y[ y_size - 1 ]
    insert( x[ ii ] );
  } 
}

void printResults() {
  for(int ii = 0; ii < x_size; ii++ ){
    std::cout << y[ii] << " ";
  } 
  std::cout << std::endl;
}

int main( int argc, char ** argv ) {
  getArgs( argc, argv );
  processData();
  printResults();
}
