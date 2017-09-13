/*
 * SaDataConsumer.cpp
 *
 *  Created on: Aug 12, 2017
 *      Author: devbin
 */

#include "SaDataConsumer.h"
#include "SaDataExchangeKeys.h"
#include "SaCommonDebug.h"
#include "SaCompanyDBManager.h"
#include "SaTypes.h"
#include "SaUtility.h"
#include "SaMessageEventManager.h"

#include <message_port.h>
#include <bundle.h>
#include <vector>
SaDataConsumer* SaDataConsumer::_instance = nullptr;

SaDataConsumer::SaDataConsumer()
{
    // TODO Auto-generated constructor stub
    _providerMap = nullptr;

    _functionTable[SaDataExchangeKeys::networkError] = SaDataConsumerHelper::parseNetworkError;

    _functionTable[SaDataExchangeKeys::analysEstimate] = SaDataConsumerHelper::parseFinanceAnalysEstimate;
    _functionTable[SaDataExchangeKeys::balanceSheet] = SaDataConsumerHelper::parseFinanceBalanceSheet;
    _functionTable[SaDataExchangeKeys::cashFlow] = SaDataConsumerHelper::parseFinanceCashFlow;
    _functionTable[SaDataExchangeKeys::companyInfo] = SaDataConsumerHelper::parseFinanceComInfo;
    _functionTable[SaDataExchangeKeys::dividenHistory] = SaDataConsumerHelper::parseFinanceDividenHistory;
    _functionTable[SaDataExchangeKeys::historicalData] = SaDataConsumerHelper::parseFinanceHistorycalData;
    _functionTable[SaDataExchangeKeys::incomeStatement] = SaDataConsumerHelper::parseFinanceIncomeHistoricalData;
    _functionTable[SaDataExchangeKeys::industry] = SaDataConsumerHelper::parseFinanceIndustry;
    _functionTable[SaDataExchangeKeys::isin] = SaDataConsumerHelper::parseFinanceISIN;
    _functionTable[SaDataExchangeKeys::keyStats] = SaDataConsumerHelper::parseFinanceKeyStats;
    _functionTable[SaDataExchangeKeys::onVista] = SaDataConsumerHelper::parseFinanceOnVista;
    _functionTable[SaDataExchangeKeys::optionContracts] = SaDataConsumerHelper::parseFinanceOptionContracts;
    _functionTable[SaDataExchangeKeys::options] = SaDataConsumerHelper::parseFinanceOptions;
    _functionTable[SaDataExchangeKeys::quant] = SaDataConsumerHelper::parseFinanceQuant;
    _functionTable[SaDataExchangeKeys::quant2] = SaDataConsumerHelper::parseFinanceQuant2;
    _functionTable[SaDataExchangeKeys::quotes] = SaDataConsumerHelper::parseFinanceQuates;
    _functionTable[SaDataExchangeKeys::quotesList] = SaDataConsumerHelper::parseFinanceQuatesList;
    _functionTable[SaDataExchangeKeys::sectors] = SaDataConsumerHelper::parseFinanceSectors;
    _functionTable[SaDataExchangeKeys::stocks] = SaDataConsumerHelper::parseFinanceStocks;
    _functionTable[SaDataExchangeKeys::xchange] = SaDataConsumerHelper::parseFinanceXChange;
    _functionTable[SaDataExchangeKeys::allinfo] = SaDataConsumerHelper::parseFinanceAllInfo;
}

SaDataConsumer::~SaDataConsumer()
{
    // TODO Auto-generated destructor stub
}

SaDataConsumer* SaDataConsumer::getInstance()
{
    if (!_instance)
        _instance = new SaDataConsumer();

    return _instance;
}

void SaDataConsumer::destroy()
{
    if (_instance)
        delete _instance;
    _instance = nullptr;
}

int SaDataConsumer::initialize()
{
    WENTER();

    do {
        // register data control
        int ret = DATA_CONTROL_ERROR_NONE;

        ret = data_control_map_create(&_providerMap);
        if (ret != DATA_CONTROL_ERROR_NONE)
        {
            WERROR("data_control_map_create failed. (%d)", ret);
            return -1;
        }

        char const *providerId = "http://w-stock-alarm-service.com/datacontrol/provider/w-stock-alarm-service";

        ret = data_control_map_set_provider_id(_providerMap, providerId);
        if (ret != DATA_CONTROL_ERROR_NONE)
        {
            WERROR("data_control_map_set_provider_id failed.(%d)", ret);
            return -1;
        }

        const char *mapDataId = "w-stock-alarm-table";
        ret = data_control_map_set_data_id(_providerMap, mapDataId);
        if (ret != DATA_CONTROL_ERROR_NONE)
        {
            WERROR("data_control_map_set_data_id failed.(%d)", ret);
            return -1;
        }

        data_control_map_response_cb mapConsumerCb;
        mapConsumerCb.get_cb = _onDataGetResponse;
        mapConsumerCb.set_cb = _onDataSetResponse;
        mapConsumerCb.add_cb = _onDataAddResponse;
        mapConsumerCb.remove_cb = _onDataDelResponse;

        ret = data_control_map_register_response_cb(_providerMap, &mapConsumerCb, this);
        if (ret != DATA_CONTROL_ERROR_NONE)
        {
            WERROR("data_control_map_register_response_cb failed.(%d)", ret);

            if (ret == DATA_CONTROL_ERROR_IO_ERROR)
                WERROR("I/O error");
            else
                WERROR("Out of memory");
            return -1;
        }

        WINFO("register data control success.");
    } while (0);

    do {
        // register message port
        int ret = MESSAGE_PORT_ERROR_NONE;
        const char *portName = "w-stock-alarm";
        bool isExist = false;

        //ret = message_port_check_remote_port("org.example.datacontrolprovider", portName, &isExist);
        ret = message_port_check_remote_port("team.tizenia.w-stock-alarm", portName, &isExist);
        if (ret != MESSAGE_PORT_ERROR_NONE)
        {
            WERROR("message_port_check_remote_port failed.");
            return -1;
        }
        else
        {
            WINFO("message port exist : %s", isExist ? "true" : "false");
        }

        int portId = message_port_register_local_port(portName,
                [](int local_port_id, const char *remote_app_id, const char *remote_port, bool trusted_remote_port, bundle *message, void *user_data)
                {
                    WHIT();
                    auto self = (SaDataConsumer *)user_data;

                    WDEBUG("local_port_id : %d, remote_app_id : %s, remote_port : %s",
                            local_port_id, remote_app_id, remote_port);

                    char *key = nullptr;
                    char *symbol = nullptr;
                    char *result = nullptr;
                    char *errorMessage = nullptr;
                    bundle_get_str(message, "key", &key);
                    bundle_get_str(message, "symbol", &symbol);
                    bundle_get_str(message, "result", &result);
                    bundle_get_str(message, "errorMessage", &errorMessage);

                    WINFO("message port message : key[%s], symbol[%s], result[%s], errorMessage[%s]",
                        key, symbol, result, errorMessage);
                    //self->_functionTable[key](message);
#if 0
                    SaCompanyInfo companyInfo;
                    SaCompanyDBManager::getInstance()->getCompanyInfo("005930", companyInfo);
                    WINFO("companyinfo : %s %s %s %s %s %s %s %s %s",
                        companyInfo.code.c_str(),
                        companyInfo.name.c_str(),
                        companyInfo.market.c_str(),
                        companyInfo.price.c_str(),
                        companyInfo.max.c_str(),
                        companyInfo.min.c_str(),
                        companyInfo.volume.c_str(),
                        companyInfo.previous.c_str(),
                        companyInfo.histroicaldata.c_str());


                    bundle *b = bundle_decode((const bundle_raw *)companyInfo.histroicaldata.c_str(), companyInfo.histroicaldata.size());
                    std::vector<HistoricalData> historicalVec;
                    SaUtility::convertBundle2HistoricalDataVec(b, historicalVec);

                    for (int i = 0; i < historicalVec.size(); ++i)
                    {
                        WDEBUG("[%3d] : %s %s %s", i, historicalVec[i].date.c_str(), historicalVec[i].closedPrice.c_str(), historicalVec[i].volume.c_str());
                    }
#endif
                    SaMessageEventManager::getInstance()->propagateEvent(key, symbol, result, errorMessage);
                }, this);

        if (portId < 0)
        {
            WERROR("message_port_register_local_port failed. portId(%d)", portId);
        }
        else
        {
            WINFO("message_port_register_local_port success. portId(%d)", portId);
        }
    } while (0);
    return 0;
}
int SaDataConsumer::finalize()
{
    WENTER();

    int ret = DATA_CONTROL_ERROR_NONE;

    ret = data_control_map_destroy(_providerMap);
    if (ret != DATA_CONTROL_ERROR_NONE)
    {
        WERROR("data_control_map_destroy failed.(%d)", ret);
        return -1;
    }
    return 0;
}

int SaDataConsumer::request(const char *key)
{
    WENTER();

    WDEBUG("key[%s] request!", key);
    int ret = DATA_CONTROL_ERROR_NONE;
    int reqId = 0;

    //ret = data_control_map_get(_providerMap, key, &reqId);
    ret = data_control_map_add(_providerMap, key, "encodedBundle", &reqId);
    if (ret != DATA_CONTROL_ERROR_NONE)
    {
        WERROR("data_control_map_get failed.(%d)", ret);
        return -1;
    }

    return 0;
}

int SaDataConsumer::requestFinanceQuates(const char *symbol)
{
    WENTER();

    int ret = DATA_CONTROL_ERROR_NONE;
    int reqId = 0;

    bundle *b = bundle_create();
    bundle_add_str(b, "key", SaDataExchangeKeys::quotesList);
    bundle_add_str(b, "symbol", symbol);
    bundle_raw *r = nullptr;
    int len = 0;
    bundle_encode(b, &r, &len);
    bundle_free(b);

    ret = data_control_map_add(_providerMap, SaDataExchangeKeys::quotesList, (const char *)r, &reqId);
    bundle_free(b);
    free(r);

    if (ret != DATA_CONTROL_ERROR_NONE)
    {
        WERROR("data_control_map_get failed.(%d)", ret);
        return -1;
    }

    return 0;
}

int SaDataConsumer::requestFinanceQuatesList(const char *symbol)
{
    WENTER();

    int ret = DATA_CONTROL_ERROR_NONE;
    int reqId = 0;

    bundle *b = bundle_create();
    bundle_add_str(b, "symbol", symbol);
    bundle_raw *r = nullptr;
    int len = 0;
    bundle_encode(b, &r, &len);
    WINFO("data : %s, len : %d", r, len);

    ret = data_control_map_add(_providerMap, SaDataExchangeKeys::quotesList, (const char *)r, &reqId);
    bundle_free(b);
    free(r);

    if (ret != DATA_CONTROL_ERROR_NONE)
    {
        WERROR("data_control_map_add failed.(%d)", ret);
        return -1;
    }

    return 0;
}

int SaDataConsumer::requestFinanceHistoricalData(const char *symbol)
{
    WENTER();

    int ret = DATA_CONTROL_ERROR_NONE;
    int reqId = 0;

    bundle *b = bundle_create();
    bundle_add_str(b, "symbol", symbol);
    bundle_raw *r = nullptr;
    int len = 0;
    bundle_encode(b, &r, &len);
    WINFO("data : %s, len : %d", r, len);

    ret = data_control_map_add(_providerMap, SaDataExchangeKeys::historicalData, (const char *)r, &reqId);
    bundle_free(b);
    free(r);

    if (ret != DATA_CONTROL_ERROR_NONE)
    {
        WERROR("data_control_map_add failed.(%d)", ret);
        return -1;
    }

    return 0;
}

int SaDataConsumer::reqeuestFinanceAllInfo(const char* symbol)
{
	 WENTER();

	 int ret = DATA_CONTROL_ERROR_NONE;
	 int reqId = 0;

	 bundle *b = bundle_create();
	 bundle_add_str(b, "symbol", symbol);
	 bundle_raw *r = nullptr;
	 int len = 0;
	 bundle_encode(b, &r, &len);
	 WINFO("data : %s, len : %d", r, len);

	 ret = data_control_map_add(_providerMap, SaDataExchangeKeys::allinfo, (const char *)r, &reqId);
	 bundle_free(b);
	 free(r);

	 if (ret != DATA_CONTROL_ERROR_NONE)
	 {
		 WERROR("data_control_map_add failed.(%d)", ret);
		 return -1;
	 }
	 return 0;
}

void SaDataConsumer::_onDataGetResponse(int request_id, data_control_h provider, char **ret_value_list, int ret_value_count, bool provider_result, const char *error, void *user_data)
{
    WENTER();
}

void SaDataConsumer::_onDataAddResponse(int request_id, data_control_h provider, bool provider_result, const char *error, void *user_data)
{
    WENTER();
}

void SaDataConsumer::_onDataDelResponse(int request_id, data_control_h provider, bool provider_result, const char *error, void *user_data)
{
    WENTER();
}

void SaDataConsumer::_onDataSetResponse(int request_id, data_control_h provider, bool provider_result, const char *error, void *user_data)
{
    WENTER();
}
