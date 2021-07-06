#include "OSHomework3.h"
#include <QtWidgets/QApplication>

#include"FSOperation.h"
#include<iostream>
# pragma execution_character_set("utf-8")
using namespace std;


int main(int argc, char *argv[])
{
    
    cout << hex;
    QFont f("NSimSun", 9);
    QApplication a(argc, argv);
    a.setFont(f);
    OSHomework3 w;
    w.show();
    return a.exec();

    return 0;
}
