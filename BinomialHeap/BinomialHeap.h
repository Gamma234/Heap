#ifndef BINOMIAL_HEAP_H_INCLUDED
#define BINOMIAL_HEAP_H_INCLUDED

# include <stdio.h>

template<class Type>
class BinomialHeap {

private:

  struct Node {
    Type content ;
    int degree ;
    Node* leftPtr ;
    Node* rightPtr ;
    Node* downPtr ;
    Node( Type content ) ; // constructor
    Node( Type content, int degree ) ; // constructor
  } ; // struct Node

  /* Degree Array */

  Node** mDegreeArray ;
  int mArrayMaxSize ;

  void SetDegreeArray( Node* nodePtr, int degree ) ;
  bool CheckDegreeArray( int degree ) ;

  void AdjustArraySize( int targetSize ) ;

  /* Forest */

  Node* mRootHead ;
  int mDataSize ;
  bool ( *mCompare ) ( Type data1, Type data2 ) ;

  void MergeWith( Node* nodePtr ) ;
  void Merge( Node*& nodePtr, Node* tempPtr ) ;
  void InsertNode( Node*& nodePtr, Node* sourceNodePtr ) ;

  void FindTopRoot( Node*& topRootPtr ) ;
  void DeleteTopRoot( Node* topRootPtr ) ;

  void CopyFrom( Node*& nodePtr, Node* sourceNodePtr ) ;
  void DeleteAllNode( Node* nodePtr ) ;

public:

  BinomialHeap( bool ( *compare ) ( Type data1, Type data2 ) ) ; // constructor
  ~BinomialHeap() ; // destructor

  void Insert( Type data ) ;

  bool GetTop( Type& data ) ;
  bool GetTopAndDelete( Type& data ) ;
  void DeleteTop() ;

  void Union( BinomialHeap<Type>& sourceHeap ) ;

  void Clear() ;
  int GetSize() ;

} ; // class BinomialHeap

template<class Type>
BinomialHeap<Type>::Node::Node( Type content ) { // constructor

  this->content = content ;
  this->degree = 0 ;
  this->leftPtr = this ;
  this->rightPtr = this ;
  this->downPtr = NULL ;

} // BinomialHeap<Type>::Node::Node()

template<class Type>
BinomialHeap<Type>::Node::Node( Type content, int degree ) { // constructor

  this->content = content ;
  this->degree = degree ;
  this->leftPtr = this ;
  this->rightPtr = this ;
  this->downPtr = NULL ;

} // BinomialHeap<Type>::Node::Node()

template<class Type>
BinomialHeap<Type>::BinomialHeap( bool ( *compare ) ( Type data1, Type data2 ) ) { // constructor

  mArrayMaxSize = 10 ;
  mDegreeArray = new Node*[mArrayMaxSize] ;

  for ( int i = 0 ; i < mArrayMaxSize ; i++ ) {
    mDegreeArray[i] = NULL ;
  } // for

  mRootHead = NULL ;
  mDataSize = 0 ;
  mCompare = compare ;

} // BinomialHeap<Type>::BinomialHeap()

template<class Type>
BinomialHeap<Type>::~BinomialHeap() { // destructor

  DeleteAllNode( mRootHead ) ;
  mRootHead = NULL ;

  delete[] mDegreeArray ;
  mDegreeArray = NULL ;

  mCompare = NULL ;

} // BinomialHeap<Type>::~BinomialHeap()

template<class Type>
void BinomialHeap<Type>::Insert( Type data ) {

  mDataSize++ ;

  MergeWith( new Node( data ) ) ;

} // BinomialHeap<Type>::Insert()

template<class Type>
void BinomialHeap<Type>::MergeWith( Node* nodePtr ) {

  if ( nodePtr == NULL ) {
    return ;
  } // if

  if ( mRootHead == NULL ) {
    mRootHead = nodePtr ;
    SetDegreeArray( mRootHead, mRootHead->degree ) ;
    return ;
  } // if

  nodePtr->leftPtr->rightPtr = NULL ;

  for ( Node* next = NULL ; nodePtr != NULL ; nodePtr = next ) {

    next = nodePtr->rightPtr ;

    while ( CheckDegreeArray( nodePtr->degree ) ) {

      /* degree exixt */

      Merge( nodePtr, mDegreeArray[nodePtr->degree] ) ;
      mDegreeArray[nodePtr->degree] = NULL ;     
      nodePtr->degree++ ;

    } // while

    /* degree NOT exixt */

    InsertNode( mRootHead, nodePtr ) ;   
    SetDegreeArray( mRootHead, mRootHead->degree ) ;

  } // for

} // BinomialHeap<Type>::MergeWith()

template<class Type>
void BinomialHeap<Type>::Merge( Node*& nodePtr, Node* tempPtr ) {

  if ( tempPtr == mRootHead ) {
    if ( tempPtr->rightPtr == mRootHead ) {
      mRootHead = NULL ;
    } // if
    else {
      mRootHead = tempPtr->rightPtr ;
    } // else    
  } // if

  tempPtr->leftPtr->rightPtr = tempPtr->rightPtr ;
  tempPtr->rightPtr->leftPtr = tempPtr->leftPtr ;

  if ( mCompare( nodePtr->content, tempPtr->content ) ) { // nodePtr up
    InsertNode( nodePtr->downPtr, tempPtr ) ;
  } // if
  else { // nodePtr down
    InsertNode( tempPtr->downPtr, nodePtr ) ;
    nodePtr = tempPtr ;
  } // else

} // BinomialHeap<Type>::Merge()

template<class Type>
void BinomialHeap<Type>::InsertNode( Node*& nodePtr, Node* sourceNodePtr ) {

  if ( nodePtr == NULL ) {
    sourceNodePtr->rightPtr = sourceNodePtr ;
    sourceNodePtr->leftPtr = sourceNodePtr ;   
  } // if
  else {
    sourceNodePtr->rightPtr = nodePtr ;
    sourceNodePtr->leftPtr = nodePtr->leftPtr ;
    nodePtr->leftPtr->rightPtr = sourceNodePtr ;
    nodePtr->leftPtr = sourceNodePtr ;
  } // else

  nodePtr = sourceNodePtr ;

} // BinomialHeap<Type>::InsertNode()

template<class Type>
bool BinomialHeap<Type>::GetTop( Type& data ) {

  if ( mRootHead == NULL ) {
    return false ;
  } // if

  Node* topRootPtr = NULL ;

  FindTopRoot( topRootPtr ) ;
  data = topRootPtr->content ;

  return true ;

} // BinomialHeap<Type>::GetTop()

template<class Type>
bool BinomialHeap<Type>::GetTopAndDelete( Type& data ) {

  if ( mRootHead == NULL ) {
    return false ;
  } // if

  Node* topRootPtr = NULL ;

  FindTopRoot( topRootPtr ) ;
  data = topRootPtr->content ;

  DeleteTopRoot( topRootPtr ) ;

  return true ;

} // BinomialHeap<Type>::GetTopAndDelete()

template<class Type>
void BinomialHeap<Type>::FindTopRoot( Node*& topRootPtr ) {

  if ( mRootHead == NULL ) {
    topRootPtr = NULL ;
    return ;
  } // if
  else {
    topRootPtr = mRootHead ;
  } // else  

  for ( Node* curPtr = mRootHead->rightPtr ; curPtr != mRootHead ; curPtr = curPtr->rightPtr ) {
    if ( mCompare( curPtr->content, topRootPtr->content ) ) {
      topRootPtr = curPtr ;
    } // if
  } // for

} // BinomialHeap<Type>::FindTopRoot()

template<class Type>
void BinomialHeap<Type>::DeleteTop() {

  if ( mDataSize == 0 ) {
    return ;
  } // if

  Node* topRootPtr = NULL ;

  FindTopRoot( topRootPtr ) ;
  DeleteTopRoot( topRootPtr ) ;

} // BinomialHeap<Type>::DeleteTop()

template<class Type>
void BinomialHeap<Type>::DeleteTopRoot( Node* topRootPtr ) {

  if ( mDataSize == 0 ) {
    return ;
  } // if

  mDataSize-- ;

  mDegreeArray[topRootPtr->degree] = NULL ;

  if ( topRootPtr == mRootHead ) {
    if ( topRootPtr->rightPtr == mRootHead ) {
      mRootHead = NULL ;
    } // if
    else {
      mRootHead = topRootPtr->rightPtr ;
    } // else    
  } // if

  topRootPtr->leftPtr->rightPtr = topRootPtr->rightPtr ;
  topRootPtr->rightPtr->leftPtr = topRootPtr->leftPtr ;

  MergeWith( topRootPtr->downPtr ) ;

  delete topRootPtr ;

} // BinomialHeap<Type>::DeleteTopRoot()

template<class Type>
void BinomialHeap<Type>::Union( BinomialHeap<Type>& sourceHeap ) {

  Node* rootHead = NULL ;

  CopyFrom( rootHead, sourceHeap.mRootHead ) ;
  MergeWith( rootHead ) ;

  mDataSize += sourceHeap.mDataSize ;

} // BinomialHeap<Type>::Union()

template<class Type>
void BinomialHeap<Type>::CopyFrom( Node*& nodePtr, Node* sourceNodePtr ) {

  if ( sourceNodePtr == NULL ) {
    nodePtr = NULL ;
    return ;
  } // if

  nodePtr = new Node( sourceNodePtr->content, sourceNodePtr->degree ) ;
  CopyFrom( nodePtr->downPtr, sourceNodePtr->downPtr ) ;

  Node* headPtr = nodePtr ;

  for ( Node* tempPtr = sourceNodePtr->rightPtr ; tempPtr != sourceNodePtr ; tempPtr = tempPtr->rightPtr ) {

    nodePtr->rightPtr = new Node( tempPtr->content, tempPtr->degree ) ;
    nodePtr->rightPtr->leftPtr = nodePtr ;
    nodePtr = nodePtr->rightPtr ;

    CopyFrom( nodePtr->downPtr, tempPtr->downPtr ) ;

  } // for

  headPtr->leftPtr = nodePtr ;
  nodePtr->rightPtr = headPtr ;

} // BinomialHeap<Type>::CopyFrom()

template<class Type>
void BinomialHeap<Type>::Clear() {

  DeleteAllNode( mRootHead ) ;
  mRootHead = NULL ;

  mDataSize = 0 ;

} // BinomialHeap<Type>::Clear()

template<class Type>
int BinomialHeap<Type>::GetSize() {

  return mDataSize ;

} // BinomialHeap<Type>::GetSize()

template<class Type>
void BinomialHeap<Type>::DeleteAllNode( Node* nodePtr ) {

  if ( nodePtr == NULL ) {
    return ;
  } // if

  nodePtr->leftPtr->rightPtr = NULL ;

  for ( Node* next ; nodePtr != NULL ; nodePtr = next ) {
    next = nodePtr->rightPtr ;
    DeleteAllNode( nodePtr->downPtr ) ;
    delete nodePtr ;
  } // for

} // BinomialHeap<Type>::DeleteAllNode()

template<class Type>
void BinomialHeap<Type>::SetDegreeArray( Node* nodePtr, int degree ) {

  if ( degree < 0 ) {
    return ;
  } // if

  if ( degree >= mArrayMaxSize ) {
    AdjustArraySize( degree ) ;
  } // if

  mDegreeArray[degree] = nodePtr ;

} // BinomialHeap<Type>::SetDegreeArray()

template<class Type>
bool BinomialHeap<Type>::CheckDegreeArray( int degree ) {

  /*
  true  : degree exixt
  false : degree NOT exist
  */

  if ( degree < 0 || degree >= mArrayMaxSize ) {
    return false ;
  } // if

  return mDegreeArray[degree] != NULL ;

} // BinomialHeap<Type>::CheckDegreeArray()

template<class Type>
void BinomialHeap<Type>::AdjustArraySize( int targetSize ) {

  Node** tempPtr = mDegreeArray ;
  int originalSize = mArrayMaxSize ;

  while ( targetSize >= mArrayMaxSize ) {
    mArrayMaxSize = mArrayMaxSize * 2 ;
  } // while  

  mDegreeArray = new Node*[mArrayMaxSize] ;

  for ( int i = 0 ; i < mArrayMaxSize ; i++ ) {
    mDegreeArray[i] = ( i < originalSize ? tempPtr[i] : NULL ) ;
  } // for 

  delete[] tempPtr ;
  tempPtr = NULL ;

} // BinomialHeap<Type>::AdjustArraySize()

#endif // BINOMIAL_HEAP_H_INCLUDED

