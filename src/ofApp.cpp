#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	int num = 1500;
	p.assign(num, Particle());
	currentMode = PARTICLE_MODE_ATTRACT;

	//initialize replay variables
	replayTimer = 0;
	replayCounter = 0;

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 

	resetParticles();
}

//--------------------------------------------------------------
void ofApp::resetParticles(){

	//these are the attraction points used in the fourth demo 
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( glm::vec3( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) , 0) );
	}
	
	attractPointsWithMovement = attractPoints;
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}	
}

//--------------------------------------------------------------
void ofApp::update(){

	//if its replaying, resets counter and executes the first command
	if (isReplaying && replayTimer % 45 == 0)
	{
		replayTimer = 0;

		if (commands.size() > replayCounter)
		{
			char c = char(commands[replayCounter]);
			keyPressed(c);
			replayCounter++;
		}
		else
		{
			isReplaying = false;
			replayCounter = 0;
		}
	}
	replayTimer++;

	//if its not paused, run the program
	if (!isPaused)
	{
		for (unsigned int i = 0; i < p.size(); i++)
		{
			p[i].setMode(currentMode);
			p[i].update();
		}

		//lets add a bit of movement to the attract points
		for (unsigned int i = 0; i < attractPointsWithMovement.size(); i++)
		{
			attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
			attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackgroundGradient(ofColor(0, 0, 200), ofColor(10, 10, 90));

	for (unsigned int i = 0; i < p.size(); i++)
	{
		p[i].draw();
	}

	ofSetColor(190);
	if (currentMode == PARTICLE_MODE_NEAREST_POINTS)
	{
		for (unsigned int i = 0; i < attractPoints.size(); i++)
		{
			ofNoFill();
			ofDrawCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofDrawCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(230);
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode. \n\nPress 'g' to change shape!", 10, 20);

	//indicators for when program is recording and replaying
	ofSetColor(230, 0, 0);
	if(isRecording){
		ofDrawCircle(ofGetWidth()*.89, ofGetHeight()*.094, 4);
		ofDrawBitmapString("RECORDING", ofGetWidth()*.9, ofGetHeight()*.1);
	}
	else if(isReplaying){
		ofSetColor(0, 230, 0);
		ofDrawCircle(ofGetWidth()*.89, ofGetHeight()*.094, 4);
		ofDrawBitmapString("REPLAYING", ofGetWidth()*.9, ofGetHeight()*.1);
	}
}

//--------------------------------------------------------------

// checks if pause is true or not
void ofApp::pause(){
	if(!isPaused){
		isPaused = true;
	}
	else{
		isPaused = false;
	}
}

//doubles the size of all the particles
void ofApp::enlarge(){
	for(int i = 0; i < p.size(); i++){
		p[i].enlarge();
	}
}

//halves the size of all the particles
void ofApp::shrink(){
	for(int i = 0; i < p.size(); i++){
		p[i].shrink();
	}
}

//doubles the velocity of all particles
void ofApp::faster(){
	for(int i = 0; i < p.size(); i++){
		p[i].faster();
	}
}

//halves the velocity of all particles
void ofApp::slower(){
	for(int i = 0; i < p.size(); i++){
		p[i].slower();
	}
}

void ofApp::record(){
	if (!isRecording){
		commands.clear();
		isRecording = true;
	}
	else{
		isRecording = false;
	}
}

void ofApp::replay(){
	if (isRecording == false){
		isReplaying = true;
	}
}

void ofApp::cancel(){
	isReplaying = false;
	replayCounter = 0;
}

void ofApp::keyPressed(int key){

	if( key == '1'){
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 		
	}
	if( key == '2'){
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse"; 				
	}
	if( key == '3'){
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force"; 						
	}
	if( key == '4'){
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation"; 						
		resetParticles();
	}	
		
	if( key == ' ' ){
		resetParticles();
	}

	//if 'a' is pressed, the particles will pause
	if( key == 'a' || key == 'A'){
		pause();
	}

	//if lowercase or uppercase 'd' is pressed, particles will enlarge
	if( key == 'd' || key == 'D'){
		enlarge();
	}

	//if lowercase or uppercase 'm' is pressed, particles will shrink in size
	if( key == 'm' || key == 'M'){
		shrink();
	}

	//if lowercase or uppercase 't' is pressed, particles will move faster
	if( key == 't' || key == 'T'){
		faster();
	}
	
	//if lowercase or uppercase 'd' is pressed, particles will move slower
	if( key == 's' || key == 'S'){
		slower();
	}

	//if lowercase or uppercase 'r' is pressed, key pressed sequence is recorded
	if( (key == 'r' || key == 'R') && !isReplaying){
		record();
	}

	//if lowercase or uppercase 'p' is pressed, key pressed sequence will playback
	if( key == 'p' || key == 'P'){
		replay();
	}

	//if lowercase or uppercase 'c' is pressed, cancels the replay
	if( key == 'c' || key == 'C'){
		cancel();
	}

	//if lowercase or uppercase 'g' is pressed, changes shape 
	if( key == 'g' || key == 'G'){
		for(int i =0; i < p.size(); i++){

			if(p[i].circle){
				p[i].circle = false;
				p[i].triangle = true;
			}
			else if(p[i].triangle){
				p[i].triangle = false;
				p[i].square = true;
			}
			else{
				p[i].square = false;
				p[i].circle = true;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
	//adds values to vector commands
	if(isRecording == true){
		commands.push_back(char(key));
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
