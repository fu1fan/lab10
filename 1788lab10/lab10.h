#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab10.h"
#include "mainWindow.h"
#include "common.h"

class lab10 : public QMainWindow
{
    Q_OBJECT

public:
    lab10(QWidget *parent = nullptr);
    ~lab10();

private:
    Ui::lab10Class ui;
    mainWindow* main_window;
public slots:
    void login_slot();
};
