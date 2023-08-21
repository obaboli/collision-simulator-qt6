#include <QApplication>
#include "animation.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    Animation animation;
    animation.resize(800, 600);
    animation.show();

    return app.exec();
}
