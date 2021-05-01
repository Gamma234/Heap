#ifndef FIBONACCI_HEAP_H_INCLUDED
#define FIBONACCI_HEAP_H_INCLUDED

# include <stdio.h>

template<class Type>
class FibonacciHeap {

private:

    struct Node {
        Type content ;
        int degree ;
        Node * parentPtr ;
        Node * leftPtr ;
        Node * rightPtr ;
        Node * downPtr ;
        Node( Type content ) ; // constructor
        Node( Type content, int degree ) ; // constructor
    } ; // struct Node

    /* Degree Array */

    Node ** mDegreeArray ;
    int mArrayMaxSize ;

    void BuildDegreeArray() ;
    void FreeDegreeArray() ;

    void SetDegreeArray( Node * nodePtr, int degree ) ;
    bool CheckDegreeArray( int degree ) ;

    void AdjustArraySize( int targetSize ) ;

    /* Forest */

    Node * mTopRoot ;
    int mDataSize ;
    bool ( *mCompare ) ( Type data1, Type data2 ) ;

    void Consolidate() ;
    void Merge( Node *& nodePtr, Node * tempPtr ) ;
    void InsertNode( Node *& nodePtr, Node * sourceNodePtr ) ;

    void ConcatenateWith( Node * nodePtr ) ;
    void DeleteTopRoot() ;

    void CopyFrom( Node *& nodePtr, Node * sourceNodePtr, Node * parentPtr ) ;
    void DeleteAllNode( Node * nodePtr ) ;

    void OutputAllNode( Node * nodePtr, int layer ) ;

public:

    FibonacciHeap( bool ( *compare ) ( Type data1, Type data2 ) ) ; // constructor
    ~FibonacciHeap() ; // destructor

    void Insert( Type data ) ;

    bool GetTop( Type & data ) ;
    bool GetTopAndDelete( Type & data ) ;
    void DeleteTop() ;

    void Union( FibonacciHeap<Type> & sourceHeap ) ;

    void Clear() ;
    int GetSize() ;

    void Output() ;

} ; // class FibonacciHeap

template<class Type>
FibonacciHeap<Type>::Node::Node( Type content ) { // constructor

    this->content = content ;
    this->degree = 0 ;
    this->parentPtr = NULL ;
    this->leftPtr = this ;
    this->rightPtr = this ;
    this->downPtr = NULL ;

} // FibonacciHeap<Type>::Node::Node()

template<class Type>
FibonacciHeap<Type>::Node::Node( Type content, int degree ) { // constructor

    this->content = content ;
    this->degree = degree ;
    this->parentPtr = NULL ;
    this->leftPtr = this ;
    this->rightPtr = this ;
    this->downPtr = NULL ;

} // FibonacciHeap<Type>::Node::Node()

template<class Type>
FibonacciHeap<Type>::FibonacciHeap( bool ( *compare ) ( Type data1, Type data2 ) ) { // constructor

    mDegreeArray = NULL ;
    mArrayMaxSize = 10 ;

    mTopRoot = NULL ;
    mDataSize = 0 ;
    mCompare = compare ;

} // FibonacciHeap<Type>::FibonacciHeap()

template<class Type>
FibonacciHeap<Type>::~FibonacciHeap() { // destructor

    DeleteAllNode( mTopRoot ) ;
    mTopRoot = NULL ;

    mCompare = NULL ;

} // FibonacciHeap<Type>::~FibonacciHeap()

template<class Type>
void FibonacciHeap<Type>::Insert( Type data ) {

    mDataSize++ ;

    ConcatenateWith( new Node( data ) ) ;

} // FibonacciHeap<Type>::Insert()

template<class Type>
void FibonacciHeap<Type>::ConcatenateWith( Node * nodePtr ) {

    if ( nodePtr == NULL ) {
        return ;
    } // if

    if ( mTopRoot == NULL ) {
        mTopRoot = nodePtr ;
    } // if
    else {

        Node * tempPtr = nodePtr->leftPtr ;

        nodePtr->leftPtr->rightPtr = mTopRoot ;
        nodePtr->leftPtr = mTopRoot->leftPtr ;

        mTopRoot->leftPtr->rightPtr = nodePtr ;
        mTopRoot->leftPtr = tempPtr ;

        if ( mCompare( nodePtr->content, mTopRoot->content ) ) {
            mTopRoot = nodePtr ;
        } // if

    } // else

} // FibonacciHeap<Type>::ConcatenateWith()

template<class Type>
bool FibonacciHeap<Type>::GetTop( Type & data ) {

    if ( mTopRoot == NULL ) {
        return false ;
    } // if

    data = mTopRoot->content ;

    return true ;

} // FibonacciHeap<Type>::GetTop()

template<class Type>
bool FibonacciHeap<Type>::GetTopAndDelete( Type & data ) {

    if ( mTopRoot == NULL ) {
        return false ;
    } // if

    data = mTopRoot->content ;
    DeleteTopRoot() ;

    return true ;

} // FibonacciHeap<Type>::GetTopAndDelete()

template<class Type>
void FibonacciHeap<Type>::DeleteTop() {

    if ( mDataSize == 0 ) {
        return ;
    } // if

    DeleteTopRoot() ;

} // FibonacciHeap<Type>::DeleteTop()

template<class Type>
void FibonacciHeap<Type>::DeleteTopRoot() {

    if ( mDataSize == 0 ) {
        return ;
    } // if

    mDataSize-- ;

    Node * tempPtr = mTopRoot->downPtr ;
    Node * nodePtr = NULL ;

    if ( mTopRoot != mTopRoot->rightPtr ) {
        nodePtr = mTopRoot->rightPtr ;
    } // if

    mTopRoot->leftPtr->rightPtr = mTopRoot->rightPtr ;
    mTopRoot->rightPtr->leftPtr = mTopRoot->leftPtr ;

    delete mTopRoot ;
    mTopRoot = nodePtr ; // NULL or This is not certainly the real top root.

    if ( tempPtr != NULL ) {

        tempPtr->parentPtr = NULL ;

        for ( Node * walkPtr = tempPtr->rightPtr ; walkPtr != tempPtr ; walkPtr = walkPtr->rightPtr ) {
            walkPtr->parentPtr = NULL ;
        } // for

        ConcatenateWith( tempPtr ) ;

    } // if

    if ( mTopRoot != NULL ) {
        Consolidate() ;
    } // if  

} // FibonacciHeap<Type>::DeleteTopRoot()

template<class Type>
void FibonacciHeap<Type>::Consolidate() {

    BuildDegreeArray() ;

    Node * endPtr = mTopRoot ; // This is not certainly the real top root, just a temporary location.
    Node * nodePtr = mTopRoot->rightPtr ;
    bool running = true ;

    while ( running ) {

        if ( nodePtr == endPtr ) {
            running = false ;
        } // if

        while ( CheckDegreeArray( nodePtr->degree ) ) {

            Merge( nodePtr, mDegreeArray[nodePtr->degree] ) ;
            mDegreeArray[nodePtr->degree] = NULL ;
            nodePtr->degree++ ;

        } // while

        SetDegreeArray( nodePtr, nodePtr->degree ) ;

        nodePtr = nodePtr->rightPtr ;

    } // while

    FreeDegreeArray() ;

    /* set the real top root */

    mTopRoot = nodePtr ;

    for ( Node * walkPtr = nodePtr->rightPtr ; walkPtr != nodePtr ; walkPtr = walkPtr->rightPtr ) {
        if ( mCompare( walkPtr->content, mTopRoot->content ) ) {
            mTopRoot = walkPtr ;
        } // if
    } // for

} // FibonacciHeap<Type>::Consolidate()

template<class Type>
void FibonacciHeap<Type>::Merge( Node *& nodePtr, Node * tempPtr ) {

    tempPtr->leftPtr->rightPtr = tempPtr->rightPtr ;
    tempPtr->rightPtr->leftPtr = tempPtr->leftPtr ;

    if ( mCompare( nodePtr->content, tempPtr->content ) ) { // nodePtr up
        InsertNode( nodePtr->downPtr, tempPtr ) ;
        tempPtr->parentPtr = nodePtr ;
    } // if
    else { // nodePtr down

        if ( nodePtr == nodePtr->rightPtr ) {
            tempPtr->leftPtr = tempPtr ;
            tempPtr->rightPtr = tempPtr ;
        } // if
        else {
            tempPtr->leftPtr = nodePtr->leftPtr ;
            tempPtr->rightPtr = nodePtr->rightPtr ;
            nodePtr->leftPtr->rightPtr = tempPtr ;
            nodePtr->rightPtr->leftPtr = tempPtr ;
        } // else

        InsertNode( tempPtr->downPtr, nodePtr ) ;

        nodePtr->parentPtr = tempPtr ;
        nodePtr = tempPtr ;

    } // else

} // FibonacciHeap<Type>::Merge()

template<class Type>
void FibonacciHeap<Type>::InsertNode( Node *& nodePtr, Node * sourceNodePtr ) {

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

} // FibonacciHeap<Type>::InsertNode()

template<class Type>
void FibonacciHeap<Type>::Output() {

    printf( "----------\n" ) ;

    OutputAllNode( mTopRoot, 0 ) ;

    printf( "\nroot = %d\n", ( mTopRoot != NULL ? mTopRoot->content : -1 ) ) ;

    printf( "----------\n" ) ;

} // FibonacciHeap<Type>::Output()

template<class Type>
void FibonacciHeap<Type>::OutputAllNode( Node * nodePtr, int layer ) {

    if ( nodePtr == NULL ) {
        return ;
    } // if

    Node * end = NULL ;

    for ( Node * walk = nodePtr ; walk != end ; walk = walk->rightPtr ) {

        end = nodePtr ;

        for ( int space = 0 ; space < layer * 2 ; space++ ) {
            printf( " " ) ;
        } // for
        printf( "layer = %d\n", layer ) ;
        for ( int space = 0 ; space < layer * 2 ; space++ ) {
            printf( " " ) ;
        } // for
        printf( "content = %d  self = %p  parentPtr = %p  leftPtr = %p  rightPtr = %p  downPtr = %p  degree = %d \n",
                walk->content, walk, walk->parentPtr, walk->leftPtr, walk->rightPtr, walk->downPtr, walk->degree ) ;

        OutputAllNode( walk->downPtr, layer + 1 ) ;

    } // for

} // FibonacciHeap<Type>::OutputAllNode()

template<class Type>
void FibonacciHeap<Type>::Union( FibonacciHeap<Type> & sourceHeap ) {

    Node * newTopRoot = NULL ;

    CopyFrom( newTopRoot, sourceHeap.mTopRoot, NULL ) ;
    ConcatenateWith( newTopRoot ) ;

    mDataSize += sourceHeap.mDataSize ;

} // FibonacciHeap<Type>::Union()

template<class Type>
void FibonacciHeap<Type>::CopyFrom( Node *& nodePtr, Node * sourceNodePtr, Node * parentPtr ) {

    if ( sourceNodePtr == NULL ) {
        return ;
    } // if

    /* Deal with first node. */

    nodePtr = new Node( sourceNodePtr->content, sourceNodePtr->degree ) ;
    nodePtr->parentPtr = parentPtr ;

    if ( sourceNodePtr->downPtr != NULL ) {
        CopyFrom( nodePtr->downPtr, sourceNodePtr->downPtr, nodePtr ) ;
    } // if

    /* Deal with other nodes. */

    for ( Node * walkPtr = nodePtr, * srcWalkPtr = sourceNodePtr->rightPtr ;
          srcWalkPtr != sourceNodePtr ;
          walkPtr = walkPtr->rightPtr, srcWalkPtr = srcWalkPtr->rightPtr ) {

        walkPtr->rightPtr = new Node( srcWalkPtr->content, srcWalkPtr->degree ) ;
        walkPtr->rightPtr->parentPtr = parentPtr ;

        nodePtr->leftPtr = walkPtr->rightPtr ;

        walkPtr->rightPtr->leftPtr = walkPtr ;
        walkPtr->rightPtr->rightPtr = nodePtr ;

        if ( srcWalkPtr->downPtr != NULL ) {
            CopyFrom( walkPtr->rightPtr->downPtr, srcWalkPtr->downPtr, walkPtr->rightPtr ) ;
        } // if

    } // for

} // FibonacciHeap<Type>::CopyFrom()

template<class Type>
void FibonacciHeap<Type>::Clear() {

    DeleteAllNode( mTopRoot ) ;
    mTopRoot = NULL ;

    mDataSize = 0 ;

} // FibonacciHeap<Type>::Clear()

template<class Type>
int FibonacciHeap<Type>::GetSize() {

    return mDataSize ;

} // FibonacciHeap<Type>::GetSize()

template<class Type>
void FibonacciHeap<Type>::DeleteAllNode( Node * nodePtr ) {

    if ( nodePtr == NULL ) {
        return ;
    } // if

    nodePtr->leftPtr->rightPtr = NULL ;

    for ( Node * next = NULL ; nodePtr != NULL ; nodePtr = next ) {
        next = nodePtr->rightPtr ;
        DeleteAllNode( nodePtr->downPtr ) ;
        delete nodePtr ;
    } // for

} // FibonacciHeap<Type>::DeleteAllNode()

template<class Type>
void FibonacciHeap<Type>::BuildDegreeArray() {

    mArrayMaxSize = 10 ;
    mDegreeArray = new Node * [mArrayMaxSize] ;

    for ( int i = 0 ; i < mArrayMaxSize ; i++ ) {
        mDegreeArray[i] = NULL ;
    } // for

} // FibonacciHeap<Type>::BuildDegreeArray()

template<class Type>
void FibonacciHeap<Type>::FreeDegreeArray() {

    delete[] mDegreeArray ;
    mDegreeArray = NULL ;

} // FibonacciHeap<Type>::FreeDegreeArray()

template<class Type>
void FibonacciHeap<Type>::SetDegreeArray( Node * nodePtr, int degree ) {

    if ( degree < 0 ) {
        return ;
    } // if

    if ( degree >= mArrayMaxSize ) {
        AdjustArraySize( degree ) ;
    } // if

    mDegreeArray[degree] = nodePtr ;

} // FibonacciHeap<Type>::SetDegreeArray()

template<class Type>
bool FibonacciHeap<Type>::CheckDegreeArray( int degree ) {

    /*
    true  : degree exixt
    false : degree NOT exist
    */

    if ( degree < 0 || degree >= mArrayMaxSize ) {
        return false ;
    } // if

    return mDegreeArray[degree] != NULL ;

} // FibonacciHeap<Type>::CheckDegreeArray()

template<class Type>
void FibonacciHeap<Type>::AdjustArraySize( int targetSize ) {

    Node ** tempPtr = mDegreeArray ;
    int originalSize = mArrayMaxSize ;

    while ( targetSize >= mArrayMaxSize ) {
        mArrayMaxSize = mArrayMaxSize * 2 ;
    } // while  

    mDegreeArray = new Node * [mArrayMaxSize] ;

    for ( int i = 0 ; i < mArrayMaxSize ; i++ ) {
        mDegreeArray[i] = ( i < originalSize ? tempPtr[i] : NULL ) ;
    } // for 

    delete[] tempPtr ;
    tempPtr = NULL ;

} // FibonacciHeap<Type>::AdjustArraySize()

#endif // FIBONACCI_HEAP_H_INCLUDED

