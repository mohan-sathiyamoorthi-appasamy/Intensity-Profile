#ifndef CGSNAPCAMERAWIDGET_H
#define CGSNAPCAMERAWIDGET_H
#include <QtGui>
#include <QWidget>
#include <QThread>
#include "qtimer.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "util.h"
#include "drawpattern.h"
#include "fovcamera.h"

#include <apps/Common/exampleHelper.h>
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>
#include <mvIMPACT_CPP/mvIMPACT_acquire_GenICam.h>
#include <mvDisplay/Include/mvIMPACT_acquire_display.h>


using namespace mvIMPACT::acquire;
using namespace mvIMPACT::acquire::GenICam;
using namespace mvIMPACT::acquire::display;




namespace Ui {
	class CgSnapCameraWidget;
}
//extern bool cgSnapCameraStatus;
class CgSnapCameraWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CgSnapCameraWidget(QWidget *parent = 0);
	~CgSnapCameraWidget();
	static CgSnapCameraWidget *getInstance();
	static bool cgSnapCameraStatus;
	static BYTE* funduImage;
	static bool sendReceiveBufferSignalConnected;
	int octScanType;
	QString scanType;
	bool proceedFundus;
	bool TakeSnapFlag;
	bool fundusImageSave = false;
	void initCgCamera();
	void setOctScanType(int scan);
	//added by sivapriya
	//void mouseDoubleClickEvent(QMouseEvent * e);
	void getB_C_H_S_Values();
	void TakeSnapEnable();
	void connectSendPixelBufferSignal();
	void disconnectSendPixelBufferSignal();
	void receiveScanModeFundusImageLine(QString Scan);
	void connectSendScanModeSignal();
	void disconnectSendScanModeSignal();
	void DestroyCgSnapCameraWidgetInstance();
	void anglereset();
	void receive_enfaceposiotnreset();
	void startLiveCg();
	void stopLiveCg();
	void initMatrixCamera();
	double angle_new;
	double radius_new;
	float cubex, cubey, cubew, cubeh;

private:
	Ui::CgSnapCameraWidget *ui;
	static CgSnapCameraWidget* instance;
	int imageCardSerialNumber;
	int cgStatusOk;
	tagVIDEO_STANDARD videoStandard;
	tagVIDEO_FORMAT videoFormat;
	tagVIDEO_SCAN scanMode;
	tagCRY_OSC crystalType;
	tagVIDEO_SOURCE_TYPE videoSource;
	int inputWindowStartX;
	int inputWindowStartY;
	int inputWindowWidth;
	int inputWindowHeight;
	int outputWindowStartX;
	int outputWindowStartY;
	int outputWindowWidth;
	int outputWindowHeight;
	int Brightnessvalue;
	int Contrastvalue;
	int Huevalue;
	int Saturationvalue;
	QPixmap FunsudusUSB3;
	





signals:
	void SendScanMode(QString);
	void SendScanModeAndWidth(QString, double, double);
	void sendGalvoValuesICWindow(float, float, float, float);
	void sendGalvoValuesICWindowCube(float, float, float, float, float, float);
	void SendCameraStatus(bool);
	void sendOpenglRequest(bool);
	void sendSignaltoDrawPatternUpdateCubeLine();
	void SendFundusImage(QPixmap image);

	public slots:
	//void receivePixmapCgSnap(QPixmap image1);
	void receivePixelBuffer(BYTE *pImageBuffer, bool);
	void GetScanModeCgSnap(QString Scan);
	void GetScanModeAndWidthToCgSnap(QString scan, double scanLength, double scanWidth);
	void reciveGalvoValuesCgSnap(float G_X1, float G_Y1, float G_X2, float G_Y2);
	void reciveGalvoValuesCgSnapCube(float start1x, float end1x, float start1y, float end1y, float start3y, float end3y);
	void ReceiveInternalFixationUpdate();
	//added by sivapriya
	void getbrightnessvalue(int value);
	void getcontrastvalue(int value);
	void gethuevalue(int value);
	void getsaturationvalue(int value);
	void saveB_C_H_S_Value(bool yesno, int B, int C, int H, int S);
	void getSignalfromdrawpattern();
	void receiveSignalFromFundusLive(bool param);
	void ReceiveSignalFromOpeGLUpdateCubeLine();

	void receivescanangle(double, double);
	void receive_enfaceposiotn(float, float, float, float);
	void ReceiveLive(QPixmap pixImage);
	void ReceiveBlackValue(double value);
	void ReceiveExposureValue(float val);
	void ReceiveGainValue(float val);
	void ReceiveCameraStatus(bool);
};

class liveThreadStreamCgSnap :public QThread
{
	Q_OBJECT
private:
	static liveThreadStreamCgSnap* instancelive;
public:
	liveThreadStreamCgSnap();
	static liveThreadStreamCgSnap *getInstance();
	//void setOctScanTypeLiveCG(int scan);
	void DestroyliveThreadStreamCgSnapInstance();
	void startLiveCgsnap();
	void stopLiveCgsnap();
	static bool proceedCgsnap;
	//static bool openglOpened;
	//static int counterTotalcg;
	//static int counterTotalOct;
	//WId liveWinId;
	void destroy();
	int imageWidth;
	int imageHeight;
	int imageNoOfPixels;
	bool verticalFlip;
	DWORD dwMemOffset;
	bool bInterLine;
	WORD wsum;
	BYTE *pImageBuffer;
	DWORD dwImageSize;
	BYTE *pStaticBuffer;
	int nNumber;
	BOOL bComplete;
	bool proceedFundusLiveCG;
	int octScanTypeLiveCG;
	bool liveCgsnapRunning;
signals:
	void sendImage(char*);
	void SendOffset(float, float, float, int);
	void sendPixmapCgSnap(QPixmap image1);
	void sendPixelBuffer(BYTE *pImageBuffer, bool);
	void sendPixmap2(QPixmap image1);
	void sendCamera1FPS(double);
	void SendOpenglEnable(bool);
	void sendSignalFromLive(bool);


	public slots:
	void receiveSignalFromOpenglWidgetToLiveCG(bool param);

private:
	void run();
};
class fundusLiveThreadImage :public QThread
{
	Q_OBJECT
public:
	bool flagcheck;
signals:
	void sendImage_Left(QPixmap);
	void sendCameraStatus(bool);


private:

	void run();
	CameraSettingsBlueFOX*   pCSBF_;
	public slots:



	public slots :
		//	void receivedeltavalue(int);
	signals :
			void SendOffset(float, float, float, int, float, float, float, float/*,int*/);

	// void SendOffsetforAuto_movement_first_step(float, float, float, int/*, float, float, float, float*/);
	//void SendOffsetforAuto_movement_afterxyzmotorflagset(float, float, float, int);
public:

	float x, y;

	float XOffset;
	float YOffset;
	float ZOffset;
	char* imgR;
	char* imgL;
};

#endif // CGSNAPCAMERAWIDGET_H
