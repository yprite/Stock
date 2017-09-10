/*
 * SaCompanyListCompanyItem.h
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#ifndef SACOMPANYLISTCOMPANYITEM_H_
#define SACOMPANYLISTCOMPANYITEM_H_

#include "WGenlistItem.h"
#include "SaCompanyInfo.h"
#include <string>

class SaCompanyListCompanyItem : public app_assist::WGenlistItem
{
public:
    SaCompanyListCompanyItem(const SaCompanyInfo& companyInfo);
    virtual ~SaCompanyListCompanyItem();

    void getCompanyInfo(SaCompanyInfo& companyInfo);
protected:
    virtual Elm_Genlist_Item_Class* getItemClassStatic() const;

private:
    static char* _glTextCb(void *data, Evas_Object *obj, const char *part);

private:
    SaCompanyInfo _companyInfo;
};

#endif /* SACOMPANYLISTCOMPANYITEM_H_ */
