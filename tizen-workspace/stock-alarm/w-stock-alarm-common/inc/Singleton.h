/*
 * Singleton.h
 *
 *  Created on: Aug 21, 2017
 *      Author: devbin
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "WDefine.h"

#define SINGLETON_IDIOM(CLASSNAME) \
    private: \
        CLASSNAME(const CLASSNAME&) = delete; \
        CLASSNAME& operator=(const CLASSNAME&) = delete; \
        \
    public: \
        static CLASSNAME* getInstance() \
        { \
            if (!_instance) \
                _instance = new CLASSNAME; \
            return _instance; \
        } \
        static void destroy() \
        { \
            if (_instance) \
                delete _instance; \
            _instance = nullptr; \
        } \
    private: \
        static CLASSNAME* _instance; \


#define SINGLETON_INSTANCE(CLASSNAME) \
    CLASSNAME* CLASSNAME::_instance = nullptr; \

#endif /* SINGLETON_H_ */
