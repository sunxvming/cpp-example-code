
#include <opencv2/opencv.hpp>

#include "SimpleIni.h"

int main() {
    // RTSP URL


	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile("config.ini");
	if (rc < 0) { 
        std::cerr << "Error: Unable to load config.ini file." << std::endl;
     }

	std::string video_url  = ini.GetValue("common", "video_url", "default");
    std::cout << "video_url is:" << video_url << std::endl;

    cv::VideoCapture cap;
    cap.open(video_url);

    // Check if the stream is opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open RTSP stream." << std::endl;
        return -1;
    }

    // Loop to read frames from the stream
    while (true) {
        cv::Mat frame;
        
        // Read a frame
        if (!cap.read(frame)) {
            std::cerr << "Error: Failed to read frame from the stream." << std::endl;
            break;
        }

        // Display the frame
        cv::imshow("RTSP Stream", frame);

        // Break the loop if 'q' is pressed
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Release the VideoCapture object and close the window
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
