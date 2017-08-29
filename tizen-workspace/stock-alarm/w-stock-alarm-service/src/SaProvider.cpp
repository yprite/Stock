/*
 * SaProvider.cpp
 *
 *  Created on: Jul 27, 2017
 *      Author: devbin
 */

#include "SaProvider.h"
#include "SaServiceDebug.h"
#include <curl/curl.h>
#include <thread>
#include <mutex>
#include <vector>
#include <data_control.h>

SaProvider* SaProvider::_instance = nullptr;

SaProvider* SaProvider::getInstance()
{
    if (_instance == nullptr)
        _instance = new SaProvider();

    return _instance;
}

void SaProvider::destroy()
{
    if (_instance)
        delete _instance;

    _instance = nullptr;
}

SaProvider::SaProvider()
{
    // TODO Auto-generated constructor stub
    curl_global_init(CURL_GLOBAL_ALL);
}

SaProvider::~SaProvider()
{
    // TODO Auto-generated destructor stub
    curl_global_cleanup();
}

bool SaProvider::initialize()
{
    WENTER();

    // load database and read all query.
    static const char *queryList[] = {
            //"APPL",
            //"GOOGL",
            "ESL.TO",
            "CHT",
            "ANDE",
            "JCS",
            "ONCS",
            "LVRT.L",
            "SOHOL",
            nullptr
    };

//    std::vector<std::thread> workerList;
    //std::mutex mtx;
    //mtx.lock();
    for (int i = 0; queryList[i] != nullptr; ++i)
    {
        WDEBUG("queryList[i] : %s", queryList[i]);
        std::thread worker(
                [this](const std::string& symbol)
                {
                    WHIT();
                    getFinanceQuatesList(symbol);
                    //return false;
                }, queryList[i]);

        worker.join();
        //workerList[i].join();
        //workerList.push_back(std::move(worker));
        WDEBUG("END !");
    }
//    for (int i = 0; i < workerList.size(); ++i)
//        workerList[i].join();

    //mtx.unlock();
    WLEAVE();

}

bool SaProvider::getFinanceQuates(const std::string& symbol)
{
    WENTER();
    std::string query = "select * from yahoo.finance.quotes where symbol='" + symbol + "'";
    return _execute(query);
}

bool SaProvider::getFinanceQuatesList(const std::string& symbol)
{
    WENTER();
    std::string query = "select * from yahoo.finance.quoteslist where symbol='" + symbol + "'";
    return _execute(query);
}

bool SaProvider::_execute(const std::string& query)
{
    WENTER();
    const std::string baseUrl = "http://query.yahooapis.com/v1/public/yql";
    const std::string envCmd = "&env=store://datatables.org/alltableswithkeys";
    std::string strRequest = "q=" + query + envCmd + "&format=json";

    CURL *curl = nullptr;
    std::string response;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strRequest.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _onDataReceived);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode ret = curl_easy_perform(curl);
        WINFO("response : %s", response.c_str());
        curl_easy_cleanup(curl);
        return ret == CURLE_OK;
    }
    return false;
}

size_t SaProvider::_onDataReceived(void* contents, size_t size, size_t nmemb, void* userData)
{
    ((std::string *)userData)->append((char *)contents, size * nmemb);

    return size * nmemb;
}
