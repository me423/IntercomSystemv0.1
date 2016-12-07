#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "client.h"
#include "audiorecorder.h"
#include "levelwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void finish();
    void on_pushButton_clicked();
    void on_checkBox_clicked(bool checked);
    void level(float size);
    void write(QByteArray data);
    void on_horizontalSlider_valueChanged(int value);
    void error(const QString &errorstr);
    void zeropointer(QObject *object);
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void openDoor();
    void notEntry();

private:
    void startStop();
    void closeEvent(QCloseEvent *);
    QTcpSocket* m_pTcpSocket;
    bool finishing;
    quint16     m_nNextBlockSize;

    QLabel *label1;
    QLineEdit *edit1;
    QLabel *label2;
    QLineEdit *edit2;
    QLabel *label3;
    QLineEdit *edit3;
    QCheckBox *checkbox;
    QLabel *label4;
    QSlider *slider;
    QPushButton *button;
    QPushButton *button1;
    LevelWidget *levelwidget;

    Client *client;
    QThread *thread;
    AudioRecorder *recorder;
    QTextEdit* m_ptxtInfo;
    QLineEdit* m_ptxtInput;
    QPushButton* pcmd;
    QPushButton* open;
    QPushButton* noEntry;
    QLabel* info;

};

#endif // MAINWINDOW_H
