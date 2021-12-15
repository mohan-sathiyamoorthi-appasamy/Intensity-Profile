#include "octcmoscamera.h"
#include <QMessageBox>
#include "util.h"
#include <QDebug>
#include <QtGui>

bool connectedGalvo = false;
int lastScanSettingType = 0;
int lastScanSettingLengthType = 1;
octCMOSCamera* octCMOSCamera::instance ;
bool octCMOSCamera::octCmosCameraStatus;
CAM_HANDLE octCMOSCamera::hCamera;
tPlugNotify * pPlugNotify;
using namespace std;

octCMOSCamera::octCMOSCamera(QWidget *parent) : QMainWindow(parent)
{
	PRINTFUNCTIONNAME
	instance = 0;
	hCamera = NULL;
	octCmosCameraStatus = false;
}

octCMOSCamera::~octCMOSCamera()
{
	PRINTFUNCTIONNAME	
	hCamera = NULL;
	delete lineScanCamera;	
}

void octCMOSCamera::DestroyInstance() 
{
	delete instance;
	instance = 0;
}

octCMOSCamera* octCMOSCamera::getInstance()
{
	PRINTFUNCTIONNAME
	if (!instance) {
		instance = new octCMOSCamera();
		return instance;
	}
	else {
		return instance;
	}
	return instance;
}

void octCMOSCamera::initOpenOCTCamera()
{
	PRINTFUNCTIONNAME
		if (octCmosCameraStatus == false)
		{
			lineScanCamera = new LineScanCamera();
			nError = lineScanCamera->initializeOCTCameraLibrary();
			if (nError != CAM_ERR_SUCCESS);
			unsigned long noOfCameras;
			nError = lineScanCamera->updateOCTCameraList();
			if (nError != CAM_ERR_SUCCESS);
			qDebug() << "USB3_GetCameraInfo  " << nError;

			noOfCameras = lineScanCamera->getNoOfCameras();
			qDebug() << "camera(s) found  " << noOfCameras;
			for (unsigned long ulIndex = 0; ulIndex < noOfCameras; ulIndex++)
			{
				nError = lineScanCamera->getOCTCameraInfo(ulIndex);
				if (nError == CAM_ERR_SUCCESS)
					break;
			}

			nError = lineScanCamera->openOCTCamera(&hCamera);
			if (nError != CAM_ERR_SUCCESS){
				/*QMessageBox::StandardButton dlg;
				dlg = QMessageBox::question(this, tr("OCT Camera Error!"),
					QString(tr("OCT Camera Not Detected!!! \n Are Want to Continue the Software?")),
					QMessageBox::Yes | QMessageBox::No);
				if (dlg == QMessageBox::No)
				{
					octCamera->closeCamera();
					this->close();
					qApp->exit();*/
				//}
				//else
				//{			
				//}
			}
			else {
				octCmosCameraStatus = true;
			}
		}
}

void octCMOSCamera::startOCTCameraAcquisition()
{
	PRINTFUNCTIONNAME
	unsigned long ulAddress = 0x12100;
	unsigned long ulValue = 1250; //2500;

	size_t iSize = sizeof(ulValue);
	iSize = sizeof(ulValue);
	nError = lineScanCamera->oCTCameraWriteRegister(hCamera, ulAddress, &ulValue, &iSize);
	nError = lineScanCamera->oCTCameraReadRegister(hCamera, ulAddress, &ulValue, &iSize);

	ulAddress = 0x1210C;
	ulValue = 7;
	iSize = sizeof(ulValue);
	nError = lineScanCamera->oCTCameraWriteRegister(hCamera, ulAddress, &ulValue, &iSize);
	nError = lineScanCamera->oCTCameraReadRegister(hCamera, ulAddress, &ulValue, &iSize);

	ulAddress = 0x12128;
	ulValue = 2000;
	//qDebug() << "scanType.lineNumber" << scanType.lineNumber;
	iSize = sizeof(ulValue);
	nError = lineScanCamera->oCTCameraWriteRegister(hCamera, ulAddress, &ulValue, &iSize);
	nError = lineScanCamera->oCTCameraReadRegister(hCamera, ulAddress, &ulValue, &iSize);

	size_t iImageHeight = 100;
	size_t iNbOfBuffer = 100;
	nError = lineScanCamera->setOCTCameraImageParameters(hCamera, iImageHeight, iNbOfBuffer);
	nError = lineScanCamera->startOCTCameraAcquisition(hCamera);
}
void octCMOSCamera::startOCTCameraAcquisition1()
{
	QSqlQueryModel* qry = new QSqlQueryModel();
	qry->setQuery("SELECT * FROM Settings");

	line = qry->record(0).value("Line").toInt();
	frame = qry->record(0).value("Frame").toInt();
	Syncmode = qry->record(0).value("SyncMode").toInt();

	//qDebug() << "From DB line: " << line;
	//qDebug() << "From DB frame: " << frame;
	//qDebug() << "From DB sync: " << Syncmode;

	if (nError == CAM_ERR_SUCCESS)
	{
		//unsigned long hCamera =  line;//
		unsigned long linePeriodAddress = 0x12100; // 0x0000;
		unsigned long SyncAddress = 0x1210C; // 0x0000;
		unsigned long gainAddress = 0x1210C; // 0x0000;
		unsigned long LineNumberAddress = 0x12128; // 0x0000;
		//qDebug() << "ulAddress" << SyncAddress;
		unsigned long linePeriodValue = line;
		unsigned long syncValue = Syncmode;
		unsigned long LineNumberValue = frame;

		unsigned long lineperiod1, sync1, linenumber1;

		//unsigned long iSize = grain;
		size_t iSize = sizeof(linePeriodAddress);
		//nError = USB3_ReadRegister(hCamera, ulAddress, &ulValue, &iSize);

		USB3_WriteRegister(hCamera, linePeriodAddress, &linePeriodValue, &iSize);
		USB3_WriteRegister(hCamera, SyncAddress, &syncValue, &iSize);
		USB3_WriteRegister(hCamera, LineNumberAddress, &LineNumberValue, &iSize);

		USB3_ReadRegister(hCamera, linePeriodAddress, &lineperiod1, &iSize);
		USB3_ReadRegister(hCamera, SyncAddress, &sync1, &iSize);
		USB3_ReadRegister(hCamera, LineNumberAddress, &linenumber1, &iSize);
		nError = lineScanCamera->startOCTCameraAcquisition(hCamera);
		//qDebug() << "lineperiod1: " << lineperiod1;
		//qDebug() << "sync1: " << sync1;
		//qDebug() << "linenumber1: " << linenumber1;
	}

}
void octCMOSCamera::stopOCTCameraAcquisition()
{
	PRINTFUNCTIONNAME
	if (hCamera != NULL){
		nError = lineScanCamera->stopOCTCameraAcquisition(hCamera);
		if (nError != CAM_ERR_SUCCESS)
		nError = lineScanCamera->flushOCTCameraBuffers(hCamera);
		if (nError != CAM_ERR_SUCCESS);
	}
}

void octCMOSCamera::closeCamera()
{
	PRINTFUNCTIONNAME
	nError = lineScanCamera->closeOCTCamera(hCamera);
	qDebug() << "   closeOCTCamera    " << nError;
	if (nError != CAM_ERR_SUCCESS)
		qDebug() << "close camera error";
	nError = lineScanCamera->terminateOCTCameraLibrary();
	qDebug() << "terminateOCTCameraLibrary     " << nError;
	if (nError != CAM_ERR_SUCCESS)
		qDebug() << "terminate lib  error";
}

void octCMOSCamera::terminateOCTCameraLibraryIn()
{
	PRINTFUNCTIONNAME	
	nError = lineScanCamera->terminateOCTCameraLibrary();
	qDebug() << "terminate lib  error   " << nError;
	if (nError != CAM_ERR_SUCCESS)
		qDebug() << "terminate lib  error";
}