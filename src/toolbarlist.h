/*
 * toolbarlist.h - Declaration of the ToolBarList class
 *
 * Author: Steffen Ott
 * Date: 10/01/2007
 *
 */

#ifndef _PENV_TOOLBARLIST_H_
#define _PENV_TOOLBARLIST_H_

#include "wxincludes.h"
#include "toolbar.h"
#include "applicationframe.h"

namespace penv {

/**
 * \brief The ToolBarList class is a container for ToolBars.
 * The layout of toolbars will be stored in an XML file which
 * can be edited manually or via the ToolBarWindow class.
 * ToolBars can also be created, edited or removed programmatically.
 * ToolBars will be stored with an unique ID in an HashMap within
 * this class.<br>
 * Here the definition of a 'toolbars' xml element:<br>
 * <code>
 *  &lt;toolbars&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- toolbar elements should be here --&gt;<br>
 *  &lt;/toolbars&gt;<br>
 * </code>
 **/
class ToolBarList
{
    private:
        friend class CommandList;
        ToolBarList();
        ToolBarList(ApplicationFrame* frame, wxAuiManager* manager);
        ~ToolBarList();
    public:
        wxArrayString* GetToolBars();
        ToolBar* GetToolBar(const wxString& id);
        bool Add(ToolBar* toolbar);
        bool Remove(const wxString& id);
        void Clear();
        wxString NewId();

        Array<ToolBarItem>* FindToolBarItems(const wxString& commandid);
        bool Update();
        void RefreshPaneInfos();
        void RealizeAllToolBars();

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();


    private:
        WX_DECLARE_STRING_HASH_MAP(ToolBar*, ToolBarHashMap);
        ToolBarHashMap* m_hashmap;
        ApplicationFrame* m_frame;
        wxAuiManager* m_manager;
};

} // namespace penv

#endif // _PENV_TOOLBARLIST_H_
