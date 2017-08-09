/*
 * RotaryManager.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#include "RotaryManager.h"
#include "WDebugBase.h"

#include <efl_extension.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "RotaryManager"

SINGLETON_INSTANCE(RotaryManager);

RotaryManager::RotaryManager()
{
    WENTER();

    _circleSurface = nullptr;
    _isFinalized = false;
}

RotaryManager::~RotaryManager()
{

}

void RotaryManager::initialize(Evas_Object* conformant)
{
    WENTER();

    _circleSurface = eext_circle_surface_conformant_add(conformant);
    WASSERT(_circleSurface);
}

void RotaryManager::setOnRotary(Evas_Object* object, const std::function<void(Eext_Rotary_Event_Direction direction)>& handler)
{
    WENTER();

    WASSERT(object);

    _add(object, handler);

    evas_object_event_callback_add(object, EVAS_CALLBACK_DEL, _objDelCb, this);

    _activateLastObject();
}

Eina_Bool RotaryManager::_onRotary(void* data, Evas_Object* object, Eext_Rotary_Event_Info* info)
{
    WENTER();

    auto self = (RotaryManager*) data;
    auto rit = self->_handlers.rbegin();
    std::function<void(Eext_Rotary_Event_Direction direction)> handler = (std::function<void(Eext_Rotary_Event_Direction direction)>) * rit;
    WASSERT(handler != nullptr);

    handler(info->direction);

    return EINA_TRUE;
}

void RotaryManager::_add(Evas_Object* object, const std::function<void(Eext_Rotary_Event_Direction direction)>& handler)
{
    _objects.push_back(object);
    _handlers.push_back(handler);

    eext_rotary_object_event_callback_priority_add(object, EEXT_CALLBACK_PRIORITY_BEFORE, _onRotary, this);
}

void RotaryManager::_remove(Evas_Object* object)
{
    WENTER();
    //WASSERT(object == _objects.top());

    int i = 0;
    for (auto it = _objects.begin(); it != _objects.end(); ++it)
    {
        if (*it == object)
        {
            _objects.erase(it);
            _handlers.erase(_handlers.begin() + i);
            break;
        }
        i++;
    }
    //_objects.pop();
    //_handlers.pop();
}

void RotaryManager::_activateLastObject()
{
    if (_objects.size() == 0)
        return;

    auto rit = _objects.rbegin();
    Evas_Object* obj = (Evas_Object*) *rit;

    eext_rotary_object_event_activated_set(obj, EINA_TRUE);
}

void RotaryManager::_objDelCb(void* userData, Evas* evas, Evas_Object* obj, void* event_info)
{
    auto self = (RotaryManager*) userData;

    // do nothing.
    if (self->_isFinalized == true)
        return;

    self->_remove(obj);
    self->_activateLastObject();
}

Eext_Circle_Surface* RotaryManager::getCircleSurface() const
{
    WASSERT(_circleSurface);
    return _circleSurface;
}

void RotaryManager::finalize()
{
    WENTER();

    _isFinalized = true;
}

void RotaryManager::active()
{
    if (_objects.size() == 0)
        return;

    auto rit = _objects.rbegin();
    Evas_Object* obj = (Evas_Object*) *rit;

    eext_rotary_object_event_activated_set(obj, EINA_TRUE);
}

void RotaryManager::inactive()
{
    if (_objects.size() == 0)
        return;

    auto rit = _objects.rbegin();
    Evas_Object* obj = (Evas_Object*) *rit;

    eext_rotary_object_event_activated_set(obj, EINA_FALSE);
}

void RotaryManager::remove(Evas_Object* object)
{
    evas_object_event_callback_del(object, EVAS_CALLBACK_DEL, _objDelCb);
    _remove(object);
    _activateLastObject();
}



