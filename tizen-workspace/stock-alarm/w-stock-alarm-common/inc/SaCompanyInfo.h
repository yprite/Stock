/*
 * SaCompanyInfo.h
 *
 *  Created on: Sep 5, 2017
 *      Author: devbin
 */

#ifndef SACOMPANYINFO_H_
#define SACOMPANYINFO_H_

#include <string>
#include "WDefine.h"

struct WAPP_ASSIST_EXPORT SaCompanyInfo
{
    std::string code;
    std::string name;
    std::string market;
    std::string price;
    std::string change;
    std::string percent;
    std::string max;
    std::string min;
    std::string volume;
    std::string previous;
    std::string histroicaldata;
};

#endif /* SACOMPANYINFO_H_ */
