/*
 * SaListStockInfoItem.cpp
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#include "SaListStockInfoItem.h"

SaListStockInfoItem::SaListStockInfoItem(const SaCompanyInfo& info)
{
    // TODO Auto-generated constructor stub
    _companyInfo = info;
}

SaListStockInfoItem::~SaListStockInfoItem()
{
    // TODO Auto-generated destructor stub
}

Elm_Genlist_Item_Class* SaListStockInfoItem::getItemClassStatic() const
{
    static Elm_Genlist_Item_Class itcNormal = { };

    itcNormal.item_style = "2text";
    itcNormal.func.text_get = _glTextCb;
    itcNormal.func.del = WGENLIST_ITEM_DEL();

    return &itcNormal;
}

WGenlistItemEx::ItemType SaListStockInfoItem::getItemType() const
{
    return WGenlistItemEx::ItemType::NORMAL;
}

char* SaListStockInfoItem::_glTextCb(void *data, Evas_Object *obj, const char *part)
{
    auto self = (SaListStockInfoItem *)data;

    if (!strcmp(part, "elm.text"))
        return strdup(self->_companyInfo.name.c_str());
    else if (!strcmp(part, "elm.text.1"))
        return strdup(self->_companyInfo.market.c_str());
    else
        return strdup(part);
    return strdup("test");
}
