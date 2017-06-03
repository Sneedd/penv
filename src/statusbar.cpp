/*
 * statusbar.cpp - Implementation of the StatusBar class
 *
 * Author: Steffen Ott
 * Date: 09/21/2007
 *
 */


#include "statusbar.h"
#include "expect.h"
#include "penvhelper.h"


namespace penv {


//----------------------------------------------------------------
StatusBar::StatusBar()
/**
 * \brief Constructor (private).
 **/
{
}


//----------------------------------------------------------------
StatusBar::StatusBar(wxWindow* parent, int id)
    : wxStatusBar(parent, id, wxST_SIZEGRIP)
/**
 * \brief Constructor, initialize the statusbar.
 **/
{
}


//----------------------------------------------------------------
StatusBar::~StatusBar()
/**
 * \brief Destructor.
 **/
{
}



} // namespace penv


