/*
 * SaDataConsumer.h
 *
 *  Created on: Aug 12, 2017
 *      Author: devbin
 */

#ifndef SADATACONSUMER_H_
#define SADATACONSUMER_H_

#include <data_control.h>
#include <bundle.h>
#include <string>
#include <unordered_map>

#include "WDefine.h"
#include "SaDataConsumerHelper.h"

class WAPP_ASSIST_EXPORT SaDataConsumer
{
public:
    static SaDataConsumer* getInstance();
    static void destroy();

public:
    int initialize();
    int finalize();

    int request(const char *key);

    int requestFinanceQuates(const char *symbol);
    int requestFinanceQuatesList(const char *symbol);

private:
    SaDataConsumer();
    virtual ~SaDataConsumer();

    static void _onDataGetResponse(int request_id, data_control_h provider, char **ret_value_list, int ret_value_count, bool provider_result, const char *error, void *user_data);
    static void _onDataAddResponse(int request_id, data_control_h provider, bool provider_result, const char *error, void *user_data);
    static void _onDataDelResponse(int request_id, data_control_h provider, bool provider_result, const char *error, void *user_data);
    static void _onDataSetResponse(int request_id, data_control_h provider, bool provider_result, const char *error, void *user_data);

private:
    typedef void (*helperFunction)(bundle *b);

private:
    static SaDataConsumer *_instance;
    data_control_h _providerMap;

    std::unordered_map<std::string, helperFunction> _functionTable;
};

#endif /* SADATACONSUMER_H_ */
