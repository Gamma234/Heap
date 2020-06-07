# include <iostream>
# include <string>
# include <fstream>
# include <vector>
# include "Deap.h"

using namespace std ;

struct Data {
  int num ;
  Data() : num( 0 ) {} // constructor
  Data( int n ) : num( n ) {} // constructor
} ; // struct Data

bool operator < ( Data data1, Data data2 ) ;

int* GetFileData( string fileName, int& size ) ;
Data* IntPtrToDataPtr( int* intPtr, int size ) ;

template<class Type>
void BuildHeapAndSort( Type* dataPtr, int size, bool smallToBig ) ;

void Print( fstream& outFile, int* intPtr, int size, bool smallToBig ) ;
void Print( fstream& outFile, Data* dataPtr, int size, bool smallToBig ) ;

int main() {

  vector<string> fileNameBox ;
  string fileName = "" ;
  int size = 0 ;
  int* intPtr = NULL ;
  Data* dataPtr = NULL ;

  fstream inFile( "test_data//TOTAL_inputFileName.txt", ios::in ) ;

  if ( !inFile ) {
    cout << "TOTAL_inputFileName.txt does not exist !" << endl ;
  } // if
  else {

    while ( inFile >> fileName ) {
      fileNameBox.push_back( fileName ) ;
    } // while

  } // else

  inFile.close() ;

  for ( int i = 0 ; i < fileNameBox.size() ; i++ ) {

    fileName = fileNameBox[i] ;
    size = 0 ;
    intPtr = GetFileData( ( "test_data//" + fileName ), size ) ;
    dataPtr = IntPtrToDataPtr( intPtr, size ) ;

    BuildHeapAndSort<int>( intPtr, size, true ) ;
    BuildHeapAndSort<Data>( dataPtr, size, true ) ;

    fstream outFile( ( "output_" + fileName ).c_str(), ios::out ) ;

    Print( outFile, intPtr, size, true ) ;
    Print( outFile, dataPtr, size, true ) ;

    outFile.close() ;

    size = 0 ;
    intPtr = GetFileData( ( "test_data//" + fileName ), size ) ;
    dataPtr = IntPtrToDataPtr( intPtr, size ) ;

    BuildHeapAndSort<int>( intPtr, size, false ) ;
    BuildHeapAndSort<Data>( dataPtr, size, false ) ;

    fstream outFileBS( ( "outputBS_" + fileName ).c_str(), ios::out ) ;

    Print( outFileBS, intPtr, size, false ) ;
    Print( outFileBS, dataPtr, size, false ) ;

    outFileBS.close() ;

    delete[] intPtr ;
    intPtr = NULL ;
    delete[] dataPtr ;
    dataPtr = NULL ;

  } // for

} // main()

bool operator < ( Data data1, Data data2 ) {

  return data1.num < data2.num ;

} // operator <

template<class Type>
void BuildHeapAndSort( Type* dataPtr, int size, bool smallToBig ) {

  Deap<Type>* mmHeap = new Deap<Type>() ;

  Type data ;

  for ( int i = 0 ; i < size ; i++ ) {
    mmHeap->Insert( dataPtr[i] ) ;
  } // for  

  if ( smallToBig ) {

    for ( int i = 0 ; mmHeap->GetMinAndDelete( data ) ; i++ ) {
      dataPtr[i] = data ;
    } // for

  } // if
  else {

    for ( int i = 0 ; mmHeap->GetMaxAndDelete( data ) ; i++ ) {
      dataPtr[i] = data ;
    } // for

  } // else  

  delete mmHeap ;
  mmHeap = NULL ;

} // BuildHeapAndSort()

int* GetFileData( string fileName, int& size ) {

  size = 0 ;

  fstream inFile( fileName.c_str(), ios::in ) ;

  if ( !inFile ) {
    cout << "File not exist !" << endl ;
    return NULL ;
  } // if

  string intro = "" ;
  int num = 0 ;
  vector<int> numBox ;
  int* numPtr = NULL ;

  getline( inFile, intro ) ; // first line is an introduction  

  while ( inFile >> num ) {
    numBox.push_back( num ) ;
  } // while

  inFile.close() ;

  size = numBox.size() ;

  cout << "Data Size : " << size << endl ;

  numPtr = new int[size] ;

  for ( int i = 0 ; i < size ; i++ ) {
    numPtr[i] = numBox[i] ;
  } // for

  return numPtr ;

} // GetFileData()

Data* IntPtrToDataPtr( int* intPtr, int size ) {

  if ( intPtr == NULL ) {
    return NULL ;
  } // if

  Data* dataPtr = new Data[size] ;

  for ( int i = 0 ; i < size ; i++ ) {
    dataPtr[i] = Data( intPtr[i] ) ;
  } // for

  return dataPtr ;

} // IntPtrToDataPtr()

void Print( fstream& outFile, int* intPtr, int size, bool smallToBig ) {

  cout << "---------- intPtr ----------" << endl ;
  outFile << "---------- intPtr ----------" << endl ;

  bool check = true ;

  if ( intPtr != NULL ) {

    for ( int i = 0 ; i < size ; i++ ) {

      cout << intPtr[i] << " " ;
      outFile << intPtr[i] << " " ;

      if ( i != 0 && ( smallToBig ? intPtr[i - 1] > intPtr[i] :
      intPtr[i - 1] < intPtr[i] ) ) {
        check = false ;
      } // if

    } // for    

  } // if
  else {
    cout << "NULL pointer !" ;
    outFile << "NULL pointer !" ;
  } // else

  cout << endl ;
  outFile << endl ;

  cout << "----------------------------" << endl ;
  outFile << "----------------------------" << endl ;

  if ( smallToBig ) {
    cout << "Small To Big : " << ( check ? "True" : "False" ) << endl ;
    outFile << "Small To Big : " << ( check ? "True" : "False" ) << endl ;
  } // if
  else {
    cout << "Big To Small : " << ( check ? "True" : "False" ) << endl ;
    outFile << "Big To Small : " << ( check ? "True" : "False" ) << endl ;
  } // else  

} // Print()

void Print( fstream& outFile, Data* dataPtr, int size, bool smallToBig ) {

  cout << "---------- dataPtr ----------" << endl ;
  outFile << "---------- dataPtr ----------" << endl ;

  bool check = true ;

  if ( dataPtr != NULL ) {

    for ( int i = 0 ; i < size ; i++ ) {

      cout << dataPtr[i].num << " " ;
      outFile << dataPtr[i].num << " " ;

      if ( i != 0 && ( smallToBig ? dataPtr[i - 1].num > dataPtr[i].num :
      dataPtr[i - 1].num < dataPtr[i].num ) ) {
        check = false ;
      } // if

    } // for    

  } // if
  else {
    cout << "NULL pointer !" ;
    outFile << "NULL pointer !" ;
  } // else

  cout << endl ;
  outFile << endl ;

  cout << "-----------------------------" << endl ;
  outFile << "-----------------------------" << endl ;

  if ( smallToBig ) {
    cout << "Small To Big : " << ( check ? "True" : "False" ) << endl ;
    outFile << "Small To Big : " << ( check ? "True" : "False" ) << endl ;
  } // if
  else {
    cout << "Big To Small : " << ( check ? "True" : "False" ) << endl ;
    outFile << "Big To Small : " << ( check ? "True" : "False" ) << endl ;
  } // else  

} // Print()


