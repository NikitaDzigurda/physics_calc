#include "ui/MainWindow.h"

#include <QApplication>
#include <clocale>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setApplicationName("Physics Calculator");
    app.setOrganizationName("physics_calc");

    // Qt picks up the system locale on init, which on non-English systems can
    // flip LC_NUMERIC to a comma decimal separator and break C-locale parsers
    // like strtod / std::ostringstream. Force the numeric locale back to "C"
    // so the expression lexer always sees "9.8" as 9.8, not 9.
    std::setlocale(LC_NUMERIC, "C");

    pc::MainWindow w;
    w.show();
    return app.exec();
}
