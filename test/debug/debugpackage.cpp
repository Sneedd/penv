/*
 * debugpackage.cpp - Implementation of the DebugPackage class
 *
 * Author: Steffen Ott
 * Date: 05/06/2008
 *
 */


#include "debugpackage.h"
#include "eventdocument.h"
#include "toolbarpane.h"
#include "auiinfopane.h"
#include "unittestdocument.h"


PENV_INITALIZE_STATIC_PACKAGE(DebugPackage);


//----------------------------------------------------------------
DebugPackage::DebugPackage()
    : penv::Package(_("Debug Package"), _T("DEBUG_PACKAGE"), penv::Version(0,0,0))
{
    // Dependency to main package
    Add( new penv::PackageDependency(_T("MAIN_PACKAGE"), penv::Version(0,0,0)) );
}



//----------------------------------------------------------------
DebugPackage::~DebugPackage()
{
}

//----------------------------------------------------------------
int DebugPackage::OnInitPackage()
/**
 * \brief Initialize DebugPackage.
 **/
{
    // Get Environment and CommandList objects (temporarily)
    penv::Environment* env = penv::Environment::Get();
    penv::CommandList* cmdlist = env->GetCommandList();

    // Register Windows
    env->GetWindowList()->Register(new ToolBarPane());
    env->GetWindowList()->Register(new EventDocument());
    env->GetWindowList()->Register(new AuiInfoPane());
    env->GetWindowList()->Register(new UnitTestDocument());

    // Initialize Commands
    penv::Command* cmd = NULL;

    cmd = new penv::Command(_T("CMD_DEBUG_SHOW_EVENTDOCUMENT"), _("Show event document"), _("Shows the event document window for testing wxEvtHandler."));
    cmd->SetIcon(wxBitmap(_T("data\\bug.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<DebugPackage>(this,&DebugPackage::OnShowEventDocument));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_DEBUG_SHOW_TOOLBARPANE"), _("Show toolbar pane"), _("Shows the toolbar pane window for testing toolbars."));
    cmd->SetIcon(wxBitmap(_T("data\\bug.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<DebugPackage>(this,&DebugPackage::OnShowToolBarPane));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_DEBUG_SHOW_AUIINFOPANE"), _("Show aui info pane"), _(""));
    cmd->SetIcon(wxBitmap(_T("data\\bug.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<DebugPackage>(this,&DebugPackage::OnShowAuiInfoPane));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_DEBUG_SHOW_UNITTESTDOCUMENT"), _("Show unit test document"), _(""));
    cmd->SetIcon(wxBitmap(_T("data\\unittest.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<DebugPackage>(this,&DebugPackage::OnShowUnitTestDocument));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_DEBUG_PRINT_PROJECTMANAGERHIERARCHY"), _("Print project manager hierarchy"), _(""));
    cmd->Connect(new penv::CommandCallback<DebugPackage>(this,&DebugPackage::OnPrintProjectManagerHierarchy));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_DEBUG_PRINT_PROJECTTREECTRL"), _("Print project tree control"), _(""));
    cmd->Connect(new penv::CommandCallback<DebugPackage>(this,&DebugPackage::OnPrintProjectTreeCtrl));
    cmdlist->Add(cmd);


    return (penv::penvPKG_NOERROR);
}



//----------------------------------------------------------------
int DebugPackage::OnExitPackage()
/**
 * \brief Exit DebugPackage.
 **/
{
    return (penv::penvPKG_NOERROR);
}



//----------------------------------------------------------------
void DebugPackage::OnShowEventDocument(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::Window* win = env->GetWindowList()->CreateWindow(_T("TESTING_EVENT_INFORMATION_DOCUMENT"), NULL);
    env->GetWindowList()->Add(win, true);
}


//----------------------------------------------------------------
void DebugPackage::OnShowToolBarPane(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::Window* win = env->GetWindowList()->CreateWindow(_T("TESTING_TOOLBAR_INFORMATION_PANE"), NULL);
    env->GetWindowList()->Add(win, true);
}



//----------------------------------------------------------------
void DebugPackage::OnShowAuiInfoPane(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::Window* win = env->GetWindowList()->CreateWindow(_T("TESTING_AUI_INFORMATION_PANE"), NULL);
    env->GetWindowList()->Add(win, true);
}


//----------------------------------------------------------------
void DebugPackage::OnShowUnitTestDocument(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::Window* win = env->GetWindowList()->CreateWindow(_T("TESTING_UNITTEST_DOCUMENT"), NULL);
    env->GetWindowList()->Add(win, true);
}



//----------------------------------------------------------------
void DebugPackage::OnPrintProjectManagerHierarchy(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    env->GetWorkspaceList()->PrintHierarchy();
}


//----------------------------------------------------------------
void DebugPackage::OnPrintProjectTreeCtrl(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    env->GetFrame()->GetProjectTreeCtrl()->LogTreeView();
}


