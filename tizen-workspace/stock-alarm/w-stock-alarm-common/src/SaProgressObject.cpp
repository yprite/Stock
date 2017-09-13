/*
 * SaProgressObject.cpp
 *
 *  Created on: Aug 4, 2017
 *      Author: devbin
 */

#include "SaProgressObject.h"
#include "SaUtility.h"
#include "SaCommonDebug.h"

#include <Elementary.h>
#include <cairo.h>
#include <app.h>

static const int DEFAULT_SIZE = 100;
static const int SCREEN_SIZE = 360;

using namespace app_assist;

SaProgressObject::SaProgressObject()
{
    // TODO Auto-generated constructor stub

    _startTime = 0.0f;
    _canvas = nullptr;
    _isRunning = false;

}

SaProgressObject::~SaProgressObject()
{
    // TODO Auto-generated destructor stub
    if (!_animator.expired())
        WTimer::destroy(_animator);
}

void SaProgressObject::run()
{
    if (_isRunning)
        return;

    _startTime = ecore_loop_time_get();
    _isRunning = true;
    _animator = WTimer::addAnimator(_onAnimatorRunning, this);
}

void SaProgressObject::stop()
{
    if (!_animator.expired())
        WTimer::destroy(_animator);
}

bool SaProgressObject::isRunning() const
{
    return _isRunning;
}

Evas_Object* SaProgressObject::onCreateView(Evas_Object *parent, void *param)
{
    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaProgressObject.edj");
        free(resPath);
    }
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaProgressObject");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    return layout;
}

void SaProgressObject::onCreated()
{
    Evas_Object *layout = getEvasObject();
    Evas_Object *canvasImg = elm_image_add(layout);
    Evas_Object *canvas = elm_image_object_get(canvasImg);

    evas_object_image_size_set(canvas, SCREEN_SIZE, SCREEN_SIZE);
    evas_object_image_alpha_set(canvas, EINA_TRUE);
    evas_object_image_colorspace_set(canvas, EVAS_COLORSPACE_ARGB8888);
    evas_object_show(canvas);

    _canvas = canvas;

    elm_object_part_content_set(layout, "elm.swallow.content", canvasImg);
/*
    edje_object_signal_callback_add(elm_layout_edje_get(layout), "loaded", "*",
            [](void *data, Evas_Object *obj, const char *emission, const char *source)
            {
                WINFO("load.finished!");
                auto self = (SaProgressObject *)data;

                Evas_Object *layout = self->getEvasObject();
                Evas_Object *canvasImg = elm_image_add(layout);
                Evas_Object *canvas = elm_image_object_get(canvasImg);

                evas_object_image_size_set(canvas, SCREEN_SIZE, SCREEN_SIZE);
                evas_object_image_alpha_set(canvas, EINA_TRUE);
                evas_object_image_colorspace_set(canvas, EVAS_COLORSPACE_ARGB8888);
                evas_object_show(canvas);

                self->_canvas = canvas;

                elm_object_part_content_set(self->getEvasObject(), "elm.swallow.content", canvasImg);
                self->run();
            }, this);
*/
}

void SaProgressObject::onDestroy()
{

}

void SaProgressObject::_renew(double ua1, double ua2, double la1, double la2)
{
    auto canvas = _canvas;

    static const int LINE_WIDTH = 6;
    static const int RADIUS = 30;     // r
    static const int CX = SCREEN_SIZE/2;         // center position x
    static const int CY = SCREEN_SIZE/2;         // center position y

    //char const *colorStr = "#7200da";
    //char const *colorStr = "#f9c00c";
    //char const *colorStr = "#00b9f1";
    char const *colorStr = "#9A0EFF";

    int r = 0, g = 0, b = 0;

    int rowStride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, SCREEN_SIZE);
    unsigned char* imgData = (unsigned char*)evas_object_image_data_get(canvas, EINA_TRUE);
    cairo_surface_t *surface = cairo_image_surface_create_for_data(imgData,
            CAIRO_FORMAT_ARGB32, SCREEN_SIZE, SCREEN_SIZE, rowStride);
    evas_object_image_data_set(canvas, imgData);
    cairo_t *cairo = cairo_create(surface);


    /* clean cairo data */
    cairo_save(cairo);
    cairo_set_operator(cairo, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cairo);
    cairo_restore(cairo);

    /* default setting - LINE_WITH, CAP STYLE */
    cairo_set_line_width(cairo, LINE_WIDTH);
    cairo_set_line_cap(cairo, CAIRO_LINE_CAP_BUTT);

    /* drawing upper half circle */
    //_setColor(cairo, 45);
    SaUtility::convertHexColorToRGBA(colorStr, &r, &g, &b);
    cairo_set_source_rgba(cairo, r/255.0f, b/255.0f, b/255.0f, 1);
    cairo_new_sub_path(cairo);
    cairo_arc(cairo, CX, CY, RADIUS, ua1, ua2);
    cairo_stroke(cairo);

    /* drawing lower half circle */
    //_setColor(cairo, 85);
    SaUtility::convertHexColorToRGBA(colorStr, &r, &g, &b);
        cairo_set_source_rgba(cairo, r/255.0f, b/255.0f, b/255.0f, 1);
    cairo_new_sub_path(cairo);
    cairo_arc(cairo, CX, CY, RADIUS, la1, la2);
    cairo_stroke(cairo);

    /* update */
    cairo_surface_flush(surface);
    evas_object_image_data_update_add(canvas, 0, 0, SCREEN_SIZE, SCREEN_SIZE);
    evas_object_image_pixels_dirty_set(canvas, EINA_TRUE);

    if (cairo)
    {
        cairo_destroy(cairo);
        cairo = nullptr;
    }
    if (surface)
    {
        cairo_surface_destroy(surface);
        surface = nullptr;
    }

}

bool SaProgressObject::_onAnimatorRunning(void *data)
{
    static double SINE_IN_OUT_70[4] = {0.33, 0.00, 0.30, 1.00};
    static double SINE_IN_OUT_33[4] = {0.33, 0.00, 0.67, 1.00};
    static double TOT_ANGLE = 1440;
    static const double DELAY_TIME = 0.1;

    auto self = (SaProgressObject *)data;
    double time = ecore_loop_time_get();
    double timeDiff = (time - self->_startTime);
    double angleRatio = ecore_animator_pos_map_n(timeDiff/3, ECORE_POS_MAP_CUBIC_BEZIER, 4, SINE_IN_OUT_70);

    if (timeDiff < 1.0f)
    {
        double lengthRatio = ecore_animator_pos_map_n((double)(time - self->_startTime),
                ECORE_POS_MAP_CUBIC_BEZIER, 4, SINE_IN_OUT_33);

        double dl = (0.6 * lengthRatio) * 90;   // delta length
        double ra = angleRatio * TOT_ANGLE;     // rotation angle

        double ua1 = (180 + ra + dl) * (M_PI / 180.0);
        double ua2 = (0 + ra - dl) * (M_PI / 180.0);
        double la1 = (0 + ra + dl) * (M_PI / 180.0);
        double la2 = (180 + ra - dl) * (M_PI / 180.0);

        self->_renew(ua1, ua2, la1, la2);
    }
    else if (timeDiff >= 1.0f && timeDiff < 2.0f)
    {
        double ra = angleRatio * TOT_ANGLE;     // rotation angle
        double ua1 = (180 + ra + 54) * (M_PI / 180.0);
        double ua2 = (0 + ra - 54) * (M_PI / 180.0);
        double la1 = (0 + ra + 54) * (M_PI / 180.0);
        double la2 = (180 + ra - 54) * (M_PI / 180.0);
        self->_renew(ua1, ua2, la1, la2);
    }
    else
    {
        double lengthRatio = ecore_animator_pos_map_n((double)(time - self->_startTime - 2),
                ECORE_POS_MAP_CUBIC_BEZIER, 4, SINE_IN_OUT_33);

        double dl = (0.6 * lengthRatio) * 90;   // delta length
        double ra = angleRatio * TOT_ANGLE;     // rotation angle

        double ua1 = ((180 + ra + 54) - dl) * (M_PI / 180.0);
        double ua2 = ((0 + ra - 54) + dl) * (M_PI / 180.0);
        double la1 = ((0 + ra + 54) - dl) * (M_PI / 180.0);
        double la2 = ((180 + ra - 54) + dl) * (M_PI / 180.0);

        self->_renew(ua1, ua2, la1, la2);
        if (timeDiff >= 3.0f)
        {
            WTimer::addTimer(DELAY_TIME,
                    [](void *userData)->bool
                    {
                        auto self = (SaProgressObject *)userData;
                        double time = ecore_loop_time_get();
                        self->_startTime = time;

                        return false;
                    }, self);
        }
    }
    return true;

}


