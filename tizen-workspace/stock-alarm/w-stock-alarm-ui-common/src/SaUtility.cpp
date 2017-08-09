/*
 * SaUtility.cpp
 *
 *  Created on: Aug 4, 2017
 *      Author: devbin
 */

#include "SaUtility.h"

SaUtility::SaUtility()
{
    // TODO Auto-generated constructor stub

}

SaUtility::~SaUtility()
{
    // TODO Auto-generated destructor stub
}

void SaUtility::convertHexColorToRGBA(char const *hexCode, int *r, int *g, int *b)
{
    if (!hexCode)
        return;

    sscanf(hexCode, "#%02x%02x%02x", r, g, b);
}
