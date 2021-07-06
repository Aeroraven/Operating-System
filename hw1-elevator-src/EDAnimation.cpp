#include "EDAnimation.h"

void EDAnimation::edaDuplicateCopies(int iNums, EDShapeBase* aBase, EDShapeBase* sStd) {
	for (int i = 0; i < iNums; i++) {
		aBase[i].edClone(*sStd);
	}
}

void EDAnimation::edaColorLinearTransform(int iNums, EDShapeBase* aBase, const EDVector3D& vBegin, const EDVector3D& vEnd) {
	GLfloat dx = vEnd.x - vBegin.x;
	GLfloat dy = vEnd.y - vBegin.y;
	GLfloat dz = vEnd.z - vBegin.z;
	GLfloat da = vEnd.alpha - vBegin.alpha;
	EDVector3D w;
	w.edvClone(vBegin);
	for (int i = 0; i < iNums; i++) {
		aBase[i].edSetColor(w);
		w.x += dx / (iNums - 1);
		w.y += dy / (iNums - 1);
		w.z += dz / (iNums - 1);
		w.alpha += da / (iNums - 1);
		//std::cout << w.alpha << std::endl;
	}
}

void EDAnimation::edabColorLinearTransform(int iNums,int vId, EDShapeBase* aBase, EDShapeBase* vBegin, EDShapeBase* vEnd) {
	GLfloat dx = vEnd->vVertexColorVector[vId].x - vBegin->vVertexColorVector[vId].x;
	GLfloat dy = vEnd->vVertexColorVector[vId].y - vBegin->vVertexColorVector[vId].y;
	GLfloat dz = vEnd->vVertexColorVector[vId].z - vBegin->vVertexColorVector[vId].z;
	GLfloat da = vEnd->vVertexColorVector[vId].alpha - vBegin->vVertexColorVector[vId].alpha;
	EDVector3D w;
	w.edvClone(vBegin->vVertexColorVector[vId]);
	for (int i = 0; i < iNums; i++) {
		aBase[i].edSetColorByVertex(vId,w);
		w.x += dx / (iNums - 1);
		w.y += dy / (iNums - 1);
		w.z += dz / (iNums - 1);
		w.alpha += da / (iNums - 1);
		//std::cout << w.alpha << std::endl;
	}
}

void EDAnimation::edabColorLinearTransformAll(int iNums, int vCnt, EDShapeBase* aBase, EDShapeBase* vBegin, EDShapeBase* vEnd) {
	for (int i = 0; i < vCnt; i++) {
		edabColorLinearTransform(iNums, i, aBase, vBegin, vEnd);
	}
}
void EDAnimation::edaColorCustomTransform1(int iNums, EDShapeBase* aBase, const EDVector3D& vBegin, const EDVector3D& vEnd) {
	GLfloat dx = vEnd.x - vBegin.x;
	GLfloat dy = vEnd.y - vBegin.y;
	GLfloat dz = vEnd.z - vBegin.z;
	GLfloat da = vEnd.alpha - vBegin.alpha;
	EDVector3D w;
	w.edvClone(vBegin);
	for (int i = 0; i < iNums; i++) {
		aBase[i].edSetColor(w);
		w.x = vBegin.x+ dx * sqrt((float)i) / sqrt((float)iNums - 1);
		w.y = vBegin.y + dy* sqrt((float)i) / sqrt((float)iNums - 1);
		w.z = vBegin.z + dz * sqrt((float)i) / sqrt((float)iNums - 1);
		w.alpha = vBegin.alpha + da * sqrt((float)i) / sqrt((float)iNums - 1);
		//std::cout << w.alpha << std::endl;
	}
}

void EDAnimation::edabColorSetByDistance(EDShapeBase* aBase, const EDVector3D& vFocus, const EDVector3D& vNearColor, const EDVector3D& vFarColor) {
	std::vector<float> dist;
	float fMax = -1e20f;
	float fMin = 1e20f;
	float tmp;
	for (int i = 0; i < aBase->vVertexVector.size(); i++) {
		tmp = (aBase->vVertexVector[i].x - vFocus.x) * (aBase->vVertexVector[i].x - vFocus.x);
		tmp += (aBase->vVertexVector[i].y - vFocus.y) * (aBase->vVertexVector[i].y - vFocus.y);
		tmp = sqrt(tmp);
		dist.push_back(tmp);
		fMax = MAX(fMax, tmp);
		fMin = MIN(fMin, tmp);
	}
	float dr = vFarColor.x - vNearColor.x;
	float dg = vFarColor.y - vNearColor.y;
	float db = vFarColor.z - vNearColor.z;
	float da = vFarColor.alpha - vNearColor.alpha;
	aBase->edSetColorReallocate();
	for (int i = 0; i < aBase->vVertexColorVector.size(); i++) {
		EDVector3D newColor;
		newColor.x = vNearColor.x + dr * (dist[i] - fMin) / (fMax - fMin);
		newColor.y = vNearColor.y + dg * (dist[i] - fMin) / (fMax - fMin);
		newColor.z = vNearColor.z + db * (dist[i] - fMin) / (fMax - fMin);
		newColor.alpha = vNearColor.alpha + da * (dist[i] - fMin) / (fMax - fMin);
		aBase->edSetColorByVertex(i, newColor);
	}

}