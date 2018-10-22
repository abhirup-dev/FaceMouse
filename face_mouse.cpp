#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "cursor.hpp"
auto WHITE_COLOR = cv::Scalar(255, 255, 255),
	YELLOW_COLOR = cv::Scalar(0, 255, 255),
	RED_COLOR = cv::Scalar(0, 0, 255),
	GREEN_COLOR = cv::Scalar(0, 255, 0),
	BLUE_COLOR = cv::Scalar(255, 0, 0),
	BLACK_COLOR = cv::Scalar(0, 0, 0),
	BROWN_COLOR = cv::Scalar(42, 42, 165),
	MAROON_COLOR = cv::Scalar(0, 0, 128),
	ORANGE_COLOR = cv::Scalar(0, 165, 255)
	;
std::pair<int,int>
	MOUTH = std::make_pair(48,68),
	LEYE = std::make_pair(42,48),
	REYE = std::make_pair(36,42),
	LBROW = std::make_pair(22,27),
	RBROW = std::make_pair(17,22),
	NOSE = std::make_pair(27,35),
	JAW = std::make_pair(0,17)
;
bool
	SCROLL_MODE = false,
	LEAN_MODE = false,
	CLICK_MODE = false;
int
	DIRECTION = 0,
	DOUBLE_BLINKS = 0,
	LEFT_BLINKS = 0,
	RIGHT_BLINKS = 0,
//	frame_counts
	DOUBLE_BLINK_COUNT = 0,
	MOUTH_COUNT = 0,
	BROW_COUNT = 0,
	RTILT_COUNT = 0,
	LTILT_COUNT = 0,
	LSQUINT_COUNT = 0,
	RSQUINT_COUNT = 0,
	LOW_FRAME_SENSITIVITY = 3,
	HIGH_FRAME_SENSITIVITY = 5,
	SQUINT_COUNTDOWN = 0;
float
//	thresholds
	MAR_THRESH = 0.3,
	LSQUINT_THRESH = 160,
	RSQUINT_THRESH = 160,
	LEAR_THRESH = 0,
	REAR_THRESH = 0;
std::vector<cv::Point>
	MOUTH_POINTS,
	LEYE_POINTS,
	REYE_POINTS;
int POINTER_X = 1920/2, POINTER_Y = 1080/2;
int PREV_X = 1920/2, PREV_Y = 1080/2;

Display *display = XOpenDisplay(0);
Window root = DefaultRootWindow(display);

using namespace dlib;
using namespace std;

std::pair<float, float> calcSquint(const full_object_detection &points);
float calcEAR(const std::vector<cv::Point> &pts);

void reset_pointer()
{
	POINTER_X = 1920/2, POINTER_Y = 1080/2;
}
void set_thresh(const full_object_detection &points)
{
	if(SQUINT_COUNTDOWN != 0)
	{
		auto val = calcSquint(points);
		if(LSQUINT_COUNT == 0)
			LSQUINT_THRESH = val.first;
		if(RSQUINT_COUNT == 0)
			RSQUINT_THRESH = val.second;
	}
	if(!CLICK_MODE)
	{
		if(DOUBLE_BLINK_COUNT==0)
		{
			LEAR_THRESH = calcEAR(LEYE_POINTS);
			REAR_THRESH = calcEAR(REYE_POINTS);
		}
	}
}
std::vector<cv::Point> extract(const full_object_detection &points, std::pair<int,int> idx)
{
	std::vector<cv::Point> pts;
	cv::Point tmp;
	int start = idx.first, end = idx.second;
	for(int i=start; i<end; i++)
	{
		tmp.x = points.part(i).x();
		tmp.y = points.part(i).y();
		pts.push_back(tmp);
	}
	return pts;
}
void extract_all(const full_object_detection &points)
{
	MOUTH_POINTS = extract(points, MOUTH);
	LEYE_POINTS = extract(points, LEYE);
	REYE_POINTS = extract(points, REYE);
}
void add_stats(cv::Mat &img)
{
	std::string release = "CLICK: " + std::string(CLICK_MODE?"ON":"OFF");
	std::string scroll = "SCROLL: " + std::string(SCROLL_MODE?"ON":"OFF");
	std::string drx =
			std::string(DIRECTION==1?"UP":
					(DIRECTION==2?"DOWN":
							(DIRECTION==3?"LEFT":"RIGHT")));
	std::string dbbl = "DOUBLE: " + std::to_string(DOUBLE_BLINKS),
			lrbl = "LEFT: " + std::to_string(LEFT_BLINKS) + " RIGHT: " + std::to_string(RIGHT_BLINKS);

	cv::putText(img, release, cvPoint(30,30),
	    cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, RED_COLOR, 1, CV_AA);
	cv::putText(img, scroll, cvPoint(30,60),
		cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, RED_COLOR, 1, CV_AA);
	if(SCROLL_MODE)
		cv::putText(img, drx, cvPoint(30,90),
			cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, RED_COLOR, 1, CV_AA);
	cv::putText(img, dbbl, cvPoint(30,150),
			cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, RED_COLOR, 1, CV_AA);
	cv::putText(img, lrbl, cvPoint(30,180),
				cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, RED_COLOR, 1, CV_AA);
}
void add_overlay(cv::Mat &img, const full_object_detection &points)
{
	if(SCROLL_MODE)
	{
		auto &pts = MOUTH_POINTS;
		for(int i=0; i<pts.size()-1; i++)
			cv::line(img, pts[i+1], pts[i], GREEN_COLOR, 2, CV_AA);
	}

	if(CLICK_MODE)
	{
		auto &pts = LEYE_POINTS;
		for(int i=0; i<pts.size()-1; i++)
		{
			auto col = YELLOW_COLOR; int lw = 1;
			if(LSQUINT_COUNT > 0)
				{
					col = BLUE_COLOR;
					lw = LSQUINT_COUNT;
				}
			cv::line(img, pts[i+1], pts[i], col, lw, CV_AA);
		}
		pts = REYE_POINTS;
		for(int i=0; i<pts.size()-1; i++)
		{
			auto col = YELLOW_COLOR; int lw = 1;
			if(RSQUINT_COUNT > 0)
				{
					col = BLUE_COLOR;
					lw = RSQUINT_COUNT;
				}
			cv::line(img, pts[i+1], pts[i], col, lw, CV_AA);
		}
	}
}
void mark_nose(const cv::Mat &img, const full_object_detection &points)
{
	cv::Point tmp;
	tmp.x = points.part(30).x();
	tmp.y = points.part(30).y();
	cv::line(img, cv::Point(points.part(30).x(), points.part(30).y()), cv::Point(0,0), GREEN_COLOR);
	std::cout << "Nosed\n";
}
void track_nose(const cv::Mat &img, const full_object_detection &points, int mult = 1)
{
	float xsns = 3, ysns = 3;
	auto ptr = getCurrentPos(display, root);
	int currx = points.part(30).x(), curry = points.part(30).y();
	float ratiox = 1920.0/img.cols, ratioy = 1080.0/img.rows;
	float delx = currx - ptr.first / ratiox, dely = curry - ptr.second / ratioy;
	if(abs(delx) > xsns)
		POINTER_X += mult*delx;
	if(abs(dely) > ysns)
		POINTER_Y += mult*dely;
//	std::cout << "Current = " << currx << " " << curry << "\n";
//	std::cout << "Pointer = " << POINTER_X << " " << POINTER_Y << "\n";
	if(POINTER_X <= 2 || POINTER_X >= 1917)
	{
		std::cout << "Repointing.\n";
		reset_pointer();
	}

	XWarpPointer(display, None, root, 0, 0, 0, 0, POINTER_X, POINTER_Y);
	XFlush(display);
}
void MAR(cv::Mat &img, const full_object_detection &points)
{
	MOUTH_POINTS = extract(points, MOUTH);;
	float mar =
			(cv::norm(cv::Mat(MOUTH_POINTS[13]), cv::Mat(MOUTH_POINTS[19])) +
			cv::norm(cv::Mat(MOUTH_POINTS[14]), cv::Mat(MOUTH_POINTS[18])) +
			cv::norm(cv::Mat(MOUTH_POINTS[15]), cv::Mat(MOUTH_POINTS[17]))) /
			(2 * cv::norm(cv::Mat(MOUTH_POINTS[12]), cv::Mat(MOUTH_POINTS[16])));
	// std::cout << "MAR = " << mar << "\n";
	if(mar >= MAR_THRESH && !SCROLL_MODE)
	{
		SCROLL_MODE = true;
		CLICK_MODE = false;
		std::cout << "Scrolling is ON.\n";
	}
	else if(mar < MAR_THRESH && SCROLL_MODE)
	{
		SCROLL_MODE = false;
		std::cout << "Scrolling is OFF.\n";
	}
}
float calcEAR(const std::vector<cv::Point> &pts)
{
	float EAR = 100*(cv::norm(cv::Mat(pts[1]), cv::Mat(pts[5]))+
			cv::norm(cv::Mat(pts[2]), cv::Mat(pts[4])))/
			(2*cv::norm(cv::Mat(pts[0]), cv::Mat(pts[3])));
	return EAR;
}
std::pair<float, float> calcSquint(const full_object_detection &points)
{
	std::vector<cv::Point> lvec(6), rvec(6);
	lvec[0].x = points.part(17).x();lvec[0].y = points.part(17).y();
	lvec[1].x = points.part(21).x();lvec[1].y = points.part(21).y();
	lvec[2].x = points.part(36).x();lvec[2].y = points.part(36).y();
	lvec[3].x = points.part(39).x();lvec[3].y = points.part(39).y();
	lvec[4].x = points.part(19).x();lvec[4].y = points.part(19).y();
	lvec[5].x = points.part(6).x();lvec[5].y = points.part(6).y();//
	rvec[0].x = points.part(26).x();rvec[0].y = points.part(26).y();
	rvec[1].x = points.part(22).x();rvec[1].y = points.part(22).y();
	rvec[2].x = points.part(45).x();rvec[2].y = points.part(45).y();
	rvec[3].x = points.part(42).x();rvec[3].y = points.part(42).y();
	rvec[4].x = points.part(24).x();rvec[4].y = points.part(24).y();
	rvec[5].x = points.part(10).x();rvec[5].y = points.part(10).y();

	return std::make_pair(
			(cv::norm(cv::Mat(lvec[0]), cv::Mat(lvec[2])) +
			 cv::norm(cv::Mat(lvec[1]), cv::Mat(lvec[3]))) *
			(0.02*cv::norm(cv::Mat(lvec[4]), cv::Mat(lvec[5]))),
			(cv::norm(cv::Mat(rvec[0]), cv::Mat(rvec[2])) +
			 cv::norm(cv::Mat(rvec[1]), cv::Mat(rvec[3]))) *
			(0.02*cv::norm(cv::Mat(rvec[4]), cv::Mat(rvec[5]))));
}
void blink_action(const full_object_detection &points)
{
	float sns = 6;
	float LEAR, REAR;
	LEAR = calcEAR(LEYE_POINTS);
	REAR = calcEAR(REYE_POINTS);
	if(REAR <= REAR_THRESH-sns && LEAR <= LEAR_THRESH-sns && !CLICK_MODE)
	{
		std:cout << "=====+++++Mark+++++=====\n";
		DOUBLE_BLINK_COUNT++;
		if(DOUBLE_BLINK_COUNT == HIGH_FRAME_SENSITIVITY)
		{
			DOUBLE_BLINKS++;
			CLICK_MODE = true;
			SCROLL_MODE = false;
			DOUBLE_BLINK_COUNT = 0;
			SQUINT_COUNTDOWN = 10;
		}
	}
	else if(REAR > REAR_THRESH-sns && LEAR > LEAR_THRESH-sns && DOUBLE_BLINK_COUNT > 0)
		DOUBLE_BLINK_COUNT = 0;
//	else if(REAR <= 17) RIGHT_BLINKS++;
//	else if(LEAR <= 17) LEFT_BLINKS++;
//	std::cout << "lear = " << LEAR << " rear = " << REAR << "\n";
//	usleep(0.3*1e5);
}
void squint_action(const full_object_detection &points)
{
	if(SQUINT_COUNTDOWN != 0)
	{
		SQUINT_COUNTDOWN--;
		return;
	}
	int sns = 15;
	auto val = calcSquint(points);
	float ls = val.first, rs = val.second;
//	std::cout << "<<" << ls << ">> <<" << rs << ">>\n";
	if(ls <= LSQUINT_THRESH-sns && !SCROLL_MODE && CLICK_MODE)
	{
		LSQUINT_COUNT++;
		RSQUINT_COUNT = 0;
		if(LSQUINT_COUNT == HIGH_FRAME_SENSITIVITY)
		{
			LEFT_BLINKS++;
			std::cout << "=====+++++Left-CLick+++++=====\n";
			mouseClick(2, display);
			LSQUINT_COUNT = 0;
			CLICK_MODE = false;
		}
	}
	else if(ls > LSQUINT_THRESH-sns && LSQUINT_COUNT > 0)
		LSQUINT_COUNT = 0;//reset count
	if(rs <= RSQUINT_THRESH-sns && !SCROLL_MODE && CLICK_MODE)
	{
		RSQUINT_COUNT++;
		if(RSQUINT_COUNT == HIGH_FRAME_SENSITIVITY)
		{
			RIGHT_BLINKS++;
			LSQUINT_COUNT = 0;
			std::cout << "=====+++++Right-CLick+++++=====\n";
			mouseClick(3, display);
			RSQUINT_COUNT = 0;
			CLICK_MODE = false;
		}
	}
	else if(rs > RSQUINT_THRESH-sns && RSQUINT_COUNT > 0)
		RSQUINT_COUNT = 0;//reset count
}
void grab_scroll()
{
	auto ptr = getCurrentPos(display, root);
	if(ptr.second > PREV_Y + 10)
	{
		mouseClick(5, display);
		DIRECTION = 2;
		// usleep(1e4);
	}
	else if(ptr.second < PREV_Y - 10)
	{
		mouseClick(4, display);
		DIRECTION = 1;
		// usleep(1e4);
	}
	PREV_X = ptr.first;
	PREV_Y = ptr.second;
}

int main()
{
    try
    {
        cv::VideoCapture cap("/dev/video0");
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        int frames=0, nframes=1000;
        time_t start, end;
        cv::namedWindow("new", 1);
        cv::moveWindow("new", 1500, 0);
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

        while(true)
        {
            cv::Mat temp, out;

            cap >> out;
            cv::flip(out, temp, 1);
            temp.copyTo(out);
            cv_image<bgr_pixel> cimg(out);
            std::vector<rectangle> faces = detector(cimg);
            std::vector<full_object_detection> shapes;
			shapes.push_back(pose_model(cimg, faces[0]));

			extract_all(shapes.back());//needs to be first commend in sequence
			if(frames == 0)
				set_thresh(shapes.back());
			blink_action(shapes.back());
			squint_action(shapes.back());
			add_stats(out);
			track_nose(out, shapes.back(), 3);
			set_thresh(shapes.back());
            MAR(out, shapes.back());
			if(SCROLL_MODE)
				grab_scroll();
			add_overlay(out, shapes.back());// needs to be last command in the sequence
            cv::imshow("new", out);
            frames++;MAR(out, shapes.back());
			if(SCROLL_MODE)
				grab_scroll();
//            std::cout <<LEAR_THRESH << " " << REAR_THRESH << "\n";

            if(cv::waitKey(30) >= 0) break;
        }


    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
}
