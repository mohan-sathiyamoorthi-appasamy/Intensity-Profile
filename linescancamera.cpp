#include "LineScanCamera.h"
#include <QDebug>


LineScanCamera::LineScanCamera()
{

}
LineScanCamera::~LineScanCamera()
{
}
/*! \fn int LineScanCamera::initializeOCTCameraLibrary()
*  Initializes the Camera Library.
*  \returns nothing
*/
int LineScanCamera::initializeOCTCameraLibrary()
{
	return USB3_InitializeLibrary();
}

unsigned long LineScanCamera::getNoOfCameras()
{
	return ulNbCameras;
}

int LineScanCamera::updateOCTCameraList()
{
	return USB3_UpdateCameraList(&ulNbCameras);
}

int LineScanCamera::getOCTCameraInfo(unsigned long ulIndex)//, tCameraInfo * pCameraInfo)
{
	return USB3_GetCameraInfo(ulIndex, &CameraInfo);
}

int LineScanCamera::openOCTCamera(CAM_HANDLE * hCamera)
{
	return USB3_OpenCamera(&CameraInfo, hCamera);
}

int LineScanCamera::oCTCameraWriteRegister(CAM_HANDLE hCamera, unsigned long ulAddress, const void * pBuffer, size_t * piSize)
{
	return USB3_WriteRegister(hCamera, ulAddress, pBuffer, piSize);
}

int LineScanCamera::oCTCameraReadRegister(CAM_HANDLE hCamera, unsigned long ulAddress, void * pBuffer, size_t * piSize)
{
	return USB3_ReadRegister(hCamera, ulAddress, pBuffer, piSize);
}

int LineScanCamera::setOCTCameraImageParameters(CAM_HANDLE hCamera, size_t iImageHeight, size_t iNbOfBuffer)
{
	return USB3_SetImageParameters(hCamera, iImageHeight, iNbOfBuffer);
}

int LineScanCamera::startOCTCameraAcquisition(CAM_HANDLE hCamera)
{
	return USB3_StartAcquisition(hCamera);
}

int LineScanCamera::stopOCTCameraAcquisition(CAM_HANDLE hCamera)
{
	return USB3_StopAcquisition(hCamera);
}

int LineScanCamera::flushOCTCameraBuffers(CAM_HANDLE hCamera)
{
	return USB3_FlushBuffers(hCamera);
}

int LineScanCamera::closeOCTCamera(CAM_HANDLE hCamera)
{
	return USB3_CloseCamera(hCamera);
}

int LineScanCamera::terminateOCTCameraLibrary()
{
	return USB3_TerminateLibrary();
}