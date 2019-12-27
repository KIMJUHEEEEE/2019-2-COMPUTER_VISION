#include <opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/core.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<math.h>
using namespace cv;
using namespace std;
double PI = 3.14159282;
int rect = 0;
typedef struct value
{
	vector<int> top;
	vector<int> left;
	vector<int> width;
	vector<int> height;
	int dir;
}values;
values detect(Mat& image,double minHue,double maxHue,
	double minSat,double maxSat,Mat &mask,int c,ofstream &out)
{
	values v;
	Mat hsv;Mat imageROI = image(Rect(0,0,720, 240));
	cvtColor(imageROI, hsv, COLOR_BGR2HSV);
	vector<Mat> channels;
	split(hsv, channels);
	Mat mask1;
	threshold(channels[0], mask1, maxHue, 255, THRESH_BINARY_INV);
	Mat mask2;
	threshold(channels[0], mask2, minHue, 255, THRESH_BINARY);
	Mat hueMask;
	if (minHue < maxHue) hueMask = mask1 & mask2;
	else hueMask = mask1 | mask2;

	threshold(channels[1], mask1, maxSat, 255, THRESH_BINARY_INV);
	threshold(channels[1], mask2, minSat, 255, THRESH_BINARY);

	Mat satMask;
	satMask = mask1 & mask2;
	mask = hueMask & satMask;
	Mat img_labels, stats, centroids;
	erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	int numOfLables = connectedComponentsWithStats(mask, img_labels,
		stats, centroids, 8, CV_32S);
	vector <Vec3f> circles;

	//영역박스 그리기
	int max = -1, idx = 0;
	Mat bgr[3];
	split(image, bgr);
	for (int j = 1; j < numOfLables; j++) {
		int area = stats.at<int>(j, CC_STAT_AREA);
		if (area>40)
		{
			string s = "";
			idx = j;
			int left = stats.at<int>(idx, CC_STAT_LEFT);
			int top = stats.at<int>(idx, CC_STAT_TOP) ;
			int width = stats.at<int>(idx, CC_STAT_WIDTH);
			int height = stats.at<int>(idx, CC_STAT_HEIGHT);
				if (height > 6&&height<30)
				{
					if (c == 0)///green
					{
						Point pt(left + width, top + height);
						if (sqrt((double)area / PI) > (width - 3) / 2 && sqrt(area / PI) < (height + 2) / 2)
						{
							if (width - 5 < height && width + 5 > height)
							{
									rectangle(image, Point(left - 2 * height, top), Point(left + width, top + height),
										Scalar(0, 0, 255), 1);
									v.left.push_back(left - 2 * height);
									v.top.push_back(top);
									v.height.push_back(height);
									v.width.push_back(width * 3);
									v.dir = 0;
									rect = 1;
								//else if (rect == 1)//신호등 검출이 이미 되었을 때
								//{
								//	if (bgr[2].at<int>(height / 2, left - width * 1.5) > 100 && width - 5 < height && width + 5 > height && left - width * 1.5 >= 0)
								//		v.dir = 0;
								//	else
								//		v.dir = 1;
								//}
							}
						}

						else if (area >= 190 && area<300&& c == 0)//직진+좌회전 신호
						{
							if (bgr[1].at<int>(height / 2, left - width / 2) > 100||height<width&&height*2>width)
							{
								rectangle(image, Point(left - 2 * height, top), Point(left + 1.3 * width, top + height),
									Scalar(0, 0, 255), 1);
								v.dir = 2;
								s += to_string(v.dir);
								s += ",";
								s += to_string(left - 2 * height);
								s += ",";
								s += to_string(top);
								s += ",";
								s += to_string(width * 4);
								s += ",";
								s += to_string(height);
								s += '\n';
								out << s;
							}
						}
					}
					else if (c == 1)//red
					{split(image, bgr);
						if (rect == 0)
						{
							rectangle(image, Point(left, top), Point(left + width * 4, top + height),
								Scalar(0, 0, 255), 1);
							v.left.push_back(left);
							v.top.push_back(top);
							v.height.push_back(height);
							v.width.push_back(width * 3);
							v.dir = 1;
						}
						else if (rect == 1&& bgr[2].at<int>(height / 2, left + width *1.5) > 100)
						{
							v.dir = 1;
						}
					}
				}
		}
		
	}
	return v;
}
int main()
{
	values vr1, vr2, vg;
	int i = 175;
	string st =to_string(i);
	Mat image = imread("../image/Traffic Signal/a ("+st+").jpg", 1);
	CV_Assert(image.data);
	Size size(720, 480);
	resize(image, image, size);
	Mat maskr1, maskr2, mask1, mask;
	int r = 0, g = 0;
	ofstream out(st+".csv");
	int red = 0, green = 0;
	vg = detect(image, 70, 100, 180, 256, mask1, 0, out);
	vr1=detect(image, 170,180,120,256,maskr1,1,out);
	vr2=detect(image, 0,15, 120, 256, maskr2,1,out);
 	//vg=detect(image, 70,100, 180, 256, mask1,0,out);
	mask = maskr1 + maskr2;
	if (vr1.dir == 1 || vr2.dir == 1)
	{
		red = 1;
	}
	else if (vg.dir == 0)
	{
		green = 1;
	}
	else if (vg.dir == 2)
	{
		green = 2;
	}
	if (red == 1&&vg.dir == 0)
	{
		green = 1;
	}
	int dir = 0;
	string s = "";
	//int vr = vr1.height.size() + vr2.height.size();
	if (red == 1&&green==0)
	{
	for (int i = 0; i < vr1.height.size(); i++)
		{
		string s = "";
			dir = 1;
			cout << dir << ' ' << vr1.left[i] << ' ' << vr1.top[i] << ' ' << vr1.width[i] << ' ' << vr1.height[i] << endl;
			s += to_string(dir);
			s += ",";
			s += to_string(vr1.left[i]);
			s += ",";
			s += to_string(vr1.top[i]);
			s += ",";
			s += to_string(vr1.width[i]);
			s += ",";
			s += to_string(vr1.height[i]);
			s += '\n';
			out << s;
		}
	for (int i = 0; i < vr2.height.size(); i++)
	{
		string s = "";
		dir = 1;
		cout << dir << ' ' << vr2.left[i] << ' ' << vr2.top[i] << ' ' << vr2.width[i] << ' ' << vr2.height[i] << endl;
		s += to_string(dir);
		s += ",";
		s += to_string(vr2.left[i]);
		s += ",";
		s += to_string(vr2.top[i]);
		s += ",";
		s += to_string(vr2.width[i]);
		s += ",";
		s += to_string(vr2.height[i]);
		s += '\n';
		out << s;
	}
	}
	else if (green == 1&&red==0)
	{
		for (int i = 0; i < vg.height.size(); i++)
		{
			dir = 0;
			string s = "";
			cout << dir << ' ' << vg.left[i] << ' ' << vg.top[i] << ' ' << vg.width[i] << ' ' << vg.height[i] << endl;
			s += to_string(dir);
			s += ",";
			s += to_string(vg.left[i]);
			s += ",";
			s += to_string(vg.top[i]);
			s += ",";
			s += to_string(vg.width[i]);
			s += ",";
			s += to_string(vg.height[i]);
			s += '\n';
			out << s;
		}
	}
	else if (red == 1 && green == 1)
	{
		if (vr1.height.size() + vr2.height.size() > vg.height.size())
		{
			for (int i = 0; i < vr1.height.size(); i++)
			{
				dir = 3;
				string s = "";
				cout << dir << ' ' << vr1.left[i] << ' ' << vr1.top[i] << ' ' << vr1.width[i] << ' ' << vr1.height[i] << endl;
				s += to_string(dir);
				s += ",";
				s += to_string(vr1.left[i]);
				s += ",";
				s += to_string(vr1.top[i]);
				s += ",";
				s += to_string(vr1.width[i]);
				s += ",";
				s += to_string(vr1.height[i]);
				s += '\n';
				out << s;
			}
			for (int i = 0; i < vr2.height.size(); i++)
			{
				dir = 3;
				string s = "";
				cout << dir << ' ' << vr2.left[i] << ' ' << vr2.top[i] << ' ' << vr2.width[i] << ' ' << vr2.height[i] << endl;
				s += to_string(dir);
				s += ",";
				s += to_string(vr2.left[i]);
				s += ",";
				s += to_string(vr2.top[i]);
				s += ",";
				s += to_string(vr2.width[i]);
				s += ",";
				s += to_string(vr2.height[i]);
				s += '\n';
				out << s;
			}
		}
		else
		{
			for (int i = 0; i < vg.height.size(); i++)
			{
				string s = "";
				dir = 3;
				cout << dir << ' ' << vg.left[i] << ' ' << vg.top[i] << ' ' << vg.width[i] << ' ' << vg.height[i] << endl;
				s += to_string(dir);
				s += ",";
				s += to_string(vg.left[i]);
				s += ",";
				s += to_string(vg.top[i]);
				s += ",";
				s += to_string(vg.width[i]);
				s += ",";
				s += to_string(vg.height[i]);
				s += '\n';
				out << s;
			}
		}
	}

	out.close();
	imshow("green", mask1);
	imshow("원본", image);
	waitKey();
}
