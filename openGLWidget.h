#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <fstream>
#include "Global.hpp"
#include "CamCmosOctUsb3.h"
#include <qtimer.h>
#include <QElapsedTimer>
#include <qsqltablemodel.h>
#include<qcustomplot.h>
#include <opencv2\opencv.hpp>
//#include "ui_IntensityProfile.h">
using namespace std;

class QExposeEvent;
class QOpenGLShaderProgram;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();
	int bufferCtrOpengl;
	CAM_HANDLE hCamera;
	int nError;

	tImageInfos ImageInfos;
	unsigned short *RAWDATA;
	FILE *rawDataFp;
	int color = 1;
	
	int numberOfImages;
	int lineNumber;
	
	//sivapriya
	bool takehistogramimage = false;
	//end

	float *d_FrameBuffer;
	unsigned short *d_FrameBufferushort;
	bool takeSnap = false;
	unsigned short *buffer1;
	void *plot2;
	float *d_FrameBuffer1;
	float *h_ss = (float*)malloc(2 * sizeof(float));
	unsigned short *buffer2;
	unsigned short *h_ss_gray = (unsigned short*)malloc(2 * sizeof(unsigned short));
	struct cudaGraphicsResource *bscanCudaRes;
	GLuint bscanTEX;
	GLuint bscanPBO;
	bool initProcess = false;
	struct scanSettings scanSetting;
	int scan;
	bool startOCTLiveStream();
	void initGLVarAndPtrs(
		int winWid,
		int winHei
		);
	void setBufferPtr(unsigned short *h_buffer);
	void registerCudaHost();
	void cudaEvent();
	void InitCameraRead();// buffer *h_buffer);// , int *buffLen);
	void runCamera();
	void InitProcess(
		int buffLen
		);
	void runProcess();
	bool showStream = true;
	bool processing = true;
	void registerBuffer();
	void setProcess(bool proc);
	static std::vector<cv::Mat> finalStack;
	float lambda_Max=880.0f;
	QString widgetGl;
	int bufferLen;
	int framesTotal = 1;
	int frameWidthGl = 2048; // 512;
	int frameHeightGl=2000;// = 4000;
	int framesPerBufferCu = 1; //This value needs to be divisble by 3, and a factor of the total number of frames
	int scanvalue = 2000;
	int windowWidth = 1200;
	int windowHeight = 700;
	int samplingMethodCu = 1;
	int	widthGl;
	int heightGl;
	int frames;
	int bscanWidth;
	int bscanHeight;
	float minVal = 2.4f;
	float maxVal = 1.3f;
	//int color = 1;
	bool minMax = false;
	int cropOffset = 50;
	bool processCompleted = false;
	bool deepChoroidalImaging = false;
	//CUDA ONLY Variables to be transferred
	int fftLenMult = 2;
	float dispMagCu = 15;
	void paintGL();
	float dispValThirdCu = 0;
	buffer *h_buff = new buffer[BUFFNUM];
	unsigned short *rawImageUshort;
	unsigned short *imageUshort;
	
	int FrameCount = 0;
	char  bufferRawImage[5];
	char * rawFileName = new char[500];
	int runCameraCount = 0;
	unsigned short* cameraDataRaw;
	bool rawDataSave = false;
	QString rawDataPath;
	void flushBuffer();
	unsigned short *referenceImageUshort;
	bool ReferenceSaveFlag = false;
	void on_CustomPlot1();
	void initializeGL();
	void resizeGL(int w, int h);
	void loadreference();
private:
	
	public slots:
signals:
	void senddata1(unsigned short *);
	void sendSignaltoFundusImageLiveCG(bool);
private:

protected:
	 
	
};

#endif // OPENGLWIDGET_H