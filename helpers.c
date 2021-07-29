#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
    {
        //For Calculating the average value of Red, Green and Blue.
        int averagergb = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.00);

        //Sets Red, Green and Blue values to the average value so that images is grayscaled.
        image[i][j].rgbtBlue = averagergb;
        image[i][j].rgbtGreen = averagergb;
        image[i][j].rgbtRed = averagergb;

    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //A temporary pixel struct while we run the code.
    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    for (int j = 0; j < width / 2; j++)
    {
        //Swapping two pixels on either side of image, swapped using temp.
        temp.rgbtBlue = image[i][j].rgbtBlue;
        temp.rgbtGreen = image[i][j].rgbtGreen;
        temp.rgbtRed = image[i][j].rgbtRed;

        image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
        image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
        image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

        image[i][width - j - 1].rgbtBlue = temp.rgbtBlue;
        image[i][width - j - 1].rgbtGreen = temp.rgbtGreen;
        image[i][width - j - 1].rgbtRed = temp.rgbtRed;

    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //A temporary image bitmap for running the code.
    RGBTRIPLE tempimage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sum[3] = {0.00, 0.00, 0.00};

            //A counter to count the number of neighbouring pixels including itself, normally 9 if in middle, 6 in edges and 4 at corner.
            int counter = 0;

            //Calculates average value of pixel depending upon various conditions. i.e, edge, corner or middle.
            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {

                    //To make sure pixel is within the range of image dimensions.
                    if ((k >= 0 && k < height))
                    {
                        if ((l >= 0 && l < width))
                        {
                            sum[0] += image[k][l].rgbtBlue;
                            sum[1] += image[k][l].rgbtGreen;
                            sum[2] += image[k][l].rgbtRed;
                            counter++;
                        }
                    }
                }
            }

            //Average Value of Blue, Green and Red respectively at any specific [i,j]
            sum[0] /= counter;
            sum[1] /= counter;
            sum[2] /= counter;

            //Makes sure that the 8 bit color value is less than or equal to 255, just incase if it goes above.
            if (sum[0] >= 255)
            {
                tempimage[i][j].rgbtBlue = 255;
            }
            else
            {
                tempimage[i][j].rgbtBlue = round(sum[0]);
            }

            if (sum[1] >= 255)
            {
                tempimage[i][j].rgbtGreen = 255;
            }
            else
            {
                tempimage[i][j].rgbtGreen = round(sum[1]);
            }

            if (sum[2] >= 255)
            {
                tempimage[i][j].rgbtRed = 255;
            }
            else
            {
                tempimage[i][j].rgbtRed = round(sum[2]);
            }
        }
    }

    //Copies the image bitmap we generated from tempimage to orginal image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = tempimage[i][j].rgbtBlue;
            image[i][j].rgbtGreen = tempimage[i][j].rgbtGreen;
            image[i][j].rgbtRed = tempimage[i][j].rgbtRed;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Creates a temporary image bitmap that contains two extra units of height and width.
    RGBTRIPLE tmpimage[height + 2][width + 2];

    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            //Copies the image bitmap to temorary image bitmap leaving the border.
            tmpimage[i][j].rgbtBlue = image[i - 1][j - 1].rgbtBlue;
            tmpimage[i][j].rgbtGreen = image[i - 1][j - 1].rgbtGreen;
            tmpimage[i][j].rgbtRed = image[i - 1][j - 1].rgbtRed;

        }

    }

    //Sets the Border values of the bitmap to zero for the temporary image file.
    for (int i = 0;  i < height + 2; i++)
    {
        tmpimage[i][0].rgbtBlue = 0;
        tmpimage[i][width + 1].rgbtBlue = 0;


        tmpimage[i][0].rgbtGreen = 0;
        tmpimage[i][width + 1].rgbtGreen = 0;

        tmpimage[i][0].rgbtRed = 0;
        tmpimage[i][width + 1].rgbtRed = 0;

    }

    //Sets the Border values of the bitmap to zero for the temporary image file.
    for (int j = 0;  j < width + 2; j++)
    {
        tmpimage[0][j].rgbtBlue = 0;
        tmpimage[height + 1][j].rgbtBlue = 0;

        tmpimage[0][j].rgbtGreen = 0;
        tmpimage[height + 1][j].rgbtGreen = 0;

        tmpimage[0][j].rgbtRed = 0;
        tmpimage[height + 1][j].rgbtRed = 0;

    }

    //Calculating the Sobel Oerator Value using kernels as per definition.
    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            //First 6 values of sum array contains the Gx's and Gy's : Blue, Green and Red values respectively and the next 3 values are Sobel Operator answer Values.
            double sum[9];

            sum[0] = (-tmpimage[i - 1][j - 1].rgbtBlue) + (tmpimage[i - 1][j + 1].rgbtBlue) - (2 * tmpimage[i][j - 1].rgbtBlue) +
                     (2 * tmpimage[i][j + 1].rgbtBlue) - (tmpimage[i + 1][j - 1].rgbtBlue) + (tmpimage[i + 1][j + 1].rgbtBlue);

            sum[1] = (-tmpimage[i - 1][j - 1].rgbtGreen) + (tmpimage[i - 1][j + 1].rgbtGreen) - (2 * tmpimage[i][j - 1].rgbtGreen) +
                     (2 * tmpimage[i][j + 1].rgbtGreen) - (tmpimage[i + 1][j - 1].rgbtGreen) + (tmpimage[i + 1][j + 1].rgbtGreen);

            sum[2] = (-tmpimage[i - 1][j - 1].rgbtRed) + (tmpimage[i - 1][j + 1].rgbtRed) - (2 * tmpimage[i][j - 1].rgbtRed) +
                     (2 * tmpimage[i][j + 1].rgbtRed) - (tmpimage[i + 1][j - 1].rgbtRed) + (tmpimage[i + 1][j + 1].rgbtRed);

            sum[3] = (-tmpimage[i - 1][j - 1].rgbtBlue) - (2 * tmpimage[i - 1][j].rgbtBlue) - (tmpimage[i - 1][j + 1].rgbtBlue) +
                     (tmpimage[i + 1][j - 1].rgbtBlue) + (2 * tmpimage[i + 1][j].rgbtBlue) + (tmpimage[i + 1][j + 1].rgbtBlue);

            sum[4] = (-tmpimage[i - 1][j - 1].rgbtGreen) - (2 * tmpimage[i - 1][j].rgbtGreen) - (tmpimage[i - 1][j + 1].rgbtGreen) +
                     (tmpimage[i + 1][j - 1].rgbtGreen) + (2 * tmpimage[i + 1][j].rgbtGreen) + (tmpimage[i + 1][j + 1].rgbtGreen);

            sum[5] = (-tmpimage[i - 1][j - 1].rgbtRed) - (2 * tmpimage[i - 1][j].rgbtRed) - (tmpimage[i - 1][j + 1].rgbtRed) +
                     (tmpimage[i + 1][j - 1].rgbtRed) + (2 * tmpimage[i + 1][j].rgbtRed) + (tmpimage[i + 1][j + 1].rgbtRed);


            //Sobel Operator sqrt(Gx^2 + Gy^2) for each color values.
            sum[6] = round(pow(pow(sum[0], 2) + pow(sum[3], 2), 0.5));
            sum[7] = round(pow(pow(sum[1], 2) + pow(sum[4], 2), 0.5));
            sum[8] = round(pow(pow(sum[2], 2) + pow(sum[5], 2), 0.5));


            //Copies the image bitmap from temporary image file to original image excluding the border and makes sure that the 8 bit color value is less than or equal to 255, just incase if it goes above.
            if (sum[6] > 255)
            {
                image[i - 1][j - 1].rgbtBlue = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtBlue = (int)sum[6];
            }

            //Makes sure that the 8 bit color value is less than or equal to 255, just incase if it goes above.
            if (sum[7] > 255)
            {
                image[i - 1][j - 1].rgbtGreen = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtGreen = (int)sum[7];
            }

            //Makes sure that the 8 bit color value is less than or equal to 255, just incase if it goes above.
            if (sum[8] > 255)
            {
                image[i - 1][j - 1].rgbtRed = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtRed = (int)sum[8];
            }

        }
    }

    return;
}