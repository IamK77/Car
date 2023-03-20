#include <iostream>
#include <opencv2/opencv.hpp>
#include <zbar.h>

using namespace std;
using namespace cv;
using namespace zbar;

int main(int argc, char* argv[])
{
    // 加载图像
    Mat img = imread("qrcode.png");

    // 解码二维码
    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    int width = gray.cols;
    int height = gray.rows;
    uchar *raw = (uchar *)gray.data;
    Image image(width, height, "Y800", raw, width * height);
    scanner.scan(image);

    // 打印结果
    for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
    {
        cout << symbol->get_data() << endl;
    }

    return 0;
}
