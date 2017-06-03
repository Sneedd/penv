/*
 * unittestdocument.h - Declaration of the UnitTestDocument class
 *
 * Author: Steffen Ott
 * Date: 05/18/2008
 *
 */



#ifndef _TEST_UNITTESTDOCUMENT_H_
#define _TEST_UNITTESTDOCUMENT_H_

#include "../../src/penv.h"


/**
 * \brief The UnitTestDocument class is for some spezial tests
 * to some functions of the framework.
 * \todo Test more ;)
 **/
class UnitTestDocument : public penv::WindowDocument
{
    private:
        typedef void (UnitTestDocument::*UnitTestFunction)();

	public:
		UnitTestDocument();
        virtual ~UnitTestDocument();

        penv::Window* OnCreate(penv::ProjectItem* item);
        bool OnInitializeUI(wxWindow* parent);
        bool OnLayout();
        void ConnectEvents();

		void OnListBoxDoubleClick(wxCommandEvent& event);
		void OnButtonTestSelectedClick(wxCommandEvent& event);
		void OnButtonTestAllClick(wxCommandEvent& event);

		void AppendHtml(const wxString& strg);
		void AppendHtml(const wxString& call, const wxString& result);
		void AppendHtml(bool resultcorrect);
     	void ClearHtml();
		void ShowHtmlInfo();

        void DisplayTestCases();
		void SetupTestCases();
        void AddTest(const wxString& name, UnitTestFunction function);

		void TestPathClass();
		void TestArrayClass();
		void TestVersionClass();
		void TestPropertyClass();
		void TestProjectTreeItemData();


	protected:
		wxListBox* m_listbox;
		wxButton* m_buttontestselected;
		wxButton* m_buttontestall;
		wxHtmlWindow* m_htmlwin;
		wxPanel* m_panel;
		wxString m_htmltext;
        WX_DECLARE_STRING_HASH_MAP(UnitTestFunction, UnitTestHashMap);
        UnitTestHashMap* m_unittests;
};



#endif // _TEST_UNITTESTDOCUMENT_H_
