#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QSpinBox>
#include "settings.h"

class SettingsDialog : public QDialog
{
public:
    explicit SettingsDialog(QWidget* parent = nullptr);

private:
    void InitGui();
    void CreateFrameAlgoSettingsTab();
    void CreateAnimsSettingsTab();

private slots:
    void OnOkButtonPressed();
    void OnCancelButtonPressed();

private:
    QTabWidget* tabWidget;
    QDialogButtonBox* buttonBox;

    QSpinBox* frameInitWidth;
    QSpinBox* frameInitHeight;
    QSpinBox* frameTolerance;
    QSpinBox* frameColorTolerance;

    QSpinBox* animationFps;
};

#endif // SETTINGSDIALOG_H
