#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "globals.h"
#include "factory/factory.h"
#include "ui/display.h"
#include "force_engine.h"
#include "stop_watch.h"

using namespace std;

void run() {
	cv::Size size
	//{ 500, 300 };
	{ 1000, 600 };
	cv::Size halfSize = size / 2;
	cv::Point2f halfSizePoint{ (float) halfSize.width, (float) halfSize.height };

	FactoryOptions ops;
	ops.nParticles(3);
	ops.massConstant(1.0f);
	ops.positionUniformCentered(halfSizePoint, halfSize);
	//ops.velocityUniform(-8.0f, 8.0f, -8.0f, 8.0f);
	ops.velocityConstant(0.0f, 0.0f);
	ops.typeDiscrete({ ParticleType::PROTON, ParticleType::NEUTRON, ParticleType::ELECTRON });
	
	Factory factory;
	Cloud cloud = factory.generateCloud(ops);
	
	//cv::VideoWriter videoWriter;
	//videoWriter = cv::VideoWriter(
	//	"part_sim_100.avi",
	//	cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
	//	60,
	//	size
	//);

	ForceEngine forceEngine;
	
	forceEngine.initialize(cloud);
	forceEngine.frameRate(120.0f);
	forceEngine.timeScalar(
		//1.0f
		//1000.0f
		2.0e+7
		//2.0e+8
		//6.67430e+13
	);
	forceEngine.setIntegralApproximationMethod(
		//ForceEngine::IntegralApproximationMethod::RAM
		//ForceEngine::IntegralApproximationMethod::TRAPAZOIDAL
		ForceEngine::IntegralApproximationMethod::SIMPSONS
	);
	forceEngine.setCalculationMethod(
		//ForceEngine::CalculationMethod::SCALAR
		ForceEngine::CalculationMethod::MATRIX
	);

	Display display;
	display.screenSize(size);
	display.frameRate(forceEngine.frameRate());

	tick::StopWatch sw;
	sw.reset();
	sw.start();

	this_thread::sleep_for(chrono::milliseconds(100));
	chrono::seconds epochTime(10);

	int counter = 0;
	while (sw.elapsed() < epochTime) {
		counter++;
		const Cloud & cloudRef = forceEngine.cloud();
		
		display.render(cloudRef);
		cv::Mat frame = display.getFrame();
		display.show();
		
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
}

int main() {
	while (true) {
		run();
	}

	cin.get();

	return 0;
}