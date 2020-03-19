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
    
    std::array<const char*, 4> directions {"horizontal", "vertical", "diagonalLeft", "diagonalRight"};
    std::vector<Mat> mats(4);
    std::for_each(mats.begin(), mats.end(), [=](Mat & obj){ obj = bw.clone(); });
    std::vector<std::future<void>> futures(4);

    auto horizontal = [&](cv::Mat &source){
        int horizontal_size = source.cols / 30;

        cv::Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));

        erode(source, source, horizontalStructure, Point(-1, -1));
        dilate(source, source, horizontalStructure, Point(-1, -1));
    };
    auto vertical = [&](cv::Mat &source) {
        int vertical_size = source.rows / 30;

        Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, vertical_size));

        erode(source, source, verticalStructure, Point(-1, -1));
        dilate(source, source, verticalStructure, Point(-1, -1));
    };

    auto diagonalLeft = [&](cv::Mat &source) {
        int side = source.rows / 30;
        int array[side * side] = {};

        for (size_t i = 0; i < side * side; i = i + side + 1)
            array[i] = 1;

        Mat diagonalLeftStructure = Mat(side, side, CV_8U, array);

        erode(source, source, diagonalLeftStructure, Point(-1, -1));
        dilate(source, source, diagonalLeftStructure, Point(-1, -1));
    };

    auto diagonalRight = [&](cv::Mat &source) {
        int side = source.rows / 30;
        int array[side * side] = {};

        for (size_t i = side - 1; i < side * side; i = i + side - 1)
            array[i] = 1;

        Mat diagonalRightStructure = Mat(side, side, CV_8U, array);

        erode(source, source, diagonalRightStructure, Point(-1, -1));
        dilate(source, source, diagonalRightStructure, Point(-1, -1));
    };

    futures[0] = std::async(std::launch::async, horizontal, std::ref(mats[0]));
    futures[1] = std::async(std::launch::async, vertical, std::ref(mats[1]));
    futures[2] = std::async(std::launch::async, diagonalLeft, std::ref(mats[2]));
    futures[3] = std::async(std::launch::async, diagonalRight, std::ref(mats[3]));    

    for (int i = 0; i < 4; i++)
    {
        futures[i].get();
        show_wait_destroy(directions[i], mats[i]);
    }  

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