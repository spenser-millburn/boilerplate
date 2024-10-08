#include <pybind11/pybind11.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <iostream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

// Function to convert OpenCV Mat to Eigen matrix
void cv2eigen(const cv::Mat& src, Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& dst) {
    dst = Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(src.data, src.rows, src.cols);
}

// Function to convert Eigen matrix back to OpenCV Mat
void eigen2cv(const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& src, cv::Mat& dst) {
    dst = cv::Mat(src.rows(), src.cols(), CV_8UC1, const_cast<unsigned char*>(src.data()));
}

// Function to load, process, and save the image
int flipped_grayscale(const std::string &image_path, const std::string &output_path) {
    // Load the image
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image." << std::endl;
        return -1;
    }

    // Convert the image to an Eigen matrix
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;
    cv2eigen(image, eigenImage);

    // Transform the image (flip it)
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.colwise().reverse();

    // Convert back to OpenCV Mat
    cv::Mat flippedMat;
    eigen2cv(flippedImage, flippedMat);

    // Save the flipped image
    cv::imwrite(output_path, flippedMat);

    return 0; // Success
}

PYBIND11_MODULE(_core, m) {
    m.def("flipped_grayscale", &flipped_grayscale, "A function that transforms (flips) an image and saves it to the output path.");
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: scikit_build_example

        .. autosummary::
           :toctree: _generate
            flipped_grayscale 

    )pbdoc";
    m.attr("__version__") = "dev";
}
