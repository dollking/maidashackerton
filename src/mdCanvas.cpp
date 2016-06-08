#include "mdCanvas.h"


mdCanvas::mdCanvas()
{
}


mdCanvas::~mdCanvas()
{
}


void mdCanvas::draw()
{
	//frame.draw();

	for (auto group : groupList)
	{
		group->draw();
	}

	if (bDrawSelectedGroup)
	{
		for (auto group : selectedGroupList)
		{
			group->drawOnSelected();
		}
	}

	if (bDrawSelectedVertices)
	{
		ofPushStyle();
		ofSetColor(ofColor::blue);
		vector<ofPoint*>::iterator iter;
		for (iter = selectedVertices.begin(); iter != selectedVertices.end(); ++iter)
			ofDrawCircle(*(*iter), 3);
		ofPopStyle();
	}
}


void mdCanvas::setDrawSelectedGroup(bool bDraw)
{
	bDrawSelectedGroup = bDraw;
}
void mdCanvas::setDrawSelectedVertices(bool bDraw)
{
	bDrawSelectedVertices = bDraw;
}


void mdCanvas::saveCanvas(string path)
{
	ofXml xml;
	xml.addChild("Canvas");
	xml.setToChild(0);
	int groupCount = 0;
	for (auto group:groupList)
	{

		xml.addChild("Group");
		xml.setToChild(groupCount);
		int polygonCount = 0;
		for (auto polygon : group->polygonList)
		{
			xml.addChild("Polygon");
			xml.setToChild(polygonCount);
			xml.addChild("Color");
			xml.setToChild(0);
			xml.addValue("R", (int)polygon->color.r);
			xml.addValue("G", (int)polygon->color.g);
			xml.addValue("B", (int)polygon->color.b);
			xml.addValue("A", (int)polygon->color.a);
			xml.setToParent();
			xml.addChild("Vertices");
			xml.setToChild(1);
			int vertexCount = 0;
			for (auto vertex : polygon->pointList)
			{
				xml.addChild("Vertex");
				xml.setToChild(vertexCount);
				xml.addValue("X", vertex->x);
				xml.addValue("Y", vertex->y);
				xml.setToParent();
				vertexCount++;
			}
			xml.setToParent();
			xml.setToParent();
			polygonCount++;
		}
		xml.setToParent();
		groupCount++;
	}
	xml.save(path);
	
}
void mdCanvas::loadCanvas(string path)
{
	ofXml xml;
	if (xml.load(path))
	{
		xml.setTo("Canvas");
		int numGroup = xml.getNumChildren();
		xml.setToChild(0);	//group
		for (int indexGroup = 0; indexGroup < numGroup; indexGroup++)
		{
			int numPolygon = xml.getNumChildren();
			if (numPolygon > 0)
			{ 
				xml.setToChild(0);	//polygon
				for (int indexPoly = 0; indexPoly < numPolygon; indexPoly++)
				{
					xml.setToChild(0);//color
					ofColor polygonColor;
					polygonColor.r = ofToInt(xml.getValue("R"));
					polygonColor.g = ofToInt(xml.getValue("G"));
					polygonColor.b = ofToInt(xml.getValue("B"));
					polygonColor.a = ofToInt(xml.getValue("A"));
					xml.setToSibling();//vertices
					vector<ofPoint> points;
					int numVertex = xml.getNumChildren();
					xml.setToChild(0);//vertex
					if (numVertex > 0)
					{ 
						for (int indexVertex = 0; indexVertex < numVertex; indexVertex++)
						{
							points.push_back(ofPoint(ofToFloat(xml.getValue("X")), ofToFloat(xml.getValue("Y"))));
							if (!xml.setToSibling())
							{
								break;
							}
						}
						addPolygon(mdPolygon(polygonColor, points));
						xml.setToSibling();//next vertex
					}
					xml.setToParent();//vertices
					xml.setToParent();//polygon
					if (!xml.setToSibling())
					{
						xml.setToParent();
						break;
					}
				}
			}
			if (!xml.setToSibling())
			{
				break;
			}
		}
	}
	else
	{
		ofSystemAlertDialog("Loading failed");
	}

}
void mdCanvas::newCanvas()
{

}

void mdCanvas::addPolygon(mdPolygon polygon)
{
	mdPolygonGroup* group = new mdPolygonGroup();
	mdPolygon* newPolygon = new mdPolygon();
	
	*newPolygon = polygon;

	group->polygonList.push_back(newPolygon);

	groupList.push_back(group);
}


bool mdCanvas::selection(ofRectangle rect)
{
	bool ret = false;

	selectedGroupList.clear();

	for (auto group : groupList)
	{
		if (group->isInside(rect))
		{
			selectedGroupList.push_back(group);
			ret = true;
		}
	}
	return ret;
}

bool mdCanvas::selection(ofPoint point)
{
	bool ret = false;

	selectedGroupList.clear();

	for (auto group : groupList)
	{
		if (group->isInside(point))
		{
			ret = true;
			selectedGroupList.clear();
			selectedGroupList.push_back(group);
		}
	}

	return ret;
}


bool mdCanvas::selectionVertex(ofRectangle rect)
{
	bool ret = false;

	selectedVertices.clear();

	for (auto group : selectedGroupList)
	{
		for (auto polygon : group->polygonList)
		{
			for (auto point : polygon->pointList)
			{
				if (rect.inside(*point))
				{
					ret = true;
					selectedVertices.push_back(point);
				}
			}
		}
	}

	return ret;
}

void mdCanvas::translate(ofPoint offset)
{
	for (auto group : groupList)
	{
		group->translate(offset);
	}
}

void mdCanvas::translateSelectedVertex(ofPoint offset)
{
	for (auto point : selectedVertices)
	{
		point->x += offset.x;
		point->y += offset.y;
	}
}

void mdCanvas::translateSelectedGroup(ofPoint offset)
{
	for (auto group : selectedGroupList)
	{
		group->translate(offset);
	}
}


void mdCanvas::setTranMatrixSelectedGroup(ofPoint offset)
{
	for (auto group : selectedGroupList)
	{
		for (auto polygon : group->polygonList)
		{
			polygon->tran = offset;
		}
	}
}


void mdCanvas::setRotateMatrixSelectedGroup(float degree)
{
	for (auto group : selectedGroupList)
	{
		for (auto polygon : group->polygonList)
		{
			polygon->rotateDegree = degree;
		}
	}
}

void mdCanvas::scaleSelectedGroup(float scale)
{
	int minX = 1600;
	int minY = 870;
	for (auto group : selectedGroupList)
	{
		for (auto polygon:group->polygonList)
		{
			minX = MIN(minX, polygon->getMinPoint().x);
			minY = MIN(minY, polygon->getMinPoint().y);
		}
	}
	for (auto group : selectedGroupList)
	{
		for (auto polygon : group->polygonList)
		{
			for (auto pt : polygon->pointList)
			{
				*pt = (*pt - ofPoint(minX, minY)) * scale + ofPoint(minX, minY);
			}
		}
	}
}
void mdCanvas::setRotateMatrixSelectedGroup(float degree, ofPoint center)
{
	for (auto group : selectedGroupList)
	{
		for (auto polygon : group->polygonList)
		{
			polygon->rotateDegree = degree;
			polygon->rotateCenter = center;
		}
	}
}

void mdCanvas::rotate(float angle)
{
	ofVec3f anchor(0, 0, 1);

	for (auto group : selectedGroupList)
	{
		group->rotate(angle, anchor);
	}
}

void mdCanvas::rotate(float angle, ofPoint anchor)
{
	for (auto group : selectedGroupList)
	{
		ofVec3f anchor(anchor.x, anchor.y, anchor.z);
		group->rotate(angle, anchor);
	}
}

void mdCanvas::copy(ofPoint offset)
{
	mdPolygonGroup* newGroup;

	for (auto group : selectedGroupList)
	{
		newGroup = new mdPolygonGroup();

		for (auto polygon : group->polygonList)
		{
			mdPolygon *newPoly = new mdPolygon();

			for (auto point : polygon->pointList)
			{
				ofPoint* newPoint = new  ofPoint();
				*newPoint = *point;

				newPoly->pointList.push_back(newPoint);
			}
			newPoly->color = polygon->color;

			newGroup->polygonList.push_back(newPoly);
		}

		newGroup->translate(offset);
		groupList.push_back(newGroup);
	}

	selectedGroupList.clear();
	selectedGroupList.push_back(newGroup);

}

// dir 0:X 1:Y
void mdCanvas::align(ofPoint pivotPoint, float degree, bool direction)
{
	for (auto polygonGroup : selectedGroupList)
		polygonGroup->align(pivotPoint, degree, direction, frame.getMaxPoint());
}

// dir 0:left 1:right 2:top 3:bottom
ofPoint mdCanvas::getAlignPosition(ofPoint start, float slope, int direction, ofPoint target)
{
	ofPoint alignPosition(target);

	switch (direction)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}

	return alignPosition;
}

void mdCanvas::group()
{
	mdPolygonGroup* newGroup = new mdPolygonGroup();

	for (auto group : selectedGroupList)
	{
		for (auto polygon : group->polygonList)
		{
			newGroup->polygonList.push_back(polygon);
		}
	}

	selectedGroupList.clear();
	selectedGroupList.push_back(newGroup);
}
