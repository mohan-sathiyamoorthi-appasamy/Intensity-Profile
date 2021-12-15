#include <QtWidgets/QDialog>
#include "ui_cameraSettings.h"
#include<QSqlDatabase>
#include<qdebug.h>
#include <qsqltablemodel.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlQueryModel>

class cameraSettings : public QDialog
{
	Q_OBJECT

public:
	cameraSettings(QWidget *parent = Q_NULLPTR);
	int line, Syncmode, frame, gain;
	void opendb();
	public slots:
	 
	private slots:
	void on_pushButton_Back_clicked();
	void on_pushButton_Update_clicked();
private:
	QSqlDatabase db;
	Ui::Dialog ui;
};