/*
 * penvhelper.h - Declaration of the PenvHelper class
 *
 * Author: Steffen Ott
 * Date: 10/02/2007
 *
 */



#include "penvhelper.h"
#include <cstdlib>
#include "environment.h"


namespace penv {

//----------------------------------------------------------------
PenvHelper::PenvHelper()
/**
 * \brief Constructor (private).
 **/
{
}

//----------------------------------------------------------------
PenvHelper::~PenvHelper()
/**
 * \brief Destructor (private).
 **/
{
}


//----------------------------------------------------------------
wxString PenvHelper::GenerateRandomHexValues(int bytes)
/**
 * \brief Creates a random hexnumber and returns it as a string.
 * \param bytes Number of bytes to generate (1 byte = 2 chars).
 * \return Hex string.
 **/
{
    char hexvalues[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    wxString output;
    for (int i=0; i<bytes; ++i)
    {
        int randnr = rand() % 256;
        output += hexvalues[randnr/16];
        output += hexvalues[randnr%16];
    }
    return (output);
}





//----------------------------------------------------------------
bool PenvHelper::ParseBoolean(const wxString& boolstring)
/**
 * \brief Parses a boolean value string into a boolean
 * variable.
 * \param boolstring Boolean string.
 * \return True or false.
 **/
{
    return (boolstring.Lower().Cmp(_T("true")) == 0);
}



//----------------------------------------------------------------
wxString PenvHelper::CreateBoolean(bool boolean)
/**
 * \brief Creates a string from a boolean value.
 * \return 'true' or 'false'.
 **/
{
    if (boolean) return (_T("true"));
    return (_T("false"));
}



//----------------------------------------------------------------
void PenvHelper::SplitString(const wxString& string, wxArrayString* array, wxChar separator)
/**
 * \brief Splits a string with a given separator into a string array.
 * The array must be initialized and empty. The array contains at least
 * one item after calling this method.
 * \param string String to split.
 * \param array Array with the splitted results.
 * \param separator Separator.
 **/
{
    if (unlikely(string.IsEmpty() || array == NULL)) return;
    int splitstringindex = 0;
    for (size_t i=0; i<string.Length(); ++i)
    {
        if (unlikely(string[i] == separator)) {
            array->Add(string.Mid(splitstringindex, i-splitstringindex));
            splitstringindex = i+1;
        }
    }
    array->Add(string.Mid(splitstringindex));
//    for (size_t i=0; i<array->Count(); ++i) wxLogMessage((*array)[i]);
}



//----------------------------------------------------------------
wxXmlNode* PenvHelper::CreateXmlNode(const wxString& elementname, const wxString& textcontent)
/**
 * \brief Creates an Xml Element Node with 'elementname' as element name and
 * 'textcontent' as the text content. The created node has no parent.
 * You could add properties to the node with node->AddProperty() but no
 * more ChildNodes!!
 * \param elementname Xml Element Name
 * \param textcontent Text content of the element node
 * \return wxXmlNode
 **/
{
    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, elementname);
    wxXmlNode* textnode = new wxXmlNode(node, wxXML_TEXT_NODE,
        wxEmptyString, textcontent);
    node->AddChild(textnode);
    return (node);
}


//----------------------------------------------------------------
bool PenvHelper::AddXmlChildNode(wxXmlNode* parent, wxXmlNode* node)
/**
 * \brief Adds an xml node at the end of the parents childnode list.
 * \param parent Parent node where the node will be appended as child.
 * \param node The node which should be appended.
 * \return True on success; false otherwise.
 **/
{
    if (parent == NULL || node == NULL) return (false);
    wxXmlNode* temp = parent->GetChildren();
    if (temp == NULL)
    {
        parent->AddChild(node);
        return (true);
    }
    // Suche das letzte Element in der childnode liste
    while (temp->GetNext() != NULL)
    {
        temp = temp->GetNext();
    }
    temp->SetNext(node);
    node->SetParent(parent);
    node->SetNext(NULL);
    return (true);
}






} // namespace penv


