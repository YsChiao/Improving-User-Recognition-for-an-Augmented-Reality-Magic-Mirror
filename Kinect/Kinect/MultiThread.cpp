//#include <iostream>
//#include <opencv2\core\core.hpp>
//#include <opencv2\highgui\highgui.hpp>
//#include <opencv2\imgproc\imgproc.hpp>
//#include <openbr\openbr_plugin.h>
//#include <reconstructmesdk\reme.h>
//#include <conio.h>
//#include <Windows.h>
//#include <process.h>  
//#include <stdio.h>  
//#include <NiTE.h>
//#include <OpenNI.h>
//#include <string>
//#include <time.h>
//
//// A simple macro that checks for any errors and exits if a method invocation failed.
//#define OK(s)                                       \
//	if (s != REME_ERROR_SUCCESS) {                  \
//	reme_context_print_errors(c);                   \
//	reme_context_destroy(&c);                       \
//	exit(-1);                                       \
//	}                                               \
//
//// Test for OpenNI errors
//#define OPENNI_ASSERT(expr)                                        \
//	if (!(expr) == openni::STATUS_OK)                              \
//	throw bad_exception(openni::OpenNI::getExtendedError());       \
//
//using namespace cv;
//using namespace std;
//using namespace openni;
//using namespace nite;
//
//Mat cameraFrame;
//Device device;
//VideoFrameRef oniColorImg;
//VideoFrameRef oniDepthImg;
//VideoStream oniColorStream;
//VideoStream oniDepthStream;
//volatile long check = 0;
//QSharedPointer<br::Transform> transformGender;
//QSharedPointer<br::Transform> transformAge;
//nite::UserTracker mUserTracker;
//nite::UserTrackerFrameRef mUserFrame;
//
//
//static void printTemplateGender(const br::Template &t)
//{
//	printf("%s gender: %s\n", qPrintable(t.file.fileName()), qPrintable(t.file.get<QString>("Gender")));
//}
//
//static void printTemplateAge(const br::Template &t)
//{
//	printf("%s age: %d\n", qPrintable(t.file.fileName()), int(t.file.get<float>("Age")));
//}
//
//int OpenBR(QSharedPointer<br::Transform> transformGender, QSharedPointer<br::Transform> transformAge, Mat img, string &Gender, int &Age)
//{
//	// Initialize templates
//	br::Template queryGender(img);
//	br::Template queryAge(img);
//
//	// Enroll templates
//	queryGender >> *transformGender;
//	queryAge >> *transformAge;
//
//	//printTemplateGender(queryGender);
//	//printTemplateAge(queryAge);
//
//	Gender = qPrintable(queryGender.file.get<QString>("Gender"));
//	Age =   int(queryAge.file.get<float>("Age"));
//
//	return 0;
//}
//
//
//unsigned int __stdcall GenderAge (PVOID pM)
//{
//	
//	string Gender; 
//	int Age; 
//	int count = 0;
//	int sum = 0;
//	for (int i = 0; i < 15; i++)
//	{
//		OpenBR(transformGender, transformAge, cameraFrame, Gender, Age);
//
//		if( Gender == "Male")
//		{ 
//			count = count + 1;
//		}
//	    if( Gender == "Female")
//		{ 
//			count = count - 1;
//		}
//		sum = sum + Age;
//	}
//	if (count > 0)
//	{
//		cout << "Gender : Male" << endl;
//	}
//    if (count < 0)
//	{
//		cout << "Gender : Female" << endl;
//	}
//
//	cout << "Age : " << int(sum/15) << endl; 
//
//	return(0);
//}
//
//
//
//unsigned int __stdcall VideoShow (PVOID pM)
//{
//	cout << "Starting video" << endl;
//	VideoCapture cap(0);
//	if(!cap.isOpened())
//	{
//		cout << "camera open failed !!!"<<endl;
//		return(-1);
//	}
//	while(true)
//	{
//		cap.read(cameraFrame);
//		imshow("Main UI Video", cameraFrame);
//		waitKey(10);
//	}
//}
//
//unsigned int __stdcall ImgShow (PVOID pM)
//{
//
//	namedWindow("capatured image");
//	imshow("capatured image",cameraFrame);
//	waitKey(0);
//	return 0;
//}
//
//
//unsigned int __stdcall Check (PVOID pM)
//{
//	while(true)
//	{
//		if(getch())
//		{
//			InterlockedExchange((LPLONG)&check, 1);
//		}
//	}
//}
//
//unsigned int __stdcall UserDetection(PVOID pM)
//{
//	// initialize NiTE
//	nite::NiTE::initialize();
//
//	// create user tracker
//	//nite::UserTracker mUserTracker;
//
//	mUserTracker.create( &device);
//	mUserTracker.setSkeletonSmoothingFactor( 0.1f );
//
//	//mUserTracker.create();
//	//nite::UserTrackerFrameRef mUserFrame;
//
//	while(true)
//	{
//		Mat mImageRGB( oniColorImg.getHeight(), oniColorImg.getWidth(), CV_8UC3, (void*)oniColorImg.getData() );
//		cvtColor( mImageRGB, mImageRGB, CV_RGB2BGR );
//
//		// get user frame
//		mUserTracker.readFrame( &mUserFrame );
//
//		// get users' data
//		const nite::Array<nite::UserData>& aUsers = mUserFrame.getUsers();
//		for( int i = 0; i < aUsers.getSize(); ++ i )
//		{
//			const nite::UserData& rUser = aUsers[i];
//			if( rUser.isNew() )
//			{
//				cout << "New User [" << rUser.getId() << "] found." << endl;
//				mUserTracker.startSkeletonTracking( rUser.getId() );
//				InterlockedExchange((LPLONG)&check, 1);
//			}
//			if( rUser.isLost() )
//			{
//				cout << "User [" << rUser.getId()  << "] lost." << endl;
//				InterlockedExchange((LPLONG)&check, 0);
//			}
//			if( rUser.isVisible() )
//			{
//				// get user skeleton
//				const Skeleton& rSkeleton = rUser.getSkeleton();
//				if( rSkeleton.getState() == SKELETON_TRACKED )
//				{
//					// build joints array
//					SkeletonJoint aJoints[15];
//					aJoints[ 0] = rSkeleton.getJoint( JOINT_HEAD );
//					aJoints[ 1] = rSkeleton.getJoint( JOINT_NECK );
//					aJoints[ 2] = rSkeleton.getJoint( JOINT_LEFT_SHOULDER );
//					aJoints[ 3] = rSkeleton.getJoint( JOINT_RIGHT_SHOULDER );
//					aJoints[ 4] = rSkeleton.getJoint( JOINT_LEFT_ELBOW );
//					aJoints[ 5] = rSkeleton.getJoint( JOINT_RIGHT_ELBOW );
//					aJoints[ 6] = rSkeleton.getJoint( JOINT_LEFT_HAND );
//					aJoints[ 7] = rSkeleton.getJoint( JOINT_RIGHT_HAND );
//					aJoints[ 8] = rSkeleton.getJoint( JOINT_TORSO );
//					aJoints[ 9] = rSkeleton.getJoint( JOINT_LEFT_HIP );
//					aJoints[10] = rSkeleton.getJoint( JOINT_RIGHT_HIP );
//					aJoints[11] = rSkeleton.getJoint( JOINT_LEFT_KNEE );
//					aJoints[12] = rSkeleton.getJoint( JOINT_RIGHT_KNEE );
//					aJoints[13] = rSkeleton.getJoint( JOINT_LEFT_FOOT );
//					aJoints[14] = rSkeleton.getJoint( JOINT_RIGHT_FOOT );
//
//					// convert joint position to image
//					cv::Point2f aPoint[15];
//					for( int  s = 0; s < 15; ++ s )
//					{
//						const nite::Point3f& rPos = aJoints[s].getPosition();
//						mUserTracker.convertJointCoordinatesToDepth( 
//							rPos.x, rPos.y, rPos.z,
//							&(aPoint[s].x), &(aPoint[s].y) );
//					}
//
//					// draw line
//					cv::line( mImageRGB, aPoint[ 0], aPoint[ 1], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 1], aPoint[ 2], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 1], aPoint[ 3], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 2], aPoint[ 4], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 3], aPoint[ 5], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 4], aPoint[ 6], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 5], aPoint[ 7], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 1], aPoint[ 8], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 8], aPoint[ 9], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 8], aPoint[10], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[ 9], aPoint[11], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[10], aPoint[12], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[11], aPoint[13], cv::Scalar( 255, 0, 0 ), 3 );
//					cv::line( mImageRGB, aPoint[12], aPoint[14], cv::Scalar( 255, 0, 0 ), 3 );
//
//					// draw joint
//					for( int  s = 0; s < 15; ++ s )
//					{
//						if( aJoints[s].getPositionConfidence() > 0.5 )
//							cv::circle( mImageRGB, aPoint[s], 3, cv::Scalar( 0, 0, 255 ), 2 );
//						else
//							cv::circle( mImageRGB, aPoint[s], 3, cv::Scalar( 0, 255, 0 ), 2 );
//					}
//				}
//			}
//		}
//		cv::imshow( "User Image", mImageRGB );
//		cv::waitKey(10);
//	}
//	mUserTracker.destroy();
//	oniColorStream.destroy();
//	oniDepthStream.destroy();
//	device.close();
//	NiTE::shutdown();
//	OpenNI::shutdown();
//	nite::NiTE::shutdown();
//
//	return (0);
//}
//
//unsigned int __stdcall Height (PVOID pM)
//{
//	//// create OpenCV Window
//	//cv::namedWindow( " Depth Image ",  CV_WINDOW_AUTOSIZE );
//
//	//// User color table
//	//cv::Vec3b aUserColor[8];
//	//aUserColor[0] = cv::Vec3b( 255, 0, 0 );
//	//aUserColor[1] = cv::Vec3b( 0, 255, 0 );
//	//aUserColor[2] = cv::Vec3b( 0, 0, 255 );
//	//aUserColor[3] = cv::Vec3b( 255, 255, 0 );
//	//aUserColor[4] = cv::Vec3b( 255, 0, 255 );
//	//aUserColor[5] = cv::Vec3b( 0, 255, 255 );
//	//aUserColor[6] = cv::Vec3b( 255, 255, 255 );
//	//aUserColor[7] = cv::Vec3b( 0, 0, 0 );
//
//	int count = 0; // take the firt entry of the depth image
//	int frame = 0; // take the entry of the frame image
//	
//	// start
//	while(true)
//	{   start:
//		if( mUserTracker.readFrame( &mUserFrame )== nite::Status::STATUS_OK )
//		{
//			const nite::Array<nite::UserData>& users = mUserFrame.getUsers();
//
//			//const cv::Mat mImageDepth( oniDepthImg.getHeight(), oniDepthImg.getWidth(), CV_16UC1, const_cast<void*>( oniDepthImg.getData() ) );
//			//// re-map depth data [0,Max] to [0,255]
//			//cv::Mat mScaledDepth;
//			//mImageDepth.convertTo( mScaledDepth, CV_8U, 255.0 / 10000 );
//			//// convert gray-scale to color
//			//cv::Mat mImageBGR;
//			//cv::cvtColor( mScaledDepth, mImageBGR, CV_GRAY2BGR );
//
//			// get user map
//			const UserMap&	rUserMap = mUserFrame.getUserMap();
//			const UserId*	pUserMapData = rUserMap.getPixels();
//
//			//get frame floor
//			Plane floorCoords;
//			NitePoint3f floorPoint;
//
//			floorCoords = mUserFrame.getFloor();
//			floorPoint = floorCoords.point;
//			//cout << floorCoords.normal.x << " " << floorCoords.normal.y << " " << floorCoords.normal.z << endl;
//			//cout << floorCoords.point.x  << " " << floorCoords.point.y  << " " << floorCoords.point.z  << "\n" << endl;
//
//			restart:
//			for (int i = 0 ; i < users.getSize(); ++i)
//			{
//				frame ++;
//
//				nite::BoundingBox userbb = users[i].getBoundingBox();
//
//				// draw BoundingBox line 
//				float minPosX = userbb.min.x;
//				float maxPosX = userbb.max.x;
//				float minPosY = userbb.min.y;
//				float maxPosY = userbb.max.y;
//
//				//cout << minPosX << " " << minPosY << " " << userbb.min.z << endl;
//				//cout << maxPosY - minPosY << endl; // height of pixels;
//
//				//cv::Point2f a(minPosX, minPosY), b(maxPosX, minPosY), c(minPosX, maxPosY), d(maxPosX, maxPosY);
//				//line(mImageBGR, a, b, cv::Scalar(255,0,0));
//				//line(mImageBGR, b, d, cv::Scalar(255,0,0));
//				//line(mImageBGR, d, c, cv::Scalar(255,0,0));
//				//line(mImageBGR, c, a, cv::Scalar(255,0,0));
//
//
//				if ( frame == 20 ) // get the 20th frame 
//				{
//					// draw user pixels
//					for( int y = 0; y < rUserMap.getHeight(); ++ y )
//					{
//						for( int x = 0; x < rUserMap.getWidth(); ++ x )
//						{
//							const UserId& rUserID = pUserMapData[ x + y * rUserMap.getWidth() ];
//							if( rUserID != 0 ) 
//							{
//								//mImageBGR.at<cv::Vec3b>( y, x ) = aUserColor[ rUserID % 8 ];
//
//								if(count == 0)
//								{
//									const DepthPixel* pDepthArray = (const DepthPixel*)oniDepthImg.getData();
//									int idx = x + y * oniDepthImg.getWidth();
//									const DepthPixel&  rDepth = pDepthArray[idx];
//									float fX, fY, fZ; 
//									CoordinateConverter::convertDepthToWorld( oniDepthStream, x, y, rDepth, &fX, &fY, &fZ );
//									//cout << fX << " " << fY << " " << fZ << endl;
//									//show the height
//									if (fY == 0)
//									{
//										cout << "Height failed, try again"<< endl;
//										frame = 0;
//										goto restart;
//									}
//									cout << "Height: " << abs( fY - floorCoords.point.y )<< endl; 
//									return 0;
//								}
//								count ++;
//							}
//
//						}
//					}
//				}
//			}
//
//			// show image
//			//cv::imshow( " Depth Image ", mImageBGR );
//			//cv::waitKey(10);
//			//mUserFrame.release();
//		}
//		else
//		{
//			cerr << "Can't get user frame" << endl;
//		}
//
//	}	
//}
//	
//
//unsigned int __stdcall OpenNIShow (PVOID pM)
//{	
//	puts( "Kinect opened" );
//	oniColorStream.create(device, SENSOR_COLOR);
//	oniColorStream.start();
//	puts("Camera ok");
//
//	oniDepthStream.create(device, SENSOR_DEPTH);
//	oniDepthStream.start();
//	puts("Depth sensor ok");
//
//	VideoMode paramvideo;
//	paramvideo.setResolution(640,480);
//	paramvideo.setFps(30);
//	paramvideo.setPixelFormat(PIXEL_FORMAT_DEPTH_100_UM);
//	oniDepthStream.setVideoMode(paramvideo);
//	paramvideo.setPixelFormat(PIXEL_FORMAT_RGB888);
//	oniColorStream.setVideoMode(paramvideo);
//	puts("Settings video streams ok");
//
//
//	// If the depth/color synchronisation is not necessary, start is faster :
//	device.setDepthColorSyncEnabled( false );
//	// image registration
//	device.setImageRegistrationMode( IMAGE_REGISTRATION_DEPTH_TO_COLOR );
//
//	// Create Mat
//	Mat colorcv( cv::Size( 640, 480 ), CV_8UC3, NULL );
//	Mat depthcv( cv::Size( 640, 480 ), CV_16UC1, NULL );
//	//namedWindow( "RGB", CV_WINDOW_AUTOSIZE );
//	//namedWindow( "Depth", CV_WINDOW_AUTOSIZE );
//
//	// Perform the Video and show on the diplay
//	VideoStream** streams = new VideoStream*[2];
//	streams[0] = &oniDepthStream;
//	streams[1] = &oniColorStream;
//	puts("Kinect initilization completed");
//
//	bool viewer_done = false;
//	// Grab from the real sensor
//	int index;
//	while (device.getSensorInfo( SENSOR_DEPTH ) != NULL) 
//	{
//		OpenNI::waitForAnyStream(streams, 2, &index);
//		switch(index)
//		{
//		case 0:
//			oniDepthStream.readFrame(&oniDepthImg);
//			if ( oniDepthImg.isValid() )
//			{
//				depthcv.data = (uchar*) oniDepthImg.getData();
//				//cv::imshow( "Depth", depthcv );
//			}
//			break;
//		case 1:
//			oniColorStream.readFrame( &oniColorImg );
//
//			if ( oniColorImg.isValid() )
//			{
//				colorcv.data = (uchar*) oniColorImg.getData();
//				cvtColor( colorcv, cameraFrame, CV_BGR2RGB );
//				//imshow( "RGB", cameraFrame );
//			}
//			break;
//		default:
//			puts( "Error retrieving a stream" );
//		}
//		cv::waitKey(10);
//	}
//
//	//destroyWindow( "RGB" );
//	//destroyWindow( "Depth" );
//
//	return 0;
//}
//
//unsigned int __stdcall ReconstructMe (PVOID pM)
////int ReconstructMe()
//{    
//	// initialize ReconstructMe
//	// Create ReconstructMe context
//	reme_context_t c;
//	reme_context_create(&c);
//
//
//	reme_options_t o;
//	reme_options_create(c, &o);
//
//	//// Create empty options binding
//	//reme_context_set_log_callback(c, reme_default_log_callback, 0);
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
//	reme_context_tune_reconstruction_options(c, REME_TUNE_PROFILE_MID_QUALITY);
//	reme_context_compile(c);
//
//	// Create external sensor.
//	reme_sensor_t s;
//	reme_sensor_create(c, "external", false, &s);
//
//	// Retrieve num_slices
//	int num_slices;
//	//OK(reme_context_bind_reconstruction_options(c, o));
//	OK(reme_options_get_int(c, o, "volume.resolution.z", &num_slices));
//	const void *bytes;
//	int length;
//
//
//	// Since we are using an external sensor, we need to tell ReMe about its image size field of view.
//	reme_sensor_bind_camera_options(c, s, o);
//	int image_width = oniDepthStream.getVideoMode().getResolutionX();
//	int image_height = oniDepthStream.getVideoMode().getResolutionY();
//	reme_options_set_int(c, o, "depth_stream.image_size.width", image_width);
//	reme_options_set_int(c, o, "depth_stream.image_size.height", image_height);
//	
//	// The following intrinsics are based on a Kinect like device. By providing
//	// intrinsics.width and intrinsics.height ReMe is able to automatically derive intrinsics
//	// for different stream sizes.
//	reme_options_set_int(c, o, "depth_stream.intrinsics.width", 640);
//	reme_options_set_int(c, o, "depth_stream.intrinsics.height", 480);
//	reme_options_set_real(c, o, "depth_stream.intrinsics.fx", 530);
//	reme_options_set_real(c, o, "depth_stream.intrinsics.fy", 530);
//	reme_options_set_real(c, o, "depth_stream.intrinsics.cx", 320);
//	reme_options_set_real(c, o, "depth_stream.intrinsics.cy", 240);
//	
//    cout << "ReconstructMe ok" << endl;
//
//	// Open the sensor like any other sensor.
//	reme_sensor_open(c, s);
//	reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT);
//
//	// In order inform ReMe about external sensor data
//	//reme_image_t raw_depth;
//	reme_image_t raw_depth;
//	reme_image_create(c, &raw_depth);
//
//	// Create a new volume
//	reme_volume_t v;
//	OK(reme_volume_create(c, &v));
//
//	// For debugging purposes open a viewer for tracking the reconstruction process.
//	reme_viewer_t viewer;
//	reme_viewer_create_image(c, "This is ReconstructMe SDK", &viewer);
//
//	reme_image_t volume;
//	reme_image_create(c, &volume);
//	reme_viewer_add_image(c, viewer, volume);
//
//	// Perform reconstruction until one complete rotation is performed
//	float prev_pos[16], cur_pos[16];
//	float rotation_axis[4] = {0,0,1,0};
//
//	reme_sensor_get_position(c, s, prev_pos);
//
//	float angle;
//	float sum_turn_angles = 0.f;
//
//	while(fabs(sum_turn_angles) < 2.f * 3.1415f) {
//
//		const unsigned short *sensor_pixel = (const unsigned short *)oniDepthImg.getData();
//
//		if (!REME_SUCCESS(reme_sensor_grab(c,s))) {
//			continue;
//		}
//		
//		reme_sensor_prepare_image(c, s, REME_IMAGE_RAW_DEPTH);
//		reme_sensor_get_image(c, s, REME_IMAGE_RAW_DEPTH, raw_depth);
//
//		void *virtual_pixel;
//		int nbytes;
//		reme_image_get_mutable_bytes(c, raw_depth, &virtual_pixel, &nbytes );
//
//		//Copy content
//		memcpy(virtual_pixel, sensor_pixel, image_width * image_height * sizeof(unsigned short));
//		// Only need to prepare volume data
//		reme_sensor_prepare_image(c, s, REME_IMAGE_VOLUME);
//
//		if (REME_SUCCESS(reme_sensor_track_position(c, s))) {
//			reme_sensor_update_volume(c, s);
//
//			reme_sensor_get_position(c, s, cur_pos);
//			reme_transform_get_projected_angle(c, rotation_axis, prev_pos, cur_pos, &angle);
//			sum_turn_angles += angle;
//			memcpy(prev_pos, cur_pos, 16 * sizeof(float));
//		}
//		else
//		{
//			cout << "Tracking failed !!!" << endl;
//			//cout << "Reconstruction is done. " << endl;
//
//			// Close and destroy the sensor, it is not needed anymore
//			reme_sensor_close(c, s);
//			reme_sensor_destroy(c, &s);
//			reme_context_destroy(&c);
//			return(-1);
//
//		}
//		//reme_sensor_get_image(c, s, REME_IMAGE_AUX, aux);
//		reme_sensor_get_image(c, s, REME_IMAGE_VOLUME, volume);
//		reme_viewer_update(c, viewer);
//		
//	}
//
//	// Close and destroy the sensor, it is not needed anymore
//	reme_sensor_close(c, s);
//	reme_sensor_destroy(c, &s);
//
//	// Create a new surface
//	reme_surface_t m;
//	reme_surface_create(c, &m);
//	reme_surface_generate(c, m, v);
//	reme_surface_save_to_file(c, m, "test.ply");
//
//	// Visualize resulting surface
//	reme_viewer_t viewer_surface;
//	reme_viewer_create_surface(c, m, "This is ReconstructMeSDK", &viewer_surface);
//	reme_viewer_wait(c, viewer_surface);
//
//	reme_surface_destroy(c, &m);
//
//	// saves the volume content to disk
//	FILE *f = fopen("volume.bin", "wb");
//	for (int i = 0; i < num_slices; i++) 
//	{
//		reme_volume_slice_get_bytes(c, v, i, &bytes, &length);
//		fwrite(&length, sizeof(int), 1, f);
//		fwrite(bytes, 1, length, f);            
//	}
//	fclose(f);
//
//	// Print pending errors
//	//reme_context_print_errors(c);
//	// Make sure to release all memory acquired
//	reme_context_destroy(&c);
//
//	cout << "Reconstruction is done. "<< endl;
//
//	return(0);
//}
//
//
//// main thread 
//int main (int argc, char** argv) 
//{   
//	//OpenBR initialize
//	br::Context::initialize(argc, argv);
//	// Retrieve class for enrolling templates using the GenderEstimation algorithm
//	//QSharedPointer<br::Transform> transformGender = br::Transform::fromAlgorithm("GenderEstimation");
//	//QSharedPointer<br::Transform> transformAge = br::Transform::fromAlgorithm("AgeEstimation");
//	transformGender = br::Transform::fromAlgorithm("GenderEstimation");
//	transformAge = br::Transform::fromAlgorithm("AgeEstimation");
//	
//	// initialize OpenNI2  
//	OPENNI_ASSERT(OpenNI::initialize());   
//	// open device      
//	if ( device.open( openni::ANY_DEVICE ) != 0 )
//	{
//		if ( device.open("depth1.oni") !=0 )
//		{
//			puts( "Kinect not found !" ); 
//			return -1;
//		}
//	}
//	
//	HANDLE handleA = (HANDLE)_beginthreadex(NULL, 0, OpenNIShow, NULL, 0, NULL); // showing the video
//	HANDLE handleB = (HANDLE)_beginthreadex(NULL, 0, UserDetection, NULL, 0, NULL); // detecting the user
//
//
//
//
//	while(true)
//	{   
//		if(check)
//		{
//			Sleep(2000);
//			//HANDLE handleC = (HANDLE)_beginthreadex(NULL, 0, ImgShow, NULL, 0, NULL);
//
//			HANDLE handleD = (HANDLE)_beginthreadex(NULL, 0, GenderAge, NULL, 0, NULL); // detecting the gender and age
//
//			HANDLE handleE = (HANDLE)_beginthreadex(NULL, 0, ReconstructMe, NULL, 0, NULL); // detecting the gender and age
//
//			HANDLE handleF = (HANDLE)_beginthreadex(NULL, 0, Height, NULL, 0, NULL); // detecting the height
//
//			InterlockedExchange((LPLONG)&check, 0);
//		}
//
//	}
//
//
//
//
//	return 0;
//} 