
//---------------------------------------------------------------------------

#ifndef VideoCaptureH
#define VideoCaptureH
//---------------------------------------------------------------------------
#include <vfw.h> // video for windows library
#include <vcl.h>
#include "ImageProcess.h"

// COMPRESSION CODE
#define IMAGE_COMPRESSION_RGB24 0x00000000
#define IMAGE_COMPRESSION_YUY2  0x32595559
#define IMAGE_COMPRESSION_I420  0x30323449
#define IMAGE_COMPRESSION_IYUV  0x56555949


#define MAX_VIDEO_WIDTH 352
#define MAX_VIDEO_HEIGHT 288
#define MAX_VIDEO_BUFFER_SIZE (MAX_VIDEO_WIDTH * MAX_VIDEO_HEIGHT * 4)

class TVideoCapture
{
private:
    int prev_fps;
    HWND prevHandle;
    CAPDRIVERCAPS capDrvCaps;
    int selectedDevice;
    int enumCapDrv();
    void getVideoFormat(void);

public:
    __fastcall TVideoCapture (HWND parent_handle, int fps);
    __fastcall ~TVideoCapture();

    void connect(int Selected);
    void format();
    void source();

    void enableOnFrameCallback(bool value);
    void drawFrame(TImage *img, uint8_t *frame);
    void drawGrayFrame(TImage *img, uint8_t *frame);

    CGSize videoSize;
};


#endif
