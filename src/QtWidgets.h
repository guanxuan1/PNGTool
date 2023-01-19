#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgets.h"
#include <string>
#include <windows.h>//setpixel(....rgb)
#include <QPainter>
#include <QWidget>
#include <iostream>
#include "Pic_data.h"
#include <QFileDialog>
#include <QMenu>
#include<thread>
#include <io.h>
#include <QMessageBox>
using namespace std;

static void FlipImage(Pic_data*, bool, int, int);
class QtWidgets : public QMainWindow
{
	Q_OBJECT

public:
	QtWidgets(QWidget* parent = nullptr);
	~QtWidgets();
	void InitImage();
	void ShowImage(QImage*);
	std::string Path;
	std::string OutPath;
	Pic_data* p;
	QImage* image;
	QAction* actionIpSet;
	QAction* actionAuthorize;
	QAction* actionSaveas;
	QString SelectFile_Info();
	QString SelectFile_out();
	int ncolor;
	//string imagename;
	bool useThread = false;
	void Initmenu();
	void SetPixelColor();
	//void GetImagesName();
	int WritePNGFile(const char*);

private slots:
	void slot_action_triggered(QAction* action);
public:
	void FlipImageth();
	/*void on_pushButton1_clicked();
	void on_pushButton2_clicked();*/
private:
	Ui::QtWidgetsClass* ui;
};
