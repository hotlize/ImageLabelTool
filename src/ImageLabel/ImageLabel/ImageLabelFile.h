#ifndef CIMAGELABELFILE_H
#define CIMAGELABELFILE_H
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*                       ��Ƶ��ע-ͼ���ע��¼�ļ�                      */
/************************************************************************/
#include "stdafx.h"
#include <list>
using namespace std;

CString UTF8TOUTF16(const char*source);

//��ƵƬ�α�ע����ṹ��
typedef struct Frect
{
	//float rect
	float left;
	float right;
	float top;
	float bottom;
}FRect;

FRect GetFrect(CRect drawRect, CRect winRect, CRect imgRect);
CRect GetCRect(FRect relativeRect, CRect winRect, CRect imgRect);


typedef struct CLabel
{
	FRect frect;
	CString name;
	CString domain;
	CString type;
	BOOL bTruncated;
	BOOL bDifficult;
	CString pose;		//Unspecified,Frontal,Rear,SideFaceLeft,SideFaceRight
	COLORREF color;
}CLabel;


typedef struct CImageFile
{
	CString fileName;
	list<CLabel> labelList;
	CRect size;
	int depth;
	CString shot; //Unspecified, CloseShot,   LongShot
}CImageFile;


//��ע�ļ�����
class CImageLabelFileIOController
{
//////////////////////////////////////////////////////////////////////////
private:
	static CImageLabelFileIOController* _instance;
	class Garbor{
	public:
		Garbor(){
			printf("Garbor constructed!");
		}
		~Garbor(){
			printf("Garbor destruct!");
			if (CImageLabelFileIOController::_instance)
				delete CImageLabelFileIOController::_instance;
		};
	};
	static Garbor garbor;
public :
	static CImageLabelFileIOController* GetInstance()
	{
		if (_instance == NULL)
		{
			_instance = new CImageLabelFileIOController();
		}
		return _instance;
	}
//////////////////////////////////////////////////////////////////////////
private:
	CString storePath;
	//��Ҫ�洢����Ϣ
	CString folderPath;
	
public:
	list<CImageFile> imageFileList;
	//���ļ��ĵ�ַ��Ϣ
	void SetFolderPath(CString folderPath);
	void SetStorePath(CString storePath);
	void AddImageFile(CString fileName);
	void DelImageFile(CString fileName);
	bool GetIfExistFile(CString fileName);
	bool SaveFileToVOCXML(CString folderPath);
	bool SaveFileToMINIObjectDetectionXML(CString outPath);
	bool ReadFileFromTMP();
	bool SaveFileToXML();
	bool SaveFileToXML(CString path);
	bool ReadFileFromXML(CString path);
	bool ReadFileFromXML();
	list<CImageFile>::iterator GetIteratorOfFile(CString fileName);
	void AddImageLabel(CString fileName,CRect imageSize,int depth, CString shot,CLabel image);
	void ModifyImageLabel(CString fileName, int index, CString shot,CLabel label);
	void DeleteImageLabel(CString fileName, int index);
};


#endif