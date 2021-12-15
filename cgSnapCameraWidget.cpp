#include "cgsnapcamerawidget.h"
#include "ui_cgsnapcamerawidget.h"
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <qmessagebox.h>
#include <QtGui>
// Fundus camera using MAtrix vision camera
#include <apps/Common/exampleHelper.h>
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>
#include <mvIMPACT_CPP/mvIMPACT_acquire_GenICam.h>
#include <mvDisplay/Include/mvIMPACT_acquire_display.h>
#include <stdlib.h>
#include <fstream>

using namespace mvIMPACT::acquire;
using namespace mvIMPACT::acquire::GenICam;
using namespace mvIMPACT::acquire::display;
using namespace std;


Device* pDev;
char* pData;
int deviceCount = 0;
GenICam::AnalogControl* pAnC_;
GenICam::AcquisitionControl* pAcC_;

double blackval = 0;
double Exposure = { 130000.0 };
double Gain = { 18.000 };

fundusLiveThreadImage *funLiveThread;
bool CameraCaptureDone = true;
#define DATAFOLDER  "..\\data\\"
#define WidgetWidth   500
#define WidgetHeight  375
liveThreadStreamCgSnap *live;
FOVCamera *fovCamera;
CgSnapCameraWidget* CgSnapCameraWidget::instance = 0;
bool CgSnapCameraWidget::cgSnapCameraStatus = false;
bool CgSnapCameraWidget::sendReceiveBufferSignalConnected;
BYTE* CgSnapCameraWidget::funduImage = NULL;
static bool g_boTerminated = false;

HCG	m_hcg;
DrawPattern *fm;

CgSnapCameraWidget::CgSnapCameraWidget(QWidget *parent) :
QWidget(parent),
ui(new Ui::CgSnapCameraWidget)
{
	PRINTFUNCTIONNAME
		ui->setupUi(this);
	TakeSnapFlag = false;
	//added by sivapriya for adjustcamera
	Brightnessvalue = 128;
	Contrastvalue = 128;
	Huevalue = 128;
	Saturationvalue = 128;
	imageCardSerialNumber = 1;
	cgStatusOk = 0;
	
	inputWindowStartX = 0;
	inputWindowStartY = 0;
	inputWindowWidth = 768;
	inputWindowHeight = 576;
	outputWindowStartX = 0;
	outputWindowStartY = 0;
	outputWindowWidth = 400;
	outputWindowHeight = 300;
	proceedFundus = false;
	fovCamera = new FOVCamera();
}

CgSnapCameraWidget* CgSnapCameraWidget::getInstance()
{
	if (!instance) {
		instance = new CgSnapCameraWidget();
		return instance;
	}
	else {
		return instance;
	}
	return instance;
}

void CgSnapCameraWidget::initCgCamera(){
	if (!cgSnapCameraStatus)
	{
		PRINTFUNCTIONNAME
			fovCamera->setImageCardSerialNumber(imageCardSerialNumber);
		int status = fovCamera->beginCGCard();

		if (status == cgStatusOk)
			cgSnapCameraStatus = true;
		else
		{
			cgSnapCameraStatus = false;
		/*	QMessageBox::StandardButton dlg;
			dlg = QMessageBox::question(this, tr("Fundus Camera Error!"),
				QString(tr("Fundus Camera Not Detected!!! \n Do You want to continue the Software?")),
				QMessageBox::Yes | QMessageBox::No);
			if (dlg == QMessageBox::Yes){}
			else{
				exit(EXIT_FAILURE);
				this->close();
				qApp->exit();
			}*/
		}

		fovCamera->setFOVCameraVideoStandard(videoStandard);
		fovCamera->callSetCGVideoStandard();

		fovCamera->setFOVCameraVideoFormat(videoFormat);
		fovCamera->callSetCGVideoFormat();

		fovCamera->setFOVCameraScanMode(scanMode);
		fovCamera->callSetCGScanMode();

		fovCamera->setFOVCameraCryOSC(crystalType);
		fovCamera->callSelectCGCryOSC();

		fovCamera->setFOVCameraVideoSource(videoSource);
		fovCamera->callSetCGVideoSource();

		fovCamera->setFOVCameraInputWindowStartX(inputWindowStartX);
		fovCamera->setFOVCameraInputWindowStartY(inputWindowStartY);
		fovCamera->setFOVCameraInputWindowWidth(inputWindowWidth);
		fovCamera->setFOVCameraInputWindowHeight(inputWindowHeight);
		fovCamera->callSetCGInputWindow();

		fovCamera->setFOVCameraOutputWindowStartX(outputWindowStartX);
		fovCamera->setFOVCameraOutputWindowStartY(outputWindowStartY);
		fovCamera->setFOVCameraOutputWindowWidth(outputWindowWidth);
		fovCamera->setFOVCameraOutputWindowHeight(outputWindowHeight);
		fovCamera->callSetCGOutputWindow();

		live = liveThreadStreamCgSnap::getInstance();
		//live->setOctScanTypeLiveCG(GHDLINE);

		if (sendReceiveBufferSignalConnected == false)
			connectSendPixelBufferSignal();

		sendReceiveBufferSignalConnected = true;
		//live->start();


		fm = new DrawPattern(ui->label);
		QObject::connect(this, SIGNAL(SendScanMode(QString)), fm, SLOT(GetScanMode(QString)));
		QObject::connect(this, SIGNAL(SendScanModeAndWidth(QString, double, double)), fm, SLOT(GetScanModeAndWidth(QString, double, double)));
		QObject::connect(fm, SIGNAL(sendgalvovaluesCgSnap(float, float, float, float)), this, SLOT(reciveGalvoValuesCgSnap(float, float, float, float)));
		QObject::connect(fm, SIGNAL(sendgalvovaluesCgSnapCube(float, float, float, float, float, float)), this, SLOT(reciveGalvoValuesCgSnapCube(float, float, float, float, float, float)));

		QObject::connect(fm, SIGNAL(scanangle(double, double)), this, SLOT(receivescanangle(double, double)));
		connect(fm, SIGNAL(sendsignaltoFundusWidget()), this, SLOT(getSignalfromdrawpattern()));
		QObject::connect(fm, SIGNAL(labelpositonforAC_window(float, float, float, float)), this, SLOT(receive_enfaceposiotn(float, float, float, float)));
		getB_C_H_S_Values();
	}

}

void CgSnapCameraWidget::initMatrixCamera()
{
	CameraCaptureDone = true;
	funLiveThread = new fundusLiveThreadImage();
	funLiveThread->start();
	connect(funLiveThread, SIGNAL(sendImage_Left(QPixmap)), this, SLOT(ReceiveLive(QPixmap)));
	connect(funLiveThread, SIGNAL(sendCameraStatus(bool)), this, SLOT(ReceiveCameraStatus(bool)));

}
void CgSnapCameraWidget::connectSendScanModeSignal()
{
	QObject::connect(this, SIGNAL(SendScanMode(QString)), fm, SLOT(GetScanMode_AC(QString)));
	QObject::connect(this, SIGNAL(SendScanMode(QString)), fm, SLOT(GetScanMode(QString)));
}
void CgSnapCameraWidget::disconnectSendScanModeSignal()
{
	QObject::disconnect(this, SIGNAL(SendScanMode(QString)), fm, SLOT(GetScanMode(QString)));
	QObject::disconnect(this, SIGNAL(SendScanMode(QString)), fm, SLOT(GetScanMode(QString)));
}
void CgSnapCameraWidget::startLiveCg()
{
	PRINTFUNCTIONNAME	
	live->startLiveCgsnap();
	live->start();
}
void CgSnapCameraWidget::stopLiveCg()
{
	PRINTFUNCTIONNAME
	live->stopLiveCgsnap();	
}
void CgSnapCameraWidget::TakeSnapEnable(){
	PRINTFUNCTIONNAME
	TakeSnapFlag = true;
	fm->msgboxflag = false;
	CameraCaptureDone = false;
	fundusImageSave = true;
}
void CgSnapCameraWidget::connectSendPixelBufferSignal()
{
	PRINTFUNCTIONNAME
	live->startLiveCgsnap();
	sendReceiveBufferSignalConnected = true;
	QObject::connect(live, SIGNAL(sendPixelBuffer(BYTE*, bool)), this, SLOT(receivePixelBuffer(BYTE*, bool)));
	QObject::connect(live, SIGNAL(sendSignalFromLive(bool)), this, SLOT(receiveSignalFromFundusLive(bool)));
}

void CgSnapCameraWidget::disconnectSendPixelBufferSignal()
{
	PRINTFUNCTIONNAME
	sendReceiveBufferSignalConnected = false;
	QObject::disconnect(live, SIGNAL(sendPixelBuffer(BYTE*, bool)), this, SLOT(receivePixelBuffer(BYTE*, bool)));
	QObject::disconnect(live, SIGNAL(sendSignalFromLive(bool)), this, SLOT(receiveSignalFromFundusLive(bool)));
}

void CgSnapCameraWidget::setOctScanType(int scan)
{
	octScanType = scan;
}

void CgSnapCameraWidget::ReceiveInternalFixationUpdate(){
	PRINTFUNCTIONNAME
	fm->updateScan();
	fm->Update_InternalFixation();
}

void CgSnapCameraWidget::receiveSignalFromFundusLive(bool param)
{
	//if (param == true && live->proceedFundusLiveCG == true && (octScanType == GRADIAL || octScanType == GRADIAL_ANTERIOR_SEGMENT))//|| octScanType == GRADIAL_ANTERIOR_SEGMENT))
		proceedFundus = param; //&& live->proceedFundusLiveCGTemp == true
	live->proceedFundusLiveCG = false;
}

void CgSnapCameraWidget::ReceiveSignalFromOpeGLUpdateCubeLine()
{
#ifdef QT_DEBUG
	PRINTFUNCTIONNAME
#else
	//PRINTFUNCTIONNAME
#endif		
//	fm->updateCubeLineInICWidndow();
}

void CgSnapCameraWidget::receive_enfaceposiotn(float cube_x, float cube_y, float cube_W, float cube_h)
{
	cubex = cube_x;
	cubey = cube_y;
	cubew = cube_W;
	cubeh = cube_h;
}

void CgSnapCameraWidget::receive_enfaceposiotnreset()
{
	cubex = WidgetWidth - 306;;
	cubey = WidgetHeight - 260;
	cubew = 255;
	cubeh = 255;
}

void CgSnapCameraWidget::anglereset()
{
	angle_new = 9.45737e-308;
}

void CgSnapCameraWidget::receivescanangle(double angle, double radius)
{
	PRINTFUNCTIONNAME
		angle_new = angle;
	radius_new = radius;
}

void CgSnapCameraWidget::receivePixelBuffer(BYTE *pImageBuffer, bool val)
{
	QImage imgLNew(reinterpret_cast<uchar*>(pImageBuffer), 400, 300, QImage::Format_RGB888);
	QPixmap image1 = QPixmap::fromImage(imgLNew);
	

	//ui->label->setPixmap(image1);
	if (TakeSnapFlag == true){
		image1.save("picCgNew.tif");
		funduImage = pImageBuffer;
		TakeSnapFlag = false;
		fundusImageSave = true;
	}
	//ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	//ui->label->setScaledContents(true);

	if (proceedFundus == true){
		qDebug() << "send opengl " << true;
		emit sendOpenglRequest(true);
		proceedFundus = false;
	}
}

void CgSnapCameraWidget::GetScanModeCgSnap(QString Scan)
{
	PRINTFUNCTIONNAME
		fm->resetLabel();
	emit SendScanMode(Scan);
}

void CgSnapCameraWidget::GetScanModeAndWidthToCgSnap(QString scan, double scanLength, double scanWidth)
{
	PRINTFUNCTIONNAME
		//fm->resetLabel();

	emit SendScanModeAndWidth(scan, scanLength, scanWidth);
}

void CgSnapCameraWidget::receiveScanModeFundusImageLine(QString Scan)
{
	PRINTFUNCTIONNAME
		//fm->setScanModeForACWindowAndDrawPattern(Scan);
}

void CgSnapCameraWidget::reciveGalvoValuesCgSnap(float G_X1, float G_Y1, float G_X2, float G_Y2)
{
	PRINTFUNCTIONNAME

		qDebug() << "galvo values X1 : " << G_X1 << "  x2  : " << G_Y1 << "  X3 : " << G_X2 << "  X4 :" << G_Y2;
	emit sendGalvoValuesICWindow(G_X1, G_Y1, G_X2, G_Y2);
}

void CgSnapCameraWidget::reciveGalvoValuesCgSnapCube(float start1x, float end1x, float start1y, float end1y, float start3y, float end3y)
{
	PRINTFUNCTIONNAME
		emit sendGalvoValuesICWindowCube(start1x, end1x, start1y, end1y, start3y, end3y);
}

CgSnapCameraWidget::~CgSnapCameraWidget()
{
	PRINTFUNCTIONNAME
	//	live->quit();
	delete ui;
}

void CgSnapCameraWidget::DestroyCgSnapCameraWidgetInstance()
{
	delete instance;
	instance = 0;
}

liveThreadStreamCgSnap* liveThreadStreamCgSnap::instancelive = 0;
bool  liveThreadStreamCgSnap::proceedCgsnap = true;
//bool liveThreadStreamCgSnap::openglOpened = false;
//int liveThreadStreamCgSnap::counterTotalcg = 0;
//int liveThreadStreamCgSnap::counterTotalOct = 0;
liveThreadStreamCgSnap* liveThreadStreamCgSnap::getInstance()
{
	if (!instancelive) {
		instancelive = new liveThreadStreamCgSnap();
		return instancelive;
	}
	else {
		return instancelive;
	}
	return instancelive;
}

void liveThreadStreamCgSnap::DestroyliveThreadStreamCgSnapInstance() {
	delete instancelive;
	instancelive = 0;
}

liveThreadStreamCgSnap::liveThreadStreamCgSnap()
{
	imageWidth = 400;
	imageHeight = 300;
	imageNoOfPixels = 24;
	verticalFlip = false;
	dwMemOffset = 0;
	bInterLine = TRUE;
	wsum = 2;
	pImageBuffer = NULL;

	dwImageSize = 0;
	pStaticBuffer = NULL;
	nNumber = -1;
	bComplete = FALSE;
	proceedFundusLiveCG = false;
}
void liveThreadStreamCgSnap::startLiveCgsnap()
{
	PRINTFUNCTIONNAME
	liveCgsnapRunning = true;
}
void liveThreadStreamCgSnap::stopLiveCgsnap()
{
	PRINTFUNCTIONNAME
		liveCgsnapRunning = false;
}
//void liveThreadStreamCgSnap::setOctScanTypeLiveCG(int scan)
//{
//	octScanTypeLiveCG = scan;
//}

void liveThreadStreamCgSnap::receiveSignalFromOpenglWidgetToLiveCG(bool param)
{
	proceedFundusLiveCG = param;// true;
	qDebug() << "receiveSignalFromOpenglWidgetToLiveCG  : " << param;
}

void liveThreadStreamCgSnap::run()
{
	PRINTFUNCTIONNAME
		pImageBuffer = new BYTE[imageWidth * imageHeight * 3];
	if (pImageBuffer)
	{
		dwImageSize = imageWidth * imageHeight * 3;
		fovCamera->setFOVCameraDwMemOffset(dwMemOffset);
		fovCamera->setFOVCameraInterline(bInterLine);
		fovCamera->setFOVCameraNoOfImages(wsum);
		fovCamera->StartCGSnap();
		for (;;)
		{
			int nStatus = fovCamera->getCGSnappingNumber();
			if ((nStatus == 2) && (!bComplete)) {
				nNumber = 0;
				bComplete = TRUE;
			}
			if ((nStatus == 0) && bComplete) {
				nNumber = 1;
				bComplete = FALSE;
			}
			if (nNumber > -1)
			{
				fovCamera->setFOVCameraStartOffset(dwImageSize * nNumber);
				fovCamera->setFOVCameraMemoryLockLength(dwImageSize);
				fovCamera->setFOVCameraDwStartOffset((VOID **)&pStaticBuffer);

				fovCamera->staticMemLock();
				if (fovCamera->getSuccessStatus() == true)
				{
					fovCamera->setFOVCameraDestinationBuffer(pImageBuffer);
					fovCamera->setFOVCameraSourceBuffer(pStaticBuffer);
					fovCamera->setFOVCameraImageWidth(imageWidth);
					fovCamera->setFOVCameraImageHeight(imageHeight);
					fovCamera->setFOVCameraBitCount(imageNoOfPixels);
					fovCamera->setFOVCameraImageFlip(verticalFlip);
					fovCamera->dataTransform();
					fovCamera->staticMemUnlock();

					if (proceedFundusLiveCG == true){
						emit sendSignalFromLive(true);
					}
				//	qDebug() << "pImageBuffer : " << pImageBuffer;
					emit sendPixelBuffer(pImageBuffer, true);
					nNumber = -1;
				}
			}
		}
		fovCamera->stopCGSnap();
	}
	if (pImageBuffer) {
		delete[]pImageBuffer;
	}
}

//this is for fundus camera brightness, contrast, hue,saturatiion added by sivapriya
void CgSnapCameraWidget::getB_C_H_S_Values()
{
	QSqlQueryModel *modl = new QSqlQueryModel();
	modl->setQuery("select fundus_brightnessvalue,fundus_contrastvalue,fundus_huevalue,fundus_saturationvalue from applicationsettings limit 1");
	Brightnessvalue = modl->record(0).value("fundus_brightnessvalue").toInt();
	Contrastvalue = modl->record(0).value("fundus_contrastvalue").toInt();
	Huevalue = modl->record(0).value("fundus_huevalue").toInt();
	Saturationvalue = modl->record(0).value("fundus_saturationvalue").toInt();

	CGAdjustVideo(m_hcg, BRIGHTNESS, Brightnessvalue);
	CGAdjustVideo(m_hcg, CONTRAST, Contrastvalue);
	CGAdjustVideo(m_hcg, HUE, Huevalue);
	CGAdjustVideo(m_hcg, SATURATION, Saturationvalue);
}

void CgSnapCameraWidget::getSignalfromdrawpattern()
{
	/*brightnesscontrastfunduscamera *bchs = new brightnesscontrastfunduscamera();
	bchs->show();
	connect(bchs, SIGNAL(sendbrightnessvalue(int)), this, SLOT(getbrightnessvalue(int)));
	connect(bchs, SIGNAL(sendcontrastvalue(int)), this, SLOT(getcontrastvalue(int)));
	connect(bchs, SIGNAL(sendhuevalue(int)), this, SLOT(gethuevalue(int)));
	connect(bchs, SIGNAL(sendsaturationvalue(int)), this, SLOT(getsaturationvalue(int)));

	connect(bchs, SIGNAL(sendapplyBCHSYesNo(bool, int, int, int, int)), this, SLOT(saveB_C_H_S_Value(bool, int, int, int, int)));*/
}

void CgSnapCameraWidget::getbrightnessvalue(int value)
{
	fovCamera->getbrightnessvalue_(value);
}

void CgSnapCameraWidget::getcontrastvalue(int value)
{
	fovCamera->getcontrastvalue_(value);
}

void CgSnapCameraWidget::gethuevalue(int value)
{
	fovCamera->gethuevalue_(value);
}

void CgSnapCameraWidget::getsaturationvalue(int value)
{
	fovCamera->getsaturationvalue_(value);
}

void CgSnapCameraWidget::saveB_C_H_S_Value(bool yesno, int B, int C, int H, int S)
{
	if (yesno == true)
	{
		QSqlQueryModel *modl = new QSqlQueryModel();
		modl->setQuery("update applicationsettings set fundus_brightnessvalue='" + QString::number(B) + "',fundus_contrastvalue='" + QString::number(C) + "',fundus_huevalue='" + QString::number(H) + "',fundus_saturationvalue='" + QString::number(S) + "'");
	}
	else if (yesno == false)
	{
		fovCamera->saveB_C_H_S_Value(B, C, H, S);
	}
}

void CgSnapCameraWidget::ReceiveLive(QPixmap pixImage)
{
	
	
	FunsudusUSB3 = pixImage;
	
	QPixmap Maskimage(FunsudusUSB3.size());
	QPainter painter(&Maskimage);

	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	QPainterPath path = QPainterPath();
	// path.addRoundedRect(20, 0, 694, 544, 350, 350);
	path.addEllipse(QPoint(FunsudusUSB3.width() / 2, FunsudusUSB3.height() / 2), 325, 325);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, FunsudusUSB3);
	Maskimage.scaled(QSize(500, 378));
	ui->label->setPixmap(Maskimage);
	ui->label->setScaledContents(true);
	ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	//qDebug() << "MaskImageSize  " << fundusImageSave;
	//qDebug() << TakeSnapFlag;
	if (fundusImageSave)
	{
		pixImage.save("fundusImageusb.tif");
		//qDebug() << "MaskImageSize  " << Maskimage;
		emit SendFundusImage(FunsudusUSB3);
	
	}
	
}
void CgSnapCameraWidget::ReceiveCameraStatus(bool cameraStatus)
{
	if (!cameraStatus)
	{
		QMessageBox::StandardButton dlg;
		dlg = QMessageBox::information(this, tr("Fundus Camera Error!"),
			QString(tr("Fundus Camera Not Detected!!! \n Do You want to continue the Software?")),
			QMessageBox::Ok);
		if (dlg == QMessageBox::Ok){
			exit(EXIT_FAILURE);
			this->close();
			qApp->exit();
		}
		
	}
	
}
void CgSnapCameraWidget::ReceiveBlackValue(double value)
{
	blackval = value;
}
void CgSnapCameraWidget::ReceiveExposureValue(float val)
{
	qDebug() << "Exposure val"<<val;
	Exposure =(double) val;
}
void CgSnapCameraWidget::ReceiveGainValue(float val)
{

	Gain =(double) val;
	qDebug() << "gain val : " << Gain;
}
void fundusLiveThreadImage::run()
{
	DeviceManager devMgr;
	Device* pDev = devMgr.getDeviceByFamily("mvBlueFOX3");

	if (pDev == 0)
	{
		cout << "There is currently no mvBlueFOX3 device connected," << endl;
		cout << "or the mvBlueFOX3 drivers have not been" << endl;
		cout << "correctly installed." << endl;
		cout << "Skipping mvBlueFOX3 single image capture..." << endl;
		emit sendCameraStatus(false);

		  //return 1;
	}
	else
	{
		cout << "Using Device: " << pDev->product.readS() << endl;
		cout << "Serial: " << pDev->serial.readS() << endl << endl;
		emit sendCameraStatus(true);
	}

	

	// Note the different InterfaceLayout selection
	pDev->interfaceLayout.write(dilGenICam);
	pDev->open();
	
	
	double dExposureTime = { 130000.0 };
	pAcC_ = new AcquisitionControl(pDev);
	if (pAcC_->exposureTime.isValid() && pAcC_->exposureTime.isWriteable())
	{
		cout << "Currently the exposure time is set to " << pAcC_->exposureTime.read() << " us. Changing to " << dExposureTime << " us" << endl;
		pAcC_->exposureTime.write(dExposureTime);
	}
	

	// ------------------------------GenICam part END--------------------------
	int requestNr = INVALID_ID;
	const int iMaxWaitTime_ms = 2000;
	int lastRequestNr_leftcamera = INVALID_ID;

	int imageWidth = 0, imageHeight = 0;
	
	pAnC_ = new AnalogControl(pDev);
	if (pAnC_->gain.isValid() && pAnC_->gain.isWriteable())
	{
		pAnC_->gain.write(18.000);
	}
	

	FunctionInterface fi(pDev);
	fi.imageRequestSingle();

	ofstream binFile("Fundusimage.txt", ios::out | ios::binary);
	binFile.open("Fundusimage.txt");
	while (CameraCaptureDone)
	{
		
		pAnC_->blackLevel.write(blackval);
		if (pAnC_->gain.isValid() && pAnC_->gain.isWriteable())
		{
			pAnC_->gain.write(Gain);
		}
		if (pAcC_->exposureTime.isValid() && pAcC_->exposureTime.isWriteable())
		{
			pAcC_->exposureTime.write(Exposure);
		}
		requestNr = fi.imageRequestWaitFor(iMaxWaitTime_ms);
		if (fi.isRequestNrValid(requestNr))
		{
			const Request* pRequest = fi.getRequest(requestNr);
			pData =(char*) pRequest->imageData.read();

			imageWidth = pRequest->imageWidth.read();
			imageHeight = pRequest->imageHeight.read();

			

			
			if (binFile)
			{
				binFile.write((char*)&pData, sizeof(pData));
				
			}

			if (fi.isRequestNrValid(lastRequestNr_leftcamera))
			{
				//this image has been displayed thus the buffer is no longer needed...
				fi.imageRequestUnlock(lastRequestNr_leftcamera);
			}
			lastRequestNr_leftcamera = requestNr;
			//send a new image request into the capture queue
			fi.imageRequestSingle();
		}
		else
		{
			cout << "imageRequestWaitFor failed (" << requestNr << ", " << ImpactAcquireException::getErrorCodeAsString(requestNr) << ", device " << pDev->serial.read() << ")"
				<< ", timeout value too small?" << endl;
		}
	
		QImage imgL_nn = QImage((unsigned char*)(pData), imageWidth, imageHeight, QImage::Format_Grayscale8);
		QPixmap image_left_raw = QPixmap::fromImage(imgL_nn);
		sendImage_Left(image_left_raw);
	}
	binFile.close();
	if (fi.isRequestNrValid(requestNr))
	{
		fi.imageRequestUnlock(requestNr);
	}
	//clear the request queue
	fi.imageRequestReset(0, 0);
}
//*********************
