#include "EDShapeBase.h"

float EDVector3D::edvGetGrayscale(){
	return x * 0.299f + y * 0.587f + z * 0.114f;
}
void EDShapeBase::edGLDraw() {
	float p = EDGlobal::edglColorMultiplier;
	if (iFillMode == EDS_FILL_BY_SHAPE) {
		if (EDGlobal::edglColorMode == EDGLM_GRAYSCALE) {
			float g = vColorVector.edvGetGrayscale();
			glColor4f(g * p, g * p, g * p, vColorVector.alpha);
		}
		else {
			glColor4f(vColorVector.x * p, vColorVector.y * p, vColorVector.z * p, vColorVector.alpha);
		}
		
	}
	if (bTextFlag == false) {
		glLineWidth(2);
		
		glBegin(cGLShapeMode);
		for (int i = 0; i < vVertexVector.size(); i++) {
			if (iFillMode == EDS_FILL_BY_VERTEX) {
				if (EDGlobal::edglColorMode == EDGLM_GRAYSCALE) {
					float g = vVertexColorVector[i].edvGetGrayscale();
					glColor4f(g * p, g * p, g * p, vColorVector.alpha);
				}else{
					glColor4f(vVertexColorVector[i].x * p, vVertexColorVector[i].y * p,
						vVertexColorVector[i].z * p, vVertexColorVector[i].alpha);
				}
			}
			glVertex3f(vVertexVector[i].x, vVertexVector[i].y,static_cast<float>(this->iZLayer));
		}
		glEnd();
		glLineWidth(1);
	}
	else {
		glPushMatrix();
		glTranslatef(vVertexVector[0].x, vVertexVector[0].y, 0);
		glScalef(iFontScale, iFontScale, 1);
		glLineWidth(iFontBold);
		for (char* i = (char*)sText.c_str(); *i != '\0'; i++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *i);
		}
		glLineWidth(1);
		glPopMatrix();
	}
}

void EDShapeBase::edSetTextMode(std::string s) {
	sText = s;
	bTextFlag = true;
}

void EDShapeBase::edSetMode(const GLenum& p) {
	cGLShapeMode = p;
}

void EDShapeBase::edClear() {
	vVertexVector.clear();
}

void EDShapeBase::edAddVertex(const EDVector3D& p) {
	if (iFillMode != EDS_FILL_BY_SHAPE) {
		return;
	}
	vVertexVector.push_back(p);
}

void EDShapeBase::edAddVertex(const EDVector3D& p,const EDVector3D& c) {
	if (iFillMode != EDS_FILL_BY_VERTEX) {
		return;
	}
	vVertexVector.push_back(p);
	vVertexVector.push_back(c);
}

void EDShapeBase::edSetColor(const EDVector3D& p) {
	if (iFillMode != EDS_FILL_BY_SHAPE) {
		return;
	}
	vColorVector.x = p.x;//R
	vColorVector.y = p.y;//G
	vColorVector.z = p.z;//B
	vColorVector.alpha = p.alpha;//A
}

void EDShapeBase::edSetColorReallocate() {
	vVertexColorVector.clear();
	EDVector3D vdef;
	vdef.alpha = 1;
	vdef.x = 0;
	vdef.y = 0;
	vdef.z = 0;
	for (int i = 0; i < vVertexVector.size(); i++) {
		vVertexColorVector.push_back(vdef);
	}
}
void EDShapeBase::edSetColorByVertex(int id, const EDVector3D& cl) {
	vVertexColorVector[id].edvClone(cl);
}
void EDShapeBase::edClone(const EDShapeBase& p) {
	vVertexVector.clear();
	vVertexColorVector.clear();
	for (int i = 0; i < p.vVertexVector.size(); i++) {
		const std::vector<EDVector3D>& tmp = p.vVertexVector;
		const std::vector<EDVector3D>& tmp2 = p.vVertexColorVector;
		vVertexVector.push_back(tmp[i]);
		if (p.iFillMode == EDS_FILL_BY_VERTEX) {
			vVertexColorVector.push_back(tmp2[i]);
		}
	}
	edSetColor(p.vColorVector);
	edSetMode(p.cGLShapeMode);
	iFillMode = p.iFillMode;
	bVisible = p.bVisible;
	bTextFlag = p.bTextFlag;
	sText = p.sText;
	iFontScale = p.iFontScale;
	iFontBold = p.iFontBold;
	iZLayer = p.iZLayer;
}

const EDShapeBase& EDShapeBase::operator=(const EDShapeBase& p) {
	this->edClone(p);
	return (*this);
}

void EDShapeBase::edToggleVisibility(bool p) {
	bVisible = p;
}

EDShapeBase::EDShapeBase(const EDShapeBase& p) {
	this->edClone(p);
}

EDShapeBase::EDShapeBase() {
	//Nothing here
}

void EDShape_Rectangle::edPresetRectangle(EDVector3D lb, EDVector3D rb, EDVector3D lt, EDVector3D rt) {
	edAddVertex(lb);
	edAddVertex(rb);
	edAddVertex(rt);
	edAddVertex(lt);
	edSetMode(GL_POLYGON);
}

void EDShape_RoundedRectangle::edPresetRoundRect(EDVector3D lb, EDVector3D rb, EDVector3D lt, EDVector3D rt, GLfloat radius) {
	float pi = 3.14150926f;
	float pi_half = pi / 2.0f;
	int step = 15;
	float dx = pi_half / step;

	//lb_bottom
	EDVector3D lbb;
	lbb.edvClone(lb);
	lbb.x += radius;
	edAddVertex(lbb);

	//rb_bottom
	EDVector3D rbb;
	rbb.edvClone(rb);
	//rbb.x -= radius;
	edAddVertex(rbb);

	//rb_arc
	EDVector3D rbcx;
	rbcx.edvClone(rb);
	rbcx.x -= radius;
	rbcx.y += radius;
	EDVector3D* rbc = new EDVector3D[step];
	for (int i = 0; i < step; i++) {
		rbc[i].edvClone(rbcx);
		rbc[i].x += radius * cos(pi * 3.0f / 2 + i * dx);
		rbc[i].y += radius * sin(pi * 3.0f / 2 + i * dx);
		edAddVertex(rbc[i]);
	}
	delete[] rbc;
	//rtr
	EDVector3D rtr;
	rtr.edvClone(rt);
	rtr.y -= radius;
	edAddVertex(rtr);

	//rt_arc
	EDVector3D rtcx;
	rtcx.edvClone(rt);
	rtcx.x -= radius;
	rtcx.y -= radius;
	EDVector3D* rtc = new EDVector3D[step];
	for (int i = 0; i < step; i++) {
		rtc[i].edvClone(rtcx);
		rtc[i].x += radius * cos(i * dx);
		rtc[i].y += radius * sin(i * dx);
		edAddVertex(rtc[i]);
	}
	delete[] rtc;

	//ltt
	EDVector3D ltt;
	ltt.edvClone(lt);
	ltt.x += radius;
	edAddVertex(ltt);

	//lt_arc
	EDVector3D ltcx;
	ltcx.edvClone(lt);
	ltcx.x += radius;
	ltcx.y -= radius;
	EDVector3D* ltc = new EDVector3D[step];
	for (int i = 0; i < step; i++) {
		ltc[i].edvClone(ltcx);
		ltc[i].x += radius * cos(pi / 2.0f + i * dx);
		ltc[i].y += radius * sin(pi / 2.0f + i * dx);
		edAddVertex(ltc[i]);
	}
	delete[] ltc;
	//lbl
	EDVector3D lbl;
	lbl.edvClone(lb);
	lbl.y += radius;
	edAddVertex(lbl);

	//lb_arc
	EDVector3D lbcx;
	lbcx.edvClone(lb);
	lbcx.x += radius;
	lbcx.y += radius;
	EDVector3D* lbc = new EDVector3D[step];
	for (int i = 0; i < step; i++) {
		lbc[i].edvClone(lbcx);
		lbc[i].x += radius * cos(pi + i * dx);
		lbc[i].y += radius * sin(pi + i * dx);
		edAddVertex(lbc[i]);
	}
	delete[] lbc;
	edSetMode(GL_POLYGON);
}



void EDShape_Circle::edPresetCircle(EDVector3D center, GLfloat radius) {
	int steps = 360;
	float pi = 3.1415926f;
	EDVector3D* t = new EDVector3D[steps];
	for (int i = 0; i < steps; i++) {
		t[i].edvClone(center);
		t[i].x += radius * cos(2 * pi * i / steps);
		t[i].y += radius * sin(2 * pi * i / steps);
		edAddVertex(t[i]);
	}
	delete[]t;
	edSetMode(GL_POLYGON);
}

void EDShape_Pie::edPresetPie(EDVector3D center, GLfloat radius, int angle) {
	int steps = 360 * angle / 360;
	float pi = 3.1415926f;
	EDVector3D* t = new EDVector3D[steps];
	for (int i = 0; i < steps; i++) {
		t[i].edvClone(center);
		t[i].x += radius * cos(2 * pi * i / steps);
		t[i].y += radius * sin(2 * pi * i / steps);
		edAddVertex(t[i]);
	}
	delete[]t;
	edSetMode(GL_POLYGON);
}

void EDShape_Arc::edPresetArc(EDVector3D center, GLfloat radius1, GLfloat radius2, int angle) {
	int steps = angle;
	float pi = 3.1415926f;
	EDVector3D* t = new EDVector3D[steps];
	for (int i = 0; i < steps; i++) {
		t[i].edvClone(center);
		t[i].x += radius1 * cos(2 * pi * i / 360 + 2 * pi * 0.25f);
		t[i].y += radius1 * sin(2 * pi * i / 360 + 2 * pi * 0.25f);
		edAddVertex(t[i]);
	}
	delete[] t;
	edSetMode(GL_LINE_STRIP);
	this->iZLayer = 9;
}

void EDVector3D::edvScale(const GLfloat& p) {
	x *= p;
	y *= p;
	z *= p;
}

GLfloat EDVector3D::edvGetDistance() {
	return sqrt(x * x + y * y + z * z);
}

void EDVector3D::edvGetDirVector(EDVector3D& p) {
	GLfloat dist = edvGetDistance();
	p.x /= dist;
	p.y /= dist;
	p.z /= dist;
}

void EDVector3D::edvClone(const EDVector3D& p) {
	x = p.x;
	y = p.y;
	z = p.z;
	alpha = p.alpha;
	
}

const EDVector3D& EDVector3D::operator=(const EDVector3D& p) {
	this->edvClone(p);
	return(*this);
}

EDVector3D::EDVector3D(const EDVector3D& p) {
	edvClone(p);
}
EDVector3D::EDVector3D() {
	x = 0;
	y = 0;
	z = 0;
	alpha = 1;
}

void EDShape::edDraw() {
	for (int i = 0; i < vComponentContainer.size(); i++) {
		vComponentContainer[i].edGLDraw();
	}
}

void EDShape::edPaste(EDShape* dest) {
	vComponentContainer.clear();
	for (int i = 0; i < dest->vComponentContainer.size(); i++) {
		EDShapeBase* p = new EDShapeBase();
		p->edClone(dest->vComponentContainer[i]);
		vComponentContainer.push_back(*p);
		delete p;
	}
}

void EDVector3D::edvSetVal(GLfloat tx, GLfloat ty, GLfloat tz) {
	x = tx;
	y = ty;
	z = tz;
}

void EDShape::edAdd(EDShapeBase* dest) {
	EDShapeBase* t = new EDShapeBase();
	(*t).edClone(*dest);
	vComponentContainer.push_back(*t);
	delete t;
}

void EDShape_Text::edPresetText(std::string s) {
	edSetTextMode(s);
}

void EDVector3D::edvSetColor(int R, int G, int B, int A) {
	x = 1.0f * R / 256;
	y = 1.0f * G / 256;
	z = 1.0f * B / 256;
	alpha = 1.0f * A / 100;
}

void EDShape_IsoscelesRightTriangle::edPresetTriangleA(EDVector3D top, GLfloat height) {
	EDVector3D lb, rb;
	lb.edvClone(top);
	lb.y -= height;
	lb.x -= height;
	rb.edvClone(top);
	rb.x += height;
	rb.y -= height;
	edAddVertex(top);
	edAddVertex(rb);
	edAddVertex(lb);
	edSetMode(GL_POLYGON);
}

void EDShape_IsoscelesRightTriangle::edPresetTriangleB(EDVector3D top, GLfloat height) {
	EDVector3D lb, rb;
	lb.edvClone(top);
	lb.y += height;
	lb.x -= height;
	rb.edvClone(top);
	rb.x += height;
	rb.y += height;
	edAddVertex(top);
	edAddVertex(rb);
	edAddVertex(lb);
	edSetMode(GL_POLYGON);
}

void EDShape_Arrow::edPresetArrowA(EDVector3D top) {
	EDVector3D left, right, bottom;
	left.edvClone(top);
	right.edvClone(top);
	bottom.edvClone(top);
	left.x -= 10.0f;
	left.y -= 10.0f;
	right.x += 10.0f;
	right.y -= 10.0f;
	bottom.y -= 20.0f;
	edAddVertex(left);
	edAddVertex(top);
	edAddVertex(right);
	edAddVertex(top);
	edAddVertex(bottom);
	edSetMode(GL_LINE_STRIP);
	return;
}

void EDShape_Arrow::edPresetArrowB(EDVector3D top) {
	EDVector3D left, right, bottom;
	left.edvClone(top);
	right.edvClone(top);
	bottom.edvClone(top);
	left.x -= 10.0f;
	left.y += 10.0f;
	right.x += 10.0f;
	right.y += 10.0f;
	bottom.y += 20.0f;
	edAddVertex(left);
	edAddVertex(top);
	edAddVertex(right);
	edAddVertex(top);
	edAddVertex(bottom);
	edSetMode(GL_LINE_STRIP);
	return;
}