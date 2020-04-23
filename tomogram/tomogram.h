#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tomogram.h"

class tomogram : public QMainWindow
{
	Q_OBJECT

public:
	tomogram(QWidget *parent = Q_NULLPTR);

private:
	Ui::tomogramClass ui;
};
