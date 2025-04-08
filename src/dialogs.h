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
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/textctrl.h"
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

#include "../build/_deps/wxwidgets-src/samples/sample.xpm"

#ifdef wxHAS_NATIVE_CALENDARCTRL
    #include "wx/generic/calctrlg.h"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------


#if wxUSE_DATEPICKCTRL

// Define a simple modal dialog which asks the user for a date
class MyDateDialog : public wxDialog
{
public:
    MyDateDialog(wxWindow *parent, const wxDateTime& dt, int dtpStyle);

    wxDateTime GetDate() const
    {
#if wxUSE_DATEPICKCTRL_GENERIC
        if ( m_datePickerGeneric )
            return m_datePickerGeneric->GetValue();
#endif // wxUSE_DATEPICKCTRL_GENERIC

        return m_datePicker->GetValue();
    }

private:
    void OnDateChange(wxDateEvent& event);


    wxDatePickerCtrl *m_datePicker;
#if wxUSE_DATEPICKCTRL_GENERIC
    wxDatePickerCtrlGeneric *m_datePickerGeneric;
#endif // wxUSE_DATEPICKCTRL_GENERIC

    wxStaticText *m_dateText;


    wxDECLARE_EVENT_TABLE();
};

#endif // wxUSE_DATEPICKCTRL

#if wxUSE_TIMEPICKCTRL

// Another simple dialog, this one asking for time.
class MyTimeDialog : public wxDialog
{
public:
    MyTimeDialog(wxWindow* parent);

    wxDateTime GetTime() const
    {
#if wxUSE_TIMEPICKCTRL_GENERIC
        if ( m_timePickerGeneric )
            return m_timePickerGeneric->GetValue();
#endif // wxUSE_TIMEPICKCTRL_GENERIC

        return m_timePicker->GetValue();
    }

private:
    void OnTimeChange(wxDateEvent& event);

    wxTimePickerCtrl* m_timePicker;
#if wxUSE_TIMEPICKCTRL_GENERIC
    wxTimePickerCtrlGeneric* m_timePickerGeneric;
#endif // wxUSE_TIMEPICKCTRL_GENERIC

    wxStaticText* m_timeText;

    wxDECLARE_EVENT_TABLE();
};

#endif // wxUSE_TIMEPICKCTRL

// Add birthday functionality
class MyAddBirthdayDialog : public wxDialog
{
public:
    MyAddBirthdayDialog(wxWindow *parent, const wxDateTime& dt, int dtpStyle);

    wxTextCtrl *m_textEntry;

    wxDateTime GetDate() const
    {
        return m_datePicker->GetValue();
    }

private:

    wxDatePickerCtrl *m_datePicker;

    wxStaticText *m_dateText;

    wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Calendar_File_About = wxID_ABOUT,
    Calendar_File_ClearLog = wxID_CLEAR,
    Calendar_File_Quit = wxID_EXIT,
    Calendar_Cal_Generic = 200,
    Calendar_Cal_AutoWeekday,
    Calendar_Cal_Sunday,
    Calendar_Cal_Monday,
    Calendar_Cal_Holidays,
    Calendar_Cal_Special,
    Calendar_Cal_Month,
    Calendar_Cal_LimitDates,
    Calendar_Cal_SeqMonth,
    Calendar_Cal_SurroundWeeks,
    Calendar_Cal_WeekNumbers,
    Calendar_Cal_SetDate,
    Calendar_Cal_Today,
    Calendar_Cal_BeginDST,
    Calendar_Cal_Resizable,
#if wxUSE_DATEPICKCTRL
    Calendar_DatePicker_AskDate = 300,
    Calendar_DatePicker_ShowCentury,
    Calendar_DatePicker_DropDown,
    Calendar_DatePicker_AllowNone,
    Calendar_DatePicker_StartWithNone,
#if wxUSE_DATEPICKCTRL_GENERIC
    Calendar_DatePicker_Generic,
#endif // wxUSE_DATEPICKCTRL_GENERIC
#endif // wxUSE_DATEPICKCTRL
#if wxUSE_TIMEPICKCTRL
    Calendar_TimePicker_AskTime = 400,
#if wxUSE_TIMEPICKCTRL_GENERIC
    Calendar_TimePicker_Generic,
#endif // wxUSE_TIMEPICKCTRL_GENERIC
#endif // wxUSE_TIMEPICKCTRL
    Calendar_BirthdayAdd_Form = 500,
    Calendar_CalCtrl = 1000
};
