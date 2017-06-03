#ifndef _PENV_PENVITEM_H_
#define _PENV_PENVITEM_H_
/*
 * penvitem.h - Declaration of penvitem
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 06/30/2008
 *
 */

#include "wxincludes.h"
#include "penvitemtype.h"

namespace penv {

/**
 * \brief The PenvItem class is the base class for Workspace,
 * Project and ProjectItem.
 * This class could change in future, do not use.
 **/
class PenvItem
{
    private:
        PenvItem();

    public:
        PenvItem(PenvItemType penvitemtype);
        virtual ~PenvItem();

    public:
        PenvItemType GetPenvItemType() const;
        void SetPenvItemType(PenvItemType penvitemtype);

    protected:
        PenvItemType m_penvitemtype;
};
} //namespace penv


#endif // _PENV_PENVITEM_H_
