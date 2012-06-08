#include <iostream>
#include <xncv\xncv.hpp>

int main(int argc, char* argv[])
{
	//Wrong usage test
	if (argc == 1)
	{
		std::cout << "xncv Simple Player" << std::endl;
		std::cout << "------------------" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << "simpleplayer <oni file name>" << std::endl;
		return 1;
	}

	try
	{
		//Create and starts the video source from a .oni file
		//To read from kinect do not provide a filename (you may also provide a
		//NULL or "" filename).
		xncv::VideoSource source(argv[1]);
		source.start();

		cv::namedWindow("Video");
		cv::namedWindow("Depth");

		// Main loop
		bool running = true;
		while (running)
		{
			source.update();

			//Reads and displays the RGB image. Notice that the image is converted
			//to BGR, since it's opencv default format.
			cv::Mat video = source.captureBGR();
			cv::imshow("Video", video);

			//Reads the depth map and calculates it's histogram distributed image
			cv::Mat dm = source.captureDepth(); //Depth map as a ushort Mat.
			cv::Mat hist = source.calcDepthHist();  //Depth map histogram
			cv::Mat histImg = dm = xncv::cvtDepthTo8UHist(dm, hist);
			cv::imshow("Depth", histImg);

			//Waits for user input
			//ESC - Leave application
			//R - Records current frame.
			char key = cv::waitKey(1);
			if (key == 27)
				running = false;
			else if (key == 'r' || key == 'R')
			{
				cv::imwrite("video.jpg", source.captureBGR());
				cv::imwrite("depth.jpg", dm);
				cv::waitKey(500);
			}
		}
	}
	catch (xncv::Exception& e)
	{
		//Show error and leave
		std::cout << "Some problems have occurred:" << std::endl;
		std::cout << e.what() << std::endl;
		return 2;
	}

	return 0;
}