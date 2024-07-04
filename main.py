import numpy as np
import cv2

# Node structure for the quad tree
class QuadTreeNode:
    def __init__(self):
        self.value = 0  # Grayscale value of the node
        self.isLeaf = False  # Is this node a leaf node?
        self.children = [None] * 4  # Four children for the quad tree

# Function to create a quad tree for the image
def buildQuadTree(image, x, y, width, height, threshold):
    node = QuadTreeNode()
    sum_pixels = np.sum(image[y:y + height, x:x + width])

    if width == 1 and height == 1:
        node.value = image[y, x]
        node.isLeaf = True
        return node

    if width > 0 and height > 0:
        node.value = sum_pixels // (width * height)
    else:
        node.value = 0  # Handle edge case where width or height is zero

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
        compressedImage[:] = node.value
    else:
        halfWidth = compressedImage.shape[1] // 2
        halfHeight = compressedImage.shape[0] // 2

        compressImage(node.children[0], compressedImage[:halfHeight, :halfWidth])
        compressImage(node.children[1], compressedImage[:halfHeight, halfWidth:])
        compressImage(node.children[2], compressedImage[halfHeight:, :halfWidth])
        compressImage(node.children[3], compressedImage[halfHeight:, halfWidth:])

def main():
    # Load the grayscale image
    image = cv2.imread("input.png", cv2.IMREAD_GRAYSCALE)
    
    if image is None:
        print("Error: Unable to load image.")
        return
    
    # Set the threshold for compression
    threshold = 3  # Adjust this threshold for better compression
    
    # Build the quad tree
    root = buildQuadTree(image, 0, 0, image.shape[1], image.shape[0], threshold)
    
    # Create a compressed image
    compressedImage = np.zeros_like(image)
    
    # Compress the image using the quad tree
    compressImage(root, compressedImage)
    
    # Save the compressed image 
    cv2.imwrite("modified.jpg", compressedImage)
    
    # Destroy the quad tree
    destroyQuadTree(root)
    
    print("Image compression complete.")

if __name__ == "__main__":
    main()
