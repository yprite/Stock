/*
 * SaWidgetDetailInfoViewController.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: devbin
 */

#include "SaWidgetDetailInfoViewController.h"
#include <app.h>
#include <Elementary.h>

SaWidgetDetailInfoViewController::SaWidgetDetailInfoViewController(const SaCompanyInfo& companyInfo)
{
    // TODO Auto-generated constructor stub
    _companyInfo = companyInfo;

    _maxTextObj = nullptr;
    _minTextObj = nullptr;
    _volumeTextObj = nullptr;
    _previousTextObj = nullptr;
}

SaWidgetDetailInfoViewController::~SaWidgetDetailInfoViewController()
{
    // TODO Auto-generated destructor stub
}

void SaWidgetDetailInfoViewController::updateInfo(const SaCompanyInfo& companyInfo)
{
    _companyInfo = companyInfo;
    elm_object_part_text_set(_previousTextObj, "elm.text", _companyInfo.previous.c_str());
    elm_object_part_text_set(_volumeTextObj, "elm.text", _companyInfo.volume.c_str());
    elm_object_part_text_set(_maxTextObj, "elm.text", _companyInfo.max.c_str());
    elm_object_part_text_set(_minTextObj, "elm.text", _companyInfo.min.c_str());

    /*
    std::string code;
    std::string name;
    std::string market;
    std::string price;
    std::string change;
    std::string percent;
    std::string max;
    std::string min;
    std::string volume;
    std::string previous;
    std::string histroicaldata;
    */
}

Evas_Object* SaWidgetDetailInfoViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetDetailInfoView.edj");
        free(resPath);
    }

    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaWidgetDetailInfoView");
    evas_object_show(layout);
    return layout;
}

void SaWidgetDetailInfoViewController::onCreated()
{
    Evas_Object *layout = getEvasObject();

    char *resPath = app_get_resource_path();
    char boldTextPath[PATH_MAX] = {0, };
    char plainTextPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(boldTextPath, sizeof(boldTextPath), "%s%s", resPath, "edje/SaWidgetDetailInfoView.edj");
        snprintf(plainTextPath, sizeof(plainTextPath), "%s%s", resPath, "edje/SaWidgetDetailInfoView.edj");
        free(resPath);
    }

    // create upper box
    {
        Evas_Object *wrapperBox = elm_box_add(layout);
        elm_box_horizontal_set(wrapperBox, EINA_TRUE);
        {
            // create left sub info
            Evas_Object *box = elm_box_add(wrapperBox);
            elm_box_horizontal_set(box, EINA_TRUE);

            Evas_Object *boldTextObj = elm_layout_add(box);
            elm_layout_file_set(boldTextObj, boldTextPath, "SaWidgetDetailInfoView/BoldText");
            elm_object_part_text_set(boldTextObj, "elm.text", "전일종가");
            evas_object_show(boldTextObj);
            elm_box_pack_end(box, boldTextObj);

            Evas_Object *padding = evas_object_rectangle_add(evas_object_evas_get(box));
            evas_object_smart_member_add(padding, box);
            evas_object_size_hint_min_set(padding, 10, 20);
            evas_object_size_hint_max_set(padding, 10, 20);
            evas_object_color_set(padding, 0, 0, 0, 0);
            evas_object_show(padding);
            elm_box_pack_end(box, padding);

            Evas_Object *plainTextObj = elm_layout_add(box);
            elm_layout_file_set(plainTextObj, plainTextPath, "SaWidgetDetailInfoView/PlainText");
            elm_object_part_text_set(plainTextObj, "elm.text", "2,352,000");
            evas_object_show(plainTextObj);
            elm_box_pack_end(box, plainTextObj);
            _previousTextObj = plainTextObj;

            elm_box_recalculate(box);
            evas_object_show(box);
            elm_box_pack_end(wrapperBox, box);
        }
        {
            Evas_Object *padding = evas_object_rectangle_add(evas_object_evas_get(wrapperBox));
            evas_object_smart_member_add(padding, wrapperBox);
            evas_object_size_hint_min_set(padding, 15, 30);
            evas_object_size_hint_max_set(padding, 15, 30);
            evas_object_color_set(padding, 0, 0, 0, 0);
            evas_object_show(padding);
            elm_box_pack_end(wrapperBox, padding);
        }
        {
            // create left sub info
            Evas_Object *box = elm_box_add(wrapperBox);
            elm_box_horizontal_set(box, EINA_TRUE);

            Evas_Object *boldTextObj = elm_layout_add(box);
            elm_layout_file_set(boldTextObj, boldTextPath, "SaWidgetDetailInfoView/BoldText");
            elm_object_part_text_set(boldTextObj, "elm.text", "거래량");
            evas_object_show(boldTextObj);
            elm_box_pack_end(box, boldTextObj);

            Evas_Object *padding = evas_object_rectangle_add(evas_object_evas_get(box));
            evas_object_smart_member_add(padding, box);
            evas_object_size_hint_min_set(padding, 10, 20);
            evas_object_size_hint_max_set(padding, 10, 20);
            evas_object_color_set(padding, 0, 0, 0, 0);
            evas_object_show(padding);
            elm_box_pack_end(box, padding);

            Evas_Object *plainTextObj = elm_layout_add(box);
            elm_layout_file_set(plainTextObj, plainTextPath, "SaWidgetDetailInfoView/PlainText");
            elm_object_part_text_set(plainTextObj, "elm.text", "200,030");
            evas_object_show(plainTextObj);
            elm_box_pack_end(box, plainTextObj);
            _volumeTextObj = plainTextObj;

            elm_box_recalculate(box);
            evas_object_show(box);
            elm_box_pack_end(wrapperBox, box);
        }
        evas_object_show(wrapperBox);
        elm_object_part_content_set(layout, "sw.upper.box", wrapperBox);
    }
    // create lower box.
    {
        Evas_Object *wrapperBox = elm_box_add(layout);
        elm_box_horizontal_set(wrapperBox, EINA_TRUE);
        {
            // create left sub info
            Evas_Object *box = elm_box_add(wrapperBox);
            elm_box_horizontal_set(box, EINA_TRUE);

            Evas_Object *boldTextObj = elm_layout_add(box);
            elm_layout_file_set(boldTextObj, boldTextPath, "SaWidgetDetailInfoView/BoldText");
            elm_object_part_text_set(boldTextObj, "elm.text", "고가");
            evas_object_show(boldTextObj);
            elm_box_pack_end(box, boldTextObj);

            Evas_Object *padding = evas_object_rectangle_add(evas_object_evas_get(box));
            evas_object_smart_member_add(padding, box);
            evas_object_size_hint_min_set(padding, 10, 20);
            evas_object_size_hint_max_set(padding, 10, 20);
            evas_object_color_set(padding, 0, 0, 0, 0);
            evas_object_show(padding);
            elm_box_pack_end(box, padding);

            Evas_Object *plainTextObj = elm_layout_add(box);
            elm_layout_file_set(plainTextObj, plainTextPath, "SaWidgetDetailInfoView/PlainText");
            elm_object_part_text_set(plainTextObj, "elm.text", "2,362,000");
            evas_object_show(plainTextObj);
            elm_box_pack_end(box, plainTextObj);
            _maxTextObj = plainTextObj;

            elm_box_recalculate(box);
            evas_object_show(box);
            elm_box_pack_end(wrapperBox, box);
        }
        {
            Evas_Object *padding = evas_object_rectangle_add(evas_object_evas_get(wrapperBox));
            evas_object_smart_member_add(padding, wrapperBox);
            evas_object_size_hint_min_set(padding, 15, 30);
            evas_object_size_hint_max_set(padding, 15, 30);
            evas_object_color_set(padding, 0, 0, 0, 0);
            evas_object_show(padding);
            elm_box_pack_end(wrapperBox, padding);
        }
        {
            // create left sub info
            Evas_Object *box = elm_box_add(wrapperBox);
            elm_box_horizontal_set(box, EINA_TRUE);

            Evas_Object *boldTextObj = elm_layout_add(box);
            elm_layout_file_set(boldTextObj, boldTextPath, "SaWidgetDetailInfoView/BoldText");
            elm_object_part_text_set(boldTextObj, "elm.text", "저가");
            evas_object_show(boldTextObj);
            elm_box_pack_end(box, boldTextObj);

            Evas_Object *padding = evas_object_rectangle_add(evas_object_evas_get(box));
            evas_object_smart_member_add(padding, box);
            evas_object_size_hint_min_set(padding, 10, 20);
            evas_object_size_hint_max_set(padding, 10, 20);
            evas_object_color_set(padding, 0, 0, 0, 0);
            evas_object_show(padding);
            elm_box_pack_end(box, padding);

            Evas_Object *plainTextObj = elm_layout_add(box);
            elm_layout_file_set(plainTextObj, plainTextPath, "SaWidgetDetailInfoView/PlainText");
            elm_object_part_text_set(plainTextObj, "elm.text", "2,312,030");
            evas_object_show(plainTextObj);
            elm_box_pack_end(box, plainTextObj);
            _minTextObj = plainTextObj;

            elm_box_recalculate(box);
            evas_object_show(box);
            elm_box_pack_end(wrapperBox, box);
        }
        evas_object_show(wrapperBox);
        elm_object_part_content_set(layout, "sw.lower.box", wrapperBox);
    }

    elm_object_part_text_set(_previousTextObj, "elm.text", _companyInfo.previous.c_str());
    elm_object_part_text_set(_volumeTextObj, "elm.text", _companyInfo.volume.c_str());
    elm_object_part_text_set(_maxTextObj, "elm.text", _companyInfo.max.c_str());
    elm_object_part_text_set(_minTextObj, "elm.text", _companyInfo.min.c_str());
}

void SaWidgetDetailInfoViewController::onDestroy()
{

}
