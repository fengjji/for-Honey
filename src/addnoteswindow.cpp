#include "addnoteswindow.h"
#include "ui_addnoteswindow.h"
#include <QFont>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>


int Addnoteswindow::number = 0;
int Addnoteswindow::offset = 0;
QString Addnoteswindow::IpSave_{""};
ushort Addnoteswindow::portSave_ = 0;

Addnoteswindow::Addnoteswindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::addnoteswindow) {
  ui->setupUi(this);
  this->setWindowTitle("AddNotes");

  // init();
  generalInit();
  this->setFixedSize(395, 495);
  // connect(ui->lineEdit,&QLineEdit::textEdited,
  connect(ui->send, &QPushButton::clicked, this, [=] {
    bool bOk = false;
    //   QString ip;
    QString ip = QInputDialog::getText(this, "Ru", "IP Address",
                                       QLineEdit::Normal, IpSave_, &bOk);

    if (bOk && ip != "") {
      // auto rec = ip.split(":");
      // IpSave_ = rec[0];
      // portSave_ = (ushort)rec[1].toShort();
      qDebug() << IpSave_;
      qDebug() << portSave_;
      // qDebug()<<ip;
      IpSave_ = ip;
      if (ui->lineEdit->text() != "" &&
          ui->plainTextEdit->toPlainText() != "") {
        sendFlag_ = true;
        // qDebug()<<"..!!!!";
      }
    }
  });
}

Addnoteswindow::Addnoteswindow(const QString &str1, const QString &str2,
                               QWidget *parent)
    : QWidget(parent), ui(new Ui::addnoteswindow) {
  ui->setupUi(this);
  ++number;
  // qDebug()
  this->setWindowTitle("吉的消息... " + QString::number(number));
  this->move(50 + offset, 100 + offset);
  offset += 20; //考虑到一天的提示不是很多，就不做过多逻辑判断了

  generalInit();
  ui->send->setEnabled(false);
  ui->send->hide();
  this->setFixedSize(340, 495);
  ui->lineEdit->setText(str1);
  ui->plainTextEdit->setPlainText(str2);
  recFlag = true;
}

Addnoteswindow::~Addnoteswindow() {
  qDebug() << "......";
  delete ui;
}

void Addnoteswindow::generalInit() {

  QFont fontHead, fontText;
  fontHead.setBold(true);
  fontHead.setPointSize(18);
  fontText.setFamily("宋体");
  fontText.setPointSize(12);
  ui->lineEdit->setFont(fontHead);
  ui->plainTextEdit->setFont(fontText);

  QPixmap image;
  // image.load("F:\\Ru\\LoveRu\\send2.png");
  image.load("./send2.png");
  ui->send->setIcon(image);

  ui->lineEdit->setStyleSheet(
      "QLineEdit{border:2px solid rgb(180, 180, 180);border-radius: 2px;}");
  ui->plainTextEdit->setStyleSheet("QPlainTextEdit{border:2px solid rgb(180, "
                                   "180, 180);border-radius: 4px;}");
  ui->send->setStyleSheet(
      "QPushButton{background-color :rgb(255, 248, 220);border:2px solid "
      "rgb(180, 180, 180);border-radius: 4px;}"
      "QPushButton:hover{background-color:rgb(255, 228, 181); }");
}

void Addnoteswindow::closeEvent(QCloseEvent *ev) {
  if (recFlag) {
    ev->accept();
    return;
  }

  QMessageBox::Button btn =
      QMessageBox::question(this, "关闭新建", "是否确认保存并退出？");

  //   QMessageBox::question(this, "关闭新建", "是否确认保存并退出？");

  if (btn == QMessageBox::Yes) {

    emit sendMessage(ui->lineEdit->text(), ui->plainTextEdit->toPlainText(),
                     IpSave_, portSave_, sendFlag_);
    ev->accept();

  } else {

    ev->ignore();
  }
}

// 10.120.67.226
