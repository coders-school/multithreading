#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <array>
#include <future>
#include <functional>
#include <iostream>
#include <vector>

using namespace cv;

constexpr size_t n = 4; // number of kernels

void prepare(const Mat &, Mat &, Mat &);
void extractLines(Mat & inputOutput, const Mat & kernel);
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
    
    std::vector<std::future<void>> futures(n);

    std::vector<cv::Mat> results(n);
    std::for_each(results.begin(), results.end(), [=](Mat & obj){ obj = bw.clone(); });
    
    int side = bw.rows / 30;

    std::vector<cv::Mat> kernels
    {
        {getStructuringElement(MORPH_RECT, Size((bw.cols / 30), 1))},
        {getStructuringElement(MORPH_RECT, Size(1, (bw.rows / 30)))},
        {Mat::eye(side, side, CV_8U)*1},
        {Mat::eye(side, side, CV_8U)*1}
    };

    cv::rotate(kernels[3], kernels[3], cv::ROTATE_90_CLOCKWISE);

    for (size_t i = 0; i < n; i++)
        futures[i] = std::async(std::launch::async, extractLines, std::ref(results[i]), std::cref(kernels[i]));

    std::for_each(futures.begin(), futures.end(), [](std::future<void> & obj){ obj.get(); });
           
    auto finalResult = results[0] + results[1] + results[2] + results[3];
    show_wait_destroy("Result", finalResult);

    return 0;
}

void extractLines(Mat & inputOutput, const Mat & kernel)
{
    erode(inputOutput, inputOutput, kernel, Point(-1, -1));
    dilate(inputOutput, inputOutput, kernel, Point(-1, -1));
    cvtColor(inputOutput, inputOutput, COLOR_GRAY2BGR);

    Mat mask;
    inRange(inputOutput, Scalar(255,255,255), Scalar(255,255,255), mask);
    inputOutput.setTo(Scalar(rand() % 255, rand() % 255, rand() % 255), mask);
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