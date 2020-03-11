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

    cv::Mat imgSource;
    cv::Mat imgPatterns[patternsCnt];

    // load images
    imgSource = cv::imread( argv[1], cv::IMREAD_COLOR );
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

    std::vector<std::future<int>> futures(patternsCnt);
    std::mutex criticalSectionMutex;

    for( int i = 0; i < patternsCnt; ++i )
    {
        auto matchFunction = [&criticalSectionMutex](const cv::Mat& source, const cv::Mat& pattern, cv::Mat& display)
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

            // "print" result
            std::unique_lock<std::mutex> lock( criticalSectionMutex );
            cv::rectangle( display,
                matchLoc,
                cv::Point( matchLoc.x + pattern.cols , matchLoc.y + pattern.rows ),
                cv::Scalar::all(0), 2, 8, 0 );

            return 0;
        };

        futures[i] = std::async(std::launch::async, matchFunction, imgSource, imgPatterns[i], std::ref(img_display));
    }

    // wait for results
    std::for_each(std::begin(futures), std::end(futures), std::mem_fn(&std::future<int>::get));

    cv::imshow( image_window, img_display );

    cv::waitKey(0);
    return EXIT_SUCCESS;
}