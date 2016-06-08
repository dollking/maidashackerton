#include "mdPolygon.h"



mdPolygon::mdPolygon()
{
}

mdPolygon::mdPolygon(ofColor setColor, vector<ofPoint> vList)
{
	color = setColor;

	for (int i = 0; i < vList.size(); ++i)
	{
		ofPoint* temp = new ofPoint();
		*temp = vList.at(i);
		pointList.push_back(temp);
	}

	tran.x = 0;
	tran.y = 0;
}
mdPolygon::mdPolygon(const mdPolygon& cpPolygon)
{
	color = cpPolygon.color;

	int count = cpPolygon.pointList.size();

	for (int i = 0; i < count; i++)
	{
		ofPoint* temp = new ofPoint();
		*temp = *(cpPolygon.pointList.at(i));
		pointList.push_back(temp);
	}
}

mdPolygon::~mdPolygon()
{
}


void mdPolygon::draw()
{
	ofPolyline tempPolygon;
	
	for (auto point : pointList)
		tempPolygon.addVertex(*point);
	
	ofPushStyle();
	ofSetColor(color);

	if (isFill)
		ofFill();
	
	else
		ofNoFill();

	ofSetPolyMode(OF_POLY_WINDING_ODD);    // this is the normal mode
				
	ofBeginShape();

	for (auto vertex : pointList)
	{
		ofVertex(*vertex);
	}

	ofEndShape();

	tempPolygon.close();
	tempPolygon.draw();

	ofPopStyle();
}


void mdPolygon::drawOnSelected()
{
	ofPushMatrix();

	ofTranslate(tran);


	ofPushStyle();
	ofSetColor(ofColor::blue);
	vector<ofPoint*>::iterator iter;

	for (iter = pointList.begin(); iter != pointList.end(); ++iter)
		ofDrawCircle(*(*iter), 3);
	ofPopStyle();

	ofPolyline tempPolygon;

	for (auto point : pointList)
		tempPolygon.addVertex(*point);

	ofPushStyle();
	ofSetColor(ofColor::blue);
	ofSetLineWidth(1);
	tempPolygon.close();
	tempPolygon.draw();
	ofPopStyle();

	ofPopMatrix();

}

bool mdPolygon::isInside(ofRectangle rect)
{
	vector<ofPoint*>::iterator iter;

	for (iter = pointList.begin(); iter != pointList.end(); ++iter)
		if (!rect.inside(*(*iter)))
			return false;

	return true;
}

bool mdPolygon::isInside(ofPoint selectPoint)
{
	ofPolyline tempPolygon;

	for (auto point : pointList)
		tempPolygon.addVertex(*point);
	
	return tempPolygon.inside(selectPoint);
}

void mdPolygon::translate(ofPoint movePoint)
{
	vector<ofPoint*>::iterator iter;

	for (iter = pointList.begin(); iter != pointList.end(); ++iter)
	{
		(*(*iter)).x += movePoint.x;
		(*(*iter)).y += movePoint.y;
	}
}

void mdPolygon::rotate(float degree, ofPoint anchor)
{
	vector<ofPoint*>::iterator iter;

	ofVec3f axis(0, 0, 1);

	rotateCenter = anchor;

	for (iter = pointList.begin(); iter != pointList.end(); ++iter)
	{
		(*(*iter)).x -= anchor.x;
		(*(*iter)).y -= anchor.y;

		(*(*iter)).rotate(degree, axis);

		(*(*iter)).x += anchor.x;
		(*(*iter)).y += anchor.y;

	}
}

ofPoint mdPolygon::getAlignPosition(int dir)
{
	vector<ofPoint*>::iterator iter = pointList.begin();

	ofPoint tempPoint = *(*iter);
	
	for (++iter; iter != pointList.end(); ++iter)
	{
		switch (dir)
		{
		case 0:
			if (tempPoint.x > (*(*iter)).x)
				tempPoint = *(*iter);
			break;
			
		case 1:
			if (tempPoint.x <= (*(*iter)).x)
				tempPoint = *(*iter);
			break;

		case 2:
			if (tempPoint.y > (*(*iter)).y)
				tempPoint = *(*iter);
			break;

		case 3:
			if (tempPoint.y <= (*(*iter)).y)
				tempPoint = *(*iter);
			break;
		}
	}

	return tempPoint;
}

void mdPolygon::setColor(ofColor changeColor)
{
	color = changeColor;
}

void mdPolygon::changeIsFill()
{
	isFill = !(isFill);
}

ofPoint mdPolygon::getMaxPoint()
{
	float maxX = -10.0;
	float maxY = -10.0;

	for (auto point : pointList)
	{
		if (point->x > maxX)
			maxX = point->x;

		if (point->y > maxY)
			maxY = point->y;
	}

	ofPoint temp(maxX, maxY);

	return temp;
}

ofPoint mdPolygon::getMinPoint()
{
	float minX = 100000.0;
	float minY = 100000.0;

	for (auto point : pointList)
	{
		if (point->x < minX)
			minX = point->x;

		if (point->y < minY)
			minY = point->y;
	}

	ofPoint temp(minX, minY);

	return temp;
}