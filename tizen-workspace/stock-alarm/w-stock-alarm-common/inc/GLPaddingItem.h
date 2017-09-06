/*
 * GLPaddingItem.h
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#ifndef GLPADDINGITEM_H_
#define GLPADDINGITEM_H_

#include "WDefine.h"
#include "WGenlistItemEx.h"

class WAPP_ASSIST_EXPORT GLPaddingItem : public WGenlistItemEx
{
public:
    GLPaddingItem();
    virtual ~GLPaddingItem();

protected:
    virtual Elm_Genlist_Item_Class* getItemClassStatic() const;
    virtual WGenlistItemEx::ItemType getItemType() const;
};

#endif /* GLPADDINGITEM_H_ */
