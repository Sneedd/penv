/*
 * projecttreectrlapperance.h - Declaration of the ProjectTreeCtrlApperance class
 *
 * Author: Steffen Ott
 * Date: 03/18/2007
 *
 */


#ifndef _PENV_UI_PROJECTTREECTRLAPPERANCE_H_
#define _PENV_UI_PROJECTTREECTRLAPPERANCE_H_

#include "../wxincludes.h"

namespace penv {


/**
 * \brief The ProjectTreeCtrlApperance class represents the apperance
 * of the ProjectTreeCtrl. Especially the Images and the Colors of
 * the items in the tree control. To change the apperance it is recommended,
 * that you inherit from this class and change the protected
 * variable members and override the GetImageIndex____() Methods.
 **/
class ProjectTreeCtrlApperance
{
    public:
        ProjectTreeCtrlApperance();
        virtual ~ProjectTreeCtrlApperance();

        void SetDefaultValues();

        const wxColor& GetFileColor();
        const wxColor& GetDirectoryColor();
        const wxColor& GetLinkedItemsColor();
        const wxColor& GetSubProjectColor();

        wxImageList* GetImageList();
        virtual int GetImageIndexFile(const wxString& ext = wxEmptyString);
        virtual int GetImageIndexDirectory();
        virtual int GetImageIndexLinkedItems();
        virtual int GetImageIndexSubProject();
        virtual int GetImageIndexProject();
        virtual int GetImageIndexWorkspace();

    protected:
        wxImageList* m_imagelist;
        wxColor m_filecolor;
        wxColor m_directorycolor;
        wxColor m_linkeditemscolor;
        wxColor m_subprojectcolor;
};

} // namespace penv

#endif // _PENV_UI_PROJECTTREECTRLAPPERANCE_H_
