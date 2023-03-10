#pragma once

#include "ofMain.h"
#include "Particle.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
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
		
		
		particleMode currentMode;
		string currentModeStr; 

		//functions added
		void resetParticles();
		void pause();
		void enlarge();
		void shrink();
		void faster();
		void slower();
		void record();
		void replay();
		void cancel();

		//variables added
		bool isPaused = false;
		bool isRecording = false;
		bool isReplaying = false;
		int replayTimer;
		int replayCounter;

		//character array 
		vector <char> commands;

		vector <Particle> p;
		vector <glm::vec3> attractPoints;
		vector <glm::vec3> attractPointsWithMovement;
		
};
