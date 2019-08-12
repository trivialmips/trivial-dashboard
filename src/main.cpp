#include "frame.h"
#include "state.h"

#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include <QString>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  // Load fonts
  int fontId = QFontDatabase::addApplicationFont("./res/fonts/source-code-pro.tff");
  QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
  QFont font(fontName);
  font.setPixelSize(24);
  font.setStyleHint(QFont::Monospace);
  QApplication::setFont(font);

  // Instantiate main frame, initialize state
  TDB::Frame frame;
  TDB::State &state = frame.state;
  state.open_local_session();
  state.start_timer();
  frame.show();


  return app.exec();
}
