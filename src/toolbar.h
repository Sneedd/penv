/*
 * toolbar.h - Declaration of the ToolBar class
 *
 * Author: Steffen Ott
 * Date: 11/05/2007
 *
 */


#ifndef _PENV_TOOLBAR_H_
#define _PENV_TOOLBAR_H_


#include "wxincludes.h"
#include "toolbaritem.h"
#include "container/objarray.h"


namespace penv {

/**
 * \brief The ToolBar class represents one toolbar in the
 * framework. The framework can handle more than one toolbar
 * and is able to create, edit or remove a toolbar within
 * the framework. You can programmatically create an toolbar,
 * use the xml file to create own toolbars or use an internally
 * dialog for editing toolbars.<br>
 * Here the definition of a 'toolbar' xml element:<br>
 * <code>
 *  &lt;toolbar id="UNIQUEID" name="toolbar name" visible="true"&gt;<br>
 *  &nbsp;&nbsp;&lt;position&gt;<br>
 *  &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- Position information for the toolbar --&gt;<br>
 *  &nbsp;&nbsp;&lt;/position&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- toolbar elements should be here --&gt;<br>
 *  &lt;/toolbar&gt;<br>
 * </code>
 **/
class ToolBar
{
    public:
        friend class ToolBarList;
        ToolBar();
        ~ToolBar();

        void SetName(const wxString& name);
        const wxString& GetName() const;

        void SetId(const wxString& id);
        const wxString& GetId() const;

        void Visible(bool visible=true);
        bool IsVisible() const;

        ToolBarItem* GetItem(size_t index);
        bool Add(ToolBarItem* item);
        bool Insert(size_t index, ToolBarItem* item);
        bool Remove(ToolBarItem* item);
        bool Remove(size_t index);
        void Clear();
        size_t Count();

        bool Update();

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();


    public:
        wxToolBar* GetToolBar();
    private:
        void SetToolBar(wxToolBar* toolbar);
    public:
        wxAuiPaneInfo& GetPaneInfo();
    private:
        void SetPaneInfo(const wxAuiPaneInfo& paneinfo);
        void CorrectToolbarId();
        void ClearToolEvents();

    private:
        wxToolBar* m_toolbar;
        Array<ToolBarItem>* m_array;
        wxString m_name;
        wxString m_id;
        bool m_visible;
        wxAuiPaneInfo m_paneinfo;

        int* m_toolbarids;
        size_t m_counttoolbarids;

};


} // namespace penv

#endif // _PENV_TOOLBAR_H_
