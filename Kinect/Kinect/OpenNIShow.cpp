//unsigned int __stdcall OpenNIShow (PVOID pM)
//{
//	// create depth stream   
//	OPENNI_ASSERT(oniDepthStream.create( device, openni::SENSOR_DEPTH ));
//
//	cout << "1" << endl;
//
//	// set depth video mode  
//	VideoMode modeDepth;  
//	modeDepth.setResolution( 640, 480 );  
//	modeDepth.setFps( 30 );  
//	modeDepth.setPixelFormat( PIXEL_FORMAT_DEPTH_1_MM );  
//	oniDepthStream.setVideoMode(modeDepth);  
//	// start depth stream  
//	OPENNI_ASSERT(oniDepthStream.start());  
//
//	// create color stream   
//	OPENNI_ASSERT(oniColorStream.create( device, openni::SENSOR_COLOR ));  
//	// set color video mode  
//	VideoMode modeColor;  
//	modeColor.setResolution( 640, 480 );  
//	modeColor.setFps( 30 );  
//	modeColor.setPixelFormat( PIXEL_FORMAT_RGB888 );  
//	oniColorStream.setVideoMode( modeColor);  
//	// start color stream  
//	OPENNI_ASSERT(oniColorStream.start());
//
//	
//
//	// Since we are using an external sensor, we need to tell ReMe about its image size field of view.
//	reme_sensor_bind_camera_options(c, s, o);
//	int image_width = oniDepthStream.getVideoMode().getResolutionX();
//	int image_height = oniDepthStream.getVideoMode().getResolutionY();
//	reme_options_set_int(c, o, "depth_stream.image_size.width", image_width);
//	reme_options_set_int(c, o, "depth_stream.image_size.height", image_height);
//	// The following intrinsics are based on a Kinect like device. By providing
//	// intrinsics.width and intrinsics.height ReMe is able to automatically derive intrinsics
//	// for different stream sizes.
//	reme_options_set_int(c, o, "depth_stream.intrinsics.width", 640);
//	reme_options_set_int(c, o, "depth_stream.intrinsics.height", 480);
//	reme_options_set_real(c, o, "depth_stream.intrinsics.fx", 530);
//	reme_options_set_real(c, o, "depth_stream.intrinsics.fy", 530);
//	reme_options_set_real(c, o, "depth_stream.intrinsics.cx", 320);
//	reme_options_set_real(c, o, "depth_stream.intrinsics.cy", 240);
//	// Open the sensor like any other sensor.
//	reme_sensor_open(c, s);
//	reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT);
//
//	// In order inform ReMe about external sensor data
//	reme_image_t raw_depth, raw_image;
//	reme_image_create(c, &raw_depth);
//	reme_image_create(c, &raw_image);
//
//
//	//// Create a viewer
//	//reme_viewer_t viewer;
//	//reme_viewer_create_image(c, "This is ReconstructMeSDK", &viewer);
//
//	reme_image_t images_to_show[2];
//	reme_image_create(c, &images_to_show[0]);
//	reme_image_create(c, &images_to_show[1]);
//	//reme_viewer_add_image(c, viewer, images_to_show[0]);
//	//reme_viewer_add_image(c, viewer, images_to_show[1]);
//
//	// Create Mat
//	const void *data;
//	int width, height, length;
//
//	// Perform the Video and show on the diplay
//	bool viewer_done = false;
//	while (!viewer_done) 
//	{
//		// Grab from the real sensor
//		int index;
//		openni::VideoStream* streams[] = {&oniDepthStream, &oniColorStream};
//		OPENNI_ASSERT(openni::OpenNI::waitForAnyStream(streams, 2, &index, 500));
//		OPENNI_ASSERT(oniDepthStream.readFrame(&oniDepthImg));
//		OPENNI_ASSERT(oniColorStream.readFrame(&oniColorImg));
//		const unsigned short *depth_pixel  = (const unsigned short *)oniDepthImg.getData();
//		const unsigned short *color_pixel  = (const unsigned short *)oniColorImg.getData();
//
//		// ... Potentially modify sensor input before passing to ReMe
//		// Get hold of raw depth image reference provded by ReMe.
//		reme_sensor_grab(c, s);
//		reme_sensor_prepare_image(c, s, REME_IMAGE_RAW_AUX );
//		reme_sensor_get_image(c, s, REME_IMAGE_RAW_AUX , raw_image);
//		reme_sensor_prepare_image(c, s, REME_IMAGE_RAW_DEPTH);
//		reme_sensor_get_image(c, s, REME_IMAGE_RAW_DEPTH, raw_depth);
//
//		void *virtual_pixel, *virtual_pixel2;
//		int nbytes,nbytes2; 
//		reme_image_get_mutable_bytes(c, raw_image, &virtual_pixel2, &nbytes2);
//		reme_image_get_mutable_bytes(c, raw_depth, &virtual_pixel, &nbytes);
//		// Copy content   
//		memcpy(virtual_pixel, depth_pixel, image_width * image_height * sizeof(unsigned short));
//		memcpy(virtual_pixel2, color_pixel, image_width * image_height * sizeof(unsigned short));
//
//		// Update the viewer
//		//reme_sensor_get_image(c, s, REME_IMAGE_AUX, images_to_show[0]);
//		//reme_sensor_get_image(c, s, REME_IMAGE_DEPTH, images_to_show[1]);
//
//
//		// Load to Mat
//		// Receive the pointer to the data
//		reme_image_get_bytes(c, images_to_show[0], &data, &length);
//		// Derive the correct image format
//		reme_image_get_info(c, images_to_show[0], &width, &height);
//		// In C++ we could now feed this to OpenCV for visualization
//		Mat rgb(height, width, CV_8UC3, (char*)data);
//		cvtColor(rgb, cameraFrame, CV_RGB2BGR);
//		cv::imshow("Main UI video", cameraFrame);
//		cv::waitKey(10);
//
//
//		//reme_viewer_update(c, viewer);
//		//reme_viewer_is_closed(c, viewer, &viewer_done);
//
//
//	}
//	return 0;
//}