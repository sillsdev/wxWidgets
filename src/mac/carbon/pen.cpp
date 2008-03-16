/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/pen.cpp
// Purpose:     wxPen
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/pen.h"

#ifndef WX_PRECOMP
    #include "wx/utils.h"
#endif

IMPLEMENT_DYNAMIC_CLASS(wxPen, wxGDIObject)

class WXDLLEXPORT wxPenRefData : public wxGDIRefData
{
public:
    wxPenRefData();
    wxPenRefData(const wxPenRefData& data);
    virtual ~wxPenRefData();

    wxPenRefData& operator=(const wxPenRefData& data);

    bool operator==(const wxPenRefData& data) const
    {
        // we intentionally don't compare m_hPen fields here
        return m_style == data.m_style &&
            m_width == data.m_width &&
            m_join == data.m_join &&
            m_cap == data.m_cap &&
            m_colour == data.m_colour &&
            (m_style != wxSTIPPLE || m_stipple.IsSameAs(data.m_stipple)) &&
            (m_style != wxUSER_DASH ||
             (m_nbDash == data.m_nbDash &&
              memcmp(m_dash, data.m_dash, m_nbDash*sizeof(wxDash)) == 0));
    }

protected:
    int           m_width;
    wxPenStyle    m_style;
    wxPenJoin     m_join ;
    wxPenCap      m_cap ;
    wxBitmap      m_stipple ;
    int           m_nbDash ;
    wxDash *      m_dash ;
    wxColour      m_colour;
    /* TODO: implementation
       WXHPEN        m_hPen;
       */

    friend class WXDLLIMPEXP_FWD_CORE wxPen;
};

wxPenRefData::wxPenRefData()
{
    m_style = wxSOLID;
    m_width = 1;
    m_join = wxJOIN_ROUND ;
    m_cap = wxCAP_ROUND ;
    m_nbDash = 0 ;
    m_dash = 0 ;
}

wxPenRefData::wxPenRefData(const wxPenRefData& data)
: wxGDIRefData()
{
    m_style = data.m_style;
    m_width = data.m_width;
    m_join = data.m_join;
    m_cap = data.m_cap;
    m_nbDash = data.m_nbDash;
    m_dash = data.m_dash;
    m_colour = data.m_colour;
}

wxPenRefData::~wxPenRefData()
{
}

// Pens

#define M_PENDATA ((wxPenRefData *)m_refData)

wxPen::wxPen()
{
}

wxPen::~wxPen()
{
}

// Should implement Create
wxPen::wxPen(const wxColour& col, int Width, wxPenStyle Style)
{
    m_refData = new wxPenRefData;

    M_PENDATA->m_colour = col;
    M_PENDATA->m_width = Width;
    M_PENDATA->m_style = Style;
    M_PENDATA->m_join = wxJOIN_ROUND ;
    M_PENDATA->m_cap = wxCAP_ROUND ;
    M_PENDATA->m_nbDash = 0 ;
    M_PENDATA->m_dash = 0 ;

    RealizeResource();
}

wxPen::wxPen(const wxColour& col, int Width, wxBrushStyle Style)
{
    m_refData = new wxPenRefData;

    M_PENDATA->m_colour = col;
    M_PENDATA->m_width = Width;
    M_PENDATA->m_style = (wxPenStyle)Style;
    M_PENDATA->m_join = wxJOIN_ROUND ;
    M_PENDATA->m_cap = wxCAP_ROUND ;
    M_PENDATA->m_nbDash = 0 ;
    M_PENDATA->m_dash = 0 ;

    RealizeResource();
}

wxPen::wxPen(const wxBitmap& stipple, int Width)
{
    m_refData = new wxPenRefData;

    M_PENDATA->m_stipple = stipple;
    M_PENDATA->m_width = Width;
    M_PENDATA->m_style = wxSTIPPLE;
    M_PENDATA->m_join = wxJOIN_ROUND ;
    M_PENDATA->m_cap = wxCAP_ROUND ;
    M_PENDATA->m_nbDash = 0 ;
    M_PENDATA->m_dash = 0 ;

    RealizeResource();
}

wxGDIRefData *wxPen::CreateGDIRefData() const
{
    return new wxPenRefData;
}

wxGDIRefData *wxPen::CloneGDIRefData(const wxGDIRefData *data) const
{
    return new wxPenRefData(*wx_static_cast(const wxPenRefData *, data));
}

bool wxPen::operator==(const wxPen& pen) const
{
    const wxPenRefData *penData = (wxPenRefData *)pen.m_refData;

    // an invalid pen is only equal to another invalid pen
    return m_refData ? penData && *M_PENDATA == *penData : !penData;
}

wxColour& wxPen::GetColour() const
{
    return (M_PENDATA ? M_PENDATA->m_colour : wxNullColour);
}

int wxPen::GetWidth() const
{
    return (M_PENDATA ? M_PENDATA->m_width : 0);
}

wxPenStyle wxPen::GetStyle() const
{
    return (M_PENDATA ? M_PENDATA->m_style : 0);
}

wxPenJoin wxPen::GetJoin() const
{
    return (M_PENDATA ? M_PENDATA->m_join : 0);
}

wxPenCap wxPen::GetCap() const
{
    return (M_PENDATA ? M_PENDATA->m_cap : 0);
}

int wxPen::GetDashes(wxDash **ptr) const
{
    *ptr = (M_PENDATA ? M_PENDATA->m_dash : (wxDash*) NULL); return (M_PENDATA ? M_PENDATA->m_nbDash : 0);
}

wxBitmap *wxPen::GetStipple() const
{
    return (M_PENDATA ? (& M_PENDATA->m_stipple) : (wxBitmap*) NULL);
}

void wxPen::Unshare()
{
    // Don't change shared data
    if (!m_refData)
    {
        m_refData = new wxPenRefData();
    }
    else
    {
        wxPenRefData* ref = new wxPenRefData(*(wxPenRefData*)m_refData);
        UnRef();
        m_refData = ref;
    }
}

void wxPen::SetColour(const wxColour& col)
{
    Unshare();

    M_PENDATA->m_colour = col;

    RealizeResource();
}

void wxPen::SetColour(unsigned char r, unsigned char g, unsigned char b)
{
    Unshare();

    M_PENDATA->m_colour.Set(r, g, b);

    RealizeResource();
}

void wxPen::SetWidth(int Width)
{
    Unshare();

    M_PENDATA->m_width = Width;

    RealizeResource();
}

void wxPen::SetStyle(wxPenStyle Style)
{
    Unshare();

    M_PENDATA->m_style = Style;

    RealizeResource();
}

void wxPen::SetStipple(const wxBitmap& Stipple)
{
    Unshare();

    M_PENDATA->m_stipple = Stipple;
    M_PENDATA->m_style = wxSTIPPLE;

    RealizeResource();
}

void wxPen::SetDashes(int nb_dashes, const wxDash *Dash)
{
    Unshare();

    M_PENDATA->m_nbDash = nb_dashes;
    M_PENDATA->m_dash = (wxDash *)Dash;

    RealizeResource();
}

void wxPen::SetJoin(wxPenJoin Join)
{
    Unshare();

    M_PENDATA->m_join = Join;

    RealizeResource();
}

void wxPen::SetCap(wxPenCap Cap)
{
    Unshare();

    M_PENDATA->m_cap = Cap;

    RealizeResource();
}

bool wxPen::RealizeResource()
{
    // nothing to do here for mac
    return true;
}
