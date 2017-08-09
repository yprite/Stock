/*
 * main.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#include "SaApp.h"

int main(int argc, char **argv)
{
    SaApp app;

    int ret = app.start(argc, argv);
    return ret;
}

