#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QPointer>
#include <QSystemTrayIcon>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);

public slots:
  void trayIconAction(QSystemTrayIcon::ActivationReason reason);
  void addTrack();

protected:
  void closeEvent(QCloseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

private:
  void setupTrayIcon();
  void addItemsToMenu(QMenu* menu) const;
  void windowShowOrHide();

  QPointer<QSystemTrayIcon> m_tray_icon;
  QPointer<QMenu> m_tray_menu;
  QPointer<QMenu> m_mouse_menu;
  QByteArray m_old_geometry;

  QPointer<QWidget> m_widget;
  QPointer<QVBoxLayout> m_box_layout;
  QPointer<QLabel> m_menu_info;
};