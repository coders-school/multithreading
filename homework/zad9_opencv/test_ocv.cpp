#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <future>
#include <thread>

void display_mat( const cv::Mat& mat )
{
    std::cout << "rows: " << mat.rows << std::endl;
    std::cout << "cols: " << mat.cols << std::endl;

    for ( int i = 0; i < mat.rows; ++i )
    {
        std::cout << "[" << std::setw(2) << i << "] > ";

        for ( int k = 0; k < mat.cols; ++k )
        {
            // std::cout << (int)(mat.data[i]) << " ";
            std::cout << (int)(mat.at<unsigned char>(i,k)) << " ";
        }

        std::cout << std::endl;
    }
}

cv::Mat createMatLineAlpha( const cv::Size& size, int iAlpha )
{
    cv::Mat mat(size, CV_8U);
    cv::Point origin( size.width / 2, size.height / 2 );
    double dRad = ( iAlpha * M_PI ) / 180.0;

    std::cout << "origin x " << origin.x << std::endl;
    std::cout << "origin y " << origin.y << std::endl;

    if ( iAlpha != 90 )
    {
        for( int iCol = 0; iCol < size.width; ++iCol )
        {
            double dValue = std::tan(dRad) * ( iCol - origin.x );
            int iPos = origin.y - std::lround(dValue);
            std::cout << "iCol " << iCol << " dValue " << dValue << " std::lround((dValue) " << std::lround(dValue) << " iPos " << iPos << std::endl;

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

int main( int argc, char** argv )
{
    int horizontal_cols = 1024;
    int horizontal_size_value = horizontal_cols / 30;
    cv::Size horSize(horizontal_size_value, 1);

    std::cout << "height: " << horSize.height << std::endl;
    std::cout << "width: " << horSize.width << std::endl;

    cv::Mat imgStructure = cv::getStructuringElement(cv::MORPH_RECT, horSize);
    display_mat(imgStructure);

    cv::Mat imgStructure2 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(7, 7));
    display_mat(imgStructure2);

    cv::Mat imgLine = createMatLineAlpha( cv::Size(7, 7), 45 );
    display_mat(imgLine);

    cv::Mat imgLine2 = createMatLineAlpha( cv::Size(21, 21), 135 );
    display_mat(imgLine2);

    return 0;
}