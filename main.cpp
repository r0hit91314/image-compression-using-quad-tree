#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Node structure for the quad tree
struct QuadTreeNode {
    uchar value;  // Grayscale value of the node
    bool isLeaf; // Is this node a leaf node?
    QuadTreeNode* children[4]; // Four children for the quad tree

    QuadTreeNode() {
        value = 0;
        isLeaf = false;
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }
};

// Function to create a quad tree for the image
QuadTreeNode* buildQuadTree(const Mat& image, int x, int y, int width, int height, uchar threshold) {
    QuadTreeNode* node = new QuadTreeNode();
    int sum = 0;

    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            sum += image.at<uchar>(j, i);
        }
    }

    node->value = sum / (width * height);

    if (width == 1 && height == 1) {
        node->isLeaf = true;
        return node;
    }

    if (width > 1 && height > 1) {
        bool homogeneous = true;
        for (int i = x; i < x + width; i++) {
            for (int j = y; j < y + height; j++) {
                if (abs(image.at<uchar>(j, i) - node->value) > threshold) {
                    homogeneous = false;
                    break;
                }
            }
        }

        if (homogeneous) {
            node->isLeaf = true;
            return node;
        }
    }

    int halfWidth = width / 2;
    int halfHeight = height / 2;

    node->children[0] = buildQuadTree(image, x, y, halfWidth, halfHeight, threshold);
    node->children[1] = buildQuadTree(image, x + halfWidth, y, halfWidth, halfHeight, threshold);
    node->children[2] = buildQuadTree(image, x, y + halfHeight, halfWidth, halfHeight, threshold);
    node->children[3] = buildQuadTree(image, x + halfWidth, y + halfHeight, halfWidth, halfHeight, threshold);

    return node;
}

// Function to destroy the quad tree
void destroyQuadTree(QuadTreeNode* node) {
    if (node != nullptr) {
        for (int i = 0; i < 4; i++) {
            destroyQuadTree(node->children[i]);
        }
        delete node;
    }
}

// Function to compress the image using the quad tree
void compressImage(QuadTreeNode* node, Mat& compressedImage) {
    if (node->isLeaf) {
        for (int i = 0; i < compressedImage.cols; i++) {
            for (int j = 0; j < compressedImage.rows; j++) {
                compressedImage.at<uchar>(j, i) = node->value;
            }
        }
    } else {
        int halfWidth = compressedImage.cols / 2;
        int halfHeight = compressedImage.rows / 2;

        compressImage(node->children[0], compressedImage(Rect(0, 0, halfWidth, halfHeight)));
        compressImage(node->children[1], compressedImage(Rect(halfWidth, 0, halfWidth, halfHeight)));
        compressImage(node->children[2], compressedImage(Rect(0, halfHeight, halfWidth, halfHeight)));
        compressImage(node->children[3], compressedImage(Rect(halfWidth, halfHeight, halfWidth, halfHeight)));
    }
}

int main() {
    // Load the grayscale image
    Mat image = imread("input_image.jpg", IMREAD_GRAYSCALE);
    
    if (image.empty()) {
        cerr << "Error: Unable to load image." << endl;
        return -1;
    }

    // Set the threshold for compression
    uchar threshold = 10;

    // Build the quad tree
    QuadTreeNode* root = buildQuadTree(image, 0, 0, image.cols, image.rows, threshold);

    // Create a compressed image
    Mat compressedImage = Mat::zeros(image.size(), CV_8U);

    // Compress the image using the quad tree
    compressImage(root, compressedImage);

    // Save the compressed image
    imwrite("compressed_image.jpg", compressedImage);

    // Destroy the quad tree
    destroyQuadTree(root);

    cout << "Image compression complete." << endl;

    return 0;
}
