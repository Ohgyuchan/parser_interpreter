#include "opencv/cv.hpp"

using namespace cv;

int main() {
    Mat lena, sobel, canny;
    lena = imread("lena.png", 0);

    imshow("Lena", lena);
    int ddepth = CV_8U;

    Canny(lena, canny, 190, 200, 3);
    imshow("Canny", canny);
    lena.copyTo(sobel);
    Sobel(lena, sobel, ddepth, 1, 1);
    threshold(sobel, sobel, 25, 255, THRESH_BINARY);

    imshow("Sobel",sobel);

    waitKey(0);
    return 0;
}