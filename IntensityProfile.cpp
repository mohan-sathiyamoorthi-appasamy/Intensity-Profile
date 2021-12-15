#include "IntensityProfile.h"
#include "openGLWidget.h"
#include "HM_CommExp.h"
#include "HM_HashuScan.h"
#include "HM_HashuUDM.h"
#include "hm_udmgenerator.h"
#include "HM_VPUBuilder.h"
#include "QtGui"
#include "galvo.h"
#include "cameraSettings.h"
#include "atlstr.h"
#include "octCMOSCamera.h"
#include "qcustomplot.h"
#include "resourceFileOCT.h"
#include "global.hpp"
#include "AppaGalvo.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define DATAFOLDER "../data_db"
#define DATAFOLDER1  "..\\data1\\"
#define DATAFOLDER2  "..\\Data\\"
#define MAX_TIMEOUT_ACQ_IN_MS 3000	
OpenGLWidget *openglwidget;
galvo *Galvo;
using namespace cv;
DrawPattern *drawpattern;
cameraSettings *camera;
octCMOSCamera *octCamera;
CgSnapCameraWidget *cameraCGLive;
liveThreadStreamCgSnap *livecg;
AppaGalvo *appaGalvo;
int scanModeType;
bool fundusImageSave = true;
int HDLineScanLength = HDLINESCAN_10MM;
int cubeScanLength;
float Galvo_Start_x1;
float Galvo_End_x1;
int previousScanModeType = 0;
int RowCount, ColumnCount;
int runCounter = 0;
IntensityProfile::IntensityProfile(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	appaGalvo = AppaGalvo::getInstance();
	appaGalvo->setWindowState(Qt::WindowMaximized);
	ui.stackedWidget->setCurrentIndex(0);
	
	QMenu* menu = new QMenu(this);
	ui.pushButton_Settings->setMenu(menu);
	QAction *Cam_Settings = menu->addAction("Camera Settings");
	QAction *Galvo_Settings = menu->addAction("Galvo Settings");
	connect(Cam_Settings, SIGNAL(triggered()), this, SLOT(camera_Settings()));
	connect(Galvo_Settings, SIGNAL(triggered()), this, SLOT(galvo_Settings()));
	ui.pushButton_Scn->setStyleSheet("font: 75 14pt Times New Roman;background-color: rgb(177, 177, 177, 255);"); 
	ui.btn_Stop->setStyleSheet("font: 75 14pt Times New Roman;background-color: rgb(177, 177, 177, 255);");
	ui.pushButton_Export->setStyleSheet("font: 75 14pt Times New Roman;background-color: rgb(177, 177, 177, 255);");
	ui.pushButton_Take_Reference->setStyleSheet("font: 75 14pt Times New Roman;background-color: rgb(177, 177, 177, 255);");
	camera = new cameraSettings(this);
	startLive();
	octlive();
	Scan_Range();

//	HDLineScanLength = 
}
//void IntensityProfile::on_pushButton_clicked()
//{
//	startLive();
//}
void IntensityProfile::camera_Settings()
{
	
	camera->show();
}
void IntensityProfile::galvo_Settings()
{
	Galvo = new galvo(this);
	Galvo->show();
}
void IntensityProfile::startLive()
{
	//PRINTFUNCTIONNAME
	octCamera = octCMOSCamera::getInstance();
	octCamera->stopOCTCameraAcquisition();
	octCamera->closeCamera();
	octCamera->octCmosCameraStatus = false;
	octCamera->initOpenOCTCamera();
	cameraCGLive = CgSnapCameraWidget::getInstance();
	cameraCGLive->initCgCamera();
	cameraCGLive->startLiveCg();
	cameraCGLive->initMatrixCamera();
	
	if (cameraCGLive->sendReceiveBufferSignalConnected == false)
		cameraCGLive->connectSendPixelBufferSignal();
	 cameraCGLive->anglereset();// cameraCGLive->angle_new = 0;
	livecg = liveThreadStreamCgSnap::getInstance();
	if (sendReceiveOpenglRequestConnected == false)
	///	connecteOpenglRequestSignal();
	connect(cameraCGLive, SIGNAL(SendFundusImage(QPixmap)), this, SLOT(ReceiveFundusIMage(QPixmap)));
	setOCTCameraAndCgSnapCamera();
	//callScan();

	
}
void IntensityProfile::octlive()
{
	qDebug("octlive");
	//camera = new cameraSettings(this);

	if (octCameraConnected == true){
		octCamera->stopOCTCameraAcquisition();
		octCameraConnected = false;
		qDebug() << "octCameraConnected" << octCameraConnected;

	}
	if (octCameraConnected == false){
		//octCamera->updatecamera();	
		octCamera->startOCTCameraAcquisition1();
		octCameraConnected = true;
		
	}
 }

void IntensityProfile::on_btn_Stop_clicked()
{
	octCamera->stopOCTCameraAcquisition();
}
//AScan
void IntensityProfile::on_CustomPlot(unsigned short* cameraData)
{		
}

//BScan
void IntensityProfile::callHDLineScanMode()
{
	openglwidget = new OpenGLWidget();
	connect(openglwidget, SIGNAL(sendSignaltoFundusImageLiveCG(bool)), livecg, SLOT(receiveSignalFromOpenglWidgetToLiveCG(bool)));
	//Pass camera paraemters to the widget.
	openglwidget->InitCameraRead();
	//Start the streaming.
	startedOctLiveStream = openglwidget->startOCTLiveStream();
	//Add the widget to the layout.
	//QObject::connect(openglwidget, SIGNAL(senddata1(unsigned short*)), this, SLOT(on_CustomPlot(unsigned short*)));
	ui.OCTViewerGridLayout->addWidget(openglwidget, 0, 0);
	qDebug("stop");
}

void IntensityProfile::on_pushButton_BScan_clicked()
{
}
void IntensityProfile::Bscan()
{
	//on_pushButton_Scn_clicked();
	//ui.OCTViewerGridLayout->addWidget(openglwidget, 0, 0);
}
void IntensityProfile::rawData(unsigned short* cameradata)
{	
	QString folder = DATAFOLDER2;
	QString path = folder;
	QDateTime capturetime = QDateTime::currentDateTime();
	folder.append("RawData");
	QString number = QStringLiteral("%1.raw").arg(1, 5, 10, QLatin1Char('0'));
	QString FilePath1 = folder  + number;
	strcpy(rawFileName, FilePath1.toLocal8Bit().data());
	rawDataFp = fopen(rawFileName, "wb");
	fwrite(cameradata, 2048 * 2000 *  sizeof(unsigned short), 1, rawDataFp);
	fclose(rawDataFp);	
}
void IntensityProfile::stopOCTCameraAcquisitionFromOpenglWidget(int err)
{
	 octcameraruntimeError = err;	
}

void IntensityProfile::on_pushButton_Take_Reference_clicked()
{
	openglwidget->loadreference();
}
void IntensityProfile::on_pushButton_Export_clicked()
{
	QMessageBox::StandardButton dlg;
	dlg = QMessageBox::critical(this, tr("Rawdata"),QString(tr("Did You Want To Save?")),
		QMessageBox::Yes | QMessageBox::No);
	if (dlg == QMessageBox::Yes)
	{
		connect(openglwidget, SIGNAL(senddata1(unsigned short*)), this, SLOT(rawData(unsigned short*)));
	}
	else
	{
		
	}

}
//
void IntensityProfile::setOCTCameraAndCgSnapCamera()
{
	ui.LeftEye_scrollArea->setWidget(cameraCGLive);
	ui.LeftEye_scrollArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	QObject::connect(this, SIGNAL(SendScanModeCgSnap(QString)), cameraCGLive, SLOT(GetScanModeCgSnap(QString)));
	QObject::connect(this, SIGNAL(SendScanModeAndWidthToCgSnap(QString, double, double)), cameraCGLive, SLOT(GetScanModeAndWidthToCgSnap(QString, double, double)));
	
}
void IntensityProfile::ReceiveFundusIMage(QPixmap FundsImage)
{
	QPixmap pix(FundsImage);
	//QImage img(fundusUsb3_0);
	QString path = DATAFOLDER2;
	path.append("/Fundus/");
	QDir dir(path);
	if (dir.exists()){}
	else
		QDir().mkdir(path);

	path.append("FundusImage");
	QDir dirImage(path);
	path.append("/");
	if (dirImage.exists()){}
	else
		QDir().mkdir(path);

	QDateTime dateandTime = QDateTime::currentDateTime();
	QString currentDate = dateandTime.toString("yyyyMMdd");

	//Currentdate
	path.append(currentDate);
	path.append("/");
	QDir dir1(path);
	if (dir1.exists()){}

	else
		QDir().mkdir(path);

	QString currentTime = dateandTime.toString("hhmm");
	QString currentTimeStamp = currentDate + currentTime;
	path.append(currentTimeStamp);
	path.append("/");
	QDir dir2(path);
	if (dir2.exists()){

		if (fundusImageSave)
		{
			pix.save(path + "/FundusImage.tif");
		}
	}
	else
		QDir().mkdir(path);
}

void IntensityProfile::on_pushButton_Scn_clicked()
{
	
#ifdef QT_DEBUG
	PRINTFUNCTIONNAME
#else
	PRINTFUNCTIONNAME
#endif	

	OpenGLWidget::finalStack.clear();
    livecg->proceedFundusLiveCG = false;
	runCounter = 0;
	scanModeType = HDLINE;
	cameraCGLive->setOctScanType(HDLINE);

	if (HDLineScanLength == HDLINESCAN_6MM){
		emit  SendScanModeCgSnap("HDLINESCAN_6MM");
	}
	else if (HDLineScanLength == HDLINESCAN_10MM){
		emit  SendScanModeCgSnap("HDLINESCAN_10MM");
	}
	//Remove any previous widgets from the layout if we are not coming from PL window.If we go back to PL window the lastScanSettingType is 0.
	
	RemoveWidgets();
	
	if (HDLineScanLength == HDLINESCAN_6MM){
		ui.radioButton->setChecked(true);
		lastScanSettingLengthType = HDLINESCAN_6MM;
		
	}
	else if (HDLineScanLength == HDLINESCAN_10MM){
		ui.radioButton_2->setChecked(true);
		lastScanSettingLengthType = HDLINESCAN_10MM;
		
	}
	
	appaGalvo->setScanLength(HDLineScanLength);
	appaGalvo->setGalvoScanPattern(scanModeType, ui.lineEdit_7->text().toInt(), ui.lineEdit->text().toFloat(), ui.lineEdit_2->text().toFloat(), ui.lineEdit->text().toFloat(), ui.lineEdit_2->text().toFloat(), ui.lineEdit->text().toFloat(), ui.lineEdit_2->text().toFloat());
	callHDLineScanMode();
	previousScanModeType = HDLINE;
}

void IntensityProfile::on_pushButton_clicked()
{
	qDebug() << "Parameters are Loading!";
	
	Galvo_Start_x1 = ui.lineEdit->text().toFloat();
	Galvo_End_x1 = ui.lineEdit_2->text().toFloat();
	on_pushButton_Scn_clicked();
	qDebug() << "Loaded Parameters are: " << Galvo_Start_x1 << Galvo_End_x1 ;

}


void IntensityProfile::on_pushButton_2_clicked()
{
	appaGalvo->ChangeGalvoSpeed(ui.lineEdit_7->text().toInt());
}


void IntensityProfile::on_radioButton_clicked(bool checked)
{
#ifdef QT_DEBUG
	//PRINTFUNCTIONNAME
#else
	//PRINTFUNCTIONNAME
#endif
	HDLineScanLength = HDLINESCAN_6MM; // "6mm";
	ui.lineEdit->setText(QString::number(GalvoStartx6mm1, 'f', 4));
	ui.lineEdit_2->setText(QString::number(GalvoEndx6mm1, 'f', 4));
	emit SendScanModeCgSnap("HDLINESCAN_6MM"); 

	on_pushButton_Scn_clicked();
}
void IntensityProfile::Scan_Range()
{

	QString scanSettingsCsvFileName = DATAFOLDER1;
	scanSettingsCsvFileName.append(ScanSettingsCSVFile);
	QFile scanSettingsValuesCSVFile(scanSettingsCsvFileName);
	scanSettingsValuesCSVFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream textStream(&scanSettingsValuesCSVFile);
	QString row;
	QStringList scanSettingsAttributes;
	int rowNumber = 1;
	qDebug() << "  scanSettingsCsvFileName   " << scanSettingsCsvFileName;
	while (!textStream.atEnd())
	{
		row = textStream.readLine();
		qDebug() << row << "  " << rowNumber;
		if (rowNumber == 2)
		{
			QStringList scanSettingsValues;
			scanSettingsValues = row.split(',');
			qDebug() << "scanSettingsValues scanSettingsValues scanSettingsValues  scanSettingsValues[0]  " << scanSettingsValues[0] << "  " << scanSettingsValues[6] << endl;

			if (scanSettingsValues[0] == ScanAreaMacula && scanSettingsValues[1] == ScanHDLine)
			{
				qDebug() << "enterd in to if condition in draw pattern..................................";
				qDebug() << scanSettingsValues[6];
				GalvoStartx6mm1 = ((QString)scanSettingsValues[6]).toFloat();
				qDebug() << "pattern GalvoStartx6mm" << GalvoStartx6mm1;
				GalvoEndx6mm1 = ((QString)scanSettingsValues[7]).toFloat();
				qDebug() << "pattern GalvoEndx6mm" << GalvoEndx6mm1;
				GalvoStartx10mm1 = ((QString)scanSettingsValues[8]).toFloat();
				qDebug() << "pattern GalvoStartx6mm" << GalvoStartx6mm1;
				GalvoEndx10mm1 = ((QString)scanSettingsValues[9]).toFloat();
				qDebug() << "pattern GalvoEndx6mm" << GalvoEndx6mm1;
				ui.lineEdit->setText(QString::number(GalvoStartx10mm1, 'f', 4));
				ui.lineEdit_2->setText(QString::number(GalvoEndx10mm1, 'f', 4));
			}
		}rowNumber = rowNumber + 1;
		qDebug() << "  rowNumber  " << rowNumber;
	}
	
}
void IntensityProfile::on_radioButton_2_clicked(bool checked)
{
#ifdef QT_DEBUG
	//	PRINTFUNCTIONNAME
#else
	//PRINTFUNCTIONNAME
#endif
	HDLineScanLength = HDLINESCAN_10MM;// "10mm";
	ui.lineEdit->setText(QString::number(GalvoStartx10mm1, 'f', 4));
	ui.lineEdit_2->setText(QString::number(GalvoEndx10mm1, 'f', 4));
	qDebug() << "GalvoStartx10mm1:" << GalvoStartx10mm1 << "GalvoEndx10mm1" << GalvoEndx10mm1;
	emit SendScanModeCgSnap("HDLINESCAN_10MM");
	on_pushButton_Scn_clicked();
}
//cube
void IntensityProfile::on_pushButton_Cube_clicked(){
#ifdef QT_DEBUG
	PRINTFUNCTIONNAME
#else
	PRINTFUNCTIONNAME
#endif	
	//scanArea = "MaculaCube";
	OpenGLWidget::finalStack.clear();
	runCounter = 0;
	livecg->proceedFundusLiveCG = false;
	cameraCGLive->setOctScanType(CUBE);
	emit SendScanModeCgSnap("CUBE");	
	RemoveWidgets();
	appaGalvo->setScanLengthCube(cubeScanLength);
	appaGalvo->setGalvoScanPattern(scanModeType, ui.lineEdit_7->text().toInt(), ui.lineEdit->text().toFloat(), ui.lineEdit_2->text().toFloat(), ui.lineEdit->text().toFloat(), ui.lineEdit_2->text().toFloat(), ui.lineEdit->text().toFloat(), ui.lineEdit_2->text().toFloat());
	scanModeType = CUBE;
	runOCTLiveStream1();
	runOCTLiveStream2();
	runOCTLiveStream3();
	previousScanModeType = CUBE;
}
void IntensityProfile::loadMaculaCube()
{

}
void IntensityProfile::runOCTLiveStream1()
{
#ifdef QT_DEBUG
	PRINTFUNCTIONNAME
#else
	PRINTFUNCTIONNAME
#endif		
	openglwidget1 = new OpenGLWidget();
//Pass camera paraemters to the widget.
	openglwidget1->InitCameraRead();// h_buffer);
	//Start the streaming.
	openglwidget1->startOCTLiveStream();
	if (scanModeType == CUBE ){
		QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		openglwidget1->setSizePolicy(sizePolicy);
		openglwidget1->setGeometry(100, 0, 500, 200);
		openglwidget1->setMaximumSize(500, 200);
		openglwidget1->setMinimumSize(500, 200);
		ui.OCTViewerGridLayout->addWidget(openglwidget1, 2, 0, Qt::AlignCenter);
	}
	else
		ui.OCTViewerGridLayout->addWidget(openglwidget1, 0, 0);
}

//Shows the stream second radial scan in openglwidget2
void IntensityProfile::runOCTLiveStream2()
{
	openglwidget2 = new OpenGLWidget();
	//Pass camera paraemters to the widget.
	openglwidget2->InitCameraRead();
	openglwidget2->startOCTLiveStream();
	if (scanModeType == CUBE ){
		QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		openglwidget2->setSizePolicy(sizePolicy);
		openglwidget2->setGeometry(100, 0, 500, 200);
		openglwidget2->setMaximumSize(500, 200);
		openglwidget2->setMinimumSize(500, 200);
		ui.OCTViewerGridLayout->addWidget(openglwidget2, 0, 0, Qt::AlignCenter);
	}
	else
		ui.OCTViewerGridLayout->addWidget(openglwidget2, 1, 0);// 0, 1);
}

//Shows the stream third radial scan in openglwidget3
void IntensityProfile::runOCTLiveStream3()
{
#ifdef QT_DEBUG
	PRINTFUNCTIONNAME
#else
	PRINTFUNCTIONNAME
#endif		
		openglwidget3 = new OpenGLWidget();
	//Pass camera paraemters to the widget.
	openglwidget3->InitCameraRead();
	openglwidget3->startOCTLiveStream();
	if (scanModeType == CUBE ){
		QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		openglwidget3->setSizePolicy(sizePolicy);
		openglwidget3->setGeometry(50, 150, 850, 350);
		openglwidget3->setMaximumSize(850, 350);
		openglwidget3->setMinimumSize(850, 350);
		ui.OCTViewerGridLayout->addWidget(openglwidget3, 1, 0, Qt::AlignCenter);
	}
	else
		ui.OCTViewerGridLayout->addWidget(openglwidget3, 2, 0);// 1, 0);		
}
void IntensityProfile::RemoveWidgets()
{
#ifdef QT_DEBUG
	PRINTFUNCTIONNAME
#else
	PRINTFUNCTIONNAME
#endif
		if (previousScanModeType == HDLINE ){
		RowCount = 0;
		ColumnCount = 0;
		}
		else if (previousScanModeType == RADIAL ){
			RowCount = 2;
			ColumnCount = 1;
		}
		else if (previousScanModeType == CUBE ){
			RowCount = 2;
			ColumnCount = 0;
		}
		if (ui.OCTViewerGridLayout->count() >= 1)
		{
			for (int Row = 0; Row <= RowCount; Row++)
			{
				for (int Column = 0; Column <= ColumnCount; Column++)
				{
					if (previousScanModeType == CUBE)
					{
					
						QLayoutItem* item = ui.OCTViewerGridLayout->itemAtPosition(Row, Column);
						QWidget* itemWidget = item->widget();
						if (itemWidget)
						{
							ui.OCTViewerGridLayout->removeWidget(itemWidget);
							delete itemWidget;
						}
					}
					else
					{
						QLayoutItem* item = ui.OCTViewerGridLayout->itemAtPosition(Row, Column);
						QWidget* itemWidget = item->widget();
						if (itemWidget)
						{
							ui.OCTViewerGridLayout->removeWidget(itemWidget);
							delete itemWidget;
						}
					}
				}
			}
		}
}