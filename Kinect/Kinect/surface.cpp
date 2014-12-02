//#include <reconstructmesdk\reme.h>
//#include <iostream>
//
//using namespace std;
//
//int main (int argc, char ** argv)
//{
//    reme_context_t c;
//    reme_context_create(&c);
//    //reme_context_set_log_callback(c, reme_default_log_callback, 0);
//
//	reme_options_t o;
//	reme_options_create(c, &o);
//
//	// Choose the OpenCL device
//	reme_context_bind_reconstruction_options(c, o);
//	reme_options_set_int(c, o, "device_id", 0);
//
//	// Compile for OpenCL device using defaults
//	reme_options_set_int(c, o, "volume.minimum_corner.x", -1000);
//	reme_options_set_int(c, o, "volume.minimum_corner.y", -1000);
//	reme_options_set_int(c, o, "volume.minimum_corner.z", -1000);
//	reme_options_set_int(c, o, "volume.maximum_corner.x", 1000);
//	reme_options_set_int(c, o, "volume.maximum_corner.y", 1000);
//	reme_options_set_int(c, o, "volume.maximum_corner.z", 1000);
//	reme_options_set_int(c, o, "volume.resolution.x", 256);
//	reme_options_set_int(c, o, "volume.resolution.y", 256);
//	reme_options_set_int(c, o, "volume.resolution.z", 256);
//
//	// Compile for OpenCL device using defaults
//	reme_context_tune_reconstruction_options(c, REME_TUNE_PROFILE_LOW_QUALITY);
//	reme_context_compile(c);
//    
//
//	// Create a new volume
//    reme_volume_t v;
//    reme_volume_create(c, &v);
//    
//	// Create a new sensor. Tries multiple backends using default
//    // sensor configurations, returns the first working one. By default
//    // each sensor works on the first volume created.
//    reme_sensor_t s;
//    reme_sensor_create(c, "openni;mskinect;file", true, &s);
//    reme_sensor_open(c, s);
//
//	//reme_sensor_grab(c, s);
//	//reme_sensor_prepare_images(c, s);
//    //reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT );
//    
//	// For debugging purposes open a viewer for tracking the reconstruction process.
//    reme_viewer_t viewer;
//    reme_viewer_create_image(c, "This is ReconstructMe SDK", &viewer);
//    reme_image_t volume, aux;
//    reme_image_create(c, &volume);
//    reme_image_create(c, &aux);
//    reme_viewer_add_image(c, viewer, aux);
//	reme_viewer_add_image(c, viewer, volume);
//
//    // Reconstruct until viewer is closed, or grabbing fails. Note that
//    // aborting reconstruction just because a single grab failed is not good stlye.
//    // You should allow for multiple grab attempts.
//    printf("Starting reconstruction. Close the viewer window to stop.\n");
//    bool viewer_done = false;
//	float coordinates[16];
//	int count = 0;
//	reme_sensor_grab(c, s);
//	reme_sensor_prepare_images(c, s);
//	//reme_sensor_find_floor(c, s, coordinates);
//	while (!REME_SUCCESS(reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_FLOOR))) {
//		count ++;
//		reme_sensor_grab(c, s);
//		reme_sensor_prepare_images(c, s); 
//		cout << count << endl;
//	}
//
//	while (!viewer_done && REME_SUCCESS(reme_sensor_grab(c, s))) {
//        // Prepare image and depth data
//        reme_sensor_prepare_images(c, s);
//		/*reme_sensor_find_floor(c, s, coordinates);
//
//		cout << coordinates[0] <<" "<< coordinates[1] <<" "<< coordinates[2] <<" "<< coordinates[3] <<" "<<endl
//			 << coordinates[4] <<" "<< coordinates[5] <<" "<< coordinates[6] <<" "<< coordinates[7] <<" "<<endl
//			 << coordinates[8] <<" "<< coordinates[9] <<" "<< coordinates[10] <<" "<< coordinates[11] <<" "<<endl
//			 << coordinates[12] <<" "<< coordinates[13] <<" "<< coordinates[14] <<" "<< coordinates[15] <<" "<<endl<<endl;
//		if (coordinates[15] == 1)
//		{reme_sensor_set_position(c, s, &coordinates[0]);}*/
//
//        // Try to determine updated sensor position.
//        // On succes, update volume, otherwise move to a recovery position
//        // and wait for the tracking to start again.
//        if (REME_SUCCESS(reme_sensor_track_position(c, s))) {
//            // Update volume with depth data from the
//            // current sensor perspective
//            reme_sensor_update_volume(c, s);
//        }
//        // Update the viewer
//        reme_sensor_get_image(c, s, REME_IMAGE_AUX, aux);
//        reme_sensor_get_image(c, s, REME_IMAGE_VOLUME, volume);
//        reme_viewer_update(c, viewer);
//        reme_viewer_is_closed(c, viewer, &viewer_done);
//    }
//    
//	// Close and destroy the sensor, it is not needed anymore
//    reme_sensor_close(c, s);
//    reme_sensor_destroy(c, &s);
//    
//	// Create a new surface
//    reme_surface_t m;
//    reme_surface_create(c, &m);
//    reme_surface_generate(c, m, v);
//    reme_surface_save_to_file(c, m, "testY.ply");
//
//
//	// Retrieve num_slices
//	int num_slices;
//	//OK(reme_context_bind_reconstruction_options(c, o));
//	reme_options_get_int(c, o, "volume.resolution.z", &num_slices);
//	const void *bytes;
//	int length;
//
//
//	// saves the volume content to disk
//	FILE *f = fopen("volumeY.bin", "wb");
//	for (int i = 0; i < num_slices; i++) 
//	{
//		reme_volume_slice_get_bytes(c, v, i, &bytes, &length);
//		fwrite(&length, sizeof(int), 1, f);
//		fwrite(bytes, 1, length, f);            
//	}
//	fclose(f);
//    
//	// Visualize resulting surface
//    reme_viewer_t viewer_surface;
//    reme_viewer_create_surface(c, m, "This is ReconstructMeSDK", &viewer_surface);
//    reme_viewer_wait(c, viewer_surface);
//    reme_surface_destroy(c, &m);
//    
//	// Print pending errors
//    reme_context_print_errors(c);
//    
//	// Make sure to release all memory acquired
//    reme_context_destroy(&c);
//
//
//}