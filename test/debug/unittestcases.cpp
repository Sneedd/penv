/*
 * unittestcases.cpp - Implementation of the UnitTestDocument test cases
 *
 * Author: Steffen Ott
 * Date: 05/18/2008
 *
 */


#include "unittestdocument.h"


using namespace penv;

//----------------------------------------------------------------
void UnitTestDocument::SetupTestCases()
/**
 * \brief
 **/
{
    AddTest(_T("Path Class"), &UnitTestDocument::TestPathClass);
    AddTest(_T("Array Class"), &UnitTestDocument::TestArrayClass);
    AddTest(_T("Version Class"), &UnitTestDocument::TestVersionClass);
    AddTest(_T("Property and PropertyList Classes"), &UnitTestDocument::TestPropertyClass);
    AddTest(_T("ProjectTreeItemData"), &UnitTestDocument::TestProjectTreeItemData);
}




//----------------------------------------------------------------
void UnitTestDocument::TestPathClass()
{
    wxString call, result;

    // -------- Path::Combine() -----------------

    call   = _T("Path::Combine('C:\\windows\\system32\\conf', '..') = 'C:\\windows\\system32'");
    result = Path::Combine(_T("C:\\windows\\system32\\conf"), _T(".."));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\windows\\system32"));

    call   = _T("Path::Combine('C:\\windows\\system32\\conf', '..\\..\\.\\.\\..') = 'C:'");
    result = Path::Combine(_T("C:\\windows\\system32\\conf"), _T("..\\..\\.\\.\\.."));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:"));

    call   = _T("Path::Combine('C:\\windows\\system32\\conf', '..\\aaaaa\\bbbbb\\') = 'C:\\windows\\system32\\aaaaa\\bbbbb'");
    result = Path::Combine(_T("C:\\windows\\system32\\conf"), _T("..\\aaaaa\\bbbbb\\"));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\windows\\system32\\aaaaa\\bbbbb"));

    call   = _T("Path::Combine('C:\\windows\\system32\\conf', '\\aaaaa\\bbbbb\\') = 'C:\\aaaaa\\bbbbb'");
    result = Path::Combine(_T("C:\\windows\\system32\\conf"), _T("\\aaaaa\\bbbbb\\"));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\aaaaa\\bbbbb"));

    call   = _T("Path::Combine('C:\\windows\\system32\\conf\\info.txt', 'aaaaa\\bbbbb') = 'C:\\windows\\system32\\conf\\info.txt\\aaaaa\\bbbbb'");
    result = Path::Combine(_T("C:\\windows\\system32\\conf\\info.txt"), _T("aaaaa\\bbbbb"));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\windows\\system32\\conf\\info.txt\\aaaaa\\bbbbb"));

    call   = _T("Path::Combine('C:\\windows\\system32\\conf\\', 'aaaaa\\bbbbb') = 'C:\\windows\\system32\\conf\\aaaaa\\bbbbb'");
    result = Path::Combine(_T("C:\\windows\\system32\\conf\\"), _T("aaaaa\\bbbbb"));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\windows\\system32\\conf\\aaaaa\\bbbbb"));

    call   = _T("Path::Combine('C:\\windows\\system32\\conf', 'aaaaa') = 'C:\\windows\\system32\\conf\\aaaaa'");
    result = Path::Combine(_T("C:\\windows\\system32\\conf"), _T("aaaaa"));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\windows\\system32\\conf\\aaaaa"));



    // -------- Path::MakeRelative() -----------------

    call   = _T("Path::MakeRelative('C:\\windows\\system32\\conf', 'C:\\windows') = '..\\..'");
    result = Path::MakeRelative(_T("C:\\windows\\system32\\conf"), _T("C:\\windows"));
    AppendHtml(call, result);
    AppendHtml(result == _T("..\\.."));

    call   = _T("Path::MakeRelative('C:\\windows\\system32\\conf', 'C:\\windows\\system') = '..\\..\\system'");
    result = Path::MakeRelative(_T("C:\\windows\\system32\\conf"), _T("C:\\windows\\system"));
    AppendHtml(call, result);
    AppendHtml(result == _T("..\\..\\system"));

    call   = _T("Path::MakeRelative('C:\\windows\\system32\\conf', 'C:\\windows\\system\\info\\bla') = '..\\..\\system\\info\\bla'");
    result = Path::MakeRelative(_T("C:\\windows\\system32\\conf"), _T("C:\\windows\\system\\info\\bla"));
    AppendHtml(call, result);
    AppendHtml(result == _T("..\\..\\system\\info\\bla"));

    call   = _T("Path::MakeRelative('C:\\windows\\system32\\conf\\info.txt', 'C:\\windows\\system\\info\\bla\\log.log') = '..\\..\\..\\system\\info\\bla\\log.log'");
    result = Path::MakeRelative(_T("C:\\windows\\system32\\conf\\info.txt"), _T("C:\\windows\\system\\info\\bla\\log.log"));
    AppendHtml(call, result);
    AppendHtml(result == _T("..\\..\\..\\system\\info\\bla\\log.log"));



    // -------- Path::IsAbsolute() -----------------

    call   = _T("Path::IsAbsolute('C:\\windows\\system32') = 'true'");
    result = PenvHelper::CreateBoolean(Path::IsAbsolute(_T("C:\\windows\\system32")));
    AppendHtml(call, result);
    AppendHtml(result == _T("true"));

    call   = _T("Path::IsAbsolute('system32\\conf') = 'false'");
    result = PenvHelper::CreateBoolean(Path::IsAbsolute(_T("system32\\conf")));
    AppendHtml(call, result);
    AppendHtml(result == _T("false"));



    // -------- Path::RemoveLastEntry() -----------------

    call   = _T("Path::RemoveLastEntry('C:\\windows\\system32\\svchost.dll') = 'C:\\windows\\system32'");
    result = Path::RemoveLastEntry(_T("C:\\windows\\system32\\svchost.dll"));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\windows\\system32"));

    call   = _T("Path::RemoveLastEntry('windows\\system32\\svchost.dll') = 'windows\\system32'");
    result = Path::RemoveLastEntry(_T("windows\\system32\\svchost.dll"));
    AppendHtml(call, result);
    AppendHtml(result == _T("windows\\system32"));

    call   = _T("Path::RemoveLastEntry('windows') = ''");
    result = Path::RemoveLastEntry(_T("windows"));
    AppendHtml(call, result);
    AppendHtml(result == _T(""));

    call   = _T("Path::RemoveLastEntry('') = ''");
    result = Path::RemoveLastEntry(_T(""));
    AppendHtml(call, result);
    AppendHtml(result == _T(""));



    // -------- Path::GetLastEntry() -----------------

    call   = _T("Path::GetLastEntry('C:\\windows\\system32\\svchost.dll') = 'svchost.dll'");
    result = Path::GetLastEntry(_T("C:\\windows\\system32\\svchost.dll"));
    AppendHtml(call, result);
    AppendHtml(result == _T("svchost.dll"));

    call   = _T("Path::GetLastEntry('windows\\system32\\svchost.dll') = 'svchost.dll'");
    result = Path::GetLastEntry(_T("windows\\system32\\svchost.dll"));
    AppendHtml(call, result);
    AppendHtml(result == _T("svchost.dll"));

    call   = _T("Path::GetLastEntry('windows') = 'windows'");
    result = Path::GetLastEntry(_T("windows"));
    AppendHtml(call, result);
    AppendHtml(result == _T("windows"));

    call   = _T("Path::GetLastEntry('') = ''");
    result = Path::GetLastEntry(_T(""));
    AppendHtml(call, result);
    AppendHtml(result == _T(""));



    // -------- Path::Normalize() -----------------

    call   = _T("Path::Normalize('C:\\/windows\\\\system32//conf') = 'C:\\windows\\system32\\conf'");
    result = Path::Normalize(_T("C:\\/windows\\\\system32//conf"));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\windows\\system32\\conf"));

    call   = _T("Path::Normalize('C:\\/windows//system32\\\\\\config/') = 'C:\\windows\\system32\\config'");
    result = Path::Normalize(_T("C:\\/windows//system32\\\\\\config/"));
    AppendHtml(call, result);
    AppendHtml(result == _T("C:\\windows\\system32\\config"));

    call   = _T("Path::Normalize('') = ''");
    result = Path::Normalize(_T(""));
    AppendHtml(call, result);
    AppendHtml(result == _T(""));
}



//----------------------------------------------------------------
void UnitTestDocument::TestArrayClass()
{
    Array<CommandToolBarTreeData> array;
    for (int i=0; i<10; ++i)
    {
        array.Add(new CommandToolBarTreeData(wxString::Format(_T("%i"), i)));
    }
    //array.Print(true);
    array.Insert(0, new CommandToolBarTreeData(_T("Insert at 0")));
    //array.Print(true);
    array.Insert(10, new CommandToolBarTreeData(_T("Insert at 10")));
    //array.Print(true);
    array.Insert(5, new CommandToolBarTreeData(_T("Insert at 5")));
    //array.Print(true);
    AppendHtml(_T("array test"));
}



//----------------------------------------------------------------
void UnitTestDocument::TestVersionClass()
{
    wxString call, result;

    // -------- Version::operators -----------------

    call   = _T("Version(0,0,0) == Version(0,0,0) = 'true'");
    result = PenvHelper::CreateBoolean(penv::Version(0,0,0) == penv::Version(0,0,0));
    AppendHtml(call, result);
    AppendHtml(result == _T("true"));

    call   = _T("Version(0,1,0) == Version(0,2,0) = 'false'");
    result = PenvHelper::CreateBoolean(penv::Version(0,1,0) == penv::Version(0,2,0));
    AppendHtml(call, result);
    AppendHtml(result == _T("false"));

    call   = _T("Version(0,0,321) != Version(0,0,32) = 'true'");
    result = PenvHelper::CreateBoolean(penv::Version(0,0,321) != penv::Version(0,0,32));
    AppendHtml(call, result);
    AppendHtml(result == _T("true"));

    call   = _T("Version(1,0,0) &lt; Version(2,0,0) = 'true'");
    result = PenvHelper::CreateBoolean(penv::Version(1,0,0) < penv::Version(2,0,0));
    AppendHtml(call, result);
    AppendHtml(result == _T("true"));

    call   = _T("Version(5,0,8) &lt; Version(2,0,7) = 'false'");
    result = PenvHelper::CreateBoolean(penv::Version(5,0,8) < penv::Version(2,0,7));
    AppendHtml(call, result);
    AppendHtml(result == _T("false"));

    call   = _T("Version(1,1,0) &gt; Version(1,2,0) = 'false'");
    result = PenvHelper::CreateBoolean(penv::Version(1,1,0) > penv::Version(1,2,0));
    AppendHtml(call, result);
    AppendHtml(result == _T("false"));

    call   = _T("Version(1,2,3) &gt; Version(1,1,7) = 'true'");
    result = PenvHelper::CreateBoolean(penv::Version(1,2,3) > penv::Version(1,1,7));
    AppendHtml(call, result);
    AppendHtml(result == _T("true"));

    call   = _T("Version(1,1,1) &gt;= Version(1,1,0) = 'true'");
    result = PenvHelper::CreateBoolean(penv::Version(1,1,1) >= penv::Version(1,1,0));
    AppendHtml(call, result);
    AppendHtml(result == _T("true"));

    call   = _T("Version(2,1,0) &gt;= Version(3,1,0) = 'false'");
    result = PenvHelper::CreateBoolean(penv::Version(2,1,0) >= penv::Version(3,1,0));
    AppendHtml(call, result);
    AppendHtml(result == _T("false"));

    call   = _T("Version(1,1,1) &lt;= Version(1,1,0) = 'false'");
    result = PenvHelper::CreateBoolean(penv::Version(1,1,1) <= penv::Version(1,1,0));
    AppendHtml(call, result);
    AppendHtml(result == _T("false"));

    call   = _T("Version(2,1,1) &lt;= Version(4,1,0) = 'true'");
    result = PenvHelper::CreateBoolean(penv::Version(2,1,1) <= penv::Version(4,1,0));
    AppendHtml(call, result);
    AppendHtml(result == _T("true"));

    call   = _T("Version(1,1,0).GetString() = '1.1.0 unicode debug'");
    result = penv::Version(1,1,0).GetVersion();
    AppendHtml(call, result);
    AppendHtml(result == _T("1.1.0 unicode debug"));
    //--
    call   = _T("Version(1,1,0).GetVersion(_T(""%a.%b.%c %u %d"")) const   = '1.1.0 unicode debug'");
    result = penv::Version(1,1,0).GetVersion(_T("%a.%b.%c %u %d"));
    AppendHtml(call, result);
    AppendHtml(result == _T("1.1.0 unicode debug"));


}


//----------------------------------------------------------------
void UnitTestDocument::TestPropertyClass()
{
}





////----------------------------------------------------------------
//void ShowAllProperties(PropertyList* list)
//{
//    wxArrayString* array = list->GetProperties(true);
//    for (size_t i=0; i<array->Count(); ++i)
//    {
//        Property* prop = list->GetProperty((*array)[i]);
//        if (prop->GetType() == penvPT_Boolean) {
//            wxString boolstrg = PenvHelper::CreateBoolean(prop->GetBoolean());
//            wxLogMessage(_T("%s\t\tBoolean\t\t%s"),(*array)[i].c_str(), boolstrg.c_str());
//        }
//        else if (prop->GetType() == penvPT_Integer) {
//            wxString strg = wxString::Format(_T("%i"), prop->GetInteger());
//            wxLogMessage(_T("%s\t\tInteger\t\t%s"),(*array)[i].c_str(), strg.c_str());
//        }
//        else if (prop->GetType() == penvPT_Double) {
//            wxString strg = wxString::Format(_T("%f"), prop->GetDouble());
//            wxLogMessage(_T("%s\t\tDouble\t\t%s"),(*array)[i].c_str(), strg.c_str());
//        }
//        else if (prop->GetType() == penvPT_String) {
//            wxString strg = prop->GetString();
//            wxLogMessage(_T("%s\t\tString\t\t%s"),(*array)[i].c_str(), strg.c_str());
//        }
//        else if (prop->GetType() == penvPT_Properties) {
//            wxString strg = wxString::Format(_T("%p"), prop->GetPropertyList());
//            wxLogMessage(_T("%s\t\tPropList\t\t%s"),(*array)[i].c_str(), strg.c_str());
//        }
//        else if (prop->GetType() == penvPT_ArrayBoolean) {
//        }
//        else if (prop->GetType() == penvPT_ArrayInteger) {
//        }
//        else if (prop->GetType() == penvPT_ArrayDouble) {
//        }
//        else if (prop->GetType() == penvPT_ArrayString) {
//        }
//    }
//    delete array;
//}
//
//
////----------------------------------------------------------------
//void TestPanel::OnTestA3( wxCommandEvent& event)
//{
//    // Properties Test !!!!!!
//    PropertyList* list = Environment::Get()->GetPropertyList();
//    wxLogMessage(_T("=========================================================="));
//    ShowAllProperties(list);
//    wxLogMessage(_T("----------------------------------------------------------"));
//    wxString dir = list->GetString(_T("packagedirectory"));
//    wxLogMessage(_T("Reading 'packagedirectory' = %s"), dir.c_str());
//    wxString info = list->GetString(_T("additional/info"));
//    wxLogMessage(_T("Reading 'additional/info' = %s"), info.c_str());
//    wxLogMessage(_T("----------------------------------------------------------"));
//    list->Set(_T("test/test/test"), true);
//    list->Set(_T("test/test/test2"), 10);
//    list->Set(_T("test/test2/test3"), 0.3254f);
//    list->Set(_T("test/test2/test4"), _T("hallo"));
//    wxString strg = list->GetString(_T("test/test2/test4"));
//    wxLogMessage(_T("Reading 'test/test2/test4' = %s"), strg.c_str());
//    list->Set(_T("test/test2/test4"), wxString(_T("welt!")));
//    strg = list->GetString(_T("test/test2/test4"));
//    wxLogMessage(_T("Reading 'test/test2/test4' = %s"), strg.c_str());
//    wxLogMessage(_T("----------------------------------------------------------"));
//    ShowAllProperties(list);
//    wxLogMessage(_T("=========================================================="));
//}
//




//----------------------------------------------------------------
void UnitTestDocument::TestProjectTreeItemData()
{
    Workspace work;
    work.SetName(_T("TEST NAME for Workspace"));
    Workspace* workptr = &work;

    ProjectTreeItemDataObject* data = new ProjectTreeItemDataObject(workptr);
    data->LogClassInfo();

    size_t size = data->GetDataSize();
    size_t sizea = sizeof(ProjectTreeItemDataObject);
    unsigned char* buffer = new unsigned char[size];
    data->GetDataHere((void*)buffer);

    char hexvalues[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    wxString output;
    for (int i=0; i<size; ++i)
    {
        if (i%4 == 0) output += _T(" ");
        output += hexvalues[((unsigned char*)buffer)[i]/16];
        output += hexvalues[((unsigned char*)buffer)[i]%16];
    }
    wxLogMessage(output);



    ProjectTreeItemDataObject* resultdata = new ProjectTreeItemDataObject();
    resultdata->LogClassInfo();
    resultdata->SetData(data->GetDataSize(), (void*)buffer);
    resultdata->LogClassInfo();

    wxString call   = _T("Test serialization");
    wxString result = _T("true");
    AppendHtml(call, result);
    AppendHtml(data->GetWorkspace() == resultdata->GetWorkspace());

    delete [] buffer;
    delete resultdata;
    delete data;

}
