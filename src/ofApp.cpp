#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	font = new ofTrueTypeFont();
	font->load("NanumBarunGothicBold.ttf", 80, true, true, true);
	ofBackground(ofColor::white);
	ofSetFrameRate(60);
	ofSetCircleResolution(50);
	ofSetWindowPosition(0, 0);

	EditGui.setup("Edit");
	EditGui.setPosition(0, 0);
	btnNew.addListener(this, &ofApp::newCanvas);
	btnLoad.addListener(this, &ofApp::loadCanvas);
	btnSave.addListener(this, &ofApp::saveCanvas);
	btnRectangle.addListener(this, &ofApp::setModeRectangle);
	btnEdit.addListener(this, &ofApp::setModeEdit);
	btnCircle.addListener(this, &ofApp::setModeCircle);
	btnPolygon.addListener(this, &ofApp::setModePolygon);
	groupColor.addListener(this, &ofApp::onGroupColorEdited);
	btnChar.addListener(this, &ofApp::setModeChar);
	btnGroup.addListener(this, &ofApp::groupSelected); // make
	btnDeleteSelectedGroup.addListener(this, &ofApp::deleteGroup); // make
	btnAlignX.addListener(this, &ofApp::doAlignX);
	btnAlignY.addListener(this, &ofApp::doAlignY);
	btnDuplicate.addListener(this, &ofApp::duplicate);
	btnScale.addListener(this, &ofApp::scale);
	btnExport.addListener(this, &ofApp::exportToPng);
	EditGui.add(btnNew.setup("New"));
	EditGui.add(btnLoad.setup("Load"));
	EditGui.add(btnSave.setup("Save"));
	EditGui.add(btnExport.setup("Export"));
	EditGui.add(btnEdit.setup("Edit"));
	EditGui.add(btnRectangle.setup("Rectangle"));
	EditGui.add(btnCircle.setup("Circle"));
	EditGui.add(btnPolygon.setup("Polygon"));
	EditGui.add(btnChar.setup("CharInput"));
	EditGui.add(btnGroup.setup("Group")); // make
	EditGui.add(btnDeleteSelectedGroup.setup("delete")); // make
	EditGui.add(btnAlignX.setup("align X"));
	EditGui.add(btnAlignY.setup("align Y"));
	EditGui.add(duplicateOffset.set("DuplicateOffset", ofVec2f(100, 100), ofVec2f(-200, -200), ofVec2f(200, 200)));
	EditGui.add(btnDuplicate.setup("Duplicate"));
	EditGui.add(scaleFactor.set("ScaleFactor", 1, 0.5, 1.5));
	EditGui.add(btnScale.setup("Scale"));
	VertexInfo.setup("Vertex");
	VertexInfo.setPosition(ofPoint(1400, 0));
	VertexInfo.add(selectedVertex.set("Vertex", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(1600, 800)));
	selectedVertex.addListener(this, &ofApp::onVertexEdited);
	GroupInfo.setup("Group Info");
	GroupInfo.setPosition(0, 390);
	GroupInfo.add(groupColor.set("Color", ofColor::red));

	string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (unsigned char index = 0; index < chars.length(); index++)
	{
		ofTTFCharacter character = font->getCharacterAsPoints(chars[index]);
		map<int, vector<vector<ofPoint>>> polylinesSorted;
		for (auto polyline : character.getOutline())
		{
			vector<ofPoint> pts;
			for (auto pt : polyline.getVertices())
			{
				pts.push_back(pt + ofPoint(300, 400));
			}
			polylinesSorted[polyline.getVertices().size()].push_back(pts);
		}
				if (chars[index] == 'i')
				{
					printf("hello");
				}
		int isFirst = true;
		for (map<int, vector<vector<ofPoint>>>::reverse_iterator rit = polylinesSorted.rbegin(); rit != polylinesSorted.rend(); ++rit)
		{
			if (isFirst)
			{
				for (auto polygon : rit->second)
				{
					characters[chars[index]].push_back(mdPolygon(ofColor::red, polygon));
				}
				isFirst = false;
			}
			else
			{
				if (string("046890abdegopqABDOPQR").find(chars[index]) != string::npos)
				{
					for (auto polygon : rit->second)
					{
						characters[chars[index]].push_back(mdPolygon(ofColor::white, polygon));
					}
				}
				else
				{
					for (auto polygon : rit->second)
					{
						characters[chars[index]].push_back(mdPolygon(ofColor::red, polygon));
					}
				}
			}
		}
	}

#if 0
	vector<ofTTFCharacter> characters = font->getStringAsPoints("abcdefghijklmnopqrstuvwxyz");
	for (auto character : characters)
	{
		vector<ofPolyline> charElements = character.getOutline();
		map<int, vector<vector<ofPoint>>> polylinesSorted;
		for (auto element : charElements)
		{
			vector<ofPoint> pts;
			for (auto pt : element.getVertices())
			{
				pts.push_back(pt + ofPoint(200, 400));
			}
			polylinesSorted[element.getVertices().size()].push_back(pts);
		}
		int isFirst = true;
		for (map<int, vector<vector<ofPoint>>>::reverse_iterator rit = polylinesSorted.rbegin(); rit != polylinesSorted.rend(); ++rit)
		{
			if (isFirst)
			{
				for (auto polygon : rit->second)
				{
					canvas.addPolygon(mdPolygon(ofColor::red, polygon));
				}
				isFirst = false;
			}
			else
			{
				for (auto polygon : rit->second)
				{
					canvas.addPolygon(mdPolygon(ofColor::white , polygon));
				}
			}
		}
	}
#endif

}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	canvas.draw();
	switch (state)
	{
	case SelectGroupState:
	case SelectVertexState:
	case TransGroupState:
	case RotateGroupState:
		if (NULL != previewPolygon)
		{
			ofPushStyle();
			ofSetColor(ofColor::blue);
			ofNoFill();
			previewPolygon->draw();
			ofPopStyle();
		}

		if (canvas.bDrawSelectedGroup)
		{
			ofPoint centerPoint(0,0);

			for (auto group : canvas.selectedGroupList)
			{
				auto area = group->getGroupArea();
				centerPoint += area.getCenter();
			}

			int cnt = canvas.selectedGroupList.size();

			if (cnt != 0)
			{
				rotatePoint.x = centerPoint.x / cnt;
				rotatePoint.y = centerPoint.y / cnt;
			}
			else
			{
				rotatePoint.x = 0;
				rotatePoint.y = 0;
			}

			ofPushStyle();

			if(bRotatePointNear)
				ofSetColor(ofColor::darkViolet);
			else
				ofSetColor(ofColor::crimson);
		
			ofDrawCircle(rotatePoint, 5); 
			ofPopStyle();
		}


		break;

	case TransVertexState:

		if (bDrawTransVertexState)
		{
			ofPushMatrix();
			ofPushStyle();

			ofTranslate(TransVertexMatirx);

			ofSetColor(ofColor::cyan);
			vector<ofPoint*>::iterator iter;

			for (iter = transSelectedVertex.begin(); iter != transSelectedVertex.end(); ++iter)
				ofDrawCircle(*(*iter), 5);
			ofPopStyle();
			ofPopMatrix();
		}

		break;
	case DrawingRectangleState:
	case DrawingCircleState:
	case DrawingNPolygonState:
		if (NULL != previewPolygon)
		{
			ofPushStyle();
			ofSetColor(ofColor::red);
			ofNoFill();
			previewPolygon->draw();
			ofPopStyle();
		}
		break;

	case AlignXState:
	case AlignYState:

		ofPushStyle();
		ofSetColor(ofColor::darkTurquoise);
		ofDrawLine(alignS, alignE);
		ofPopStyle();
		break;
	default:
		break;
	}
	ofPushStyle();
	ofSetColor(ofColor::black);
	ofDrawBitmapString("X : " + ofToString(ofGetMouseX()) + ", Y : " + ofToString(ofGetMouseY()), ofPoint(ofGetMouseX(), ofGetMouseY()));
	ofPopStyle();
	EditGui.draw();
	VertexInfo.draw();
	GroupInfo.draw();
#ifdef _DEBUG
//	stateMonitor->draw();
#endif // _DEBUG
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (state)
	{
	case DrawingChar:
		if (IsCharAlphaNumeric(key))
		{
			int minX = 1600;
			int maxX = 0;
			for (auto polygon : characters[key])
			{
				minX = MIN(minX, polygon.getMinPoint().x);
				maxX = MAX(maxX, polygon.getMaxPoint().x);
				mdPolygon charPolygon = polygon;
				charPolygon.translate(charInputOffset);
				canvas.addPolygon(charPolygon);
			}
			
			charInputOffset += ofPoint(maxX - minX + 15, 0);
		}
		break;
	default:
	if ('d' == key)
	{
		if (!canvas.selectedGroupList.empty())
		{
			canvas.copy(ofPoint(100, 100));
		}
	}
	else if (key == OF_KEY_DEL)
	{
		deleteGroup();
	}
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	bool bNear = false;
	ofPoint mouse(x, y);

	switch (state)
	{
	case TransVertexState:


		for (auto point : canvas.selectedVertices)
		{
			if (point->distance(mouse) < 10)
			{
				bNear = true;
				break;
			}
		}

		if (bDrawTransVertexState)
		{
			transSelectedVertex.clear();
			for (auto point : canvas.selectedVertices)
			{
				ofPoint* newPoint = new ofPoint();
				*newPoint = *point;
				transSelectedVertex.push_back(newPoint);
			}

			TransVertexMatirx.x = 0;
			TransVertexMatirx.y = 0;

		}

		bDrawTransVertexState = bNear;

		break;

	case SelectVertexState:

		if (rotatePoint.distance(mouse) < 10)
		{
			bRotatePointNear = true;
		}
		else
		{
			bRotatePointNear = false;
		}


		break;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (ofRectangle(EditGui.getPosition(), EditGui.getWidth(), EditGui.getHeight()).inside(ofPoint(x, y)))
	{
		isMousePressedNotInGui = false;
	}
	else
	{
		mousePressedPoint.x = x;
		mousePressedPoint.y = y;

		bool bSel = false;

		switch (state)
		{
		case SelectVertexState:

			if (rotatePoint.distance(mousePressedPoint) < 10)
			{
				bRotatePointNear = true;
				state = RotateGroupState;

				canvas.setRotateMatrixSelectedGroup(0, rotatePoint);
				break;
			}
			else
			{
				bRotatePointNear = false;
			}


			for (auto polyGroup : canvas.selectedGroupList)
			{
				if (polyGroup->isInside(mousePressedPoint))
				{
					TransVertexMatirx.x = 0;
					TransVertexMatirx.y = 0;
					bSel = true;
					state = TransGroupState;

					break;
				}
			}


			break;
		case TransVertexState:

			if (bDrawTransVertexState)
			{
				transSelectedVertex.clear();
				for (auto point : canvas.selectedVertices)
				{
					ofPoint* newPoint = new ofPoint();
					*newPoint = *point;
					transSelectedVertex.push_back(newPoint);
				}
			}
			else
			{
				state = SelectGroupState;
				bDrawTransVertexState = false;
			}

			break;

		case DrawingNPolygonState:

			break;

		case AlignXState:
		case AlignYState:
			alignS = mousePressedPoint;
			alignE = mousePressedPoint;
			break;
		default:
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

	ofPoint anchor(0, 0, 1);
	ofPoint offset;

	switch (state)
	{
	case TransVertexState:
		TransVertexMatirx.x = -(mousePressedPoint.x - x);
		TransVertexMatirx.y = -(mousePressedPoint.y - y);

		//canvas.translateSelectedGroup(offset);
		//canvas.setTranMatrixSelectedGroup(offset);

		break;
	case TransGroupState:

		offset.x = -(mousePressedPoint.x - x);
		offset.y = -(mousePressedPoint.y - y);

		//canvas.translateSelectedGroup(offset);

		canvas.setTranMatrixSelectedGroup(offset);
	

		break;
	case RotateGroupState:

		offset.x = -(mousePressedPoint.x - x);
		offset.y = -(mousePressedPoint.y - y);
		//canvas.rotate(1, anchor);

		canvas.setRotateMatrixSelectedGroup(offset.x);

		break;

	case SelectVertexState:
	case SelectGroupState:
		if (NULL != previewPolygon)
		{
			delete(previewPolygon);
		}
		previewPolygon = new ofPolyline(ofPolyline::fromRectangle(ofRectangle(mousePressedPoint, ofPoint(x, y))));
		previewPolygon->close();
		break;
	case DrawingRectangleState:
		if (NULL != previewPolygon)
		{
			delete(previewPolygon);
		}
		previewPolygon = new ofPolyline(ofPolyline::fromRectangle(ofRectangle(mousePressedPoint, ofPoint(x, y))));
		previewPolygon->close();
		break;
	case DrawingCircleState:
		if (NULL != previewPolygon)
		{
			delete(previewPolygon);
		}
		previewPolygon = new ofPolyline(ofPolyline::fromCircle(mousePressedPoint, mousePressedPoint.distance(ofPoint(x, y)), 40));
		previewPolygon->close();
		break;
	case DrawingNPolygonState:

		break;


	case AlignXState:
	case AlignYState:
		alignE.x = x;
		alignE.y = y;
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	ofPoint anchor;
	ofPoint offset;
	ofPoint diff;
	float degree;

	switch (state)
	{
	case SelectGroupState:
		if (mousePressedPoint == ofPoint(x, y))
		{
			if (canvas.selection(mousePressedPoint))
			{
				canvas.setDrawSelectedGroup(true);
				state = SelectVertexState;
				if (1 == canvas.selectedGroupList.size())
				{
					if (1 == canvas.selectedGroupList.front()->polygonList.size())
					{
						groupColor.set(canvas.selectedGroupList.front()->polygonList.front()->color);
					}
				}

				//Group Select 회전 

			}
		}
		else
		{
			if (true == canvas.selection(ofRectangle(mousePressedPoint, ofPoint(x, y))) && state == SelectGroupState)
			{
				canvas.setDrawSelectedGroup(true);
				state = SelectVertexState;
				canvas.selectedVertices.clear();				
				if (1 == canvas.selectedGroupList.size())
				{
					if (1 == canvas.selectedGroupList.front()->polygonList.size())
					{
						groupColor.set(canvas.selectedGroupList.front()->polygonList.front()->color);
					}

					//Group Select 회전


				}
			}
			else
			{
				if (canvas.selectionVertex(ofRectangle(mousePressedPoint, ofPoint(x, y))))
				{
					if (canvas.selectedVertices.size() == 1)
					{
						selectedVertex.set((*canvas.selectedVertices.front()));
					}
				}
			}
		}
		break;

	case SelectVertexState:
		canvas.setDrawSelectedGroup(false);
		if (true != canvas.selectionVertex(ofRectangle(mousePressedPoint, ofPoint(x, y))))
		{
			state = SelectGroupState;
		}
		else
		{

			state = TransVertexState;

			if (canvas.selectedVertices.size() == 1)
			{
				selectedVertex.set((*canvas.selectedVertices.front()));
			}
		}
		break;

	case TransGroupState:

		offset.x = -(mousePressedPoint.x - x);
		offset.y = -(mousePressedPoint.y - y);

		canvas.translateSelectedGroup(offset);
		canvas.setTranMatrixSelectedGroup(ofPoint(0,0));

		state = SelectVertexState;

		break;
	case RotateGroupState:


		offset.x = -(mousePressedPoint.x - x);
		offset.y = -(mousePressedPoint.y - y);

		anchor.x = rotatePoint.x;
		anchor.y = rotatePoint.y;


		canvas.rotate(offset.x, anchor);
		canvas.setRotateMatrixSelectedGroup(0);


		state = SelectVertexState;
		break;
	case TransVertexState:

		if (bDrawTransVertexState)
		{
			bDrawTransVertexState = false;

			offset.x = -(mousePressedPoint.x - x);
			offset.y = -(mousePressedPoint.y - y);

			for (auto point : canvas.selectedVertices)
			{
				point->x += offset.x;
				point->y += offset.y;
			}
		}

		break;

	case DrawingRectangleState:
		canvas.addPolygon(mdPolygon(groupColor.get(), ofPolyline::fromRectangle(ofRectangle(mousePressedPoint, ofPoint(x, y))).getVertices()));
		break;
	case DrawingCircleState:
		canvas.addPolygon(mdPolygon(groupColor.get(), ofPolyline::fromCircle(mousePressedPoint, mousePressedPoint.distance(ofPoint(x, y)), 40).getVertices()));
		break;
	case DrawingNPolygonState:
		if (previewPolygon->getVertices().size() == 0)
		{
			previewPolygon->addVertex(ofPoint(x, y));
		}
		else if (previewPolygon->size() > 1)
		{
			if (previewPolygon->getVertices().front().distance(ofPoint(x, y)) < 5)
			{
				canvas.addPolygon(mdPolygon(groupColor.get(), previewPolygon->getVertices()));
				if (NULL != previewPolygon) {
					delete(previewPolygon);
					previewPolygon = NULL;
				}
			}
			else
			{
				previewPolygon->addVertex(ofPoint(x, y));
			}
		}
		else
		{
			previewPolygon->addVertex(ofPoint(x, y));
		}
		break;

	case AlignXState:
		diff = alignS - alignE;
		degree = 0;

		if (diff.x != 0)
		{
			degree = diff.y / diff.x;
		}

		canvas.align(alignS, degree, false);
		state = SelectGroupState;
		break;

	case AlignYState:
		diff = alignS - alignE;
		degree = 0;

		if (diff.x != 0)
		{
			degree = diff.y / diff.x;
		}
		canvas.align(alignS, degree, true);
		state = SelectGroupState;
		break;


	default:
		break;
	}
	if (state != DrawingNPolygonState)
	{
		if (NULL != previewPolygon)
		{
			delete(previewPolygon);
			previewPolygon = NULL;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::newCanvas()
{
	canvas = mdCanvas();
}

void ofApp::loadCanvas()
{
	ofFileDialogResult result = ofSystemLoadDialog("Load File");
	if (result.bSuccess)
	{
		canvas = mdCanvas();
		canvas.loadCanvas(result.getPath());
	}
	else
	{
		ofLog(OF_LOG_WARNING, "Load Failed");
	}
}

void ofApp::saveCanvas()
{
	ofFileDialogResult result = ofSystemSaveDialog("save.xml", "Save File");
	if (result.bSuccess)
	{
		canvas.saveCanvas(result.getPath());
	}
	else
	{
		ofLog(OF_LOG_WARNING, "Save Failed");
	}
}


void ofApp::setModeRectangle()
{
	state = DrawingRectangleState;
}

void ofApp::setModeEdit()
{
	state = SelectGroupState;
}

void ofApp::setModeCircle() {
	state = DrawingCircleState;
}

void ofApp::setModePolygon() {
	state = DrawingNPolygonState;
	if (NULL != previewPolygon)
	{
		previewPolygon->clear();
	}
	else
	{
		previewPolygon = new ofPolyline();
	}
}

void ofApp::groupSelected() // make
{
	if (canvas.selectedGroupList.size() > 1)
		canvas.group();
}

void ofApp::deleteGroup() // make
{
	for (auto group : canvas.selectedGroupList)
	{
		vector<mdPolygonGroup*>::iterator iter = canvas.groupList.begin();

		for (iter; iter != canvas.groupList.end(); ++iter)
		{
			if (*iter == group)
			{
				canvas.groupList.erase(iter);
				break;
			}
		}
	}

	canvas.selectedGroupList.clear();
}

void ofApp::vertexTranslateSelected()
{

}


void ofApp::polygonTranslateSelected()
{

}
void ofApp::polygonRotateSelected()
{

}
void ofApp::polygonScaleSelected()
{

}

void ofApp::onVertexEdited(ofVec2f &pt)
{
	if (canvas.selectedVertices.size() == 1)
	{
		(*canvas.selectedVertices.front()) = pt;
	}
}


void ofApp::onGroupColorEdited(ofColor&color)
{
	if (0 < canvas.selectedGroupList.size())
	{
		for (auto group : canvas.selectedGroupList)
		{
			for (auto polygon : group->polygonList)
			{
				polygon->color = color;
			}
		}
	}
}

void ofApp::setModeChar()
{
	state = DrawingChar;
	charInputOffset = ofPoint(0, 0);
}
void ofApp::doAlignX()
{
	state = AlignXState;
}

void ofApp::doAlignY()
{
	state = AlignYState;
}

void ofApp::duplicate()
{
	canvas.copy((ofPoint)duplicateOffset);
}

void ofApp::scale()
{
	canvas.scaleSelectedGroup(scaleFactor.get());
}

void ofApp::exportToPng()
{
	ofFileDialogResult result = ofSystemSaveDialog("poster.png", "exportTo PNG File");
	if (result.bSuccess)
	{
		ofImage image2export;
		image2export.grabScreen(200, 0, 1200, 870);
		image2export.save(result.getPath());
	}
	else
	{
		ofLog(OF_LOG_WARNING, "Export Failed");
	}
}

