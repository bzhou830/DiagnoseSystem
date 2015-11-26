/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MatImg.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: Dcm images to Mat-info images Type, iamges elements all in vector 
  Others:         
***********************************************************************************/
#include "StdAfx.h"
#include "MatImg.h"


/***************************************************************************************
Function: 构造函数
Input:    LPCTSTR lpszPathName dcm图像的全路径
Output:   NULL
Description: 调用LoadDcm完成
Return:   NULL
Others:   NULL
***************************************************************************************/
CImgSerial::CImgSerial(CString lpszPathName)
{
	LoadDcm(lpszPathName);
}


/***************************************************************************************
Function: 加载Dcm图像，供给类的外部调用接口
Input:    LPCTSTR lpszPathName dcm图像的全路径
Output:   导入是否成功
Description: 调用Dcm2MatGroup完成，并将导入的图像加入当前图像序列
Return:   BOOL
Others:   NULL
***************************************************************************************/
BOOL CImgSerial::LoadDcm(CString lpszPathName)
{
	sOneImg mg = Dcm2MatGroup(lpszPathName);	//图像进行转换
	mg.pSingleNodules = NULL;					//图像加载的时候结节信息全部清零
	m_vImgSerial.push_back(mg);					//将一张图的数据添加到序列中
	m_itr = m_vImgSerial.begin();				//指定迭代器itr指向序列的开头
	return TRUE;
}



/***************************************************************************************
Function: Dcm图像转换成容易处理的对象（Mat图像+附加信息）
Input:    LPCTSTR lpszPathName dcm图像的全路径
Output:   dcm转换后的Mat+info结构类型
Description: 使用DCMTK库函数，原DCM图像转换为8bit灰度图像会损失精度
Return:   MatGroup
Others:   NULL
***************************************************************************************/
sOneImg CImgSerial::Dcm2MatGroup(CString lpszPathName)
{
	sOneImg mg;													//申明一个MatGroup组
	DcmFileFormat fileformat;
	OFCondition oc = fileformat.loadFile((LPSTR)(LPCTSTR)lpszPathName);				//读取一个dcm图
	if(!oc.good())	                                                //判断文件是否打开成功
	{
		AfxMessageBox("打开文件失败, 应用程序将终止！");              //文件打开失败后退出
		AfxGetMainWnd()->PostMessage(WM_CLOSE, 0, 0);               //文件不对后直接终止应用程序
		return mg;
	}

	DcmDataset *mDataset = fileformat.getDataset();                 //获取数据集
	E_TransferSyntax xfer = mDataset->getOriginalXfer();            //传输语法
	DicomImage *dcmImg = new DicomImage(lpszPathName);              //创建一个dcm图像
	//获取图像高宽
	int width  = dcmImg->getWidth();
	int height = dcmImg->getHeight();
	dcmImg->setMinMaxWindow();

	const char *pStr;
	mg.info.isDisplay = TRUE;										//显示图像信息
	mDataset->findAndGetString(DCM_PatientName, pStr);				//患者姓名
	mg.info.sName = pStr;
	mDataset->findAndGetString(DCM_PatientID, pStr);				//患者ID
	mg.info.sId = pStr;
	mDataset->findAndGetString(DCM_InstitutionName, pStr);
	mg.info.sInstitutionName  = pStr;								//患者姓名
	mDataset->findAndGetString(DCM_PatientSex, pStr);
	mg.info.sSex  = pStr;											//患者性别
	mDataset->findAndGetString(DCM_ImagePositionPatient, pStr);
	mg.info.sImgPositionPatient = pStr;								//患者图像位置
	
	int num = 0;
	CString* pPosition = CImgInfoLayer::SplitString(mg.info.sImgPositionPatient,'\\', num);
	mg.info.nPositionX = CType::CString2Double(pPosition[0]);
	mg.info.nPositionY = CType::CString2Double(pPosition[1]);
	mg.info.nPositionZ = CType::CString2Double(pPosition[2]);
	
	mDataset->findAndGetString(DCM_WindowCenter, pStr);
	mg.info.sWindowCenter = pStr;									//窗位
	mDataset->findAndGetString(DCM_WindowWidth, pStr);
	mg.info.sWindowWidth = pStr;									//窗宽

	Uint8 *pixelData = (Uint8 *)(dcmImg->getOutputData(8));         //转换为8位灰度图像
	Mat tmp = Mat(width, height, CV_8UC1, (BYTE *)pixelData);		//转换成Mat的形式
	mg.pixle  = tmp.clone();										//克隆到结构中
	m_index = 1;													//有图像导入，则定位到第一张上
	delete dcmImg;
	return mg;
}


/***************************************************************************************
Function: 返回序列下一张图片，迭代器向后移动
Input:    NULL
Output:   如果迭代器所指的不是序列最后一张则返回下一张，若是最后一张则返回当前张
Description: 注意序列的end()并不是序列的最后一张，而是序列最后一张的下一个元素
             位置，是不可访问的内存区
Return:   MatGroup
Others:   NULL
***************************************************************************************/
sOneImg CImgSerial::NextMatImg()
{
	if (++m_itr != m_vImgSerial.end())   
	{
		++m_index;			//图像张数递加
		return *m_itr;
	}
	else
	{
		MessageBox(NULL,"已经是最后一张图片了！","RBDcm提示您",MB_OK);
		return *(--m_itr);
	}
}


/***************************************************************************************
Function: 返回序列上一张图片，迭代器向后移动
Input:    NULL
Output:   如果迭代器所指的不是序列第一张则返回上一张，若是第一张则返回当前张
Description: 
Return:   MatGroup
Others:   NULL
***************************************************************************************/
sOneImg CImgSerial::LastMatImg()
{
	if (m_itr != m_vImgSerial.begin())   
	{
		m_itr--;
		--m_index;			//图像张数递减
		return *m_itr;
	}
	else
	{
		MessageBox(NULL,"已经是第一张图片了！","RBDcm提示您",MB_OK);
		return *(m_itr);
	}
}



/***************************************************************************************
Function: 加载Xml文件
Input:    文件路径+文件名
Output:   
Description: 
Return:   MatGroup
Others:   NULL
***************************************************************************************/
BOOL CImgSerial::LoadXml(CString lpszPathName)
{
	const char* pFile = (LPSTR)(LPCTSTR)lpszPathName;						//XML文件路径
	m_vSingle.clear();														//加载前先清除信息
	TiXmlDocument doc(pFile);												//创建读取XML临时对象
	BOOL loadOK = doc.LoadFile();											//加载XML文件
	if (!loadOK)															//加载失败弹出提示并退出该函数
	{
		MessageBox(NULL, "xml文件读取失败！", "RBDcm提示您", MB_OK);
		return FALSE;
	}

	TiXmlElement* root = doc.RootElement();									//XML的根节点
	for (TiXmlNode* SpecialistItem = root->FirstChild("readingSession");    //对readingSession节点进行循环
		SpecialistItem; SpecialistItem = SpecialistItem->NextSibling("readingSession"))
	{   
		TiXmlNode* unblindedReadNodule = SpecialistItem->FirstChild("unblindedReadNodule");		//结节
		TiXmlNode* nonNodule = SpecialistItem->FirstChild("nonNodule");							//非结节

		while (unblindedReadNodule)								//节点unblindedReadNodule循环
		{
			TiXmlNode* roi = unblindedReadNodule->FirstChild("roi");
			while (roi)											//节点roi循环
			{
				SingleImgNodule single;
				TiXmlNode* imageZposition = roi->FirstChild("imageZposition");	 //imageZposition信息
				const char* Zposition = imageZposition->ToElement()->GetText();  
				single.ZPosition = CType::pChar2Double(Zposition); 
				TiXmlNode* edgeMap = roi->FirstChild("edgeMap"); 
				while (edgeMap)													//edgeMap节点循环
				{
					TiXmlNode* xCoord = edgeMap->FirstChild("xCoord");
					TiXmlNode* yCoord = edgeMap->FirstChild("yCoord");
					const char* szX = xCoord->ToElement()->GetText();
					const char* szY = yCoord->ToElement()->GetText();
					NodulePoint pt;
					pt.nt = ISNODULE;
					pt.x = CType::pChar2int(szX);
					pt.y = CType::pChar2int(szY);
					single.vcNodulePoint.push_back(pt);
					edgeMap = edgeMap->NextSibling("edgeMap");
				}
				m_vSingle.push_back(single);
				roi = roi->NextSibling("roi");
			}
			unblindedReadNodule = unblindedReadNodule->NextSibling("unblindedReadNodule");
		}

		while (nonNodule)																		
		{
			TiXmlNode* roi = nonNodule->FirstChild("roi");
			while (roi)
			{
				SingleImgNodule single;
				TiXmlNode* imageZposition = roi->FirstChild("imageZposition");		//获取到了新的节点
				const char* Zposition = imageZposition->ToElement()->GetText();		//zPosition的数值			
				single.ZPosition = CType::pChar2Double(Zposition);
				TiXmlNode* edgeMap = roi->FirstChild("edgeMap");
				while (edgeMap)
				{
					TiXmlNode* xCoord = edgeMap->FirstChild("xCoord");
					TiXmlNode* yCoord = edgeMap->FirstChild("yCoord");
					const char* szX = xCoord->ToElement()->GetText();
					const char* szY = yCoord->ToElement()->GetText();
					NodulePoint pt;
					pt.nt = NONENODULE;
					pt.x = CType::pChar2int(szX);
					pt.y = CType::pChar2int(szY);
					single.vcNodulePoint.push_back(pt);
					edgeMap = edgeMap->NextSibling("edgeMap");
				}
				//存放到序列中
				m_vSingle.push_back(single);
				roi = roi->NextSibling("roi");
			}
			nonNodule = nonNodule->NextSibling("nonNodule");
		}
	}

	//解析完成的信息与原数据进行关联
	list<sOneImg>::iterator itr = m_vImgSerial.begin();
	int count = 1;
	for (;itr != m_vImgSerial.end(); ++count)
	{
		double dbTmp = itr->info.nPositionZ;
		list<SingleImgNodule>::iterator itrSg = m_vSingle.begin();        //xml解析信息
 		for (; itrSg != m_vSingle.end(); ++itrSg)						   //在解析的表中查找匹配项，然后导入
		{
			double tmp = itrSg->ZPosition;
			if (fabs(tmp - dbTmp) > 0.1)
			{
				itr->pSingleNodules = NULL;		//没有相关信息时则清空指针
				++count;
				continue;
			}
			itr->pSingleNodules = &(*itrSg);   //如果写入了结节信息，直接结束本轮查询
			break;
		}
		++itr;
	}
	return TRUE;
}







