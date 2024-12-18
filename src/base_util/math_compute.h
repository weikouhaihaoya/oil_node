#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "base_util/base64.h"

std::vector<int> GenerateColorMap(int num_class);

double compute_rect_distance(cv::Rect& a, cv::Rect& b, cv::Mat img=cv::Mat());

double nearest_dis_of_two_rect(cv::Rect& box1, cv::Rect& box2);

// opencv math deal
  // iou
double compute_iou(cv::Rect box1, cv::Rect box2, int * const &diff_width=new int(0), int * const &diff_height=new int(0));
  // point in rect
bool point_in_rect(const cv::Point &p,const cv::Rect &rect);
  // point_distance
float point_distance(const cv::Point2f &p1, const cv::Point2f &p2);
  // distance of point to line 
float point_distance_to_line(cv::Point &p, cv::Vec4f &top_line);
cv::Mat calc_hist(cv::Mat& gray_img, cv::Rect det_box);

// opencv draw tool
int draw_rotated_rect(cv::Mat &draw_mat, const std::vector<cv::RotatedRect> &rrects);

	// 由斜率计算角度 与x轴夹角
float ratio_to_angle(float degree);
	// 由角度计算弧度
float angle_to_radians(float angle);

int cv_image_encode(cv::Mat &cv_image, std::string &encoded);
int cv_image_decode(std::string &encoded, cv::Mat &cv_image, int color);
int bgr2nv12(cv::Mat &src, cv::Mat &dst);
int nv122bgr(std::vector<char>& src, cv::Size size, cv::Mat &dst);
