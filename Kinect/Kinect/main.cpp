//#include <openbr/openbr_plugin.h>
//#include <reconstructmesdk/reme.h>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//#include <conio.h>
//#include <windows.h>
//#include <NiTE.h>
//
//
//// A simple macro that checks for any errors and exits if a method invocation failed.
//#define OK(s)                                       \
//  if (s != REME_ERROR_SUCCESS) {                    \
//    reme_context_print_errors(c);                   \
//    reme_context_destroy(&c);                       \
//    exit(-1);                                       \
//  }                                                 \
// 
//// using namespace
//using namespace std;
//using namespace cv;
//
//static void printTemplateGender(const br::Template &t)
//{
//    printf("%s gender: %s\n", qPrintable(t.file.fileName()), qPrintable(t.file.get<QString>("Gender")));
//}
//
//static void printTemplateAge(const br::Template &t)
//{
//	printf("%s age: %d\n", qPrintable(t.file.fileName()), int(t.file.get<float>("Age")));
//}
// 
//int UserDetection()
//{
//  // initialize NiTE
//  nite::NiTE::initialize();
// 
//  // create user tracker
//  nite::UserTracker mUserTracker;
//  mUserTracker.create();
// 
//  nite::UserTrackerFrameRef mUserFrame;
//  while(true)
//  {
//    // get user frame
//    mUserTracker.readFrame( &mUserFrame );
// 
//    // get users' data
//    const nite::Array<nite::UserData>& aUsers = mUserFrame.getUsers();
//    for( int i = 0; i < aUsers.getSize(); ++ i )
//    {
//		const nite::UserData& rUser = aUsers[i];
//        if( rUser.isNew() )
//		{
//			cout << "New User [" << rUser.getId() << "] found." << endl;
//		    //Sleep(5000);
//		    goto end;
//	    }
//		if( rUser.isLost() )
//			cout << "User [" << rUser.getId()  << "] lost." << endl;
//    }
//  }
//  end: cout << "Detection End" << endl;
//  nite::NiTE::shutdown();
// 
//  return (0);
//}
//
//int OpenBR(int argc, char *argv[], Mat img)
//{
//    br::Context::initialize(argc, argv);
//	
//    // Retrieve class for enrolling templates using the GenderEstimation algorithm
//    QSharedPointer<br::Transform> transformGender = br::Transform::fromAlgorithm("GenderEstimation");
//	QSharedPointer<br::Transform> transformAge = br::Transform::fromAlgorithm("AgeEstimation");
//	
//    // Initialize templates
//    br::Template queryGender(img);
//	br::Template queryAge(img);
//	
//
//    // Enroll templates
//    queryGender >> *transformGender;
//    queryAge >> *transformAge;
//	
//    printTemplateGender(queryGender);
//    printTemplateAge(queryAge);
//	
//    br::Context::finalize();
//    return 0;
//}
//
//int main (int argc, char** argv)
//{
//    // detect the user within the system
//    UserDetection();
//	
//	// Create a new context
//	reme_context_t c;
//	OK(reme_context_create(&c));
//	
//
//	// Create a new sensor. Tries multiple backends using default
//    // sensor configurations, returns the first working one. By default
//    // each sensor works on the first volume created.
//    reme_sensor_t s;
//    OK(reme_sensor_create(c, "openni;mskinect;file", true, &s));
//
//    reme_sensor_open(c, s);
//    reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT);
//
//	// Create an image to detect gender and age
//	reme_image_t i;
//	reme_image_create(c, &i);
//	const void *data;
//	int width, height, length;
//	Mat image;
//	bool count = true;
//
//	while (REME_SUCCESS(reme_sensor_grab(c, s))) {
//        // Update internal state of images
//        reme_sensor_prepare_image(c, s, REME_IMAGE_AUX);
//        // Receive the image into our image handle
//        reme_sensor_get_image(c, s, REME_IMAGE_AUX, i);
//        // Receive the pointer to the data
//        reme_image_get_bytes(c, i, &data, &length);
//        // Derive the correct image format
//        reme_image_get_info(c, i, &width, &height);
//        // In C++ we could now feed this to OpenCV for visualization
//        Mat rgb(height, width, CV_8UC3, (char*)data);
//        Mat bgr;
//
//
//		//Sleep(5000);
//	    cvtColor(rgb, image, CV_RGB2BGR); 
//	    cout << "Loading image ......"<<endl;
//	    break;
//	}
//
//
//	cout << "Strating Gender and Age detection." << endl;
//	OpenBR(argc, argv, image);
//	cout << "Gender and Age detection is done." << endl;
//	cout << "Starting Reconstruction." << endl;
////-------------------------------------------------------------//
//	 // Create empty options binding
//	reme_context_set_log_callback(c, reme_default_log_callback, 0);
//
//    reme_options_t o, o_sub;
//    reme_options_create(c, &o);
//	reme_options_create(c, &o_sub);
//
//    // Bind to OpenCL infos
//    // See method documentation for protocol buffer specification
//    reme_context_bind_opencl_info(c, o);
//    // See how many devices are available
//    int num_devices = 0;
//    reme_options_get_repeated_count(c, o, "devices", &num_devices);
//    printf("Found %i OpenCL compatible devices\n", num_devices);
//    // Each device is a nested message. Iterate over all of them
//    char name[256], vendor[256], type[256];
//    for (int i = 0; i < num_devices; i += 1) {
//        printf("----\n");
//        reme_options_bind_repeated_message(c, o, "devices", i, o_sub);
//        reme_options_get(c, o_sub, "name", name, 256);
//        reme_options_get(c, o_sub, "vendor", vendor, 256);
//        reme_options_get(c, o_sub, "type", type, 256);
//        printf("Device '%s' \n", name);
//        printf(" - by '%s' \n", vendor);
//        printf(" - type %s \n", type);
//    }
//	// Choose the OpenCL device
//	reme_context_bind_reconstruction_options(c, o);
//	reme_options_set_int(c, o, "device_id", 0);
//	
//	// Compile for OpenCL device using defaults
//	reme_context_tune_reconstruction_options(c, REME_TUNE_PROFILE_MID_QUALITY);
//    OK(reme_context_compile(c));
//
//	// Create a new volume
//    reme_volume_t v;
//    OK(reme_volume_create(c, &v));
//
//    // For debugging purposes open a viewer for tracking the reconstruction process.
//    reme_viewer_t viewer;
//    reme_viewer_create_image(c, "This is ReconstructMe SDK", &viewer);
//
//	
//	reme_image_t volume, aux;
//	reme_image_create(c, &volume);
//	reme_image_create(c, &aux);
//
//	reme_viewer_add_image(c, viewer, aux);
//	reme_viewer_add_image(c, viewer, volume);
//
//	// Perform reconstruction until one complete rotation is performed
//	float prev_pos[16], cur_pos[16];
//	float rotation_axis[4] = {0,0,1,0};
//	reme_sensor_get_position(c, s, prev_pos);
//
//	float angle;
//	float sum_turn_angles = 0.f;
//
//	while(fabs(sum_turn_angles) < 2.f * 3.1415f) {
//		if (!REME_SUCCESS(reme_sensor_grab(c,s))) {
//			continue;
//		}
//
//		reme_sensor_prepare_images(c, s);
//
//		if (REME_SUCCESS(reme_sensor_track_position(c, s))) {
//			reme_sensor_update_volume(c, s);
//            reme_sensor_get_position(c, s, cur_pos);
//            reme_transform_get_projected_angle(c, rotation_axis, prev_pos, cur_pos, &angle);
//            sum_turn_angles += angle;
//            memcpy(prev_pos, cur_pos, 16 * sizeof(float));
//        }
//        reme_sensor_get_image(c, s, REME_IMAGE_AUX, aux);
//        reme_sensor_get_image(c, s, REME_IMAGE_VOLUME, volume);
//        reme_viewer_update(c, viewer);
//	}
//
//	// Close and destroy the sensor, it is not needed anymore
//    reme_sensor_close(c, s);
//    reme_sensor_destroy(c, &s);
//
//	// Create a new surface
//    reme_surface_t m;
//    reme_surface_create(c, &m);
//    reme_surface_generate(c, m, v);
//    reme_surface_save_to_file(c, m, "test.ply");
//
//	// Visualize resulting surface
//    reme_viewer_t viewer_surface;
//    reme_viewer_create_surface(c, m, "This is ReconstructMeSDK", &viewer_surface);
//    reme_viewer_wait(c, viewer_surface);
//
//	//reme_surface_destroy(c, &m);
//
//	// Print pending errors
//    reme_context_print_errors(c);
//    // Make sure to release all memory acquired
//    reme_context_destroy(&c);
//
//	cout << "Reconstruction is done. "<< endl;
//	return(0);
//
//
//}