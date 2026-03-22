#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onSelectFileClicked();
    void onSelectSigFileClicked();
    void onSignFileClicked();
    void onFileSigned(const QByteArray& zipData);
    
    void onVerifyClicked();
    
    void onApiError(const QString& error);

private:
    Ui::MainWindow* ui;
    QString m_currentFilePath;
    QString m_currentPublicKey;
    QString m_currentSignature;
    QByteArray m_currentFileData;
    
    void logMessage(const QString& message);
    bool loadZipAndExtract(const QString& zipPath);
};
#endif
