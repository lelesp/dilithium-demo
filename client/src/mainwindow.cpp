#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "apiclient.h"
#include "crypto.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>

#include <zip.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->algorithmCombo->addItems({"ML-DSA-44", "ML-DSA-65", "ML-DSA-87"});
    ui->algorithmCombo->setCurrentText(Crypto::ALGORITHM);

    connect(ui->selectFileBtn, &QPushButton::clicked, this, &MainWindow::onSelectFileClicked);
    connect(ui->signFileBtn, &QPushButton::clicked, this, &MainWindow::onSignFileClicked);
    connect(ui->selectSigFileBtn, &QPushButton::clicked, this, &MainWindow::onSelectSigFileClicked);
    connect(ui->verifyBtn, &QPushButton::clicked, this, &MainWindow::onVerifyClicked);

    logMessage("ML-DSA Client initialized");
    logMessage("Default algorithm: " + Crypto::ALGORITHM);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::logMessage(const QString& message) {
    ui->logText->append(message);
}

void MainWindow::onSelectFileClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select File to Sign");
    if (!filePath.isEmpty()) {
        m_currentFilePath = filePath;
        QFileInfo info(filePath);
        ui->selectedFileLabel->setText(info.fileName());
        logMessage("File selected: " + filePath);
    }
}

void MainWindow::onSignFileClicked() {
    if (m_currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a file first");
        return;
    }
    
    QString serverUrl = ui->serverUrlEdit->text();
    ApiClient* client = new ApiClient(serverUrl, this);
    
    connect(client, &ApiClient::fileSigned, this, &MainWindow::onFileSigned);
    connect(client, &ApiClient::errorOccurred, this, &MainWindow::onApiError);

    const QString algorithm = ui->algorithmCombo->currentText();
    logMessage("Sending file for signing (" + algorithm + ")...");
    client->signFile(m_currentFilePath, algorithm);
}

void MainWindow::onFileSigned(const QByteArray& zipData) {
    QString savePath = QFileDialog::getSaveFileName(
        this, 
        "Save Signed Package",
        "signed_package.zip",
        "ZIP Files (*.zip)"
    );
    
    if (!savePath.isEmpty()) {
        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(zipData);
            file.close();
            logMessage("Signed package saved: " + savePath);
            QMessageBox::information(this, "Success", "File signed successfully!\nSaved to: " + savePath);
        } else {
            logMessage("Error: Could not save file");
        }
    }
}

void MainWindow::onSelectSigFileClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Select Signed ZIP",
        QString(),
        "ZIP Files (*.zip)"
    );
    
    if (!filePath.isEmpty()) {
        if (loadZipAndExtract(filePath)) {
            ui->sigFileLabel->setText(QFileInfo(filePath).fileName());
            logMessage("ZIP loaded: " + filePath);
        }
    }
}

bool MainWindow::loadZipAndExtract(const QString& zipPath) {
    int error;
    zip_t* zip = zip_open(zipPath.toStdString().c_str(), ZIP_RDONLY, &error);
    
    if (!zip) {
        logMessage("Error: Cannot open ZIP file, error code: " + QString::number(error));
        return false;
    }
    
    zip_int64_t numEntries = zip_get_num_entries(zip, 0);
    logMessage("ZIP entries count: " + QString::number(numEntries));
    
    QByteArray fileData;
    QByteArray sigData;
    QByteArray pubKeyData;
    QString fileName;
    
    for (zip_int64_t i = 0; i < numEntries; i++) {
        const char* name = zip_get_name(zip, i, 0);
        if (!name) continue;
        
        QString fname = QString::fromUtf8(name);
        logMessage("Processing: " + fname);
        
        if (fname.contains('/')) continue;
        
        zip_file_t* zf = zip_fopen_index(zip, i, 0);
        if (!zf) continue;
        
        zip_stat_t st;
        zip_stat_index(zip, i, 0, &st);
        
        QByteArray data;
        if (st.size > 0) {
            data.resize(st.size);
            zip_int64_t bytesRead = zip_fread(zf, data.data(), st.size);
            if (bytesRead != st.size) {
                data.resize(bytesRead);
            }
        }
        zip_fclose(zf);
        
        if (fname.endsWith(".sig")) {
            sigData = data;
            logMessage("  -> Found .sig file");
        } else if (fname.endsWith(".pub")) {
            pubKeyData = data;
            logMessage("  -> Found .pub file");
        } else {
            fileName = fname;
            fileData = data;
            logMessage("  -> Found original file, size: " + QString::number(data.size()));
        }
    }
    
    zip_close(zip);
    
    if (fileData.isEmpty() || sigData.isEmpty() || pubKeyData.isEmpty()) {
        logMessage("Error: Invalid ZIP format - missing files");
        logMessage("  File: " + QString(fileData.isEmpty() ? "MISSING" : "OK"));
        logMessage("  .sig: " + QString(sigData.isEmpty() ? "MISSING" : "OK"));
        logMessage("  .pub: " + QString(pubKeyData.isEmpty() ? "MISSING" : "OK"));
        return false;
    }
    
    m_currentFileData = fileData;
    m_currentSignature = QString::fromLatin1(sigData);
    m_currentPublicKey = QString::fromLatin1(pubKeyData);
    
    ui->pubKeyLabel->setText("Public Key: " + m_currentPublicKey.left(32) + "...");
    ui->sigFileLabel->setText("Loaded: " + fileName);
    
    logMessage("Signature verified:");
    logMessage("  Original file: " + fileName);
    logMessage("  File size: " + QString::number(m_currentFileData.size()) + " bytes");
    logMessage("  Sig size: " + QString::number(m_currentSignature.length()) + " hex chars");
    
    return true;
}

void MainWindow::onVerifyClicked() {
    if (m_currentFileData.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please load a signed ZIP first");
        return;
    }
    
    if (m_currentPublicKey.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No public key loaded");
        return;
    }
    
    if (m_currentSignature.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No signature data loaded");
        return;
    }
    
    logMessage("Verifying signature...");
    logMessage("  File size: " + QString::number(m_currentFileData.size()) + " bytes");

    const int sigBytesLen = Crypto::hexToBytes(m_currentSignature).size();
    const QString detected = Crypto::detectAlgorithm(sigBytesLen);
    if (detected.isEmpty()) {
        logMessage("  Algorithm: UNKNOWN (signature is " + QString::number(sigBytesLen) + " bytes)");
    } else {
        logMessage("  Algorithm (auto-detected): " + detected);
    }

    bool isValid = Crypto::verifySignature(
        m_currentPublicKey.toLatin1(),
        m_currentFileData,
        m_currentSignature.toLatin1()
    );
    
    if (isValid) {
        logMessage("VERIFICATION SUCCESSFUL!");
        QMessageBox::information(this, "Verification Result", 
            "Signature is VALID!\n\nThe file was signed with the corresponding private key.");
    } else {
        logMessage("VERIFICATION FAILED!");
        QMessageBox::warning(this, "Verification Result",
            "Signature is INVALID!\n\nThe file may have been tampered with.");
    }
}

void MainWindow::onApiError(const QString& error) {
    logMessage("ERROR: " + error);
    QMessageBox::critical(this, "API Error", error);
}
