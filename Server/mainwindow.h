#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "audioinput.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void finish();
    void getDevInfo();
    void on_pushButton_clicked();
    void startStop();
    void error(const QString &errorstr);
    void started();
    void adjustSettings(const QAudioFormat &format);
    void zeropointer(QObject *object);
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(                            );
    void slotConnected   (                            );
    void fst           (                            );
    void snd          (                            );
    void thd           (                            );
    void fth          (                            );
    void ffth           (                            );
    void sth           (                            );
    void snth         (                            );
    void eth          (                            );
    void nth           (                            );
    void staar            (                            );
    void zeero            (                            );
    void reesh            (                            );

private:
    void closeEvent(QCloseEvent *);
    QTcpSocket* m_pTcpSocket;
    quint16     m_nNextBlockSize;

    QComboBox *combobox;
    QLabel *label1;
    QLineEdit *edit1;
    QPushButton *button;
    QLabel *label2;
    QLineEdit *edit2;
    QLabel *label3;
    QLineEdit *edit3;
    QLabel *label4;
    QLineEdit *edit4;
    QGroupBox *box1;
    QGroupBox *box2;
    QRadioButton *radiobutton1;
    QRadioButton *radiobutton2;
    QRadioButton *radiobutton3;
    QRadioButton *radiobutton4;

    QThread *thread;
    AudioInput *input;
    Server *server;
    QPushButton *pcmd;
    QTextEdit *m_ptxtInfo;
    QLineEdit *m_ptxtInput;
    QPushButton* first;
    QPushButton* second;
    QPushButton* third;
    QPushButton* fourth;
    QPushButton* fifth;
    QPushButton* sixth;
    QPushButton* seventh;
    QPushButton* eighth;
    QPushButton* ninth;
    QPushButton* star;
    QPushButton* zero;
    QPushButton* resh;
};

#endif // MAINWINDOW_H
