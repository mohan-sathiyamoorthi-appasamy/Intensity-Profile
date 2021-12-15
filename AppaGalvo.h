#ifndef APPAGALVO_H
#define APPAGALVO_H
#include <QMainWindow>
#include <QAbstractNativeEventFilter>
#include <QThread>
#include"intensityProfile.h"
//#define scan	     0
//#define capture		 1

#include <QObject>
//#include <atlstr.h>


//#ifndef HANSGalvo
//	//#include "HM_HashuScan.h"
//	//#include "HM_HashuUDM.h"
//	#pragma comment(lib,"HM_HashuScan.lib")	
//#endif
//
//#ifndef CambridgeGalvo
//	
//#endif




class AppaGalvo : public QMainWindow
{
	Q_OBJECT

public:
	explicit AppaGalvo(QWidget *parent = 0);
	~AppaGalvo();

	void GetGalvoValue10mm();
	void SetInitialGalvoValues();
	float  GalvoStartx6mm;
	float  GalvoStarty6mm;
	float  GalvoStartz6mm;
	float  GalvoStarta6mm;
	float  GalvoEndx6mm;
	float  GalvoEndy6mm;
	float  GalvoEndz6mm;
	float  GalvoEnda6mm;

	float GalvoStartx10mm;
	float GalvoStarty10mm;
	float GalvoStartz10mm;
	float GalvoStarta10mm;
	float GalvoEndx10mm;
	float GalvoEndy10mm;
	float GalvoEndz10mm;
	float GalvoEnda10mm;
	static AppaGalvo *getInstance();
	
	static bool connectedGalvoAppa;
	int GalvoInit(MSG *msg);
	void SetLaserSource();
	void ChangeGalvoSpeed(unsigned int GalvoSpeed);
	void CreateTemporaryBin();
	void SetStartRegisterData();
	void SetEndRegisterData();
	void GetMemoryAndBinfile();
	float GetAngleLine_Xparam(float Angle, float radius);
	float GetAngleLine_Yparam(float Angle, float radius);
	void SetScanpattern(unsigned int ScanPattern);
	void SetCapturePattern(unsigned int CapturePattern);
	void SetGalvoValue6mm(float  GalvoStartx6mmIn, float  GalvoStarty6mmIn, float  GalvoEndx6mmIn, float  GalvoEndy6mmIn);
	void setGalvoScanPattern(unsigned int scanPattern, int speed, float x, float y,float x1,float y1,float x2 ,float y2 );
	void RunGalvo(unsigned int Pattern, unsigned int Runtype, int speed);
	void StartGalvo();
	void StopGalvo();
	void SetPoint2D();
	void GetGalvoValue6mm();
	structUdmPos startP1;
	structUdmPos endP1;
	void polariserLeft();
	void polariserRight();
	void setGalvoValues(float G_X1, float G_Y1, float G_X2, float G_Y2);//void setGalvoValues(int scanModeType, int HDLineScanLength, float G_X1, float G_Y1, float G_X2, float G_Y2);
	void setGalvoValuesCube(QString scanArea, int scanModeType, int HDLineScanLength, float start1x, float end1x, float start1y, float end1y, float start3y, float end3y);
	void resetGalvoValues(int scanModeType, int HDLineScanLength);
	unsigned int getInput();
	void DestroyappaGalvoWidgetInstance();
	void resetHMSetOutPut();
	void setScanLength(int HDLineScanLength);
	void setScanLengthCube(int HDLineScanLength);
	void runCubeCaptureXFast(uint Pattern);
signals:
	void galvoStat(bool);


protected:
	bool nativeEvent(const QByteArray& eventType, void* message, long* result);

private:
	static AppaGalvo* appaGalvoInstance;
	char* m_UdmBuffer;//Added
	int m_nBuffSize;//Added
	//CString strPath;//Added

	HWND m_hWnd;//Added
	int nStart;
	
	int hdLineScanLength;
	int cubeScanLength;
	public slots:
	//void recivegalvovalues(float, float, float, float);
};

/*!
*****************************************************************************************************
*
*  \brief	PatternType Declarations.
*
*****************************************************************************************************
*/

/*****************************  END OF FILE ******************************************************/
#endif // APPAGALVO_H


