#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

using namespace cv;

std::mutex m;

Mat img;
Mat result;

const char *image_window = "Source Image";
int match_method;

void MatchingMethod(const Mat &, const Mat &, Mat &, int);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "Not enough parameters" << std::endl;
        std::cout << "Usage:\n"
             "<match_method> <image_name> <template1_name> <template2_name> ..." << std::endl;
        std::cout << "Match methods:\n"
            "0 - TM_SQDIFF, 1 - TM_SQDIFF_NORMED, 2 - TM_CCORR, 3 - TM_CCORR_NORMED, 4- TM_CCOEFF, 5 - TM_CCOEFF_NORMED\n";
        return -1;
    }

    if (atoi(argv[1]) > 5)
    {
        std::cout << "Choose one of the following matching methods " << std::endl;
        std::cout << "0 - TM_SQDIFF, 1 - TM_SQDIFF_NORMED, 2 - TM_CCORR, 3 - TM_CCORR_NORMED, 4- TM_CCOEFF, 5 - TM_CCOEFF_NORMED\n";
        return -1;
    }

    int templates = argc - 3;
    Mat templ[templates];
    match_method = atoi(argv[1]);

    img = imread(argv[2], IMREAD_COLOR);

    for (size_t i = 0; i < templates; i++)
        templ[i] = imread(argv[i + 3], IMREAD_COLOR);

    if (img.empty() || std::any_of(templ, templ + templates, [](const Mat & x){ return x.empty(); }))
    {
        std::cout << "Can't read one of the images" << std::endl;
        return EXIT_FAILURE;
    }

    namedWindow(image_window, WINDOW_AUTOSIZE);

    Mat img_display;
    img.copyTo(img_display);

    std::vector<std::thread> threads;

    for (int i = 0; i < templates; i++)
        threads.emplace_back(std::thread(MatchingMethod, img, templ[i], std::ref(img_display), match_method));
    
    for (auto &&i : threads)
        i.join();

    imshow(image_window, img_display);
    waitKey(0);

    return EXIT_SUCCESS;
}

void MatchingMethod(const Mat & img, const Mat & templ, Mat & img_display, int method)
{
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create(result_rows, result_cols, method);

    matchTemplate(img, templ, result, method);

    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    matchLoc = minLoc;

    std::unique_lock<std::mutex> lock(m);
    rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
}