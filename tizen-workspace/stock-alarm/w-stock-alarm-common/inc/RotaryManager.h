/*
 * RotaryManager.h
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#ifndef _ROTARY_MANAGER_H_
#define _ROTARY_MANAGER_H_

#include <efl_extension.h>
#include <vector>

#include "Singleton.h"
#include "WDefine.h"

#define ROTARY_MANAGER_DEFAULT_ROTARY_HANDLER [](Eext_Rotary_Event_Direction direction){}

class WAPP_ASSIST_EXPORT RotaryManager
{
SINGLETON_IDIOM(RotaryManager);
protected:
    RotaryManager();
    virtual ~RotaryManager();

public:
    void initialize(Evas_Object* conformant);
    void finalize();
    void setOnRotary(Evas_Object* object, const std::function<void(Eext_Rotary_Event_Direction direction)>& handler);
    Eext_Circle_Surface* getCircleSurface() const;
    void active();
    void inactive();
    void remove(Evas_Object* object);

private:
    void _add(Evas_Object* object, const std::function<void(Eext_Rotary_Event_Direction direction)>& handler);
    void _remove(Evas_Object* object);
    void _activateLastObject();
    static void _objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info);

private:
    static Eina_Bool _onRotary(void* data, Evas_Object* object, Eext_Rotary_Event_Info* info);

private:
    Eext_Circle_Surface* _circleSurface;
    std::vector<Evas_Object*> _objects;
    std::vector<std::function<void(Eext_Rotary_Event_Direction direction)>> _handlers;
    bool _isFinalized;
};

#endif /* _ROTARY_MANAGER_H_ */
