///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for OPCODE models.
 *	\file		OPC_Model.h
 *	\author		Pierre Terdiman
 *	\date		March, 20, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_MODEL_H__
#define __OPC_MODEL_H__

//! Model creation structure
struct OPCODE_API OPCODECREATE
{
    //! Constructor
    OPCODECREATE();

    udword NbTris; //!< Number of triangles in the input model
    udword NbVerts; //!< Number of vertices in the input model
    const udword* Tris; //!< List of indexed triangles
    const Point* Verts; //!< List of points
    udword Rules; //!< Splitting rules (SPLIT_COMPLETE is mandatory in OPCODE)
    bool NoLeaf; //!< true => discard leaf nodes (else use a normal tree)
    bool Quantized; //!< true => quantize the tree (else use a normal tree)
#ifdef __MESHMERIZER_H__
    bool CollisionHull; //!< true => use convex hull + GJK
#endif // __MESHMERIZER_H__
    bool KeepOriginal; //!< true => keep a copy of the original tree (debug purpose)
};

class OPCODE_API OPCODE_Model
{
public:
    // Constructor/Destructor
    OPCODE_Model();
    ~OPCODE_Model();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     *	Builds a collision model.
     *	\param		create		[in] model creation structure
     *	\return		true if success
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool Build(const OPCODECREATE& create);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     *	A method to access the tree.
     *	\return		the collision tree
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline_ const AABBOptimizedTree* GetTree() const { return mTree; }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     *	A method to access the source tree.
     *	\return		generic tree
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline_ const AABBTree* GetSourceTree() const { return mSource; }

#ifdef __MESHMERIZER_H__
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     *	A method to access the collision hull.
     *	\return		the collision hull if it exists
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline_ const CollisionHull* GetHull() const { return mHull; }
#endif // __MESHMERIZER_H__

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     *	Checks whether the tree has leaf nodes or not.
     *	\return		true if the tree has leaf nodes (normal tree), else false (optimized tree)
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline_ bool HasLeafNodes() const { return !mNoLeaf; }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     *	Checks whether the tree is quantized or not.
     *	\return		true if the tree is quantized
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline_ bool IsQuantized() const { return mQuantized; }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     *	Gets the number of nodes in the tree.
     *	Should be 2*N-1 for normal trees and N-1 for optimized ones.
     *	\return		number of nodes
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    udword GetNbNodes() const { return mTree->GetNbNodes(); }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     *	Gets the number of bytes used by the tree.
     *	\return		amount of bytes used
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    udword GetUsedBytes() const { return mTree->GetUsedBytes(); }

private:
    AABBTree* mSource; //!< Original source tree
    AABBOptimizedTree* mTree; //!< Optimized tree
#ifdef __MESHMERIZER_H__
    CollisionHull* mHull; //!< Possible convex hull
#endif // __MESHMERIZER_H__
    bool mNoLeaf; //!< Leaf/NoLeaf tree
    bool mQuantized; //!< Compressed/uncompressed tree
};

#endif //__OPC_MODEL_H__
