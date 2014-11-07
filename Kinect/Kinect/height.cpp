//// STL Header
//#include <iostream>
//#include <math.h>
//
//// OpenCV Header
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//// o1. OpenNI Header
//#include <OpenNI.h>
//
//// n1. NiTE Header
//#include <NiTE.h>
//
//// namespace
//using namespace std;
//using namespace openni;
//using namespace nite;
//
//cv::Scalar colors[] = {
//	cv::Scalar( 240,40,0 ),
//	cv::Scalar( 0, 128, 0),
//	cv::Scalar( 0,255,0 ),
//	cv::Scalar( 0,128,200),
//	cv::Scalar( 0,0,255),
//	cv::Scalar( 0, 1, 1 ),
//};
//
//cv::Mat drawUser( nite::UserTrackerFrameRef& userFrame )
//{
//	cv::Mat depthImage;
//
//	openni::VideoFrameRef depthFrame = userFrame.getDepthFrame();
//	if ( depthFrame.isValid() ) 
//	{
//		openni::VideoMode videoMode = depthFrame.getVideoMode();
//		depthImage = cv::Mat( videoMode.getResolutionY(), videoMode.getResolutionX(), CV_8UC4 );
//
//		openni::DepthPixel* depth = (openni::DepthPixel*)depthFrame.getData();
//		const nite::UserId* pLabels = userFrame.getUserMap().getPixels();
//		for (int i = 0; i < (depthFrame.getDataSize()/sizeof(openni::DepthPixel)); ++i)
//		{
//			// 画像インデックスを生成
//			int index = i * 4;
//
//			// 距離データを画像化する
//			UCHAR* data = &depthImage.data[index];
//			if ( pLabels[i] != 0 ) {
//				data[0] *= colors[pLabels[i]][0];
//				data[1] *= colors[pLabels[i]][1];
//				data[2] *= colors[pLabels[i]][2];
//			}
//			else 
//			{
//				// 0-255のグレーデータを作成する
//				// distance : 10000 = gray : 255
//				int gray = ~((depth[i] * 255) / 10000);
//				data[0] = gray;
//				data[1] = gray;
//				data[2] = gray;
//			}
//		}
//	}
//
//	return depthImage;
//}
//
//void drawSkeleton( cv::Mat& depthImage, nite::UserTracker& userTracker,
//	nite::UserTrackerFrameRef& userFrame )
//{
//	const nite::Array<nite::UserData>& users = userFrame.getUsers();
//	for ( int i = 0; i < users.getSize(); ++i ) {
//		const nite::UserData& user = users[i];
//		if ( user.isNew() ) {
//			userTracker.startSkeletonTracking( user.getId() );
//		}
//		else if ( !user.isLost() ) {
//			const nite::Skeleton& skeelton = user.getSkeleton();
//			if ( skeelton.getState() == nite::SkeletonState::SKELETON_TRACKED ) {
//				for ( int j = 0; j <= 14; ++j ) {
//					const nite::SkeletonJoint& joint = skeelton.getJoint((nite::JointType)j);
//					if ( joint.getPositionConfidence() >= 0.7f ) {
//						const nite::Point3f& position = joint.getPosition();
//						float x = 0, y = 0;
//						userTracker.convertJointCoordinatesToDepth(
//							position.x, position.y, position.z, &x, &y );
//
//						cv::circle( depthImage, cvPoint( (int)x, (int)y ),
//							5, cv::Scalar( 0, 0, 255 ), 5 );
//					}
//				}
//			}
//		}
//	}
//}
//
//float getDistance ( Plane floorCoords, SkeletonJoint aJoints)
//{
//	// get normal 
//	float normalA = floorCoords.normal.x;
//	float normalB = floorCoords.normal.y;
//	float normalC = floorCoords.normal.z;
//
//	// get point
//	float pointA = floorCoords.point.x;
//	float pointB = floorCoords.point.y;
//	float pointC = floorCoords.point.z;
//
//	float normalD = -(normalA*pointA + normalB*pointB + normalC*pointC); // Ax+By+Cz+D=0 for plane
//
//	// get head point
//	float headA = aJoints.getPosition().x;
//	float headB = aJoints.getPosition().y;
//	float headC = aJoints.getPosition().z;
//
//	float distance; 
//	float Up, Down;
//	Up = abs(normalA*headA + normalB*headB + normalC*headC);
//	Down = sqrt(normalA*normalA + normalB*normalB + normalC*normalC);
//	distance = Up/Down;
//
//	cout << "Height: " << distance << endl;
//
//	return distance;
//
//}
//
//
//
//
//int main( int argc, char **argv )
//{
//	// o2. Initial OpenNI
//	OpenNI::initialize();
//
//	// o3. Open Device
//	Device  mDevice;
//	mDevice.open( "depth1.oni" );
//
//	// o4. create depth stream
//	VideoStream mDepthStream;
//	mDepthStream.create( mDevice, SENSOR_DEPTH );
//
//	// o4a. set video mode
//	VideoMode mDMode;
//	mDMode.setResolution( 640, 480 );
//	mDMode.setFps( 30 );
//	mDMode.setPixelFormat( PIXEL_FORMAT_DEPTH_1_MM );
//	mDepthStream.setVideoMode( mDMode);
//
//	// o5. Create color stream
//	VideoStream mColorStream;
//	mColorStream.create( mDevice, SENSOR_COLOR );
//
//	// o5a. set video mode
//	VideoMode mCMode;
//	mCMode.setResolution( 640, 480 );
//	mCMode.setFps( 30 );
//	mCMode.setPixelFormat( PIXEL_FORMAT_RGB888 );
//	mColorStream.setVideoMode( mCMode);
//
//	// o6. image registration
//	mDevice.setImageRegistrationMode( IMAGE_REGISTRATION_DEPTH_TO_COLOR );
//
//	// n2. Initial NiTE
//	NiTE::initialize();
//
//	// n3. create user tracker
//	UserTracker mUserTracker;
//	mUserTracker.create( &mDevice );
//	mUserTracker.setSkeletonSmoothingFactor( 0.1f );
//
//
//	// p1. start
//	mColorStream.start();
//	mDepthStream.start();
//
//	while( true )
//	{
//		// main loop
//
//		// p2. prepare background
//		cv::Mat cImageBGR; cv::Mat depthImage;
//		// p2a. get color frame
//		VideoFrameRef mColorFrame;
//		mColorStream.readFrame( &mColorFrame );
//		// p2b. convert data to OpenCV format
//		const cv::Mat mImageRGB( mColorFrame.getHeight(), mColorFrame.getWidth(), CV_8UC3, (void*)mColorFrame.getData() );
//		// p2c. convert form RGB to BGR
//		cv::cvtColor( mImageRGB, cImageBGR, CV_RGB2BGR );
//
//		// p3. get user frame
//		UserTrackerFrameRef  mUserFrame;
//		mUserTracker.readFrame( &mUserFrame );
//
//		depthImage = drawUser( mUserFrame );
//		drawSkeleton( depthImage, mUserTracker, mUserFrame );
//		cv::imshow( "User", depthImage );
//		cv::waitKey(10);
//
//		// get frame floor
//		Plane floorCoords;
//		NitePoint3f floorPoint;
//
//		floorCoords = mUserFrame.getFloor();
//		floorPoint = floorCoords.point;
//
//		cout << floorCoords.normal.x << " " << floorCoords.normal.y << " " << floorCoords.normal.z << endl;
//		cout << floorCoords.point.x  << " " << floorCoords.point.y  << " " << floorCoords.point.z  << "\n" << endl;
//
//
//
//		// p4. get users data
//		const nite::Array<UserData>& aUsers = mUserFrame.getUsers();
//		for( int i = 0; i < aUsers.getSize(); ++ i )
//		{
//			const UserData& rUser = aUsers[i];
//
//			// p4a. check user status
//			if( rUser.isNew() )
//			{
//				// start tracking for new user
//				mUserTracker.startSkeletonTracking( rUser.getId() );
//			}
//
//			if( rUser.isVisible() )
//			{
//				// p4b. get user skeleton
//				const Skeleton& rSkeleton = rUser.getSkeleton();
//				if( rSkeleton.getState() == SKELETON_TRACKED )
//				{
//					// p4c. build joints array
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
//					// show the height
//					float height = getDistance ( floorCoords, aJoints[0]);
//				}
//			}
//		}
//	}
//
//	// p7. stop
//	mUserTracker.destroy();
//	mColorStream.destroy();
//	mDepthStream.destroy();
//	mDevice.close();
//	NiTE::shutdown();
//	OpenNI::shutdown();
//
//	return 0;
//}
