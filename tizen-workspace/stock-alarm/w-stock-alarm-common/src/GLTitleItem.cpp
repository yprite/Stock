/*
 * GLTitleItem.cpp
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#include "GLTitleItem.h"

GLTitleItem::GLTitleItem(const std::string& text)
{
    // TODO Auto-generated constructor stub
    _text = text;
}

GLTitleItem::~GLTitleItem()
{
    // TODO Auto-generated destructor stub
}

Elm_Genlist_Item_Class* GLTitleItem::getItemClassStatic() const
{
    static Elm_Genlist_Item_Class itcNormal = { };

    itcNormal.item_style = "title";
    itcNormal.func.text_get = _glTextCb;
    itcNormal.func.del = WGENLIST_ITEM_DEL();

    return &itcNormal;
}

WGenlistItemEx::ItemType GLTitleItem::getItemType() const
{
    return WGenlistItemEx::ItemType::TITLE;
}

char* GLTitleItem::_glTextCb(void *data, Evas_Object *obj, const char *part)
{
    auto self = (GLTitleItem *)data;

    return strdup(self->_text.c_str());
}
