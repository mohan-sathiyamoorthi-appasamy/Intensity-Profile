#include "afxwin.h"
#include "fovcamera.h"
#include <QPixmap>
#include <QDebug>
#include <SDKDDKVer.h>
#include"util.h"

/**
* Constructs the fovcamera.
*/

FOVCamera::FOVCamera()
{
	handle = NULL;
}

/*! \fn FOVCamera::setImageCardSerialNumber(int imageCardSerialNumber)
*  Sets the image card serial number.
*  \param imageCardSerialNumber an int.
*  \returns nothing.
*/

void FOVCamera::setImageCardSerialNumber(int imageCardSerialNumber)
{
	cgIimageCardSerialNumber = imageCardSerialNumber;
}

/*! \fn FOVCamera::beginCGCard()
*  Starts specifying the image card operation. 
*  \returns status an int.
*/
int FOVCamera::beginCGCard()
{
	int status = BeginCGCard(cgIimageCardSerialNumber, &cgHandle);
	return status;	
}

/*! \fn FOVCamera::setCGVideoStandard()
*  Sets the video card video standard.
*  \param videoStandard an tagVIDEO_STANDARD.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraVideoStandard(tagVIDEO_STANDARD videoStandard)
{
	cgVideoStandard = videoStandard;
}

/*! \fn FOVCamera::setCGVideoStandard()
*  Calls the camera method to set the video signal format.
*  \returns nothing.
*/
void FOVCamera::callSetCGVideoStandard()
{	
	CGSetVideoStandard(cgHandle, cgVideoStandard);
}

/*! \fn FOVCamera::setFOVCameraVideoFormat(tagVIDEO_FORMAT videoFormat)
*  Sets the image data format output by the image card.
*  \param videoFormat an tagVIDEO_FORMAT.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraVideoFormat(tagVIDEO_FORMAT videoFormat)
{
	cgVideoFormat = videoFormat;
}

/*! \fn FOVCamera::setCGVideoFormat()
*  Calls the camera method to set the image data format output by the image card.
*  \returns nothing.
*/
void FOVCamera::callSetCGVideoFormat()
{
	
	CGSetVideoFormat(cgHandle, cgVideoFormat);
}

/*! \fn FOVCamera::setFOVCameraScanMode(tagVIDEO_SCAN scanMode)
*  Sets the scan mode.
*  \param scanMode an tagVIDEO_SCAN.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraScanMode(tagVIDEO_SCAN scanMode)
{
	cgScanMode = scanMode;
}

/*! \fn FOVCamera::callSetCGScanMode()
*  Calls the camera method to set the scan mode.
*  \returns nothing.
*/
void FOVCamera::callSetCGScanMode()
{
	CGSetScanMode(cgHandle, cgScanMode);
}

/*! \fn FOVCamera::setFOVCameraCryOSC(tagCRY_OSC crystalType)
*  Selects the crystal type of the image card configuration.
*  \param crystalType an tagCRY_OSC.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraCryOSC(tagCRY_OSC crystalType)
{
	cgCrystalType= crystalType;
}

/*! \fn FOVCamera::callSelectCGCryOSC()
*  Calls the camera to select the crystal type of the image card configuration.
*  \returns nothing.
*/
void FOVCamera::callSelectCGCryOSC()
{
	CGSelectCryOSC(cgHandle, cgCrystalType);
}

/*! \fn FOVCamera::setFOVCameraVideoSource(tagVIDEO_SOURCE_TYPE videoSource)
*  Sets the video source path of the image card.
*  \param videoSource an tagVIDEO_SOURCE_TYPE.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraVideoSource(tagVIDEO_SOURCE_TYPE videoSource)
{
	VIDEO_SOURCE source;
	source.type =  videoSource;//COMPOSITE_VIDEO;
	source.nIndex = 0;
	cgVideoSource=source;
}

/*! \fn FOVCamera::setCGVideoSource()
*  Calls the camera method to set the video source path of the image card.
*  \returns nothing.
*/
void FOVCamera::callSetCGVideoSource()
{	
	CGSetVideoSource(cgHandle, cgVideoSource);
}

/*! \fn FOVCamera::setFOVCameraInputWindowStartX(int startX)
*  Sets the video capture input window startX.
*  \param startX an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraInputWindowStartX(int startX)
{
	cgInputWindowStartX = startX;
}

/*! \fn FOVCamera::setFOVCameraInputWindowStartY(int startY)
*  Sets the video capture input window startY.
*  \param startY an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraInputWindowStartY(int startY)
{
	cgInputWindowStartY = startY;
}

/*! \fn FOVCamera::setFOVCameraInputWindowWidth(int width)
*  Sets the video capture input window width.
*  \param width an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraInputWindowWidth(int width)
{
	cgInputWindowWidth = width;
}

/*! \fn FOVCamera::setFOVCameraInputWindowHeight(int height)
*  Sets the video capture input window height.
*  \param height an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraInputWindowHeight(int height)
{
	cgInputWindowHeight = height;
}

/*! \fn FOVCamera::callSetCGInputWindow()
*  Calls the camera method to set the video capture input window.
*  \returns nothing.
*/
void FOVCamera::callSetCGInputWindow()
{
	CGSetInputWindow(cgHandle, cgInputWindowStartX, cgInputWindowStartY, cgInputWindowWidth, cgInputWindowHeight);
}

/*! \fn FOVCamera::setFOVCameraOutputWindowStartX(int startX)
*  Sets the video capture output window startX.
*  \param startX an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraOutputWindowStartX(int startX)
{
	cgOutputWindowStartX = startX;
}

/*! \fn FOVCamera::setFOVCameraOutputWindowStartY(int startY)
*  Sets the video capture output window startY.
*  \param startY an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraOutputWindowStartY(int startY)
{
	cgOutputWindowStartY = startY;
}

/*! \fn FOVCamera::setFOVCameraOutputWindowWidth(int width)
*  Sets the video capture output window width.
*  \param width an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraOutputWindowWidth(int width)
{
	cgOutputWindowWidth = width;
}

/*! \fn FOVCamera::setFOVCameraOutputWindowHeight(int height)
*  Sets the video capture output window height.
*  \param height an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraOutputWindowHeight(int height)
{
	cgOutputWindowHeight = height;
}

/*! \fn FOVCamera::setCGInputWindow()
*  Calls the camera method to set the image display output(Output) window.
*  \returns nothing.
*/
void FOVCamera::callSetCGOutputWindow()
{
	CGSetOutputWindow(cgHandle, cgOutputWindowStartX, cgOutputWindowStartY, cgOutputWindowWidth, cgOutputWindowHeight);
}

/*! \fn FOVCamera::setFOVCameraDwMemOffset(DWORD dwMemOffset)
*  Sets the memory start address..
*  \param dwMemOffset a DWORD.Memory start address.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraDwMemOffset(DWORD dwMemOffset)
{
	cgMemOffset = dwMemOffset;
}

/*! \fn FOVCamera::setFOVCameraInterline(BOOL bInterline)
*  Sets to store image data when the video signal scanning mode is frame mode.
*  \param bInterline a bool.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraInterline(BOOL bInterline)
{
	cgInterline = bInterline;
}

/*! \fn FOVCamera::setFOVCameraNoOfImages(WORD wSum)
*  Sets the number of images captured in a loop.
*  \param wSum a WORD.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraNoOfImages(WORD wSum)
{
	cgSum = wSum;
}

/*! \fn FOVCamera::StartCGSnap(DWORD dwMemOffset, BOOL bInterline, WORD wSum)
*  Calls the camera method to start the image card to collect several frames / field image data to the memory function.
*  \returns nothing.
*/
void FOVCamera::StartCGSnap()
{
	CGStartSnap(cgHandle, cgMemOffset, cgInterline, cgSum);
}

/*! \fn FOVCamera::getCGSnappingNumber()
*  Gets the status of the currently acquired image.
*  \returns acquisitionStatus an int.
*/
int FOVCamera::getCGSnappingNumber()
{
	CGGetSnappingNumber(cgHandle, &acquisitionStatus);
	return acquisitionStatus;
}

/*! \fn FOVCamera::setFOVCameraStartOffset(DWORD dwStartOffset)
*  Sets the memory start address.
*  \param dwStartOffset an DWORD.Memory start address.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraStartOffset(DWORD dwStartOffset)
{
	cgStartOffset = dwStartOffset;
}

/*! \fn FOVCamera::setFOVCameraMemoryLockLength(DWORD dwLength)
*  Sets the length of the memory lock.
*  \param dwLength an DWORD.The length of the memory lock, in bytes.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraMemoryLockLength(DWORD dwLength)
{
	cgMemoryLockLength = dwLength;
}

/*! \fn FOVCamera::setFOVCameraDwStartOffset(PVOID *ppLineAddr)
*  Sets the pointer to the locked memory.
*  \param ppLineAddr an PVOID.Pointer to the locked memory.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraDwStartOffset(PVOID *ppLineAddr)
{
	cgPointerMemoryLock = ppLineAddr;
}

/*! \fn FOVCamera::staticMemLock()
*  Calls the camera method to lock the static memory of the specified position and size.
*  \returns nothing.
*/
void FOVCamera::staticMemLock()
{
	cgStatus = CGStaticMemLock(cgStartOffset, cgMemoryLockLength, &handle, cgPointerMemoryLock);
}

/*! \fn FOVCamera::setFOVCameraDestinationBuffer(BYTE *pDestBuf)
*  Sets the target buffer.
*  \param pDestBuf an BYTE pointer.Point to the target buffer.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraDestinationBuffer(BYTE *pDestBuf)
{
	cgDestinationBuffer = pDestBuf;
}

/*! \fn FOVCamera::setFOVCameraSourceBuffer(BYTE *pSourceBuf)
*  Sets the source buffer.
*  \param pSourceBuf an BYTE pointer.Point to the source buffer.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraSourceBuffer(BYTE *pSourceBuf)
{
	cgSourceBuffer = pSourceBuf;
}

/*! \fn FOVCamera::setFOVCameraImageWidth(int nWidth)
*  Sets the Image Width.
*  \param nWidth an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraImageWidth(int nWidth)
{	
	cgImageWidth = nWidth;
}

/*! \fn FOVCamera::setFOVCameraImageHeight(int nHeight)
*  Sets the Image height.
*  \param nHeight an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraImageHeight(int nHeight)
{
	cgImageHeight = nHeight;
}

/*! \fn FOVCamera::setFOVCameraBitCount(int nBitCount)
*  Sets the Image data format (number of bits/pixel).
*  \param nBitCount an int.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraBitCount(int nBitCount)
{
	cgBitCount = nBitCount;
}

/*! \fn FOVCamera::setFOVCameraImageFlip(BOOL bVerFlip)
*  Sets the Image flip.
*  \param bVerFlip a bool.The image is flipped in the vertical direction, TRUE is flipped, and FALSE is normal.
*  \returns nothing.
*/
void FOVCamera::setFOVCameraImageFlip(BOOL bVerFlip)
{
	cgImageFlip = bVerFlip;
}

/*! \fn FOVCamera::dataTransform()
*  Calls the camera method to passes the image data of the source buffer to the target buffer.
*  \returns nothing.
*/
void FOVCamera::dataTransform()
{
	CGDataTransform(cgDestinationBuffer, cgSourceBuffer, cgImageWidth, cgImageHeight, cgBitCount, cgImageFlip);
}

/*! \fn FOVCamera::staticMemUnlock()
*  Unlocks the static memory locked by the function CGStaticMemLock.
*  \returns nothing.
*/
void FOVCamera::staticMemUnlock()
{
	cgStatus = CGStaticMemUnlock(handle);
}

/*! \fn FOVCamera::stopCGSnap()
*  Stops the image card to capture image data to the memory.
*  \returns nothing.
*/
void FOVCamera::stopCGSnap()
{
	cgStatus = CGStopSnap(cgHandle);
}

/*! \fn FOVCamera::verifyStatus(CGSTATUS status)
*  The CG_VERIFY macro verifies that the function execution was successful.
*  \param CGSTATUS status.The return status of the function execution
*  \returns nothing.
*/
void FOVCamera::verifyStatus(CGSTATUS status)
{
	CG_VERIFY(status);
}

/*! \fn bool FOVCamera::getSuccessStatus()
*  Gets the succeess status of the last executed call.
*  \returns status as bool.
*/
bool FOVCamera::getSuccessStatus()
{
	if (cgStatus == CG_OK)
		return true;
	else
		return false;
}
void FOVCamera::getbrightnessvalue_(int value)
{

	CGAdjustVideo(cgHandle, BRIGHTNESS, value);

}
void FOVCamera::getcontrastvalue_(int value)
{
	CGAdjustVideo(cgHandle, CONTRAST, value);
}
void FOVCamera::gethuevalue_(int value)
{
	CGAdjustVideo(cgHandle, HUE, value);
}
void FOVCamera::getsaturationvalue_(int value)
{
	CGAdjustVideo(cgHandle, SATURATION, value);
}
void FOVCamera::saveB_C_H_S_Value(int B, int C, int H, int S)
{
	CGAdjustVideo(cgHandle, BRIGHTNESS, B);
	CGAdjustVideo(cgHandle, CONTRAST, C);
	CGAdjustVideo(cgHandle, HUE, H);
	CGAdjustVideo(cgHandle, SATURATION, S);
}