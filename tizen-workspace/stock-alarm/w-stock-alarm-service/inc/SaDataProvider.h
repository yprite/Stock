/*
 * SaDataProvider.h
 *
 *  Created on: Aug 10, 2017
 *      Author: devbin
 */

#ifndef SADATAPROVIDER_H_
#define SADATAPROVIDER_H_

#include <data_control.h>
#include <unordered_map>
#include <string>

#include "SaDataProviderHelper.h"

class SaDataProvider
{
public:
    static SaDataProvider* getInstance();
    static void destroy();

public:
    int initialize();
    int finalize();

private:
    SaDataProvider();
    virtual ~SaDataProvider();

    static void _onDataGetRequested(int reqId, data_control_h provider, const char *key, void *userData);
    static void _onDataAddRequested(int reqId, data_control_h provider, const char *key, const char *value, void *userData);
    static void _onDataDelRequested(int reqId, data_control_h provider, const char *key, const char *value, void *userData);
    static void _onDataSetRequested(int reqId, data_control_h provider, const char *key, const char *oldValue, const char *newValue, void *userData);

private:
    typedef bundle* (*helperFunction)(const char* encodedBundle);

private:
    static SaDataProvider *_instance;
    //data_control_h _provider;
    data_control_provider_map_cb _mapProviderCb;

    std::unordered_map<std::string, helperFunction> _functionTable;
};

#endif /* SADATAPROVIDER_H_ */
