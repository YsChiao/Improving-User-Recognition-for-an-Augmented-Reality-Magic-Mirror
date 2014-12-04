////read the volume file from ReconstructMe and calcluate
////the girth of each layer.
////volume size 256x256x256
////vox size 2000/256
//
//
//#include <reconstructmesdk/reme.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <algorithm>    // std::min_element, std::max_element
//#include <stdlib.h>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//
//int thresh_output(int thresh,  Mat src)
//{
//	Mat canny_output;
//	RNG rng(12345);
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarchy;
//
//	// use Canndy detect contour
//	Canny(src, canny_output, thresh, thresh*2, 3);
//	// find contour
//	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
//
//	int *length = new int[contours.size()]; 
//
//	for( int i = 0; i< contours.size(); i++ )
//	{
//		length[i] = arcLength( contours[i], true );
//		//cout << " Length: " <<  length[i] << " ";
//	}
//	cout << "The length is "  << *max_element(length, length + contours.size()) << '\n';
//
//
//	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
//	for( int i = 0; i< contours.size(); i++ )
//	{
//		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//	}
//
//	/// Show in a window
//	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//	imshow( "Contours", drawing );
//
//	return(0);
//
//
//}
//
//int main (int argc, char** argv)
//{
//	// Create a new context
//	reme_context_t c;
//	reme_context_create(&c);
//
//	// Compile for OpenCL device using defaults
//	reme_context_compile(c);
//
//	// Create a new volume
//	reme_volume_t v;
//	reme_volume_create(c, &v);
//
//	// Retrieve num_slices
//	int num_slices;
//	reme_options_t o;
//	reme_options_create(c, &o);
//	reme_context_bind_reconstruction_options(c, o);
//
//	int resX, resY;
//	reme_options_get_int(c, o, "volume.resolution.x", &resX);
//	reme_options_get_int(c, o, "volume.resolution.y", &resY);
//	reme_options_get_int(c, o, "volume.resolution.z", &num_slices);
//	printf("Volume size in x-dim is: %i\n", resX);
//	printf("Volume size in y-dim is: %i\n", resY);
//	printf("Volume size in z-dim is: %i\n", num_slices);
//
//	const void *bytes;
//	int length;
//	int *load_buffer = 0;
//	Mat src, src_gray; int thresh = 100;
//	int load_buffer_length = 0;
//	int count = 0;
//
//	FILE *f = fopen("volume.bin", "rb");
//	for (int i = 0; i < num_slices; i++) 
//	{
//		fread(&length, sizeof(int), 1, f);
//
//		cout << length << " ";
//		if (load_buffer_length != length) 
//		{
//			if (load_buffer != 0) 
//			{
//				free(load_buffer);
//			}
//			load_buffer =  (int *)malloc(length);
//			load_buffer_length = length;
//		}
//		fread(load_buffer, 1, length, f);
//
//		cout << load_buffer[0] << " " <<  count << " ";
//		count ++;
//
//		Mat videoFrame(512, 512, CV_32SC1, load_buffer);
//		normalize(videoFrame, videoFrame, 0, 255, NORM_MINMAX, CV_8UC3);
//		namedWindow( "Display window", CV_WINDOW_AUTOSIZE );         // Create a window for display.
//		imshow( "Display window", videoFrame );                   // Show our image inside it.
//		waitKey(300);                                             // Wait for a keystroke in the window
//
//
//		//if (i == 64)
//		//{
//		//	src = videoFrame;
//		//}
//		// contour
//		blur( videoFrame, videoFrame, Size(3,3) );
//		thresh_output(thresh, videoFrame);
//
//
//	}
//	fclose(f);
//	return 0;
//}