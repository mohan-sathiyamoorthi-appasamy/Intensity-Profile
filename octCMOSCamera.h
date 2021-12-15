#ifndef OCTCMOSCAMERA_H
#define OCTCMOSCAMERA_H

#include <QMainWindow>
#include <QObject>
#include "CamCmosOctUsb3.h"
#include "Global.hpp"
#include "linescancamera.h"
#include "octCMOSCamera.h"
#include <qsqltablemodel.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlQueryModel>
extern bool connectedGalvo;
extern int lastScanSettingType;
extern int lastScanSettingLengthType;
class octCMOSCamera : public QMainWindow
{
	Q_OBJECT
private:
	static octCMOSCamera* instance;
	int nError;
public:
	octCMOSCamera *octCamera;
	explicit octCMOSCamera(QWidget *parent = 0);
	~octCMOSCamera();
	static octCMOSCamera *getInstance();
	int bufferLen;
	void initOpenOCTCamera();// struct scanSettings scan);
	void startOCTCameraAcquisition();
	void startOCTCameraAcquisition1();
	void stopOCTCameraAcquisition();
	void closeCamera();
	void getOCTCameraNotification();
	LineScanCamera *lineScanCamera;
	static bool octCmosCameraStatus;
	static CAM_HANDLE hCamera;
	void terminateOCTCameraLibraryIn();
	void DestroyInstance();
	int line, Syncmode, frame, gain;
	//signals:
	//	void SendOCTCameraStatus(bool);
};

#endif // OCTCMOSCAMERA_H