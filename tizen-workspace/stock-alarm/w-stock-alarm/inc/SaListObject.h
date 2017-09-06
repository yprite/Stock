/*
 * SaListObject.h
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#ifndef SALISTOBJECT_H_
#define SALISTOBJECT_H_

#include "GenlistObject.h"

class SaListObject : public GenlistObject
{
public:
    SaListObject();
    virtual ~SaListObject();

private:
    virtual void onCreated();
    virtual void onDestroy();

    virtual void onLongpressed(Elm_Object_Item *item);
};

#endif /* SALISTOBJECT_H_ */
