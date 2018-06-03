#include<iostream>
#include<opencv.hpp>
#include"MapHandler.h"

#define MAP1_FILE	("map_pic/map1.jpg")
#define MAP2_FILE	("map_pic/map2.jpg")

using namespace cv;
using namespace std;

#define TEM_WIE (30)
#define TEM_HEI (30)

Point2i p1, p2, p3, p4;
Mat ROI;
Mat src1 = imread(MAP1_FILE);
Mat src2 = imread(MAP2_FILE);
MapHandler handler(src1, src2);
Point maxPoint;
bool map1Clicked = false, map2Clicked = false;
int cCount1 = 0;
int cCount2 = 0;

void on_mouse(int event, int x, int y, int flags, void* param)
{
	int flag = int(param);
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		if (flag == 1)
		{
			if (cCount1 == 0)
			{
				p1 = Point2i(x, y);
			}
			else if (cCount1 == 1)
			{
				p2 = Point2i(x, y);
			}
			cCount1++;
			cout << "Click on map1: (" << x << ", " << y << ")" << endl;
		}
		else if (flag == 2)
		{
			if (cCount2 == 0)
			{
				p3 = Point2i(x, y);
			}
			else if (cCount2 == 1)
			{
				p4 = Point2i(x, y);
			}
			cCount2++;
			cout << "Click on map2: (" << x << ", " << y << ")" << endl;

		}
		if (cCount1 == 2 && cCount2 == 2)
		{
			cCount1 = 0;
			cCount2 = 0;
			cout << "Matching..." << p1 << p2 << p3 << p4 << endl;
			Mat out = handler.Match2(p1, p2, p3, p4);
			imshow("Out", out);
		}
		break;
	default:
		break;
	}
}

int main()
{
	namedWindow("Map1");
	namedWindow("Map2");

	imshow("Map1", src1);
	imshow("Map2", src2);

	setMouseCallback("Map1", on_mouse, (void *)1);
	setMouseCallback("Map2", on_mouse, (void *)2);

	waitKey(0);
	return 0;
}