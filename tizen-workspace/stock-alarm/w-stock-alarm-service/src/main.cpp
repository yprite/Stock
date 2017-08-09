/*
 * main.cpp
 *
 *  Created on: Jul 25, 2017
 *      Author: devbin
 */

#include "SaServiceApp.h"
#include "SaServiceDebug.h"
#include "SaProvider.h"

int main(int argc, char **argv)
{
    //yql_test();
    SaProvider::getInstance()->initialize();
    SaProvider::getInstance()->getFinanceQuatesList("APPL");
    SaProvider::getInstance()->getFinanceQuatesList("GOOGL");
    //SaProvider::getInstance()->getFinanceQuatesList("APPL");
    SaServiceApp app;

    int ret = app.start(argc, argv);
    return ret;
}
