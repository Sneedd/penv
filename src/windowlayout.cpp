/*
 * windowlayout.cpp - Implementation of the WindowLayout class
 *
 * Author: Steffen Ott
 * Date: 05/01/2008
 *
 */


#include "windowlayout.h"

namespace penv {

//----------------------------------------------------------------
WindowLayout::WindowLayout()
/**
 * \brief Constructor, initializes the WindowLayout object.
 **/
{
    // Nothing to do
}


//----------------------------------------------------------------
WindowLayout::~WindowLayout()
/**
 * \brief Destructor.
 **/
{
    // Nothing to do
}



//----------------------------------------------------------------
const wxAuiPaneInfo& WindowLayout::GetPaneInfo(size_t index)
/**
 * \brief Returns the wxAuiPaneInfo for this window.
 * \param index Index of the wxAuiPaneInfo object.
 * \return The wxAuiPaneInfo.
 **/
{
    return (m_array[index]);
}


//----------------------------------------------------------------
const wxString& WindowLayout::GetClassId(size_t index)
/**
 * \brief Returns the class id for this window.
 * \param index Index of the class id.
 * \return The class id.
 **/
{
    return (m_classidarray[index]);
}


//----------------------------------------------------------------
const wxString& WindowLayout::GetProjectItem(size_t index)
/**
 * \brief Returns the string representation of a project item.
 * \param index Index for the project item.
 * \return Project item string represtation.
 **/
{
    return (m_projectitemarray[index]);
}


//----------------------------------------------------------------
void WindowLayout::Add(const wxAuiPaneInfo& paneinfo,
    const wxString& classid, const wxString& projectitem)
/**
 * \brief Adds a new window layout to this container.
 * \param paneinfo Pane info for this object.
 * \param classid Class id for this object.
 * \param projectitem String represtation of the project item.
 **/
{
    m_array.Add(paneinfo);
    m_classidarray.Add(classid);
    m_projectitemarray.Add(classid);
}


//----------------------------------------------------------------
size_t WindowLayout::Count() const
/**
 * \brief Returns the number of window layouts
 * of this container.
 * \return Number of window layouts.
 **/
{
    return (m_array.Count());
}


} // namespace penv

