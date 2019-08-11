#include "frame.h"

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

  // Instantiate main frame
  TDB::Frame frame;
  frame.show();

  return app.exec();
}
