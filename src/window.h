/*
 * window.h - Declaration of the Window class
 *
 * Author: Steffen Ott
 * Date: 09/23/2007
 *
 */


#ifndef _PENV_WINDOW_H_
#define _PENV_WINDOW_H_

#include "wxincludes.h"
#include "windowtype.h"
#include "projectitem.h"


namespace penv {

/**
 * \brief The window class represents a abstract window in the framework.
 * The class itself is not used and always returns the type penvWT_None.
 * From class Window are inherited class WindowPane and WindowDocument.
 * There are two different ID's for a window.<br>
 * The class id is often called registered ID, is created in the constructor
 * and does not change for the inherited window. The window id or open window
 * id is only set if a window is opened by the framework and therefore visible
 * for the user.<br>
 **/
class Window : public wxEvtHandler
{
    private:
        friend class WindowList;
        Window();
    public:
        Window(WindowType type, const wxString& classid);
        virtual ~Window();
        void Initialize(const wxString& caption = wxEmptyString, ProjectItem* item = NULL);

        virtual wxString GetCaption() const;
        virtual void SetCaption(const wxString& caption = wxEmptyString);

        wxWindow* GetWindow() const;
        WindowType GetType() const;

        void Modified(bool modified=true);
        bool IsModified() const;

        const wxString& GetClassId() const;

        const wxString& GetId() const;
        void SetId(const wxString& id);

        void SetProjectItem(ProjectItem* item);
        ProjectItem* GetProjectItem() const;

    private:
        void SetWindow(wxWindow* window);

    public:
        // Nur in WindowPane und WindowDokument abgeleitet, danach nicht mehr virtual!
        virtual bool InitializeUI();
        virtual bool Layout();
        virtual bool Load(const wxString& filename = wxEmptyString);
        virtual bool Save(const wxString& filename = wxEmptyString);
        virtual bool Update();

    protected:

        /**
         * \brief This method is called to create a window. This is
         * a abstract method and must be overriddden.
         * \param item Project item or NULL.
         * \return Created window or NULL on error.
         **/
        virtual Window* OnCreate(ProjectItem* item = NULL) = 0;

        /**
         * \brief This method is called to initialize the window user
         * interface components. This is a abstract method and must
         * be overriddden.
         * \param parent The parent window for this window.
         * \return True on success; false otherwise.
         **/
        virtual bool OnInitializeUI(wxWindow* parent) = 0;

        /**
         * \brief This method is called to update the window layout.
         * This is a abstract method and must be overriddden.
         * \return True on success; false otherwise.
         **/
        virtual bool OnLayout() = 0;

        /**
         * \brief This method is called to load the window content.
         * This is a abstract method and must be overriddden.
         * \param filename The filename from where to load the content.
         * \return True on success; false otherwise.
         **/
        virtual bool OnLoad(const wxString& filename = wxEmptyString) = 0;

        /**
         * \brief This method is called to save the window content.
         * This is a abstract method and must be overriddden.
         * \param filename The filename where to save the content.
         * \return True on success; false otherwise.
         **/
        virtual bool OnSave(const wxString& filename = wxEmptyString) = 0;

        /**
         * \brief This method is called to close the window.
         * This is a abstract method and must be overriddden.
         * \param cancel Set this to false if you want to cancel the
         * close action.
         * \return True on success; false otherwise.
         **/
        virtual bool OnClose(bool& cancel) = 0;

        /**
         * \brief This method is called to update the window.
         * This is a abstract method and must be overriddden.
         * \return True on success; false otherwise.
         **/
        virtual bool OnUpdate() = 0;


    protected:
        ProjectItem* m_item;
        wxWindow* m_window;
        wxString m_caption;
        WindowType m_type;
        wxString m_classid;
        wxString m_id;
        bool m_modified;
        bool m_registered;
};

}; // namespace penv

#endif // _PENV_WINDOW_H_
