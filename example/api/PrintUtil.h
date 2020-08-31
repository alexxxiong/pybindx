#pragma once

#include "DemoApi.h"
#include <sstream>

namespace Demo
{

std::string etos(const EtaBarTimespan &etaBarTimespan);

std::string etos(const EtaPriceMode &etaPriceMode);

std::string etos(const EtaPositionSide &etaPositionSide);

std::string etos(const EtaOrderSide &etaOrderSide);

std::string etos(const EtaPositionEffect &etaPositionEffect);

std::string etos(const EtaOrderType &etaOrderType);

std::string etos(const EtaOrdStatus &etaOrdStatus);

std::string etos(const EtaTimeInForce &etaTimeInForce);

std::string etos(const EtaExecType &etaExecType);

std::string writeToJsonString(const EtaBar& arg);

std::string writeToJsonString(const EtaStrategyInfo& arg);

std::string writeToJsonString(const EtaQtyPrice& arg);

std::string writeToJsonString(const EtaQuote& arg);

std::string writeToJsonString(const EtaAccount& arg);

std::string writeToJsonString(const EtaOverallPosition& arg);

std::string writeToJsonString(const EtaRefData& arg);

std::string writeToJsonString(const EtaStrategyPnL& arg);

std::string writeToJsonString(const EtaBackTestResult& arg);

std::string writeToJsonString(const EtaStrategyExitInfo& arg);

std::string writeToJsonString(const EtaSymbolPosition& arg);

std::string writeToJsonString(const EtaSymbolPositionList& arg);

std::string writeToJsonString(const EtaInstrument& arg);

std::string writeToJsonString(const EtaBackTestParam& arg);

std::string writeToJsonString(const EtaMarketParam& arg);

std::string writeToJsonString(const EtaSymbolField& arg);

std::string writeToJsonString(const EtaOrderStatusUpdate& arg);

std::string writeToJsonString(const EtaOrderUpdate& arg);

std::string writeToJsonString(const EtaOrderExecReport& arg);

template<class T>
std::string writeToJsonString(const T *v, int size)
{
    std::ostringstream oss;
    bool firstFlag = true;
    oss << "[ ";
    for (int i = 0; i < size; ++i)
    {
        if (firstFlag) firstFlag = false;
        else
            oss << ", ";
        oss << writeToJsonString(v[i]);
    }

    oss << "]";

    return oss.str();
}

}