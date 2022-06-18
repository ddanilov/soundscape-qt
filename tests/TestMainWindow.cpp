#include "MainWindow.h"
#include "Track.h"
#include "TrackControls.h"

#include <QTemporaryFile>
#include <QTest>

class TestMainWindow : public QObject
{
  Q_OBJECT

private slots:
  void testTrackFromMedia();
  void testSaveTracksToJson();
  void testLoadTracksFromJson();

  void testMoveTrackUp();
  void testMoveTrackDown();
};

void TestMainWindow::testTrackFromMedia()
{
  MainWindow window;
  QVERIFY(window.m_box_layout->count() == 1);
  // menu info is the first item in the box layout
  auto* menu_info = dynamic_cast<QLabel*>(window.m_box_layout->itemAt(0)->widget());
  QVERIFY(menu_info != nullptr);

  window.addTrackFromMedia(QString("/tmp/sound_01.mp3"));
  QVERIFY(window.m_box_layout->count() == 2);
  // menu info is still the first item in the box layout
  menu_info = dynamic_cast<QLabel*>(window.m_box_layout->itemAt(0)->widget());
  QVERIFY(menu_info != nullptr);
  // added track is the second item
  auto* track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(1)->widget());
  QVERIFY(track_control != nullptr);

  window.addTrackFromMedia(QString("sound_02.mp3"));
  QVERIFY(window.m_box_layout->count() == 3);
  track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(2)->widget());
  QVERIFY(track_control != nullptr);
}

void TestMainWindow::testSaveTracksToJson()
{
  QTemporaryFile file;
  file.open();
  file.write(QString("=== abc ===").toUtf8());
  file.close();
  const QFileInfo file_info(file.fileName());
  const auto& base_dir = file_info.dir().absolutePath();

  MainWindow window;
  window.addTrackFromMedia(QString(base_dir + "/sound_01.mp3"));
  window.addTrackFromMedia(QString(base_dir + "/../data1/sound_02.mp3"));
  window.addTrackFromMedia(QString(base_dir + "/../data2/sound_03.mp3"));

  auto* track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(1)->widget());
  auto* track = track_control->findChild<Track*>();
  track->setVolume(0.11);

  track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(2)->widget());
  track = track_control->findChild<Track*>();
  track->setVolume(0.21);

  track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(3)->widget());
  track = track_control->findChild<Track*>();
  track->setVolume(0.32);

  file.open();
  window.saveTracksToJson(file);
  file.close();

  file.open();
  QString json(file.readAll());
  QString jsonExpected(R"({
    "tracks": [
        {
            "fileName": "sound_01.mp3",
            "volume": 0.11
        },
        {
            "fileName": "../data1/sound_02.mp3",
            "volume": 0.21
        },
        {
            "fileName": "../data2/sound_03.mp3",
            "volume": 0.32
        }
    ]
}
)");
  QCOMPARE(json, jsonExpected);
  file.close();
}

void TestMainWindow::testLoadTracksFromJson()
{
  QTemporaryFile file;
  file.open();
  QString json(R"({
    "tracks": [
        {
            "fileName": "sound_01.mp3",
            "volume": 0.51
        },
        {
            "fileName": "../data1/sound_02.mp3",
            "volume": 0.52
        },
        {
            "fileName": "../data2/sound_03.mp3",
            "volume": 0.53
        }
    ]
}
)");
  file.write(json.toUtf8());
  file.close();

  MainWindow window;

  file.open();
  window.loadTracksFromJson(file);
  file.close();

  QVERIFY(window.m_box_layout->count() == 4);
  // menu info is the first item in the box layout
  auto* menu_info = dynamic_cast<QLabel*>(window.m_box_layout->itemAt(0)->widget());
  QVERIFY(menu_info != nullptr);
  // followed by the three tracks
  auto* track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(1)->widget());
  QVERIFY(track_control != nullptr);
  auto* track = track_control->findChild<Track*>();
  QCOMPARE(track->title(), "sound_01");
  QCOMPARE(track->volume(), 0.51);
  //
  track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(2)->widget());
  QVERIFY(track_control != nullptr);
  track = track_control->findChild<Track*>();
  QCOMPARE(track->title(), "sound_02");
  QCOMPARE(track->volume(), 0.52);
  //
  track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(3)->widget());
  QVERIFY(track_control != nullptr);
  track = track_control->findChild<Track*>();
  QCOMPARE(track->title(), "sound_03");
  QCOMPARE(track->volume(), 0.53);
}

void TestMainWindow::testMoveTrackUp()
{
  MainWindow window;

  window.addTrackFromMedia(QString("track_01.mp3"));
  window.addTrackFromMedia(QString("track_02.mp3"));
  window.addTrackFromMedia(QString("track_03.mp3"));

  auto move_track = [&](int index) {
    auto* track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(index)->widget());
    track_control->moveUp();
  };

  auto check_track = [&](int index, const QString& title) {
    auto* track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(index)->widget());
    auto* track = track_control->findChild<Track*>();
    QCOMPARE(track->title(), title);
  };

  check_track(1, "track_01");
  check_track(2, "track_02");
  check_track(3, "track_03");

  move_track(1);
  check_track(1, "track_01");
  check_track(2, "track_02");
  check_track(3, "track_03");

  move_track(2);
  check_track(1, "track_02");
  check_track(2, "track_01");
  check_track(3, "track_03");

  move_track(3);
  check_track(1, "track_02");
  check_track(2, "track_03");
  check_track(3, "track_01");
}

void TestMainWindow::testMoveTrackDown()
{
  MainWindow window;

  window.addTrackFromMedia(QString("track_01.mp3"));
  window.addTrackFromMedia(QString("track_02.mp3"));
  window.addTrackFromMedia(QString("track_03.mp3"));

  auto move_track = [&](int index) {
    auto* track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(index)->widget());
    track_control->moveDown();
  };

  auto check_track = [&](int index, const QString& title) {
    auto* track_control = dynamic_cast<TrackControls*>(window.m_box_layout->itemAt(index)->widget());
    auto* track = track_control->findChild<Track*>();
    QCOMPARE(track->title(), title);
  };

  check_track(1, "track_01");
  check_track(2, "track_02");
  check_track(3, "track_03");

  move_track(1);
  check_track(1, "track_02");
  check_track(2, "track_01");
  check_track(3, "track_03");

  move_track(2);
  check_track(1, "track_02");
  check_track(2, "track_03");
  check_track(3, "track_01");

  move_track(3);
  check_track(1, "track_02");
  check_track(2, "track_03");
  check_track(3, "track_01");
}

QTEST_MAIN(TestMainWindow)
#include "TestMainWindow.moc"
