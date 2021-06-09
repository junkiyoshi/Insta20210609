#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);

	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);

	this->cap.open("D:\\MP4\\production ID_3761468.mp4");
	this->cap_size = cv::Size(96, 54);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->number_of_frames = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			continue;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}

	this->mesh.addVertex(glm::vec3(this->cap_size.width * -0.5, this->cap_size.height * -0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * 0.5, this->cap_size.height * -0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * 0.5, this->cap_size.height * 0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * -0.5, this->cap_size.height * 0.5, 0));

	this->mesh.addTexCoord(glm::vec3(0, 0, 0));
	this->mesh.addTexCoord(glm::vec3(this->cap_size.width, 0, 0));
	this->mesh.addTexCoord(glm::vec3(this->cap_size.width, this->cap_size.height, 0));
	this->mesh.addTexCoord(glm::vec3(0, this->cap_size.height, 0));

	this->mesh.addIndex(0); mesh.addIndex(1); mesh.addIndex(2);
	this->mesh.addIndex(0); mesh.addIndex(2); mesh.addIndex(3);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < 13; i++) {

		int y = i * 54 + 27 + 9;

		float noise_value = ofNoise(y * 0.0015 + ofGetFrameNum() * 0.015);

		int frame_count = ofMap(noise_value, 0, 1, -300, this->number_of_frames + 300);

		if (frame_count < 0) { frame_count = 0; }
		if (frame_count > this->number_of_frames) { frame_count = this->number_of_frames - 1; }

		int x = ofMap(frame_count, 0, this->number_of_frames, 100, ofGetWidth() - 100);


		y = 720 - y;

		ofPushMatrix();
		ofTranslate(x, y);

		int seq_frame_index = frame_count % this->number_of_frames;
		this->frame_list[seq_frame_index].copyTo(this->frame);
		this->image.update();

		ofSetColor(255);
		ofFill();
		this->image.bind();
		this->mesh.draw();
		this->image.unbind();

		ofNoFill();
		ofSetColor(39);
		ofDrawRectangle(0, 0, 96, 54);

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}