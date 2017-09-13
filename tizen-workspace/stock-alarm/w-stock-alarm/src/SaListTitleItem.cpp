/*
 * SaListTitleItem.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: devbin
 */

#include "SaListTitleItem.h"
#include <app.h>
#include <Elementary.h>

SaListTitleItem::SaListTitleItem()
{
    // TODO Auto-generated constructor stub

}

SaListTitleItem::~SaListTitleItem()
{
    // TODO Auto-generated destructor stub
}

Elm_Genlist_Item_Class* SaListTitleItem::getItemClassStatic() const
{
	static Elm_Genlist_Item_Class itcNormal = { };
	itcNormal.item_style = "stock-alarm/title.2icon";
	itcNormal.func.content_get = _glContentCb;
	itcNormal.func.del = WGENLIST_ITEM_DEL();

	return &itcNormal;
}

WGenlistItemEx::ItemType SaListTitleItem::getItemType() const
{
	return WGenlistItemEx::ItemType::TITLE;
}

Evas_Object* SaListTitleItem::_glContentCb(void *data, Evas_Object *obj, const char *part)
{
	auto self = (SaListTitleItem *)data;
	if (!strcmp(part, "elm.icon.1"))
	{
		char *resPath = app_get_resource_path();
		char imgPath[PATH_MAX] = {0, };
		if (resPath)
		{
			snprintf(imgPath, sizeof(imgPath), "%s%s", resPath, "images/gl_title_refresh.png");
			free(resPath);
		}
		Evas_Object *image = elm_image_add(obj);
		elm_image_file_set(image, imgPath, nullptr);
		evas_object_size_hint_min_set(image, 76, 76);
		evas_object_size_hint_max_set(image, 76, 76);
		evas_object_show(image);

		return image;
	}
	else if (!strcmp(part, "elm.icon.2"))
	{
		char *resPath = app_get_resource_path();
		char imgPath[PATH_MAX] = {0, };
		if (resPath)
		{
			snprintf(imgPath, sizeof(imgPath), "%s%s", resPath, "images/gl_title_add.png");
			free(resPath);
		}
		Evas_Object *image = elm_image_add(obj);
		elm_image_file_set(image, imgPath, nullptr);
		evas_object_size_hint_min_set(image, 76, 76);
		evas_object_size_hint_max_set(image, 76, 76);
		evas_object_show(image);

		return image;

	}
	return nullptr;
}

