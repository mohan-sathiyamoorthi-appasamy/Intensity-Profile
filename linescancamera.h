#ifndef LINESCANCAMERA_H
#define LINESCANCAMERA_H

#include "CamCmosOctUsb3.h"

class LineScanCamera
{
private:
	tCameraInfo CameraInfo;
	unsigned long  ulNbCameras;
public:
	LineScanCamera();
	~LineScanCamera();
//	LineScanCamera::getOCTCameraErrorText(int nErrorCode, char * pcErrText, size_t * piSize);
	int initializeOCTCameraLibrary();
	unsigned long getNoOfCameras();
	int updateOCTCameraList();
	int getOCTCameraInfo(unsigned long ulIndex);
	int openOCTCamera(CAM_HANDLE * hCamera);
	int oCTCameraWriteRegister(CAM_HANDLE hCamera, unsigned long ulAddress, const void * pBuffer, size_t * piSize);
	int oCTCameraReadRegister(CAM_HANDLE hCamera, unsigned long ulAddress, void * pBuffer, size_t * piSize);
	int setOCTCameraImageParameters(CAM_HANDLE hCamera, size_t iImageHeight, size_t iNbOfBuffer);	
	int startOCTCameraAcquisition(CAM_HANDLE hCamera);
	int stopOCTCameraAcquisition(CAM_HANDLE hCamera);
	int flushOCTCameraBuffers(CAM_HANDLE hCamera);
	int closeOCTCamera(CAM_HANDLE hCamera);
	int terminateOCTCameraLibrary();
	//int getCameraNotification( tPlugNotify  pPlugNotify);
};

#endif // LINESCANCAMERA_H