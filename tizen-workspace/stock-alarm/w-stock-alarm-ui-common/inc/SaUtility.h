/*
 * SaUtility.h
 *
 *  Created on: Aug 4, 2017
 *      Author: devbin
 */

#ifndef SAUTILITY_H_
#define SAUTILITY_H_

#include "WDefine.h"

class WAPP_ASSIST_EXPORT SaUtility
{
public:
    SaUtility();
    virtual ~SaUtility();

public:
    static void convertHexColorToRGBA(char const *hexCode, int *r, int *g, int *b);
};

#endif /* SAUTILITY_H_ */
