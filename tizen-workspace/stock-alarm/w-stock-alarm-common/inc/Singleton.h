/*
 * Singleton.h
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "WDefine.h"

#define SINGLETON_IDIOM(TYPE)\
    public:\
        static TYPE* getInstance()\
        {\
            if (_pInstance == nullptr) {\
                _pInstance = new TYPE;\
            }\
            return _pInstance;\
        };\
        static void destroy()\
        {\
            delete _pInstance;\
            _pInstance = nullptr;\
        };\
    private:\
        static TYPE* _pInstance;

#define SINGLETON_INSTANCE(TYPE)\
    TYPE* TYPE::_pInstance = nullptr;

#endif /* _SINGLETON_H_ */
