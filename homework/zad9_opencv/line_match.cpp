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

auto lineMatchFunction = [](const cv::Size& size, cv::Mat& bw_image)
{
    // Create structure element for extracting horizontal/vertical lines through morphology operations
    cv::Mat imgStructure = cv::getStructuringElement(cv::MORPH_RECT, size);

    // Apply morphology operations
    cv::erode(bw_image, bw_image, imgStructure, cv::Point(-1, -1));
    cv::dilate(bw_image, bw_image, imgStructure, cv::Point(-1, -1));

    return bw_image;
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

    // Create the images that will use to extract the horizontal and vertical lines
    cv::Mat horizontal = bw.clone();
    cv::Mat vertical = bw.clone();

    // Specify size on horizontal axis
    int horizontal_size_value = horizontal.cols / 30;
    cv::Size horSize(horizontal_size_value, 1);

    // Specify size on vertical axis
    int vertical_size_value = vertical.rows / 30;
    cv::Size verSize(1, vertical_size_value);

    std::future<cv::Mat> hor_result = std::async(std::launch::async, lineMatchFunction, std::cref(horSize), std::ref(horizontal));
    std::future<cv::Mat> ver_result = std::async(std::launch::async, lineMatchFunction, std::cref(verSize), std::ref(vertical));

    // Show extracted horizontal lines
    show_wait_destroy("horizontal", hor_result.get());

    // Show extracted vertical lines
    show_wait_destroy("vertical", ver_result.get());

    // Inverse vertical image
    cv::bitwise_not(vertical, vertical);
    show_wait_destroy("vertical_bit", vertical);
    
    // Extract edges and smooth image according to the logic
    // 1. extract edges
    // 2. dilate(edges)
    // 3. src.copyTo(smooth)
    // 4. blur smooth img
    // 5. smooth.copyTo(src, edges)
    // Step 1
    cv::Mat edges;
    cv::adaptiveThreshold(vertical, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 3, -2);
    show_wait_destroy("edges", edges);

    // Step 2
    cv::Mat kernel = cv::Mat::ones(2, 2, CV_8UC1);
    cv::dilate(edges, edges, kernel);
    show_wait_destroy("dilate", edges);
    
    // Step 3
    cv::Mat smooth;
    vertical.copyTo(smooth);
    
    // Step 4
    cv::blur(smooth, smooth, cv::Size(2, 2));
    
    // Step 5
    smooth.copyTo(vertical, edges);
    
    // Show final result
    show_wait_destroy("smooth - final", vertical);

    return 0;
}