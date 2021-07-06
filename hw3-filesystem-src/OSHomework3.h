#pragma once
#pragma execution_character_set("utf-8")

//#include <QtWidgets/QMainWindow>
#include "qmainwindow.h"
#include <QStandardItemModel>
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextCodec>
#include "ui_OSHomework3.h"
#include "FSBoundary.h"

class OSHomework3 : public QMainWindow
{
    Q_OBJECT

public:
    OSHomework3(QWidget *parent = Q_NULLPTR);
    FSBoundary fb;
    QStandardItemModel* stModel = nullptr;

    void fsqRefreshWindow();
    void fsqFormat();
    void fsqNewFile();
    void fsqOpenFile();

    void fsqNewFileL();
    void fsqNewFileFolder();

    void fsqDeleteFile();
    void fsqShowPath();
    void fsqGoParentFolder();

    void fsqSave();
    void fsqRead();
    void fsqGetDevInf();

    void fsqRoute();
    void fsqUpdateInfo();

    void fsqRename();
    void fsqClose();
    void fsqGotoNewFile();
private:
    Ui::OSHomework3Class ui;
};
