#include "afxwin.h"
#include "AppaGalvo.h"
#include <QDebug>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>

#include "HM_HashuScan.h"
#include "HM_HashuUDM.h"
#include"HM_CommExp.h"
#include"HM_VPUBuilder.h"
#include"galvo.h"
//#include"hm_udmgenerator.h"
#include"intensityProfile.h"
#include"resourceFileOCT.h"

typedef unsigned int  uint;
#define UDMGenBinFile "UDMGen.bin"
galvo *Galvo1;
unsigned int g_IOOutPut;
unsigned int g_IO;
int galvoStatus = 0;
unsigned int g_IOMccAG;
float ZGalvoStart = 0.0000;
float ZGalvoEnd = 0.0000;
float AStart = 0.0000;
float AEnd = 0.0000;
float radius = 6.0000;
float CenterX = 0;
float CenterY = 0;
/*!
*****************************************************************************************************
*
*  \brief	Marking Parameters Structure Creation
*
*****************************************************************************************************
*/

STMarkSetting Gg_Mark;//Added

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::GalvoInit()
*  \brief	Initialize the Galvo MCC Board
*  \param	Nothing
*  \returns Nothing.
*****************************************************************************************************
*/
structUdmPos start1, start2, start3;
structUdmPos end1, end2,end3;
AppaGalvo* AppaGalvo::appaGalvoInstance = 0;
bool AppaGalvo::connectedGalvoAppa = false;
IntensityProfile* a;

AppaGalvo::AppaGalvo(QWidget *parent):QMainWindow(parent)
{
	connectedGalvoAppa = false;
	qDebug() << "Galvo Init";
	qDebug() << "Return : " << HM_InitComm(m_hWnd, HM_MSG_DeviceStatusUpdate);
	SetInitialGalvoValues();
}

AppaGalvo::~AppaGalvo()
{
	appaGalvoInstance = NULL; // so GetInstance will still work.
	connectedGalvoAppa = false;
	g_IOOutPut = 0;
}

AppaGalvo* AppaGalvo::getInstance()
{
	if (!appaGalvoInstance) 
	{
		appaGalvoInstance = new AppaGalvo();
		return appaGalvoInstance;
	}
	else 
	{
		return appaGalvoInstance;
	}
}

void AppaGalvo::resetHMSetOutPut()
{
	g_IOOutPut = 0;
}

void AppaGalvo::DestroyappaGalvoWidgetInstance() {
	delete appaGalvoInstance;
	appaGalvoInstance = NULL;
}
void AppaGalvo::SetInitialGalvoValues()
{
	qDebug() << "Sscan range" << GalvoStartx10mm << "," << GalvoEndx10mm;
	GalvoStarty6mm = Galvo_Start_y_6mm;
	GalvoStartz6mm = Galvo_Start_z_6mm;
	GalvoStarta6mm = Galvo_Start_a_6mm;
	GalvoEndy6mm = Galvo_End_y_6mm;
	GalvoEndz6mm = Galvo_End_z_6mm;
	GalvoEnda6mm = Galvo_End_a_6mm;
	GalvoStarty10mm = Galvo_Start_y_10mm;
	GalvoStartz10mm = Galvo_Start_z_10mm;
	GalvoStarta10mm = Galvo_Start_a_10mm;
	GalvoEndy10mm = Galvo_End_y_10mm;
	GalvoEndz10mm = Galvo_End_z_10mm;
	GalvoEnda10mm = Galvo_End_a_10mm;
}

bool AppaGalvo::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	qDebug() << "Native Event";
	if (connectedGalvoAppa != true)
	{
		MSG *msg = static_cast<MSG*>(message);
		int devId = (int)msg->lParam;
			
		if (devId >= 0)
		{
			int galvoSta = GalvoInit(msg);	
			if (1 == galvoSta)
			{
				qDebug() << "Galvo Connected!";
				connectedGalvoAppa = true;
				emit galvoStat(connectedGalvoAppa);
			}
			else if (2 == galvoSta)
			{
				connectedGalvoAppa = false;
				emit galvoStat(connectedGalvoAppa);
			}
			else{
				
			}
		}
	}
	return false;
}

void AppaGalvo::setGalvoValues( float G_X1, float G_Y1, float G_X2, float G_Y2)
{
	PRINTFUNCTIONNAME
	float x1 = G_X1;
	float y1 = G_Y1;
	float x2 = G_X2;
	float y2 = G_Y2;
	qDebug() << "setGalvoValues" << x1 << "," << y1 << "," << x2 << "," << y2;
	SetGalvoValue6mm(x1, y1, x2, y2);
	
	//imagescan->SetGalvoValue10mm(x1, y1, x2, y2);
}
void AppaGalvo::GetGalvoValue10mm()
{
	PRINTFUNCTIONNAME
	
		start1 = { GalvoStartx10mm, GalvoStarty10mm, 0.0000, 0.0000 };
	end1 = { GalvoEndx10mm, GalvoEndy10mm, 0.0000, 0.0000 };
	start2 = { 0.00, -7.00, 0.0000, 0.0000 };
	end2 = { 0.00, 7.00, 0.0000, 0.0000 };
	/*startP3 = { -7.00, 0.00, 0.000000, 0 };
	endP3 = { 7.00, 0.00, 0.000000, 0 };*/

	
	qDebug() << "SetGalvoValue10mm" << GalvoStartx10mm << "," << GalvoStarty10mm << "," << GalvoEndx10mm << "," << GalvoEndy10mm;
	qDebug() << "SetGalvoV" << GalvoStartz10mm << "," << GalvoStarta10mm << "," << GalvoEndz10mm << "," << GalvoEnda10mm;

}

void AppaGalvo::setGalvoValuesCube(QString scanArea,int scanModeType, int HDLineScanLength, float start1x, float end1x, float start1y, float end1y, float start3y, float end3y)
{
	
}

void AppaGalvo::resetGalvoValues(int scanModeType, int HDLineScanLength)
{

}

int AppaGalvo::GalvoInit(MSG *msg)
{
	
	int devId = (int)msg->lParam;
	if (devId >= 0){
		int i = HM_InitComm(msg->hwnd, HM_MSG_DeviceStatusUpdate);
		qDebug() << "index is " << i;
		if (1 == HM_GetDeviceStatus(msg->lParam))
		{
			qDebug() << "Connected!";
			HM_ConnectTo(msg->lParam);
			HM_MarkStart();
			galvoStatus = 1;
			SetLaserSource();
		}
		else if (2 == HM_GetDeviceStatus(msg->lParam))
		{
			galvoStatus = 2;
			qDebug() << "disConnected!";
		}
		else
			galvoStatus = 3;
	}
	return galvoStatus;

}

void AppaGalvo::SetLaserSource()
{
	if (g_IOOutPut & 0x01 == 0x01)
	{
		g_IOOutPut = g_IOOutPut & 0xFE;
		HM_SetOutPut(0, g_IOOutPut);
	}
	else
	{
		g_IOOutPut = g_IOOutPut | 0x01;
		HM_SetOutPut(0, g_IOOutPut);
	}
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::ChangeGalvoSpeed(unsigned int GalvoSpeed)
*  \brief	Changing the Galvo Marking Speed.
*  \param	GalvoSpeed an unsigned int
*  \returns Nothing.
*****************************************************************************************************
*/

void AppaGalvo::ChangeGalvoSpeed(unsigned int GalvoSpeed)
{
	uint Speed = GalvoSpeed;

	if (0 == HM_GetDeviceStatus(0) || 1 == HM_GetDeviceStatus(0))		//  Check status of the device
	{
		CString str = std::to_string(Speed).c_str();
		std::wcout << str.GetString();
		CW2A nSpeed((LPCWSTR)str);
		Gg_Mark.nMarkV = strtol(nSpeed, NULL, 10);//Added
		HM_SetSingleMarkPara(&Gg_Mark, ENUM_Mark::MarkV);//Added
	}
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::CreateTemporaryBin()
*  \brief	Create TemporaryBin File For Storage of Galvo Parameters.
*  \param	Nothing
*  \returns Nothing.
*****************************************************************************************************
*/

void AppaGalvo::CreateTemporaryBin()
{
	if (0 == HM_GetDeviceStatus(0) || 1 == HM_GetDeviceStatus(0))     // Check status of the device
	{
		CString strPath;
		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
		strPath.ReleaseBuffer();
		int pos = strPath.ReverseFind('\\');
		strPath = strPath.Left(pos + 1);
		strPath = strPath + CString(UDMGenBinFile);
		std::cout << strPath << std::endl;
		CW2A ps2aPath((LPCWSTR)strPath);
	}
	else
	{
		#ifdef QT_DEBUG
		#else
		#endif
	}
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::SetStartRegisterData()
*  \brief	Set R0 and R8 Register for comparison, wait for the loop to run
*  \param	Nothing
*  \returns Nothing.
*****************************************************************************************************
*/

void AppaGalvo::SetStartRegisterData()
{
	UDM_NewFile();
	UDM_Main();
	UDM_Irmov(0x01, 0x00);
	UDM_Irmov(0x00, 0x08);
}

void AppaGalvo::SetPoint2D()
{
	nStart = UDM_Point2D(0.00000, 0.000000, 0.000000, 0);
	qDebug() << "UDM_Point2D: " << nStart;
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::SetEndRegisterData()
*  \brief	Set R0 and R8 Register for comparison, wait for the loop to run
*  \param	Nothing
*  \returns Nothing.
*****************************************************************************************************
*/

void AppaGalvo::SetEndRegisterData()
{
	UDM_Jne(0x08, 0);	//Ê¹ÓÃR0ºÍR8¼Ä´æÆ÷×÷±È½Ï£¬²»µÈÑ­»·ÔËÐÐ£¬ÏàµÈÍË³ö£»
	UDM_Jump(0, 0, 0);
	UDM_EndMain();
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::GetMemoryAndBinfile()
*  \brief	Get UDM Memory Address and Generate UDM Bin File
*  \param	Nothing
*  \returns Nothing.
*****************************************************************************************************
*/

void AppaGalvo::GetMemoryAndBinfile()
{
	CString strPath;
	UDM_GetUDMBuffer(&m_UdmBuffer, &m_nBuffSize);
	CW2A ps2aPath((LPCWSTR)strPath);//Added
	UDM_SaveToFile(ps2aPath);
	HM_DownloadStart_DDR_UDMBuff(m_UdmBuffer, m_nBuffSize, this->m_hWnd, 0);
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::GetAngleLine_Xparam(float Angle , float radius)
*  \brief	Calculate the Angular Line X co-ordinate.
*  \param	Angle an float and radius an float
*  \returns xparam.
*****************************************************************************************************
*/

float AppaGalvo::GetAngleLine_Xparam(float Angle, float radius)
{
	float picalc = 180 / (90 - (float)Angle);
	float xparam = radius*cos(3.14 / picalc);
	return xparam;
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::GetAngleLine_Yparam(float Angle , float radius)
*  \brief	Calculate the Angular Line Y co-ordinate.
*  \param	Angle an float and radius an float
*  \returns yparam.
*****************************************************************************************************
*/

float AppaGalvo::GetAngleLine_Yparam(float Angle, float radius)
{
	float picalc = 180 / (90 - (float)Angle);
	float yparam = radius*sin(3.14 / picalc);
	return yparam;
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::SetScanpattern(unsigned int ScanPattern)
*  \brief	Set Galvo Scan Pattern Parameters.
*  \param	ScanPattern an unsigned int
*  \returns Nothing.
*****************************************************************************************************
*/

void AppaGalvo::SetScanpattern(unsigned int ScanPattern)
{
}

/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::SetCapturePattern(unsigned int CapturePattern)
*  \brief	Set Galvo Capture Pattern parameters
*  \param	CapturePattern an unsigned int
*  \returns Nothing.
*****************************************************************************************************
*/

void AppaGalvo::SetCapturePattern(unsigned int CapturePattern)
{


}




/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::RunGalvo(uint Pattern,uint Runtype)
*  \brief	To Run the Galvo, Set Pattern Type and Scan Type to Run Galvo.
*  \param	Pattern an unsigned int , Runtype an unsigned int
*  \returns Nothing.
*****************************************************************************************************
*/

void AppaGalvo::RunGalvo(uint Pattern, uint Runtype, int speed)
{
	CreateTemporaryBin();
	ChangeGalvoSpeed(speed);
	SetStartRegisterData();
	SetPoint2D();
	SetScanpattern(Pattern);
	SetEndRegisterData();
	GetMemoryAndBinfile();
	StartGalvo();
}




/*!
*****************************************************************************************************
*  \fn 		AppaGalvo::StopGalvo()
*  \brief	To Stop the Galvo.
*  \param	Nothing
*  \returns Nothing.
*****************************************************************************************************
*/
void AppaGalvo::StartGalvo()
{
	
	HM_MarkStart();
}

void AppaGalvo::StopGalvo()
{
	PRINTFUNCTIONNAME
	int r0Data = 0x00;
	HM_WriteUDMReg(0x00, &r0Data);    // R0 register is set to zero,the UDM program automatically exits after all the calls are completed
	HM_MarkStop();
}

void AppaGalvo::polariserLeft()
{	
	g_IOOutPut = g_IOOutPut & 0xFB;
	HM_SetOutPut(0, g_IOOutPut);
	Sleep(10);
	g_IOOutPut = g_IOOutPut | 0x04;
	HM_SetOutPut(0, g_IOOutPut);
}

void AppaGalvo::polariserRight()
{
	g_IOOutPut = g_IOOutPut & 0xFD;
	HM_SetOutPut(0, g_IOOutPut);
	Sleep(10);
	g_IOOutPut = g_IOOutPut | 0x02;
	HM_SetOutPut(0, g_IOOutPut);
}

unsigned int AppaGalvo::getInput()
{
	HM_GetInPut(0, &g_IOMccAG);
	return g_IOMccAG;
}
/*****************************  END OF FILE ******************************************************/
void AppaGalvo::setGalvoScanPattern(unsigned int scanPattern, int speed, float x, float y, float x1, float y1, float x2, float y2)
{
	int r0Data = 0x00;
	HM_WriteUDMReg(0x00, &r0Data);
	CreateTemporaryBin();
	ChangeGalvoSpeed(speed);
	SetStartRegisterData();
	
	//SetPoint2D();
	
	structUdmPos start1, start2, start3, start4, start5, start6, start7, start8, start9, start10;
	structUdmPos end1, end2, end3, end4, end5, end6, end7, end8, end9, end10;
	float xGalvoStart = 0.0000;
	float xGalvoEnd = 0.0000;
	
	// YGalvo Parameters
	float yGalvoStart = 0.0000;
	float yGalvoEnd = 0.0000;
	if (0 == HM_GetDeviceStatus(0) || 1 == HM_GetDeviceStatus(0))
	{

		int pattern = scanPattern;
		switch (pattern)
		{
		case HDLINE:         // HDLINE	
		{
			if (hdLineScanLength == 0){
				start2.x = x;
				start2.y = 0.00;

				end2.x = y;
				end2.y = 0.00;
				UDM_Line2D(start2, end2);
				ChangeGalvoSpeed(1000);
				qDebug() << scanPattern << "x2 : " << start2.x << ", " << end2.x;
				qDebug() << scanPattern << "y2 : " << start2.y << ", " << end2.y;

			}
			else
			{
				qDebug() << "hdLineScanLength" << 1;
				start2.x = x;
				start2.y = 0.00;
				end2.x = y;
				end2.y = 0.00;
				UDM_Line2D(start2, end2);
				ChangeGalvoSpeed(1000);
				//	GetGalvoValue10mm();
				qDebug() << scanPattern << "x2 : " << start2.x << ", " << end2.x;
				qDebug() << scanPattern << "y2 : " << start2.y << ", " << end2.y;
			}

			break;
		}
		case CUBE:
		{
			
			start1.x = x1;
			start1.y = 10.00;
			start1.z = 0.00;
			start1.a = 0.00;
			end1.x = y1;
			end1.y = 10.00;
			end1.z = 0.00;
			end1.a = 0.00;
			//start2 = { x, 0.000, 0.000000, 0 };
			//end2 = { y, 0.000, 0.000000, 0 };
			start2.x = x;
			start2.y = 0.00;
			start2.z = 0.00;
			start2.a = 0.00;
			end2.x = y;
			end2.y = 0.00;
			end2.z = 0.00;
			end2.a = 0.00;
			start3.x = x2;
			start3.y = -28.00;
			start3.z = 0.00;
			start3.a = 0.00;
			end3.x = y2;
			end3.y = -28.00;
			end3.z = 0.00;
			end3.a = 0.00;
			qDebug() << scanPattern << "x2 : " << start2.x << end2.x;
			UDM_Line2D(start1, end1);
			UDM_Line2D(start2, end2);
			UDM_Line2D(start3, end3);
			float yGalvoScanStart;
			float yGalvoScanEnd;
			float yGalvoStepSize;
			uint yGalvoSteps = 0;
			yGalvoSteps = 128;   //100
			yGalvoStepSize = abs((yGalvoEnd - yGalvoStart)) / yGalvoSteps;
			for (int iYGalvoPosition = 0; iYGalvoPosition < yGalvoSteps; iYGalvoPosition++){
				yGalvoScanStart = yGalvoStart - (yGalvoStepSize * iYGalvoPosition);
				yGalvoScanEnd = yGalvoScanStart;
				//for (int iBM3 = 0; iBM3 < 3; iBM3++){//BM3
				//startP1.y = yGalvoScanStart;
				//endP1.y = yGalvoScanEnd;
				//UDM_Line2D(start1, end1);
				UDM_Line2D(start2, end2);
				//UDM_Line2D(start3, end3);
				//UDM_Line2D(startP1, endP1);
				//}//BM3
			}
		
			break;
		}
		}
	}

	//	if (scanPattern == "HDLINESCAN_10MM")
	//	{
	//		qDebug() << "Setting ScanPattern: " << scanPattern;

	//		

	//		start2.x = x;
	//		start2.y = 0.00;

	//		end2.x = y;
	//		end2.y = 0.00;

	//		
	//		//nStart = UDM_Point2D(start1.x, start1.y, 0.000000, 0);
	//		
	//		UDM_Line2D(start2, end2);
	//	

	//		ChangeGalvoSpeed(1000);
	//	//	GetGalvoValue10mm();
	//		
	//		qDebug() << scanPattern << "x2 : " << start2.x << ", " << end2.x;
	//		qDebug() << scanPattern << "y2 : " << start2.y << ", " << end2.y;
	//		
	//	}
	//	else if (scanPattern == "HDLINESCAN_6MM")
	//	{
	//		qDebug() << "Setting ScanPattern: " << scanPattern;



	//		start2.x = x;
	//		start2.y = 0.00;

	//		end2.x = y;
	//		end2.y = 0.00;


	//		//nStart = UDM_Point2D(start1.x, start1.y, 0.000000, 0);

	//		UDM_Line2D(start2, end2);


	//		ChangeGalvoSpeed(1000);
	//		//	GetGalvoValue10mm();

	//		qDebug() << scanPattern << "x2 : " << start2.x << ", " << end2.x;
	//		qDebug() << scanPattern << "y2 : " << start2.y << ", " << end2.y;
	//	
	//	}
	//	else if (scanPattern == "Glaucoma3D")
	//	{
	//		
	//	}
	//	else if (scanPattern == "Disc3D")
	//	{
	//		
	//	}

	//	else if (scanPattern == "MultiCross")
	//	{
	//	
	//	}

	//	else
	//	{

	//	}
	//}
	SetEndRegisterData();
	GetMemoryAndBinfile();
	
	StartGalvo();
	
}

void AppaGalvo::SetGalvoValue6mm(float  GalvoStartx6mmIn, float  GalvoStarty6mmIn, float  GalvoEndx6mmIn, float  GalvoEndy6mmIn)
{
	PRINTFUNCTIONNAME
//	SetInitialGalvoValues();
   
	GalvoStartx6mm = GalvoStartx6mmIn;
	GalvoStarty6mm = GalvoStarty6mmIn;
	GalvoEndx6mm = GalvoEndx6mmIn;
	GalvoEndy6mm = GalvoEndy6mmIn;
	
}
void AppaGalvo::GetGalvoValue6mm()
{
	PRINTFUNCTIONNAME
	startP1 = { GalvoStartx6mm, GalvoStarty6mm, GalvoStartz6mm, GalvoStarta6mm };
	endP1 = { GalvoEndx6mm, GalvoEndy6mm, GalvoEndz6mm, GalvoEnda6mm };
}
void AppaGalvo::setScanLength(int HDLineScanLength)
{
	hdLineScanLength = HDLineScanLength;
}
void AppaGalvo::setScanLengthCube(int HDLineScanLength)
{
	PRINTFUNCTIONNAME
		cubeScanLength = HDLineScanLength;
}
void AppaGalvo::runCubeCaptureXFast(uint Pattern)
{
	PRINTFUNCTIONNAME
		CString strPath;
	//maculaCube->GetGalvoValueForScanLength(cubeScanLength);
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	int pos = strPath.ReverseFind('\\');
	strPath = strPath.Left(pos + 1);
	strPath = strPath + CString(UDMGenBinFile);
	CW2A ps2aPath((LPCWSTR)strPath);
	UDM_NewFile();
	UDM_Main();
	UDM_Irmov(0x01, 0x00);
	UDM_Irmov(0x00, 0x08);

	int nStart = UDM_Point2D(0.00000, 0.000000, 0.000000, 0);
	float xGalvoStart, xGalvoEnd;


	float xGalvoStepSize;
	int xGalvoSteps = 128;
	xGalvoStepSize = (xGalvoEnd - xGalvoStart) / xGalvoSteps;

	// Unused galvo initialization parameters from Galvo SDK
	float Galvo_Start_a = 0.0000;
	float Galvo_End_a = 0.0000;
	float Galvo_Start_z = 0.0000;
	float Galvo_End_z = 0.0000;

	float yGalvoStart;  // 5.500;
	float yGalvoEnd;  //-6.200;

	float yGalvoStepSize;
	uint yGalvoSteps = 0;
	yGalvoSteps = 128;   //100
	yGalvoStepSize = abs((yGalvoEnd - yGalvoStart)) / yGalvoSteps;


	float yGalvoScanStart;
	float yGalvoScanEnd;

	structUdmPos startP1;
	structUdmPos endP1;

	startP1.x = xGalvoStart;
	startP1.y = yGalvoStart;
	startP1.z = Galvo_Start_z;
	startP1.a = Galvo_Start_a;
	endP1.x = xGalvoEnd;//xGalvoStart
	endP1.y = yGalvoStart; //ygalvoend// Same as the start position since we are scanning only in X
	endP1.z = Galvo_End_z;
	endP1.a = Galvo_End_a;

	for (int iYGalvoPosition = 0; iYGalvoPosition < yGalvoSteps; iYGalvoPosition++){
		yGalvoScanStart = yGalvoStart - (yGalvoStepSize * iYGalvoPosition);
		yGalvoScanEnd = yGalvoScanStart;
		qDebug() << "yGalvoStepSize :" << yGalvoStepSize << "iYGalvoPosition " << iYGalvoPosition;
		qDebug() << "yGalvoScanStart  : " << yGalvoScanStart;
		qDebug() << "yGalvoSteps : " << yGalvoSteps;
		qDebug() << "yGalvoEnd : " << yGalvoEnd << "  yGalvoStart:" << yGalvoStart;

		for (int iBM3 = 0; iBM3 < 3; iBM3++){//BM3
			startP1.y = yGalvoScanStart;
			endP1.y = yGalvoScanEnd;
			UDM_Line2D(startP1, endP1);
		}//BM3
	}
	UDM_Jne(0x08, nStart);
	UDM_EndMain();
	UDM_GetUDMBuffer(&m_UdmBuffer, &m_nBuffSize);
	UDM_SaveToFile(ps2aPath);
	if (0 == HM_GetDeviceStatus(0) || 1 == HM_GetDeviceStatus(0))
	{
#ifdef QT_DEBUG
#else
#endif		
		CString strPath;
		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
		strPath.ReleaseBuffer();
		int pos = strPath.ReverseFind('\\');
		strPath = strPath.Left(pos + 1);
		strPath = strPath + CString(UDMGenBinFile);
		CW2A ps2aPath((LPCWSTR)strPath);
		int va = HM_DownloadStart_DDR_UDMBuff(m_UdmBuffer, m_nBuffSize, this->m_hWnd, 0);
	}
	else
	{
#ifdef QT_DEBUG
#else
#endif
	}
	HM_MarkStart();
}