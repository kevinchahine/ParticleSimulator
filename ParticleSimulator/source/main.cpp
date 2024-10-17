#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "globals.h"
#include "factory/factory.h"
#include "ui/display.h"
#include "force_engine.h"
#include "stop_watch.h"

using namespace std;

int main() {
	cv::Size size{ 1000, 600 };
	cv::Size halfSize = size / 2;
	cv::Point2f halfSizePoint{ 500, 300 };

	FactoryOptions ops;
	ops.nParticles(3);
	ops.massConstant(1.0f);
	ops.positionUniformCentered(halfSizePoint, cv::Size{ 500, 500 });
	ops.velocityConstant(0.0f, 0.0f);
	
	Factory factory;
	Cloud cloud = factory.generateCloud(ops);
	
	Display display;
	display.screenSize(size);

	//cv::VideoWriter videoWriter;
	//videoWriter = cv::VideoWriter(
	//	"part_sim_100.avi",
	//	cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
	//	60,
	//	size
	//);

	ForceEngine forceEngine;
	
	forceEngine.initializeCloud(cloud);
	forceEngine.setFrameDuration(1.0f / 60.0f);
	
	tick::StopWatch sw;
	sw.reset();
	sw.start();

	int counter = 0;
	while (sw.elapsed() < chrono::seconds(10)) {
		counter++;
		const Cloud & cloudRef = forceEngine.cloud();
		
		display.render(cloudRef);
		cv::Mat frame = display.getFrame();
		display.show(1);
		
		//videoWriter.write(frame);

		forceEngine.update();
	}

	sw.stop();

	//videoWriter.release();

	long long ms = duration_cast<chrono::milliseconds>(sw.elapsed()).count();
	long long sec = duration_cast<chrono::seconds>(sw.elapsed()).count();

	cout << "Processed " << counter << " frames in " 
		<< ms
		<< "ms which is " 
		<< (float) counter / (float) sec << " frames per second" << endl;

	cin.get();

	return 0;
}