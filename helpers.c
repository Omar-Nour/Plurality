#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avgRGB;
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            avgRGB = ceil((image[row][col].rgbtBlue + image[row][col].rgbtGreen + image[row][col].rgbtRed) / 3);
            image[row][col].rgbtBlue = avgRGB;
            image[row][col].rgbtGreen = avgRGB;
            image[row][col].rgbtRed = avgRGB;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sRED, sGREEN, sBLUE;
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            sBLUE = ensure(ceil(.272 * image[row][col].rgbtRed + .534 * image[row][col].rgbtGreen + .131 * image[row][col].rgbtBlue));
            sGREEN = ensure(ceil(.349 * image[row][col].rgbtRed + .686 * image[row][col].rgbtGreen + .168 * image[row][col].rgbtBlue));
            sRED = ensure(ceil(.393 * image[row][col].rgbtRed + .769 * image[row][col].rgbtGreen + .189 * image[row][col].rgbtBlue));
            image[row][col].rgbtBlue = sBLUE;
            image[row][col].rgbtGreen = sGREEN;
            image[row][col].rgbtRed = sRED;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE TempImage[height][width];

    // Reflect in temp array as to not tamper with original values that would break the algorithm
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            TempImage[row][col].rgbtBlue = image[row][width - 1 - col].rgbtBlue;
            TempImage[row][col].rgbtGreen = image[row][width - 1 - col].rgbtGreen;
            TempImage[row][col].rgbtRed = image[row][width - 1 - col].rgbtRed;
        }
    }

    // copy the temporary array to the actual array
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            image[row][col].rgbtBlue = TempImage[row][col].rgbtBlue;
            image[row][col].rgbtGreen = TempImage[row][col].rgbtGreen;
            image[row][col].rgbtRed = TempImage[row][col].rgbtRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            //case 1
            if (col == 0 && row == 0)
            {
                temp[row][col].rgbtBlue = ensure(round((image[row][col].rgbtBlue + image[row][col + 1].rgbtBlue + image[row + 1][col].rgbtBlue + image[row + 1][col + 1].rgbtBlue) / 4));
                temp[row][col].rgbtGreen = ensure(round((image[row][col].rgbtGreen + image[row][col + 1].rgbtGreen + image[row + 1][col].rgbtGreen + image[row + 1][col + 1].rgbtGreen) / 4));
                temp[row][col].rgbtRed = ensure(round((image[row][col].rgbtRed + image[row][col + 1].rgbtRed + image[row + 1][col].rgbtRed + image[row + 1][col + 1].rgbtRed) / 4));

            }
            //case 2
            else if (row == 0 && col != width - 1 && col != 0)
            {
                temp[row][col].rgbtBlue = ensure(round((image[row][col - 1].rgbtBlue + image[row][col].rgbtBlue + image[row][col + 1].rgbtBlue + image[row + 1][col - 1].rgbtBlue + image[row + 1][col].rgbtBlue + image[row + 1][col + 1].rgbtBlue) / 6));
                temp[row][col].rgbtGreen = ensure(round((image[row][col - 1].rgbtGreen + image[row][col].rgbtGreen + image[row][col + 1].rgbtGreen + image[row + 1][col - 1].rgbtGreen + image[row + 1][col].rgbtGreen + image[row + 1][col + 1].rgbtGreen) / 6));
                temp[row][col].rgbtRed = ensure(round((image[row][col - 1].rgbtRed + image[row][col].rgbtRed + image[row][col + 1].rgbtRed + image[row + 1][col - 1].rgbtRed + image[row + 1][col].rgbtRed + image[row + 1][col + 1].rgbtRed) / 6));

            }
            //case 3
            else if (row == 0 && col == width - 1)
            {
                temp[row][col].rgbtBlue = ensure(round((image[row][col - 1].rgbtBlue + image[row][col].rgbtBlue + image[row + 1][col - 1].rgbtBlue + image[row + 1][col].rgbtBlue) / 4));
                temp[row][col].rgbtGreen = ensure(round((image[row][col - 1].rgbtGreen + image[row][col].rgbtGreen + image[row + 1][col - 1].rgbtGreen + image[row + 1][col].rgbtGreen) / 4));
                temp[row][col].rgbtRed = ensure(round((image[row][col - 1].rgbtRed + image[row][col].rgbtRed + image[row + 1][col - 1].rgbtRed + image[row + 1][col].rgbtRed) / 4));
            }
            //case 4
            else if (col == 0 && row != 0 && row != height - 1)
            {
                temp[row][col].rgbtBlue = ensure(round((image[row][col].rgbtBlue + image[row][col + 1].rgbtBlue + image[row + 1][col].rgbtBlue + image[row + 1][col + 1].rgbtBlue + image[row - 1][col].rgbtBlue + image[row - 1][col + 1].rgbtBlue) / 6));
                temp[row][col].rgbtGreen = ensure(round((image[row][col].rgbtGreen + image[row][col + 1].rgbtGreen + image[row + 1][col].rgbtGreen + image[row + 1][col + 1].rgbtGreen + image[row - 1][col].rgbtGreen + image[row - 1][col + 1].rgbtGreen) / 6));
                temp[row][col].rgbtRed = ensure(round((image[row][col].rgbtRed + image[row][col + 1].rgbtRed + image[row + 1][col].rgbtRed + image[row + 1][col + 1].rgbtRed + image[row - 1][col].rgbtRed + image[row - 1][col + 1].rgbtRed) / 6));
            }
            //case 5
            else if (col == width - 1  && row != 0 && row != height - 1)
            {
                temp[row][col].rgbtBlue = ensure(round((image[row][col].rgbtBlue + image[row][col - 1].rgbtBlue + image[row + 1][col].rgbtBlue + image[row + 1][col - 1].rgbtBlue + image[row - 1][col].rgbtBlue + image[row - 1][col - 1].rgbtBlue) / 6));
                temp[row][col].rgbtGreen = ensure(round((image[row][col].rgbtGreen + image[row][col - 1].rgbtGreen + image[row + 1][col].rgbtGreen + image[row + 1][col - 1].rgbtGreen + image[row - 1][col].rgbtGreen + image[row - 1][col - 1].rgbtGreen) / 6));
                temp[row][col].rgbtRed = ensure(round((image[row][col].rgbtRed + image[row][col - 1].rgbtRed + image[row + 1][col].rgbtRed + image[row + 1][col - 1].rgbtRed + image[row - 1][col].rgbtRed + image[row - 1][col - 1].rgbtRed) / 6));
            }
            //case 6
            else if (row == height - 1  && col == 0)
            {
                temp[row][col].rgbtBlue = ensure(round((image[row][col].rgbtBlue + image[row][col + 1].rgbtBlue + image[row - 1][col].rgbtBlue + image[row - 1][col + 1].rgbtBlue) / 4));
                temp[row][col].rgbtGreen = ensure(round((image[row][col].rgbtGreen + image[row][col + 1].rgbtGreen + image[row - 1][col].rgbtGreen + image[row - 1][col + 1].rgbtGreen) / 4));
                temp[row][col].rgbtRed = ensure(round((image[row][col].rgbtRed + image[row][col + 1].rgbtRed + image[row - 1][col].rgbtRed + image[row - 1][col + 1].rgbtRed) / 4));
            }
            //case 7
            else if (row == height - 1 && col != 0 && col != width - 1)
            {
                temp[row][col].rgbtBlue = ensure(round((image[row][col].rgbtBlue + image[row][col + 1].rgbtBlue + image[row][col - 1].rgbtBlue + image[row - 1][col + 1].rgbtBlue + image[row - 1][col].rgbtBlue + image[row - 1][col - 1].rgbtBlue) / 6));
                temp[row][col].rgbtGreen = ensure(round((image[row][col].rgbtGreen + image[row][col + 1].rgbtGreen + image[row][col - 1].rgbtGreen + image[row - 1][col + 1].rgbtGreen + image[row - 1][col].rgbtGreen + image[row - 1][col - 1].rgbtGreen) / 6));
                temp[row][col].rgbtRed = ensure(round((image[row][col].rgbtRed + image[row][col + 1].rgbtRed + image[row][col - 1].rgbtRed + image[row - 1][col + 1].rgbtRed + image[row - 1][col].rgbtRed + image[row - 1][col - 1].rgbtRed) / 6));
            }
            //case 8
            else if (row == height - 1 && col == width - 1)
            {
                temp[row][col].rgbtBlue = ensure(round((image[row][col].rgbtBlue + image[row][col - 1].rgbtBlue + image[row - 1][col].rgbtBlue + image[row - 1][col - 1].rgbtBlue) / 4));
                temp[row][col].rgbtGreen = ensure(round((image[row][col].rgbtGreen + image[row][col - 1].rgbtGreen + image[row - 1][col].rgbtGreen + image[row - 1][col - 1].rgbtGreen) / 4));
                temp[row][col].rgbtRed = ensure(round((image[row][col].rgbtRed + image[row][col - 1].rgbtRed + image[row - 1][col].rgbtRed + image[row - 1][col - 1].rgbtRed) / 4));
            }
            //case 9
            else
            {
                temp[row][col].rgbtBlue = ensure(round((image[row - 1][col - 1].rgbtBlue + image[row - 1][col].rgbtBlue + image[row - 1][col + 1].rgbtBlue + image[row][col - 1].rgbtBlue + image[row][col].rgbtBlue + image[row][col + 1].rgbtBlue + image[row + 1][col - 1].rgbtBlue + image[row + 1][col].rgbtBlue + image[row + 1][col + 1].rgbtBlue) / 9));
                temp[row][col].rgbtGreen = ensure(round((image[row - 1][col - 1].rgbtGreen + image[row - 1][col].rgbtGreen + image[row - 1][col + 1].rgbtGreen + image[row][col - 1].rgbtGreen + image[row][col].rgbtGreen + image[row][col + 1].rgbtGreen + image[row + 1][col - 1].rgbtGreen + image[row + 1][col].rgbtGreen + image[row + 1][col + 1].rgbtGreen) / 9));
                temp[row][col].rgbtRed = ensure(round((image[row - 1][col - 1].rgbtRed + image[row - 1][col].rgbtRed + image[row - 1][col + 1].rgbtRed + image[row][col - 1].rgbtRed + image[row][col].rgbtRed + image[row][col + 1].rgbtRed + image[row + 1][col - 1].rgbtRed + image[row + 1][col].rgbtRed + image[row + 1][col + 1].rgbtRed) / 9));
            }
        }
    }

    // copy the temporary array to the actual array
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            image[row][col].rgbtBlue = temp[row][col].rgbtBlue;
            image[row][col].rgbtGreen = temp[row][col].rgbtGreen;
            image[row][col].rgbtRed = temp[row][col].rgbtRed;
        }
    }
    
    return;
}

// round to 255 in sepia if value > 255
int ensure(int color)
{
    if (color > 255)
    {
        return 255;
    }
    return color;
}