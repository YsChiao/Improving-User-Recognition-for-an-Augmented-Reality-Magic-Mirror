//#include <reconstructmesdk/reme.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <stdlib.h>
//#include <iostream>
//#include <conio.h>
//
//using namespace cv;
//using namespace std;
//
//
//int main (int argc, char** argv)
//{
//	puts("Key mapping");
//	puts("x: quit scanning");
//	puts("r: reset volume to empty state");
//	puts("s: save volume to disk");
//	puts("l: load volume from disk and resume");
//	puts("All keys need to be pressed with the command line in the foreground");
//	// Create a new context
//	reme_context_t c;
//	reme_context_create(&c);
//	// Compile for OpenCL device using defaults
//	reme_context_compile(c);
//	// Create a new volume
//	reme_volume_t v;
//	reme_volume_create(c, &v);
//	// Retrieve num_slices
//	int num_slices;
//	reme_options_t o;
//	reme_options_create(c, &o);
//	reme_context_bind_reconstruction_options(c, o);
//	reme_options_get_int(c, o, "volume.resolution.z", &num_slices);
//	// Create a new openni sensor using default intrinsics
//	reme_sensor_t s;
//	reme_sensor_create(c, "openni;mskinect;file", true, &s);
//	reme_sensor_open(c, s);
//	reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT);
//	reme_viewer_t viewer;
//	reme_viewer_create_image(c, "This is ReconstructMeSDK", &viewer);
//	reme_image_t imgs[2];
//	reme_image_create(c, &imgs[0]);
//	reme_image_create(c, &imgs[1]);
//	reme_viewer_add_image(c, v, imgs[0]);
//	reme_viewer_add_image(c, v, imgs[1]);
//	const void *bytes;
//	int length;
//	void *load_buffer = 0;
//	int load_buffer_length = 0;
//	bool continue_scanning = true;
//	while (continue_scanning && REME_SUCCESS(reme_sensor_grab(c, s))) {
//		// Keyboard handling
//		if (_kbhit()) {
//			char k = _getch();
//			switch (k) {
//				// Key 'r' resets the volume to empty state
//			case 'r': {
//				reme_volume_reset(c, v);
//				reme_sensor_reset(c, s);
//				break;
//					  }
//					  // Key 's' saves the volume content to disk
//			case 's' : {
//				FILE *f = fopen("volume.bin", "wb");
//				for (int i = 0; i < num_slices; i++) {
//					reme_volume_slice_get_bytes(c, v, i, &bytes, &length);
//					fwrite(&length, sizeof(int), 1, f);
//					fwrite(bytes, 1, length, f);
//				}
//				fclose(f);
//				break;
//					   }
//					   // Key 'l' loads the volume content from disk
//			case 'l': {
//				FILE *f = fopen("volume.bin", "rb");
//				for (int i = 0; i < num_slices; i++) {
//					fread(&length, sizeof(int), 1, f);
//					if (load_buffer_length != length) {
//						if (load_buffer != 0) {
//							free(load_buffer);
//						}
//						load_buffer = malloc(length);
//						load_buffer_length = length;
//					}
//					fread(load_buffer, 1, length, f);
//					reme_volume_slice_set_bytes(c , v, i, load_buffer, length);
//				}
//				fclose(f);
//				if (load_buffer != 0) {
//					free(load_buffer);
//					load_buffer = 0;
//					load_buffer_length = 0;
//				}
//				reme_sensor_reset(c, s);
//				break;
//					  }
//			case 'x': {
//				continue_scanning = false;
//				break;
//					  }
//			} // end switch
//		} // end if kbhit()
//		// Prepare image and depth data
//		reme_sensor_prepare_images(c, s);
//		// Try to determine updated sensor position by
//		// matching current data against volume content
//		if (REME_SUCCESS(reme_sensor_track_position(c, s))) {
//			// Update volume with depth data from the
//			// current sensor perspective
//			reme_sensor_update_volume(c, s);
//		}
//		// Update the viewer
//		reme_sensor_get_image(c, s, REME_IMAGE_DEPTH, imgs[0]);
//		reme_sensor_get_image(c, s, REME_IMAGE_VOLUME, imgs[1]);
//		reme_viewer_update(c, viewer);
//	}
//	// Print pending errors
//	reme_context_print_errors(c);
//	// Make sure to release all memory acquired
//	reme_context_destroy(&c);
//
//}