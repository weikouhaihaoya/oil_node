#include "math_compute.h"
#include <iostream>
#include <fstream>


bool point_in_rect(const cv::Point &p, const cv::Rect &rect)
{
  return p.x >= rect.x && p.x <= rect.x+rect.width && p.y >= rect.y && p.y <= rect.y + rect.height; 
}

float point_distance(const cv::Point2f &p1, const cv::Point2f &p2)
{
    return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
}

float point_distance_to_line(cv::Point &pointP, cv::Vec4f &top_line)
{
  // // y-b = k(x-a)
  // double k = top_line[1] / top_line[0];
  // cv::Point2f pointA(top_line[2], top_line[3]);
  // cv::Point2f pointB(0, k*(0 - pointA.x) + pointA.y);
    // //求直线方程
    // int A = 0, B = 0, C = 0;
    // A = pointA.y - pointB.y;
    // B = pointB.x - pointA.x;
    // C = pointA.x*pointB.y - pointA.y*pointB.x;
    // //代入点到直线距离公式
    // float distance = 0;
    // distance = ((float)abs(A*pointP.x + B*pointP.y + C)) / ((float)sqrtf(A*A + B*B));
    // return distance;


    //求直线方程
    int A = 0, B = 0, C = 0;
    A = top_line[1] - top_line[3];
    B = top_line[2] - top_line[0];
    C = top_line[0]*top_line[3] - top_line[1]*top_line[2];
    //代入点到直线距离公式
    float distance = 0;
    distance = ((float)abs(A*pointP.x + B*pointP.y + C)) / ((float)sqrtf(A*A + B*B));
    return distance;

}
double compute_iou(cv::Rect box1, cv::Rect box2, int * const &diff_width, int * const &diff_height)
{
  int x0 = std::max(box1.x, box2.x);
  int x1 = std::min(box1.x + box1.width, box2.x + box2.width);

  int y0 = std::max(box1.y, box2.y);
  int y1 = std::min(box1.y + box1.height, box2.y + box2.height);

  *diff_width = x1 - x0;
  *diff_height = y1- y0;
  
  if (*diff_height > 0 && *diff_width > 0){
    return ((*diff_height) * (*diff_width)) * 1.0 / (box1.width * box1.height + box2.width * box2.height - (*diff_height) * (*diff_width));
  } else { return -1.;}
}
std::vector<int> GenerateColorMap(int num_class) {
  auto colormap = std::vector<int>(3 * num_class, 0);
  for (int i = 0; i < num_class; ++i) {
    int j = 0;
    int lab = i;
    while (lab) {
      colormap[i * 3] |= (((lab >> 0) & 1) << (7 - j));
      colormap[i * 3 + 1] |= (((lab >> 1) & 1) << (7 - j));
      colormap[i * 3 + 2] |= (((lab >> 2) & 1) << (7 - j));
      ++j;
      lab >>= 3;
    }
  }
  std::reverse(colormap.begin(), colormap.end());
  return colormap;
}

double compute_rect_distance(cv::Rect& a, cv::Rect& b, cv::Mat img) {
  if (img.empty()) {
    int ax = a.x + a.width/2;
    int ay = a.y + a.height/2;
    int bx = b.x + b.width/2;
    int by = b.y + b.height/2;
    return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
  }
  else {
    // 转换为归一化距离
    float acx = (a.x + a.width/2)* 1.0 / img.cols;
    float acy = (a.y + a.height/2)* 1.0 / img.rows;
    float bcx = (b.x + b.width/2)* 1.0 / img.cols;
    float bcy = (b.y + b.height/2)* 1.0 / img.rows;
    return sqrt(pow(bcx - acx, 2) + pow(bcy - acy, 2)) / sqrt(2);
  }
}

double nearest_dis_of_two_rect(cv::Rect& box1, cv::Rect& box2) {
  int x0 = std::max(box1.x, box2.x);
  int x1 = std::min(box1.x + box1.width, box2.x + box2.width);

  int y0 = std::max(box1.y, box2.y);
  int y1 = std::min(box1.y + box1.height, box2.y + box2.height);

  int diff_width = x1 - x0;
  int diff_height = y1- y0;

  if (diff_width > 0 && diff_height > 0) {
    return sqrt(diff_width*diff_width + diff_height*diff_height);
  }
  else if (diff_width > 0) {
    return diff_width;
  }
  else if (diff_height > 0) {
    return diff_height;
  }
  return 0.;
}

cv::Mat calc_hist(cv::Mat& gray_img, cv::Rect det_box) {
  cv::Mat h1;
  int channels[] = {0}, histSize[]={256};
  float range[2] = {0, 255};
  const float *ranges[1];
  ranges[0] = range;
  cv::Mat img_t = gray_img(det_box);
  
  cv::calcHist(&img_t, 1, channels, cv::Mat(), h1, 1, histSize, ranges);
  return h1;
}

int draw_rotated_rect(cv::Mat &draw_mat, const std::vector<cv::RotatedRect> &rrects) {
	for (auto r_i : rrects) {
		cv::Point2f four_p[4];
		r_i.points(four_p);
		for (int i = 0; i < 4; i++) {
			cv::line(draw_mat, four_p[i % 4], four_p[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
		}
	}
	return 0;
}


float ratio_to_angle(float ratio_k)
{
	return atan(ratio_k) * 180 / CV_PI;
}
float angle_to_radians(float angle)
{
	return angle * CV_PI / 180;
}


int cv_image_encode(cv::Mat &cv_image, std::string &encoded)
{
  std::vector<uchar> image_buf;
  cv::imencode(".jpeg",cv_image, image_buf);
  // image_buf.push_back('\0');
  std::vector<uchar>::pointer pt = &image_buf[0];
  const uchar *tt = pt;

  encoded = base64_encode(tt, (unsigned int)image_buf.size());
	return 0;
}

int cv_image_decode(std::string &encoded, cv::Mat &cv_image, int color)
{
	std::string decode_string = base64_decode(encoded);
	
	std::vector<uchar> data(decode_string.begin(), decode_string.end());
  cv_image = cv::imdecode(data, color == 1 ? cv::IMREAD_COLOR:cv::IMREAD_GRAYSCALE);
	return 0;
}


int bgr2nv12(cv::Mat &src, cv::Mat &dst) {
  auto src_h = src.rows;
  auto src_w = src.cols;
  dst = cv::Mat(src_h * 3/2, src_w, CV_8UC1);
  cv::cvtColor(src, dst, cv::COLOR_BGR2YUV_I420);  // I420: YYYY...UU...VV...

  auto n_y = src_h * src_w;
  auto n_uv = n_y / 2;
  auto n_u = n_y / 4;
  std::vector<uint8_t> uv(n_uv);
  std::copy(dst.data+n_y, dst.data+n_y+n_uv, uv.data());
  for (auto i = 0; i < n_u; i++) {
    dst.data[n_y + 2*i] = uv[i];            // U
    dst.data[n_y + 2*i + 1] = uv[n_u + i];  // V
  }
  return 0;
}

int nv122bgr(std::vector<char>& src, cv::Size size, cv::Mat &dst) {
  auto src_w = size.width;
  auto src_h = size.height;
  cv::Mat src_img = cv::Mat(src_h * 3/2, src_w, CV_8UC1, src.data());
  cv::cvtColor(src_img, dst, cv::COLOR_YUV2BGR_NV12); 
  return 0;
}
