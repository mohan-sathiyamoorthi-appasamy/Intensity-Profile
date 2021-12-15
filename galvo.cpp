#include"galvo.h"
#include"qdebug"
#include "IntensityProfile.h"
#include "atlstr.h"
#define DATAFOLDER  "..\\data1\\"
STMarkSetting g_Mark;
HWND sd;
//DrawPattern *drawpattern;
//unsigned int g_IOOutPut;
//float  GalvoStartx6mm1;
//float  GalvoStartx10mm1;
//float  GalvoEndx6mm1;
//float GalvoEndx10mm1;
//float  GalvoStarty6mm1;
//float  GalvoEndy6mm1;
bool Referenceflag = false;
int nStart;
float Galvo_Start_x = -6.0000, Galvo_Start_y = 0.0000, Galvo_Start_z = 0.0000, Galvo_Start_a = 0.0000;
float Galvo_End_x = 6.0000, Galvo_End_y = 0.0000, Galvo_End_z = 0.0000, Galvo_End_a = 0.0000;

float Galvo_Start_x_Top = -6.0000, Galvo_Start_y_Top = 6.0000, Galvo_Start_z_Top = 0.0000, Galvo_Start_a_Top = 0.0000;
float Galvo_End_x_Top = 6.0000, Galvo_End_y_Top = 6.0000, Galvo_End_z_Top = 0.0000, Galvo_End_a_Top = 0.0000;

float Galvo_Start_x_Middle = -6.0000, Galvo_Start_y_Middle = 0.0000, Galvo_Start_z_Middle = 0.0000, Galvo_Start_a_Middle = 0.0000;
float Galvo_End_x_Middle = 6.0000, Galvo_End_y_Middle = 0.0000, Galvo_End_z_Middle = 0.0000, Galvo_End_a_Middle = 0.0000;

float Galvo_Start_x_Bottom = -6.0000, Galvo_Start_y_Bottom = -6.0000, Galvo_Start_z_Bottom = 0.0000, Galvo_Start_a_Bottom = 0.0000;
float Galvo_End_x_Bottom = 6.0000, Galvo_End_y_Bottom = -6.0000, Galvo_End_z_Bottom = 0.0000, Galvo_End_a_Bottom = 0.0000;
galvo::galvo(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Scan_Range();
	ui.lineEdit_12->setText("1000");
	ui.label_32->hide();
	ui.label_7->hide();
	ui.label_15->hide();
	ui.label_33->hide();
	ui.label_34->hide();
	ui.label_35->hide();
	ui.lineEdit_Start_Y->hide();
	ui.lineEdit_End_Y->hide();
	ui.lineEdit_Start_Z->hide();
	ui.lineEdit_End_Z->hide();
	ui.lineEdit_Start_A->hide();
	ui.lineEdit_End_A->hide();

	//drawpattern->Scan_Range();
	//drawpattern->Scan_Range().GalvoStartx6mm;
}

//galvo
//bool galvo::nativeEvent(const QByteArray &eventType, void *message, long *result)
//{

	//if (connected != true){
	//	qDebug("winEvent");
	//	MSG *msg = static_cast<MSG*>(message);

	//	int devId = (int)msg->lParam;
	//	qDebug() << msg->lParam << "   " << devId;
	//	if (devId >= 0){
	//		int i = HM_InitComm(msg->hwnd, HM_MSG_DeviceStatusUpdate);
	//		qDebug() << "i  is " << i;
	//		if (1 == HM_GetDeviceStatus(msg->lParam))
	//		{
	//			HM_ConnectTo(msg->lParam);
	//			HM_MarkStart();
	//			connected = true;
	//			SetLaserSource();
	//			on_pushButton_Run_clicked();
	//			qDebug() << "Device Connected";
	//		}
	//		else if (2 == HM_GetDeviceStatus(msg->lParam))
	//		{
	//			qDebug() << "Device Disconnected";
	//		}
	//	}
	//}
	//return false;
//}
void galvo::on_pushButton_Run_clicked()
{
	HM_MarkStart();
}
void galvo::on_pushButton_Close_clicked()
{
	this->close();
}
void galvo::on_pushButton_Speed_2_clicked()
{
	//if (0 == HM_GetDeviceStatus(0) || 1 == HM_GetDeviceStatus(0))	// ???????????
	//{
	//	QString s = ui.lineEdit_12->text();
	//	CString str = s.toStdString().c_str();
	//	CW2A nSpeed((LPCWSTR)str);
	//	g_Mark.nMarkV = strtol(nSpeed, NULL, 10);
	//	int y = HM_SetSingleMarkPara(&g_Mark, ENUM_Mark::MarkV);
	//}
	//else
	//{
	//	qDebug("The device is not connected!");
	//}
}

//laser
void galvo::SetLaserSource()
{
	////qDebug("Source is connected inside the function");
	//if (g_IOOutPut & 0x01 == 0x01)
	//{

	//	g_IOOutPut = g_IOOutPut & 0xFE;
	//	HM_SetOutPut(0, g_IOOutPut);
	//	qDebug("Source is connected");
	//}
	//else
	//{

	//	g_IOOutPut = g_IOOutPut | 0x01;
	//	HM_SetOutPut(0, g_IOOutPut);
	//	qDebug("Source is not connected");
	//}
}
void galvo::on_pushButton_Laser_clicked()
{
	SetLaserSource();
}
void galvo::on_pushButton_ok_clicked()
{
	qDebug() << "Parameters are Loading!";

	Galvo_Start_x = ui.lineEdit_Start_X->text().toFloat();
	Galvo_Start_y = ui.lineEdit_Start_Y->text().toFloat();
	Galvo_Start_z = ui.lineEdit_Start_Z->text().toFloat();
	Galvo_Start_a = ui.lineEdit_Start_A->text().toFloat();

	Galvo_End_x = ui.lineEdit_End_X->text().toFloat();
	Galvo_End_y = ui.lineEdit_End_Y->text().toFloat();
	Galvo_End_z = ui.lineEdit_End_Z->text().toFloat();
	Galvo_End_a = ui.lineEdit_End_A->text().toFloat();
	//on_pushButton_HDLine_clicked();
	qDebug() << "Loaded Parameters are: " << Galvo_Start_x << Galvo_Start_y << Galvo_Start_z << Galvo_Start_a << Galvo_End_x << Galvo_End_y << Galvo_End_z << Galvo_End_a;

}
void galvo::on_pushButton_Stop_clicked()
{
	int r0Data = 0x00;
	int u = HM_WriteUDMReg(0x00, &r0Data);
}
void galvo::on_pushButton_HDLine_clicked()
{
	ui.label_32->hide();
	ui.label_7->hide();
	ui.label_15->hide();
	ui.label_33->hide();
	ui.label_34->hide();
	ui.label_35->hide();
	ui.lineEdit_Start_Y->hide();
	ui.lineEdit_End_Y->hide();
	ui.lineEdit_Start_Z->hide();
	ui.lineEdit_End_Z->hide();
	ui.lineEdit_Start_A->hide();
	ui.lineEdit_End_A->hide();
	//ScanType = "HDLine";
	//CString strPath;														// µ±Ç°ÎÄ¼þ¼ÐÏÂ´´½¨tem.binÎÄ¼þ

	//GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	//strPath.ReleaseBuffer();
	//int pos = strPath.ReverseFind('\\');
	//strPath = strPath.Left(pos + 1);
	//strPath = strPath + CString("UDMGen.bin");
	//CW2A ps2aPath((LPCWSTR)strPath);					// ÁÙÊ±binÎÄ¼þÂ·¾¶
	//on_pushButton_Speed_2_clicked();
	//UDM_NewFile();
	//UDM_Main();
	//UDM_Irmov(0x01, 0x00);	 //0x01,r0;
	//UDM_Irmov(0x00, 0x08); //0x01,r8;	Ê¹ÓÃR0ºÍR8¼Ä´æÆ÷×÷±È½Ï£¬²»µÈÑ­»·ÔËÐÐ£¬ÏàµÈÍË³ö£»

	//int nStart = UDM_Point2D(-8.00000, 8.000000, 0.000000, 0);

	//for (float i = -8.0000; i < 8.0000; i += 0.16)
	//{
	//	UDM_Point2D(i, 0.000000, 0.000000, 0);
	//}

	///********************************** HD Line *****************************/
	//structUdmPos startP1;// = { -8.00000, 0.00000, 0.000000, 0 };
	//
	//startP1.x = Galvo_Start_x;
	//startP1.y = Galvo_Start_y;
	//startP1.z = Galvo_Start_z;
	//startP1.a = Galvo_Start_a;

	//structUdmPos endP1;// = { 8.00000, 0.00000, 0.000000, 0 };
	//endP1.x = Galvo_End_x;
	//endP1.y = Galvo_End_y;
	//endP1.z = Galvo_End_z;
	//endP1.a = Galvo_End_a;

	////UDM_Line2D(startP1, endP1);
	//UDM_Jne(0x08, nStart);	//Ê¹ÓÃR0ºÍR8¼Ä´æÆ÷×÷±È½Ï£¬²»µÈÑ­»·ÔËÐÐ£¬ÏàµÈÍË³ö£»        //  Use the R0 and R8 registers for comparison, wait for the loop to run, and exit equally;
	////UDM_Jump(0, 0, 0);
	//UDM_EndMain();
	////¿ÉÒÔÖ±½Ó»ñµÃ´ò±êUDMÄÚ´æµØÖ·
	//UDM_GetUDMBuffer(&m_UdmBuffer, &m_nBuffSize);
	////Ò²¿ÉÒÔ½«UDMÉú³ÉÎÄ¼þ
	//UDM_SaveToFile(ps2aPath);
	//if (0 == HM_GetDeviceStatus(0) || 1 == HM_GetDeviceStatus(0))		// 设备当前状态为连接状态
	//{
	//	qDebug("The device is connected!");
	//	CString strPath;
	//	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	//	strPath.ReleaseBuffer();
	//	int pos = strPath.ReverseFind('\\');
	//	strPath = strPath.Left(pos + 1);
	//	strPath = strPath + CString("UDMGen.bin");
	//	CW2A ps2aPath((LPCWSTR)strPath);					// 当前文件夹下tem.bin文件路径
	//	//可以下载文件
	//	//		HM_DownloadStart_DDR_UDM(ps2aPath,0,this->m_hWnd,0);	// 下载bin文件

	//	//也可以将内存Buffer数据传下去
	//	int va = HM_DownloadStart_DDR_UDMBuff(m_UdmBuffer, m_nBuffSize, sd, 0);	// 下载bin文件
	//}
	//else
	//{
	//	qDebug("The device is not connected!");
	//}
	//HM_MarkStart();
}
void galvo::on_pushButton_Cube_clicked()
{
	//ui.label_32->show();
	//ui.label_7->show();
	//ui.label_15->show();
	//ui.label_33->show();
	//
	//ui.lineEdit_Start_Y->show();
	//ui.lineEdit_End_Y->show();
	//ui.lineEdit_Start_Z->show();
	//ui.lineEdit_End_Z->show();
	//ScanType = "Cube";
	//CString strPath;														// µ±Ç°ÎÄ¼þ¼ÐÏÂ´´½¨tem.binÎÄ¼þ
	//GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	//strPath.ReleaseBuffer();
	//int pos = strPath.ReverseFind('\\');
	//strPath = strPath.Left(pos + 1);
	//strPath = strPath + CString("UDMGen.bin");
	//CW2A ps2aPath((LPCWSTR)strPath);					// ÁÙÊ±binÎÄ¼þÂ·¾¶
	//on_pushButton_Speed_2_clicked();
	//UDM_NewFile();
	//UDM_Main();
	//UDM_Irmov(0x01, 0x00);  //0x01,r0;
	//UDM_Irmov(0x00, 0x08); //0x01,r8;	Ê¹ÓÃR0ºÍR8¼Ä´æÆ÷×÷±È½Ï£¬²»µÈÑ­»·ÔËÐÐ£¬ÏàµÈÍË³ö£»
	//int nStart = UDM_Point2D(0.00000, 0.000000, 0.000000, 0);

	////galvo parameters
	///******************************* cube **********************************/

	//structUdmPos startP_Top;   // = { -6.0000, 6.0000, 0.000000, 0 };
	//structUdmPos endP_Top;    // = { 6.0000, 6.0000, 0.000000, 0 };
	////to set the parameters from the users.
	//
	//Galvo_Start_x_Top = Galvo_Start_x;
	//startP_Top.x = Galvo_Start_x_Top;

	//Galvo_Start_y_Top = Galvo_Start_y;
	//startP_Top.y = Galvo_Start_y_Top;

	//Galvo_Start_z_Top = Galvo_Start_z;
	//startP_Top.z = Galvo_Start_z_Top;

	//Galvo_Start_a_Top = Galvo_Start_a;
	//startP_Top.a = Galvo_Start_a_Top;

	//Galvo_End_x_Top = Galvo_End_x;
	//endP_Top.x = Galvo_End_x_Top;

	//Galvo_End_y_Top = Galvo_End_y;
	//endP_Top.y = Galvo_End_y_Top;

	//Galvo_End_z_Top = Galvo_End_z;
	//endP_Top.z = Galvo_End_z_Top;

	//Galvo_End_a_Top = Galvo_End_a;
	//endP_Top.a = Galvo_End_a_Top;

	//UDM_Line2D(startP_Top, endP_Top);

	//structUdmPos startP_Middle;    // = { -6.00000, 0.00000, 0.000000, 0 };
	//structUdmPos endP_Middle;   // = { 6.00000, 0.00000, 0.000000, 0 };

	//startP_Middle.x = Galvo_Start_x_Middle;
	//startP_Middle.y = Galvo_Start_y_Middle;
	//startP_Middle.z = Galvo_Start_z_Middle;
	//startP_Middle.a = Galvo_Start_a_Middle;

	//endP_Middle.x = Galvo_End_x_Middle;
	//endP_Middle.y = Galvo_End_y_Middle;
	//endP_Middle.z = Galvo_End_z_Middle;
	//endP_Middle.a = Galvo_End_a_Middle;

	//UDM_Line2D(startP_Middle, endP_Middle);

	//structUdmPos startP_Bottom;    // = { -6.00000, -6.0000, 0.000000, 0 };
	//structUdmPos endP_Bottom;    // = { 6.00000, -6.0000, 0.000000, 0 };

	//startP_Bottom.x = Galvo_Start_x_Bottom;
	//startP_Bottom.y = Galvo_Start_y_Bottom;
	//startP_Bottom.z = Galvo_Start_z_Bottom;
	//startP_Bottom.a = Galvo_Start_a_Bottom;

	//endP_Bottom.x = Galvo_End_x_Bottom;
	//endP_Bottom.y = Galvo_End_y_Bottom;
	//endP_Bottom.z = Galvo_End_z_Bottom;
	//endP_Bottom.a = Galvo_End_a_Bottom;

	//UDM_Line2D(startP_Bottom, endP_Bottom);

	//UDM_Jne(0x08, nStart);	//Ê¹ÓÃR0ºÍR8¼Ä´æÆ÷×÷±È½Ï£¬²»µÈÑ­»·ÔËÐÐ£¬ÏàµÈÍË³ö£»
	//UDM_Jump(0, 0, 0);
	//UDM_EndMain();
	////¿ÉÒÔÖ±½Ó»ñµÃ´ò±êUDMÄÚ´æµØÖ·
	//UDM_GetUDMBuffer(&m_UdmBuffer, &m_nBuffSize);
	////Ò²¿ÉÒÔ½«UDMÉú³ÉÎÄ¼þ
	//UDM_SaveToFile(ps2aPath);

	//if (0 == HM_GetDeviceStatus(0) || 1 == HM_GetDeviceStatus(0))		// 设备当前状态为连接状态
	//{
	//	qDebug("The device is connected!");
	//	CString strPath;
	//	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	//	strPath.ReleaseBuffer();
	//	int pos = strPath.ReverseFind('\\');
	//	strPath = strPath.Left(pos + 1);
	//	strPath = strPath + CString("UDMGen.bin");
	//	CW2A ps2aPath((LPCWSTR)strPath);					// 当前文件夹下tem.bin文件路径
	//	//可以下载文件
	//	//		HM_DownloadStart_DDR_UDM(ps2aPath,0,this->m_hWnd,0);	// 下载bin文件

	//	//也可以将内存Buffer数据传下去
	//	int va = HM_DownloadStart_DDR_UDMBuff(m_UdmBuffer, m_nBuffSize, sd, 0);	// 下载bin文件
	//}
	//else
	//{
	//	qDebug("The device is not connected!");
	//}
	//HM_MarkStart();
}
void galvo::on_pushButton_Radial_clicked()
{
	
	//ScanType = "Radial";
	//CString strPath;														// µ±Ç°ÎÄ¼þ¼ÐÏÂ´´½¨tem.binÎÄ¼þ
	//GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	//strPath.ReleaseBuffer();
	//int pos = strPath.ReverseFind('\\');
	//strPath = strPath.Left(pos + 1);
	//strPath = strPath + CString("UDMGen.bin");
	//CW2A ps2aPath((LPCWSTR)strPath);					// ÁÙÊ±binÎÄ¼þÂ·¾¶
	//on_pushButton_Speed_2_clicked();
	//UDM_NewFile();
	//UDM_Main();
	//UDM_Irmov(0x01, 0x00);	 //0x01,r0;
	//UDM_Irmov(0x00, 0x08); //0x01,r8;	Ê¹ÓÃR0ºÍR8¼Ä´æÆ÷×÷±È½Ï£¬²»µÈÑ­»·ÔËÐÐ£¬ÏàµÈÍË³ö£»

	//int nStart = UDM_Point2D(0.00000, 0.000000, 0.000000, 0);
	///*******************************Radial beam**************************************/
	//float radius = 7.0000;
	//structUdmPos startPy = { 0, -7.000, 0 };
	//structUdmPos endPy = { 0, 7.000, 0 };
	//UDM_Line2D(startPy, endPy);
	//float  dec2 = 210;
	//float picalc2 = 180 / (90 - dec2);
	//float x2 = radius *cos(3.14 / picalc2);
	//float y2 = radius *sin(3.14 / picalc2);
	//float dec3 = 30;
	//float picalc3 = 180 / (90 - dec3);
	//float x3 = radius *cos(3.14 / picalc3);
	//float y3 = radius *sin(3.14 / picalc3);

	//structUdmPos startP2 = { x2, y2, 0.000000, 0 };
	//structUdmPos endP2 = { x3, y3, 0.000000, 0 };
	//UDM_Line2D(startP2, endP2);

	//float  dec = 240;
	//float picalc = 180 / (90 - dec);
	//float x = radius *cos(3.14 / picalc);
	//float y = radius *sin(3.14 / picalc);

	//float dec1 = 60;
	//float picalc1 = 180 / (90 - dec1);
	//float x1 = radius *cos(3.14 / picalc1);
	//float y1 = radius *sin(3.14 / picalc1);

	//structUdmPos startP1 = { x, y, 0.000000, 0 };
	//structUdmPos endP1 = { x1, y1, 0.000000, 0 };
	//UDM_Line2D(startP1, endP1);

	//structUdmPos startP = { -7.00, 0, 0, 0 };
	//structUdmPos endP = { 7.00, 0, 0, 0 };
	//UDM_Line2D(startP, endP);

	//float  dec6 = 120;
	//float picalc6 = 180 / (90 - dec6);
	//float x6 = radius *cos(3.14 / picalc6);
	//float y6 = radius *sin(3.14 / picalc6);

	//float dec7 = 300;
	//float picalc7 = 180 / (90 - dec7);
	//float x7 = radius *cos(3.14 / picalc7);
	//float y7 = radius *sin(3.14 / picalc7);

	//structUdmPos startPy2 = { x7, y7, 0.000000, 0 };
	//structUdmPos endPy2 = { x6, y6, 0.000000, 0 };
	//UDM_Line2D(startPy2, endPy2);

	//float  dec4 = 150;
	//float picalc4 = 180 / (90 - dec4);
	//float x4 = radius *cos(3.14 / picalc4);
	//float y4 = radius *sin(3.14 / picalc4);

	//float dec5 = 330;
	//float picalc5 = 180 / (90 - dec5);
	//float x5 = radius *cos(3.14 / picalc5);
	//float y5 = radius *sin(3.14 / picalc5);

	//structUdmPos startPy1 = { x5, y5, 0.000000, 0 };
	//structUdmPos endPy1 = { x4, y4, 0.000000, 0 };
	//UDM_Line2D(startPy1, endPy1);

	//UDM_Jne(0x08, nStart);	//Ê¹ÓÃR0ºÍR8¼Ä´æÆ÷×÷±È½Ï£¬²»µÈÑ­»·ÔËÐÐ£¬ÏàµÈÍË³ö£»
	//UDM_Jump(0, 0, 0);
	//UDM_EndMain();
	////¿ÉÒÔÖ±½Ó»ñµÃ´ò±êUDMÄÚ´æµØÖ·
	//UDM_GetUDMBuffer(&m_UdmBuffer, &m_nBuffSize);
	////Ò²¿ÉÒÔ½«UDMÉú³ÉÎÄ¼þ
	//UDM_SaveToFile(ps2aPath);
	//if (0 == HM_GetDeviceStatus(0) || 1 == HM_GetDeviceStatus(0))		// 设备当前状态为连接状态
	//{
	//	qDebug("The device is connected!");
	//	CString strPath;
	//	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	//	strPath.ReleaseBuffer();
	//	int pos = strPath.ReverseFind('\\');
	//	strPath = strPath.Left(pos + 1);
	//	strPath = strPath + CString("UDMGen.bin");
	//	CW2A ps2aPath((LPCWSTR)strPath);					// 当前文件夹下tem.bin文件路径
	//	//可以下载文件
	//	//		HM_DownloadStart_DDR_UDM(ps2aPath,0,this->m_hWnd,0);	// 下载bin文件

	//	//也可以将内存Buffer数据传下去
	//	int va = HM_DownloadStart_DDR_UDMBuff(m_UdmBuffer, m_nBuffSize, sd, 0);	// 下载bin文件
	//}
	//else
	//{
	//	qDebug("The device is not connected!");
	//}

	//HM_MarkStart();
}
void galvo::Scan_Range()
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



				GalvoStartx6mm1 = ((QString)scanSettingsValues[6]).toFloat();
				qDebug() << "pattern GalvoStartx6mm" << GalvoStartx6mm1;
				GalvoEndx6mm1 = ((QString)scanSettingsValues[7]).toFloat();
				qDebug() << "pattern GalvoEndx6mm" << GalvoEndx6mm1;
				GalvoStartx10mm1 = ((QString)scanSettingsValues[8]).toFloat();
				qDebug() << "pattern GalvoStartx6mm" << GalvoStartx6mm1;
				GalvoEndx10mm1= ((QString)scanSettingsValues[9]).toFloat();
				qDebug() << "pattern GalvoEndx6mm" << GalvoEndx6mm1;





				ui.lineEdit_Start_X->setText(QString::number(GalvoStartx6mm1, 'f', 4));
				ui.lineEdit_End_X->setText(QString::number(GalvoEndx6mm1, 'f', 4));
				ui.lineEdit_Start_Y->setText(QString::number(Galvo_Start_y, 'f', 4));
				ui.lineEdit_End_Y->setText(QString::number(Galvo_End_y, 'f', 4));
				ui.lineEdit_Start_Z->setText(QString::number(Galvo_Start_z, 'f', 4));
				ui.lineEdit_End_Z->setText(QString::number(Galvo_End_z, 'f', 4));
				ui.lineEdit_Start_A->setText(QString::number(Galvo_Start_a, 'f', 4));
				ui.lineEdit_End_A->setText(QString::number(Galvo_End_a, 'f', 4));
				//qDebug() << GalvoStartx6mm<< endl;

				/*float   Range_StartX = GalvoStartx6mm;
				float   Range_EndX = GalvoEndx6mm;
				qDebug() << "Range_StartX" << Range_StartX;
				qDebug() << "Range_EndX" << Range_EndX;

				QString s1 = QString::number(Range_StartX);
				QString s2 = QString::number(Range_EndX);
				qDebug() << "s1" << s1;
				qDebug() << "s2" << s2;
				Galvo_startx_6mm =  s1;
				Galvo_endx_6mm = s2;
				qDebug() << "6mm Scan range " << Galvo_startx_6mm << "    " << Galvo_endx_6mm<< endl;*/

			}
		}rowNumber = rowNumber + 1;
		qDebug() << "  rowNumber  " << rowNumber;
	}
	//qDebug() << "**************************************************Galvo_startx_6mm" << Galvo_startx_6mm << endl;
}

void galvo::setGalvoValues(int scanModeType, int HDLineScanLength, float G_X1, float G_Y1, float G_X2, float G_Y2)
{
//	PRINTFUNCTIONNAME
//		if (HDLineScanLength == 0)
//		{
//		   SetGalvoValue6mm(G_X1, G_Y1, G_X2, G_Y2);
//		}
//		else if (HDLineScanLength == 1)
//		{
//			//SetGalvoValue10mm(G_X1, G_Y1, G_X2, G_Y2);
//		}
	
}
void galvo::SetGalvoValue6mm(float  GalvoStartx6mmIn, float  GalvoStarty6mmIn, float  GalvoEndx6mmIn, float  GalvoEndy6mmIn)
{
	/*PRINTFUNCTIONNAME
	GalvoStartx6mm1 = GalvoStartx6mmIn;
	GalvoStarty6mm1 = GalvoStarty6mmIn;
	GalvoEndx6mm1 = GalvoEndx6mmIn;
	GalvoEndy6mm1 = GalvoEndy6mmIn;*/
}
void galvo::setScanLength(int HDLineScanLength)
{
	PRINTFUNCTIONNAME
	//hdLineScanLength = HDLineScanLength;
}

