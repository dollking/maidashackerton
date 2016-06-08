#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "mdCanvas.h"
#include "ofTrueTypeFont.h"

#ifdef _DEBUG
#include "ofxOscilloscope.h"
#endif // DEBUG

typedef enum
{
	SelectGroupState=0,
	SelectVertexState,
	TransGroupState,
	RotateGroupState,
	TransVertexState,
	DrawingRectangleState, 
	DrawingCircleState, 
	DrawingNPolygonState,
	DrawingChar,
	AlignXState,
	AlignYState
} PosterDesignerState;


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void newCanvas();
		void loadCanvas();
		void saveCanvas();
		void setModeRectangle();
		void setModeEdit();
		void setModeCircle();
		void setModePolygon();
		void doAlignX();
		void doAlignY();
		void deleteGroup(); // make

		mdCanvas canvas;
		ofxButton btnNew;
		ofxButton btnLoad;
		ofxButton btnSave;
		ofxButton btnEdit;
		ofxButton btnRectangle;
		ofxButton btnCircle;
		ofxButton btnPolygon;
		ofxButton btnChar;
		ofxButton btnDeleteSelectedGroup; // make
		ofxButton btnAlignX;
		ofxButton btnAlignY;
		ofxPanel EditGui;

		ofxPanel VertexInfo;
		ofxPanel GroupInfo;


		ofPoint alignS;
		ofPoint alignE;


		ofPoint rotatePoint;
		bool bRotatePointNear = false;

		ofPoint TransVertexMatirx;
		bool bDrawTransVertexState = false;
		vector<ofPoint*> transSelectedVertex;

		ofPolyline * previewPolygon = NULL;
		PosterDesignerState state = SelectGroupState;
		bool isMousePressedNotInGui = false;
		ofPoint mousePressedPoint;
		
#pragma region Chulseung
		ofParameter<ofVec2f> selectedVertex;
		ofxButton btnGroup;
		ofxButton btnDuplicate;
		ofParameter<ofColor> groupColor;
		void onGroupColorEdited(ofColor & color);
		bool isPolygonSelected = false;

		void groupSelected();
		void onVertexEdited(ofVec2f & pt);
		void vertexTranslateSelected();
		ofParameter<ofVec2f> duplicateOffset;
		void duplicate();
		ofParameter<string> textInput;
		ofPoint charInputOffset;
		ofTrueTypeFont* font;
		ofParameter<float> scaleFactor;
		void scale();
		ofxButton btnScale;
		map<unsigned char, vector<mdPolygon>> characters;
		void setModeChar();
		ofxButton btnExport;
		void exportToPng();

#pragma endregion

#pragma region Jungmin

		void polygonTranslateSelected();
		void polygonRotateSelected();
		void polygonScaleSelected();

#pragma endregion

};
