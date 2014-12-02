#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/surface/mls.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <algorithm>    // std::min_element, std::max_element
#include <stdlib.h>
#include <iostream>
#include "grith.h"


using namespace cv;
using namespace std;

int thresh_output(int thresh,  Mat src)
{
	Mat canny_output;
	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// use Canndy detect contour
	Canny(src, canny_output, thresh, thresh*2, 3);

	// find contour
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Find the convex hull object for each contour
	vector<vector<Point> >hull( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{  convexHull( Mat(contours[i]), hull[i], false ); }

	int *length = new int[contours.size()]; 

	for( int i = 0; i< contours.size(); i++ )
	{
		length[i] = arcLength( contours[i], true );
		//cout << " Length: " <<  length[i] << " ";
	}

	cout << "The length is "  << *max_element(length, length + contours.size()) << '\n';
	int max_position = distance(length, max_element(length, length + contours.size()));

	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		if (i ==max_position)
		{
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
			//drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
		}
	}

	//// Show in a window
	//namedWindow( "Contours", 1);
	//imshow( "Contours", drawing );
	//waitKey();
	return(0);

}

int Slice (pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filter, int layer)
{
	// Build a filter to remove spurious NaNs
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud (cloud);
	pass.setFilterFieldName ("z");
	pass.setFilterLimits (layer, layer+7.8);
	pass.filter (*cloud_filter);


	// Testing upsampling
	pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointXYZ> mls_upsampling;
	// Set parameters
	mls_upsampling.setInputCloud (cloud_filter);
	mls_upsampling.setSearchRadius (200);
	mls_upsampling.setPolynomialFit (true);
	mls_upsampling.setPolynomialOrder (2);

	mls_upsampling.setUpsamplingMethod (pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointXYZ>::SAMPLE_LOCAL_PLANE);
	mls_upsampling.setUpsamplingRadius (10);
	mls_upsampling.setUpsamplingStepSize (1);

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_smoothed (new pcl::PointCloud<pcl::PointXYZ> ());
	mls_upsampling.process (*cloud_smoothed);

	//pcl::visualization::CloudViewer viewerU("Upsampling Cloud Viewer");
	//viewerU.showCloud(cloud_smoothed);
	//while (!viewerU.wasStopped()){}


	std::cerr << "PointCloud after filtering has: " << cloud_smoothed->points.size () << " data points." << std::endl;

	//pcl::visualization::CloudViewer viewer("Simple Cloud Viewer1");
	//viewer.showCloud(cloud);
	//while (!viewer.wasStopped()){}
	//pcl::visualization::CloudViewer viewer2("Simple Cloud Viewer2");
	//viewer2.showCloud(cloud_smoothed);
	//while (!viewer2.wasStopped()){}

	//for (int i = 0; i < cloud_smoothed->points.size(); i++) 
	//{ 
	//	cout << cloud_smoothed->points[i].x <<" "<<
	//		cloud_smoothed->points[i].y <<" "<<
	//		cloud_smoothed->points[i].z <<endl;
	//}


	Mat img = Mat::zeros(2000, 2000, CV_8U);

	for (int i = 0; i < cloud_smoothed->points.size(); i++) 
	{ 
		int x = (int)cloud_smoothed->points[i].x+1000;
		int y = -(int)cloud_smoothed->points[i].y+1000;

		img.at<uchar>(y, x) = 255;
	}

	resize(img,img,Size(512,512),0,0,CV_INTER_LINEAR);

	//namedWindow( "image", 1);
	//imshow("image", img);
	////waitKey();

	int thresh = 1;
	// contour
	blur( img, img, Size(3,3) );
	thresh_output(thresh, img);

	return (0);

}




//int main (int argc, char** argv)
int grith(string path, double hip, double torso, double neck)
{    
	//string path = "test.ply";
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>), 
		cloud_hip  (new pcl::PointCloud<pcl::PointXYZ>),
		cloud_waist  (new pcl::PointCloud<pcl::PointXYZ>),
		cloud_bust  (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PLYReader reader;
	reader.read (path, *cloud);

	Slice (cloud,cloud_hip, hip);
	Slice (cloud,cloud_waist, torso-100);
	Slice (cloud,cloud_bust, neck-200);
	cout << "girth of body done" << endl;

	return(0);
	
}