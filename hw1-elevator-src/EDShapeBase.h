#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include "EDLog.h"
#include "glut.h"

//Elevator Dispatch - Shape Managements

class EDVector3D {
public:
	GLfloat x, y, z;
	GLfloat alpha = 1.0;

	void edvScale(const GLfloat& p);
	GLfloat edvGetDistance();
	void edvGetDirVector(EDVector3D& p);
	void edvClone(const EDVector3D& p);
	void edvSetVal(GLfloat tx, GLfloat ty, GLfloat tz = 0.0);
	void edvSetColor(int R, int G, int B ,int A = 100);
	float edvGetGrayscale();
	const EDVector3D& operator = (const EDVector3D&);

	EDVector3D(const EDVector3D& p);
	EDVector3D();
};
typedef EDVector3D EDVector4D;

class EDShapeBase{
public:
	static const int EDS_FILL_BY_SHAPE = 0;
	static const int EDS_FILL_BY_VERTEX = 1;

	int iFillMode = 0;
	int iZLayer = 0;
	std::vector<EDVector3D> vVertexVector;
	std::vector<EDVector3D> vVertexColorVector;
	EDVector3D vColorVector;
	GLenum cGLShapeMode = 0;
	bool bVisible = true;
	bool bTextFlag = false;
	std::string sText = "";
	GLfloat iFontScale = 0.15f;
	GLfloat iFontBold = 1;

	void edGLDraw();
	void edClear();
	void edSetMode(const GLenum&);
	void edAddVertex(const EDVector3D&);
	void edAddVertex(const EDVector3D&,const EDVector3D&);
	void edSetColor(const EDVector3D&);

	void edSetColorReallocate();
	void edSetColorByVertex(int,const EDVector3D&);

	void edClone(const EDShapeBase&);
	void edToggleVisibility(bool);
	void edSetZIndex(const int& z) { iZLayer = z; }

	void edSetTextMode(std::string);

	const EDShapeBase&  operator =(const EDShapeBase&);
	EDShapeBase(const EDShapeBase& p);
	EDShapeBase();
};

class EDShape_Rectangle :public EDShapeBase {
public:
	void edPresetRectangle(EDVector3D lb, EDVector3D rb, EDVector3D lt, EDVector3D rt);
};

class EDShape_RoundedRectangle : public EDShapeBase {
public:
	void edPresetRoundRect(EDVector3D lb, EDVector3D rb, EDVector3D lt, EDVector3D rt, GLfloat radius);
};

class EDShape_Circle :public EDShapeBase {
public:
	void edPresetCircle(EDVector3D center, GLfloat radius);
};

class EDShape_Pie :public EDShapeBase {
public:
	void edPresetPie(EDVector3D center, GLfloat radius,int angle);
};

class EDShape_Arc :public EDShapeBase {
public:
	void edPresetArc(EDVector3D center, GLfloat radius1, GLfloat radius2, int angle);
};
class EDShape_Text :public EDShapeBase {
public:
	void edPresetText(std::string s);
};

class EDShape_IsoscelesRightTriangle : public EDShapeBase {
public:
	void edPresetTriangleA(EDVector3D top, GLfloat height);
	void edPresetTriangleB(EDVector3D top, GLfloat height);
};

class EDShape_Arrow :public EDShapeBase {
public:
	void edPresetArrowA(EDVector3D top);
	void edPresetArrowB(EDVector3D top);
};

class EDShape {
public:
	std::vector<EDShapeBase> vComponentContainer;
	void edPaste(EDShape* dest);
	void edAdd(EDShapeBase* dest);
	void edDraw();
};