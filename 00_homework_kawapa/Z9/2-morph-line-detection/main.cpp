#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <array>
#include <future>
#include <functional>
#include <iostream>
#include <vector>

using namespace cv;

void prepare(const Mat &, Mat &, Mat &);
void show_wait_destroy(const char* winname, cv::Mat img);

int main(int argc, char** argv)
{
    Mat gray;
    Mat bw;
    
    CommandLineParser parser(argc, argv, "{@input | notes.png | input image}");
    Mat src = imread(samples::findFile(parser.get<String>("@input")), IMREAD_COLOR);

    if (src.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    prepare(src, gray, bw);
    
    auto diagonalLeft = [&](cv::Mat &source){

                int side = source.rows / 30;
                int array[side*side] = {};

                for (size_t i = 0; i < side * side; i = i + side + 1)
                    array[i] = 1;

                Mat diagonalLeftStructure = Mat(side, side, CV_8U, array);

                erode(source, source, diagonalLeftStructure, Point(-1, -1));
                dilate(source, source, diagonalLeftStructure, Point(-1, -1));
    };

    Mat empty = bw.clone();
    std::future<void> futures = std::async(std::launch::async, diagonalLeft, std::ref(empty));
    futures.get();
    show_wait_destroy("diagonalLeft", empty);
 
    return 0;
}

void prepare(const Mat & src, Mat & gray, Mat & bw)
{
    if (src.channels() == 3)
        cvtColor(src, gray, COLOR_BGR2GRAY);
    else
        gray = src;

    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
}

void show_wait_destroy(const char* winname, cv::Mat img)
{
    imshow(winname, img);
    moveWindow(winname, 500, 0);
    waitKey(0);
    destroyWindow(winname);
}