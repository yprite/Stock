/*
 * SaCompanyListCompanyItem.cpp
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#include "SaCompanyListCompanyItem.h"

SaCompanyListCompanyItem::SaCompanyListCompanyItem(const SaCompanyInfo& companyInfo)
{
    // TODO Auto-generated constructor stub
    _companyInfo = companyInfo;
}

SaCompanyListCompanyItem::~SaCompanyListCompanyItem()
{
    // TODO Auto-generated destructor stub
}

Elm_Genlist_Item_Class* SaCompanyListCompanyItem::getItemClassStatic() const
{
    static Elm_Genlist_Item_Class itcNormal = { };

    itcNormal.item_style = "2text";
    itcNormal.func.text_get = _glTextCb;
    itcNormal.func.del = WGENLIST_ITEM_DEL();

    return &itcNormal;
}

char* SaCompanyListCompanyItem::_glTextCb(void *data, Evas_Object *obj, const char *part)
{
    auto self = (SaCompanyListCompanyItem *)data;

    if (!strcmp(part, "elm.text"))
        return strdup(self->_companyInfo.name.c_str());
    else if (!strcmp(part, "elm.text.1"))
        return strdup(self->_companyInfo.market.c_str());
    else
        return strdup(part);
}


