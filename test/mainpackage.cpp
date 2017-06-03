/*
 * mainpackage.cpp - Implementation of the MainPackage class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */



#include "mainpackage.h"
#include "textdocument.h"
#include "htmlpane.h"
#include "ogldocument.h"
#include "aboutdialog.h"



PENV_INITALIZE_STATIC_PACKAGE(MainPackage);


//----------------------------------------------------------------
MainPackage::MainPackage()
    : penv::Package(_("Main Package"), _T("MAIN_PACKAGE"), penv::Version(0,0,0))
/**
 * \brief
 **/
{
}

//----------------------------------------------------------------
MainPackage::~MainPackage()
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
int MainPackage::OnInitPackage()
/**
 * \brief
 **/
{
    // Get Environment and CommandList objects (temporarily)
    penv::Environment* env = penv::Environment::Get();
    penv::CommandList* cmdlist = env->GetCommandList();

    // Show ProjectTreeCtrl, because at start the ProjectTreeCtrl
    //   is hidden, just toggle its visibility
    env->GetFrame()->ToogleProjectTreeCtrl();

    // Register Windows
    env->GetWindowList()->Register(new TextDocument());
    env->GetWindowList()->Register(new HtmlPane());
    env->GetWindowList()->Register(new OGLDocument());

    // Initialize Commands
    penv::Command* cmd = NULL;

    cmd = new penv::Command(_T("CMD_MAIN_CREATETEXTWINDOW"), _("Text Ctrl Document"), _("Shows a simple text ctrl in a document. For testing purposes only."));
    cmd->SetIcon(wxBitmap(_T("data\\testdoc.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnTextWindow));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_MAIN_CREATEOGLWINDOW"), _("Ogl Document"), _("Shows a ogl window in a document. For testing purposes only."));
    cmd->SetIcon(wxBitmap(_T("data\\testdoc2.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnOglWindow));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_MAIN_CREATEHTMLWINDOW"), _("Html Window Pane"), _("Shows a html window in a pane. For testing purposes only."));
    cmd->SetIcon(wxBitmap(_T("data\\html.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnHtmlWindow));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_MAIN_SHOWCOMMANDDIALOG"), _("Customize commands"), _("Opens a dialog to customize the menubar, toolbars and shortcuts."));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnShowCommandDialog));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_MAIN_QUIT"), _("Quit"), _("Quits the application immediatly."));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnQuit));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_MAIN_SHOWABOUTDIALOG"), _("About"), _("About Help Message"));
    cmd->SetIcon(wxBitmap(_T("data\\about.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnAbout));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_MAIN_TOOGLE_DEBUG_WINDOW"), _("Toogle debug window"), _("Toogles the debug window's visibility."));
    cmd->SetIcon(wxBitmap(_T("data\\debugtoogle.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnToogleDebugWindow));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_MAIN_TOOGLE_MESSAGE_WINDOW"), _("Toogle message window"), _("Toogles the message window's visibility."));
    cmd->SetIcon(wxBitmap(_T("data\\messagetoogle.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnToogleMessageWindow));
    cmdlist->Add(cmd);

    cmd = new penv::Command(_T("CMD_MAIN_TOOGLE_PROJECT_TREE"), _("Toogle project tree"), _("Toogles the project tree's visibility."));
    cmd->SetIcon(wxBitmap(_T("data\\projecttoogle.png"), wxBITMAP_TYPE_PNG));
    cmd->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnToogleProjectWindow));
    cmdlist->Add(cmd);

    penv::Command* cmd16 = new penv::Command(_T("COMMON_TOOGLE_TOOLBAR_STANDARD"), _("Toogle standard toolbar"), _("Shows the toolbar pane window for testing toolbars."));
    cmd16->SetIcon(wxBitmap(_T("data\\bug.png"), wxBITMAP_TYPE_PNG));
    cmd16->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnToogleToolBarStandard));
    cmdlist->Add(cmd16);

    penv::Command* cmd17 = new penv::Command(_T("COMMON_TOOGLE_TOOLBAR_VIEW"), _("Toogle view toolbar"), _("Shows the toolbar pane window for testing toolbars."));
    cmd17->SetIcon(wxBitmap(_T("data\\bug.png"), wxBITMAP_TYPE_PNG));
    cmd17->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnToogleToolBarView));
    cmdlist->Add(cmd17);

    penv::Command* cmd18 = new penv::Command(_T("COMMON_TOOGLE_TOOLBAR_TEST"), _("Toogle test toolbar"), _("Shows the toolbar pane window for testing toolbars."));
    cmd18->SetIcon(wxBitmap(_T("data\\bug.png"), wxBITMAP_TYPE_PNG));
    cmd18->Connect(new penv::CommandCallback<MainPackage>(this,&MainPackage::OnToogleToolBarTest));
    cmdlist->Add(cmd18);

    return (penv::penvPKG_NOERROR);
}


//----------------------------------------------------------------
int MainPackage::OnExitPackage()
/**
 * \brief
 **/
{
    // Nothing to clean up
    return (penv::penvPKG_NOERROR);
}



//----------------------------------------------------------------
void MainPackage::OnTextWindow(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::Window* win = env->GetWindowList()->CreateWindow(_T("MAIN_TEXT_WINDOC"), NULL);
    env->GetWindowList()->Add(win, true);
}



//----------------------------------------------------------------
void MainPackage::OnOglWindow(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::Window* win = env->GetWindowList()->CreateWindow(_T("MAIN_OGL_WINDOC"), NULL);
    env->GetWindowList()->Add(win, true);
}


//----------------------------------------------------------------
void MainPackage::OnHtmlWindow(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment* env = penv::Environment::Get();
    penv::Window* win = env->GetWindowList()->CreateWindow(_T("MAIN_HTML_WINPANE"), NULL);
    env->GetWindowList()->Add(win, true);
}



//----------------------------------------------------------------
void MainPackage::OnShowCommandDialog(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::CommandDialog dialog(penv::Environment::Get()->GetFrame());
    dialog.ShowModal();
}



//----------------------------------------------------------------
void MainPackage::OnQuit(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment::Get()->GetFrame()->Close();
}



//----------------------------------------------------------------
void MainPackage::OnAbout(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    AboutDialog dialog(penv::Environment::Get()->GetFrame());
    dialog.ShowModal();
}



//----------------------------------------------------------------
void MainPackage::OnToogleDebugWindow(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment::Get()->GetFrame()->ToogleDebugWindow();
}


//----------------------------------------------------------------
void MainPackage::OnToogleMessageWindow(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment::Get()->GetFrame()->ToogleMessageWindow();
}

//----------------------------------------------------------------
void MainPackage::OnToogleProjectWindow(penv::CommandEvent& event)
/**
 * \brief
 **/
{
    penv::Environment::Get()->GetFrame()->ToogleProjectTreeCtrl();
}




//----------------------------------------------------------------
void MainPackage::OnToogleToolBarStandard(penv::CommandEvent& event)
{
    penv::Environment* env = penv::Environment::Get();
    penv::ToolBar* toolbar = env->GetCommandList()->GetToolBarList()->GetToolBar(_T("TOOLBAR_STANDARD"));
    if (toolbar->IsVisible()) toolbar->Visible(false);
    else toolbar->Visible(true);
}


//----------------------------------------------------------------
void MainPackage::OnToogleToolBarView(penv::CommandEvent& event)
{
    penv::Environment* env = penv::Environment::Get();
    penv::ToolBar* toolbar = env->GetCommandList()->GetToolBarList()->GetToolBar(_T("TOOLBAR_VIEW"));
    if (toolbar->IsVisible()) toolbar->Visible(false);
    else toolbar->Visible(true);
}


//----------------------------------------------------------------
void MainPackage::OnToogleToolBarTest(penv::CommandEvent& event)
{
    penv::Environment* env = penv::Environment::Get();
    penv::ToolBar* toolbar = env->GetCommandList()->GetToolBarList()->GetToolBar(_T("TOOLBAR_TEST"));
    if (toolbar->IsVisible()) toolbar->Visible(false);
    else toolbar->Visible(true);
}




















