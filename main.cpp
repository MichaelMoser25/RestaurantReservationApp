#include "loginscreen.h"
#include "restaurant.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginScreen w;
    w.show();
    return a.exec();
}
