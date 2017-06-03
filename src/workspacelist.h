/*
 * workspacelist.h - Declaration of the WorkspaceList class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/01/2007
 *
 */


#ifndef _PENV_WORKSPACELIST_H_
#define _PENV_WORKSPACELIST_H_

#include "workspace.h"
#include "container/objarray.h"
#include "commandevent.h"


namespace penv {

/**
 * \brief The WorkspaceList class is a container for workspaces. The main
 * function of this class are adding, removing and copying workspaces. The
 * workspacelist can fully saved into an xml file, which is defined later.
 * This file can be used to save the names and paths to a workspace, so that
 * the application can find the workspace on disk.<br>
 * Here is the definition of a 'workspaces' xml element:<br>
 * <code>
 *  &lt;workspaces&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- workspace elements should be here --&gt;<br>
 *  &lt;/workspaces&gt;<br>
 * </code>
 * \todo Implement CopyWorkspace() method.
 **/
class WorkspaceList
{
    private:
        friend class Environment;
        WorkspaceList();
        ~WorkspaceList();

    public:
        Workspace* GetItem(size_t index);
        int GetItemIndex(Workspace* workspace);
        bool Add(Workspace* workspace);
        bool Remove(size_t index);
        void Clear();
        size_t Count() const;

        bool CopyWorkspace(size_t index);
        bool IsWorkspaceOrProjectModified();
        bool ReloadWorkspaces();

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

        void PrintHierarchy();
    private:
        void PrintHierarchy(ProjectItemList* list, int depth);

    public:
        void OnItemActivated(CommandEvent& event);


    private:
        Array<Workspace>* m_array;
};

} // namespace penv

#endif // _PENV_WORKSPACELIST_H_
