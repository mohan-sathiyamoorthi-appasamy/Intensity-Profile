#include <QtWidgets/QDialog>
#include "ui_galvo.h"

class galvo : public QDialog
{
	Q_OBJECT

public:
	galvo(QWidget *parent = Q_NULLPTR);
	bool connected;
	QString ScanType;
	
	//bool nativeEvent(const QByteArray &eventType, void *message, long *result);
	void Scan_Range();
	void setGalvoValues(int, int, float, float, float, float);
	void SetGalvoValue6mm(float, float, float, float);
	void setScanLength(int);
	

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
	//void GetGalvoValue10mm();
	//void SetEndRegisterData();
	//void GetMemoryAndBinfile();
	private slots:
	void on_pushButton_Close_clicked();
	void on_pushButton_Speed_2_clicked();
	
	void on_pushButton_Laser_clicked();
	void on_pushButton_Run_clicked();
	void on_pushButton_Stop_clicked();
	void on_pushButton_ok_clicked();
	void on_pushButton_HDLine_clicked();
	void on_pushButton_Cube_clicked();
	void on_pushButton_Radial_clicked();
	public slots:
	void SetLaserSource();
	
private:
	char* m_UdmBuffer;
	int m_nBuffSize;
	int hdLineScanLength;
	Ui::galvo ui;
};