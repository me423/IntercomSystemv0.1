#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_nNextBlockSize = 0;
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost("localhost", 8082);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
           );
    finishing = false;

    thread = NULL;
    client = NULL;
    recorder = NULL;
    m_ptxtInfo = new QTextEdit;
    m_ptxtInput = new QLineEdit;
    pcmd = new QPushButton;


    label1 = new QLabel(this);
    edit1 = new QLineEdit(this);
    edit1->setMinimumWidth(100);
    label1->setText("Host:");
    edit1->setText("1");

    //label2 = new QLabel(this);
    edit2 = new QLineEdit(this);
    //edit2->setMinimumWidth(100);
    //label2->setText("Port:");
    //edit2->setText("1024");
    edit2->setVisible(false);
    label1->setVisible(false);
    edit1->setVisible(false);



    label3 = new QLabel(this);
    edit3 = new QLineEdit(this);
    edit3->setMinimumWidth(100);
    label3->setText("Buffer time (ms):");
    edit3->setText("300");
    label3->setVisible(false);
    edit3->setVisible(false);
    checkbox = new QCheckBox(this);
    checkbox->setText("Record");

    label4 = new QLabel(this);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);
    slider->setValue(100);

    button = new QPushButton(this);
    button->setText("Answer");

    button1 = new QPushButton(this);
    button1->setText("Reject");

    open = new QPushButton(this);
    open->setText("Open the door!");

    noEntry = new QPushButton(this);
    noEntry->setText("No entry!");

    levelwidget = new LevelWidget(this);
    levelwidget->setFixedWidth(20);

    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout1 = new QVBoxLayout(widget);

    connect(slider, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_valueChanged);
    connect(checkbox, &QCheckBox::clicked, this, &MainWindow::on_checkBox_clicked);
    connect(button, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    layout1->addWidget(info = new QLabel("<H3>ROOM<H3>"));
    layout1->addWidget(m_ptxtInfo);
    layout1->addWidget(m_ptxtInput);
   // QVBoxLayout *layout1 = new QVBoxLayout();
    //layout1->addWidget(label1);
    //layout1->addWidget(edit1);
    //layout1->addWidget(label2, 1, 0);
    //layout1->addWidget(edit2, 1, 1);
    //layout1->addWidget(label3);
    //layout1->addWidget(edit3);
    //layout1->addLayout(layout1);
    //layout1->addWidget(checkbox);
    layout1->addWidget(label4);
    layout1->addWidget(slider);
    //QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    //QGridLayout *layout3 = new QGridLayout();
    //layout1->addItem(spacer);
    layout1->addWidget(button);
    layout1->addWidget(button1);
    layout1->addWidget(open);
    layout1->addWidget(noEntry);

    //layout1->addLayout(layout3);
    layout1->addWidget(levelwidget);

    //layout1->addWidget(pcmd);
    connect(m_ptxtInput, SIGNAL(returnPressed()),
            this,        SLOT(slotSendToServer())
           );
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
    connect(open, SIGNAL(clicked()), SLOT(openDoor()));
    connect(noEntry, SIGNAL(clicked()), SLOT(notEntry()));

    on_horizontalSlider_valueChanged(100);

    setCentralWidget(widget);
    checkbox->setVisible(false);
    pcmd->setVisible(false);

    //setFixedSize(minimumSizeHint());

    QMetaObject::invokeMethod(this, "level", Qt::QueuedConnection, Q_ARG(float, 0));
}

MainWindow::~MainWindow()
{

}
void MainWindow::openDoor()
{
    m_ptxtInput->setText("Yes");
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << m_ptxtInput->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
    m_ptxtInput->setText("");
}
void MainWindow::notEntry()
{
    m_ptxtInput->setText("No");
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << m_ptxtInput->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
    m_ptxtInput->setText("");
}

void MainWindow::slotSendToServer()
{

        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_2);
        out << quint16(0) << QTime::currentTime() << m_ptxtInput->text();

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        m_pTcpSocket->write(arrBlock);
        m_ptxtInput->setText("");


}
void MainWindow::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        m_ptxtInfo->append(time.toString() + " " + str);
        m_nNextBlockSize = 0;
    }
}

void MainWindow::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    m_ptxtInfo->append(strError);
}



void MainWindow::slotConnected()
{
    m_ptxtInfo->append("Received the connected() signal");
}

void MainWindow::finish()
{
    if (thread)
    {
        button->setEnabled(false);
        finishing = true;

        if (client)
            QMetaObject::invokeMethod(client, "deleteLater", Qt::QueuedConnection);
        QEventLoop loop;
        connect(thread, &QThread::destroyed, &loop, &QEventLoop::quit);
        loop.exec();

        finishing = false;
        button->setEnabled(true);
        button->setText("Connect");
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    finish();
}

void MainWindow::on_pushButton_clicked()
{
    startStop();
}

void MainWindow::startStop()
{
    if (finishing)
        return;

    if (!thread)
    {
        QString host = "localhost";

        /*if(edit1->text() == QString::number(1)){
            edit1->setText("localhost");
            host = edit1->text();
        }
        else if(edit1->text() == QString::number(2)){
            edit1->setText("192.168.0.1");
            host = edit1->text();
        }
        else
        {
            edit1->setText("Неправильный ввод");
        }*/


        quint16 port = 1024;//edit2->text().toInt();
        uint timetobuffer = 300;//edit3->text().toUInt();

        client = new Client();
        thread = new QThread();

        client->moveToThread(thread);
        connect(client, &Client::error, this, &MainWindow::error);
        connect(client, &Client::currentlevel, this, &MainWindow::level);
        connect(client, &Client::destroyed, thread, &QThread::quit);
        connect(client, &Client::destroyed, this, &MainWindow::zeropointer);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        connect(thread, &QThread::destroyed, this, &MainWindow::zeropointer);
        QMetaObject::invokeMethod(client, "start", Qt::QueuedConnection, Q_ARG(QString, host), Q_ARG(quint16, port), Q_ARG(uint, timetobuffer));
        QMetaObject::invokeMethod(client, "setVolume", Qt::QueuedConnection, Q_ARG(int, slider->value()));
        thread->start();

        edit1->setEnabled(false);
        edit2->setEnabled(false);
        edit3->setEnabled(false);
        checkbox->setEnabled(true);

        button->setText("Disconnect");
    }
    else
    {
        if (recorder)
        {
            disconnect(client, &Client::audioReady, this, &MainWindow::write);
            recorder->deleteLater();
        }

        finish();

        level(0);

        edit1->setEnabled(true);
        edit2->setEnabled(true);
        edit3->setEnabled(true);
        checkbox->setChecked(false);
        checkbox->setEnabled(false);
    }
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if (checked)
    {
        QAudioFormat format;
        QMetaObject::invokeMethod(client, "currentAudioFormat", Qt::BlockingQueuedConnection, Q_RETURN_ARG(QAudioFormat, format));

        QString filename = QFileDialog::getSaveFileName(this, "Save to wav file",
                                                        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                        "Sound (*.wav)");

        if (!filename.isEmpty())
        {
            if (!filename.endsWith(".wav"))
                filename.append(".wav");

            recorder = new AudioRecorder(filename, format, this);
            connect(recorder, &AudioRecorder::destroyed, this, &MainWindow::zeropointer);

            if (!recorder->open())
            {
                QMessageBox::warning(this, "Error", recorder->errorString());
                checkbox->setChecked(false);
                recorder->deleteLater();
                return;
            }

            if (client)
                connect(client, &Client::audioReady, this, &MainWindow::write);
        }
        else
        {
            checkbox->setChecked(false);
        }
    }
    else if (recorder)
    {
        if (client)
            disconnect(client, &Client::audioReady, this, &MainWindow::write);
        recorder->deleteLater();
    }
}

void MainWindow::level(float size)
{
    levelwidget->setlevel(size);
}

void MainWindow::write(QByteArray data)
{
    recorder->write(data);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if (client)
        QMetaObject::invokeMethod(client, "setVolume", Qt::QueuedConnection, Q_ARG(int, value));

    label4->setText(QString("Volume: %0%").arg(value));
}

void MainWindow::error(const QString &errorstr)
{
    startStop();

    QMessageBox::information(this, "Error", errorstr);
}

void MainWindow::zeropointer(QObject *object)
{
    if (object == client)
        client = NULL;
    else if (object == thread)
        thread = NULL;
    else if (object == recorder)
        recorder = NULL;
}
