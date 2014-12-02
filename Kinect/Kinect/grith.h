#ifndef GIRTH_H_
#define GIRTH_H_

#include <pcl/point_types.h>
#include <opencv2\core\core.hpp>

int thresh_output(int thresh,  cv::Mat src);
int Slice (pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filter, int layer);
int grith(string path, double hip, double torso, double neck);

#endif