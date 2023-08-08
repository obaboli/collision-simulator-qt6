#include <QApplication>
#include "Animation.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    Animation animation;
    animation.show();

    return app.exec();
}
