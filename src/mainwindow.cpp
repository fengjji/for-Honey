
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QPixmap>

// https://linuxcpp.0voice.com/?id=90646
// https://blog.csdn.net/csdn_user23/article/details/130267247
// https://www.cnblogs.com/woniu201/p/10569883.html
// https://blog.51cto.com/hongpangzi/5225072

void RuRec::run() {
  QUdpSocket *ruRec = new QUdpSocket();
  ruRec->bind(QHostAddress(localIP_), 8179);
  QByteArray message;

  while (1) {
    while (ruRec->hasPendingDatagrams()) {
      // 动态设置大小为第一个挂起的 udp 数据包的大小
      message.resize(ruRec->pendingDatagramSize());
      /* 接收数据
       * 第一个参数：存储接收到数据
       * 第二个参数：接收的数据最大的大小
       * 返回值：成功返回接收的数据字节数，失败返回-1
       */
      int r = ruRec->readDatagram(message.data(), message.size());
      // 判断是否接收成功
      if (r == -1) {
        qDebug() << "接收数据失败";
      }

      QDataStream in(message);

      Message recvmessage;

      in >> recvmessage.head_ >> recvmessage.text_;
      emit create(recvmessage.head_, recvmessage.text_);
      // qDebug()<<recvmessage.head_<<"[[["<<recvmessage.text_;
      break;
    }
  }
}

YRu::YRu(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initUi();
  sendUdp_ = new QUdpSocket(this);
  loveRu_ = new RuRec("127.0.0.1");
  // loveRu_->start();

  //  获取本机IP
  connect(ui->login, &QPushButton::clicked, this, [=] {
    bool bOk = false;
    //   QString ip;
    QString ip = QInputDialog::getText(this, "Ru", "IP Address",
                                       QLineEdit::Normal, "", &bOk);

    if (bOk && ip != "") {
      localIP_ = ip;
    }

    loveRu_->localIP_ = ip;
    loveRu_->start();
  });

  connect(ui->addNotes, &QPushButton::clicked, this, [=] {
    Addnoteswindow *addnotes = new Addnoteswindow();
    addnotes->setWindowModality(Qt::ApplicationModal);
    // 关闭后释放资源
    addnotes->setAttribute(Qt::WA_DeleteOnClose, true);
    addnotes->show();

    connect(addnotes, &Addnoteswindow::sendMessage, this,
            [=](const QString &str1, const QString &str2, const QString &ip,
                ushort port, bool sendFlag) {
              qDebug() << str1;
              head_ = str1;
              qDebug() << str2;
              text_ = str2;
              if (sendFlag)
                sendMsgToRu(ip, port);
            });
  });

  connect(
      loveRu_, &RuRec::create, this,
      [=](const QString &head, const QString &text) {
        Addnoteswindow *addnotes = new Addnoteswindow(head, text);
        //  addnotes->setWindowModality(Qt::ApplicationModal);
        // 关闭后释放资源
        addnotes->setAttribute(Qt::WA_DeleteOnClose, true);
        addnotes->show();
      },
      Qt::BlockingQueuedConnection);

  // 槽函数区域
}

YRu::~YRu() { delete ui; }

//界面的初始化设置
// https://blog.csdn.net/Superman___007/article/details/88713914
void YRu::initUi() {

  this->setWindowTitle("Ru");
  this->setStyleSheet("QMainWindow {"
                      // " color: rgb(204, 229, 255);"
                      "background-color :rgb(255, 255, 255);}");

  ui->addNotes->setStyleSheet(
      "QPushButton{background-color: rgb(204, 229, 255);\
                                border-top-left-radius:10px;border-top-right-radius:10px;\
                                border-bottom-left-radius:10px;border-bottom-right-radius:10px;}"
      "QPushButton:hover{background-color:rgb(153, 204, 255); }");

  ui->checkNotes->setStyleSheet(
      "QPushButton{background-color: rgb(255, 204, 229);\
                                border-top-left-radius:10px;border-top-right-radius:10px;\
                                border-bottom-left-radius:10px;border-bottom-right-radius:10px;}"
      "QPushButton:hover{background-color:rgb(255, 153, 204); }");

  QPixmap image;

  image.load("./huiguniang.png");
  image.scaled(ui->label->size(), Qt::KeepAspectRatio);
  ui->label->setPixmap(image);
  ui->label->setScaledContents(true);

  this->setWindowFlags(this->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
  this->setFixedSize(225, 380);
}

void YRu::sendMsgToRu(const QString &ip, ushort port) {
  // 定义一个MyStruct类型的对象
  Message msg;
  msg.head_ = head_;
  msg.text_ = text_;

  QByteArray datagram;
  QDataStream stream(&datagram, QIODevice::WriteOnly);
  stream << msg.head_ << msg.text_;

  int w = sendUdp_->writeDatagram(datagram, QHostAddress(ip), 8179);
  // 判断是否发送成功
  if (w == -1) {
    qDebug() << "发送失败！";
  } else {
    qDebug() << "发送成功，字节数：" << w;
  }
}
