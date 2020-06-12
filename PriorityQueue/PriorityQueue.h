#ifndef PRIORITY_QUEUE_H_INCLUDED
#define PRIORITY_QUEUE_H_INCLUDED

# include <stdio.h>

template<class Type>
class PriorityQueue {

private:

  Type* mHeap ;
  bool ( *mCompare ) ( Type data1, Type data2 ) ;
  int mMaxSize ;
  int mHeapSize ;

  void Heapify( int index ) ;
  void AdjustSize() ;

public:

  PriorityQueue( bool ( *compare ) ( Type data1, Type data2 ) ) ; // constructor
  ~PriorityQueue() ; // destructor

  void Insert( Type data ) ;
  bool GetTop( Type& data ) ;
  bool GetTopAndDelete( Type& data ) ;
  void DeleteTop() ;
  int GetSize() ;

} ; // class PriorityQueue

template<class Type>
PriorityQueue<Type>::PriorityQueue( bool ( *compare ) ( Type data1, Type data2 ) ) { // constructor

  mCompare = compare ;
  mMaxSize = 100 ;
  mHeapSize = 0 ;

  mHeap = new Type[mMaxSize] ;

} // PriorityQueue<Type>::PriorityQueue()

template<class Type>
PriorityQueue<Type>::~PriorityQueue() { // destructor

  mCompare = NULL ;
  delete[] mHeap ;
  mHeap = NULL ;

} // PriorityQueue<Type>::~PriorityQueue()

template<class Type>
void PriorityQueue<Type>::Insert( Type data ) {

  if ( mHeapSize == mMaxSize ) {
    AdjustSize() ;
  } // if

  mHeap[mHeapSize] = data ;

  int currentIndex = mHeapSize, parentIndex = 0 ;
  bool running = true ;

  while ( running ) {

    if ( currentIndex - 1 < 0 ) {
      running = false ;
    } // if
    else {

      parentIndex = ( currentIndex - 1 ) / 2 ;

      if ( mCompare( mHeap[currentIndex], mHeap[parentIndex] ) ) {

        Type temp = mHeap[currentIndex] ;
        mHeap[currentIndex] = mHeap[parentIndex] ;
        mHeap[parentIndex] = temp ;

        currentIndex = parentIndex ;
      
      } // if
      else {
        running = false ;
      } // else      

    } // else

  } // while

  mHeapSize++ ;

} // PriorityQueue<Type>::Insert()

template<class Type>
bool PriorityQueue<Type>::GetTop( Type& data ) {

  if ( mHeapSize != 0 ) {
    data = mHeap[0] ;
    return true ;
  } // if
  else {
    return false ;
  } // else

} // PriorityQueue<Type>::GetTop()

template<class Type>
bool PriorityQueue<Type>::GetTopAndDelete( Type& data ) {

  if ( mHeapSize != 0 ) {
    data = mHeap[0] ;
    DeleteTop() ;
    return true ;
  } // if
  else {
    return false ;
  } // else

} // PriorityQueue<Type>::GetTopAndDelete()

template<class Type>
void PriorityQueue<Type>::DeleteTop() {
  
  if ( mHeapSize == 0 ) {
    return ;
  } // if

  mHeapSize-- ;
  mHeap[0] = mHeap[mHeapSize] ;

  if ( mHeapSize != 0 ) {
    Heapify( 0 ) ;
  } // if

} // PriorityQueue<Type>::DeleteTop()

template<class Type>
int PriorityQueue<Type>::GetSize() {

  return mHeapSize ;

} // PriorityQueue<Type>::GetSize()

template<class Type>
void PriorityQueue<Type>::AdjustSize() {

  Type* temp = mHeap ;

  mMaxSize = mMaxSize * 2 ;
  mHeap = new Type[mMaxSize] ;

  for ( int i = 0 ; i < mHeapSize ; i++ ) {
    mHeap[i] = temp[i] ;
  } // for 

  delete[] temp ;
  temp = NULL ;

} // PriorityQueue<Type>::AdjustSize()

template<class Type>
void PriorityQueue<Type>::Heapify( int index ) {

  int leftChildIndex = index * 2 + 1 ;
  int rightChildIndex = leftChildIndex + 1 ;
  int targetIndex = index ;

  if ( leftChildIndex < mHeapSize && mCompare( mHeap[leftChildIndex], mHeap[targetIndex] ) ) {
    targetIndex = leftChildIndex ;
  } // if

  if ( rightChildIndex < mHeapSize && mCompare( mHeap[rightChildIndex], mHeap[targetIndex] ) ) {
    targetIndex = rightChildIndex ;
  } // if

  if ( targetIndex != index ) {

    Type temp = mHeap[index] ;
    mHeap[index] = mHeap[targetIndex] ;
    mHeap[targetIndex] = temp ;

    Heapify( targetIndex ) ;

  } // if

} // PriorityQueue<Type>::Heapify()

#endif // PRIORITY_QUEUE_H_INCLUDED

