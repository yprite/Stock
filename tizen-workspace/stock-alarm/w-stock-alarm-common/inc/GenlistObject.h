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

#ifndef _SC_GENLIST_OBJECT_H_
#define _SC_GENLIST_OBJECT_H_

#include "WViewController.h"
#include "WGenlistItem.h"
#include "WDefine.h"
#include <Elementary.h>
#include <Evas.h>

class WAPP_ASSIST_EXPORT GenlistObject: public app_assist::WViewController
{
    public:
        GenlistObject();
        virtual ~GenlistObject();

        Elm_Object_Item* addItem(app_assist::WGenlistItem *item, Evas_Smart_Cb selectCb = nullptr);
        Elm_Object_Item* addItemAfter(app_assist::WGenlistItem *current, app_assist::WGenlistItem *item, Evas_Smart_Cb selectCb = nullptr);
        Elm_Object_Item* addTitleItem(app_assist::WGenlistItem *item, Evas_Smart_Cb selectCb = nullptr);
        Elm_Object_Item* addGroupItem(app_assist::WGenlistItem *item);
        Elm_Object_Item* addPaddingItem(app_assist::WGenlistItem *item);

        app_assist::WGenlistItem* getFirstItem();
        app_assist::WGenlistItem* getLastItem();
        app_assist::WGenlistItem* getNextItem(app_assist::WGenlistItem *item);
        app_assist::WGenlistItem* getPrevItem(app_assist::WGenlistItem *item);

        void clear();
        void setHomogeneousMode(bool value);
        //void setRealizationMode(bool value);

        void setCustomTheme(const char *themePath);

    private:
        virtual Evas_Object* onCreateView(Evas_Object *parent, void *param);

    protected:
        // if you want to some actions,
        // please override under functions.
        virtual void onCreated();
        virtual void onDestroy();

        virtual void onLongpressed(Elm_Object_Item *item);

    protected:
        // when you want to custom theme, call under functions.
        void _initTheme(const char *themePath);
        void _finalTheme();

    private:
        Elm_Theme *_theme;
        bool _isHomogeneousMode;
        //bool _isRealizationMode;
};

#endif /* _SC_GENLIST_OBJECT_H_ */
