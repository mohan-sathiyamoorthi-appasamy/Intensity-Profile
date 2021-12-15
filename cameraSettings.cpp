#include"galvo.h"
#include"qdebug"
#include "IntensityProfile.h"
#include "cameraSettings.h"
#define DATAFOLDER "../data_db"
cameraSettings::cameraSettings(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	opendb();
	
}
void cameraSettings::opendb()
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	QString dbFile = DATAFOLDER;
	dbFile.append("/fundus/camera.db");
	QString dbfilename = dbFile;
	//qDebug() << "datafolder" << dbfilename;
	if (QFile(dbfilename).exists())
	{
		qDebug() << "file exsist";
		db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName(dbfilename);
		db.open();
		if (db.isOpen())
		{
			//	qDebug() << "DataBase Opened";
		}
		else
		{
			//qDebug() << "dataBase Not opened";
		}
	}
	else
	{
		//qDebug() << "file not exsist";
	}
	
	QSqlQueryModel* qry = new QSqlQueryModel();
	qry->setQuery("SELECT * FROM Settings");

	line = qry->record(0).value("Line").toInt();
	frame = qry->record(0).value("Frame").toInt();
	Syncmode = qry->record(0).value("SyncMode").toInt();

	qDebug() << "From DB line: " << line;
	qDebug() << "From DB frame: " << frame;
	qDebug() << "From DB sync: " << Syncmode;
	ui.Line->setValue(line);
	ui.Frame->setValue(frame);
	ui.SyncMode->setCurrentIndex(Syncmode);
}
void cameraSettings::on_pushButton_Back_clicked()
{
	this->close();
}
void cameraSettings::on_pushButton_Update_clicked()
{
	int line = ui.Line->value();
	int frame = ui.Frame->value();
	int gain = ui.Gain->value();
	int sync = ui.SyncMode->currentIndex();
	qDebug() << "line" << line;
	qDebug() << "frame" << frame;
	qDebug() << "gain" << gain;
	qDebug() << "sync" << sync;
	qDebug() << "UPDATE Settings SET Line = '" + QString::number(line) + "', SyncMode = '" + QString::number(sync) + "', Frame = '" + QString::number(frame) + "' WHERE Id = 1;";
	QSqlQueryModel* qry = new QSqlQueryModel();
	qry->setQuery("UPDATE Settings SET Line ='" + QString::number(line) + "', SyncMode ='" + QString::number(sync) + "', Frame ='" + QString::number(frame) + "' WHERE Id = 1;");	

}
