//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "VideoCapture.h"
#include "MeanShiftTracker.h"

//---------------------------------------------------------------------------

class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TBevel *Bevel3;
    TLabel *labInput;
    TPanel *PanelCamera;
    TGroupBox *GroupBox1;
    TLabel *labImageFormat;
    TLabel *labImageSize;
    TBevel *Bevel1;
    TComboBox *comboCameraDevice;
    TButton *btnSource;
    TButton *btnFormat;
    TButton *btnConnect;
    TRadioButton *optCamera;
    TButton *btnDetection;
    TBevel *Bevel4;
    TPanel *PanelOutput;
    TImage *imgOutput;
    TShape *shpSelectObj;
    TLabel *Label1;
    void __fastcall btnConnectClick(TObject *Sender);
    void __fastcall btnSourceClick(TObject *Sender);
    void __fastcall btnFormatClick(TObject *Sender);
    void __fastcall btnDetectionClick(TObject *Sender);
    void __fastcall imgOutputMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall imgOutputMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall imgOutputMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
    TVideoCapture *camera;
    MeanShiftTracker *tracker;

public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    void processInputFrame(byte *frame);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
