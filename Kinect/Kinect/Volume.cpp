#include <reconstructmesdk/reme.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

int main (int argc, char** argv)
{
	// Create a new context
	reme_context_t c;
	reme_context_create(&c);

	// Compile for OpenCL device using defaults
	reme_context_compile(c);

	// Create a new volume
	reme_volume_t v;
	reme_volume_create(c, &v);

	// Retrieve num_slices
	int num_slices;
	reme_options_t o;
	reme_options_create(c, &o);
	reme_context_bind_reconstruction_options(c, o);

	int resX, resY;
    reme_options_get_int(c, o, "volume.resolution.x", &resX);
	reme_options_get_int(c, o, "volume.resolution.y", &resY);
    reme_options_get_int(c, o, "volume.resolution.z", &num_slices);
    printf("Volume size in x-dim is: %i\n", resX);
	printf("Volume size in y-dim is: %i\n", resY);
	printf("Volume size in z-dim is: %i\n", num_slices);

	 // Create a new openni sensor using default intrinsics
    reme_sensor_t s;
    reme_sensor_create(c, "openni;mskinect;file", true, &s);
    reme_sensor_open(c, s);
    reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT);

	const void *bytes;
	int length;
	int *load_buffer = 0;
	int *data =  (int *)malloc(256); 
	int load_buffer_length = 0;
	int count = 0;

	FILE *f = fopen("volume.bin", "rb");
	for (int i = 0; i < num_slices; i++) 
	{
		fread(&length, sizeof(int), 1, f);
		//fread(&length, 1, sizeof(int), f);

		cout << length << " ";
		if (load_buffer_length != length) 
		{
			if (load_buffer != 0) 
			{
				free(load_buffer);
			}
			load_buffer =  (int *)malloc(length);
			load_buffer_length = length;
		}
		fread(load_buffer, 1, length, f);
		
		//reme_volume_slice_set_bytes(c , v, i, load_buffer, length);
		//reme_sensor_update_volume(c, s);
		
		
		cout << load_buffer[57727] << " " <<  count << endl;
		count ++;
		
		Mat videoFrame(256, 256, CV_32SC1, load_buffer);
		normalize(videoFrame, videoFrame, 0, 255, NORM_MINMAX, CV_8UC1);
		namedWindow( "Display window", WINDOW_AUTOSIZE );         // Create a window for display.
		imshow( "Display window", videoFrame );                   // Show our image inside it.
		waitKey(100);                                             // Wait for a keystroke in the window
	}
	fclose(f);
	
	return 0;
}