object Form1: TForm1
  Left = 513
  Top = 371
  Width = 681
  Height = 419
  Caption = 'Form1'
  Color = clSilver
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel3: TBevel
    Left = 5
    Top = 18
    Width = 322
    Height = 242
  end
  object labInput: TLabel
    Left = 14
    Top = 3
    Width = 66
    Height = 15
    Caption = 'Input Video:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clMaroon
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Bevel4: TBevel
    Left = 333
    Top = 18
    Width = 322
    Height = 242
  end
  object Label1: TLabel
    Left = 334
    Top = 3
    Width = 41
    Height = 15
    Caption = 'Output:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clMaroon
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object PanelCamera: TPanel
    Left = 6
    Top = 19
    Width = 320
    Height = 240
    BevelOuter = bvNone
    Color = clMaroon
    TabOrder = 0
  end
  object GroupBox1: TGroupBox
    Left = 5
    Top = 264
    Width = 321
    Height = 105
    Caption = 'Source'
    Font.Charset = ANSI_CHARSET
    Font.Color = clMaroon
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    object labImageFormat: TLabel
      Left = 179
      Top = 31
      Width = 73
      Height = 14
      Caption = 'Compression: -'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object labImageSize: TLabel
      Left = 179
      Top = 46
      Width = 60
      Height = 14
      Caption = 'Resolution: -'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Bevel1: TBevel
      Left = 172
      Top = 33
      Width = 18
      Height = 59
      Shape = bsLeftLine
    end
    object comboCameraDevice: TComboBox
      Left = 29
      Top = 36
      Width = 139
      Height = 22
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ItemHeight = 14
      ParentFont = False
      TabOrder = 0
    end
    object btnSource: TButton
      Left = 100
      Top = 65
      Width = 68
      Height = 25
      Caption = 'Source'
      Font.Charset = ANSI_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = btnSourceClick
    end
    object btnFormat: TButton
      Left = 178
      Top = 65
      Width = 67
      Height = 25
      Caption = 'Format'
      Font.Charset = ANSI_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = btnFormatClick
    end
    object btnConnect: TButton
      Left = 29
      Top = 65
      Width = 68
      Height = 25
      Caption = 'Connect'
      Font.Charset = ANSI_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = btnConnectClick
    end
    object optCamera: TRadioButton
      Left = 13
      Top = 17
      Width = 81
      Height = 17
      Caption = 'Camera'
      Checked = True
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      TabStop = True
    end
    object btnDetection: TButton
      Left = 248
      Top = 65
      Width = 67
      Height = 25
      Caption = 'Start'
      TabOrder = 5
      OnClick = btnDetectionClick
    end
  end
  object PanelOutput: TPanel
    Left = 334
    Top = 19
    Width = 320
    Height = 240
    BevelOuter = bvNone
    Color = clGray
    TabOrder = 2
    object imgOutput: TImage
      Left = 0
      Top = 0
      Width = 320
      Height = 240
      Cursor = crArrow
      OnMouseDown = imgOutputMouseDown
      OnMouseMove = imgOutputMouseMove
      OnMouseUp = imgOutputMouseUp
    end
    object shpSelectObj: TShape
      Left = 104
      Top = 112
      Width = 65
      Height = 65
      Brush.Style = bsClear
      Pen.Color = clRed
      Pen.Style = psDot
      Visible = False
    end
  end
end
