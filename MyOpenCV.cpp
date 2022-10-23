#include "MyOpenCV.h"


cv::Mat toCvMat(ImageRGB& rgb)
{
	cv::Mat src_mat;
	src_mat.create(rgb.getHeight(), rgb.getWidth(), CV_8UC3);
	memcpy(src_mat.data, rgb.getData(), 1.0*rgb.getWidth() * rgb.getHeight() * sizeof(PixelRGB));
	return src_mat;
}

cv::Mat dnnExecute(cv::Mat img, const char* model)
{
	cv::dnn::Net net = cv::dnn::readNetFromONNX(model);
	cv::Mat blob;
	cv::dnn::blobFromImage(img, blob);
	net.setInput(blob);
	cv::Mat predict = net.forward();
	return predict;
}


