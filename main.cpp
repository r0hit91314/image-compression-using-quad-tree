#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct QuadTreeNode {
    int color; // Grayscale color value
    bool isLeaf;
    QuadTreeNode* children[4];

    QuadTreeNode() : color(0), isLeaf(false) {
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }
};

void insertPixel(QuadTreeNode* node, Mat& image, int x, int y, int size) {
    if (size == 1) {
        node->color = static_cast<int>(image.at<uchar>(y, x));
        node->isLeaf = true;
        return;
    }

    node->isLeaf = false;
    int halfSize = size / 2;

    for (int i = 0; i < 4; i++) {
        int newX = x + (i % 2) * halfSize;
        int newY = y + (i / 2) * halfSize;

        if (node->children[i] == nullptr) {
            node->children[i] = new QuadTreeNode();
        }

        insertPixel(node->children[i], image, newX, newY, halfSize);
    }
}

void compressImage(QuadTreeNode* root, Mat& compressedImage, int x, int y, int size) {
    if (root->isLeaf) {
        for (int i = y; i < y + size; i++) {
            for (int j = x; j < x + size; j++) {
                compressedImage.at<uchar>(i, j) = static_cast<uchar>(root->color);
            }
        }
    } else {
        int halfSize = size / 2;

        for (int i = 0; i < 4; i++) {
            int newX = x + (i % 2) * halfSize;
            int newY = y + (i / 2) * halfSize;

            compressImage(root->children[i], compressedImage, newX, newY, halfSize);
        }
    }
}

int main() {
    Mat image = imread("input.jpg", IMREAD_GRAYSCALE);

    if (image.empty()) {
        cerr << "Failed to open the image!" << endl;
        return -1;
    }

    int width = image.cols;
    int height = image.rows;

    Mat compressedImage(height, width, CV_8UC1, Scalar(0));
    QuadTreeNode* root = new QuadTreeNode();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            insertPixel(root, image, x, y, width);
        }
    }

    compressImage(root, compressedImage, 0, 0, width);

    imwrite("compressed.jpg", compressedImage);

    return 0;
}
