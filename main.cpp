#include "loginview.h"
#include <QApplication>
#include"chatting.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginView* w = new LoginView();
    w->show();

    return a.exec();
}
