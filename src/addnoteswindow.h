#ifndef ADDNOTESWINDOW_H
#define ADDNOTESWINDOW_H

#include <QWidget>

namespace Ui {
class addnoteswindow;
}

class Addnoteswindow : public QWidget {
  Q_OBJECT

public:
  explicit Addnoteswindow(QWidget *parent = nullptr);
  Addnoteswindow(const QString &str1, const QString &str2,
                 QWidget *parent = nullptr);
  ~Addnoteswindow();

protected:
  void closeEvent(QCloseEvent *ev) override;

private:
  Ui::addnoteswindow *ui;

  // 字体初始化设置
  // void init();
  void generalInit();
  bool sendFlag_{false};
  bool recFlag{false};
  static int number;
  static int offset;
  static QString IpSave_;
  static ushort portSave_;
signals:
  void sendMessage(const QString &str1, const QString &str2, const QString &ip,
                   ushort port, bool sendFlag);
};

// int Addnoteswindow::number{0};

#endif // ADDNOTESWINDOW_H
