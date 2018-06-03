#include "MapHandler.h"



MapHandler::MapHandler(cv::Mat src1, cv::Mat src2) :map1(src1), map2(src2)
{
}


MapHandler::~MapHandler()
{
}

double MapHandler::Match(const cv::Point2i& point1, const cv::Point2i& point2, cv::Point& out)
{
	this->templateImg = map1(cv::Rect(point1.x - roiWid / 2, point1.y - roiHei / 2, roiWid, roiHei));
	cv::Mat tmpMap2 = map2.clone();
	double maxValue = 0, maybeMax = 0, angle = 0, scale = 1;
	cv::Point maxPoint;
	cv::minMaxLoc(result, nullptr, &maxValue, &out, nullptr);
	cv::Point tp;

	for (int i = -30; i <= 30; i++) {
		for (double j = 0.7; j <= 1.3; j += 0.05)
		{
			cv::Mat M = cv::getRotationMatrix2D(point2, i, j);
			cv::warpAffine(map2, tmpMap2, M, cv::Size(tmpMap2.size().width, tmpMap2.size().height));//旋转map2
			cv::Mat tmp = tmpMap2(cv::Rect(point2.x - 1.5 * roiWid / 2, point2.y - 1.5 * roiHei / 2, 1.5 * roiWid, 1.5 * roiHei));//map2 ROI
			result = cv::Mat(tmpMap2.cols - templateImg.cols + 1, tmpMap2.rows - templateImg.rows + 1, CV_32FC1);
			cv::matchTemplate(tmp, templateImg, result, cv::TM_CCOEFF_NORMED);
			cv::minMaxLoc(result, nullptr, &maybeMax, &out, nullptr);
			if (maybeMax > maxValue)
			{
				maxValue = maybeMax;
				angle = i;
				maxPoint = out;
				scale = j;
			}
		}
	}
	cv::Mat M = cv::getRotationMatrix2D(point2, angle, scale);
	cv::warpAffine(map2, tmpMap2, M, cv::Size(tmpMap2.size().width, tmpMap2.size().height));//旋转map2
	cv::Mat tmp = tmpMap2(cv::Rect(point2.x - 1.5 * roiWid / 2, point2.y - 1.5 * roiHei / 2, 1.5 * roiWid, 1.5 * roiHei));//map2 ROI
	tp.x = maxPoint.x + roiWid / 2;
	tp.y = maxPoint.y + roiHei / 2;
	cv::circle(tmp, tp, 3, cv::Scalar(0, 0, 255), 3);
	cv::imshow("TmpMap2", tmpMap2);

	//out.x += point2.x /*- 2 * roiWid / 2*/;
	//out.y += point2.y /*- 2 * roiHei / 2*/;
	return maxValue;
}


cv::Mat MapHandler::Match2(const cv::Point2i & point1, const cv::Point2i & point2, const cv::Point2i & point3, const cv::Point2i & point4)
{
	//计算两点间距离以确定缩放比
	double d1 = std::sqrt(std::pow(point1.x - point2.x, 2) + cv::pow(point1.y - point2.y, 2));
	double d2 = std::sqrt(std::pow(point3.x - point4.x, 2) + std::pow(point3.y - point4.y, 2));
	double scale = d2 / d1;

	std::cout << "Scale: " << scale << std::endl;

	//计算斜率及角度
	double k1 = (1.0*point1.y - point2.y) / (point1.x - point2.x);
	double k2 = (1.0*point3.y - point4.y) / (point3.x - point4.x);
	double angle1 = std::atan(k1);
	double angle2 = std::atan(k2);
	double angle = (k1 - k2) * 180 / PI;
	std::cout << "Angle: " << angle << std::endl;

	//以map1中心点旋转
	cv::Point2f center = cv::Point2f(map1.cols / 2.0, map1.rows / 2.0);

	cv::Mat transMatrix = cv::getRotationMatrix2D(center, angle, scale);
	cv::warpAffine(map1, map1, transMatrix, map1.size());

	cv::Mat map22 = cv::Mat::zeros(map2.size(), map2.type());
	cv::Mat tar = map22(cv::Rect(0, 0, map1.cols, map1.rows));

	//将变换后的map1与map2叠加在一起
	map1.copyTo(tar);
	Shift(map22, point3.x - point1.x, point4.y - point2.y);
	cv::Mat out;
	cv::addWeighted(map2, 0.5, map22, 0.5, 0, out);
	//cv::imshow("Final", out);
	return out;

}

void MapHandler::Shift(cv::Mat & src, const float & dx, const float & dy)
{
	cv::Mat t_mat = cv::Mat::zeros(2, 3, CV_32FC1);

	t_mat.at<float>(0, 0) = 1;
	t_mat.at<float>(0, 2) = dx; //水平平移量  
	t_mat.at<float>(1, 1) = 1;
	t_mat.at<float>(1, 2) = dy; //竖直平移量  

	cv::warpAffine(src, src, t_mat, src.size());
}


