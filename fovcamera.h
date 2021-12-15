#ifndef FOVCAMERA_H
#define FOVCAMERA_H

#include "CGVidEx.h"
#include "CGVideo.h"

class FOVCamera
{
private:
	HCG	cgHandle;
	HANDLE handle;
	CGSTATUS cgStatus;
	int acquisitionStatus = 0;
	int cgIimageCardSerialNumber;
	tagVIDEO_STANDARD cgVideoStandard;
	tagVIDEO_FORMAT cgVideoFormat;
	tagVIDEO_SCAN cgScanMode;
	tagCRY_OSC cgCrystalType;
	VIDEO_SOURCE cgVideoSource;
	int cgInputWindowStartX;
	int cgInputWindowStartY;
	int cgInputWindowWidth;
	int cgInputWindowHeight;
	int cgOutputWindowStartX;
	int cgOutputWindowStartY;
	int cgOutputWindowWidth;
	int cgOutputWindowHeight;
	DWORD cgMemOffset;
	BOOL cgInterline;
	WORD cgSum;
	DWORD cgStartOffset;
	DWORD cgMemoryLockLength;
	PVOID *cgPointerMemoryLock;
	BYTE *cgDestinationBuffer;
	BYTE *cgSourceBuffer;
	int	cgImageWidth;
	int	cgImageHeight;
	int	cgBitCount;
	BOOL cgImageFlip;

public:
	FOVCamera();
	~FOVCamera();	
	void setImageCardSerialNumber(int imageCardSerialNumber);
	int beginCGCard();

	void setFOVCameraVideoStandard(tagVIDEO_STANDARD videoStandard);
	void callSetCGVideoStandard();

	void setFOVCameraVideoFormat(tagVIDEO_FORMAT videoFormat);
	void callSetCGVideoFormat();

	void setFOVCameraScanMode(tagVIDEO_SCAN scanMode);
	void callSetCGScanMode();

	void setFOVCameraCryOSC(tagCRY_OSC crystalType);
	void callSelectCGCryOSC();

	void setFOVCameraVideoSource(tagVIDEO_SOURCE_TYPE videoSource);
	void callSetCGVideoSource();
	
	void setFOVCameraInputWindowStartX(int startX);
	void setFOVCameraInputWindowStartY(int startY);
	void setFOVCameraInputWindowWidth(int width);
	void setFOVCameraInputWindowHeight(int height);
	void callSetCGInputWindow();
	
	void setFOVCameraOutputWindowStartX(int startX);
	void setFOVCameraOutputWindowStartY(int startY);
	void setFOVCameraOutputWindowWidth(int width);
	void setFOVCameraOutputWindowHeight(int height);
	void callSetCGOutputWindow();

	void setFOVCameraDwMemOffset(DWORD dwMemOffset);
	void setFOVCameraInterline(BOOL bInterline);
	void setFOVCameraNoOfImages(WORD wSum);
	void StartCGSnap();

	int getCGSnappingNumber();

	void setFOVCameraStartOffset(DWORD dwStartOffset);
	void setFOVCameraMemoryLockLength(DWORD dwLength);
	void setFOVCameraDwStartOffset(PVOID *ppLineAddr);
	
	void staticMemLock();
	
	void setFOVCameraDestinationBuffer(BYTE *pDestBuf);	
	void setFOVCameraSourceBuffer(BYTE *pSourceBuf);	
	void setFOVCameraImageWidth(int nWidth);	
	void setFOVCameraImageHeight(int nHeight);	
	void setFOVCameraBitCount(int nBitCount);	
	void setFOVCameraImageFlip(BOOL bVerFlip);
	
	void dataTransform();

	void staticMemUnlock();
	void stopCGSnap();
	void verifyStatus(CGSTATUS status);
	bool getSuccessStatus();
	void setDevice(int nDevice);
	void getbrightnessvalue_(int vlue);
	void getcontrastvalue_(int value);
	void gethuevalue_(int value);
	void getsaturationvalue_(int value);
	void saveB_C_H_S_Value(int B, int C, int H, int S);



};

#endif // FOVCAMERA_H