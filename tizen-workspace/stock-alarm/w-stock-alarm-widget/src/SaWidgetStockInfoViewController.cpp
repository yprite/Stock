/*
 * SaWidgetStockInfoViewController.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#include "SaWidgetStockInfoViewController.h"
#include "SaWidgetDebug.h"
#include "WTimer.h"

#include <app.h>
#include <app_alarm.h>
#include <Elementary.h>

using namespace app_assist;

SaWidgetStockInfoViewController::SaWidgetStockInfoViewController()
{
    // TODO Auto-generated constructor stub
    _isAnimatorRunning = false;
    _animationStartTime = 0.0f;

	_titlePriceInfoBox = nullptr;
    _titlePriceInfoIcon = nullptr;
    _titlePriceInfoText = nullptr;
}

SaWidgetStockInfoViewController::~SaWidgetStockInfoViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaWidgetStockInfoViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    WENTER();

    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetStockInfoView.edj");
        free(resPath);
    }

    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaWidgetStockInfoView");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    return layout;
}

void SaWidgetStockInfoViewController::onCreated()
{
    WENTER();

    Evas_Object *layout = getEvasObject();
    _createTitlePriceInfo();
    _createSubPriceInfo();

    edje_object_signal_callback_add(elm_layout_edje_get(layout), "touch.area.mouse.clicked", "*",
        [](void *data, Evas_Object *obj, const char *emission, const char *source)
        {
            auto self = (SaWidgetStockInfoViewController *)data;
            if (self->_isAnimatorRunning)
                return;

            self->_animationStartTime = ecore_loop_time_get();
            self->_isAnimatorRunning = true;

#if 1
            WINFO("START ANIMATOR!");
            self->_animator = WTimer::addAnimator(
                [](void *data)->bool
                {
                    auto self = (SaWidgetStockInfoViewController *)data;
                    double duration = 10.0f;
                    double time = ecore_loop_time_get();
                    double t = (time - self->_animationStartTime) / duration;

                    if (t >= 0.2f)
                    {
                        if (!self->_animator.expired())
                            WTimer::destroy(self->_animator);
                        self->_isAnimatorRunning = false;

                        WINFO("FINISH ANIMATOR!");
                        return false;
                    }

                    double deltaX = 30.0f;
                    double deltaY = 100.0f;
                    double ratio = ecore_animator_pos_map(t, ECORE_POS_MAP_LINEAR, 0.0, 1.0);
                    //WINFO("ratio - %lf", ratio);
                    // TODO
                    {
                        Evas_Object *layout = self->getEvasObject();
                        Evas_Coord x = 0, y = 0, w = 0, h = 0;
                        evas_object_geometry_get(layout, &x, &y, &w, &h);
                        WINFO("%d %d %d %d", x, y, w, h);
                        Evas_Map *m = evas_map_new(4);
                        evas_map_util_points_populate_from_object(m, layout);
                        evas_map_util_zoom(m, 1 - ratio, 1 - ratio, w/2, 0);
                        evas_object_map_set(layout, m);
                        evas_object_map_enable_set(layout, EINA_TRUE);
                        evas_map_free(m);
                    }
                    return true;
                }, data);
#endif
        }, this);
}

void SaWidgetStockInfoViewController::onDestroy()
{

}

void SaWidgetStockInfoViewController::onEventOccured(AppEventListener::EventType eventType)
{
    WENTER();
    if (eventType == AppEventListener::EventType::FONT_CHANGED ||
        eventType == AppEventListener::EventType::COLOR_CHANGED ||
        eventType == AppEventListener::EventType::LANGUAGE_CHANGED ||
        eventType == AppEventListener::EventType::REGION_FORMAT_CHANGED)
    {
        _updateText();
    }

}

void SaWidgetStockInfoViewController::_createTitlePriceInfo()
{
    WENTER();

    static const int TITLE_PRICE_ICON_SIZE = 46;
    static const int TITLE_PRICE_INFO_PADDING = 20;
    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = { 0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetStockInfoView.edj");
        free(resPath);
    }

    Evas_Object *layout = getEvasObject();
    Evas_Object *box = elm_box_add(layout);
    elm_box_horizontal_set(box, EINA_TRUE);

    Evas_Object *priceInfoIcon = evas_object_rectangle_add(evas_object_evas_get(box));
    evas_object_size_hint_min_set(priceInfoIcon, TITLE_PRICE_ICON_SIZE, TITLE_PRICE_ICON_SIZE);
    evas_object_size_hint_max_set(priceInfoIcon, TITLE_PRICE_ICON_SIZE, TITLE_PRICE_ICON_SIZE);
    evas_object_resize(priceInfoIcon, TITLE_PRICE_ICON_SIZE, TITLE_PRICE_ICON_SIZE);
    evas_object_color_set(priceInfoIcon, 0, 0, 255, 255);
    evas_object_show(priceInfoIcon);

    Evas_Object *paddingRect = evas_object_rectangle_add(evas_object_evas_get(box));
    evas_object_size_hint_min_set(paddingRect, TITLE_PRICE_INFO_PADDING, TITLE_PRICE_ICON_SIZE);
    evas_object_size_hint_max_set(paddingRect, TITLE_PRICE_INFO_PADDING, TITLE_PRICE_ICON_SIZE);
    evas_object_resize(paddingRect, TITLE_PRICE_INFO_PADDING, TITLE_PRICE_ICON_SIZE);
    evas_object_color_set(paddingRect, 0, 0, 0, 0);
    evas_object_show(paddingRect);

    Evas_Object *priceInfoText = elm_layout_add(box);
    elm_layout_file_set(priceInfoText, edjPath, "SaWidgetStockInfoView/TitlePriceInfoText");
    evas_object_show(priceInfoText);

    elm_box_pack_end(box, priceInfoIcon);
    elm_box_pack_end(box, paddingRect);
    elm_box_pack_end(box, priceInfoText);

    elm_box_recalculate(box);
    evas_object_show(box);

    elm_object_part_content_set(layout, "sw.price.info", box);
}

void SaWidgetStockInfoViewController::_createSubPriceInfo()
{
    //SaWidgetStockInfoView/SubPriceInfoIcon
    //SaWidgetStockInfoView/SubPriceInfoText
    static const int SUB_PRICE_ICON_SIZE = 30;
    static const int CENTER_PADDING_RECT_WIDTH = 30;
    static const int PRICE_INFO_PADDING = 10;

    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = { 0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetStockInfoView.edj");
        free(resPath);
    }
    Evas_Object *layout = getEvasObject();
    Evas_Object *wrapperBox = elm_box_add(layout);
    elm_box_horizontal_set(wrapperBox, EINA_TRUE);

    { // create left box sub price info
        Evas_Object *box = elm_box_add(wrapperBox);
        elm_box_horizontal_set(box, EINA_TRUE);

        Evas_Object *iconLayout = elm_layout_add(box);
        elm_layout_file_set(iconLayout, edjPath, "SaWidgetStockInfoView/SubPriceInfoIcon");
        evas_object_show(iconLayout);

        Evas_Object *icon = evas_object_rectangle_add(evas_object_evas_get(box));
        evas_object_size_hint_min_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_resize(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_color_set(icon, 0, 0, 255, 255);
        evas_object_show(icon);
        elm_object_part_content_set(iconLayout, "elm.swallow.content", icon);

        Evas_Object *paddingRect = evas_object_rectangle_add(evas_object_evas_get(box));
        evas_object_size_hint_min_set(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_resize(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_color_set(paddingRect, 0, 0, 0, 0);
        evas_object_show(paddingRect);

        Evas_Object *priceInfoText = elm_layout_add(box);
        elm_layout_file_set(priceInfoText, edjPath, "SaWidgetStockInfoView/SubPriceInfoText");
        elm_object_part_text_set(priceInfoText, "elm.text", "39,000");
        evas_object_show(priceInfoText);

        elm_box_pack_end(box, iconLayout);
        elm_box_pack_end(box, paddingRect);
        elm_box_pack_end(box, priceInfoText);
        elm_box_recalculate(box);
        evas_object_show(box);

        elm_box_pack_end(wrapperBox, box);
    }

    { // create center padding rect
        Evas_Object *paddingRect = evas_object_rectangle_add(evas_object_evas_get(wrapperBox));
        evas_object_size_hint_min_set(paddingRect, CENTER_PADDING_RECT_WIDTH, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(paddingRect, CENTER_PADDING_RECT_WIDTH, SUB_PRICE_ICON_SIZE);
        evas_object_resize(paddingRect, CENTER_PADDING_RECT_WIDTH, SUB_PRICE_ICON_SIZE);
        evas_object_color_set(paddingRect, 0, 0, 0, 0);
        evas_object_show(paddingRect);

        elm_box_pack_end(wrapperBox, paddingRect);
    }

    { // create right box sub price info
        Evas_Object *box = elm_box_add(wrapperBox);
        elm_box_horizontal_set(box, EINA_TRUE);

        Evas_Object *iconLayout = elm_layout_add(box);
        elm_layout_file_set(iconLayout, edjPath, "SaWidgetStockInfoView/SubPriceInfoIcon");
        evas_object_show(iconLayout);

        Evas_Object *icon = evas_object_rectangle_add(evas_object_evas_get(box));
        evas_object_size_hint_min_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_resize(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_color_set(icon, 0, 0, 255, 255);
        evas_object_show(icon);
        elm_object_part_content_set(iconLayout, "elm.swallow.content", icon);

        Evas_Object *paddingRect = evas_object_rectangle_add(evas_object_evas_get(box));
        evas_object_size_hint_min_set(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_resize(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_color_set(paddingRect, 0, 0, 0, 0);
        evas_object_show(paddingRect);

        Evas_Object *priceInfoText = elm_layout_add(box);
        elm_layout_file_set(priceInfoText, edjPath, "SaWidgetStockInfoView/SubPriceInfoText");
        elm_object_part_text_set(priceInfoText, "elm.text", "1.78%");
        evas_object_show(priceInfoText);

        elm_box_pack_end(box, iconLayout);
        elm_box_pack_end(box, paddingRect);
        elm_box_pack_end(box, priceInfoText);
        elm_box_recalculate(box);
        evas_object_show(box);

        elm_box_pack_end(wrapperBox, box);
    }

    elm_box_recalculate(wrapperBox);
    evas_object_show(wrapperBox);

    elm_object_part_content_set(layout, "sw.sub.price.info", wrapperBox);
}

void SaWidgetStockInfoViewController::_updateText()
{

}

