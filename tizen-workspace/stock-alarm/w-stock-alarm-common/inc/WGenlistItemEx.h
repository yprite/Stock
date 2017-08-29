/*
 * WGenlistItemEx.h
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#ifndef _WGENLIST_ITEM_EX_H_
#define _WGENLIST_ITEM_EX_H_

#include "WGenlistItem.h"

class WAPP_ASSIST_EXPORT WGenlistItemEx: public app_assist::WGenlistItem
{
public:
    enum class ItemType: unsigned
    {
        TITLE,
        GROUP,
        NORMAL,
        PADDING
    };

public:
    WGenlistItemEx();
    virtual ~WGenlistItemEx();

    virtual WGenlistItemEx::ItemType getItemType() const;

};



#endif /* _WGENLIST_ITEM_EX_H_ */
