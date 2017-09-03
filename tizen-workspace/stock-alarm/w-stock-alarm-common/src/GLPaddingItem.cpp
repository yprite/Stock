/*
 * GLPaddingItem.cpp
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#include "GLPaddingItem.h"

using namespace app_assist;

GLPaddingItem::GLPaddingItem()
{
    // TODO Auto-generated constructor stub

}

GLPaddingItem::~GLPaddingItem()
{
    // TODO Auto-generated destructor stub
}

Elm_Genlist_Item_Class* GLPaddingItem::getItemClassStatic() const
{
    static Elm_Genlist_Item_Class itcNormal = { };

    itcNormal.item_style = "padding";
    itcNormal.homogeneous = EINA_TRUE;
    itcNormal.func.del = WGENLIST_ITEM_DEL();

    return &itcNormal;
}

WGenlistItemEx::ItemType GLPaddingItem::getItemType() const
{
    return WGenlistItemEx::ItemType::PADDING;
}
