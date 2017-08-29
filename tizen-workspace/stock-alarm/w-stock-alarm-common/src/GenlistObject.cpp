/*
 * Copyright (c) 2000-2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * This software is the confidential and proprietary information of
 * SAMSUNG ELECTRONICS ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance with the terms of the license agreement
 * you entered into with SAMSUNG ELECTRONICS.
 * SAMSUNG make no representations or warranties about the suitability
 * of the software, either express or implied, including but not
 * limited to the implied warranties of merchantability, fitness for
 * a particular purpose, or non-infringement.
 * SAMSUNG shall not be liable for any damages suffered by licensee as
 * a result of using, modifying or distributing this software or its derivatives.
 */

#include "WDebugBase.h"
#include <app.h>
#include "GenlistObject.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "GenlistObject"

GenlistObject::GenlistObject()
{
	_theme = nullptr;
	_isHomogeneousMode = true;
	//_isRealizationMode = false;
}
GenlistObject::~GenlistObject()
{
	_finalTheme();
}

Elm_Object_Item* GenlistObject::addItem(app_assist::WGenlistItem *item, Evas_Smart_Cb selectCb)
{
	Elm_Genlist_Item_Class* itc = item->getItemClassStatic();

	Elm_Object_Item* objectItem = nullptr;

	objectItem = elm_genlist_item_append(getEvasObject(), itc, item, NULL, ELM_GENLIST_ITEM_NONE, selectCb, this);

	item->setElmObjectItem(objectItem);

	return objectItem;
}

Elm_Object_Item* GenlistObject::addItemAfter(app_assist::WGenlistItem *current, app_assist::WGenlistItem *item, Evas_Smart_Cb selectCb)
{
	Elm_Genlist_Item_Class *itc = item->getItemClassStatic();

	Elm_Object_Item *objectItem = nullptr;

	objectItem = elm_genlist_item_insert_after(getEvasObject(), itc, item, nullptr, current->getElmObjectItem(), ELM_GENLIST_ITEM_NONE, selectCb, this);

	item->setElmObjectItem(objectItem);

	return objectItem;
}

Elm_Object_Item* GenlistObject::addTitleItem(app_assist::WGenlistItem *item, Evas_Smart_Cb selectCb)
{
	Elm_Genlist_Item_Class* itc = item->getItemClassStatic();

	Elm_Object_Item* titleItem = nullptr;

	titleItem = elm_genlist_item_append(getEvasObject(), itc, item, nullptr, ELM_GENLIST_ITEM_NONE, selectCb, this);

	item->setElmObjectItem(titleItem);

	/*
	 Guide from jongtae.oh
	 list 최 상단 dummy item을 touch하면 align이 다시 틀어지는데요.
	 그 상태에서 item을 실행하다 보면 첫번째 item에 대해 select 오동작이 발생하고 있습니다.
	 원인은 최 상단 dummy item touch시 해당 item 으로 bring in 시도하다 scroll 끝 부분 이여서 bring in 멈추며 실
	 제 select된 item과 align item이 불일치 하며 발생하는 이슈 입니다.
	 하여 select 동작이 없는 최상단 dummy item에 대해
	 elm_genlist_select_mode_set(Evas_Object *obj, ELM_OBJECT_SELECT_MODE_NONE); 설정해 주시면
	 문제 해결 될 것으로 보입니다.
	 */
	if (selectCb == nullptr)
		elm_genlist_item_select_mode_set(titleItem, ELM_OBJECT_SELECT_MODE_NONE);

	return titleItem;
}

Elm_Object_Item* GenlistObject::addGroupItem(app_assist::WGenlistItem *item)
{
	Elm_Genlist_Item_Class* itc = item->getItemClassStatic();

	Elm_Object_Item* objectItem = nullptr;

	objectItem = elm_genlist_item_append(getEvasObject(), itc, item, NULL, ELM_GENLIST_ITEM_NONE, nullptr, this);

	item->setElmObjectItem(objectItem);

	elm_genlist_item_select_mode_set(objectItem, ELM_OBJECT_SELECT_MODE_NONE);

	return objectItem;
}

Elm_Object_Item* GenlistObject::addPaddingItem(app_assist::WGenlistItem *item)
{
	Elm_Genlist_Item_Class* itc = item->getItemClassStatic();

	Elm_Object_Item* objectItem = nullptr;

	objectItem = elm_genlist_item_append(getEvasObject(), itc, item, NULL, ELM_GENLIST_ITEM_NONE, nullptr, this);

	item->setElmObjectItem(objectItem);

	elm_genlist_item_select_mode_set(objectItem, ELM_OBJECT_SELECT_MODE_NONE);

	return objectItem;
}

app_assist::WGenlistItem* GenlistObject::getFirstItem()
{
	Elm_Object_Item* it = elm_genlist_first_item_get(getEvasObject());

	if (it == nullptr)
		return nullptr;

	app_assist::WGenlistItem *item = (app_assist::WGenlistItem *) elm_object_item_data_get(it);

	return item;
}

app_assist::WGenlistItem* GenlistObject::getLastItem()
{
	Elm_Object_Item* it = elm_genlist_last_item_get(getEvasObject());

	if (it == nullptr)
		return nullptr;

	app_assist::WGenlistItem* item = (app_assist::WGenlistItem *) elm_object_item_data_get(it);

	return item;
}

app_assist::WGenlistItem* GenlistObject::getNextItem(app_assist::WGenlistItem *item)
{
	if (item == nullptr)
	{
		WERROR("param item is nullptr");
		return nullptr;
	}

	Elm_Object_Item* it = elm_genlist_item_next_get(item->getElmObjectItem());

	if (it == nullptr)
		return nullptr;

	item = (app_assist::WGenlistItem *) elm_object_item_data_get(it);

	return item;
}

app_assist::WGenlistItem* GenlistObject::getPrevItem(app_assist::WGenlistItem *item)
{
	Elm_Object_Item* it = elm_genlist_item_next_get(item->getElmObjectItem());

	if (it == nullptr)
		return nullptr;

	item = (app_assist::WGenlistItem*) elm_object_item_data_get(it);

	return item;
}

void GenlistObject::clear()
{
	Evas_Object *genlist = getEvasObject();
	if (genlist)
		elm_genlist_clear(genlist);
}

void GenlistObject::setHomogeneousMode(bool value)
{
	_isHomogeneousMode = value ? true : false;
}

//void GenlistObject::setRealizationMode(bool value)
//{
//	_isRealizationMode = value ? true : false;
//}

void GenlistObject::setCustomTheme(const char *themePath)
{
	_initTheme(themePath);
}

Evas_Object* GenlistObject::onCreateView(Evas_Object *parent, void *param)
{
	Evas_Object* genlist = elm_genlist_add(parent);

	if (_theme)
		elm_object_theme_set(genlist, _theme);
	if (_isHomogeneousMode)
		elm_genlist_homogeneous_set(genlist, EINA_TRUE);
	//if (_isRealizationMode)
	//elm_genlist_realization_mode_set(genlist, EINA_TRUE);
	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);

	evas_object_smart_callback_add(genlist, "longpressed",
			[](void *data, Evas_Object *obj, void *eventInfo)
			{
				if (eventInfo == nullptr)
				return;
				auto self = (GenlistObject *)data;
				Elm_Object_Item *item = (Elm_Object_Item *)eventInfo;
				self->onLongpressed(item);
			}, this);

	evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	evas_object_show(genlist);

	return genlist;
}
void GenlistObject::onCreated()
{
}

void GenlistObject::onDestroy()
{
}

void GenlistObject::onLongpressed(Elm_Object_Item *item)
{

}

void GenlistObject::_initTheme(const char *themePath)
{
	if (!themePath)
		return;

	_theme = elm_theme_new();
	if (!_theme)
	{
		WERROR("elm_theme_new failed!\n");
		return;
	}

	char *resPath = app_get_resource_path();
	char edjPath[PATH_MAX] = {0, };
	if (resPath)
	{
		snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, themePath);
		free(resPath);
	}

	elm_theme_ref_set(_theme, nullptr);
	elm_theme_extension_add(_theme, edjPath);
}

void GenlistObject::_finalTheme()
{
	if (_theme)
	{
		elm_theme_free(_theme);
		_theme = nullptr;
	}
}
