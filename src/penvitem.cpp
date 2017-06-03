/*
 * penvitem.cpp - Implementation of penvitem
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 06/30/2008
 *
 */

#include "penvitem.h"

namespace penv {

PenvItem::PenvItem()
/**
 * \brief Constructor (private).
 **/
{
}


//----------------------------------------------------------------
PenvItem::PenvItem(PenvItemType penvitemtype)
/**
 * \brief Constructor initializes the PenvItem object.
 **/
{
    m_penvitemtype = penvitemtype;
}

//----------------------------------------------------------------
PenvItem::~PenvItem()
/**
 * \brief Destructor (private).
 **/
{
}


//----------------------------------------------------------------


PenvItemType PenvItem::GetPenvItemType() const
/**
 * \brief Returns the type of the penv item.
 * \return type of the penv item.
 **/
{
    return m_penvitemtype;
}


//----------------------------------------------------------------
void PenvItem::SetPenvItemType(PenvItemType penvitemtype)
/**
 * \brief Sets the type of the penv item.
 * \param PenvItemType type of the penv item.
 **/
{
    m_penvitemtype = penvitemtype;
}

} //namespace penv
