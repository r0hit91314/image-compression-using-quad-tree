import numpy as np
import cv2

# Node structure for the quad tree
class QuadTreeNode:
    def __init__(self):
        self.value = [0, 0, 0]  # RGB value of the node
        self.isLeaf = False  # Is this node a leaf node?
        self.children = [None] * 4  # Four children for the quad tree

# Function to create a quad tree for the image
def buildQuadTree(image, x, y, width, height, threshold):
    node = QuadTreeNode()
    sum_pixels = np.sum(image[y:y + height, x:x + width], axis=(0, 1))

    if width == 1 and height == 1:
        node.value = image[y, x].tolist()
        node.isLeaf = True
        return node

    if width > 0 and height > 0:
        node.value = (sum_pixels // (width * height)).tolist()
    else:
        node.value = [0, 0, 0]  # Handle edge case where width or height is zero

    if width > 1 and height > 1:
        homogeneous = np.all(np.abs(image[y:y + height, x:x + width] - node.value) <= threshold)

        if homogeneous:
            node.isLeaf = True
            return node

    halfWidth = max(1, width // 2)
    halfHeight = max(1, height // 2)

    node.children[0] = buildQuadTree(image, x, y, halfWidth, halfHeight, threshold)
    node.children[1] = buildQuadTree(image, x + halfWidth, y, halfWidth, halfHeight, threshold)
    node.children[2] = buildQuadTree(image, x, y + halfHeight, halfWidth, halfHeight, threshold)
    node.children[3] = buildQuadTree(image, x + halfWidth, y + halfHeight, halfWidth, halfHeight, threshold)

    return node

# Function to destroy the quad tree
def destroyQuadTree(node):
    if node is not None:
        for child in node.children:
            destroyQuadTree(child)
        del node

# Function to compress the image using the quad tree
def compressImage(node, compressedImage):
    if node.isLeaf:
        compressedImage[:, :] = node.value
    else:
        halfWidth = compressedImage.shape[1] // 2
        halfHeight = compressedImage.shape[0] // 2

        compressImage(node.children[0], compressedImage[:halfHeight, :halfWidth])
        compressImage(node.children[1], compressedImage[:halfHeight, halfWidth:])
        compressImage(node.children[2], compressedImage[halfHeight:, :halfWidth])
        compressImage(node.children[3], compressedImage[halfHeight:, halfWidth:])

def main():
    # Load the color image
    image = cv2.imread("input.jpg")
    
    if image is None:
        print("Error: Unable to load image.")
        return
    
    # User chooses the threshold for compression
    print("Input Threshold")
    print("Choose greater than 5 for good compression")
    print("Choose 5 for better compression")
    print("Choose lesser than 5 for best compression")
    print("More the value of threshold, lesser will be the runtime")
    
    try:
        threshold = int(input("Threshold: "))
    except ValueError:
        print("Invalid input. Please enter an integer value.")
        return
    
    # Build the quad tree
    root = buildQuadTree(image, 0, 0, image.shape[1], image.shape[0], threshold)
    
    # Create a compressed image
    compressedImage = np.zeros_like(image)
    
    # Compress the image using the quad tree
    compressImage(root, compressedImage)
    
    # Save the compressed image with lower quality
    cv2.imwrite("compressed_image.jpg", compressedImage, [cv2.IMWRITE_JPEG_QUALITY, 50])
    
    # Destroy the quad tree
    destroyQuadTree(root)
    
    print("Image compression complete.")

if __name__ == "__main__":
    main()
