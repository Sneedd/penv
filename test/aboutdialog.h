/*
 * aboutdialog.h - Declaration of the AboutDialog class
 *
 * Author: Steffen Ott
 * Date: 05/16/2008
 *
 */

#ifndef _TEST_ABOUTDIALOG_H_
#define _TEST_ABOUTDIALOG_H_


#include "../src/penv.h"


/**
 * \brief
 **/
class AboutDialog : public wxDialog
{
	public:
		AboutDialog(wxWindow* parent, int id = wxID_ANY);


    protected:
		void OnButtonCloseClick(wxCommandEvent& event);



	protected:
		wxNotebook* m_notebook;
		wxPanel* m_panelabout;
		wxHtmlWindow* m_htmlwin;
		wxPanel* m_panelpackages;
		wxTextCtrl* m_textctrl;
		wxButton* m_buttonclose;


};


#endif // _TEST_ABOUTDIALOG_H_
