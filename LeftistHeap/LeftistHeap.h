#ifndef LEFTIST_HEAP_H_INCLUDED
#define LEFTIST_HEAP_H_INCLUDED

# include <stdio.h>

template<class Type>
class LeftistHeap {

private:

  struct Node {
    Type content ;
    int distance ;
    Node* leftPtr ;
    Node* rightPtr ;
    Node( Type content ) ; // constructor
    Node( Type content, int distance ) ; // constructor
  } ; // struct Node

  /* stack */

  Node** mStack ;
  int mStackMaxSize ;
  int mStackIndex ;

  void PushIn( Node* nodePtr ) ;
  void PopOut( Node*& nodePtr ) ;
  void AdjustStackSize() ;

  /* tree */

  Node* mRoot ;
  int mDataSize ;
  bool ( *mCompare ) ( Type data1, Type data2 ) ;

  void MergeWith( Node* nodePtr ) ;
  void CopyFrom( Node*& newRoot, LeftistHeap<Type>& sourceHeap ) ;

  void SwapPtr( Node*& nodePtrL, Node*& nodePtrR ) ;
  void DeleteAllNode( Node* nodePtr ) ;

public:

  LeftistHeap( bool ( *compare ) ( Type data1, Type data2 ) ) ; // constructor
  ~LeftistHeap() ; // destructor

  void Insert( Type data ) ;

  bool GetTop( Type& data ) ;
  bool GetTopAndDelete( Type& data ) ;
  void DeleteTop() ;

  void Union( LeftistHeap<Type>& sourceHeap ) ;

  void Clear() ;
  int GetSize() ;

} ; // class LeftistHeap

template<class Type>
LeftistHeap<Type>::LeftistHeap( bool ( *compare ) ( Type data1, Type data2 ) ) { // constructor

  mStackMaxSize = 100 ;
  mStackIndex = 0 ;

  mRoot = NULL ;
  mDataSize = 0 ;
  mCompare = compare ;

  mStack = new Node*[mStackMaxSize] ;

} // LeftistHeap<Type>::LeftistHeap()

template<class Type>
LeftistHeap<Type>::~LeftistHeap() { // destructor

  DeleteAllNode( mRoot ) ;
  mRoot = NULL ;

  delete[] mStack ;
  mStack = NULL ;

  mCompare = NULL ;  

} // LeftistHeap<Type>::~LeftistHeap()

template<class Type>
LeftistHeap<Type>::Node::Node( Type content ) { // constructor

  this->content = content ;
  this->distance = 0 ;
  this->leftPtr = NULL ;
  this->rightPtr = NULL ;

} // LeftistHeap<Type>::Node::Node()

template<class Type>
LeftistHeap<Type>::Node::Node( Type content, int distance ) { // constructor

  this->content = content ;
  this->distance = distance ;
  this->leftPtr = NULL ;
  this->rightPtr = NULL ;

} // LeftistHeap<Type>::Node::Node()

template<class Type>
void LeftistHeap<Type>::Insert( Type data ) { 

  mDataSize++ ;

  MergeWith( new Node( data ) ) ;

} // LeftistHeap<Type>::Insert()

template<class Type>
bool LeftistHeap<Type>::GetTop( Type& data ) {

  if ( mRoot == NULL ) {
    return false ;
  } // if

  data = mRoot->content ;
  return true ;

} // LeftistHeap<Type>::GetTop()

template<class Type>
bool LeftistHeap<Type>::GetTopAndDelete( Type& data ) {

  if ( mRoot == NULL ) {
    return false ;
  } // if

  data = mRoot->content ;
  DeleteTop() ;
  return true ;

} // LeftistHeap<Type>::GetTopAndDelete()

template<class Type>
void LeftistHeap<Type>::DeleteTop() {

  if ( mDataSize == 0 ) {
    return ;
  } // if

  mDataSize-- ;

  Node* leftPtr = mRoot->leftPtr, * rightPtr = mRoot->rightPtr ;

  delete mRoot ;

  mRoot = leftPtr ;
  MergeWith( rightPtr ) ;

} // LeftistHeap<Type>::DeleteTop()

template<class Type>
void LeftistHeap<Type>::MergeWith( Node* nodePtr ) {

  if ( nodePtr == NULL ) {
    return ;
  } // if
  
  if ( mRoot == NULL ) {
    mRoot = nodePtr ;
    return ;
  } // if

  Node* tempPtr = NULL, * walkLeft = NULL, * walkRight = NULL, * parentPtr = NULL ;

  if ( mCompare( nodePtr->content, mRoot->content ) ) {
    SwapPtr( nodePtr, mRoot ) ;
  } // if

  PushIn( mRoot ) ;

  walkLeft = mRoot->rightPtr, walkRight = nodePtr ;

  while ( walkLeft != NULL && walkRight != NULL ) {
    if ( mCompare( walkLeft->content, walkRight->content ) ) {
      PushIn( walkLeft ) ;
      walkLeft = walkLeft->rightPtr ;
    } // if
    else {
      PushIn( walkRight ) ;
      walkRight = walkRight->rightPtr ;
    } // else
  } // while

  tempPtr = ( walkLeft == NULL ? walkRight : walkLeft ) ;

  while ( mStackIndex != 0 ) { // stack is not empty

    PopOut( parentPtr ) ;
    parentPtr->rightPtr = tempPtr ;

    if ( parentPtr->leftPtr == NULL || parentPtr->leftPtr->distance < parentPtr->rightPtr->distance ) {
      SwapPtr( parentPtr->leftPtr, parentPtr->rightPtr ) ;
    } // if

    if ( parentPtr->rightPtr != NULL ) {
      parentPtr->distance = 1 + parentPtr->rightPtr->distance ;
    } // if
    
    tempPtr = parentPtr ;

  } // while

} // LeftistHeap<Type>::MergeWith()

template<class Type>
void LeftistHeap<Type>::Union( LeftistHeap<Type>& sourceHeap ) {

  Node* newRoot = NULL ;

  CopyFrom( newRoot, sourceHeap ) ;

  MergeWith( newRoot ) ;

  mDataSize += sourceHeap.mDataSize ;

} // LeftistHeap<Type>::Union()

template<class Type>
void LeftistHeap<Type>::CopyFrom( Node*& newRoot, LeftistHeap<Type>& sourceHeap ) {

  /*
  Copy the tree of mRoot for root.
  */

  if ( sourceHeap.mRoot == NULL ) {
    newRoot = NULL ;
    return ;
  } // if
  else {
    newRoot = new Node( sourceHeap.mRoot->content, sourceHeap.mRoot->distance ) ;
  } // else

  PushIn( newRoot ) ;
  sourceHeap.PushIn( sourceHeap.mRoot ) ;

  Node* destinationPtr = NULL, * sourcePtr = NULL ;

  while ( mStackIndex != 0 ) { // stack is not empty

    PopOut( destinationPtr ) ;
    sourceHeap.PopOut( sourcePtr ) ;

    if ( sourcePtr->leftPtr != NULL ) {
      destinationPtr->leftPtr = new Node( sourcePtr->leftPtr->content, sourcePtr->leftPtr->distance ) ;
      PushIn( destinationPtr->leftPtr ) ;
      sourceHeap.PushIn( sourcePtr->leftPtr ) ;
    } // if

    if ( sourcePtr->rightPtr != NULL ) {
      destinationPtr->rightPtr = new Node( sourcePtr->rightPtr->content, sourcePtr->rightPtr->distance ) ;
      PushIn( destinationPtr->rightPtr ) ;
      sourceHeap.PushIn( sourcePtr->rightPtr ) ;
    } // if

  } // while

} // LeftistHeap<Type>::CopyFrom()

template<class Type>
void LeftistHeap<Type>::Clear() {

  DeleteAllNode( mRoot ) ;
  mRoot = NULL ;

  mDataSize = 0 ;

} // LeftistHeap<Type>::Clear()

template<class Type>
int LeftistHeap<Type>::GetSize() {

  return mDataSize ;

} // LeftistHeap<Type>::GetSize()

template<class Type>
void LeftistHeap<Type>::SwapPtr( Node*& nodePtrL, Node*& nodePtrR ) {

  Node* tempPtr = nodePtrL ;
  nodePtrL = nodePtrR ;
  nodePtrR = tempPtr ;

} // LeftistHeap<Type>::SwapPtr()

template<class Type>
void LeftistHeap<Type>::DeleteAllNode( Node* nodePtr ) {

  if ( nodePtr == NULL ) {
    return ;
  } // if

  /* delete by the iterative method */

  PushIn( nodePtr ) ;

  while ( mStackIndex != 0 ) { // stack is not empty

    PopOut( nodePtr ) ;

    if ( nodePtr->leftPtr != NULL ) {
      PushIn( nodePtr->leftPtr ) ;
    } // if

    if ( nodePtr->rightPtr != NULL ) {
      PushIn( nodePtr->rightPtr ) ;
    } // if

    delete nodePtr ;

  } // while

} // LeftistHeap<Type>::DeleteAllNode()

template<class Type>
void LeftistHeap<Type>::PushIn( Node* nodePtr ) {

  if ( mStackIndex == mStackMaxSize ) {
    AdjustStackSize() ;
  } // if

  mStack[mStackIndex] = nodePtr ;
  mStackIndex++ ;

} // LeftistHeap<Type>::PushIn()

template<class Type>
void LeftistHeap<Type>::PopOut( Node*& nodePtr ) {

  if ( mStackIndex == 0 ) {
    nodePtr = NULL ;
  } // if
  else {

    mStackIndex-- ;
    nodePtr = mStack[mStackIndex] ;
    mStack[mStackIndex] = NULL ;

  } // else

} // LeftistHeap<Type>::PopOut()

template<class Type>
void LeftistHeap<Type>::AdjustStackSize() {

  Node** tempPtr = mStack ;

  mStackMaxSize = mStackMaxSize * 2 ;
  mStack = new Node*[mStackMaxSize] ;

  for ( int i = 0 ; i < mStackIndex ; i++ ) {
    mStack[i] = tempPtr[i] ;
  } // for 

  delete[] tempPtr ;
  tempPtr = NULL ;

} // LeftistHeap<Type>::AdjustStackSize()

#endif // LEFTIST_HEAP_H_INCLUDED

