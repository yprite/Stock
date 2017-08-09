/*
 * SaProgressObject.h
 *
 *  Created on: Aug 4, 2017
 *      Author: devbin
 */

#ifndef SAPROGRESSOBJECT_H_
#define SAPROGRESSOBJECT_H_

#include "WDefine.h"
#include "WViewController.h"
#include "WTimer.h"
#include <cairo.h>

class WAPP_ASSIST_EXPORT SaProgressObject : public app_assist::WViewController
{
    public:
        SaProgressObject();
        virtual ~SaProgressObject();

    public:
        void setSize(int w, int h);
        void run();
        void stop();
        bool isRunning() const;

    private:
        virtual Evas_Object* onCreateView(Evas_Object *parent, void *param) override;
        virtual void onCreated() override;
        virtual void onDestroy() override;

    private:
        void _renew(double ua1, double ua2, double la1, double la2);
        static bool _onAnimatorRunning(void *data);

    private:
        double _startTime;
        Evas_Object *_canvas;
        bool _isRunning;
        app_assist::WTimerWeakPtr _animator;

};

#endif /* SAPROGRESSOBJECT_H_ */
