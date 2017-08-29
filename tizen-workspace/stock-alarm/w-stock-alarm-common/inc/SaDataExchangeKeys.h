/*
 * SaDataExchangeKeys.h
 *
 *  Created on: Aug 12, 2017
 *      Author: devbin
 */

#ifndef SADATAEXCHANGEKEYS_H_
#define SADATAEXCHANGEKEYS_H_

#include "WDefine.h"
class WAPP_ASSIST_EXPORT SaDataExchangeKeys
{
public:
    SaDataExchangeKeys();
    virtual ~SaDataExchangeKeys();

	static constexpr char const *networkError = "networkError";
    static constexpr char const *analysEstimate = "analystestimate";
    static constexpr char const *balanceSheet = "balancesheet";
    static constexpr char const *cashFlow = "cashflow";
    static constexpr char const *companyInfo = "com-info";
    static constexpr char const *dividenHistory = "dividenhistory";
    static constexpr char const *historicalData = "historycaldata";
    static constexpr char const *incomeStatement = "incomestatement";
    static constexpr char const *industry = "industry";
    static constexpr char const *isin = "isin";
    static constexpr char const *keyStats = "keystats";
    static constexpr char const *onVista = "onvista";
    static constexpr char const *optionContracts = "option_contracts";
    static constexpr char const *options = "options";
    static constexpr char const *quant = "quant";
    static constexpr char const *quant2 = "quant2";
    static constexpr char const *quotes = "quotes";
    static constexpr char const *quotesList = "quoteslist";
    static constexpr char const *sectors = "sectors";
    static constexpr char const *stocks = "stocks";
    static constexpr char const *xchange = "xchange";
};

#endif /* SADATAEXCHANGEKEYS_H_ */
