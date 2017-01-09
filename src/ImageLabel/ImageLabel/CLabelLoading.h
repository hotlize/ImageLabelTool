#ifndef CLABELLOADING_H
#define CLABELLOADING_H
#include <list>
using namespace std;
/********��ǩ����綯������Ա****/
typedef struct Category
{
	CString name;
	list<CString> strList;
}Category;




/********��ǩ�飺����������******/
typedef struct Group
{
	CString name;
	list<Category> categoryList;
	int count;
}Label;
void GetLabels(const char * xmlPath, Label** outLabels, int &groupNum);
void EraseLabel(Label* label);

#endif