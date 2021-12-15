//
#ifndef DRAWPATTERN_H
#define DRAWPATTERN_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QtGui>
#include <qlabel.h>
#include "Global.hpp"
#include <QThread>
#include "resourceFileOCT.h"

namespace Ui {
	class DrawPattern;
}

class DrawPattern : public QWidget
{
	Q_OBJECT

public:
	explicit DrawPattern(QWidget *parent = 0);
	~DrawPattern();
	void DrawLine(double x1, double y1, double x2, double y2, double radius);
	//void DrawLine_inAC_window(double, double, double, double);
	void DrawLine_6mm(double x1, double y1, double x2, double y2);
	bool msgboxflag = false;
	void comportread_internalfixation();
	struct scanSettings  COM;
	QPixmap *pixmapCube;
	QPainter *painterCube;
	QPen *penCube; QPainter *painterCube1;// QPen *penCube1;
	QPen *penCube1;
	bool mousePress = false;
	bool mousePressInternalFixation = false;
	bool mousePressLine = false;
	bool cubelabel_pressed = false;
	bool disklabel_pressed = false;
	bool drawingPatternFromACWindow = false;
	double x1, y1, x2, y2, radius;
	int cubeXPos;
	int cubeYPos;
	double cubeScanLength;
	double cubeScanWidth;
	void updateScan();
	void resetLabel();
	void Update_InternalFixation();
	double result;
	void GetScanModeAndWidth(QString scan, double length, double width);
	void Scan_Range();
	QString	ScanTypeForOverLay_AC;
	int ScanType;
	void DrawCube();
	void readCubeScanSettingsCSVFile();
	double startP1xDP;
	double startP2xDP;
	double startP3xDP;

	double endP1xDP;
	double endP2xDP;
	double endP3xDP;

	double startP1yDP;
	double startP2yDP;
	double startP3yDP;

	double endP1yDP;
	double endP2yDP;
	double endP3yDP;
	int internalFixationLabelStartX;
	int internalFixationLabelStartY;
signals:
	void sendgalvovaluesCgSnap(float, float, float, float);
	void sendgalvovaluesCgSnapCube(float, float, float, float, float, float);
	void labelpositonforAC_window(float, float, float, float);

	void sendsignaltoFundusWidget();
	void SendCubeCounterValue(int);
	void scanangle(double, double);
	void sendpoint(double, double, double, double);
	

	public slots:
	void GetScanMode(QString Scan);
	//void GetScanModeAndWidth(QString, double length, double width);
	void receiveangle(double, double, double, double);

protected:
	/*void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent*);*/
private:
	Ui::DrawPattern *ui;

};

#endif // DRAWPATTERN_H
