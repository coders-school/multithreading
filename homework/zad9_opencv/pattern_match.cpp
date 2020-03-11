#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

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

    cv::Mat result;

    for( int i = 0; i < patternsCnt; ++i )
    {
        // Create the result matrix
        const int result_cols =  imgSource.cols - imgPatterns[i].cols + 1;
        const int result_rows = imgSource.rows - imgPatterns[i].rows + 1;

        result.create( result_rows, result_cols, CV_32FC1 );

        // match pattern and normalize
        cv::matchTemplate(imgSource, imgPatterns[i], result, cv::TM_SQDIFF);
        cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        // localize the best match
        double minVal;
        double maxVal;
        cv::Point minLoc;
        cv::Point maxLoc;
        cv::Point matchLoc;

        cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
        matchLoc = minLoc;

        // show results
        cv::rectangle( img_display, matchLoc, cv::Point( matchLoc.x + imgPatterns[i].cols , matchLoc.y + imgPatterns[0].rows ), cv::Scalar::all(0), 2, 8, 0 );
    }

    cv::imshow( image_window, img_display );

    cv::waitKey(0);
    return EXIT_SUCCESS;
}