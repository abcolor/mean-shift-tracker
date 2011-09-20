//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    camera =  new TVideoCapture(PanelCamera->Handle, 30);
    tracker = 0;

    imgOutput->Canvas->Pen->Width = 2;
    imgOutput->Canvas->Pen->Color = clRed;
    imgOutput->Canvas->Brush->Style = bsClear;

    imgOutput->Canvas->Brush->Style = bsClear;
    imgOutput->Canvas->Pen->Width = 2;
    imgOutput->Canvas->Pen->Color = clYellow;    
}

//---------------------------------------------------------------------------
void __fastcall TForm1::btnConnectClick(TObject *Sender)
{
    camera->connect(comboCameraDevice->ItemIndex);
    btnSource->Enabled = true;
    btnFormat->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnSourceClick(TObject *Sender)
{
    camera->source();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnFormatClick(TObject *Sender)
{
    if(btnDetection->Caption == "Stop")
        btnDetectionClick(Form1);

    camera->format();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnDetectionClick(TObject *Sender)
{
    if(btnDetection->Caption == "Start")
    {
        btnDetection->Caption = "Stop";
        camera->enableOnFrameCallback(1);
    }
    else
    {
        btnDetection->Caption = "Start";
        camera->enableOnFrameCallback(0);
    }
}
//---------------------------------------------------------------------------

void TForm1::processInputFrame(byte *frame)
{
    camera->drawFrame(imgOutput, frame);

    if(tracker) {
        CGRect box = tracker->inputFrame(frame);
        Rectangle(imgOutput->Canvas->Handle, box.origin.x, box.origin.y, box.origin.x + box.size.width, box.origin.y + box.size.height);
        imgOutput->Repaint();
    }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::imgOutputMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    shpSelectObj->Left = X;
    shpSelectObj->Top = Y;
    shpSelectObj->Width = 0;
    shpSelectObj->Height = 0;
    shpSelectObj->Visible = true;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::imgOutputMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if(shpSelectObj->Visible)
    {
        shpSelectObj->Width = X - shpSelectObj->Left;
        shpSelectObj->Height = Y - shpSelectObj->Top;
    }    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::imgOutputMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    shpSelectObj->Visible = false;

    CGRect rect;
    rect.origin.x = shpSelectObj->Left + (shpSelectObj->Width>>1);
    rect.origin.y = shpSelectObj->Top + (shpSelectObj->Height>>1);
    rect.size.width = shpSelectObj->Width;
    rect.size.height = shpSelectObj->Height;

    if(tracker) delete tracker;
    tracker = new MeanShiftTracker(rect);
}
//---------------------------------------------------------------------------

