#include "mdPolygonGroup.h"



mdPolygonGroup::mdPolygonGroup()
{
}

mdPolygonGroup::mdPolygonGroup(const mdPolygonGroup& group)
{
	for (auto polygon : group.polygonList)
	{
		mdPolygon *newPoly = new mdPolygon();
		*newPoly = *polygon;

		polygonList.push_back(newPoly);
	}
}

mdPolygonGroup::~mdPolygonGroup()
{
}

// dir 0:left 1:right 2:top 3:bottom
ofPoint mdPolygonGroup::getAlignPostion(int direction)
{
	ofPoint alignPoint(0,0);

	for (auto polygon : polygonList)
	{
		auto var = polygon->getAlignPosition(direction);

		switch (direction)
		{
		case 0:
			if (alignPoint.x > var.x)
				alignPoint = var;
			break;
		case 1:
			if (alignPoint.x < var.x)
				alignPoint = var;
			break;
		case 2:
			if (alignPoint.y < var.y)
				alignPoint = var;
			break;
		case 3:
			if (alignPoint.y > var.y)
				alignPoint = var;
			break;
		default:
			break;
		}
	}
	return alignPoint;
}


void mdPolygonGroup::draw()
{
	for (auto polygon : polygonList)
	{
		polygon->draw();
	}
}

void mdPolygonGroup::drawOnSelected()
{
	for (auto polygon : polygonList)
	{
		polygon->drawOnSelected();
	}
}

bool mdPolygonGroup::isInside(ofRectangle rect)
{
	for (auto polygon : polygonList)
	{
		if (!polygon->isInside(rect))
		{
			return false;
		}
	}
	return true;
}


bool mdPolygonGroup::isInside(ofPoint point)
{
	for (auto polygon : polygonList)
	{
		if (!polygon->isInside(point))
		{
			return false;
		}
	}
	return true;
}


void mdPolygonGroup::translate(ofPoint offset)
{
	for (auto polygon : polygonList)
	{
		polygon->translate(offset);
	}
}

void mdPolygonGroup::rotate(float angle, ofPoint anchor)
{
	for (auto polygon : polygonList)
	{
		polygon->rotate(angle, anchor);
	}
}

void mdPolygonGroup::rotate(float angle)
{
	vector<ofPoint> tempVector;

	for (auto polygon : polygonList)
		for (auto vertex : polygon->pointList)
			tempVector.push_back(*vertex);

	ofPolyline tempPolygon(tempVector);

	ofPoint axisPoint = tempPolygon.getCentroid2D();
	ofVec3f axis(axisPoint.x, axisPoint.y);

	for (auto polygon : polygonList)
	{
		polygon->rotate(angle, axis);
	}
}

ofRectangle mdPolygonGroup::getGroupArea()
{
	float maxX = -10.0;
	float maxY = -10.0;
	float minX = 10000.0;
	float minY = 10000.0;

	ofPoint temp;

	for (auto polygon : polygonList)
	{
		temp = polygon->getMaxPoint();

		if (temp.x > maxX)
			maxX = temp.x;

		if (temp.y > maxY)
			maxY = temp.y;

		temp = polygon->getMinPoint();

		if (temp.x < minX)
			minX = temp.x;

		if (temp.y < minY)
			minY = temp.y;
	}

	ofRectangle tempRec(ofPoint(minX, minY), ofPoint(maxX, maxY));

	return tempRec;
}


void mdPolygonGroup::align(ofPoint pivotPoint, float degree, bool direction, ofPoint limitPoint)
{
	float alp = pivotPoint.y - (degree * pivotPoint.x);
	float moveSize;

	for (auto polygon : polygonList)
	{
		ofPoint tempPoint;
		tempPoint = polygon->getMinPoint();
		if (direction)	// Y©ö©¡Ca
		{
			moveSize = ((tempPoint.y - alp) / degree) - tempPoint.x;
			//if (moveSize + tempPoint.x < -5)
			//	polygon->translate(ofPoint(-tempPoint.x, 0));
			//else if(moveSize + tempPoint.x > limitPoint.x - 10)
			//	polygon->translate(ofPoint(limitPoint.x - tempPoint.x - 10, 0));
			//else
				polygon->translate(ofPoint(moveSize, 0));
		}

		else
		{
			moveSize = ((degree * tempPoint.x) + alp) - tempPoint.y;
			//if (moveSize + tempPoint.y < -1)
			//	polygon->translate(ofPoint(0, -tempPoint.y));
			//else if(moveSize + tempPoint.y > limitPoint.y - 10)
			//	polygon->translate(ofPoint(0, limitPoint.y - tempPoint.y - 10));
			//else
				polygon->translate(ofPoint(0, moveSize));
		}
	}
}