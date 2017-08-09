/*
 * SaProvider.h
 *
 *  Created on: Jul 27, 2017
 *      Author: devbin
 */

#ifndef SAPROVIDER_H_
#define SAPROVIDER_H_

#include <string>

class SaProvider
{
    public:
        static SaProvider* getInstance();
        static void destroy();

        bool initialize();

        bool getFinanceQuates(const std::string& symbol);
        bool getFinanceQuatesList(const std::string& symbol);

    private:
        bool _execute(const std::string& query);
        static size_t _onDataReceived(void* contents, size_t size, size_t nmemb, void* userData);

    private:
        SaProvider();
        virtual ~SaProvider();

    private:
        static SaProvider *_instance;
};

#endif /* SAPROVIDER_H_ */
