#include "myserver.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>

MyServer::MyServer(int nPort, QWidget* pwgt /*=0*/) : QWidget(pwgt)
                                                    , m_nNextBlockSize(0)
{
       m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + m_ptcpServer->errorString()
                             );
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );

    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);

}

/*virtual*/ void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );
    listClients.push_back(pClientSocket);
    QList<QTcpSocket*>::iterator j;
    /*for (j = listClients.begin(); j != listClients.end(); j++)
    {
        sendToClient(*j, QString::number(listClients.size()));
    }*/


    if(listClients.size()>1)
    {
        sendToClient(pClientSocket, "Room №" + QString::number(listClients.size()-1));
        sendToClient(listClients.at(0),"Room №" + QString::number(listClients.size()-1));
    }
}

void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);

    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        QString strMessage =
            time.toString() + " " + "Query has sended - " +str;

        m_ptxt->append(strMessage);

        m_nNextBlockSize = 0;

        if(pClientSocket == listClients.at(0))
        {
            sendToClient(listClients.at(str.toInt()),

                     "Open the door, please [Yes\\No] \"" + str + "\""
                    );
            continue;
        }

        if(pClientSocket != listClients.at(0) && str == "Yes")
        {
            sendToClient(listClients.at(0),

                         "Door is opened! \"" + str + "\""
                        );
            continue;

        }
        else if (pClientSocket != listClients.at(0) && str == "No")
        {
            sendToClient(listClients.at(0),

                         "No entry! \"" + str + "\""
                        );
            continue;
        }
        else if (pClientSocket != listClients.at(0))
        {
            sendToClient(listClients.at(0),

                         "Failed system! \"" + str + "\""
                        );
        }
    }
}


void MyServer::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

