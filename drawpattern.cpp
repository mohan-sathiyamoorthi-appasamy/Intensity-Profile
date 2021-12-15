#include "drawpattern.h"
#include "ui_drawpattern.h"
#include <QtGui>	
#include "intensityProfile.h"
#include <qsize.h>
#include <QMouseEvent>
#include <QtGui>
#include <QByteArray>
#include <qlabel.h>
#include <QPoint>
#include "resourceFileOCT.h"
#include <QMessageBox>
#define DATAFOLDER  "..\\data1\\"
float  GalvoStartx6mm;
float  GalvoStartx10mm;
float  GalvoEndx6mm;
float GalvoEndx10mm;
#define DATAFOLDER_new_internal_fixation_port  "..\\data\\"
int x, y;
bool drawlineinAC_flag = false;
int x1_new=0; int y1_new=133; int x2_new=183; int y2_new=133;
#define INTERNALFIXATION_X_MIN 0
#define INTERNALFIXATION_Y_MIN 0
#define INTERNALFIXATION_X_MAX 5
#define INTERNALFIXATION_Y_MAX 9
#define INTERNALFIXATION_GREEN_DOT_WIDTH 26
#define INTERNALFIXATION_GREEN_DOT_HEIGHT 26
#define PI 3.14
#define MAX_SCAN_LENGTH 8    // 10
#define TOTAL_LED_IN_X 5
#define TOTAL_LED_IN_Y 9
#define CubeScanSettingsCSVFile "CubeScan.csv"
QString ScanTypeForOverLay;
int InternalFixation = 0;
float angle = 0;
int counter = 0;
QMessageBox::StandardButton dlg;
int CubeScan_StartX_New, CubeScan_StartY_New, CubeScan_StartX_Old, CubeScan_StartY_Old;
int CubeScan_StartX, CubeScan_StartY;
int CubeScan_EndX, CubeScan_MiddleX;
int CubeScan_EndY, CubeScan_MiddleY;
int CubeScan_Reference_Y;
QString ReceivedData1 = 0; QByteArray Received1;
int CubeReferencePointX, CubeReferencePointY;
QString comport_Internalfixation;
float cube_StartX, cube_EndX;
float cube_StartY, cube_EndY;
int _global_WidgetWidth;
int _global_WidgetHeight;
int InternalFixationWidth;
int InternalFixationHeight;
int maculaCubeWidth = 400;// 260; // 258;// 256;// 260;// 255;
int maculaCubeHeight = 375;// 260;// 260;// 258;// 256;// 260;// 255;
int cubeX, cubeY;
int cubex_new, cubey_new;
int initialCubeScanStartX, initialCubeScanStartY;
char Inital_InternalFixation_command[] = { 0x7F, 0x02, 0x02, 0x05, 0xFF, 0xFF };
char OS_GlaucomaDisc_InternalFixation_command[] = { 0x7F, 0x02, 0x03, 0x09, 0xFF, 0xFF };
char OD_GlaucomaDisc_InternalFixation_command[] = { 0x7F, 0x02, 0x03, 0x02, 0xFF, 0xFF };
bool mouseclick_disable_flag_forAC_window = false;
QPointF mousePressPosition;
bool fixationPressed = false;
bool linePressed = false;
bool cubePressed = false;
bool lineMoved = false;

float lineStartX = 0;
float lineStartY = 0;
float lineEndX = 0;
float lineEndY = 0;


int MouseRelasePosX = 0;
int MouseRelasePosY = 0;

int MousePressPosX = 0;
int MousePressPosY = 0;


DrawPattern::DrawPattern(QWidget *parent) :QWidget(parent), ui(new Ui::DrawPattern)
{
	//FreeConsole();
	//PRINTFUNCTIONNAME
	ui->setupUi(this);
	this->installEventFilter(this);
	Scan_Range();
	this->installEventFilter(this);
	//ser = new Serial();
	comportread_internalfixation();
	qDebug() << "comport_Internalfixation  : " << comport_Internalfixation;
	_global_WidgetWidth = this->width();
	_global_WidgetHeight = this->height();

	InternalFixationWidth = _global_WidgetWidth / TOTAL_LED_IN_X;
	InternalFixationHeight = _global_WidgetHeight / TOTAL_LED_IN_Y;

	qDebug() << "InternalFixationWidth = " << InternalFixationWidth;
	qDebug() << "InternalFixationHeight = " << InternalFixationHeight;

	ui->lineLabel->setGeometry(0, 0, _global_WidgetWidth, _global_WidgetHeight);
	ui->lineLabel->hide();
	qDebug() << "internalFixationLabel  initail posn : " << (_global_WidgetWidth / 2) - 13 << "   " << (_global_WidgetHeight / 2) - 13 << INTERNALFIXATION_GREEN_DOT_WIDTH << "   " << INTERNALFIXATION_GREEN_DOT_HEIGHT;
	internalFixationLabelStartX = (_global_WidgetWidth / 2) - 13;
	internalFixationLabelStartY = (_global_WidgetHeight / 2) - 13;
	ui->internalFixationLabel->setGeometry(internalFixationLabelStartX, internalFixationLabelStartY, INTERNALFIXATION_GREEN_DOT_WIDTH, INTERNALFIXATION_GREEN_DOT_HEIGHT);
	ui->internalFixationLabel->hide();
	QString pngFileName_cube = DATAFOLDER;
	pngFileName_cube.append("PNGFiles\\macula_cube_green.png");
	QPixmap mypix_cube(pngFileName_cube);
	ui->cube_label->setPixmap(mypix_cube.scaled(maculaCubeWidth, maculaCubeHeight));
	ui->cube_label->setGeometry(_global_WidgetWidth - 340, _global_WidgetHeight - maculaCubeHeight, maculaCubeWidth, maculaCubeHeight);//cubeBox_Width, cubeBox_Height);
	ui->cube_label->hide();

	ui->lineLabel->installEventFilter(this);
	ui->internalFixationLabel->installEventFilter(this);
	ui->cube_label->installEventFilter(this);
	ui->disk_label->installEventFilter(this);

	//************
	QString pngFileName = DATAFOLDER;
	pngFileName.append("PNGFiles\\dotGreen.png");
	QPixmap mypix(pngFileName);
	ui->internalFixationLabel->setPixmap(mypix.scaled(25, 25));
	ui->internalFixationLabel->setStyleSheet("background : transparent");
	//*****************
	ui->disk_label->hide();
	QObject::connect(this, SIGNAL(sendpoint(double, double, double, double)), this, SLOT(receiveangle(double, double, double, double)));
}

void DrawPattern::resetLabel()
{
	internalFixationLabelStartX = ((_global_WidgetWidth / MAX_SCAN_LENGTH) * 7) - 13;
	internalFixationLabelStartY = (_global_WidgetHeight / 2) - 13;
}
//=========================================
DrawPattern::~DrawPattern()
{
	delete ui;
}

void DrawPattern::Update_InternalFixation(){

	//PRINTFUNCTIONNAME
	QByteArray command;

	qDebug() << "Update_InternalFixation" << ScanTypeForOverLay;
	resetLabel();
}

void DrawPattern::updateScan(){
	PRINTFUNCTIONNAME
	mouseclick_disable_flag_forAC_window = false;

	if (ScanTypeForOverLay == "HDLINESCAN_6MM" || ScanTypeForOverLay == "HDLineAnteriorSegment10mm"){
		DrawLine((_global_WidgetWidth / MAX_SCAN_LENGTH), (_global_WidgetHeight / MAX_SCAN_LENGTH) * 4, (_global_WidgetWidth / MAX_SCAN_LENGTH) * 7, (_global_WidgetHeight / MAX_SCAN_LENGTH) * 4, 0);               //  for 6 mm Scan Range
			//emit sendgalvovaluesCgSnap(Galvo_Start_x_6mm, 0, Galvo_End_x_6mm,0);        //    for 6 MM Hd Line
	}
	else if (ScanTypeForOverLay == "HDLINESCAN_10MM"){
		DrawLine(0, (_global_WidgetHeight / MAX_SCAN_LENGTH) * 4, (_global_WidgetWidth / MAX_SCAN_LENGTH) * 10, (_global_WidgetHeight / MAX_SCAN_LENGTH) * 4, 0);              //  for 10 mm Scan Range
		//	emit sendgalvovaluesCgSnap(Galvo_Start_x_10mm, 0, Galvo_End_x_10mm, 0);        //    for 10 MM Hd Line
	}
	else if (ScanTypeForOverLay == "CUBE"){
		qDebug() << "cube";
		readCubeScanSettingsCSVFile();
		DrawCube();
	}

}
void DrawPattern::GetScanMode(QString Scan){
	PRINTFUNCTIONNAME
		ScanTypeForOverLay = Scan;
	
	cubeScanLength = 6;
	cubeScanWidth = 6;
	ui->internalFixationLabel->show();
	ui->lineLabel->show();
	
	updateScan();
	Update_InternalFixation();
}

void DrawPattern::DrawLine(double x1, double y1, double x2, double y2, double radius){
	PRINTFUNCTIONNAME
		QPixmap pixmap(QSize(_global_WidgetWidth, _global_WidgetHeight));
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	QBrush brush;
	if (mousePressLine == true)
	{
		QPen pen(Qt::yellow, 1, Qt::SolidLine);
		painter.setPen(pen);
		//QPolygonF arrowHead;

		brush.setColor(QColor(255, 255, 0));
		brush.setStyle(Qt::SolidPattern);
	}
	else{
		QPen pen(Qt::red, 1, Qt::SolidLine);
		painter.setPen(pen);

		//	QPolygonF arrowHead;
		//QBrush brush;
		brush.setColor(QColor(255, 0, 0));
		brush.setStyle(Qt::SolidPattern);
	}
	QLine line(x1, y1, _global_WidgetWidth / 2, _global_WidgetHeight / 2);
	painter.drawLine(line);

	QLine line1(_global_WidgetWidth / 2, _global_WidgetHeight / 2, x2, y2);
	painter.drawLine(line1);
	QPolygonF arrowHead;
	if (x2 < _global_WidgetWidth / 2)
	{

		float aa = atan2(_global_WidgetHeight / 2 - y1, _global_WidgetWidth / 2 - x1);
		angle = (PI * 2) - aa;
		//result * 180 / 3.141592
		qDebug() << " the arrow angle in if condition is" << angle;
		QPointF sourcepoint = QPointF(x1, y1);
		//QPointF sourcepoint = QPointF(X_MousePosition_linelabel, y_MousePosition_linelabel);
		qreal arrowSize = 10;
		QPointF arrowP1 = sourcepoint + QPointF(std::sin(angle + PI / 3) * arrowSize,
			std::cos(angle + PI / 3) * arrowSize);
		QPointF arrowP2 = sourcepoint + QPointF(std::sin(angle + PI - PI / 3) * arrowSize,
			std::cos(angle + PI - PI / 3) * arrowSize);
		arrowHead << sourcepoint << arrowP1 << arrowP2;
		// Fill polygon
		QPainterPath path;
		path.addPolygon(arrowHead);
		painter.drawPolygon(arrowHead);
		painter.fillPath(path, brush);
	}
	else
	{
	
		float aa = atan2(_global_WidgetHeight / 2 - y2, _global_WidgetWidth / 2 - x2);
		angle = (PI * 2) - aa;
		qDebug() << " the arrow angle in else condition is" << angle;
		//QPolygonF headarrow;
		QPointF sourcepoint = QPointF(x2, y2);
		//QPointF sourcepoint = QPointF(X_MousePosition_linelabel, y_MousePosition_linelabel);
		qreal arrowSize = 10;
		QPointF arrowP1 = sourcepoint + QPointF(std::sin(angle + PI / 3) * arrowSize,
			std::cos(angle + PI / 3) * arrowSize);
		QPointF arrowP2 = sourcepoint + QPointF(std::sin(angle + PI - PI / 3) * arrowSize,
			std::cos(angle + PI - PI / 3) * arrowSize);
		arrowHead << sourcepoint << arrowP1 << arrowP2;
		// Fill polygon
		QPainterPath path;
		path.addPolygon(arrowHead);
		painter.drawPolygon(arrowHead);
		painter.fillPath(path, brush);
		//	painter.drawPolygon(QPolygonF() << sourcepoint << arrowP1 << arrowP2);

	}
	QPen pen1(Qt::green, 1, Qt::SolidLine);
	painter.setPen(pen1);
	QRect elipse(_global_WidgetWidth / 2, _global_WidgetHeight / 2, 5, 5);
	painter.drawEllipse(elipse.center(), 15, 15);

	ui->lineLabel->setStyleSheet("background:transparent");
	ui->lineLabel->setPixmap(pixmap);
	ui->cube_label->hide();
	ui->disk_label->hide();
}



void DrawPattern::DrawLine_6mm(double x1, double y1, double x2, double y2)
{
	PRINTFUNCTIONNAME
		qDebug() << " --------  DrawLine " << x1 << "   " << y1 << "   " << x2 << "  " << y2 << "   " << radius;
	QPixmap pixmap(QSize(_global_WidgetWidth, _global_WidgetHeight));
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	QPen pen(Qt::red, 1, Qt::SolidLine);
	painter.setPen(pen);

	QLine line(x1, y1, _global_WidgetWidth / 2, _global_WidgetHeight / 2);
	//painter.drawLine(line);

	QLine line1(_global_WidgetWidth / 2, _global_WidgetHeight / 2, x2, y2);
	//painter.drawLine(line1);

	QBrush brush;
	QPolygonF arrowHead;
	brush.setColor(QColor(255, 0, 0));
	brush.setStyle(Qt::SolidPattern);
	float aa = atan2(_global_WidgetHeight / 2 - y2, _global_WidgetWidth / 2 - x2);
	angle = (PI * 2) - aa;
	QPointF sourcepoint = QPointF(x2, y2);
	qreal arrowSize = 10;
	QPointF arrowP1 = sourcepoint + QPointF(std::sin(angle + PI / 3) * arrowSize,
		std::cos(angle + PI / 3) * arrowSize);
	QPointF arrowP2 = sourcepoint + QPointF(std::sin(angle + PI - PI / 3) * arrowSize,
		std::cos(angle + PI - PI / 3) * arrowSize);
	arrowHead << sourcepoint << arrowP1 << arrowP2;
	// Fill polygon
	QPainterPath path;
	path.addPolygon(arrowHead);
	painter.drawPolygon(arrowHead);
	painter.fillPath(path, brush);

	ui->lineLabel->setStyleSheet("background:transparent");
	ui->lineLabel->setPixmap(pixmap);
}
void DrawPattern::comportread_internalfixation()
{

	QString scanSettingsCsvFileName = DATAFOLDER;
	scanSettingsCsvFileName.append("portDetails.csv");
	QFile scanSettingsValuesCSVFile(scanSettingsCsvFileName);
	scanSettingsValuesCSVFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream textStream(&scanSettingsValuesCSVFile);
	QString row;
	QStringList scanSettingsAttributes;
	int rowNumber = 1;
	qDebug() << "Reading csv Files " << scanSettingsCsvFileName;
	while (!textStream.atEnd())
	{
		row = textStream.readLine();
		if (rowNumber == 1)
		{
			QStringList scanSettingsValues;
			scanSettingsValues = row.split(',');

			if (scanSettingsValues[0] == "Internalfixationcom" && scanSettingsValues[1] == "Internalfixationportnumber")
			{

				int portname = scanSettingsValues[2].toInt();
				comport_Internalfixation = "COM" + QString::number(portname);
				qDebug() << " Internalfixationcom portname  : " << comport_Internalfixation;
			}


		}
	}
}

void DrawPattern::Scan_Range()
{

	QString scanSettingsCsvFileName = DATAFOLDER;
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



				GalvoStartx6mm = ((QString)scanSettingsValues[6]).toFloat();
				qDebug() << "pattern GalvoStartx6mm" << GalvoStartx6mm;
				GalvoEndx6mm = ((QString)scanSettingsValues[7]).toFloat();
				qDebug() << "pattern GalvoEndx6mm" << GalvoEndx6mm;
				GalvoStartx10mm = ((QString)scanSettingsValues[8]).toFloat();
				qDebug() << "pattern GalvoStartx6mm" << GalvoStartx6mm;
				GalvoEndx10mm = ((QString)scanSettingsValues[9]).toFloat();
				qDebug() << "pattern GalvoEndx6mm" << GalvoEndx6mm;
			}
		}rowNumber = rowNumber + 1;
		qDebug() << "  rowNumber  " << rowNumber;
	}
	//qDebug() << "**************************************************Galvo_startx_6mm" << Galvo_startx_6mm << endl;
}
void DrawPattern::receiveangle(double x, double y, double x_end, double y_end)
{
	PRINTFUNCTIONNAME
		x1_new = x;
	y1_new = y;
	x2_new = x_end;
	y2_new = y_end;

}
void DrawPattern::GetScanModeAndWidth(QString scan, double length, double width)
{
	PRINTFUNCTIONNAME
		//msgboxflag = false;
	//mouseclick_disable_flag_forAC_window = false;
	ScanTypeForOverLay = scan;
	cubeScanLength = length;
	cubeScanWidth = width;
	updateScan();
	Update_InternalFixation();
	//GetValuesForCubeScanLength();
}
void DrawPattern::DrawCube()
{
	PRINTFUNCTIONNAME
	ui->cube_label->show();
	counter = 0;
	
	qDebug() << "CubeScan_StartX  = " << CubeScan_StartX << endl;
	qDebug() << "CubeScan_StartY  = " << CubeScan_StartY << endl;
	qDebug() << "maculaCubeWidth  = " << maculaCubeWidth << endl;
	qDebug() << "maculaCubeHeight  = " << maculaCubeHeight << endl;
	maculaCubeWidth = 400;// 260;// 258;// 260;
	maculaCubeHeight = 375;// 260;// 260;// 258;// 260;
	if (cubeScanLength >6)
	{
		double scanLengthDiff = cubeScanLength - 6;
		int newCubeWidth = scanLengthDiff * 35;	//(256/6=43)
		maculaCubeWidth = maculaCubeWidth + newCubeWidth;
		maculaCubeHeight = maculaCubeHeight + newCubeWidth;
		CubeScan_StartX = (_global_WidgetWidth - maculaCubeWidth) / 2;
		CubeScan_StartY = (_global_WidgetHeight - maculaCubeHeight) / 2;

		qDebug() << " 12 mm  " << CubeScan_StartX << "   " << CubeScan_StartY << "   " << maculaCubeWidth << "  " << maculaCubeHeight;

	}
	QString pngFileName_cube = DATAFOLDER;
	pngFileName_cube.append("PNGFiles\\macula_cube_green.png");
	QPixmap mypix_cube(pngFileName_cube);
	ui->cube_label->setPixmap(mypix_cube.scaled(maculaCubeWidth, maculaCubeHeight));
	ui->cube_label->setGeometry(CubeScan_StartX, CubeScan_StartY, maculaCubeWidth, maculaCubeHeight);
	QPixmap pixmap(QSize(_global_WidgetWidth, _global_WidgetHeight));
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	QPen pen(Qt::red, 1, Qt::SolidLine);
	painter.setPen(pen);
	qDebug() << "_global_WidgetWidth  _global_WidgetHeight = " << _global_WidgetWidth << "   " << _global_WidgetHeight << endl;
	qDebug() << "CubeScan_StartX  = " << CubeScan_StartX << endl;
	qDebug() << "CubeScan_StartY  = " << CubeScan_StartY << endl;
	//line
	QLine line50(CubeScan_StartX, CubeScan_StartY + (maculaCubeHeight / 2), (CubeScan_StartX + maculaCubeWidth), CubeScan_StartY + (maculaCubeHeight / 2));
	painter.drawLine(line50);
	QLine linearrow1((CubeScan_StartX + maculaCubeWidth), CubeScan_StartY + (maculaCubeHeight / 2), (CubeScan_StartX + maculaCubeWidth) - 10, (CubeScan_StartY + (maculaCubeHeight / 2)) - 5);
	painter.drawLine(linearrow1);
	QLine linearrow2((CubeScan_StartX + maculaCubeWidth), CubeScan_StartY + (maculaCubeHeight / 2), (CubeScan_StartX + maculaCubeWidth) - 10, (CubeScan_StartY + (maculaCubeHeight / 2)) + 5);
	painter.drawLine(linearrow2);
	QPen pen1(Qt::green, 1, Qt::SolidLine);
	painter.setPen(pen1);
	QRect elipse(_global_WidgetWidth / 2, _global_WidgetHeight / 2, 5, 5);
	painter.drawEllipse(elipse.center(), 15, 15);
	ui->lineLabel->setStyleSheet("background:transparent");
	ui->lineLabel->setPixmap(pixmap);
	qDebug() << "  ---  offset is  ---- " << (CubeScan_StartX + maculaCubeWidth / 2) << "     " << (CubeScan_StartY + maculaCubeHeight / 2);
	qDebug() << " 12 mm  " << CubeScan_StartX << "   " << CubeScan_StartY << "   " << maculaCubeWidth << "  " << maculaCubeHeight;
}
void DrawPattern::readCubeScanSettingsCSVFile()
{
	PRINTFUNCTIONNAME
		QString scanSettingsCsvFileName = DATAFOLDER;
	scanSettingsCsvFileName.append(CubeScanSettingsCSVFile);
	QFile scanSettingsValuesCSVFile(scanSettingsCsvFileName);
	scanSettingsValuesCSVFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream textStream(&scanSettingsValuesCSVFile);
	QString row;
	QStringList scanSettingsAttributes;
	int rowNumber = 1;
	while (!textStream.atEnd())
	{
		row = textStream.readLine();
		qDebug() << "---row is   " << row;
		if (rowNumber > 1)
		{
			QStringList scanSettingsValues;
			scanSettingsValues = row.split(',');
			initialCubeScanStartX = ((QString)scanSettingsValues[0]).toInt();
			initialCubeScanStartY = ((QString)scanSettingsValues[1]).toInt();
			CubeScan_StartX = initialCubeScanStartX;
			CubeScan_StartY = initialCubeScanStartY;
			qDebug() << " x y is   " << CubeScan_StartX << "   " << CubeScan_StartY;
		}
		rowNumber = rowNumber + 1;
	}
}