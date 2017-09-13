/*
 * SaListTitleItem.h
 *
 *  Created on: Sep 11, 2017
 *      Author: devbin
 */

#ifndef SALISTTITLEITEM_H_
#define SALISTTITLEITEM_H_

#include "WGenlistItemEx.h"

class SaListTitleItem : public WGenlistItemEx
{
public:
    SaListTitleItem();
    virtual ~SaListTitleItem();

protected:
    virtual Elm_Genlist_Item_Class* getItemClassStatic() const;
    virtual WGenlistItemEx::ItemType getItemType() const;

private:
    static Evas_Object* _glContentCb(void *data, Evas_Object *obj, const char *part);
};

#endif /* SALISTTITLEITEM_H_ */
