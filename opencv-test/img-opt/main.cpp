#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 读取输入图像
    cv::Mat src = cv::imread("test.png");
    if (src.empty()) {
        std::cerr << "Failed to read input image!" << std::endl;
        return -1;
    }

    // 计算放大倍数
    double scale = 2.0;

    // 计算输出图像尺寸
    int dstWidth = static_cast<int>(src.cols * scale);
    int dstHeight = static_cast<int>(src.rows * scale);

    // 创建输出图像
    cv::Mat dst(dstHeight, dstWidth, src.type());

    // 生成映射表
    cv::Mat mapX(dstHeight, dstWidth, CV_32FC1);
    cv::Mat mapY(dstHeight, dstWidth, CV_32FC1);
    for (int y = 0; y < dstHeight; y++) {
        for (int x = 0; x < dstWidth; x++) {
            mapX.at<float>(y, x) = static_cast<float>(x / scale);
            mapY.at<float>(y, x) = static_cast<float>(y / scale);
        }
    }

    // 执行图像放大
    cv::remap(src, dst, mapX, mapY, cv::INTER_LINEAR);

    // 显示结果图像
    cv::imshow("Input", src);
    cv::imshow("Output", dst);
    cv::waitKey(0);

    return 0;
}
