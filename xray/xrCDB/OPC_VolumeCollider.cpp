///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains base volume collider class.
 *	\file		OPC_VolumeCollider.cpp
 *	\author		Pierre Terdiman
 *	\date		June, 2, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains the abstract class for volume colliders.
 *
 *	\class		VolumeCollider
 *	\author		Pierre Terdiman
 *	\version	1.2
 *	\date		June, 2, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "stdafx.h"
#pragma hdrstop

using namespace Opcode;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeCollider::VolumeCollider()
    : mTouchedPrimitives(null),
#ifdef OPC_USE_CALLBACKS
      mUserData(0), mObjCallback(null),
#else
      mFaces(null), mVerts(null),
#endif
      mNbVolumeBVTests(0), mNbVolumePrimTests(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeCollider::~VolumeCollider() { mTouchedPrimitives = null; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Validates current settings. You should call this method after all the settings / callbacks have been defined for a
 *collider. \return		null if everything is ok, else a string describing the problem
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* VolumeCollider::ValidateSettings()
{
#ifdef OPC_USE_CALLBACKS
    if (!mObjCallback)
        return "Object callback must be defined! Call: SetCallback().";
#else
    if (!mFaces || !mVerts)
        return "Object pointers must be defined! Call: SetPointers().";
#endif
    return null;
}

// Pretty dumb way to dump - to do better

#define IMPLEMENT_NOLEAFDUMP(type)                                                                                     \
    void VolumeCollider::_Dump(const type* node)                                                                       \
    {                                                                                                                  \
        if (node->HasLeaf())                                                                                           \
            mTouchedPrimitives->Add(node->GetPrimitive());                                                             \
        else                                                                                                           \
            _Dump(node->GetPos());                                                                                     \
                                                                                                                       \
        if (ContactFound())                                                                                            \
            return;                                                                                                    \
                                                                                                                       \
        if (node->HasLeaf2())                                                                                          \
            mTouchedPrimitives->Add(node->GetPrimitive2());                                                            \
        else                                                                                                           \
            _Dump(node->GetNeg());                                                                                     \
    }

#define IMPLEMENT_LEAFDUMP(type)                                                                                       \
    void VolumeCollider::_Dump(const type* node)                                                                       \
    {                                                                                                                  \
        if (node->IsLeaf())                                                                                            \
        {                                                                                                              \
            mTouchedPrimitives->Add(node->GetPrimitive());                                                             \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            _Dump(node->GetPos());                                                                                     \
                                                                                                                       \
            if (ContactFound())                                                                                        \
                return;                                                                                                \
                                                                                                                       \
            _Dump(node->GetNeg());                                                                                     \
        }                                                                                                              \
    }

IMPLEMENT_NOLEAFDUMP(AABBNoLeafNode)
IMPLEMENT_NOLEAFDUMP(AABBQuantizedNoLeafNode)

IMPLEMENT_LEAFDUMP(AABBCollisionNode)
IMPLEMENT_LEAFDUMP(AABBQuantizedNode)
