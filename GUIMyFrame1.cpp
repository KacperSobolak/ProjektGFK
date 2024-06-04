#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
    _penColor = *wxBLACK;
    _fillColor = *wxWHITE;
    _penSize = 2;
}

void GUIMyFrame1::loadFileOnButtonClick( wxCommandEvent& event )
{
wxTextFile file;
wxFileDialog WxOpenFileDialog1(this, _("Choose a file"), _(""), _(""), _("*.*"), wxFD_OPEN);
WxOpenFileDialog1.SetWildcard("TXT files (*.txt)|*.txt");
if (WxOpenFileDialog1.ShowModal() == wxID_OK)
{
_filename = WxOpenFileDialog1.GetPath();

if (!file.Open(_filename))
wxLogError(_("Can't load this file"));
}
}

void GUIMyFrame1::startAnimOnButtonClick( wxCommandEvent& event )
{
FileParser(std::string(_filename.mb_str()));
}


void GUIMyFrame1::DrawRectangle(int x1, int y1, int x2, int y2, int flag)
{
    wxClientDC dc(panel);
    dc.SetBrush(wxBrush(_fillColor));
    if (flag == 0) {
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
    }
    dc.SetPen(wxPen(_penColor, _penSize));
    int width = abs(x1 - x2);
    int height = abs(y1 - y2);
    int x = std::min(x1, x2);
    int y = std::min(y1, y2);
    dc.DrawRectangle(x, y, width, height);
}

void GUIMyFrame1::DrawLine(int x1, int y1, int x2, int y2)
{
    wxClientDC dc(panel);
    dc.SetBrush(wxBrush(_fillColor));
    dc.SetPen(wxPen(_penColor, _penSize));
    dc.DrawLine(x1, y1, x2, y2);
}

void GUIMyFrame1::FileParser(const std::string& filename) 
{
    std::ifstream file(filename);
    int width, height, FrameNmbr, FrameTime;
    std::string command;
    char comma;
    file >> width >> comma >> height;
    SetPanelSize(width, height);
    file >> std::ws;
    while (file >> FrameNmbr >> comma >> FrameTime) {
        //std::cout << "\nNext Frame: " << FrameNmbr << " with time: " << FrameTime << " ms" << std::endl;
        file >> std::ws;
        float a, b, c, d, thickness;
        bool flag = false;
        while (file >> command) {
            if (command == "stop" || command == "ST")
                break;
            else if (command == "punkt" || command == "PT") {
                file >> a >> comma >> b;
                DrawPoint(int(a), int(b));
                //std::cout << "Drawing point: " << a << " " << b << std::endl;
            }
            else if (command == "linia" || command == "LN") {
                file >> a >> comma >> b >> comma >> c >> comma >> d;
                DrawLine(int(a), int(b), int(c), int(d));
                //std::cout << "Drawing line: " << "(" << a << ", " << b << ") " << "(" << c << " " << d << ")" << std::endl;
            }
            else if (command == "prostokat" || command == "PR") {
                file >> a >> comma >> b >> comma >> c >> comma >> d >> flag;
                DrawRectangle(int(a), int(b), int(c), int(d), int(flag));
                //std::cout << "Drawing rectangle: " << "(" << a << ", " << b << ") " << "(" << c << " " << d << ")"
                    //" Wypelnienie: " << flag << std::endl;
            }
            else if (command == "elipsa" || command == "EL") {
                file >> a >> comma >> b >> comma >> c >> comma >> d >> flag;
                DrawElipse(int(a), int(b), int(c), int(d), int(flag));
                //std::cout << "Drawing elipse: " << "(" << a << ", " << b << ") " << "(" << c << " " << d << ")"
                    //" Wypelnienie: " << flag << std::endl;
            }
            else if (command == "rozmiar_piora" || command == "RP") {
                file >> thickness;
                //std::cout << "Changing Pen size to: " << thickness << std::endl;
                _penSize = int(thickness);
            }
            else if (command == "kolor_piora" || command == "KP") {
                file >> a >> b >> c;
                _penColor = wxColour(int(a), int(b), int(c));
                //std::cout << "Changing Pen colour to (" << a << ", " << b << ", " << c << ") " << std::endl;
            }
            else if (command == "kolor_wypelnienia" || command == "KW") {
                file >> a >> b >> c;
                _fillColor = wxColour(int(a), int(b), int(c));
                //std::cout << "Changing Fill colour to (" << a << ", " << b << ", " << c << ")" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime));
        if (saveAnim->IsChecked()) {
            wxString frameNumber;
            frameNumber << FrameNmbr;
            SaveClientDCToBitmap(frameNumber);
        }
        Repaint();
    }
}

void GUIMyFrame1::Repaint()
{
    wxClientDC dc(panel);
    dc.Clear();
    dc.SetBackground(wxColor(255, 255, 255));
}

void GUIMyFrame1::SaveClientDCToBitmap(wxString filename) 
{
    wxClientDC dc(panel);
    wxSize size = panel->GetSize();

    // Create a bitmap to hold the client area content
    wxBitmap bitmap(size.x, size.y, -1);

    // Create a memory DC to draw into the bitmap
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);

    // Blit (copy) the content from clientDC to memDC
    memDC.Blit(0, 0, size.x, size.y, &dc, 0, 0);

    // Deselect the bitmap from the memory DC
    memDC.SelectObject(wxNullBitmap);
    // Save the bitmap to disk
    bitmap.SaveFile(filename+".bmp", wxBITMAP_TYPE_BMP);
}

void GUIMyFrame1::DrawElipse(int x1, int y1, int x2, int y2, int flag)
{
    wxClientDC dc(panel);
    dc.SetBrush(wxBrush(_fillColor));
    if (flag == 0) {
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
    }
    dc.SetPen(wxPen(_penColor, _penSize));
    int width = abs(x1 - x2);
    int height = abs(y1 - y2);
    int x = std::min(x1, x2);
    int y = std::min(y1, y2);
    dc.DrawEllipse(x, y, width, height);
}

void GUIMyFrame1::DrawPoint(int x, int y)
{
    wxClientDC dc(panel);
    dc.SetBrush(wxBrush(_fillColor));
    dc.SetPen(wxPen(_penColor, _penSize));
    dc.DrawPoint(x, y);
}

void GUIMyFrame1::SetPanelSize(int width, int height)
{
    panel->SetSize(width, height);
}