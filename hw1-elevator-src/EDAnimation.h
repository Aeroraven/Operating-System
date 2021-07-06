#pragma once
#include <algorithm>
#include"EDShapeBase.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// Class used for graphical animations.

class EDAnimation{
public:

	static void edaDuplicateCopies(int iNums, EDShapeBase* aBase,EDShapeBase* sStd);
	static void edaColorLinearTransform(int iNums, EDShapeBase* aBase, const EDVector3D& vBegin, const EDVector3D& vEnd);
	static void edaColorCustomTransform1(int iNums, EDShapeBase* aBase, const EDVector3D& vBegin, const EDVector3D& vEnd);

	static void edabColorSetByDistance(EDShapeBase* aBase, const EDVector3D& vFocus,const EDVector3D& vNearColor,const EDVector3D& vFarColor);
	static void edabColorLinearTransform(int iNums, int vId, EDShapeBase* aBase, EDShapeBase* vBegin, EDShapeBase* vEnd);
	static void edabColorLinearTransformAll(int iNums, int vCnt, EDShapeBase* aBase, EDShapeBase* vBegin, EDShapeBase* vEnd);

};

typedef  EDAnimation EDEffect;