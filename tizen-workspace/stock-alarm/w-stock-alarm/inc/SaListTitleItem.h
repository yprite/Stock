/*
 * SaListTitleItem.h
 *
 *  Created on: Sep 11, 2017
 *      Author: devbin
 */

#ifndef SALISTTITLEITEM_H_
#define SALISTTITLEITEM_H_

#include "WGenlistItemEx.h"
#include <functional>

class SaListTitleItem : public WGenlistItemEx
{
public:
    SaListTitleItem();
    virtual ~SaListTitleItem();

    void setOnRefreshButtonClicked(const std::function<void(void)>& refreshButtonClickedCb);
    void setOnAddButtonClicked(const std::function<void(void)>& addButtonClickedCb);

protected:
    virtual Elm_Genlist_Item_Class* getItemClassStatic() const;
    virtual WGenlistItemEx::ItemType getItemType() const;

private:
    static Evas_Object* _glContentCb(void *data, Evas_Object *obj, const char *part);

private:
    std::function<void(void)> _refreshButtonClickedCb;
    std::function<void(void)> _addButtonClickedCb;
};

#endif /* SALISTTITLEITEM_H_ */
