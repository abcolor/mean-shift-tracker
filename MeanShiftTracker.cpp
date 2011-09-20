#include "MeanShiftTracker.h"
#include <mem.h>


MeanShiftTracker::MeanShiftTracker(CGRect rect)
{
    box = rect;
    halfBoxSize.width = box.size.width / 2.0;
    halfBoxSize.height = box.size.height / 2.0;
    activate = 0;
}

//---------------------------------------------------------------------------
CGRect MeanShiftTracker::inputFrame(uint8_t *pixels)
{
    if(!activate) {
        findHistogram(pixels, initHistogram);
        activate = 1;
    }
    else {
        updateNextLocation(pixels);
    }

    CGRect rect;
    rect.origin.x = box.origin.x - halfBoxSize.width;
    rect.origin.y = box.origin.y - halfBoxSize.height;
    rect.size.width = box.size.width;
    rect.size.height = box.size.height;

    return rect;
}


//---------------------------------------------------------------------------
void MeanShiftTracker::findHistogram(uint8_t *pixels, CGFloat *histogram)
{
    int r, g, b, e;
    int i;
    int box_l, box_r, box_t, box_b;
    float base;
    int width_offset;

    memset(histogram, 0, sizeof(CGFloat)*HISTOGRAM_LENGTH);

    box_l = max( (int)(box.origin.x - halfBoxSize.width), 0);
    box_r = min( (int)(box.origin.x + halfBoxSize.width), SCREEN_WIDTH-1);
    box_t = max( (int)(box.origin.y - halfBoxSize.height), 0);
    box_b = min( (int)(box.origin.y + halfBoxSize.height), SCREEN_HEIGHT-1);
    base = 1 / (CGFloat)(((box_r-box_l)+1) * ((box_b-box_t)+1));

    width_offset = (SCREEN_WIDTH - ((box_r-box_l)+1)) * BYTES_PER_PIXEL;
    pixels+=(((box_t*SCREEN_WIDTH)+box_l)*BYTES_PER_PIXEL);
    trackingPixelNumber = 0;

    for(int y = box_t; y <= box_b; y++)
    {
        for(int x = box_l; x <= box_r; x++)
        {
            b = *pixels >> 4;
            g = *(pixels + 1) >> 4;
            r = *(pixels + 2) >> 4;
            e = checkEdgeExistance(pixels, x, y);

            i = (e << 12) | (r << 8) | (g << 4) | b;
            histogram[i]+=base;

            trackingPixels[trackingPixelNumber].position.x = x;
            trackingPixels[trackingPixelNumber].position.y = y;
            trackingPixels[trackingPixelNumber].histogramIndex = i;
            trackingPixelNumber++;

            pixels+=BYTES_PER_PIXEL;
        }
        pixels+=width_offset;
    }
}

//---------------------------------------------------------------------------
uint8_t MeanShiftTracker::getGrayValue(uint8_t *pixels)
{
    int r, g, b;
    b = *pixels;
    g = *(pixels + 1);
    r = *(pixels + 2);

    int y = r * 0.299 + g * 0.587 + b * 0.114;
    return y;
}

//---------------------------------------------------------------------------
int MeanShiftTracker::checkEdgeExistance(uint8_t *pixels, int x, int y)
{
    int sum;
    long sumX, sumY;
    uint8_t *prev_ptr;
    uint8_t *next_ptr;

#if 1 /* use green */
    pixels++;
    prev_ptr = pixels - BYTES_PER_LINE;
    next_ptr = pixels + BYTES_PER_LINE;

    // x gradient
    sumX = ( -*(prev_ptr-BYTES_PER_PIXEL)  ) +  *(prev_ptr+BYTES_PER_PIXEL) +
            (-( *(pixels-BYTES_PER_PIXEL) << 1  ))  + ( *(pixels+BYTES_PER_PIXEL) <<1 ) +
            ( -*(next_ptr-BYTES_PER_PIXEL)  ) +  *(next_ptr+BYTES_PER_PIXEL);

    // y gradient
    sumY =  *(prev_ptr+BYTES_PER_PIXEL)  + ( *(prev_ptr) << 1 ) +  *(prev_ptr-BYTES_PER_PIXEL)  +
            ( -*(next_ptr-BYTES_PER_PIXEL) ) + (-( *(next_ptr) <<1 )) + ( -*(next_ptr+BYTES_PER_PIXEL)  ) ;

#else /* convert rgb -> y */

    prev_ptr = pixels - BYTES_PER_LINE;
    next_ptr = pixels + BYTES_PER_LINE;

    // x gradient
    sumX = ( -getGrayValue(prev_ptr-BYTES_PER_PIXEL)  ) +  getGrayValue(prev_ptr+BYTES_PER_PIXEL) +
            (-( getGrayValue(pixels-BYTES_PER_PIXEL) << 1  ))  + ( getGrayValue(pixels+BYTES_PER_PIXEL) <<1 ) +
            ( -getGrayValue(next_ptr-BYTES_PER_PIXEL)  ) +  getGrayValue(next_ptr+BYTES_PER_PIXEL);

    // y gradient
    sumY =  getGrayValue(prev_ptr+BYTES_PER_PIXEL)  + ( getGrayValue(prev_ptr) << 1 ) +  getGrayValue(prev_ptr-BYTES_PER_PIXEL)  +
            ( -getGrayValue(next_ptr-BYTES_PER_PIXEL) ) + (-( getGrayValue(next_ptr) <<1 )) + ( -getGrayValue(next_ptr+BYTES_PER_PIXEL)  ) ;
#endif

    // GRADIENT MAGNITUDE APPROXIMATION
    sum = abs(sumX) + abs(sumY);

    return (sum > EDGE_THRESHOLD)? 1: 0;
}

//---------------------------------------------------------------------------
void MeanShiftTracker::updateNextLocation(uint8_t *pixels)
{
    CGFloat currentHistogram[HISTOGRAM_LENGTH];


    for(int i=0; i<MEANSHIFT_ITARATION_NO; i++)
    {
        findHistogram(pixels, currentHistogram);
        findWightsAndCOM(pixels, currentHistogram);
    }
}

void MeanShiftTracker::findWightsAndCOM(uint8_t *pixels, CGFloat *histogram)
{
    CGFloat weights[HISTOGRAM_LENGTH];
    int r, g, b, e;
    int i;
    int box_l, box_r, box_t, box_b;
    int width_offset;
    CGPoint newOrigin;
    CGFloat sumOfWeights;
    int index;

    for(i=0; i<HISTOGRAM_LENGTH; i++)
        if(histogram[i] > 0.0)
            weights[i] = initHistogram[i] / histogram[i];
        else
            weights[i] = 0;

    newOrigin.x = 0.0;
    newOrigin.y = 0.0;
    sumOfWeights = 0.0;

    for(i=0; i<trackingPixelNumber; i++)
    {
        index = trackingPixels[i].histogramIndex;
        newOrigin.x += (weights[index] * trackingPixels[i].position.x);
        newOrigin.y += (weights[index] * trackingPixels[i].position.y);
        sumOfWeights += weights[index];
    }

    if (sumOfWeights>0) {
        box.origin.x = newOrigin.x / sumOfWeights + 0.5;
        box.origin.y = newOrigin.y / sumOfWeights + 0.5;
    }
}

