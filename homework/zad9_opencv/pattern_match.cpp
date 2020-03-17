#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <mutex>
#include <future>
#include <thread>

int main( int argc, char** argv )
{
    if (argc < 3)
    {
        std::cout << "Not enough parameters" << std::endl;
        std::cout << "Usage:\n" << argv[0] << " <source_name> <pattern1_name> [<pattern2_name> ...]" << std::endl;
        return -1;
    }

    const int patternsCnt = argc - 2;

    // load images
    cv::Mat imgSource = cv::imread( argv[1], cv::IMREAD_COLOR );
    cv::Mat imgPatterns[patternsCnt];

    for ( int i = 0; i < patternsCnt; ++i)
    {
        imgPatterns[i] = cv::imread( argv[i + 2], cv::IMREAD_COLOR );
    }

    // create windows
    const char* image_window = "Source Image";
    cv::namedWindow( image_window, cv::WINDOW_AUTOSIZE );

    // copy source image
    cv::Mat img_display;
    imgSource.copyTo( img_display );

    std::vector<std::future<cv::Point>> futures(patternsCnt);

    for( int i = 0; i < patternsCnt; ++i )
    {
        auto matchFunction = [](const cv::Mat& source, const cv::Mat& pattern)
        {
            cv::Mat result;

            // Create the result matrix
            const int result_cols =  source.cols - pattern.cols + 1;
            const int result_rows = source.rows - pattern.rows + 1;

            result.create( result_rows, result_cols, CV_32FC1 );

            // match pattern and normalize
            cv::matchTemplate(source, pattern, result, cv::TM_SQDIFF);
            cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

            // localize the best match
            double minVal;
            double maxVal;
            cv::Point minLoc;
            cv::Point maxLoc;
            cv::Point matchLoc;

            cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
            matchLoc = minLoc;

            return matchLoc;
        };

        futures[i] = std::async(std::launch::async, matchFunction, std::cref(imgSource), std::cref(imgPatterns[i]));
    }

    // wait for results
    for ( int i = 0; i < patternsCnt; ++i )
    {
        const cv::Point& point = futures[i].get();

        // "print" result
        cv::rectangle( img_display,
            point,
            cv::Point( point.x + imgPatterns[i].cols, point.y + imgPatterns[i].rows ),
            cv::Scalar::all( 0 ),
            2,
            8,
            0 );
    }

    cv::imshow( image_window, img_display );

    cv::waitKey(0);
    return EXIT_SUCCESS;
}