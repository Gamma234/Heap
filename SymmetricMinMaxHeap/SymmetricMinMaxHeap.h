#ifndef SYMMETRIC_MIN_MAX_HEAP_H_INCLUDED
#define SYMMETRIC_MIN_MAX_HEAP_H_INCLUDED

# include <stdio.h>

/*
operator overloading :
bool operator < ( Type data1, Type data2 ) ;
*/

template<class Type>
class SymmetricMinMaxHeap {

private:

  Type* mHeap ;
  int mMaxSize ;
  int mHeapSize ;

  int AdjustWithSibling( int currentIndex ) ;
  int AdjustWithGrandparentChildren( int currentIndex ) ;

  int AdjustWithChildren( int currentIndex ) ;

  void DataSwap( int index1, int index2 ) ;
  void AdjustSize() ;

public:

  SymmetricMinMaxHeap() ; // constructor
  ~SymmetricMinMaxHeap() ; // destructor

  void Insert( Type data ) ;

  bool GetMin( Type& data ) ;
  bool GetMinAndDelete( Type& data ) ;
  void DeleteMin() ;

  bool GetMax( Type& data ) ;
  bool GetMaxAndDelete( Type& data ) ;
  void DeleteMax() ;

  int GetSize() ;

} ; // class SymmetricMinMaxHeap

template<class Type>
SymmetricMinMaxHeap<Type>::SymmetricMinMaxHeap() { // constructor

  mMaxSize = 100 ;
  mHeapSize = 1 ;

  mHeap = new Type[mMaxSize] ;

} // SymmetricMinMaxHeap<Type>::SymmetricMinMaxHeap()

template<class Type>
SymmetricMinMaxHeap<Type>::~SymmetricMinMaxHeap() { // destructor

  delete[] mHeap ;
  mHeap = NULL ;

} // SymmetricMinMaxHeap<Type>::~SymmetricMinMaxHeap()

template<class Type>
void SymmetricMinMaxHeap<Type>::Insert( Type data ) {

  if ( mHeapSize == mMaxSize ) {
    AdjustSize() ;
  } // if

  mHeap[mHeapSize] = data ;

  int currentIndex = mHeapSize, nextIndex = 0 ;
  bool running = true ;

  while ( running ) {

    currentIndex = AdjustWithSibling( currentIndex ) ;
    nextIndex = AdjustWithGrandparentChildren( currentIndex ) ;

    if ( currentIndex == nextIndex ) {
      running = false ;
    } // if
    else {
      currentIndex = nextIndex ;
    } // else

  } // while

  mHeapSize++ ;

} // SymmetricMinMaxHeap<Type>::Insert()

template<class Type>
int SymmetricMinMaxHeap<Type>::AdjustWithSibling( int currentIndex ) {

  int siblingIndex = 0 ;

  if ( ( currentIndex - 1 ) % 2 == 0 ) { // currentIndex is left child

    siblingIndex = currentIndex + 1 ; // siblingIndex is right child

    if ( siblingIndex >= mHeapSize ) {
      return currentIndex ;
    } // if

    if ( mHeap[siblingIndex] < mHeap[currentIndex] ) {
      DataSwap( siblingIndex, currentIndex ) ;
      return siblingIndex ;
    } // if

  } // if
  else { // currentIndex is right child

    siblingIndex = currentIndex - 1 ; // siblingIndex is left child

    if ( mHeap[currentIndex] < mHeap[siblingIndex] ) {
      DataSwap( currentIndex, siblingIndex ) ;
      return siblingIndex ;
    } // if

  } // else

  return currentIndex ;

} // SymmetricMinMaxHeap<Type>::AdjustWithSibling()

template<class Type>
int SymmetricMinMaxHeap<Type>::AdjustWithGrandparentChildren( int currentIndex ) {

  if ( currentIndex <= 2 ) {
    return currentIndex ;
  } // if

  int grandparentIndex = ( ( currentIndex - 1 ) / 2 - 1 ) / 2 ;
  int grandparentLeftChildIndex = grandparentIndex * 2 + 1 ;
  int grandparentRightChildIndex = grandparentLeftChildIndex + 1 ;

  if ( mHeap[currentIndex] < mHeap[grandparentLeftChildIndex] ) {
    DataSwap( currentIndex, grandparentLeftChildIndex ) ;
    return grandparentLeftChildIndex ;
  } // if
  else if ( mHeap[grandparentRightChildIndex] < mHeap[currentIndex] ) {
    DataSwap( grandparentRightChildIndex, currentIndex ) ;
    return grandparentRightChildIndex ;
  } // else if

  return currentIndex ;

} // SymmetricMinMaxHeap<Type>::AdjustWithGrandparentChildren()

template<class Type>
bool SymmetricMinMaxHeap<Type>::GetMin( Type& data ) {

  if ( mHeapSize != 1 ) {
    data = mHeap[1] ;
    return true ;
  } // if
  else {
    return false ;
  } // else

} // SymmetricMinMaxHeap<Type>::GetMin()

template<class Type>
bool SymmetricMinMaxHeap<Type>::GetMinAndDelete( Type& data ) {

  if ( mHeapSize != 1 ) {
    data = mHeap[1] ;
    DeleteMin() ;
    return true ;
  } // if
  else {
    return false ;
  } // else

} // SymmetricMinMaxHeap<Type>::GetMinAndDelete()

template<class Type>
void SymmetricMinMaxHeap<Type>::DeleteMin() {

  mHeapSize-- ;

  if ( mHeapSize != 1 ) {

    mHeap[1] = mHeap[mHeapSize] ;

    int currentIndex = 1, nextIndex = 0 ;
    bool running = true ;

    while ( running ) {

      AdjustWithSibling( currentIndex ) ;
      nextIndex = AdjustWithChildren( currentIndex ) ;

      if ( currentIndex == nextIndex ) {
        running = false ;
      } // if
      else {
        currentIndex = nextIndex ;
      } // else 

    } // while

  } // if

} // SymmetricMinMaxHeap<Type>::DeleteMin()

template<class Type>
bool SymmetricMinMaxHeap<Type>::GetMax( Type& data ) {

  if ( mHeapSize != 1 ) {
    data = ( mHeapSize == 2 ? mHeap[1] : mHeap[2] ) ;
    return true ;
  } // if
  else {
    return false ;
  } // else

} // SymmetricMinMaxHeap<Type>::GetMax()

template<class Type>
bool SymmetricMinMaxHeap<Type>::GetMaxAndDelete( Type& data ) {

  if ( mHeapSize != 1 ) {
    data = ( mHeapSize == 2 ? mHeap[1] : mHeap[2] ) ;
    DeleteMax() ;
    return true ;
  } // if
  else {
    return false ;
  } // else

} // SymmetricMinMaxHeap<Type>::GetMaxAndDelete()

template<class Type>
void SymmetricMinMaxHeap<Type>::DeleteMax() {

  mHeapSize-- ;

  if ( mHeapSize != 1 && mHeapSize != 2 ) {

    mHeap[2] = mHeap[mHeapSize] ;

    int currentIndex = 2, nextIndex = 0 ;    
    bool running = true ;

    while ( running ) {

      AdjustWithSibling( currentIndex ) ;
      nextIndex = AdjustWithChildren( currentIndex ) ;

      if ( currentIndex == nextIndex ) {
        running = false ;
      } // if
      else {
        currentIndex = nextIndex ;
      } // else 

    } // while

  } // if

} // SymmetricMinMaxHeap<Type>::DeleteMax()

template<class Type>
int SymmetricMinMaxHeap<Type>::AdjustWithChildren( int currentIndex ) {

  if ( ( currentIndex - 1 ) % 2 == 0 ) { // currentIndex is left child

    int targetChild = currentIndex * 2 + 1 ; // left child of current
    int leftChildOfSibling = targetChild + 2 ;

    if ( targetChild >= mHeapSize ) { // currentIndex is leaf
      return currentIndex ;
    } // if

    if ( leftChildOfSibling < mHeapSize && mHeap[leftChildOfSibling] < mHeap[targetChild] ) {
      targetChild = leftChildOfSibling ;
    } // if

    if ( mHeap[targetChild] < mHeap[currentIndex] ) {
      DataSwap( targetChild, currentIndex ) ;
      return targetChild ;
    } // if

  } // if
  else { // currentIndex is right child

    int rightChildOfCurrent = currentIndex * 2 + 2 ;
    int targetChild = rightChildOfCurrent - 2 ; // right child of sibling

    if ( targetChild >= mHeapSize ) { // sibling is leaf or sibling does not own a right child
      return currentIndex ;
    } // if

    if ( targetChild < mHeapSize ) {

      if ( rightChildOfCurrent < mHeapSize && mHeap[targetChild] < mHeap[rightChildOfCurrent] ) {
        targetChild = rightChildOfCurrent ;
      } // if

      if ( mHeap[currentIndex] < mHeap[targetChild] ) {
        DataSwap( currentIndex, targetChild ) ;
        return targetChild ;
      } // if

    } // if

  } // else

  return currentIndex ;

} // SymmetricMinMaxHeap<Type>::AdjustWithChildren()

template<class Type>
int SymmetricMinMaxHeap<Type>::GetSize() {

  return mHeapSize ;

} // SymmetricMinMaxHeap<Type>::GetSize()

template<class Type>
void SymmetricMinMaxHeap<Type>::AdjustSize() {

  Type* temp = mHeap ;

  mMaxSize = mMaxSize * 2 ;
  mHeap = new Type[mMaxSize] ;

  for ( int i = 0 ; i < mHeapSize ; i++ ) {
    mHeap[i] = temp[i] ;
  } // for 

  delete[] temp ;
  temp = NULL ;

} // SymmetricMinMaxHeap<Type>::AdjustSize()

template<class Type>
void SymmetricMinMaxHeap<Type>::DataSwap( int index1, int index2 ) {

  Type temp = mHeap[index1] ;
  mHeap[index1] = mHeap[index2] ;
  mHeap[index2] = temp ;

} // SymmetricMinMaxHeap<Type>::DataSwap()

#endif // SYMMETRIC_MIN_MAX_HEAP_H_INCLUDED

