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
	@func һ���������ƥ�䣨ģ��ƥ�䣩
	@return ƥ������ֵ outΪ���ֵ������
	@param point1 The first point of the first image
	@param point3 The first point of the second image
	*/
	double Match(const cv::Point2i& point1, const cv::Point2i& point2, cv::Point& out);
	/*
	@func �����������ƥ�䣨ֻ��������ƥ�䣩
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
	@func ��ͼ��src����ƽ��
	@param src ��������ͼ��
	@param dx �����ƶ�����
	@param dy �����ƶ�����
	*/
	void Shift(cv::Mat & src, const float& dx, const float& dy);

};

