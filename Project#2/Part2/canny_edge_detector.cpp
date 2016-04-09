//
//  main.cpp
//  canny_edge_dect using opencv
//
//  Created by Yi Zheng on 10/01/15.
//  Copyright (c) 2015 Ein. All rights reserved.
//
#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char* argv[])
{
        double low,high,max_mag; // define the low threshold and high threshold
        max_mag = 195*4; // max magnitude calculated from sobel edge detector // farm:246 cougar:195
        low=max_mag*0.4;
        high=max_mag*0.5;
        // read the original iamge
        Mat src = imread("/Users/YiZheng/Desktop/Codes/canny_edge_dect/canny_edge_dect/cougar_gray.jpg");
        // define matrix to store image data
        Mat dst;
        // applying canny edge detection
        Canny(src,dst,low,high,3);
        dst = 255-dst;
        // output the edge map
        imwrite("/Users/YiZheng/Desktop/Codes/canny_edge_dect/canny_edge_dect/cougar_0.4_0.5.jpg",dst);
        waitKey();
        return 0;
}