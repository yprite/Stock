/*
 * SaWidgetGraphObject.h
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#ifndef SAWIDGETGRAPHOBJECT_H_
#define SAWIDGETGRAPHOBJECT_H_

#include "WViewController.h"
#include <cairo.h>
#include <vector>
#include <list>

class SaWidgetGraphObject : public app_assist::WViewController
{
public:
    struct Point
    {
        double x;
        double y;

        Point() : x(0), y(0) { }
        Point(const int& _x, const int& _y) : x(_x), y(_y) { }
    };

public:
    SaWidgetGraphObject(int width, int height);
    virtual ~SaWidgetGraphObject();

    void setColor(int r, int g, int b, int a);
    bool updateGraph(const char *jsonData);
    void addPoint(const double &y);
    void drawGraph();

private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *param) override;
    virtual void onCreated() override;
    virtual void onDestroy() override;

    void _drawQuadraticCurveTo(double x1, double y1, double x2, double y2);

private:
    Evas_Object *_canvas;
    cairo_surface_t *_surface;
    cairo_t *_cairo;
    std::list<Point> _pointList;
    int _r, _g, _b, _a;
    int _width, _height;
};

#endif /* SAWIDGETGRAPHOBJECT_H_ */
