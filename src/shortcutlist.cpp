/*
 * shorcutlist.cpp - Implementation of the ShortcutList class
 *
 * Author: Steffen Ott
 * Date: 03/17/2008
 *
 */

#include "shortcutlist.h"
#include "penvhelper.h"

namespace penv {


//----------------------------------------------------------------
ShortcutList::ShortcutList()
/**
 * \brief
 **/
{
    //ctor
    NOT_IMPLEMENTED_YET();
}

//----------------------------------------------------------------
ShortcutList::~ShortcutList()
/**
 * \brief
 **/
{
    //dtor
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
bool ShortcutList::Update()
/**
 * \brief
 **/
{
    NOT_IMPLEMENTED_YET();
    return (true);
}


//----------------------------------------------------------------
bool ShortcutList::ReadNode(wxXmlNode* node)
/**
 * \brief
 **/
{
    NOT_IMPLEMENTED_YET();
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* ShortcutList::WriteNode()
/**
 * \brief
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("shortcuts"), _T(" "));
    NOT_IMPLEMENTED_YET();
    return (node);
}


} // namespace penv
