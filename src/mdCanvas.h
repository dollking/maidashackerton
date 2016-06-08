#pragma once
#include "mdPolygonGroup.h"
#include "mdPolygon.h"


class mdCanvas
{
public:
	mdCanvas();
	~mdCanvas();

	void draw();

	void setDrawSelectedGroup(bool bDraw);
	void setDrawSelectedVertices(bool bDraw);

	void saveCanvas(string path);
	void loadCanvas(string path);
	void newCanvas();
	void addPolygon(mdPolygon polygon);
	bool selection(ofRectangle rect);
	bool selection(ofPoint point);
	bool selectionVertex(ofRectangle rect);

	void translate(ofPoint offset);

	void translateSelectedVertex(ofPoint offset);
	void translateSelectedGroup(ofPoint offset);


	void setTranMatrixSelectedGroup(ofPoint offset);
	void setRotateMatrixSelectedGroup(float degree);
	void setRotateMatrixSelectedGroup(float degree, ofPoint center);
	
	void scaleSelectedGroup(float scale);

	void rotate(float angle);
	void rotate(float angle, ofPoint anchor);
	void copy(ofPoint offset);

	void align(ofPoint pivotPoint, float degree, bool direction);
	void group();
	ofPoint getAlignPosition(ofPoint start, float slope, int direction, ofPoint target);
	
	bool bDrawSelectedGroup = true;
	bool bDrawSelectedVertices = true;



	mdPolygon frame;

	vector<ofPoint*> selectedVertices;
	vector<mdPolygonGroup*> selectedGroupList;
	vector<mdPolygonGroup*> groupList;
	
	
};

