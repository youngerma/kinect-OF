#include "ofApp.h"
int pp=0;

//--------------------------------------------------------------
void ofApp::setup() {
	//windows环境需要安装解码mov格式的解码器插件(K-Lite_Codec_Pack)
	video.loadMovie("handsTrees.mov");	//Load the video file
	video.play();						//Start the video to play
}

//--------------------------------------------------------------
void ofApp::update() {
	video.update();		//Decode the new frame if needed
	//Do computing only if the new frame was obtained
	if (video.isFrameNew()) {
		//bAllocated分配的,该分支判断grayImage所指向地址是否被分配,可理解为grayImage是否有值
		//Store the previous frame, if it exists till now
		if (grayImage.bAllocated) {
			grayImagePrev = grayImage;
		}

		//获取最新的一帧，存入image(ofxCvColorImage),和grayImage(ofxCvGrayscaleImage)
		//Getting a new frame
		image.setFromPixels(video.getPixelsRef());
		grayImage = image;	//Convert to grayscale image
		//grayImage的操作仅此一行，即可实现灰度处理(左上)

		//bAllocated分配的,该分支判断grayImage所指向地址是否被分配，可理解为grayImagePrev是否有值
		//Do processing if grayImagePrev is inited
		if (grayImagePrev.bAllocated) {
			//absDiff函数获取绝对差异
			//通过像素对比，获取运动部分，差异通过灰度的值来体现
			//Get absolute difference
			diff.absDiff(grayImage, grayImagePrev);

			//扩大差异，通过乘法实现，即扩大像素点之间的灰度值差。
			//We want to amplify the difference to obtain
			//better visibility of motion
			//We do it by multiplication. But to do it, we
			//need to convert diff to float image first
			diffFloat = diff;	//Convert to float image
			diffFloat *= 5.0;	//Amplify the pixel values

			//更新缓存
			//Update the accumulation buffer
			if (!bufferFloat.bAllocated) {
				//If the buffer is not initialized, then
				//just set it equal to diffFloat
				//首次赋值，则直接取diffFloat
				bufferFloat = diffFloat;
			}
			else {
				//Slow damping the buffer to zero
				//控制其所有像素点的灰度值，在累加后，不至于太亮，或太暗
				bufferFloat *= 0.6;
				//Add current difference image to the buffer
				bufferFloat += diffFloat;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(255, 255, 255);	//Set the background colorS

	//以difffloat图像是否准备就绪作为判断依据
	//即视频的第二帧以后,开始进入该分支
	//Draw only if diffFloat image is ready.
	//It happens when the second frame from the video is obtained
	if (diffFloat.bAllocated) {
		//Get image dimensions（尺寸）
		int w = grayImage.width;
		int h = grayImage.height;

		//Set color for images drawing
		ofSetColor(255, 255, 255);

		//[1,2]
		//[3,4]
		//Draw images grayImage,  diffFloat, bufferFloat
		grayImage.draw(0, 0, w / 2, h / 2);//[1]
		diffFloat.draw(w / 2 + 10, 0, w / 2, h / 2);//[2]
		bufferFloat.draw(0, h / 2 + 10, w / 2, h / 2);//[3]

		//Draw the image motion areas

		//坐标系的转换与尺度
		//Shift and scale the coordinate system
		ofPushMatrix();
		ofTranslate(w / 2 + 10, h / 2 + 10);

		//ofScale 产生沿X，Y和Z轴的非均匀缩放。三参数xamnt，yamnt和zamnt显示所需的尺度因子沿三个轴。
		//ofScale(0.5, 0.5);

		//画边框
		//Draw bounding rectangle
		ofSetColor(0, 0, 0);
		ofNoFill();
		ofRect(-1, -1, w + 2, h + 2);

		//Get bufferFloat pixels
		float *pixels = bufferFloat.getPixelsAsFloats();
		//Scan all pixels
		for (int y = 0; y<h; y++) {
			for (int x = 0; x<w; x++) {
				//Get the pixel value
				float value = pixels[x + w * y];
				//如果值超过阈值，则绘制像素
				//因bufferFloat像素为灰度，效果即，描绘bufferFloat的高亮点
				//If value exceed threshold, then draw pixel
				if (value >= 0.9) {
					ofSetColor(192,255,62); //设为绿色
					ofRect(x, y, 1, 1);
					//矩形大小1x1像素
					//Rectangle with size 1x1 means pixel
					//Note, this is slow function, 
					//we use it here just for simplicity
				}
			}
		}
		ofPopMatrix();	//Restore the coordinate system
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
