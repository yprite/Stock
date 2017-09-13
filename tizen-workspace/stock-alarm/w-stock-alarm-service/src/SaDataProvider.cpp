#include "SaDataProvider.h"
#include "SaDataExchangeKeys.h"
#include "NetConnection.h"
#include "SaServiceDebug.h"

#include <curl/curl.h>
#include <glib.h>
#include <message_port.h>

SaDataProvider* SaDataProvider::_instance = nullptr;

static char const *gAppList[] =
{
    "team.tizenia.w-stock-alarm",
    "team.tizenia.w-stock-alarm-widget",
    nullptr
};

SaDataProvider::SaDataProvider()
{
    // TODO Auto-generated constructor stub
    _mapProviderCb.add_cb = nullptr;
    _mapProviderCb.get_cb = nullptr;
    _mapProviderCb.remove_cb = nullptr;
    _mapProviderCb.set_cb = nullptr;

    _functionTable[SaDataExchangeKeys::networkError] = SaDataProviderHelper::getNetworkError;

    _functionTable[SaDataExchangeKeys::analysEstimate] = SaDataProviderHelper::getFinanceAnalystEstimate;
    _functionTable[SaDataExchangeKeys::balanceSheet] = SaDataProviderHelper::getFinanceBalanceSheet;
    _functionTable[SaDataExchangeKeys::cashFlow] = SaDataProviderHelper::getFinanceCashFlow;
    _functionTable[SaDataExchangeKeys::companyInfo] = SaDataProviderHelper::getFinanceComInfo;
    _functionTable[SaDataExchangeKeys::dividenHistory] = SaDataProviderHelper::getFinanceDividenHistory;
    _functionTable[SaDataExchangeKeys::historicalData] = SaDataProviderHelper::getFinanceHistoricalData;
    _functionTable[SaDataExchangeKeys::incomeStatement] = SaDataProviderHelper::getFinanceIncomeHistoricalData;
    _functionTable[SaDataExchangeKeys::industry] = SaDataProviderHelper::getFinanceIndustry;
    _functionTable[SaDataExchangeKeys::isin] = SaDataProviderHelper::getFinanceISIN;
    _functionTable[SaDataExchangeKeys::keyStats] = SaDataProviderHelper::getFinanceKeyStats;
    _functionTable[SaDataExchangeKeys::onVista] = SaDataProviderHelper::getFinanceOnVista;
    _functionTable[SaDataExchangeKeys::optionContracts] = SaDataProviderHelper::getFinanceOptionContracts;
    _functionTable[SaDataExchangeKeys::options] = SaDataProviderHelper::getFinanceOptions;
    _functionTable[SaDataExchangeKeys::quant] = SaDataProviderHelper::getFinanceQuant;
    _functionTable[SaDataExchangeKeys::quant2] = SaDataProviderHelper::getFinanceQuant2;
    _functionTable[SaDataExchangeKeys::quotes] = SaDataProviderHelper::getFinanceQuates;
    _functionTable[SaDataExchangeKeys::quotesList] = SaDataProviderHelper::getFinanceQuatesList;
    _functionTable[SaDataExchangeKeys::sectors] = SaDataProviderHelper::getFinanceSectors;
    _functionTable[SaDataExchangeKeys::stocks] = SaDataProviderHelper::getFinanceStocks;
    _functionTable[SaDataExchangeKeys::xchange] = SaDataProviderHelper::getFinanceXChange;
    _functionTable[SaDataExchangeKeys::allinfo] = SaDataProviderHelper::getFinanceAllInfo;
}

SaDataProvider::~SaDataProvider()
{
    // TODO Auto-generated destructor stub
}

SaDataProvider* SaDataProvider::getInstance()
{
    if (!_instance)
        _instance = new SaDataProvider();
    return _instance;
}

void SaDataProvider::destroy()
{
    if (_instance)
        delete _instance;
    _instance = nullptr;
}

int SaDataProvider::initialize()
{
    WENTER();

    int ret = DATA_CONTROL_ERROR_NONE;

    _mapProviderCb.get_cb = _onDataGetRequested;
    _mapProviderCb.add_cb = _onDataAddRequested;
    _mapProviderCb.remove_cb = _onDataDelRequested;
    _mapProviderCb.set_cb = _onDataSetRequested;

    ret = data_control_provider_map_register_cb(&_mapProviderCb, (void *)this);
    if (ret != DATA_CONTROL_ERROR_NONE)
    {
        WERROR("data_control_provider_map_register_cb failed.(%d)", ret);
        return -1;
    }
    else
    {
        WINFO("initialize success.");
    }

    return 0;
}

int SaDataProvider::finalize()
{
    WENTER();

    int ret = DATA_CONTROL_ERROR_NONE;

    ret = data_control_provider_map_unregister_cb();
    if (ret != DATA_CONTROL_ERROR_NONE)
        WERROR("data_control_provider_map_unregister_cb failed.(%d)", ret);
    else
        WINFO("finalize success.");

    return ret;
}

void SaDataProvider::_onDataGetRequested(int reqId, data_control_h provider, const char *key, void *userData)
{
    WENTER();
    WDEBUG("reqId : %d, key : %s", reqId, key);

#if 0
    int ret = DATA_CONTROL_ERROR_NONE;
    int valueArrSize = 1;
    char **valueArr = (char **)calloc(valueArrSize, sizeof(char *));
    valueArr[0] = g_strdup("requested value!");

    ret = data_control_provider_send_map_get_value_result(reqId, valueArr, valueArrSize);
    if (ret != DATA_CONTROL_ERROR_NONE)
        WERROR("data_control_provider_send_map_get_value_result failed.(%d)", ret);
    else
        WINFO("data_control_provider_send_map_get_value_result success!");
#endif
#if 0
    int ret = MESSAGE_PORT_ERROR_NONE;
    static char const *portName = "w-stock-alarm";

    bundle *b = bundle_create();
    bundle_add_str(b, "bundle_key", "bundle_value");
    bundle_add_str(b, "bundle_key2", "bundle_value2");
    for (int i = 0; gAppList[i] != nullptr; ++i)
    {
        ret = message_port_send_message(gAppList[i], portName, b);

        if (ret != MESSAGE_PORT_ERROR_NONE)
            WERROR("message_port_send_message failed.(%d)", ret);
        else
            WINFO("message_port_send_message success.");
    }

    bundle_free(b);
#endif
}

void SaDataProvider::_onDataAddRequested(int reqId, data_control_h provider, const char *key, const char *value, void *userData)
{
    WENTER();

    // key : request api.
    // value - encoded bundle data.
    WINFO("key : %s, value : %s", key, value);

    auto self = (SaDataProvider *)userData;
    auto p = self->_functionTable.find(key);

    if (p == self->_functionTable.end())
    {
        WERROR("Cannot Find key (%s)", key);
        return;
    }

    bundle *resultB = nullptr;
    NetConnection::NetState netState = NetConnection::getInstance()->getNetworkState();
    if (netState == NetConnection::NetState::DISCONNECTED)
    {
        auto func = self->_functionTable[SaDataExchangeKeys::networkError];
        resultB = func(nullptr);
    }
    else
    {
        resultB = p->second(value);
    }

    if (resultB == nullptr)
        WERROR("b is nullptr!");
    else
        WINFO("success!");

    static char const *portName = "w-stock-alarm";

    for (int i = 0; gAppList[i] != nullptr; ++i)
    {
        int r = message_port_send_message(gAppList[i], portName, resultB);

        if (r != MESSAGE_PORT_ERROR_NONE)
            WERROR("message_port_send_message failed.(%d) - appid : %s", r, gAppList[i]);
        else
            WINFO("message_port_send_message success.");
    }
    bundle_free(resultB);
}

void SaDataProvider::_onDataDelRequested(int reqId, data_control_h provider, const char *key, const char *value, void *userData)
{
    WENTER();
}

void SaDataProvider::_onDataSetRequested(int reqId, data_control_h provider, const char *key, const char *oldValue, const char *newValue, void *userData)
{
    WENTER();
}
