/*
 * SaListStockInfoItem.h
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#ifndef SALISTSTOCKINFOITEM_H_
#define SALISTSTOCKINFOITEM_H_

#include "WGenlistItemEx.h"
#include "SaCompanyInfo.h"

class SaListStockInfoItem : public WGenlistItemEx
{
public:
    SaListStockInfoItem(const SaCompanyInfo& info);
    virtual ~SaListStockInfoItem();

    SaCompanyInfo getCompanyInfo() const;

protected:
    virtual Elm_Genlist_Item_Class* getItemClassStatic() const;
    virtual WGenlistItemEx::ItemType getItemType() const;

private:
    static char* _glTextCb(void *data, Evas_Object *obj, const char *part);

private:
    SaCompanyInfo _companyInfo;
};

#endif /* SALISTSTOCKINFOITEM_H_ */
