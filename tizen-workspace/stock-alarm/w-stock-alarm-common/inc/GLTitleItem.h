/*
 * GLTitleItem.h
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#ifndef GLTITLEITEM_H_
#define GLTITLEITEM_H_

#include "WDefine.h"
#include "WGenlistItemEx.h"
#include <string>

class WAPP_ASSIST_EXPORT GLTitleItem : public WGenlistItemEx
{
public:
    GLTitleItem(const std::string& text);
    virtual ~GLTitleItem();

protected:
    virtual Elm_Genlist_Item_Class* getItemClassStatic() const;
    virtual WGenlistItemEx::ItemType getItemType() const;

    static char* _glTextCb(void *data, Evas_Object *obj, const char *part);

private:
    std::string _text;
};

#endif /* GLTITLEITEM_H_ */
