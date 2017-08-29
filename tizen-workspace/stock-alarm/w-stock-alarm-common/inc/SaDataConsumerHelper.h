/*
 * SaDataConsumerHelper.h
 *
 *  Created on: Aug 12, 2017
 *      Author: devbin
 */

#ifndef SADATACONSUMERHELPER_H_
#define SADATACONSUMERHELPER_H_

#include "WDefine.h"
#include <bundle.h>
class WAPP_ASSIST_EXPORT SaDataConsumerHelper
{
public:
    SaDataConsumerHelper();
    virtual ~SaDataConsumerHelper();

	static void parseNetworkError(bundle *b);
    static void parseFinanceAnalysEstimate(bundle *b);
    static void parseFinanceBalanceSheet(bundle *b);
    static void parseFinanceCashFlow(bundle *b);
    static void parseFinanceComInfo(bundle *b);
    static void parseFinanceDividenHistory(bundle *b);
    static void parseFinanceHistorycalData(bundle *b);
    static void parseFinanceIncomeHistoricalData(bundle *b);
    static void parseFinanceIndustry(bundle *b);
    static void parseFinanceISIN(bundle *b);
    static void parseFinanceKeyStats(bundle *b);
    static void parseFinanceOnVista(bundle *b);
    static void parseFinanceOptionContracts(bundle *b);
    static void parseFinanceOptions(bundle *b);
    static void parseFinanceQuant(bundle *b);
    static void parseFinanceQuant2(bundle *b);

    static void parseFinanceQuates(bundle *b);
    static void parseFinanceQuatesList(bundle *b);

    static void parseFinanceSectors(bundle *b);
    static void parseFinanceStocks(bundle *b);
    static void parseFinanceXChange(bundle *b);
};

#endif /* SADATACONSUMERHELPER_H_ */
