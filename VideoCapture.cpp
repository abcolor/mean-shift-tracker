#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "VideoCapture.h"
#include "Unit1.h"




uint8_t frame_data[MAX_VIDEO_BUFFER_SIZE];
int pixelNumber;

BITMAPINFO output_bitmap;
uint8_t *output_data;

long compression_method;
bool callback_enable;

//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TVideoCapture::TVideoCapture (HWND parent_handle, int fps)
{
    prev_fps = fps;

    // create video capture window
    prevHandle = capCreateCaptureWindow(
                    (LPSTR) "My Capture Window",
                    WS_CHILD | WS_VISIBLE,
                    0, 0, 352, 288,
                    parent_handle,
                    (int) 1);

    callback_enable = 0;
    enumCapDrv();
    selectedDevice = -1;
}

__fastcall TVideoCapture::~TVideoCapture ()
{
//    delete pStringCapDrivers;

    capPreview(prevHandle, FALSE); // end preview
    capDriverConnect(prevHandle, selectedDevice);
    capDriverDisconnect(prevHandle); // disconnect from driver
}

//---------------------------------------------------------------------------
// enumerate the installed capture drivers
//---------------------------------------------------------------------------
int TVideoCapture::enumCapDrv ()
{
    char szDeviceName[80]; // driver name
    char szDeviceVersion[80]; // driver version
    int xcap; // counter

    xcap = 0;
    Form1->comboCameraDevice->Clear();
    do
    {
         if (capGetDriverDescription(xcap, szDeviceName, sizeof(szDeviceName),szDeviceVersion, sizeof(szDeviceVersion)))
              Form1->comboCameraDevice->Items->Add(szDeviceName);
         else
            break;

         xcap++;
    }
    while (true);

    Form1->comboCameraDevice->ItemIndex=0;
    return xcap;
}


//----------切換VFW顯示頁面Call Back
LRESULT PASCAL OnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
    if(callback_enable)
    {
        uint8_t *srcData = lpVHdr->lpData;
        uint8_t *dstData = frame_data;

        switch(compression_method)
        {
        case IMAGE_COMPRESSION_RGB24:
        {
            //memcpy(dstData, srcData, pixelNumber * 3);
                int offset = 320 * 3;
                int src_pos =0;
                int dst_pos = 320 * 239 * 3;

                for(int y=0;y<240;y++)
                {
                    memcpy(&dstData[dst_pos], &srcData[src_pos], offset);
                    src_pos+=offset;
                    dst_pos-=offset;
                }
            }
            break;

        case IMAGE_COMPRESSION_YUY2:
            for(int i=0; i<pixelNumber; i++) {
                *dstData = *srcData;
                srcData+=2;
                dstData++;
            }
            break;

        case IMAGE_COMPRESSION_I420:
        case IMAGE_COMPRESSION_IYUV:
            for(int i=0; i<pixelNumber; i++) {
                *dstData = *srcData;
                srcData++;
                dstData++;
            }
            break;
        }
        Form1->processInputFrame(frame_data);
    }
    return (LRESULT) true;
}




//---------------------------------------------------------------------------
//  connecting to selected device and starts preview
//---------------------------------------------------------------------------
void TVideoCapture::connect (int Selected)
{
    CAPSTATUS CapStatus;
    int hsize;

    // connect to the driver
    if (selectedDevice != -1)
    {
        capPreview (prevHandle, FALSE);
        capDriverConnect(prevHandle, selectedDevice);
    }
    else
    {
        if (!capDriverConnect(prevHandle, Selected))
            return; // Unable to connect to driver
    }

    // update the driver capabilities
    capDriverGetCaps (prevHandle, sizeof(CAPDRIVERCAPS), &capDrvCaps);

//    capDlgVideoFormat(ParentHandle);

    // Are there new image dimensions
    capGetStatus(prevHandle, &CapStatus, sizeof(CAPSTATUS));



    hsize = GetSystemMetrics(SM_CYMENU);
    hsize += GetSystemMetrics(SM_CYCAPTION);

    float rate = (1 / (float)prev_fps) * 1000;
    capPreviewRate (prevHandle, rate);

    // start preview video
    capPreview (prevHandle, TRUE);

    // Remember selected device
    selectedDevice = Selected;

    capSetCallbackOnFrame(prevHandle,OnFrame);

    getVideoFormat();
}



//---------------------------------------------------------------------------
//  Get access to the video source format box
//---------------------------------------------------------------------------
void TVideoCapture::format ()
{
    int       hsize;

    CAPSTATUS CapStatus;


    capDlgVideoFormat(prevHandle);
    // Are there new image dimensions
    capGetStatus(prevHandle, &CapStatus, sizeof(CAPSTATUS));

    hsize = GetSystemMetrics(SM_CYMENU);
    hsize += GetSystemMetrics(SM_CYCAPTION);

//    SetWindowPos(ParentHandle, NULL, 0, hsize, CapStatus.uiImageWidth,
//                CapStatus.uiImageHeight+hsize, SWP_NOZORDER | SWP_NOMOVE);
    SetWindowPos(prevHandle, NULL, 0, 0, CapStatus.uiImageWidth,
                CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);

    getVideoFormat();                
}
//---------------------------------------------------------------------------
//  Get access to the video source dialog box
//---------------------------------------------------------------------------
void TVideoCapture::source ()
{
    capDlgVideoSource(prevHandle);
}

//---------------------------------------------------------------------------
//  capture a frame and save it
//---------------------------------------------------------------------------

void TVideoCapture::drawFrame(TImage *img, uint8_t *frame)
{
    memcpy(output_data, frame, pixelNumber * 3);
    SetDIBitsToDevice(img->Canvas->Handle,
                                    0,0,
                                    videoSize.width, videoSize.height,
                                    0,0, 0, videoSize.height,
                                    output_data,&output_bitmap,
                                    DIB_RGB_COLORS);

    img->Repaint();
}

void TVideoCapture::drawGrayFrame(TImage *img, uint8_t *frame)
{
    uint8_t *dstData = output_data;

    for(int i=0; i<pixelNumber; i++) {
#if 1
        *dstData = *frame;
        *(dstData+1) = *frame;
        *(dstData+2) = *frame;
#else
        *dstData = 0;
        *(dstData+1) = *frame;
        *(dstData+2) = 0;
#endif
        frame++;
        dstData+=3;
    }

    SetDIBitsToDevice(img->Canvas->Handle,
                                    0,0,
                                    videoSize.width, videoSize.height,
                                    0,0, 0, videoSize.height,
                                    output_data,&output_bitmap,
                                    DIB_RGB_COLORS);

    img->Repaint();
}




void TVideoCapture::enableOnFrameCallback(bool value)
{
    callback_enable = value;
    if(callback_enable)
        getVideoFormat();
}

void TVideoCapture::getVideoFormat(void)
{
    char info[30];
    BITMAPINFO bmi;
    uint8_t bits_per_pix;

    capGetVideoFormat(prevHandle, &bmi, sizeof(BITMAPINFO));

    compression_method = bmi.bmiHeader.biCompression;
    bits_per_pix = bmi.bmiHeader.biBitCount;

    switch(compression_method)
    {
    case IMAGE_COMPRESSION_RGB24:
        sprintf(info, "Compression: RGB (%d)", bits_per_pix);
        break;

    case IMAGE_COMPRESSION_YUY2:
        sprintf(info, "Compression: YUY2 (%d)", bits_per_pix);
        break;

    case IMAGE_COMPRESSION_I420:
        sprintf(info, "Compression: I420 (%d)", bits_per_pix);
        break;

    case IMAGE_COMPRESSION_IYUV:
        sprintf(info, "Compression: IYUV (%d)", bits_per_pix);
        break;

    default:
        sprintf(info, "Compression: unsupport(0x%x)", compression_method);
    }
    Form1->labImageFormat->Caption = info;

    sprintf(info, "Resolution: %dx%d", bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight);
    Form1->labImageSize->Caption = info;



    videoSize.width = bmi.bmiHeader.biWidth;
    videoSize.height = bmi.bmiHeader.biHeight;
    pixelNumber = videoSize.width * videoSize.height;


    // create output bitmap
    ZeroMemory(&output_bitmap.bmiHeader,sizeof(output_bitmap.bmiHeader)); // 清理BITMAPINFO結構內資料
    output_bitmap.bmiHeader.biSize=sizeof(output_bitmap.bmiHeader);
    output_bitmap.bmiHeader.biWidth=videoSize.width;
    output_bitmap.bmiHeader.biHeight=-videoSize.height; // 圖像圖素由上而下排列
    output_bitmap.bmiHeader.biPlanes=1;
    output_bitmap.bmiHeader.biBitCount=24; // 色彩為24位元(R,G,B三色)
    output_bitmap.bmiHeader.biCompression=BI_RGB;

    // 以BITMAPINFO結構創造一24位元的新圖檔
    ::CreateDIBSection(0,&output_bitmap,DIB_RGB_COLORS,(void **)&output_data,0,0);
    // 更新目前執行緒的GDI繪製功能
    ::GdiFlush();
}
