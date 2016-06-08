#pragma once
#include "ofMain.h"




class mdPolygon
{
public:
	mdPolygon();
	mdPolygon(ofColor setColor, vector<ofPoint> vList);
	mdPolygon(const mdPolygon& cpPolygon);
	~mdPolygon();

	void draw();
	void drawOnSelected();
	void rotate(float degree, ofPoint anchor);
	void translate(ofPoint moveSize);
	ofPoint getAlignPosition(int dir);
	bool isInside(ofRectangle rect);
	bool isInside(ofPoint selectPoint);
	void setColor(ofColor changeColor);
	void changeIsFill();
	ofPoint getMaxPoint();
	ofPoint getMinPoint();
	
	vector<ofPoint*> pointList;
	ofColor color;
	bool isFill = true;

	ofPoint tran;

	float rotateDegree = 0;
	ofPoint rotateCenter;

};

