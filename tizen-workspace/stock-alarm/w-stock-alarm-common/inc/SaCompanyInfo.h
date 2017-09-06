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
};

#endif /* SACOMPANYINFO_H_ */
