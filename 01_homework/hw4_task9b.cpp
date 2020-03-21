#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <future>

using namespace std;
using namespace cv;

void show_wait_destroy(const char* winname, cv::Mat img) {
    imshow(winname, img);
    moveWindow(winname, 500, 0);
    waitKey(0);
    destroyWindow(winname);
}

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | notes.png | input image}");
    Mat src = imread( samples::findFile( parser.get<String>("@input") ), IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }

    // Show source image
    imshow("src", src);

    // Transform source image to gray if it is not already
    Mat gray;
    if (src.channels() == 3)
    {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    }
    else
    {
        gray = src;
    }

    // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    Mat bw;
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);

    // Create the images that will use to extract the horizontal and vertical lines
    Mat horizontal = bw.clone();
    Mat vertical = bw.clone();

    // Specify size on horizontal/vertical axis
    int horizontal_size = horizontal.cols / 30;
    int vertical_size = vertical.rows / 30;

    // // Create structure element for extracting horizontal/vertical lines through morphology operations
    // Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));
    // Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, vertical_size));

    // Apply morphology operations
    auto morphFunction = [](Mat& bw_image, const int param_a, const int param_b)
    {
        // Create structure element for extracting horizontal/vertical lines through morphology operations
        cv::Mat imgStructure = cv::getStructuringElement(cv::MORPH_RECT,
                                                         Size(param_a, param_b));

        // Apply morphology operations
        cv::erode(bw_image, bw_image, imgStructure, cv::Point(-1, -1));
        cv::dilate(bw_image, bw_image, imgStructure, cv::Point(-1, -1));

        return bw_image;
    };

    std::future<cv::Mat> horizontal_result = std::async(std::launch::async,
                                                        morphFunction,
                                                        std::ref(horizontal),
                                                        horizontal_size,
                                                        1);
    std::future<cv::Mat> vertical_result = std::async(std::launch::async,
                                                      morphFunction,
                                                      std::ref(vertical),
                                                      1,
                                                      vertical_size);

    // Show extracted horizontal lines
    show_wait_destroy("horizontal", horizontal_result.get());

    // Show extracted vertical lines
    show_wait_destroy("vertical", vertical_result.get());
    return 0;
}
