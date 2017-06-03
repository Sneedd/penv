/*
 * editorpackage.h - Declaration of the EditorPackage class
 *
 * Author: Steffen Ott
 * Date: 05/16/2008
 *
 */


#include "editorpackage.h"
#include "scintilladocument.h"



PENV_INITALIZE_STATIC_PACKAGE(EditorPackage);

//----------------------------------------------------------------
EditorPackage::EditorPackage()
    : penv::Package(_("Scintilla Editor Package"), _T("EDITOR_PACKAGE"), penv::Version(0,1,0))
/**
 * \brief
 **/
{
    // Dependency to main package
    Add(new penv::PackageDependency(_T("MAIN_PACKAGE"), penv::Version(0,0,0)));

}



//----------------------------------------------------------------
EditorPackage::~EditorPackage()
/**
 * \brief
 **/
{
}

//----------------------------------------------------------------
int EditorPackage::OnInitPackage()
/**
 * \brief
 **/
{
    // Get all needed lists
    penv::Environment* env = penv::Environment::Get();
    penv::CommandList* cmdlist = env->GetCommandList();
    penv::Command* cmd = NULL;

    // Set the scintilla windows as the default editor
    env->SetDefaultDocument(_T("EDITOR_SCINTILLA"));

    // Register Windows
    env->GetWindowList()->Register(new ScintillaDocument());

    // Register commands for the editor
    cmd = new penv::Command(_T("EDITOR_FILE_NEW"), _("New File ..."), _("Create a new text file whithin a dialog."));
    cmd->SetIcon(wxBitmap(_T("data\\new.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<EditorPackage>(this,&EditorPackage::OnFileNew));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("EDITOR_FILE_OPEN"), _("Open File ..."), _("Opens a dialog to select a file to open."));
    cmd->SetIcon(wxBitmap(_T("data\\open.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<EditorPackage>(this,&EditorPackage::OnFileOpen));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("EDITOR_FILE_SAVE"), _("Save File"), _("Save the actual opened file."));
    cmd->SetIcon(wxBitmap(_T("data\\save.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<EditorPackage>(this,&EditorPackage::OnFileSave));
    cmd->Enable(false);
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("EDITOR_FILE_SAVEAS"), _("Save As File ..."), _("Opens a dialog where to save the file."));
    cmd->Connect(new penv::CommandCallback<EditorPackage>(this,&EditorPackage::OnFileSaveAs));
    cmd->Enable(false);
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("EDITOR_FILE_CLOSE"), _("Close File"), _("Closes the actual opend file."));
    cmd->Connect(new penv::CommandCallback<EditorPackage>(this,&EditorPackage::OnFileClose));
    cmd->Enable(false);
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("EDITOR_ZOOM_OUT"), _("Zoom out"), _("Zoom out the actual text in the document."));
    cmd->SetIcon(wxBitmap(_T("data\\zoom_out.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<EditorPackage>(this,&EditorPackage::OnZoomOut));
    cmd->Enable(false);
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("EDITOR_ZOOM_IN"), _("Zoom in"), _("Zoom in the actual text in the document."));
    cmd->SetIcon(wxBitmap(_T("data\\zoom_in.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<EditorPackage>(this,&EditorPackage::OnZoomIn));
    cmd->Enable(false);
    cmdlist->Add(cmd);

    cmd = cmdlist->GetCommand(_T("PENV_EVT_ACTIVATED_PROJECTITEMFILE"));
    cmd->Connect(new penv::CommandCallback<EditorPackage>(this,&EditorPackage::OnProjectItemFileActivated));

    return (penv::penvPKG_NOERROR);
}



//----------------------------------------------------------------
int EditorPackage::OnExitPackage()
/**
 * \brief
 **/
{
    return (penv::penvPKG_NOERROR);
}


//----------------------------------------------------------------
void EditorPackage::OnFileNew(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment::Get()->GetFrame()->GetProjectTreeCtrl()->LogTreeView();
}



//----------------------------------------------------------------
void EditorPackage::OnFileOpen(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::WindowList* windowlist = env->GetWindowList();
    wxFileDialog dialog(env->GetFrame(), _("Choose a file"),
        wxGetCwd(), _T(""),
        _T("*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
    if(dialog.ShowModal() == wxID_OK)
    {
        wxArrayString filenames;
        dialog.GetFilenames(filenames);
        for(size_t i = 0; i < filenames.Count(); i++)
        {
            wxString filename = filenames[i];
            penv::WindowDocument* windoc = (penv::WindowDocument*)windowlist->CreateWindow(_T("EDITOR_SCINTILLA"),NULL);
            windowlist->Add(windoc, true);
            windoc->SetCaption(filename);
            windoc->Load(filename);
        }
    }
}

//----------------------------------------------------------------
void EditorPackage::OnFileSave(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::WindowList* windowlist = env->GetWindowList();
    penv::WindowDocument* doc = windowlist->GetSelectedNotebookPage();
    if(doc == NULL)
    {
        return;
    }
    if(!doc->Save())
    {
        wxMessageBox(_T("Error occured while saving."), _T("Error"),
            wxOK|wxICON_ERROR, env->GetFrame());
    }
}

//----------------------------------------------------------------
void EditorPackage::OnFileSaveAs(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::WindowList* windowlist = env->GetWindowList();
    penv::WindowDocument* doc = windowlist->GetSelectedNotebookPage();
    if(doc == NULL)
    {
        return;
    }
    wxFileDialog dialog(env->GetFrame(), _("Choose a file"),
        wxGetCwd(), _T(""),
        _T("*.*"), wxFD_SAVE  | wxFD_OVERWRITE_PROMPT );
    if(dialog.ShowModal() == wxID_OK)
    {
        wxString filename = dialog.GetFilename();
        if(!doc->Save(filename))
        {
            wxMessageBox(_T("Error occured while saving."), _T("Error"),
                wxOK|wxICON_ERROR, env->GetFrame());
        }
    }
}

//----------------------------------------------------------------
void EditorPackage::OnFileClose(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::WindowList* windowlist = env->GetWindowList();
    penv::WindowDocument* doc = windowlist->GetSelectedNotebookPage();
    if(doc == NULL)
    {
        return;
    }
    windowlist->Close(doc->GetId(), true);
}


//----------------------------------------------------------------
void EditorPackage::OnZoomOut(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::WindowList* list = penv::Environment::Get()->GetWindowList();
    penv::WindowDocument* doc = list->GetSelectedNotebookPage();
    if (doc != NULL)
    {
        if (doc->GetClassId() == _T("EDITOR_SCINTILLA"))
        {
            ScintillaDocument* sdoc = (ScintillaDocument*)doc;
            sdoc->ZoomOut();
        }
    }
}

//----------------------------------------------------------------
void EditorPackage::OnZoomIn(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::WindowList* list = penv::Environment::Get()->GetWindowList();
    penv::WindowDocument* doc = list->GetSelectedNotebookPage();
    if (doc != NULL)
    {
        if (doc->GetClassId() == _T("EDITOR_SCINTILLA"))
        {
            ScintillaDocument* sdoc = (ScintillaDocument*)doc;
            sdoc->ZoomIn();
        }
    }
}




//----------------------------------------------------------------
void EditorPackage::OnProjectItemFileActivated(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::ProjectItemFile* item = (penv::ProjectItemFile*)event.GetVariant().GetVoidPtr();
    if (item == NULL) return;
    if (!item->Load()) {
        wxLogError(_T("[EditorPackage::OnProjectItemFileActivated] Error occured while loading file."));
    }
}




