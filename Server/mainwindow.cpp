#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_nNextBlockSize = 0;
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost("localhost", 8082);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
    this,     SLOT(slotError(QAbstractSocket::SocketError))
       );
    input = NULL;
    server = NULL;
    thread = NULL;

    combobox = new QComboBox(this);

    label1 = new QLabel(this);
    label1->setText("Port:");

    edit1 = new QLineEdit(this);
    edit1->setMinimumWidth(100);
    edit1->setText("1024");

    button = new QPushButton(this);
    button->setText("Call");

    label2 = new QLabel(this);
    label2->setText("Sample size:");
    edit2 = new QLineEdit(this);
    edit2->setText("16");

    label3 = new QLabel(this);
    label3->setText("Sample rate:");
    edit3 = new QLineEdit(this);
    edit3->setText("44100");

    label4 = new QLabel(this);
    label4->setText("Channels:");
    edit4 = new QLineEdit(this);
    edit4->setText("2");

    box1 = new QGroupBox(this);
    box1->setTitle("Sample type:");
    radiobutton1 = new QRadioButton(this);
    radiobutton1->setText("Signed int");
    radiobutton2 = new QRadioButton(this);
    radiobutton2->setText("Unsigned int");

    box2 = new QGroupBox(this);
    box2->setTitle("Byte order:");
    radiobutton3 = new QRadioButton(this);
    radiobutton3->setText("Little Endian");
    radiobutton4 = new QRadioButton(this);
    radiobutton4->setText("Big Endian");

    connect(button, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    pcmd = new QPushButton;
    pcmd->setText("Send");
    first= new QPushButton("&1");
    second= new QPushButton("&2");
    third = new QPushButton("&3");
    fourth= new QPushButton("&4");
    fifth= new QPushButton("&5");
    sixth = new QPushButton("&6");
    seventh= new QPushButton("&7");
    eighth= new QPushButton("&8");
    ninth = new QPushButton("&9");
    star= new QPushButton("&*");
    zero= new QPushButton("&0");
    resh = new QPushButton("&#");
    m_ptxtInfo = new QTextEdit;
    m_ptxtInput = new QLineEdit;
    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);

    //layout->addWidget(combobox, 0, 0, 1, 2);
    //layout->addWidget(label1, 1, 0);
    QGridLayout *layout1 = new QGridLayout();
    combobox->setVisible(false);
    //layout1->addWidget(edit1, 0, 0);

    layout->addLayout(layout1, 1, 1);
    label2->setVisible(false);
    edit2->setVisible(false);
    label3->setVisible(false);
    edit3->setVisible(false);
    label4->setVisible(false);
    edit4->setVisible(false);
    radiobutton1->setVisible(false);
    radiobutton2->setVisible(false);
    radiobutton3->setVisible(false);
    radiobutton4->setVisible(false);
    box1->setVisible(false);
    box2->setVisible(false);
    edit1->setVisible(false);
    label1->setVisible(false);
    //layout->addWidget(label2, 2, 0);
    //layout->addWidget(edit2, 2, 1);
    //layout->addWidget(label3, 3, 0);
    //layout->addWidget(edit3, 3, 1);
    //layout->addWidget(label4, 4, 0);
    //layout->addWidget(edit4, 4, 1);
    //QGridLayout *layout2 = new QGridLayout(box1);
    //layout2->addWidget(radiobutton1, 0, 0);
    //layout2->addWidget(radiobutton2, 0, 1);
    //QGridLayout *layout3 = new QGridLayout(box2);
    //layout3->addWidget(radiobutton3, 0, 0);
    //layout3->addWidget(radiobutton4, 0, 1);
    //layout->addWidget(box1, 5, 0, 1, 2);
    //layout->addWidget(box2, 6, 0, 1, 2);
    layout1->addWidget(m_ptxtInfo,1,0);
    layout1->addWidget(m_ptxtInput,4,0);
    layout1->addWidget(button, 6, 0);
    layout1->addWidget(first,2,1);
    layout1->addWidget(second,2,2);
    layout1->addWidget(third,2,3);
    layout1->addWidget(fourth,4,1);
    layout1->addWidget(fifth,4,2);
    layout1->addWidget(sixth,4,3);
    layout1->addWidget(seventh,5,1);
    layout1->addWidget(eighth,5,2);
    layout1->addWidget(ninth,5,3);
    layout1->addWidget(star,6,1);
    layout1->addWidget(zero,6,2);
    layout1->addWidget(resh,6,3);
    layout1->addWidget(pcmd,5,0);
    connect(m_ptxtInput, SIGNAL(returnPressed()),
    this,    SLOT(slotSendToServer())
       );
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
    connect(first, SIGNAL(clicked()), SLOT(fst()));
    connect(second, SIGNAL(clicked()), SLOT(snd()));
    connect(third, SIGNAL(clicked()), SLOT(thd()));
    connect(fourth, SIGNAL(clicked()), SLOT(fth()));
    connect(fifth, SIGNAL(clicked()), SLOT(ffth()));
    connect(sixth, SIGNAL(clicked()), SLOT(sth()));
    connect(seventh, SIGNAL(clicked()), SLOT(snth()));
    connect(eighth, SIGNAL(clicked()), SLOT(eth()));
    connect(ninth, SIGNAL(clicked()), SLOT(nth()));
    connect(star, SIGNAL(clicked()), SLOT(staar()));
    connect(zero, SIGNAL(clicked()), SLOT(zeero()));
    connect(resh, SIGNAL(clicked()), SLOT(reesh()));


    radiobutton1->setChecked(true);
    radiobutton3->setChecked(true);

    getDevInfo();

    setCentralWidget(widget);

    //setFixedSize(minimumSizeHint());
}

MainWindow::~MainWindow()
{

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
void MainWindow::fst()
{
    m_ptxtInput->setText("1");
}

void MainWindow::snd()
{
     m_ptxtInput->setText("2");
}
void MainWindow::thd()
{
     m_ptxtInput->setText("3");
}
void MainWindow::fth()
{
     m_ptxtInput->setText("4");
}
void MainWindow::ffth()
{
     m_ptxtInput->setText("5");
}
void MainWindow::sth()
{
     m_ptxtInput->setText("6");
}
void MainWindow::snth()
{
     m_ptxtInput->setText("7");
}
void MainWindow::eth()
{
     m_ptxtInput->setText("8");
}


void MainWindow::nth()
{
     m_ptxtInput->setText("9");
}
void MainWindow::staar()
{
     m_ptxtInput->setText("*");
}
void MainWindow::zeero()
{
     m_ptxtInput->setText("0");
}

void MainWindow::reesh()
{
     m_ptxtInput->setText("#");
}



void MainWindow::finish()
{
    if (thread)
    {
    if (input)
    QMetaObject::invokeMethod(input, "deleteLater", Qt::QueuedConnection);
    QEventLoop loop;
    connect(thread, &QThread::destroyed, &loop, &QEventLoop::quit);
    loop.exec();
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    finish();
}

void MainWindow::getDevInfo()
{
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    if (!devices.isEmpty())
    {
    for (int i = 0; i < devices.size(); i++)
    combobox->addItem(devices.at(i).deviceName(), qVariantFromValue(devices.at(i)));
    }
    else
    {
    QMessageBox::warning(this, "Error", "No input device found!");
    }
}

void MainWindow::on_pushButton_clicked()
{
    startStop();
}

void MainWindow::startStop()
{
    if (!thread)
    {
    quint16 port = 1024;//edit1->text().toInt();

    QAudioDeviceInfo devinfo = combobox->itemData(combobox->currentIndex()).value<QAudioDeviceInfo>();

    thread = new QThread();
    input = new AudioInput();
    server = new Server();
    input->moveToThread(thread);
    server->moveToThread(thread);
    connect(input, &AudioInput::adjustSettings, this, &MainWindow::adjustSettings);
    connect(input, &AudioInput::error, this, &MainWindow::error);
    connect(input, &AudioInput::dataReady, server, &Server::writeData);
    connect(input, &AudioInput::destroyed, thread, &QThread::quit);
    connect(input, &AudioInput::destroyed, this, &MainWindow::zeropointer);
    connect(server, &Server::destroyed, this, &MainWindow::zeropointer);
    connect(server, &Server::error, this, &MainWindow::error);
    connect(thread, &QThread::finished, server, &Server::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::destroyed, this, &MainWindow::zeropointer);
    connect(thread, &QThread::started, this, &MainWindow::started);

    QMetaObject::invokeMethod(input, "start", Qt::QueuedConnection,
              Q_ARG(QAudioDeviceInfo, devinfo),
              Q_ARG(int, edit2->text().toInt()),
              Q_ARG(int, edit3->text().toInt()),
              Q_ARG(int, edit4->text().toInt()),
              Q_ARG(int, (int)(radiobutton1->isChecked() ?
                       QAudioFormat::SignedInt : QAudioFormat::UnSignedInt)),
              Q_ARG(int, (int)(radiobutton3->isChecked() ?
                       QAudioFormat::LittleEndian : QAudioFormat::BigEndian)));

    QMetaObject::invokeMethod(server, "start", Qt::QueuedConnection, Q_ARG(quint16, port));

    thread->start();

    combobox->setEnabled(false);
    edit1->setEnabled(false);
    edit2->setEnabled(false);
    edit3->setEnabled(false);
    edit4->setEnabled(false);
    box1->setEnabled(false);
    box2->setEnabled(false);

    button->setText("End Call");
    }
    else
    {
    button->setEnabled(false);
    finish();
    button->setEnabled(true);

    combobox->setEnabled(true);
    edit1->setEnabled(true);
    edit2->setEnabled(true);
    edit3->setEnabled(true);
    edit4->setEnabled(true);
    box1->setEnabled(true);
    box2->setEnabled(true);

    button->setText("Call");
    }
}

void MainWindow::error(const QString &errorstr)
{
    startStop();

    QMessageBox::information(this, "Error", errorstr);
}

void MainWindow::started()
{
    QByteArray header;
    QMetaObject::invokeMethod(input, "header", Qt::BlockingQueuedConnection, Q_RETURN_ARG(QByteArray, header));
    QMetaObject::invokeMethod(server, "setHeader", Qt::BlockingQueuedConnection, Q_ARG(QByteArray, header));
}

void MainWindow::adjustSettings(const QAudioFormat &format)
{
    edit2->setText(QString::number(format.sampleSize()));
    edit3->setText(QString::number(format.sampleRate()));
    edit4->setText(QString::number(format.channelCount()));

    if (format.sampleType() == QAudioFormat::SignedInt)
    radiobutton1->setChecked(true);
    else
    radiobutton2->setChecked(true);

    if (format.byteOrder() == QAudioFormat::LittleEndian)
    radiobutton3->setChecked(true);
    else
    radiobutton4->setChecked(true);

    QMessageBox::information(this, "Info", "The audio setting are not accepted by the input device and must be adjusted!");
}

void MainWindow::zeropointer(QObject *object)
{
    if (object == input)
    input = NULL;
    else if (object == server)
    server = NULL;
    else if (object == thread)
    thread = NULL;
}
