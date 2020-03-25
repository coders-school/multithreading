#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <future>

using namespace std;
using namespace cv;

constexpr int angle_step = 5;

void show_wait_destroy(const char* winname, cv::Mat img) {
    imshow(winname, img);
    moveWindow(winname, 500, 0);
    waitKey(0);
    destroyWindow(winname);
}

Mat get_diagonal_line_mask(int line_size, int angle) {
    double angle_rad = ( angle * M_PI ) / 180.0;

    // Create mask of proper size
    Mat test_image(abs((line_size * cos(angle_rad))) + 1,
                   abs((line_size * sin(angle_rad))) + 1,
                   CV_8U);

    // Create rotated rectagle and width = 0
    RotatedRect rRect = RotatedRect(Point2f(abs((line_size/2 * sin(angle_rad))),
                                            abs((line_size/2 * cos(angle_rad)))),
                                    Size2f(0, line_size),
                                    angle);

    // Get longer side as mask
    Point2f vertices[4];
    rRect.points(vertices);
    line(test_image, vertices[0], vertices[1], Scalar(255,255,255), 1);

    return test_image;
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

    // Specify axis lenght
    int line_lenght = ((src.cols / 30) + (src.rows / 30) / 2);

    // Apply morphology operations
    auto morphFunction = [](Mat bw_image, const int line_lenght, const int angle)
    {
        // Create structure element for extracting horizontal/vertical lines through morphology operations
        Mat imgStructure = get_diagonal_line_mask(line_lenght, angle);

        // Apply morphology operations
        erode(bw_image, bw_image, imgStructure, Point(-1, -1));
        dilate(bw_image, bw_image, imgStructure, Point(-1, -1));

        return bw_image;
    };

    vector<future<Mat>> results;
    for( int angle = 0; angle < 180; angle += angle_step) {
        // Launch thread for line matching
        results.push_back(std::async(std::launch::async,
                                     morphFunction,
                                     bw.clone(),
                                     line_lenght,
                                     angle));
    }

    // Coloring step helpers
    int r_step = 256 / (180 / angle_step);
    int g_step = r_step * 3;
    int b_step = g_step * 3;

    // Show extracted lines
    Mat result_img = src.clone();
    for(auto i = 0; i < static_cast<int>(results.size()); ++i) {
        // Add detected lines to final result
        result_img.setTo(cv::Scalar((i * r_step) % 255,
                                    (i * g_step) % 255,
                                    (i * b_step) % 255),
                         results[i].get());
    }
    show_wait_destroy("result", result_img);
    return 0;
}
