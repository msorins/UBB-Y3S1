#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setupButtonsSlots();
}


void MainWindow::setupButtonsSlots() {
    QObject::connect(this->ui->encryptPushButton, SIGNAL(clicked()), this, SLOT(doEncrypt()));
    QObject::connect(this->ui->decryptPushButton, SIGNAL(clicked()), this, SLOT(doDecrypt()));
}


void MainWindow::doEncrypt() {
    string inputText = this->ui->inputTextEdit->toPlainText().toStdString();
    string encrypted = this->ui->rsaText.encrypt(inputText);

    this->ui->outputLabel->setText( '"' + QString::fromStdString(encrypted) + '"' );

    // Testing
    cout << "Decrypting: <" << encrypted << ">\n";
    cout << "Result of decryption is: " << this->ui->rsaText.decrypt(encrypted) << "\n";
}

void MainWindow::doDecrypt() {
    string inputText = this->ui->inputTextEdit->toPlainText().toStdString();
    string decrypted = this->ui->rsaText.decrypt(inputText);
    this->ui->outputLabel->setText( '"' +  QString::fromStdString(decrypted) + '"' );
}

MainWindow::~MainWindow()
{
    delete ui;
}
