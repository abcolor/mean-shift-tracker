#ifndef MeanShiftTrackerH
#define MeanShiftTrackerH

#include "ImageProcess.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BYTES_PER_PIXEL 3
#define BYTES_PER_LINE (SCREEN_WIDTH * BYTES_PER_PIXEL)
#define HISTOGRAM_LENGTH 8192 /* R(16) x G(16) x B(16) x Edge(2) */

#define MAX_TRACKING_PIXEL_NO ((SCREEN_WIDTH * SCREEN_HEIGHT) / 2)
#define EDGE_THRESHOLD 128
#define MEANSHIFT_ITARATION_NO 5

#define MOSAIC_SIZE 5
#define MAX_MOSAIC_NO ( (SCREEN_WIDTH / (2*MOSAIC_SIZE)) * (SCREEN_HEIGHT / (2*MOSAIC_SIZE)) )
#define MOSAIC_PIXEL_OFFSET (BYTES_PER_PIXEL * MOSAIC_SIZE)
#define MOSAIC_LINE_OFFSET (SCREEN_WIDTH * BYTES_PER_PIXEL * MOSAIC_SIZE)

#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

typedef struct {
    CGPoint position;
    int histogramIndex;
}tracking_pixel_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
}mosaic_pixel_t;


class MeanShiftTracker
{
private:
    CGFloat initHistogram[HISTOGRAM_LENGTH];
    CGRect box;
    CGSize halfBoxSize;
    uint8_t activate;
    tracking_pixel_t trackingPixels[MAX_TRACKING_PIXEL_NO];
    int trackingPixelNumber;

    void findHistogram(uint8_t *pixels, CGFloat *histogram);
    int checkEdgeExistance(uint8_t *pixels, int x, int y);
    uint8_t getGrayValue(uint8_t *pixels);
    void updateNextLocation(uint8_t *pixels);
    void findWightsAndCOM(uint8_t *pixels, CGFloat *histogram);


public:
    MeanShiftTracker(CGRect rect);
    CGRect inputFrame(uint8_t *pixels);
};

#endif
 