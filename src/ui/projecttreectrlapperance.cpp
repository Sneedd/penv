/*
 * projecttreectrlapperance.cpp - Implementation of the ProjectTreeCtrlApperance class
 *
 * Author: Steffen Ott
 * Date: 03/18/2007
 *
 */


#include "projecttreectrlapperance.h"

namespace penv {

//----------------------------------------------------------------
ProjectTreeCtrlApperance::ProjectTreeCtrlApperance()
/**
 * \brief
 **/
{
    SetDefaultValues();
}


//----------------------------------------------------------------
ProjectTreeCtrlApperance::~ProjectTreeCtrlApperance()
/**
 * \brief
 **/
{
    if (m_imagelist != NULL) delete m_imagelist;
}

//----------------------------------------------------------------
void ProjectTreeCtrlApperance::SetDefaultValues()
/**
 * \brief
 **/
{
    // TODO SO : Nur DEMO Daten
    m_imagelist = new wxImageList(16,16);
    m_imagelist->Add(wxBitmap(_T("data\\treeitemfile.png"), wxBITMAP_TYPE_PNG));
    m_imagelist->Add(wxBitmap(_T("data\\treeitemdirectory.png"), wxBITMAP_TYPE_PNG));
    m_imagelist->Add(wxBitmap(_T("data\\treeitemlinkeditems.png"), wxBITMAP_TYPE_PNG));
    m_imagelist->Add(wxBitmap(_T("data\\treeitemsubproject.png"), wxBITMAP_TYPE_PNG));
    m_imagelist->Add(wxBitmap(_T("data\\treeitemproject.png"), wxBITMAP_TYPE_PNG));
    m_imagelist->Add(wxBitmap(_T("data\\treeitemworkspace.png"), wxBITMAP_TYPE_PNG));
    m_filecolor = wxColor(0x00, 0x00, 0x00);
    m_directorycolor = wxColor(0x00, 0x00, 0x00);
    m_linkeditemscolor = wxColor(0x30, 0x30, 0x30);
    m_subprojectcolor = wxColor(0x00, 0x00, 0x00);
}

//----------------------------------------------------------------
const wxColor& ProjectTreeCtrlApperance::GetFileColor()
/**
 * \brief
 **/
{
    return (m_filecolor);
}


//----------------------------------------------------------------
const wxColor& ProjectTreeCtrlApperance::GetDirectoryColor()
/**
 * \brief
 **/
{
    return (m_directorycolor);
}


//----------------------------------------------------------------
const wxColor& ProjectTreeCtrlApperance::GetLinkedItemsColor()
/**
 * \brief
 **/
{
    return (m_linkeditemscolor);
}


//----------------------------------------------------------------
const wxColor& ProjectTreeCtrlApperance::GetSubProjectColor()
/**
 * \brief
 **/
{
    return (m_subprojectcolor);
}


//----------------------------------------------------------------
wxImageList* ProjectTreeCtrlApperance::GetImageList()
/**
 * \brief
 **/
{
    return (m_imagelist);
}


//----------------------------------------------------------------
int ProjectTreeCtrlApperance::GetImageIndexFile(const wxString& ext)
/**
 * \brief
 **/
{
    return (0);
}


//----------------------------------------------------------------
int ProjectTreeCtrlApperance::GetImageIndexDirectory()
/**
 * \brief
 **/
{
    return (1);
}


//----------------------------------------------------------------
int ProjectTreeCtrlApperance::GetImageIndexLinkedItems()
/**
 * \brief
 **/
{
    return (2);
}


//----------------------------------------------------------------
int ProjectTreeCtrlApperance::GetImageIndexSubProject()
/**
 * \brief
 **/
{
    return (3);
}



//----------------------------------------------------------------
int ProjectTreeCtrlApperance::GetImageIndexProject()
/**
 * \brief
 **/
{
    return (4);
}


//----------------------------------------------------------------
int ProjectTreeCtrlApperance::GetImageIndexWorkspace()
/**
 * \brief
 **/
{
    return (5);
}



} // namespace penv


