#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include <future>

using namespace std;
using namespace cv;

int error_msg() {
    cout << "Can't read one of the images" << endl;
    return EXIT_FAILURE;
}

int main(int argc, char** argv)
{
    // Validate input args
    if (argc < 3) {
        cout << "Not enough parameters" << endl;
        cout << "Usage:\n" << argv[0] << " <image_name> <template1_name> [template2_name ...]" << endl;
        return -1;
    }

    const int no_of_templates = argc - 2;

    // Validate source image
    Mat source = imread(argv[1], IMREAD_COLOR);
    if(source.empty())
        return error_msg();

    // Validate templates
    vector<Mat> templates(no_of_templates);
    for (int i = 0; i < no_of_templates; ++i) {
        templates[i] = imread(argv[i + 2], cv::IMREAD_COLOR);
        if(templates[i].empty())
            return error_msg();
    }

    // Matchmaking function that results Point
    auto matchingMethod = [](Mat img, Mat templ) {
        Mat result;

        int result_cols =  img.cols - templ.cols + 1;
        int result_rows = img.rows - templ.rows + 1;

        result.create(result_rows, result_cols, CV_32FC1);

        matchTemplate(img, templ, result, TM_SQDIFF);
        normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;

        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
        return minLoc;
    };
    // Copy source image for output
    Mat img_display;
    source.copyTo(img_display);

    // Run threads for matchmaking
    vector<std::future<cv::Point>> results(no_of_templates);
    for(int i = 0; i < no_of_templates; ++i) {
        results[i] = std::async(std::launch::async,
                                matchingMethod,
                                ref(source),
                                ref(templates[i]));
    }

    // Collects result and draw rectangles around them
    for(int i = 0; i < no_of_templates; ++i) {
        Point p = results[i].get();

        rectangle(img_display,
                  p,
                  Point(p.x + templates[i].cols , p.y + templates[i].rows),
                  Scalar::all(0),
                  2,
                  8,
                  0);

    }

    // Show result
    const char* image_window = "Source Image";
    namedWindow(image_window, WINDOW_AUTOSIZE);
    imshow(image_window, img_display);
    waitKey(0);

    return EXIT_SUCCESS;
}
