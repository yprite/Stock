/*
 * SaCompanyListSearchItem.h
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#ifndef SACOMPANYLISTSEARCHITEM_H_
#define SACOMPANYLISTSEARCHITEM_H_

#include "WGenlistItem.h"

class SaCompanyListSearchItem : public app_assist::WGenlistItem
{
public:
    SaCompanyListSearchItem();
    virtual ~SaCompanyListSearchItem();

protected:
    virtual Elm_Genlist_Item_Class* getItemClassStatic() const;
};

#endif /* SACOMPANYLISTSEARCHITEM_H_ */
