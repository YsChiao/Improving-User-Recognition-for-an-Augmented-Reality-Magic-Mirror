// Example to display NiTE user map with OpenCV
//
// 		by Heresy
// 		http://kheresy.wordpress.com
//
// version 1.00 @2013/03/08

// STL Header
#include <iostream>

// OpenCV Header
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// o1. OpenNI Header
#include <OpenNI.h>

// NiTE Header
#include <NiTE.h>


// namespace
using namespace std;
using namespace openni;
using namespace nite;


int main( int argc, char **argv )
{

	// o2. Initial OpenNI
	OpenNI::initialize();

	// o3. Open Device
	Device  mDevice;
	//mDevice.open( "depth1.oni" );
	mDevice.open(ANY_DEVICE);

	// o4. create depth stream
	VideoStream mDepthStream;
	mDepthStream.create( mDevice, SENSOR_DEPTH );

	// o4a. set video mode
	VideoMode mDMode;
	mDMode.setResolution( 640, 480 );
	mDMode.setFps( 30 );
	mDMode.setPixelFormat( PIXEL_FORMAT_DEPTH_1_MM );
	mDepthStream.setVideoMode( mDMode);

	// o5. Create color stream
	VideoStream mColorStream;
	mColorStream.create( mDevice, SENSOR_COLOR );

	// o5a. set video mode
	VideoMode mCMode;
	mCMode.setResolution( 640, 480 );
	mCMode.setFps( 30 );
	mCMode.setPixelFormat( PIXEL_FORMAT_RGB888 );
	mColorStream.setVideoMode( mCMode);

	// o6. image registration
	mDevice.setImageRegistrationMode( IMAGE_REGISTRATION_DEPTH_TO_COLOR );



	// Initial NiTE
	if( NiTE::initialize() != nite::Status::STATUS_OK )
	{
		cerr << "NiTE initial error" << endl;
		return -1;
	}

	// create user tracker
	UserTracker mUserTracker;
	mUserTracker.create( &mDevice );
	//if( mUserTracker.create() != nite::Status::STATUS_OK )
	//{
	//	cerr << "Can't create user tracker" << endl;
	//	return -1;
	//}

	// create OpenCV Window
	cv::namedWindow( "User Image",  CV_WINDOW_AUTOSIZE );

	// User color table
	cv::Vec3b aUserColor[8];
	aUserColor[0] = cv::Vec3b( 255, 0, 0 );
	aUserColor[1] = cv::Vec3b( 0, 255, 0 );
	aUserColor[2] = cv::Vec3b( 0, 0, 255 );
	aUserColor[3] = cv::Vec3b( 255, 255, 0 );
	aUserColor[4] = cv::Vec3b( 255, 0, 255 );
	aUserColor[5] = cv::Vec3b( 0, 255, 255 );
	aUserColor[6] = cv::Vec3b( 255, 255, 255 );
	aUserColor[7] = cv::Vec3b( 0, 0, 0 );

	// start
	while( true )
	{
		// get user frame
		UserTrackerFrameRef	mUserFrame;
		if( mUserTracker.readFrame( &mUserFrame )== nite::Status::STATUS_OK )
		{
			const nite::Array<nite::UserData>& users = mUserFrame.getUsers();

			// get depth data and convert to OpenCV format
			openni::VideoFrameRef vfDepthFrame = mUserFrame.getDepthFrame();
			const cv::Mat mImageDepth( vfDepthFrame.getHeight(), vfDepthFrame.getWidth(), CV_16UC1, const_cast<void*>( vfDepthFrame.getData() ) );
			// re-map depth data [0,Max] to [0,255]
			cv::Mat mScaledDepth;
			mImageDepth.convertTo( mScaledDepth, CV_8U, 255.0 / 10000 );

			// convert gray-scale to color
			cv::Mat mImageBGR;
			cv::cvtColor( mScaledDepth, mImageBGR, CV_GRAY2BGR );


			// get user map
			const UserMap&	rUserMap = mUserFrame.getUserMap();
			const UserId*	pUserMapData = rUserMap.getPixels();

			//get frame floor
			Plane floorCoords;
			NitePoint3f floorPoint;

			floorCoords = mUserFrame.getFloor();
			floorPoint = floorCoords.point;
			//cout << floorCoords.normal.x << " " << floorCoords.normal.y << " " << floorCoords.normal.z << endl;
			//cout << floorCoords.point.x  << " " << floorCoords.point.y  << " " << floorCoords.point.z  << "\n" << endl;


			int count = 0;
			for (int i = 0 ; i < users.getSize(); ++i)
			{
				//cout << "User: " << users[i].getId() << endl;

				nite::BoundingBox userbb = users[i].getBoundingBox();
				
				// draw BoundingBox line 
				float minPosX = userbb.min.x;
				float maxPosX = userbb.max.x;
				float minPosY = userbb.min.y;
				float maxPosY = userbb.max.y;

				//cout << minPosX << " " << minPosY << " " << userbb.min.z << endl;
				//cout << maxPosY - minPosY << endl; // height of pixels;

				cv::Point2f a(minPosX, minPosY), b(maxPosX, minPosY), c(minPosX, maxPosY), d(maxPosX, maxPosY);
				line(mImageBGR, a, b, cv::Scalar(255,0,0));
				line(mImageBGR, b, d, cv::Scalar(255,0,0));
				line(mImageBGR, d, c, cv::Scalar(255,0,0));
				line(mImageBGR, c, a, cv::Scalar(255,0,0));

				// draw user pixels
				for( int y = 0; y < rUserMap.getHeight(); ++ y )
				{
					for( int x = 0; x < rUserMap.getWidth(); ++ x )
					{
						const UserId& rUserID = pUserMapData[ x + y * rUserMap.getWidth() ];
						if( rUserID != 0 ) 
						{
							mImageBGR.at<cv::Vec3b>( y, x ) = aUserColor[ rUserID % 8 ];

							if(count == 0)
							{
								const DepthPixel* pDepthArray = (const DepthPixel*)vfDepthFrame.getData();
								int idx = x + y * vfDepthFrame.getWidth();
								const DepthPixel&  rDepth = pDepthArray[idx];
								float fX, fY, fZ; 
								CoordinateConverter::convertDepthToWorld( mDepthStream, x, y, rDepth, &fX, &fY, &fZ );
								//cout << fX << " " << fY << " " << fZ << endl;
								//show the height
								cout << abs( fY - floorCoords.point.y )<< endl; 
							}
							count ++;
						}

					}
				}
			}

			// show image
			cv::imshow( "User Image", mImageBGR );
			cv::waitKey(300);
			mUserFrame.release();
		}
		else
		{
			cerr << "Can't get user frame" << endl;
		}

	}


	// stop
	mUserTracker.destroy();
	NiTE::shutdown();

	return 0;
}