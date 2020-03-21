#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <mutex>
#include <future>
#include <thread>

void show_wait_destroy(const char* winname, cv::Mat img) {
    cv::imshow(winname, img);
    cv::moveWindow(winname, 500, 0);
    cv::waitKey(0);
    cv::destroyWindow(winname);
}

cv::Mat createMatLineAlpha( const cv::Size& size, int iAlpha )
{
    cv::Mat mat(size, CV_8U);
    cv::Point origin( size.width / 2, size.height / 2 );
    double dRad = ( iAlpha * M_PI ) / 180.0;

    if ( iAlpha != 90 )
    {
        for( int iCol = 0; iCol < size.width; ++iCol )
        {
            double dValue = std::tan(dRad) * ( iCol - origin.x );
            int iPos = origin.y - std::lround(dValue);

            if ( iPos >= 0 && iPos < size.height )
            {
                mat.at<unsigned char>( iPos , iCol) = 1;
            }
        }
    }
    else
    {
        for ( int iRow = 0; iRow < size.height; ++iRow )
        {
            mat.at<unsigned char>(iRow, origin.x) = 1;
        }
    }

    return mat;
}

auto lineMatchFunction = [](const cv::Mat& bw_image, const cv::Size& size, int alpha)
{
    // Create structure element for extracting lines through morphology operations
    cv::Mat imgStructure = createMatLineAlpha(size, alpha);
    cv::Mat imgLocal = bw_image.clone();

    // Apply morphology operations
    cv::erode(imgLocal, imgLocal, imgStructure, cv::Point(-1, -1));
    cv::dilate(imgLocal, imgLocal, imgStructure, cv::Point(-1, -1));

    return imgLocal.clone();
};

int main( int argc, char** argv )
{
    cv::CommandLineParser parser(argc, argv, "{@input | notes.png | input image}");
    cv::Mat src = cv::imread( cv::samples::findFile( parser.get<cv::String>("@input") ), cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    // Show source image
    cv::imshow("src", src);

    // Transform source image to gray if it is not already
    cv::Mat gray;
    if (src.channels() == 3)
    {
        cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray = src;
    }

    // Show gray image
    show_wait_destroy("gray", gray);

    // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    cv::Mat bw;
    cv::adaptiveThreshold(~gray, bw, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, -2);
    
    // Show binary image
    show_wait_destroy("binary", bw);

    // Create the images that will use to extract the angle lines
    cv::Mat matAngle0  = bw.clone();
    cv::Mat matAngle30 = bw.clone();
    cv::Mat matAngle45 = bw.clone();
    cv::Mat matAngle60 = bw.clone();
    cv::Mat matAngle90 = bw.clone();
    cv::Mat matAngle135 = bw.clone();
    cv::Mat matAngle150 = bw.clone();

    // Specify size on horizontal and vertical
    int horizontal_size_value = bw.cols / 30;
    int vertical_size_value = bw.rows / 30;
    cv::Size matchSize(horizontal_size_value, vertical_size_value);

    std::future<cv::Mat> result_ang0   = std::async(std::launch::async, lineMatchFunction, std::ref(matAngle0),   std::cref(matchSize), 0);
    std::future<cv::Mat> result_ang30  = std::async(std::launch::async, lineMatchFunction, std::ref(matAngle30),  std::cref(matchSize), 30);
    std::future<cv::Mat> result_ang45  = std::async(std::launch::async, lineMatchFunction, std::ref(matAngle45),  std::cref(matchSize), 45);
    std::future<cv::Mat> result_ang60  = std::async(std::launch::async, lineMatchFunction, std::ref(matAngle60),  std::cref(matchSize), 60);
    std::future<cv::Mat> result_ang90  = std::async(std::launch::async, lineMatchFunction, std::ref(matAngle90),  std::cref(matchSize), 90);
    std::future<cv::Mat> result_ang135 = std::async(std::launch::async, lineMatchFunction, std::ref(matAngle135), std::cref(matchSize), 135);
    std::future<cv::Mat> result_ang150 = std::async(std::launch::async, lineMatchFunction, std::ref(matAngle150), std::cref(matchSize), 150);

    cv::Mat mat_result_ang0 = result_ang0.get();
    cv::Mat mat_result_ang30 = result_ang30.get();
    cv::Mat mat_result_ang45 = result_ang45.get();
    cv::Mat mat_result_ang60 = result_ang60.get();
    cv::Mat mat_result_ang90 = result_ang90.get();
    cv::Mat mat_result_ang135 = result_ang135.get();
    cv::Mat mat_result_ang150 = result_ang150.get();

    // Show extracted lines at 0 degree
    show_wait_destroy("Angle_0", mat_result_ang0);

    // Show extracted lines at 30 degree
    show_wait_destroy("Angle_30", mat_result_ang30);

    // Show extracted lines at 45 degree
    show_wait_destroy("Angle_45", mat_result_ang45);

    // Show extracted lines at 60 degree
    show_wait_destroy("Angle_60", mat_result_ang60);

    // Show extracted lines at 90 degree
    show_wait_destroy("Angle_90", mat_result_ang90);

    // Show extracted lines at 135 degree
    show_wait_destroy("Angle_135", mat_result_ang135);

    // Show extracted lines at 150 degree
    show_wait_destroy("Angle_150", mat_result_ang150);

    // display results
    cv::Mat display = src.clone();
    display.setTo(cv::Scalar(0,0,255), mat_result_ang0);  // red
    display.setTo(cv::Scalar(0,255,0), mat_result_ang30); // green
    display.setTo(cv::Scalar(255,0,0), mat_result_ang45); // blue

    display.setTo(cv::Scalar(0,255,255), mat_result_ang60);  // yellow
    display.setTo(cv::Scalar(255,255,0), mat_result_ang90);  // cyan
    display.setTo(cv::Scalar(255,0,255), mat_result_ang135); // magenta

    display.setTo(cv::Scalar(128,128,255), mat_result_ang150); // pink

    show_wait_destroy("result", display);

    return 0;
}