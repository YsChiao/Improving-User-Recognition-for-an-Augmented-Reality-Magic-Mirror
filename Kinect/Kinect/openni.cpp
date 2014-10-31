//#include <stdlib.h>
//#include <iostream>
//#include <string>
//#include <exception>
//#include <OpenNI.h>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/core.hpp>
//#include <reconstructmesdk/reme.h>
//
//using namespace std;
//using namespace cv;
//using namespace openni;
//
//// Test for OpenNI errors
//#define OPENNI_ASSERT(expr) \
//    if (!(expr) == openni::STATUS_OK) \
//        throw bad_exception(openni::OpenNI::getExtendedError());
//
//
//int main (int argc, char** argv)
//{
//	//OpenNI2 image   
//    VideoFrameRef oniColorImg;
//	VideoFrameRef oniDepthImg;
//
//	// initialize OpenNI2  
//    OPENNI_ASSERT(OpenNI::initialize());   
//  
//    // open device    
//    Device device;  
//    OPENNI_ASSERT(device.open( openni::ANY_DEVICE ));  
//       
//    // create depth stream   
//    VideoStream oniDepthStream;  
//    OPENNI_ASSERT(oniDepthStream.create( device, openni::SENSOR_DEPTH ));  
//
//	// set depth video mode  
//    VideoMode modeDepth;  
//    modeDepth.setResolution( 640, 480 );  
//    modeDepth.setFps( 30 );  
//    modeDepth.setPixelFormat( PIXEL_FORMAT_DEPTH_1_MM );  
//    oniDepthStream.setVideoMode(modeDepth);  
//    // start depth stream  
//    OPENNI_ASSERT(oniDepthStream.start());  
//
//	 // create color stream  
//    VideoStream oniColorStream;  
//    OPENNI_ASSERT(oniColorStream.create( device, openni::SENSOR_COLOR ));  
//    // set color video mode  
//    VideoMode modeColor;  
//    modeColor.setResolution( 640, 480 );  
//    modeColor.setFps( 30 );  
//    modeColor.setPixelFormat( PIXEL_FORMAT_RGB888 );  
//    oniColorStream.setVideoMode( modeColor);  
//
//    // start color stream  
//	OPENNI_ASSERT(oniColorStream.start()); 
//
//	 // Create ReconstructMe context
//    reme_context_t c;
//    reme_context_create(&c);
//    reme_context_compile(c);
//    reme_options_t o;
//    reme_options_create(c, &o);
//    // Create external sensor.
//    reme_sensor_t s;
//    reme_sensor_create(c, "external", false, &s);
//
//	// Since we are using an external sensor, we need to tell ReMe about its image size field of view.
//    reme_sensor_bind_camera_options(c, s, o);
//	int image_width = oniDepthStream.getVideoMode().getResolutionX();
//	int image_height = oniDepthStream.getVideoMode().getResolutionY();
//    reme_options_set_int(c, o, "depth_stream.image_size.width", image_width);
//    reme_options_set_int(c, o, "depth_stream.image_size.height", image_height);
//    
//    // The following intrinsics are based on a Kinect like device. By providing
//    // intrinsics.width and intrinsics.height ReMe is able to automatically derive intrinsics
//    // for different stream sizes.
//    reme_options_set_int(c, o, "depth_stream.intrinsics.width", 640);
//    reme_options_set_int(c, o, "depth_stream.intrinsics.height", 480);
//    reme_options_set_real(c, o, "depth_stream.intrinsics.fx", 530);
//    reme_options_set_real(c, o, "depth_stream.intrinsics.fy", 530);
//    reme_options_set_real(c, o, "depth_stream.intrinsics.cx", 320);
//    reme_options_set_real(c, o, "depth_stream.intrinsics.cy", 240);
//
//	// Open the sensor like any other sensor.
//    reme_sensor_open(c, s);
//    reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT);
//
//	// In order inform ReMe about external sensor data
//	reme_image_t raw_depth, raw_image;
//	reme_image_create(c, &raw_depth);
//	reme_image_create(c, &raw_image);
//
//    // Create a viewer
//    reme_viewer_t viewer;
//    reme_viewer_create_image(c, "This is ReconstructMe SDK", &viewer);
// 
//    reme_image_t images_to_show[2];
//    reme_image_create(c, &images_to_show[0]);
//    reme_image_create(c, &images_to_show[1]);
//    reme_viewer_add_image(c, viewer, images_to_show[0]);
//    reme_viewer_add_image(c, viewer, images_to_show[1]);
//
//	// Perform reconstruction while viewer is not closed.
//    bool viewer_done = false;
//    while (!viewer_done) 
//    {
//        // Grab from the real sensor
//        int index;
//        openni::VideoStream* streams[] = {&oniDepthStream};
//        OPENNI_ASSERT(openni::OpenNI::waitForAnyStream(streams, 1, &index, 500));
//        OPENNI_ASSERT(oniDepthStream.readFrame(&oniDepthImg));
//		OPENNI_ASSERT(oniColorStream.readFrame(&oniColorImg));
//        const unsigned short *depth_pixel  = (const unsigned short *)oniDepthImg.getData();
//		const unsigned short *color_pixel  = (const unsigned short *)oniColorImg.getData();
//
//        // ... Potentially modify sensor input before passing to ReMe
//        // Get hold of raw depth image reference provded by ReMe.
//        reme_sensor_grab(c, s);
//        reme_sensor_prepare_image(c, s, REME_IMAGE_RAW_AUX );
//        reme_sensor_get_image(c, s, REME_IMAGE_RAW_AUX , raw_image);
//		reme_sensor_prepare_image(c, s, REME_IMAGE_RAW_DEPTH);
//		reme_sensor_get_image(c, s, REME_IMAGE_RAW_DEPTH, raw_depth);
//
//        void *virtual_pixel, *virtual_pixel2;
//        int nbytes,nbytes2; 
//		reme_image_get_mutable_bytes(c, raw_image, &virtual_pixel2, &nbytes2);
//		reme_image_get_mutable_bytes(c, raw_depth, &virtual_pixel, &nbytes);
//        // Copy content   
//        memcpy(virtual_pixel, depth_pixel, image_width * image_height * sizeof(unsigned short));
//		memcpy(virtual_pixel2, color_pixel, image_width * image_height * sizeof(unsigned short));
//
//        // Update the viewer
//        reme_sensor_get_image(c, s, REME_IMAGE_AUX, images_to_show[0]);
//        reme_sensor_get_image(c, s, REME_IMAGE_DEPTH, images_to_show[1]);
//        reme_viewer_update(c, viewer);
//        reme_viewer_is_closed(c, viewer, &viewer_done);
//    }
//
//	reme_context_print_errors(c);
//    reme_context_destroy(&c);
//
//	//OpenNI2 destroy  
//    oniDepthStream.destroy();  
//    oniColorStream.destroy();  
//    device.close();  
//    OpenNI::shutdown(); 
//	return(0);
//
//}
//
//
//
