#include "frame.h"
#include "state.h"

#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include <QString>
#include <QByteArray>
#include <QCommandLineParser>

#include <optional>
#include <string>

using namespace std;

extern uint8_t font_start[] asm("_binary_res_fonts_source_code_pro_tff_start");
extern uint8_t font_end[] asm("_binary_res_fonts_source_code_pro_tff_end");

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  // Load fonts
  QByteArray fontData((const char *) font_start, font_end - font_start);
  int fontId = QFontDatabase::addApplicationFontFromData(fontData);
  QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
  QFont font(fontName);
  font.setPixelSize(24);
  font.setStyleHint(QFont::Monospace);
  QApplication::setFont(font);

  // Parse ssh options
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addOptions({
    {{"s", "ssh"}, "Connect to remote host", "ssh"},
    {{"p", "port"}, "SSH port. Defaults to 22", "port"},
    {{"w", "width"}, "Graph width", "width"},
  });
  parser.process(app);

  optional<tuple<string, string, int>> remote;
  int port = 22;
  int width = 20;
  if(parser.isSet("port"))
    port = atoi(parser.value("port").toUtf8().constData());
  if(parser.isSet("width"))
    width = atoi(parser.value("width").toUtf8().constData());
  if(parser.isSet("ssh")) {
    string ssh = parser.value("ssh").toUtf8().constData();
    const auto pos = ssh.find("@");
    remote = {
      ssh.substr(pos + 1),
      ssh.substr(0, pos),
      port,
    };
  }

  // Instantiate main frame, initialize state
  TDB::Frame frame(remote, width);
  TDB::State &state = frame.state;
  frame.show();


  return app.exec();
}
