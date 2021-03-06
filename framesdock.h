#ifndef FRAMESDOCK_H
#define FRAMESDOCK_H

#include <QDockWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include "frame.h"
#include "backgroundcolorlistwidget.h"

class FramesDock : public QDockWidget
{
public:
    FramesDock(const QString& title, QWidget* parent);
    void SelectFrame(int index);

private:
    void InitGui();
    void CreateFramesWidget();
    void CreateCalcFrameWidget(QWidget* framesWidget,QVBoxLayout* mainDockLayout);
    void AddFrameToList(int index);
    void ClearFramesList();

private slots:
    void OnSelectedFrameChanged(int frameIndex);
    void OnAddFrameButtonClicked();
    void OnRemoveFrameButtonClicked();
    void OnClearFramesButtonClicked();
    void OnUseAdaptiveStepCheckboxStateChanged(int state);
    void OnCalculateFrameButtonClicked();
    void OnCalculateAllFramesButtonClicked();

// Event Handlers
private:
    void OnSpriteSheetLoaded(void* data);
    void OnSelectedFrameOnImage(void* data);
    void OnToleranceValueChanged(int i);

private:
    QListWidget* framesList;
    QWidget* stepsWidget;
    BackgroundColorListWidget* bgColorListWidget;
    int tolerance;
};

#endif // FRAMESDOCK_H
