#ifndef MIN_MAX_HEAP_H_INCLUDED
#define MIN_MAX_HEAP_H_INCLUDED

# include <stdio.h>

/*
operator overloading :
bool operator < ( Type data1, Type data2 ) ;
*/

template<class Type>
class MinMaxHeap {

private:

  Type* mHeap ;
  int mMaxSize ;
  int mHeapSize ;

  void ReHeapUp( int index, bool isMin ) ;
  void ReHeapDown( int index, bool isMin ) ;
  bool IsMin( int index ) ;
  void DataSwap( int index1, int index2 ) ;
  void AdjustSize() ;

public:

  MinMaxHeap() ; // constructor
  ~MinMaxHeap() ; // destructor

  void Insert( Type data ) ;

  bool GetMin( Type& data ) ;
  bool GetMinAndDelete( Type& data ) ;
  void DeleteMin() ;

  bool GetMax( Type& data ) ;
  bool GetMaxAndDelete( Type& data ) ;
  void DeleteMax() ;

  int GetSize() ;

} ; // class MinMaxHeap

template<class Type>
MinMaxHeap<Type>::MinMaxHeap() { // constructor

  mMaxSize = 100 ;
  mHeapSize = 0 ;

  mHeap = new Type[mMaxSize] ;

} // MinMaxHeap<Type>::MinMaxHeap()

template<class Type>
MinMaxHeap<Type>::~MinMaxHeap() { // destructor

  delete[] mHeap ;
  mHeap = NULL ;

} // MinMaxHeap<Type>::~MinMaxHeap()

template<class Type>
void MinMaxHeap<Type>::Insert( Type data ) {

  if ( mHeapSize == mMaxSize ) {
    AdjustSize() ;
  } // if

  mHeap[mHeapSize] = data ;

  if ( mHeapSize != 0 ) {

    bool isMin = IsMin( mHeapSize ) ;
    int parentIndex = ( mHeapSize - 1 ) / 2 ;

    if ( isMin ) {

      if ( mHeap[parentIndex] < mHeap[mHeapSize] ) {
        DataSwap( parentIndex, mHeapSize ) ;
        ReHeapUp( parentIndex, ( !isMin ) ) ;
      } // if
      else {
        ReHeapUp( mHeapSize, isMin ) ;
      } // else

    } // if
    else {

      if ( mHeap[mHeapSize] < mHeap[parentIndex] ) {
        DataSwap( parentIndex, mHeapSize ) ;
        ReHeapUp( parentIndex, ( !isMin ) ) ;
      } // if
      else {
        ReHeapUp( mHeapSize, isMin ) ;
      } // else

    } // else

  } // if

  mHeapSize++ ;

} // MinMaxHeap<Type>::Insert()

template<class Type>
bool MinMaxHeap<Type>::GetMin( Type& data ) {

  if ( mHeapSize != 0 ) {
    data = mHeap[0] ;
    return true ;
  } // if
  else {
    return false ;
  } // else

} // MinMaxHeap<Type>::GetMin()

template<class Type>
bool MinMaxHeap<Type>::GetMinAndDelete( Type& data ) {

  if ( mHeapSize != 0 ) {
    data = mHeap[0] ;
    DeleteMin() ;
    return true ;
  } // if
  else {
    return false ;
  } // else

} // MinMaxHeap<Type>::GetMinAndDelete()

template<class Type>
void MinMaxHeap<Type>::DeleteMin() {

  if ( mHeapSize == 0 ) {
    return ;
  } // if

  mHeapSize-- ;
  mHeap[0] = mHeap[mHeapSize] ;

  if ( mHeapSize != 0 ) {
    ReHeapDown( 0, true ) ;
  } // if

} // MinMaxHeap<Type>::DeleteMin()

template<class Type>
bool MinMaxHeap<Type>::GetMax( Type& data ) {

  if ( mHeapSize != 0 ) {

    if ( mHeapSize == 0 ) {
      data = mHeap[0] ;
    } // if
    else if ( mHeapSize == 1 ) {
      data = mHeap[1] ;
    } // else if
    else {
      data = ( mHeap[1] < mHeap[2] ? mHeap[2] : mHeap[1] ) ;
    } // else

    return true ;

  } // if
  else {
    return false ;
  } // else

} // MinMaxHeap<Type>::GetMax()

template<class Type>
bool MinMaxHeap<Type>::GetMaxAndDelete( Type& data ) {

  if ( mHeapSize != 0 ) {

    if ( mHeapSize == 1 ) {
      data = mHeap[0] ;
    } // if
    else if ( mHeapSize == 2 ) {
      data = mHeap[1] ;
    } // else if
    else {
      data = ( mHeap[1] < mHeap[2] ? mHeap[2] : mHeap[1] ) ;
    } // else

    DeleteMax() ;
    return true ;

  } // if
  else {
    return false ;
  } // else

} // MinMaxHeap<Type>::GetMaxAndDelete()

template<class Type>
void MinMaxHeap<Type>::DeleteMax() {

  if ( mHeapSize == 0 ) {
    return ;
  } // if

  int maxIndex = 0 ;

  if ( mHeapSize == 1 ) {
    maxIndex = 0 ;
  } // if
  else if ( mHeapSize == 2 ) {
    maxIndex = 1 ;
  } // else if
  else {
    maxIndex = ( mHeap[1] < mHeap[2] ? 2 : 1 ) ;
  } // else

  mHeapSize-- ;

  mHeap[maxIndex] = mHeap[mHeapSize] ;

  if ( mHeapSize != 0 ) {
    ReHeapDown( maxIndex, false ) ;
  } // if

} // MinMaxHeap<Type>::DeleteMax()

template<class Type>
int MinMaxHeap<Type>::GetSize() {

  return mHeapSize ;

} // MinMaxHeap<Type>::GetSize()

template<class Type>
void MinMaxHeap<Type>::AdjustSize() {

  Type* temp = mHeap ;

  mMaxSize = mMaxSize * 2 ;
  mHeap = new Type[mMaxSize] ;

  for ( int i = 0 ; i < mHeapSize ; i++ ) {
    mHeap[i] = temp[i] ;
  } // for 

  delete[] temp ;
  temp = NULL ;

} // MinMaxHeap<Type>::AdjustSize()

template<class Type>
void MinMaxHeap<Type>::ReHeapUp( int index, bool isMin ) {

  int currentIndex = index, parentIndex = 0, grandParentIndex = 0 ;
  bool running = true ;

  while ( running ) {

    parentIndex = ( currentIndex - 1 ) / 2 ;

    if ( parentIndex - 1 < 0 ) {
      running = false ;
    } // if
    else {

      grandParentIndex = ( parentIndex - 1 ) / 2 ;

      if ( isMin ? mHeap[currentIndex] < mHeap[grandParentIndex] :
           mHeap[grandParentIndex] < mHeap[currentIndex] ) {
        DataSwap( currentIndex, grandParentIndex ) ;
        currentIndex = grandParentIndex ;
      } // if
      else {
        running = false ;
      } // else      

    } // else

  } // while

} // MinMaxHeap<Type>::ReHeapUp()

template<class Type>
void MinMaxHeap<Type>::ReHeapDown( int index, bool isMin ) {

  int leftChildIndex = 0, rightChildIndex = 0 ;
  int leftMostGrandChildIndex = 0, rightMostGrandChildIndex = 0 ;
  int targetIndex = 0 ;
  bool running = true ;

  leftChildIndex = index * 2 + 1 ;

  while ( running && leftChildIndex < mHeapSize ) { // children exist   

    targetIndex = leftChildIndex ;
    rightChildIndex = leftChildIndex + 1 ;

    if ( rightChildIndex < mHeapSize && ( isMin ? mHeap[rightChildIndex] < mHeap[targetIndex] :
                                                  mHeap[targetIndex] < mHeap[rightChildIndex] ) ) {
      targetIndex = rightChildIndex ;
    } // if

    leftMostGrandChildIndex = leftChildIndex * 2 + 1 ;    

    bool isGrandChild = false ;

    if ( leftMostGrandChildIndex < mHeapSize ) { // grandChildren exist   

      rightMostGrandChildIndex = leftMostGrandChildIndex + 3 ;

      for ( int grandChildIndex = leftMostGrandChildIndex ;
            grandChildIndex <= rightMostGrandChildIndex ;
            grandChildIndex++ ) {

        if ( grandChildIndex < mHeapSize && ( isMin ? mHeap[grandChildIndex] < mHeap[targetIndex] :
                                                      mHeap[targetIndex] < mHeap[grandChildIndex] ) ) {
          targetIndex = grandChildIndex ;
          isGrandChild = true ;
        } // if

      } // for  

    } // if

    if ( isGrandChild ) {

      if ( isMin ? mHeap[targetIndex] < mHeap[index] : mHeap[index] < mHeap[targetIndex] ) {

        DataSwap( targetIndex, index ) ;

        int parent = ( targetIndex - 1 ) / 2 ;

        if ( isMin ? mHeap[parent] < mHeap[targetIndex] : mHeap[targetIndex] < mHeap[parent] ) {
          DataSwap( targetIndex, parent ) ;
        } // if

        index = targetIndex ;
        leftChildIndex = index * 2 + 1 ;

      } // if
      else {
        running = false ;
      } // else

    } // if
    else { // is child

      if ( isMin ? mHeap[targetIndex] < mHeap[index] : mHeap[index] < mHeap[targetIndex] ) {
        DataSwap( targetIndex, index ) ;
      } // if

      running = false ;

    } // else     

  } // while

} // MinMaxHeap<Type>::ReHeapDown()

template<class Type>
bool MinMaxHeap<Type>::IsMin( int index ) {

  bool isMin = true ;
  int compareIndex = 0, increaseRange = 2 ;

  while ( index > compareIndex ) {
    compareIndex += increaseRange ;
    increaseRange *= 2 ;
    isMin = ( !isMin ) ;
  } // while

  return isMin ;

} // MinMaxHeap<Type>::IsMin()

template<class Type>
void MinMaxHeap<Type>::DataSwap( int index1, int index2 ) {

  Type temp = mHeap[index1] ;
  mHeap[index1] = mHeap[index2] ;
  mHeap[index2] = temp ;

} // MinMaxHeap<Type>::DataSwap()

#endif // MIN_MAX_HEAP_H_INCLUDED

