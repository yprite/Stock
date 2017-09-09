/*
 * SaListEditEventListener.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: devbin
 */

#include "SaListEditEventListener.h"
#include "SaListEditEventManager.h"

SaListEditEventListener::SaListEditEventListener()
{
    // TODO Auto-generated constructor stub
    SaListEditEventManager::getInstance()->addListener(this);
}

SaListEditEventListener::~SaListEditEventListener()
{
    // TODO Auto-generated destructor stub
    SaListEditEventManager::getInstance()->removeListener(this);
}

