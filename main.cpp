#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Define a structure to represent nodes in a Quadtree
struct QuadTreeNode {
    int color; // Grayscale color value for the node
    bool isLeaf; // Indicates whether the node is a leaf or not
    QuadTreeNode* children[4]; // Pointers to child nodes

    // Constructor to initialize the node with a color value
    QuadTreeNode(int c) : color(c), isLeaf(true) {
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }
};

// Function to insert a pixel into the Quadtree
void insertPixel(QuadTreeNode* node, Mat& image, int x, int y, int size) {
    if (size == 1) {
        // If the size is 1x1, store the color value and mark the node as a leaf
        node->color = image.at<uchar>(y, x);
        node->isLeaf = true;
        return;
    }

    // If the node represents a larger area, it's not a leaf
    node->isLeaf = false;
    int halfSize = size / 2;

    for (int i = 0; i < 4; i++) {
        // Calculate the coordinates of the four child nodes
        int newX = x + (i % 2) * halfSize;
        int newY = y + (i / 2) * halfSize;

        if (node->children[i] == nullptr) {
            // If the child node doesn't exist, create it
            node->children[i] = new QuadTreeNode(0);
        }

        // Recursively insert the pixel into the child node
        insertPixel(node->children[i], image, newX, newY, halfSize);
    }
}

// Function to compress the image using the Quadtree
void compressImage(QuadTreeNode* root, Mat& compressedImage, int x, int y, int size) {
    if (root->isLeaf) {
        // If the node is a leaf, set the color of the corresponding pixels in the compressed image
        for (int i = y; i < y + size; i++) {
            for (int j = x; j < x + size; j++) {
                compressedImage.at<uchar>(i, j) = static_cast<uchar>(root->color);
            }
        }
    } else {
        int halfSize = size / 2;

        for (int i = 0; i < 4; i++) {
            // Calculate the coordinates of the four child nodes
            int newX = x + (i % 2) * halfSize;
            int newY = y + (i / 2) * halfSize;

            // Recursively compress the child node
            compressImage(root->children[i], compressedImage, newX, newY, halfSize);
        }
    }
}

int main() {
    // Load the input image
    Mat image = imread("input.jpg", IMREAD_GRAYSCALE);

    if (image.empty()) {
        cerr << "Failed to open the image!" << endl;
        return -1;
    }

    int width = image.cols;
    int height = image.rows;

    // Create a blank image for the compressed result
    Mat compressedImage(height, width, CV_8UC1, Scalar(0));

    // Create the root of the Quadtree
    QuadTreeNode* root = new QuadTreeNode(0);

    // Insert each pixel into the Quadtree
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            insertPixel(root, image, x, y, width);
        }
    }

    // Compress the image using the Quadtree
    compressImage(root, compressedImage, 0, 0, width);

    // Save the compressed image
    imwrite("compressed.jpg", compressedImage);

    return 0;
}
