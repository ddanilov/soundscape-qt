#pragma once

#include <QDir>
#include <QMediaPlayer>
#include <QObject>
#include <QPointer>

class Player;

class Track : public QObject
{
  Q_OBJECT

public:
  explicit Track(QObject* parent = nullptr);

  void fromJsonObject(const QJsonObject& json, const QDir& base_dir);
  QJsonObject toJsonObject(const QDir& base_dir) const;

  QString title() const;
  QString fileName() const;

  double volume() const;
  void setVolume(double volume);
  float fadeVolume(qint64 position) const;

  bool isPlaying() const;
  void play();
  void pause();

  const QList<QString>& errors() const;

signals:
  void loaded();
  void errorOccurred();

private slots:
  void playerLoaded();
  void playerErrorOccurred(QMediaPlayer::Error error, const QString& error_string);

private:
  float fade(qint64 position) const;

  QPointer<Player> m_player;

  QString m_file_name;
  double m_volume;
  bool m_playing;
  qint64 m_track_duration;
  qint64 m_fade_in_duration;
  qint64 m_fade_out_duration;

  QList<QString> m_errors;

  friend class TestTrack;
};
