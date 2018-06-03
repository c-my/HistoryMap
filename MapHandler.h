#pragma once
#include <opencv2\opencv.hpp>
#include <cmath>
#include <iostream>

class MapHandler
{
public:
	MapHandler(cv::Mat src1, cv::Mat src2);
	~MapHandler();

	/*
	@func 一个点的坐标匹配（模板匹配）
	@return 匹配的最大值 out为最大值的坐标
	@param point1 The first point of the first image
	@param point3 The first point of the second image
	*/
	double Match(const cv::Point2i& point1, const cv::Point2i& point2, cv::Point& out);
	/*
	@func 两个点的坐标匹配（只根据坐标匹配）
	@param point1 The first point of the first image
	@param point2 The second point of the first image
	@param point3 The first point of the second image
	@param point4 The second point of the second image
	*/
	cv::Mat Match2(const cv::Point2i& point1, const cv::Point2i& point2, const cv::Point2i& point3, const cv::Point2i& point4);

private:
	const int roiWid = 100, roiHei = 100;
	const double PI = 3.1415926;
	cv::Mat map1, map2;
	cv::Mat templateImg;
	cv::Mat result;

	/*
	@func 对图像src进行平移
	@param src 待操作的图像
	@param dx 横向移动距离
	@param dy 纵向移动距离
	*/
	void Shift(cv::Mat & src, const float& dx, const float& dy);

};

