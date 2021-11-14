#include <math.h>

#include "../config.h"
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE *image)
{
    // update to avg of blue green and red
    float avg = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            avg = (((float)PIXEL(image,height,width,i,j).rgbtBlue + PIXEL(image,height,width,i,j).rgbtGreen + PIXEL(image,height,width,i,j).rgbtRed) / 3);
            int roundi = round(avg);
            PIXEL(image,height,width,i,j).rgbtBlue = roundi;
            PIXEL(image,height,width,i,j).rgbtGreen = roundi;
            PIXEL(image,height,width,i,j).rgbtRed = roundi;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE *image)
{
    // swaping 2 vals [end to start]
    for (int i = 0; i < height; i++)
    {
        //only till width / 2 because we dont want it to swap again
        for (int j = 0; j < (int)width / 2 ; j ++)
        {
            int tmpblue = PIXEL(image,height,width,i,j).rgbtBlue;
            PIXEL(image,height,width,i,j).rgbtBlue = PIXEL(image,heigth,width,i,(width - j - 1)).rgbtBlue;
            PIXEL(image,height,width,i,(width - j - 1)).rgbtBlue = tmpblue;

            int tmpgreen = PIXEL(image,height,width,i,j).rgbtGreen;
            PIXEL(image,height,width,i,j).rgbtGreen = PIXEL(image,height,width,i,(width - j - 1)).rgbtGreen;
            PIXEL(image,height,width,i,(width - j - 1)).rgbtGreen = tmpgreen;

            int tmpred = PIXEL(image,height,width,i,j).rgbtRed;
            PIXEL(image,height,width,i,j).rgbtRed = PIXEL(image,height,width,i,(width - j - 1)).rgbtRed;
            PIXEL(image,height,width,i,(width - j - 1)).rgbtRed = tmpred;

        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE *image)
{
    //sepia red blue green is all 0 for now
    float sepiaRed = 0;
    float sepiaBlue = 0;
    float sepiaGreen = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            // applying algo to all
            sepiaRed = (0.393 * (float)PIXEL(image,height,width,i,j).rgbtRed) + (0.769 * (float)PIXEL(image,height,width,i,j).rgbtGreen) + (0.189 * (float)PIXEL(image,height,width,i,j).rgbtBlue);
            sepiaGreen = (0.349 * (float)PIXEL(image,height,width,i,j).rgbtRed) + (0.686 * (float)PIXEL(image,height,width,i,j).rgbtGreen) + (0.168 * (float)PIXEL(image,height,width,i,j).rgbtBlue);
            sepiaBlue = (0.272 * (float)PIXEL(image,height,width,i,j).rgbtRed) + (0.534 * (float)PIXEL(image,height,width,i,j).rgbtGreen) + (0.131 * (float)PIXEL(image,height,width,i,j).rgbtBlue);

            int sred = round(sepiaRed);
            int sgreen = round(sepiaGreen);
            int sblue = round(sepiaBlue);

            // if more than 255 which is max of rgb cap it to 255
            if (sred > 255)
            {
                sred = 255;
            }
            if (sgreen > 255)
            {
                sgreen = 255;
            }
            if (sblue > 255)
            {
                sblue = 255;
            }
            PIXEL(image,height,width,i,j).rgbtBlue = sblue;
            PIXEL(image,height,width,i,j).rgbtGreen = sgreen;
            PIXEL(image,height,width,i,j).rgbtRed = sred;
        }
    }
    return;

}

// Blur image
void blur(int height, int width, RGBTRIPLE *image)
{
    // RGBTRIPLE tmp[height][width];
    RGBTRIPLE *tmp = (RGBTRIPLE *)alloca(height * width * sizeof(RGBTRIPLE));

    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col ++)
        {
            int count = 0;

            int xaxis[] = {row - 1, row, row + 1};
            int yaxis[] = {col - 1, col, col + 1};

            float totalR = 0, totalG = 0, totalB = 0;

            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int curRow = xaxis[r];
                    int curCol = yaxis[c];

                    if (curRow >= 0 && curRow < height && curCol >= 0 && curCol < width)
                    {
                        RGBTRIPLE pixel = PIXEL(image,height,width,curRow,curCol);

                        totalR += pixel.rgbtRed;
                        totalG += pixel.rgbtGreen;
                        totalB += pixel.rgbtBlue;
                        count ++;

                    }
                }
            }

            PIXEL(tmp,height,width,row,col).rgbtRed = round(totalR / count);
            PIXEL(tmp,height,width,row,col).rgbtGreen = round(totalG / count);
            PIXEL(tmp,height,width,row,col).rgbtBlue = round(totalB / count);

        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            PIXEL(image,height,width,i,j) = PIXEL(tmp,height,width,i,j);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE *image)
{
    // RGBTRIPLE tmp[height][width];
    RGBTRIPLE *tmp = (RGBTRIPLE *)alloca(height * width * sizeof(RGBTRIPLE));

    //gx matrix
    int Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    //gy matrix
    int Gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col ++)
        {
            //x axis
            int xaxis[] = {row - 1, row, row + 1};
            // y axis
            int yaxis[] = {col - 1, col, col + 1};

            // flaot vals for gx rgb
            float Gx_R = 0, Gx_G = 0, Gx_B = 0;

            // flaot vals for gy rgb
            float Gy_R = 0, Gy_G = 0, Gy_B = 0;

            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int curRow = xaxis[r];
                    int curCol = yaxis[c];

                    if (curRow >= 0 && curRow < height && curCol >= 0 && curCol < width)
                    {
                        RGBTRIPLE pixel = PIXEL(image,height,width,curRow,curCol);

                        // matrix for gx_rgb * the gx vals
                        Gx_R += Gx[r][c] * pixel.rgbtRed;
                        Gx_G += Gx[r][c] * pixel.rgbtGreen;
                        Gx_B += Gx[r][c] * pixel.rgbtBlue;

                        // matrix for gy_rgb * the gy vals
                        Gy_R += Gy[r][c] * pixel.rgbtRed;
                        Gy_G += Gy[r][c] * pixel.rgbtGreen;
                        Gy_B += Gy[r][c] * pixel.rgbtBlue;
                    }
                }
            }

            //sqrt of the vals of gx and gy rgb then roud it
            int final_red = round(sqrt((Gx_R * Gx_R) + (Gy_R * Gy_R)));
            int final_green = round(sqrt((Gx_G * Gx_G) + (Gy_G * Gy_G)));
            int final_blue = round(sqrt((Gx_B * Gx_B) + (Gy_B * Gy_B)));

            // if the value more than 255 then cap it to 255
            if (final_red > 255)
            {
                final_red = 255;
            }
            if (final_green > 255)
            {
                final_green = 255;
            }
            if (final_blue > 255)
            {
                final_blue = 255;
            }

            //update vals in the tmp
            PIXEL(tmp,height,width,row,col).rgbtRed = final_red;
            PIXEL(tmp,height,width,row,col).rgbtGreen = final_green;
            PIXEL(tmp,height,width,row,col).rgbtBlue = final_blue;
        }
    }

    // updating the vals into the new image output
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            PIXEL(image,heigth,width,i,j) = PIXEL(tmp,height,width,i,j);
        }
    }
    return;
}

