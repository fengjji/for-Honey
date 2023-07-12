
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "addnoteswindow.h"
#include <QMainWindow>
#include <QThread>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Message {
  QString head_;
  QString text_;
};

class RuRec : public QThread {
  Q_OBJECT
public:
  RuRec() = default;
  explicit RuRec(QString ip) : localIP_(ip) {}
  void run() override;

  QString localIP_;
signals:
  void create(const QString &str1, const QString &str2);

private:
  bool breakFlag;
};

// 主窗口
// YRu
class YRu : public QMainWindow {
  Q_OBJECT
  using YRuUi = Ui::MainWindow;

public:
  YRu(QWidget *parent = nullptr);
  ~YRu();

private:
  YRuUi *ui;
  RuRec *loveRu_;

  //界面的初始化设置
  void initUi();
  void sendMsgToRu(const QString &ip, ushort port);

private:
  bool isAnchor_{false}; //是否固定
  QPoint currentPos_;
  QUdpSocket *sendUdp_;
  QString head_;
  QString text_;
  QString localIP_;
};

#endif // MAINWINDOW_H
