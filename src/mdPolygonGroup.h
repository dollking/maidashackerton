#pragma once
#include "mdPolygon.h"

class mdPolygonGroup
{
public:
	mdPolygonGroup();
	mdPolygonGroup(const mdPolygonGroup& group);
	~mdPolygonGroup();

	list<mdPolygon*> polygonList;

	ofPoint getAlignPostion(int direction);

	void draw();
	void drawOnSelected();
	bool isInside(ofRectangle rect);
	bool isInside(ofPoint rect);

	void translate(ofPoint offset);
	ofRectangle getGroupArea();

	void align(ofPoint pivotPoint, float degree, bool direction, ofPoint limitPoint);

	void rotate(float angle);
	void rotate(float angle, ofPoint anchor);


};

