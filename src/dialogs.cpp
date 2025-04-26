 /////////////////////////////////////////////////////////////////////////////
// Name:          main.cpp
// Adapted from:  wxCalendarCtrl sample by Vadim Zeitlin
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
 
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include "dialogs.h"
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/sizer.h"
    #include "wx/textctrl.h"
    #include "wx/settings.h"
#endif

#include "wx/calctrl.h"

#if wxUSE_DATEPICKCTRL
    #include "wx/datectrl.h"
    #if wxUSE_DATEPICKCTRL_GENERIC
        #include "wx/generic/datectrl.h"
    #endif // wxUSE_DATEPICKCTRL_GENERIC
#endif // wxUSE_DATEPICKCTRL

#if wxUSE_TIMEPICKCTRL
    #include "wx/timectrl.h"
    #if wxUSE_TIMEPICKCTRL_GENERIC
        #include "wx/generic/timectrl.h"
    #endif // wxUSE_TIMEPICKCTRL_GENERIC
#endif // wxUSE_TIMEPICKCTRL

#ifdef wxHAS_NATIVE_CALENDARCTRL
    #include "wx/generic/calctrlg.h"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// MyDateDialog
// ----------------------------------------------------------------------------

#if wxUSE_DATEPICKCTRL

wxBEGIN_EVENT_TABLE(MyDateDialog, wxDialog)
    EVT_DATE_CHANGED(wxID_ANY, MyDateDialog::OnDateChange)
wxEND_EVENT_TABLE()

MyDateDialog::MyDateDialog(wxWindow *parent, const wxDateTime& dt, int dtpStyle)
        : wxDialog(parent, wxID_ANY, wxString("Calendar: Choose a date"))
{
    wxWindow* datePickerWindow = nullptr;

#if wxUSE_DATEPICKCTRL_GENERIC
    m_datePickerGeneric = nullptr;
    m_datePicker = nullptr;

    wxFrame *frame = (wxFrame *)wxGetTopLevelParent(parent);
    if ( frame && frame->GetMenuBar()->IsChecked(Calendar_DatePicker_Generic) )
    {
        m_datePickerGeneric = new wxDatePickerCtrlGeneric(this, wxID_ANY, dt,
                                                          wxDefaultPosition,
                                                          wxDefaultSize,
                                                          dtpStyle);
        m_datePickerGeneric->SetRange(wxDateTime(1, wxDateTime::Jan, 1900),
                                      wxDefaultDateTime);

        datePickerWindow = m_datePickerGeneric;
    }
    else
#endif // wxUSE_DATEPICKCTRL_GENERIC
    {
        m_datePicker = new wxDatePickerCtrl(this, wxID_ANY, dt,
                                            wxDefaultPosition, wxDefaultSize,
                                            dtpStyle);
        m_datePicker->SetRange(wxDateTime(1, wxDateTime::Jan, 1900),
                               wxDefaultDateTime);

        datePickerWindow = m_datePicker;
    }

    m_dateText = new wxStaticText(this, wxID_ANY,
                                  dt.IsValid() ? dt.FormatISODate()
                                               : wxString());

    const wxSizerFlags flags = wxSizerFlags().Centre().Border();
    wxFlexGridSizer* const sizerMain = new wxFlexGridSizer(2);
    sizerMain->Add(new wxStaticText(this, wxID_ANY, "Enter &date:"), flags);
    sizerMain->Add(datePickerWindow, flags);

    sizerMain->Add(new wxStaticText(this, wxID_ANY, "Date in ISO format:"),
                   flags);
    sizerMain->Add(m_dateText, flags);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
    sizerTop->Add(sizerMain, flags);
    sizerTop->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), flags);

    SetSizerAndFit(sizerTop);
}

void MyDateDialog::OnDateChange(wxDateEvent& event)
{
    const wxDateTime dt = event.GetDate();
    if ( dt.IsValid() )
        m_dateText->SetLabel(dt.FormatISODate());
    else
        m_dateText->SetLabel(wxEmptyString);
}

#endif // wxUSE_DATEPICKCTRL

// ----------------------------------------------------------------------------
// MyTimeDialog
// ----------------------------------------------------------------------------

#if wxUSE_TIMEPICKCTRL

wxBEGIN_EVENT_TABLE(MyTimeDialog, wxDialog)
    EVT_TIME_CHANGED(wxID_ANY, MyTimeDialog::OnTimeChange)
wxEND_EVENT_TABLE()

MyTimeDialog::MyTimeDialog(wxWindow *parent)
        : wxDialog(parent, wxID_ANY, wxString("Calendar: Choose time"))
{
    wxWindow* timePickerWindow = nullptr;

#if wxUSE_TIMEPICKCTRL_GENERIC
    m_timePickerGeneric = nullptr;
    m_timePicker = nullptr;

    wxFrame *frame = (wxFrame *)wxGetTopLevelParent(parent);
    if ( frame && frame->GetMenuBar()->IsChecked(Calendar_TimePicker_Generic) )
    {
        m_timePickerGeneric = new wxTimePickerCtrlGeneric(this, wxID_ANY);
        timePickerWindow = m_timePickerGeneric;
    }
    else
#endif // wxUSE_TIMEPICKCTRL_GENERIC
    m_timePicker = new wxTimePickerCtrl(this, wxID_ANY);

    if ( !timePickerWindow )
        timePickerWindow = m_timePicker;

    m_timeText = new wxStaticText(this, wxID_ANY, GetTime().FormatISOTime());

    const wxSizerFlags flags = wxSizerFlags().Centre().Border();
    wxFlexGridSizer* const sizerMain = new wxFlexGridSizer(2);
    sizerMain->Add(new wxStaticText(this, wxID_ANY, "Enter &time:"), flags);
    sizerMain->Add(timePickerWindow, flags);

    sizerMain->Add(new wxStaticText(this, wxID_ANY, "Time in ISO format:"),
                   flags);
    sizerMain->Add(m_timeText, flags);

    wxSizer* sizerTop = new wxBoxSizer(wxVERTICAL);
    sizerTop->Add(sizerMain, flags);
    sizerTop->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), flags);

    SetSizerAndFit(sizerTop);
}

void MyTimeDialog::OnTimeChange(wxDateEvent& event)
{
    m_timeText->SetLabel(event.GetDate().FormatISOTime());
}

#endif // wxUSE_TIMEPICKCTRL

// ----------------------------------------------------------------------------
// MyAddBirthdayDialog
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyAddBirthdayDialog, wxDialog)

wxEND_EVENT_TABLE()

MyAddBirthdayDialog::MyAddBirthdayDialog(wxWindow *parent, const wxDateTime& dt, int dtpStyle)
        : wxDialog(parent, wxID_ANY, wxString("Calendar: Add Birthday"))
{
    wxWindow *datePickerWindow = nullptr;
    wxWindow *textEntry = nullptr;
    wxSize *textEntrySize = new wxSize(150, 30);

    m_datePicker = new wxDatePickerCtrl(this, wxID_ANY, dt,
                                        wxDefaultPosition, wxDefaultSize,
                                        dtpStyle);
    m_datePicker->SetRange(wxDateTime(1, wxDateTime::Jan, 1900),
                           wxDefaultDateTime);

    datePickerWindow = m_datePicker;

    m_textEntry = new wxTextCtrl(this, wxID_ANY, "",
                               wxDefaultPosition, *textEntrySize,
                               dtpStyle);

    textEntry = m_textEntry;

    const wxSizerFlags flags = wxSizerFlags().Centre().Border();
    wxFlexGridSizer* const sizerMain = new wxFlexGridSizer(2);
    sizerMain->Add(new wxStaticText(this, wxID_ANY, "Enter &date of birth:"), flags);
    sizerMain->Add(datePickerWindow, flags);
    sizerMain->Add(new wxStaticText(this, wxID_ANY, "Enter &full name:"), flags);
    sizerMain->Add(textEntry, flags);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
    sizerTop->Add(sizerMain, flags);
    sizerTop->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), flags);

    SetSizerAndFit(sizerTop);
}

// ----------------------------------------------------------------------------
// MyAddBirthdayDialog
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyRmvBirthdayDialog, wxDialog)

wxEND_EVENT_TABLE()

MyRmvBirthdayDialog::MyRmvBirthdayDialog(wxWindow *parent, int dtpStyle)
        : wxDialog(parent, wxID_ANY, wxString("Calendar: Delete a Birthday"))
{
    wxWindow *textEntry = nullptr;
    wxSize *textEntrySize = new wxSize(150, 30);

    m_textEntry = new wxTextCtrl(this, wxID_ANY, "",
                               wxDefaultPosition, *textEntrySize,
                               dtpStyle);

    textEntry = m_textEntry;

    const wxSizerFlags flags = wxSizerFlags().Centre().Border();
    wxFlexGridSizer* const sizerMain = new wxFlexGridSizer(2);
    sizerMain->Add(new wxStaticText(this, wxID_ANY, "Enter &full name:"), flags);
    sizerMain->Add(textEntry, flags);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
    sizerTop->Add(sizerMain, flags);
    sizerTop->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), flags);

    SetSizerAndFit(sizerTop);
}
