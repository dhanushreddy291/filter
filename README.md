# A Simple C-Program to Find the Boundary Edges of an Image using [Sobel Edge Detection Operator](https://en.wikipedia.org/wiki/Sobel_operator).

# How to Run the Program
## Make an Executable .filter/ to run the prorgram using the command
        make filter

## To find edges/boundary of image using Sobel-Feldman Operator run
        ./filter -e inputimage.bmp outputimage.bmp

### A Test Image of a Tower is added in repo as an example.

## To blur an image run
        ./filter -b inputimage.bmp outputimage.bmp

## To convert an image to grayscale run
        ./filter -g inputimage.bmp outputimage.bmp

## To mirror an image run
        ./filter -m inputimage.bmp outputimage.bmp
