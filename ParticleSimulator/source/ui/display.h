#pragma once

#include <opencv2/opencv.hpp>

#include "../cloud/cloud.h"

/*
	+-----------+-----------------------------------+
	|			|									|
	| Info		|									|
	| (hidden)	|		Particle Space				|
	| (by)		|									|
	| (default)	|									|
	|			|									|
	|			|									|
	+-----------+-----------------------------------+
*/
class Display
{
public:
	void render(const Cloud & cloud);

	void show();
	void show(int delayMS);

	cv::Mat getFrame() const { return _frame; }

	void screenSize(const cv::Size & size) {
		_particleSpaceSize = size;
	}

private:
	void setCenter(const Cloud & cloud);

	void renderBackground(cv::Mat & img);
	void renderMarkers(cv::Mat & img);
	void renderCentroid(cv::Mat & img);
	void renderParticles(cv::Mat & img, const Cloud & cloud);
	void renderTrails(cv::Mat & img);
	void renderTextOverlay(cv::Mat & img);
	void overlayParticlePanel(cv::Mat & particlePanel);

private:
	cv::Mat _frame;
	cv::Size _particleSpaceSize{ 1000, 600 };
	cv::Size _infoPanelSize;
	cv::Point2f _particleSpaceCenter;
	int _markerSpacing = 100;
	const Cloud * currentCloud = nullptr;
}; // class Display
