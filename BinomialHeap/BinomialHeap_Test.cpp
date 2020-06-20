# include <iostream>
# include <string>
# include <fstream>
# include <vector>
# include "BinomialHeap.h"

using namespace std ;

struct Data {
  int num ;
  Data() : num( 0 ) {} // constructor
  Data( int n ) : num( n ) {} // constructor
} ; // struct Data

bool IntSmallerEqual( int value1, int value2 ) ;
bool DataSmallerEqual( Data data1, Data data2 ) ;

bool IntBigger( int value1, int value2 ) ;
bool DataBigger( Data data1, Data data2 ) ;

void EachFileTest( string path, vector<string> fileNameBox ) ;
void UnionEachFileTest( string path, vector<string> fileNameBox ) ;

int* GetFileData( string fileName, int& size ) ;
Data* IntPtrToDataPtr( int* intPtr, int size ) ;

template<class Type>
void BuildHeapAndSort( Type* dataPtr, int size,
                       bool ( *compare ) ( Type data1, Type data2 ) ) ;
template<class Type>
BinomialHeap<Type>* BuildHeapAndUnion( BinomialHeap<Type>* sourceHeap, Type* dataPtr, int size,
                                       bool ( *compare ) ( Type data1, Type data2 ) ) ;

void Print( fstream& outFile, int* intPtr, int size, bool smallToBig, bool printOnScreen ) ;
void Print( fstream& outFile, Data* dataPtr, int size, bool smallToBig, bool printOnScreen ) ;

int main() {

  vector<string> fileNameBox ;
  string fileName = "" ;

  fstream inFile( "..//test_data//TOTAL_inputFileName.txt", ios::in ) ;

  if ( !inFile ) {
    cout << "TOTAL_inputFileName.txt does not exist !" << endl ;
  } // if
  else {

    while ( inFile >> fileName ) {
      fileNameBox.push_back( fileName ) ;
    } // while

  } // else

  inFile.close() ;

  EachFileTest( "..//test_data//", fileNameBox ) ;
  /*
  Let the data in each file be heaps separately and sort in ascending order and descending order by getting.
  */

  UnionEachFileTest( "..//test_data//", fileNameBox ) ;
  /*
  Let the data in each file be heaps separately.
  Then, union them together and sort in ascending order and descending order by getting.
  */

} // main()

void EachFileTest( string path, vector<string> fileNameBox ) {

  /*
  Let the data in each file be heaps separately and sort in ascending order and descending order by getting.
  */

  cout << "EachFileTest()\n\n" ;

  int size = 0 ;
  int* intPtr = NULL ;
  Data* dataPtr = NULL ;
  bool printOnScreen = false ;

  for ( int i = 0 ; i < fileNameBox.size() ; i++ ) {

    /* sorted in ascending order */

    size = 0 ;
    intPtr = GetFileData( ( path + fileNameBox[i] + ".txt" ), size ) ;
    dataPtr = IntPtrToDataPtr( intPtr, size ) ;

    BuildHeapAndSort<int>( intPtr, size, IntSmallerEqual ) ;
    BuildHeapAndSort<Data>( dataPtr, size, DataSmallerEqual ) ;

    fstream outFile( ( "Output//output_" + fileNameBox[i] + ".txt" ).c_str(), ios::out ) ;

    Print( outFile, intPtr, size, true, printOnScreen ) ;
    Print( outFile, dataPtr, size, true, printOnScreen ) ;

    outFile.close() ;

    delete[] intPtr ;
    intPtr = NULL ;
    delete[] dataPtr ;
    dataPtr = NULL ;

    /* sorted in descending order */

    size = 0 ;
    intPtr = GetFileData( ( path + fileNameBox[i] + ".txt" ), size ) ;
    dataPtr = IntPtrToDataPtr( intPtr, size ) ;

    BuildHeapAndSort<int>( intPtr, size, IntBigger ) ;
    BuildHeapAndSort<Data>( dataPtr, size, DataBigger ) ;

    fstream outFileBS( ( "OutputBS//outputBS_" + fileNameBox[i] + ".txt" ).c_str(), ios::out ) ;

    Print( outFileBS, intPtr, size, false, printOnScreen ) ;
    Print( outFileBS, dataPtr, size, false, printOnScreen ) ;

    outFileBS.close() ;

    delete[] intPtr ;
    intPtr = NULL ;
    delete[] dataPtr ;
    dataPtr = NULL ;

  } // for

} // EachFileTest()

void UnionEachFileTest( string path, vector<string> fileNameBox ) {

  /*
  Let the data in each file be heaps separately.
  Then, union them together and sort in ascending order and descending order by getting.
  */

  cout << "\nUnionEachFileTest()\n\n" ;

  int size = 0, totalSize = 0, tempInt = 0 ;
  Data tempData ;
  int* intPtr = NULL ;
  Data* dataPtr = NULL ;
  bool printOnScreen = false ;
  BinomialHeap<int>* intHeap = NULL ;
  BinomialHeap<Data>* dataHeap = NULL ;

  /* union and sorted in ascending order */

  for ( int i = 0 ; i < fileNameBox.size() ; i++ ) {

    cout << "Union : " << fileNameBox[i] << endl ;

    size = 0 ;
    intPtr = GetFileData( ( path + fileNameBox[i] + ".txt" ), size ) ;
    dataPtr = IntPtrToDataPtr( intPtr, size ) ;

    intHeap = BuildHeapAndUnion<int>( intHeap, intPtr, size, IntSmallerEqual ) ;
    dataHeap = BuildHeapAndUnion<Data>( dataHeap, dataPtr, size, DataSmallerEqual ) ;

    delete[] intPtr ;
    intPtr = NULL ;
    delete[] dataPtr ;
    dataPtr = NULL ;

    totalSize += size ;

  } // for

  intPtr = new int[totalSize] ;
  dataPtr = new Data[totalSize] ;

  for ( int i = 0 ; intHeap->GetTopAndDelete( tempInt ) && dataHeap->GetTopAndDelete( tempData ) ; i++ ) {
    intPtr[i] = tempInt ;
    dataPtr[i] = tempData ;
  } // for

  fstream outFile( "Output//output_ALL.txt", ios::out ) ;

  Print( outFile, intPtr, totalSize, true, printOnScreen ) ;
  Print( outFile, dataPtr, totalSize, true, printOnScreen ) ;

  outFile.close() ;

  delete[] intPtr ;
  intPtr = NULL ;
  delete[] dataPtr ;
  dataPtr = NULL ;
  delete intHeap ;
  intHeap = NULL ;
  delete dataHeap ;
  dataHeap = NULL ;

  totalSize = 0 ;

  cout << endl ;

  /* union and sorted in descending order */

  for ( int i = 0 ; i < fileNameBox.size() ; i++ ) {

    cout << "Union : " << fileNameBox[i] << endl ;

    size = 0 ;
    intPtr = GetFileData( ( path + fileNameBox[i] + ".txt" ), size ) ;
    dataPtr = IntPtrToDataPtr( intPtr, size ) ;

    intHeap = BuildHeapAndUnion<int>( intHeap, intPtr, size, IntBigger ) ;
    dataHeap = BuildHeapAndUnion<Data>( dataHeap, dataPtr, size, DataBigger ) ;

    delete[] intPtr ;
    intPtr = NULL ;
    delete[] dataPtr ;
    dataPtr = NULL ;

    totalSize += size ;

  } // for

  intPtr = new int[totalSize] ;
  dataPtr = new Data[totalSize] ;

  for ( int i = 0 ; intHeap->GetTopAndDelete( tempInt ) && dataHeap->GetTopAndDelete( tempData ) ; i++ ) {
    intPtr[i] = tempInt ;
    dataPtr[i] = tempData ;
  } // for

  fstream outFileBS( "OutputBS//outputBS_ALL.txt", ios::out ) ;

  Print( outFileBS, intPtr, totalSize, false, printOnScreen ) ;
  Print( outFileBS, dataPtr, totalSize, false, printOnScreen ) ;

  outFileBS.close() ;

  delete[] intPtr ;
  intPtr = NULL ;
  delete[] dataPtr ;
  dataPtr = NULL ;
  delete intHeap ;
  intHeap = NULL ;
  delete dataHeap ;
  dataHeap = NULL ;

} // UnionEachFileTest()

template<class Type>
void BuildHeapAndSort( Type* dataPtr, int size,
                       bool ( *compare ) ( Type data1, Type data2 ) ) {

  BinomialHeap<Type>* heap = new BinomialHeap<Type>( compare ) ;

  Type data ;

  for ( int i = 0 ; i < size ; i++ ) {
    heap->Insert( dataPtr[i] ) ;
  } // for

  for ( int i = 0 ; heap->GetTopAndDelete( data ) ; i++ ) {
    dataPtr[i] = data ;
  } // for

  delete heap ;
  heap = NULL ;

} // BuildHeapAndSort()

template<class Type>
BinomialHeap<Type>* BuildHeapAndUnion( BinomialHeap<Type>* sourceHeap, Type* dataPtr, int size,
                                      bool ( *compare ) ( Type data1, Type data2 ) ) {

  BinomialHeap<Type>* heap = new BinomialHeap<Type>( compare ) ;

  Type data ;

  for ( int i = 0 ; i < size ; i++ ) {
    heap->Insert( dataPtr[i] ) ;
  } // for

  if ( sourceHeap == NULL ) {
    sourceHeap = heap ;
  } // if
  else {

    sourceHeap->Union( *heap ) ;

    delete heap ;
    heap = NULL ;

  } // else

  return sourceHeap ;

} // BuildHeapAndUnion()

bool IntSmallerEqual( int value1, int value2 ) {
  return value1 <= value2 ;
} // IntSmallerEqual()

bool DataSmallerEqual( Data data1, Data data2 ) {
  return data1.num <= data2.num ;
} // DataSmallerEqual()

bool IntBigger( int value1, int value2 ) {
  return value1 > value2 ;
} // IntBigger()

bool DataBigger( Data data1, Data data2 ) {
  return data1.num > data2.num ;
} // DataBigger()

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

void Print( fstream& outFile, int* intPtr, int size, bool smallToBig, bool printOnScreen ) {

  cout << "---------- intPtr ----------" << endl ;
  outFile << "---------- intPtr ----------" << endl ;

  bool check = true ;

  if ( intPtr != NULL ) {

    for ( int i = 0 ; i < size ; i++ ) {

      if ( printOnScreen ) {
        cout << intPtr[i] << " " ;
      } // if      

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

void Print( fstream& outFile, Data* dataPtr, int size, bool smallToBig, bool printOnScreen ) {

  cout << "---------- dataPtr ----------" << endl ;
  outFile << "---------- dataPtr ----------" << endl ;

  bool check = true ;

  if ( dataPtr != NULL ) {

    for ( int i = 0 ; i < size ; i++ ) {

      if ( printOnScreen ) {
        cout << dataPtr[i].num << " " ;
      } // if

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


