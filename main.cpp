#include "controller/MenuController.h"

int main() 
{
    MenuController menuController = MenuController();
    menuController.run();
    return Fl::run();
}