#include "restaurantapp.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// UI QSS STYLING (Tema Nobu Light & Luxury)
// Loaded dynamically from Qt resource :/main.qss
void RestaurantApp::applyStyles() {
    QFile file(":/main.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString qss = stream.readAll();
        file.close();
        qApp->setStyleSheet(qss);
    } else {
        qWarning() << "Failed to open QSS file from resources (:/main.qss)! Make sure main.qss is added to resources.";
    }
}