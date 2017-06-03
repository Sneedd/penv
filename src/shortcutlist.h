/*
 * shortcutlist.h - Declaration of the ShortcutList Class
 *
 * Author: Steffen Ott
 * Date: 03/17/2008
 *
 */

#ifndef _PENV_SHORTCUTLIST_H_
#define _PENV_SHORTCUTLIST_H_


#include "wxincludes.h"


namespace penv {

/**
 * \brief The ShortcutList class is a container for Shortcut's.
 * \todo Not implemented yet.
 **/
class ShortcutList
{
    public:
        ShortcutList();
        ~ShortcutList();


        bool Update();

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

    protected:
    private:
};

} // namespace penv


#endif // _PENV_SHORTCUTLIST_H_
