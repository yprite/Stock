/*
 * main.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */


#include "SaWidget.h"
#include <Elementary.h>

int main(int argc, char **argv)
{
    SaWidget widget;

    int ret = widget.start(argc, argv);

    return ret;
}
