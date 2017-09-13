/*
 * SaDataProviderHelper.cpp
 *
 *  Created on: Aug 12, 2017
 *      Author: devbin
 */

#include "SaDataProviderHelper.h"
#include "SaCompanyDBManager.h"
#include "SaDataExchangeKeys.h"
#include "SaServiceDebug.h"
#include "NetConnection.h"
#include "SaUtility.h"
#include "SaTypes.h"

#include <bundle.h>
#include <curl/curl.h>
#include <string>
#include <vector>

#include <libxml/HTMLparser.h>

#define HISTORICAL_SIZE 31
#define COMPARE(a, b) (!strcasecmp((a), (b)))

struct Context
{
    Context(): addTitle(false) {}

    bool addTitle;
    std::string title;
};
static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

static void StartElement(void *voidContext,
                         const xmlChar *name,
                         const xmlChar **attributes)
{
  Context *context = (Context *)voidContext;

  if(COMPARE((char *)name, "table")) {
    context->title = "";
    context->addTitle = true;
  }
  (void) attributes;
}
static void EndElement(void *voidContext,
                       const xmlChar *name)
{
  Context *context = (Context *)voidContext;

  if(COMPARE((char *)name, "table"))
    context->addTitle = false;
}
static void handleCharacters(Context *context,
                             const xmlChar *chars,
                             int length)
{
  if(context->addTitle)
    context->title.append((char *)chars, length);
}
static void Characters(void *voidContext,
                       const xmlChar *chars,
                       int length)
{
  Context *context = (Context *)voidContext;

  handleCharacters(context, chars, length);
}
static void cdata(void *voidContext,
                  const xmlChar *chars,
                  int length)
{
  Context *context = (Context *)voidContext;

  handleCharacters(context, chars, length);
}
static htmlSAXHandler saxHandler =
{
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  StartElement,
  EndElement,
  NULL,
  Characters,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  cdata,
  NULL
};
static void parseHtml(const std::string &html, std::string &title)
{
    WENTER();
    htmlParserCtxtPtr ctxt;
    Context context;

    ctxt = htmlCreatePushParserCtxt(&saxHandler, &context, "", 0, "", XML_CHAR_ENCODING_NONE);

    htmlParseChunk(ctxt, html.c_str(), html.size(), 0);
    htmlParseChunk(ctxt, "", 0, 1);

    htmlFreeParserCtxt(ctxt);

    title = context.title;
}

SaDataProviderHelper::SaDataProviderHelper()
{
    // TODO Auto-generated constructor stub

}

SaDataProviderHelper::~SaDataProviderHelper()
{
    // TODO Auto-generated destructor stub
}

bundle* SaDataProviderHelper::getNetworkError(const char *encodedBundle)
{
    WENTER();

    bundle *resultBundle = bundle_create();
    bundle_add_str(resultBundle, "key", SaDataExchangeKeys::networkError);
    bundle_add_str(resultBundle, "result", "fail");

    return resultBundle;
}


bundle* SaDataProviderHelper::getFinanceAnalystEstimate(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceBalanceSheet(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceCashFlow(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceComInfo(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceDividenHistory(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceHistoricalData(const char *encodedBundle)
{
    WENTER();
    WPRET_VM(encodedBundle == NULL, NULL, "Invalid parameter");
    WINFO("data : %s, len : %d", encodedBundle, strlen(encodedBundle));

    int r =  BUNDLE_ERROR_NONE;
    struct tm* time_info = NULL;
    bundle *b = bundle_decode((const bundle_raw *) encodedBundle, strlen(encodedBundle));
    char *symbol = nullptr;
    std::string market;


    r = bundle_get_str(b, "symbol", &symbol);
    if (r != BUNDLE_ERROR_NONE)
        WERROR("bundle_get_str failed.(%d)", r);
    SaCompanyDBManager::getInstance()->searchByCode(std::string(symbol), market);

    WINFO("request symbol : %s", symbol);
    WINFO("request makret : %s", market.c_str());
    std::string code = std::string(symbol);
    time_t raw_time;
    time(&raw_time);
    raw_time -= (60 * 60 * 24 * 51); // 60sec * 60 min * 24hour * 51 days
    time_info = localtime(&raw_time);

    std::string query;
    //std::string query = std::string(symbol) + "&startdate=" + std::string(SaUtility::strToMonth(time_info->tm_mon)) + "+" + std::to_string(time_info->tm_mday);
#if 0
    if (strcmp(market, "") == 0 || strcmp(market, "KOSPI") == 0)
    {
        query = "KRX:" + std::string(symbol);
    }
    else if (strcmp(market, "KOSAQ") == 0)
    {
        query ="KOSDAQ:" + std::string(symbol);
    }
#endif
    if ( market=="" || market=="KOSPI")
    {
       query = "KRX:" + std::string(symbol);
    }
    else if (market == "KOSDAQ")
    {
       query ="KOSDAQ:" + std::string(symbol);
    }
    else
    {
        bundle_free(b);

        bundle *resultBundle = bundle_create();
        bundle_add_str(resultBundle, "key", SaDataExchangeKeys::historicalData);
        bundle_add_str(resultBundle, "symbol", code.c_str());
        bundle_add_str(resultBundle, "result", "fail");
        bundle_add_str(resultBundle, "errorMessage", "No find market");
        return resultBundle;
    }

    std::string response;
    std::string errorMessage;
    bundle_free(b);

    bool result = SaDataProviderHelper::_executeForHistorycalData(query, response, errorMessage);

    bundle *resultBundle = bundle_create();

    bundle_add_str(resultBundle, "key", SaDataExchangeKeys::historicalData);
    bundle_add_str(resultBundle, "symbol", code.c_str());
    bundle_add_str(resultBundle, "result", result ? "success" : "fail");
    bundle_add_str(resultBundle, "errorMessage", errorMessage.c_str());


    std::string parserResponse;
    std::vector<std::string> vParserRespone;
    std::vector<HistoricalData> vHistoricalData;

    parseHtml(response, parserResponse);
    SaUtility::split(vParserRespone, parserResponse, "\n");
    for(unsigned int i = 9; i < vParserRespone.size(); i+=7)
    {
        WDEBUG("%d\t%s\t%s\t%s", i / 7, vParserRespone[i].c_str(), vParserRespone[i+HISTORICAL_DATA_INDEX_CLOSE].c_str(), vParserRespone[i+ HISTORICAL_DATA_INDEX_VOLUME].c_str());
        vHistoricalData.push_back(HistoricalData{vParserRespone[i], vParserRespone[i+HISTORICAL_DATA_INDEX_CLOSE], vParserRespone[i+ HISTORICAL_DATA_INDEX_VOLUME]});
    }

    //encode bundle
    {
        std::string savingData;
        int totSize = vHistoricalData.size();
        char **valueArr = (char **) calloc(totSize, sizeof(char *));
        for (unsigned int i = 0; i < vHistoricalData.size(); i++)
        {
            bundle *subBundle = SaUtility::convertHistoricalData2Bundle(vHistoricalData[i]);
            bundle_raw *encodedStr = nullptr;
            int len = 0;
            bundle_encode(subBundle, &encodedStr, &len);
            valueArr[i] = strdup((char *) encodedStr);
            bundle_free(subBundle);
            free(encodedStr);
        }

        bundle *b = bundle_create();
        bundle_add_str_array(b, "historicalData", (const char **) valueArr, totSize);
        {
            int len = 0;
            bundle_raw *encodedStr = nullptr;
            bundle_encode(b, &encodedStr, &len);

            savingData = std::string((char *) encodedStr);

            free(encodedStr);
        }
        bundle_free(b);

        for (int i = 0; i < totSize; ++i)
            free(valueArr[i]);
        free(valueArr);

        //WINFO("saving data : %s",savingData.c_str());
        SaCompanyDBManager::getInstance()->saveHistoricalData(code, savingData);
    }

    return resultBundle;
}

bundle* SaDataProviderHelper::getFinanceIncomeHistoricalData(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceIndustry(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceISIN(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceKeyStats(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceOnVista(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceOptionContracts(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceOptions(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceQuant(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceQuant2(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceQuates(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceQuatesList(const char *encodedBundle)
{
    WENTER();
    WINFO("data : %s, len : %d", encodedBundle, strlen(encodedBundle));

    std::vector<std::string> v;
    bundle *b = bundle_decode((const bundle_raw *)encodedBundle, strlen(encodedBundle));
    char *symbol = nullptr;
    std::string market;
    int r = bundle_get_str(b, "symbol", &symbol);
    if (r != BUNDLE_ERROR_NONE)
        WERROR("bundle_get_str failed.(%d)", r);

    SaCompanyDBManager::getInstance()->searchByCode(std::string(symbol), market);

    WINFO("request symbol : %s", symbol);
    WINFO("request market : %s", market.c_str());
    //std::string query = "select * from yahoo.finance.quoteslist where symbol='" + std::string(symbol) + "'";
    std::string code = std::string(symbol);
    std::string query;
    if ( market=="" || market=="KOSPI")
    {
        query = code + ".KS";
    }
    else if (market == "KOSDAQ")
    {
        query = code + ".KQ";
    }
    else
    {
        bundle_free(b);

        bundle *resultBundle = bundle_create();
        bundle_add_str(resultBundle, "key", SaDataExchangeKeys::quotesList);
        bundle_add_str(resultBundle, "symbol", code.c_str());
        bundle_add_str(resultBundle, "result", "fail");
        bundle_add_str(resultBundle, "errorMessage", "No find market");
        return resultBundle;
    }

    std::string response;
    std::string errorMessage;
    bundle_free(b);

    bool result = SaDataProviderHelper::_execute(query, response, errorMessage);

    bundle *resultBundle = bundle_create();

    bundle_add_str(resultBundle, "key", SaDataExchangeKeys::quotesList);
    bundle_add_str(resultBundle, "symbol", query.c_str());
    bundle_add_str(resultBundle, "result", result ? "success" : "fail");
    bundle_add_str(resultBundle, "errorMessage", errorMessage.c_str());

    SaUtility::split(v, response, ",");
    SaCompanyDBManager::getInstance()->saveInfo(code,
        SaUtility::trim(v[INFO_RESULT_INDEX_CURRENT_PRICE]),
        SaUtility::trim(v[INFO_RESULT_INDEX_CHANGE]),
        SaUtility::trim(v[INFO_RESULT_INDEX_PERCENT]),
        SaUtility::trim(v[INFO_RESULT_INDEX_MAX_PRICE]),
        SaUtility::trim(v[INFO_RESULT_INDEX_MIN_PRICE]),
        SaUtility::trim(v[INFO_RESULT_INDEX_PRIVIOUS]),
        SaUtility::trim(v[INFO_RESULT_INDEX_VOLUME]));

    return resultBundle;
}

bundle* SaDataProviderHelper::getFinanceSectors(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceStocks(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceXChange(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceAllInfo(const char *encodedBundle)
{
    WINFO("data : %s, len : %d", encodedBundle, strlen(encodedBundle));

    bundle *b = NULL;

    char *symbol = nullptr;
    int r = BUNDLE_ERROR_NONE;

    b = bundle_decode((const bundle_raw *) encodedBundle, strlen(encodedBundle));
    if (b == NULL)
        WERROR("bundle_decode failed.");

    r = bundle_get_str(b, "symbol", &symbol);
    if (r != BUNDLE_ERROR_NONE)
        WERROR("bundle_get_str failed.(%d)", r);

    WINFO("request symbol : %s", symbol);

    bundle *resultBundle = bundle_create();

    bundle *historicalBundle = getFinanceHistoricalData(encodedBundle);
    if (historicalBundle)
    {
        char *symbol2 = nullptr;
        char *result2 = nullptr;
        char *errorMessage = nullptr;

        bundle_get_str(historicalBundle, "symbol", &symbol2);
        bundle_get_str(historicalBundle, "result", &result2);
        bundle_get_str(historicalBundle, "errorMessage", &errorMessage);

        bundle_add_str(resultBundle, "symbol", symbol2);
        bundle_add_str(resultBundle, "result", result2);
        bundle_add_str(resultBundle, "errorMessage", errorMessage);
        bundle_free(historicalBundle);
    }

    bundle *quatesListBundle = getFinanceQuatesList(encodedBundle);
    if (quatesListBundle)
    {
        free(quatesListBundle);
    }
    bundle_add_str(resultBundle, "key", SaDataExchangeKeys::allinfo);
    return resultBundle;
}

bool SaDataProviderHelper::_execute(const std::string &query, std::string &output, std::string &errorMessage)
{
    WENTER();
    //const std::string baseUrl = "http://query.yahooapis.com/v1/public/yql";
    //const std::string envCmd = "&env=store://datatables.org/alltableswithkeys";
    //std::string strRequest = "q=" + query + envCmd + "&format=json";
    const std::string baseUrl =  "http://download.finance.yahoo.com/d/quotes.csv?s="+ query+"&f=sc1p2bhgpv&e=.csv";


    CURL *curl = nullptr;
    std::string response;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        //curl_easy_setopt(curl, CURLOPT_POST, 1);
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strRequest.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _onDataReceived);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_PROXY, NetConnection::getInstance()->getProxyAddress().c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode ret = curl_easy_perform(curl);
        std::string errMsg = curl_easy_strerror(ret);
        WINFO("baseUrl : %s", baseUrl.c_str());
        WINFO("response : %s", response.c_str());
        WINFO("errMsg : %s", errMsg.c_str());
        output = response;
        curl_easy_cleanup(curl);
        return ret == CURLE_OK;
    }
    return false;
}

bool SaDataProviderHelper::_executeForHistorycalData(const std::string &query, std::string &output, std::string &errorMessage)
{
    WENTER();

    //const std::string baseUrl = "http://www.google.com/finance/historical?q=KRX:" + query + "%2C+2017&output=csv";
    const std::string baseUrl = "http://www.google.com/finance/historical?q="+ query;

    CURL *curl = nullptr;
    std::string response;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _onDataReceived);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_PROXY, NetConnection::getInstance()->getProxyAddress().c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode ret = curl_easy_perform(curl);
        std::string errMsg = curl_easy_strerror(ret);
        WINFO("baseUrl : %s", baseUrl.c_str());
        WINFO("response : %s", response.c_str());
        WINFO("errMsg : %s, err=[%d]", errMsg.c_str(), ret);
        output = response;
        curl_easy_cleanup(curl);
        return ret == CURLE_OK;
    }
    return false;
}

size_t SaDataProviderHelper::_onDataReceived(void* contents, size_t size, size_t nmemb, void* userData)
{
    WENTER();
    ((std::string *)userData)->append((char *)contents, size * nmemb);

    return size * nmemb;
}
