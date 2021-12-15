#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_IntensityProfile.h"

#include<QPixmap>
#include<QPalette>
#include<QSqlDatabase>
#include<qdebug.h>
#include <qsqltablemodel.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlQueryModel>
#include <QThread>
#include "CamCmosOctUsb3.h"
#include "qcustomplot.h"
#include <QOpenGLWidget>
#include<openglwidget.h>
#include "Global.hpp"
#include"drawpattern.h"
//
#include "HM_HashuScan.h"
#include "HM_HashuUDM.h"
//����Hashu���ƿ����ӿ�
#pragma comment(lib,"HM_HashuScan.lib")


//opengl
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <fstream>
#include "Global.hpp"
#include <qtimer.h>
#include <QElapsedTimer>
using namespace std;

class QExposeEvent;
class QOpenGLShaderProgram;

//
#include "cgSnapCameraWidget.h "

class IntensityProfile : public QMainWindow
{
	Q_OBJECT

public:
	IntensityProfile(QWidget *parent = Q_NULLPTR);
	char* m_UdmBuffer;
	int m_nBuffSize;
	bool connected ;
	tCameraInfo CameraInfo;
	QString CMOSCameraID = "";
	bool deepChoroidalImaging = false;
	unsigned short* cameraDataRaw;
	QCustomPlot *plt1;
	bool octDeviceOnOffThreadStarted;
	bool startedOctLiveStream = false;
	bool octCameraConnected = false;
	char * rawFileName = new char[500];
	FILE *rawDataFp;
	void startLive();
	int octcameraruntimeError;
	bool  dataSave;
	OpenGLWidget *openglwidget1 = NULL;
	OpenGLWidget *openglwidget2 = NULL;
	OpenGLWidget *openglwidget3 = NULL;
	struct scanSettings maculaHDLine6mm, maculaHDLine10mm, maculaRadial10mm, maculaCube10mm, glaucomaOpticDisc10mm, glaucomaMaculaCube10mm, anteriorHDLine10mm, anteriorRadial10mm, COM;
	//void callSingleLinePatternRedraw();
	//
	bool sendReceiveOpenglRequestConnected = false;
	//void connecteOpenglRequestSignal();
	float  GalvoStartx6mm1;
	float  GalvoStarty6mm1;
	float  GalvoStartz6mm1;
	float  GalvoStarta6mm1;
	float  GalvoEndx6mm1;
	float  GalvoEndy6mm1;
	float  GalvoEndz6mm1;
	float  GalvoEnda6mm1;

	float GalvoStartx10mm1;
	float GalvoStarty10mm1;
	float GalvoStartz10mm1;
	float GalvoStarta10mm1;
	float GalvoEndx10mm1;
	float GalvoEndy10mm1;
	float GalvoEndz10mm1;
	float GalvoEnda10mm1;
private slots:
	void on_pushButton_Scn_clicked();
	void on_btn_Stop_clicked();
	void on_pushButton_Take_Reference_clicked();
	void on_pushButton_BScan_clicked();
	void on_pushButton_Export_clicked();
	//void on_pushButton_Close_clicked();
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();
	void on_radioButton_clicked(bool checked);
	void on_radioButton_2_clicked(bool checked);
	void on_pushButton_Cube_clicked();
signals:
	//bool sendSignaltoFundusImageLiveCG(bool);
	void SendScanModeCgSnap(QString);
	void SendScanModeAndWidthToCgSnap(QString, double, double);
public slots:
	void camera_Settings();
	void galvo_Settings();
	void callHDLineScanMode();
	void stopOCTCameraAcquisitionFromOpenglWidget(int err);
	void on_CustomPlot(unsigned short*);
	void rawData(unsigned short*);
	void Bscan();
	void setOCTCameraAndCgSnapCamera();
	void ReceiveFundusIMage(QPixmap FundsImage);
	//void callScan();
	void octlive();
	void Scan_Range();
	void loadMaculaCube();
	void runOCTLiveStream1();
	void runOCTLiveStream2();
	void runOCTLiveStream3();
	void RemoveWidgets();
	//void receiveGalvoValuesICWindow(float, float, float, float);
	//void BtnSingleLine();
private:
	Ui::IntensityProfileClass ui;
};

