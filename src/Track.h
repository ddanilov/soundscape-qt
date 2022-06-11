#pragma once

#include <QDir>
#include <QObject>

class Track : public QObject
{
  Q_OBJECT

public:
  explicit Track(QObject* parent = nullptr);

  void fromJsonObject(const QJsonObject& json, const QDir& base_dir);

private:
  QString m_file_name;
};