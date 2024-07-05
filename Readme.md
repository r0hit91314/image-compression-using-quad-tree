# Image Compression using Quadtree
Our project on focuses on 'Image Compression using quad trees'. It is a method to reduce the file size of an image while maintaining its quality. It does this by dividing the image into smaller regions and representing these regions using a hierarchical data structure called a quadtree. Regions that are uniform or have similar pixel values are compressed, resulting in a smaller file size. This technique is used in applications where preserving image quality is essential, such as in medical imaging or satellite imagery.
Our project focuses on method to reduce the file size of an image while maintaining its quality. It does this by dividing the image into smaller regions and representing these regions using a hierarchical data structure called a quadtree. Regions that are uniform or have similar pixel values are compressed, resulting in a smaller file size. This technique is used in applications where preserving image quality is essential, such as in medical imaging or satellite imagery.



# How to use our program?
Follow the below instructions<br>
To download our program, please copy and paste this line to your terminal:
```
git clone https://github.com/r0hit91314/image-compression-using-quad-tree.git```
```
### Further Steps to run Image Compression
It is expected that your system has OpenCV library installed (it has been  used for reading and writing images as we cant do this directly in C/C++). If not steps for the same are given below.
***Steps to install OpenCV***
Build and install OpenCV in your machine if you already haven't. Here are guides to do the same:
* [For Ubuntu](http://techawarey.com/programming/install-opencv-c-c-in-ubuntu-18-04-lts-step-by-step-guide/) 
* [For Windows](https://cv-tricks.com/how-to/installation-of-opencv-4-1-0-in-windows-10-from-source/)
* [For MacOS](https://docs.opencv.org/master/d0/db2/tutorial_macos_install.html)
***Recommendation***: Installing OpenCV on windows is a tediuos task and not advised. For easier and quicker installation use Ubuntu(Best Choice) or MacOS(Second Best Choice). If you are on a windows machine, and dont have a Linux distribution installed in your system, you may use a virtual machine.
***Linux***<br>
After installing OpenCV, type ```make``` in your terminal. This will produce an output file ***test***. To run this file type ```./test``` in your terminal. If makefile gives error follow the steps mentioned for Windows.
***Windows***<br>
After installing OpenCV,type the below command in your terminal:
```
g++ main.cpp image.cpp image.hpp -o test -std=c++11 `pkg-config --cflags --libs opencv`
```
This will generate an output file ***test.exe***. To run this file type ```./test```  or ```./test.exe``` in your terminal.
### Further Common Steps for any OS
After this the programme will ask you the path of image which is to compressed. After you enter the required path, it will ask you if you want to proceed with default setting for image variance(a measure for image compression) or want to change it. After providing the required inputs the programme will display the original and compressed file. The compressed file will also be saved with a name ***compressed_image.jpeg*** in the directory where code is present. 

---
### Demonstration

Here is the demonstration of the images before and after the compression:

1. **Original Image (Size 4.41 MB)**
   ![Original Image](input.jpg)


4. **Compressed Image (Size 670 KB)**
   ![Compressed Image](compressed_image.jpg)

---

**Group Members**
| Name            | Entry Number |
| --------------- | ------------ |
| Pankaj Kumar    | 2022MCB1274  |
| Rohit Agrawal   | 2022MCB1277  |
---
**Project Mentor**
- Mr. Neeraj Dwivedi
