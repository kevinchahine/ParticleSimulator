#include "display.h"

using namespace std;

const cv::Scalar BLACK{ 0, 0, 0 };
const cv::Scalar GRAY{ 64, 64, 64 };
const cv::Scalar LIGHT_GRAY{ 32, 32, 32 };
const cv::Scalar BLUE{ 255, 0, 0 };
const cv::Scalar GREEN{ 0, 255, 0 };

void Display::render(const Cloud & cloud) {
	currentCloud = &cloud;

	cv::Mat particlePanel = cv::Mat::zeros(_particleSpaceSize, CV_8UC3);

	renderBackground(particlePanel);
	renderMarkers(particlePanel);
	renderParticles(particlePanel, cloud);
	//renderCentroid(particlePanel);
	renderTrails(particlePanel);
	renderTextOverlay(particlePanel);

	overlayParticlePanel(particlePanel);
}

void Display::show() {
	show(1);
}

void Display::show(int delayMS) {
	cv::imshow("Particle Simulator", _frame);
	cv::waitKey(delayMS);
}

void Display::setCenter(const Cloud & cloud) {
	_particleSpaceCenter = cloud.position().centroid();
}

void Display::renderBackground(cv::Mat & img) {
	cv::rectangle(img, cv::Rect{ cv::Point{0, 0}, img.size() }, BLACK, -1);
}

void Display::renderMarkers(cv::Mat & img) {
	cv::Size halfSize = img.size() / 2;

	cv::Point origin{ 
		(int) (_particleSpaceCenter.x - halfSize.width), 
		(int) (_particleSpaceCenter.y - halfSize.height)
	};
	
	cv::Point offset = cv::Point{
		_markerSpacing - origin.x % _markerSpacing,
		_markerSpacing - origin.y % _markerSpacing
	};

	for (
		cv::Point topPoints = offset; 
		topPoints.x < img.cols; 
		topPoints += cv::Point(_markerSpacing, 0)
		) {
		for (
			cv::Point downPoints = topPoints; 
			downPoints.y < img.rows; 
			downPoints += cv::Point(0, _markerSpacing)
			) {
			cv::drawMarker(img, downPoints, LIGHT_GRAY, cv::MarkerTypes::MARKER_CROSS, 5);
		}
	}
}

void Display::renderCentroid(cv::Mat & img) {
	cv::Point2f centroid = currentCloud->position().centroid();

	cv::circle(img, centroid, 1, cv::Scalar{ 0, 0, 255 });
}

void Display::renderParticles(cv::Mat & img, const Cloud & cloud) {
	for (const Cloud::Particle & part : cloud) {
		cv::Point pos = part.position();
	
		cv::circle(img, pos, 4, BLUE, -1, cv::LineTypes::FILLED);
	}
}

void Display::renderTrails(cv::Mat & img) {}

void Display::renderTextOverlay(cv::Mat & img) {}

void Display::overlayParticlePanel(cv::Mat & particlePanel) {
	if (_frame.size() != particlePanel.size()) {
		_frame = cv::Mat::zeros(particlePanel.size(), CV_8UC3);
	}

	particlePanel.copyTo(_frame);
}
