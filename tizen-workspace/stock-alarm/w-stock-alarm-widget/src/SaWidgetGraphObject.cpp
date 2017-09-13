/*
 * SaWidgetGraphObject.cpp
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#include "SaWidgetGraphObject.h"
#include "SaWidgetDebug.h"
#include <Elementary.h>

SaWidgetGraphObject::SaWidgetGraphObject(int width, int height)
{
    // TODO Auto-generated constructor stub
    _canvas = nullptr;
    _surface = nullptr;
    _cairo = nullptr;
    _r = _g = _b = _a = 255;
    _width = width;
    _height = height;
}

SaWidgetGraphObject::~SaWidgetGraphObject()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaWidgetGraphObject::onCreateView(Evas_Object *parent, void *param)
{
   return elm_image_add(parent);
}

void SaWidgetGraphObject::onCreated()
{
    Evas_Object *base = getEvasObject();
    Evas_Object *canvas = elm_image_object_get(base);
    evas_object_image_size_set(canvas, _width, _height);
    evas_object_image_alpha_set(canvas, EINA_TRUE);
    evas_object_image_colorspace_set(canvas, EVAS_COLORSPACE_ARGB8888);
    evas_object_show(canvas);

    _canvas = canvas;

    _surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, _width, _height);
    _cairo = cairo_create(_surface);

    unsigned char *imageData = (unsigned char *)cairo_image_surface_get_data(cairo_get_target(_cairo));
    evas_object_image_data_set(_canvas, imageData);
    evas_object_image_data_update_add(_canvas, 0, 0, _width, _height);

    // add first point
    //_pointList.push_back(Point{-20.0f, _height/2.0f});
    //_pointList.push_back(Point{-10.0f, _height/2.0f});
}

void SaWidgetGraphObject::onDestroy()
{
    if (_surface)
        cairo_surface_destroy(_surface);
    if (_cairo)
        cairo_destroy(_cairo);
}

void SaWidgetGraphObject::setColor(int r, int g, int b, int a)
{
    _r = r;
    _g = g;
    _b = b;
    _a = a;
}

bool SaWidgetGraphObject::updateGraph(const char *jsonData)
{
    if (!_pointList.empty())
        _pointList.clear();

    // parse jsonData

    return true;
}

void SaWidgetGraphObject::addPoint(const double &y)
{
    Point p(0, y);
    _pointList.push_back(p);
}

void SaWidgetGraphObject::drawGraph()
{
    Point prevPos2, prevPos3, prevPos1, mid1, mid2;

    cairo_set_line_width(_cairo, 4);
    cairo_set_source_rgba(_cairo, _r/255.0f, _g/255.0f, _b/255.0f, _a/255.0f);

    double _flagW = _width / (_pointList.size() - 2);
    double _x_ = -1 * _flagW;
    double minY = 9999999;
    double maxY = 0;

    for (auto p = _pointList.begin(); p != _pointList.end(); ++p)
    {
        if (p->y >= maxY)
            maxY = p->y;
        if (p->y <= minY)
            minY = p->y;
    }

    WINFO("maxY : %f, minY : %f", maxY, minY);
    size_t len = _pointList.size();

    //_pointList.push_back(Point{_pointList[len-1].x, _pointList[len-1].y});
    _pointList.push_back(Point{_pointList.back().x, _pointList.back().y});

    _pointList.push_front(Point{-10, (maxY + minY) / 2});
    _pointList.push_front(Point{-20, (maxY + minY) / 2});

    for (auto it = _pointList.begin(); it != _pointList.end(); ++it)
    {
        prevPos2 = prevPos3;
        prevPos3 = prevPos1;
        double deltaY = maxY - minY;

        //double _y_ = _height - ((_height * it->y) / deltaY);
        //double _y_ = _height - (it->y * _height / maxY);
        double _y_ = _height - ((it->y - minY) * _height) / deltaY;
        WINFO("%lf", _y_);
        mid1.x = (prevPos3.x + prevPos2.x)/2.0f;
        mid1.y = (prevPos3.y + prevPos2.y)/2.0f;

        mid2.x = (_x_ + prevPos3.x) / 2.0f;
        mid2.y = (_y_ + prevPos3.y) / 2.0f;

        //WINFO("mid1.x : %f, mid1.y : %f, mid2.x : %f, mid2.y: %f", mid1.x, mid1.y, mid2.x, mid2.y);
        cairo_move_to(_cairo, mid1.x, mid1.y);
        _drawQuadraticCurveTo(prevPos3.x, prevPos3.y, mid2.x, mid2.y);
        prevPos1.x = _x_;
        prevPos1.y = _y_;

        _x_ += _flagW;
    }
    cairo_stroke(_cairo);
    cairo_surface_flush(_surface);
}


void SaWidgetGraphObject::_drawQuadraticCurveTo(double x1, double y1, double x2, double y2)
{
    double x0 = 0.0f;
    double y0 = 0.0f;
    cairo_get_current_point(_cairo, &x0, &y0);
    cairo_curve_to(_cairo,
            ((2.0 / 3.0 * x1) + (1.0 / 3.0 * x0)),
            ((2.0 / 3.0 * y1) + (1.0 / 3.0 * y0)),
            ((2.0 / 3.0 * x1) + (1.0 / 3.0 * x2)),
            ((2.0 / 3.0 * y1 )+ (1.0 / 3.0 * y2)),
            x2, y2);
}
