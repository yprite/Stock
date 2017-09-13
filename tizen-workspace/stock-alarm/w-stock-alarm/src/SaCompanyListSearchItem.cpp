/*
 * SaCompanyListSearchItem.cpp
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#include "SaCompanyListSearchItem.h"

SaCompanyListSearchItem::SaCompanyListSearchItem()
{
    // TODO Auto-generated constructor stub

}

SaCompanyListSearchItem::~SaCompanyListSearchItem()
{
    // TODO Auto-generated destructor stub
}

Elm_Genlist_Item_Class* SaCompanyListSearchItem::getItemClassStatic() const
{
    static Elm_Genlist_Item_Class itcNormal = { };

    itcNormal.item_style = "stock-alarm/search";
    itcNormal.func.del = WGENLIST_ITEM_DEL();

    return &itcNormal;
}
