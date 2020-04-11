#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <future>
#include <vector>

const char* image_window = "Source Image";
cv::Mat img_display;

int main( int argc, char** argv )
{
    if (argc < 3)
    {
        std::cout << "Not enough parameters" << std::endl;
        std::cout << "Usage:\n" << argv[0] << " <source_name> <pattern_1> [... <pattern_n>]" << std::endl;
        return -1;
    }

    const int numberOfTemplates = argc - 2;

    cv::Mat img = cv::imread( argv[1], cv::IMREAD_COLOR );
    cv::Mat templates[numberOfTemplates];

    for ( int i = 0; i < numberOfTemplates; ++i)
    {
        templates[i] = cv::imread( argv[i + 2], cv::IMREAD_COLOR );
    }

    cv::namedWindow( image_window, cv::WINDOW_AUTOSIZE );

    img.copyTo( img_display );

    std::vector<std::future<cv::Point>> futureVec(numberOfTemplates);

    for( int i = 0; i < numberOfTemplates; ++i )
    {
        auto MatchingFunction = [](const cv::Mat& source, const cv::Mat& pattern)
        {
            cv::Mat result;

            const int resultCols =  source.cols - pattern.cols + 1;
            const int resultRows = source.rows - pattern.rows + 1;

            result.create( resultRows, resultCols, CV_32FC1 );

            cv::matchTemplate(source, pattern, result, cv::TM_SQDIFF);
            cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
            
            double minVal;
            double maxVal;
            cv::Point minLoc;
            cv::Point maxLoc;

            cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

            return minLoc;
        };

        futureVec[i] = std::async(std::launch::async, MatchingFunction, std::cref(img)  , std::cref(templates[i]));
    }

    for ( int i = 0; i < numberOfTemplates; ++i )
    {
        const cv::Point& point = futureVec[i].get();

        cv::rectangle( img_display, point, cv::Point( point.x + templates[i].cols, point.y + templates[i].rows ), cv::Scalar::all( 0 ), 2, 8, 0 );
    }

    cv::imshow( image_window, img_display );
    cv::waitKey(0);

    return EXIT_SUCCESS;
} 