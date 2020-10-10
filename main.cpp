#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load application Style Sheet
    QFile file("stylesheet.qss");
    file.open(QFile::ReadOnly);
    if (file.exists())
    {
        QString stylesheet = QLatin1String(file.readAll());
        a.setStyleSheet(stylesheet);
    }

    // Load Application Settings
    Settings::LoadSettings();

    MainWindow w;
    w.show();
    return a.exec();
}
