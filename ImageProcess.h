#ifndef ImageProcessH
#define ImageProcessH

#include <vcl.h>


typedef float CGFloat;    // 32-bit
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

struct CGPoint {
   CGFloat x;
   CGFloat y;
};
typedef struct CGPoint CGPoint;
 
struct CGSize {
   CGFloat width;
   CGFloat height;
};
typedef struct CGSize CGSize;

struct CGRect {
   CGPoint origin;
   CGSize size;
};
typedef struct CGRect CGRect;


/*
struct _Color
{
        int red;
        int green;
        int blue;
        int gray;
};

struct _frame
{
        //_Color pixel[162][122];
        _Color pixel[321][241];
        int BlobNumbers;
};
              */
struct _Histogram
{
        int value[256];

        int max_index;
        int max_value;
};

          /*
void DrawFrame(TImage *img, _frame frm, int BlockSize)
{

        for(int y=BlockSize; y<(240-BlockSize); y+=BlockSize)
                for(int x=BlockSize; x<(320-BlockSize); x+=BlockSize)
                {
                      img->Canvas->Brush->Color = RGB( frm.pixel[x>>1][y>>1].red, frm.pixel[x>>1][y>>1].green, frm.pixel[x>>1][y>>1].blue);
                      img->Canvas->Pen->Color = img->Canvas->Brush->Color;
                      Rectangle( img->Canvas->Handle, x, y, x+BlockSize, y+BlockSize );
                 }

        img->Repaint();
}

void SetColor(_Color *target, TColor source)
{
        target->red = GetRValue(source);
        target->green = GetGValue(source);
        target->blue = GetBValue(source);
}
           */
#endif