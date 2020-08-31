#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>

#if defined _WIN32 || defined _WIN64
#ifdef DEMO_DLL_EXPORTS
#ifndef DEMO_DLL_API
#define DEMO_DLL_API __declspec(dllexport)
#endif
#else
#ifdef DEMO_USE_DLL
#define DEMO_DLL_API __declspec(dllimport)
#else
#define DEMO_DLL_API
#endif
#endif

#else

#ifndef DEMO_DLL_API
#define DEMO_DLL_API
#endif

#pragma GCC visibility push(default)
#endif

namespace Demo
{

#define SYMBOL_SIZE 24
#define EXCHANGE_SIZE 8
#define MARKET_SIZE 8
#define PRODUCT_SIZE 24
#define CURRENCY_SIZE 8
#define DATEFORMAT_SIZE 24
#define DATESTR_SIZE 8
#define UUID_SIZE 32
#define ERROR_SIZE 64
#define STRATEGY_NAME_SIZE 128

class DEMO_DLL_API FactorDataFrame {
public:
    virtual ~FactorDataFrame() = default;

    static size_t PRINT_LINE_SIZE;
    virtual std::string print() const = 0;
    virtual std::vector<std::string> get_columns() const = 0;
    virtual const std::string & get_index_name() const = 0;
    virtual const std::vector<double>& get_double_column(const std::string& name) const = 0;
    virtual const std::vector<int64_t>& get_int_column(const std::string& name) const = 0;
    virtual const std::vector<std::string>& get_string_column(const std::string& name) const  = 0;
};

typedef std::shared_ptr<FactorDataFrame> DataFramePtr;

enum DEMO_DLL_API EtaBarTimespan
{
    EBAR_TICK = 0,
    EBAR_MIN_1 = 60, 
    EBAR_MIN_5 = 300, 
    EBAR_MIN_15 = 900, 
    EBAR_MIN_30 = 1800, 
    EBAR_MIN_60 = 3600, 
    EBAR_DAY_1 = 86400, 
};


enum DEMO_DLL_API EtaPriceList
{
    EPL_NONE = 0,
    EPL_BID1 = 1,
    EPL_BID2 = 2,
    EPL_BID3 = 3,
    EPL_BID4 = 4,
    EPL_BID5 = 5,
    EPL_BID6 = 6,
    EPL_BID7 = 7,
    EPL_BID8 = 8,
    EPL_BID9 = 9,
    EPL_BID10 = 10,

    EPL_ASK1 = 101,
    EPL_ASK2 = 102,
    EPL_ASK3 = 103,
    EPL_ASK4 = 104,
    EPL_ASK5 = 105,
    EPL_ASK6 = 106,
    EPL_ASK7 = 107,
    EPL_ASK8 = 108,
    EPL_ASK9 = 109,
    EPL_ASK10 = 110,
};


struct DEMO_DLL_API EtaStrategyInfo
{
    char id[UUID_SIZE]{};
    char name[SYMBOL_SIZE]{};
};


struct DEMO_DLL_API EtaQtyPrice
{
    double quantity = 0;
    double price = 0;
};


struct DEMO_DLL_API EtaQuote
{
    char symbol[SYMBOL_SIZE]{};                
    double bid = 0;    
    double ask = 0;    
    double bidVol = 0;    
    double askVol = 0;    
    double last = 0;    
    double lastVolume = 0;    
    double lastTurnover = 0;    
    double totalVolume = 0;    
    double totalTurnover = 0;    
    double high = 0;    
    double low = 0;    
    double open = 0;    
    double close = 0;    
    int tradeDate = 0;    
    int64_t timeExch = 0;    
    double ceil = 0;    
    double floor = 0;    
    double position = 0;    
    EtaQtyPrice bids[10];    
    EtaQtyPrice asks[10];   
    double preClose = 0;    
    double preSettle = 0;       
    char timeStr[DATEFORMAT_SIZE]{};
};


struct DEMO_DLL_API EtaBar
{
    char symbol[SYMBOL_SIZE]{};
    int tradeDate = 0;      
    int64_t timeStop = 0;
    int64_t timeSpan = 0;   
    double high = 0;
    double low = 0;
    double open = 0;
    double close = 0;
    double volume = 0;
    double turnover = 0;
    double totalVolume = 0;
    double totalTurnover = 0;
    double preClose = 0;
    double position = 0;
    double settle = 0;
    double preSettle = 0;
    bool isSuspended = false;        
    char timeStr[DATEFORMAT_SIZE]{};        
};


inline bool operator<(const EtaBar &l, const EtaBar &r)
{
    if (l.timeStop != r.timeStop) return (l.timeStop < r.timeStop);
    return false;
}

inline bool operator<=(const EtaBar &l, const EtaBar &r)
{
    return !(r < l);
}

inline bool operator>(const EtaBar &l, const EtaBar &r)
{
    return r < l;
}

inline bool operator>=(const EtaBar &l, const EtaBar &r)
{
    return !(l < r);
}


struct DEMO_DLL_API EtaAccount
{
    char id[UUID_SIZE]{};                            
    char market[MARKET_SIZE]{};                        
    char currency[CURRENCY_SIZE]{};                        
    double dailyPnL = 0;                        
    double urLastPnL = 0;                        
    double allTimePnL = 0;                        
    double cashDeposited = 0;                    
    double cashAvailable = 0;                    
    double unitValue = 0;                        
    double margin = 0;                            
    double bonus = 0;                            
    double marketValue = 0;                        
    double totAssets = 0;                        
    double totalCommission = 0;                    
};


struct DEMO_DLL_API EtaRefData
{
    char symbol[SYMBOL_SIZE]{};                     
    char exchange[EXCHANGE_SIZE]{};                   
    char market[MARKET_SIZE]{};                 
    char currency[CURRENCY_SIZE]{};                   
    double lotSize = 0;                         
    char name[SYMBOL_SIZE]{};                       
    int32_t tPlus{};
    int32_t valuePerUnit{};                   
    double marginRate = 0;                      
    bool shortSellable{};                     
    double priceTick = 0;                       
    char originSymbol[SYMBOL_SIZE]{};                 
    bool isStandard = false;                        
    char tradeMarket[MARKET_SIZE]{};                
    int listDate = 0;                
    int lastTradeDate = 0;                
    char productName[PRODUCT_SIZE]{};                
    double minOrderVolume = 0;              
};


struct DEMO_DLL_API EtaOverallPosition
{
    char accountId[UUID_SIZE]{};        
    char symbol[SYMBOL_SIZE]{};            
    double buyPrice = 0;    
    double sellPrice = 0;    
    double buyQty = 0;    
    double sellQty = 0;    
    double lastUrPnL = 0;    
    double longAvailableQty = 0;   
    double shortAvailableQty = 0;  
    double longQty = 0;         
    double shortQty = 0;        
    double longPrice = 0;       
    double shortPrice = 0;      
    double longLastUrPnL = 0;        
    double shortLastUrPnL = 0;        
    double longMargin = 0;            
    double shortMargin = 0;            
    double longMarketValue = 0;        
    double shortMarketValue = 0;    
};


struct DEMO_DLL_API EtaStrategyPnL
{
    char id[UUID_SIZE]{};    
    double overallPnL = 0;    
    double urPnL = 0;        
    double dailyPnL = 0;    
    double totalCommission = 0; 
};


enum DEMO_DLL_API EtaPositionSide
{
    EPS_UNKNOWN = 0, 
    EPS_LONG = 1,   
    EPS_SHORT = 2,  
};


struct DEMO_DLL_API EtaSymbolPosition
{
    EtaPositionSide positionSide = EPS_UNKNOWN;
    char symbol[SYMBOL_SIZE]{}; 
    double posQty = 0; 
    double posDailyQty = 0; 
    double posPrice = 0; 
    double posHigh = 0; 
    double posLow = 0; 
    int64_t posTime = 0; 
    double posUrPnL = 0; 
    double availableQty = 0;        
    double dailyUrPnL = 0;   
    int posTradeDate = 0;   
    double posMargin = 0;       
    double posMarketValue = 0;  
    double posDailyOverallPnL = 0;  
};


struct DEMO_DLL_API EtaSymbolPositionList
{
    EtaSymbolPosition longPosition;
    EtaSymbolPosition shortPosition;
};


struct DEMO_DLL_API EtaInstrument
{
    char symbol[SYMBOL_SIZE]{};
    char accountId[UUID_SIZE]{};
    char tradeAccount[UUID_SIZE]{};
};


struct DEMO_DLL_API EtaBackTestParam
{
    int startDate = 0;               
    int endDate = 0;                    
    int64_t startTimestamp = 0;    
    int64_t endTimestamp = 0;    
};




struct DEMO_DLL_API EtaMarketParam
{
    int tradeDate;
};


struct DEMO_DLL_API EtaSymbolField
{
    int tradeDate;
    int64_t updateTime;
    double value;
};


enum DEMO_DLL_API EtaFieldValueType
{
    FVT_UNKNOWN = 0,
    FVT_INTEGER = 1, 
    FVT_FLOAT = 2, 
    FVT_STRING = 3, 
};


enum DEMO_DLL_API EtaPriceMode
{
    DEMO_PM_REAL = 0, 
    DEMO_PM_FORMER = 1, 
};


struct DEMO_DLL_API EtaBackTestResult
{
    char backTestId[UUID_SIZE]{};                      
    double annualizedReturn = 0;    
    double maximumDrawdown = 0;    
    double sharpeRatio = 0;    
    double standardAnnualizedReturn = 0;    
    int dayCount = 0;    
    double volatility = 0;    
    double strategyReturn = 0;    
    double urAsset = 0;    
    int64_t quoteCount = 0;    
    int64_t costTime = 0;    
    double winRatio = 0;    
};

struct DEMO_DLL_API EtaStrategyExitInfo
{
    int32_t code = 0;
    char msg[ERROR_SIZE]{};
    EtaBackTestResult result; 
};

enum DEMO_DLL_API EtaOrderSide
{
    OS_UNKNOWN = 0,
    OS_BUY = 1,
    OS_SELL = 2,
};

enum DEMO_DLL_API EtaPositionEffect
{
    PE_NONE = 0,
    PE_OPEN = 1,
    PE_CLOSE = 2,
    PE_CLOSE_TODAY = 3,
    PE_CLOSE_YESTERDAY = 4,
};

enum DEMO_DLL_API EtaOrderType
{
    OT_NONE = 0,
    OT_MARKET = 1,
    OT_LIMIT = 2,
};

enum DEMO_DLL_API EtaOrdStatus
{
    OST_NONE = 0,
    OST_NEW = 1,
    OST_PARTIALLY_FILLED = 2,
    OST_FILLED = 3,
    OST_DONE_FOR_DAY = 4,
    OST_CANCELED = 5,
    OST_REPLACED = 6,
    OST_PENDING_CANCEL = 7,
    OST_STOPPED = 8,
    OST_REJECTED = 9,
    OST_SUSPENDED = 10,
    OST_PENDING_NEW = 11,
    OST_CALCULATED = 12,
    OST_EXPIRED = 13,
    OST_ACCEPTED_FOR_BIDDING = 14,
    OST_PENDING_REPLACE = 15,
};

enum DEMO_DLL_API EtaTimeInForce
{
    TIF_NONE = 0,  
    TIF_DAY = 1,                  
    TIF_GOOD_TILL_CANCEL = 2,     
    TIF_AT_THE_OPENING = 3,       
    TIF_IMMEDIATE_OR_CANCEL = 4,  
    TIF_FILL_OR_KILL = 5,         
    TIF_GOOD_TILL_CROSSING = 6,   
    TIF_GOOD_TILL_DATE = 7,       
    TIF_AT_THE_CLOSE = 8,         
};

struct DEMO_DLL_API EtaOrderStatusUpdate
{
    char symbol[SYMBOL_SIZE]{};
    EtaOrderSide side;
    EtaPositionSide positionSide;
    EtaPositionEffect positionEffect;
    double qty{};
    double price{};
    char userId[UUID_SIZE]{};
    char accountId[UUID_SIZE]{};
    int64_t created{};
    char id[UUID_SIZE]{};
    int32_t tradeDate{};
    char strategyId[UUID_SIZE]{};
    char portfolioId[UUID_SIZE]{};
    double commission{};
    char tradeAccount[UUID_SIZE]{};
    char externalOrderId[UUID_SIZE]{};
    EtaOrderType orderType;
    EtaOrdStatus ordStatus;
    double cumQty{};
    double avgPx{};
    EtaTimeInForce tif;
    int64_t modified{};
    bool hasExecution{};
};

enum DEMO_DLL_API EtaExecType
{
    EXEC_NONE = 0,
    EXEC_NEW = 1,
    EXEC_PARTIALLY_FILLED = 2,
    EXEC_FILLED = 3,
    EXEC_DONE_FOR_DAY = 4,
    EXEC_CANCELED = 5,
    EXEC_REPLACE = 6,
    EXEC_PENDING_CANCEL = 7,
    EXEC_STOPPED = 8,
    EXEC_REJECTED = 9,
    EXEC_SUSPENDED = 10,
    EXEC_PENDING_NEW = 11,
    EXEC_CALCULATED = 12,
    EXEC_EXPIRED = 13,
    EXEC_RESTATED = 14,
    EXEC_PENDING_REPLACE = 15,
};

struct DEMO_DLL_API EtaOrderUpdate
{
    char symbol[SYMBOL_SIZE]{};
    EtaOrderSide side;
    EtaPositionSide positionSide;
    EtaPositionEffect positionEffect;
    double qty{};
    double price{};
    char userId[UUID_SIZE]{};
    char accountId[UUID_SIZE]{};
    int64_t created{};
    char id[UUID_SIZE]{};
    int32_t tradeDate{};
    char strategyId[UUID_SIZE]{};
    char portfolioId[UUID_SIZE]{};
    double commission{};
    char tradeAccount[UUID_SIZE]{};
    char externalOrderId[UUID_SIZE]{};
    EtaOrderType orderType;
    EtaOrdStatus ordStatus;
    double cumQty{};
    double avgPx{};
    EtaTimeInForce tif;
    int64_t modified{};
    EtaExecType execType;
    char subStrategyName[STRATEGY_NAME_SIZE]{};
};

struct DEMO_DLL_API EtaOrderExecReport
{
    char symbol[SYMBOL_SIZE]{};
    EtaOrderSide side;
    EtaPositionSide positionSide;
    EtaPositionEffect positionEffect;
    char userId[UUID_SIZE]{};
    char accountId[UUID_SIZE]{};
    char id[UUID_SIZE]{};
    int32_t tradeDate{};
    char strategyId[UUID_SIZE]{};
    char portfolioId[UUID_SIZE]{};
    double commission{};
    char tradeAccount[UUID_SIZE]{};
    char externalOrderId[UUID_SIZE]{};
    EtaOrderType orderType;
    EtaOrdStatus ordStatus;
    double cumQty{};
    double avgPx{};
    double qty{};
    double price{};
    EtaTimeInForce tif;
    int64_t modified{};
};



class DEMO_DLL_API StrategyAPI
{
    
public:
    explicit StrategyAPI(const std::string &analyzerName);

    
    void set_required_data(const std::vector<std::string> &instset, const std::vector<std::string> &symbols,
                           const std::vector<std::string> &fields, const std::vector<std::pair<EtaBarTimespan, int>> &barSets);

    
    static void set_pre_market_time(const std::string &time);

    static void set_closing_time(const std::string &time);

    void set_custom_back_test_param(int beginDate, int endDate);

    void set_match_params(int64_t timeSpan, const std::vector<std::pair<std::string, std::string>> &needTimeRanges);

    void set_back_test_slippage(int slippage);

    void set_back_test_cash(const std::string &market, double value);

    void set_commission(double commission);

    
    EtaRefData get_ref_data(const std::string &symbol);

    std::vector<int> get_prev_trade_dates(int tradeDate, const std::string &market, int count);

    std::vector<int> get_next_trade_dates(int tradeDate, const std::string &market, int count);

    void get_constituent_symbols(const std::vector<std::string> &indexs, int tradeDate, std::map<std::string, std::vector<std::string> > &res);

    std::string get_continuous_symbol(const std::string &maincfs, int tradeDate);

    std::vector<std::string> get_constituent_symbols_by_set(const std::string &instset, int tradeDate);

    std::vector<std::string> get_appointed_symbols(const std::string &eType, int tradeDate);

    void get_trade_dates(int startTradeDate, int endTradeDate, const std::string &market, std::vector<int> &tradingDays);

    int get_trade_day_interval(const std::string &symbol, int startDate, int endDate);

    bool is_suspend(const std::string &symbol, int date);

    bool is_ST(const std::string &symbol, int date);

    bool is_listed(const std::string &symbol, int date);

    int get_last_trade_date(const std::string &symbol, int date);

    int get_next_trade_date(const std::string &symbol, int date);

    void set_require_bars(EtaBarTimespan timeSpan, int count);

    void set_require_fields(const std::vector<std::string> &fields);

    std::string get_version();

    void exit();

    std::string target_position(const std::string &symbol, double qty, double price = 0.0, EtaPositionSide side = EPS_LONG, EtaTimeInForce tif = TIF_NONE,
                                const std::string &remark = "", EtaPriceList priceList = EPL_NONE);

    
    void target_position(const std::string &mode, const std::string &symbol, double qty, EtaPositionSide side = EPS_LONG);

    void set_algo_param(bool on, int timeSpan, int avgCount, const std::string &endTime = "145500");

    
    std::string target_percent(const std::string &symbol, double percent, double price = 0.0, EtaPositionSide side = EPS_LONG, EtaTimeInForce tif = TIF_NONE,
                               const std::string &remark = "");

    
    void cancel_order(const std::string &symbol, EtaPositionSide side = EPS_LONG, const std::string &remark = "");

    
    double get_value_as_double(const std::string &dynamic_param);

    std::string get_value_as_string(const std::string &dynamic_param);

    int64_t get_value_as_long(const std::string &dynamic_param);

    const EtaInstrument get_instrument_by_symbol(const std::string &symbol);

    const std::vector<EtaInstrument> &get_instruments();

    std::vector<std::string> get_position_symbols() const;

    std::vector<std::string> get_focus_and_position_symbols() const;

    const std::string &get_name() const
    {
        return _analyzerName;
    }

    
    EtaStrategyInfo get_strategy_info();

    
    const EtaAccount &get_account(const std::string &symbol, const std::string &market = "");

    
    const EtaSymbolPosition &get_symbol_position(const std::string &symbol, EtaPositionSide side = EPS_LONG);

    
    std::vector<EtaSymbolPosition> get_symbol_positions();

    
    const EtaOverallPosition &get_overall_position(const std::string &symbol);

    
    const EtaStrategyPnL &get_strategyPnL();

    
    int64_t get_now_ms();

    
    int64_t get_seconds_to_next_close_time(const std::string &symbol);

    
    bool is_trading_now(const std::string &symbol);

    
    int64_t get_seconds_to_next_start_time(const std::string &symbol);

    
    int64_t time_now();

    
    int get_curr_trade_date();

    
    void send_custom_message(const std::string &msg);

    
    void set_symbol_pool(const std::vector<std::string> &instset, const std::vector<std::string> &symbols);

    
    std::vector<std::string> get_symbol_pool();

    
    void set_focus_symbols(const std::set<std::string> &symbols);

    
    bool is_focus_symbol(const std::string &symbol);

    
    bool is_back_test();

    
    bool is_real_env();

    
    void sync_data(const std::string &table, int start_date, int end_date);

    
    EtaBackTestParam get_back_test_param();

    
    std::vector<EtaBar> get_bars_history(const std::string &symbol, EtaBarTimespan timeSpan, int count, EtaPriceMode priceMode = DEMO_PM_REAL, int skipSuspended = 1);

    
    std::map<std::string, std::vector<EtaBar>> get_bars_history(const std::vector<std::string> &symbols, EtaBarTimespan timeSpan, int count, EtaPriceMode priceMode = DEMO_PM_REAL, int skipSuspended = 1);

    
    void set_timer_cycle(uint32_t cycleMs);

    
    void set_group_mode(uint32_t timeOutMs = 10000, bool onlyGroup = true);

    
    std::string get_conf_path();


    DataFramePtr get_fields_one_day(const std::vector<std::string> &symbols, const std::vector<std::string> &fields, int tradedate);

    DataFramePtr get_fields_range_days(const std::string &symbol, const std::vector<std::string> &fields, int startTradedate, int endTradedate);

    DataFramePtr get_fields_count_days(const std::string &symbol, const std::vector<std::string> &fields, int tradeDate, int count);


    DataFramePtr get_finance_fields(const std::vector<std::string> &symbols, const std::vector<std::string> &fields, int tradeDate,
                                    const std::string &report);

    DataFramePtr get_table_field_one_day(const std::string &symbol, const std::string &field, int tradeDate, const std::vector<std::string> &columns);

    DataFramePtr get_table_field_count_days(const std::string &symbol, const std::string &field, int tradeDate, int count, const std::vector<std::string> &columns);

    DataFramePtr get_table_field_range_days(const std::string &symbol, const std::string &field, int startDate, int endDate, const std::vector<std::string> &columns);

    static std::string error_code_to_string(int errorCode);

private:
    std::string _analyzerName;
};

class DEMO_DLL_API StrategyAnalyzer
{
public:
    virtual ~StrategyAnalyzer()
    {};

    virtual void on_initialize(StrategyAPI *sApi)
    {
        std::cout << "multi do nothing" << std::endl;
    }

    virtual void on_before_market_open(StrategyAPI *sApi, const EtaMarketParam &marketParam)
    {
        std::cout << " onBeforeMarketBegin do nothing" << std::endl;
    }

    virtual void on_tick(StrategyAPI *sApi, const EtaQuote &quote)
    {
        std::cout << "multi on tick do nothing" << std::endl;
    }

    virtual void on_bar(StrategyAPI *sApi, const EtaBar &bar)
    {
        std::cout << "multi on bar do nothing" << std::endl;
    }

    virtual void on_handle_data(StrategyAPI *sApi, int64_t timeExch)
    {
        std::cout << "on handle bar do nothing" << std::endl;
    }

    virtual void on_timer(StrategyAPI *sApi)
    {
        std::cout << "multi on timer do nothing" << std::endl;
    }

    virtual void on_terminate(StrategyAPI *sApi, const EtaStrategyExitInfo &exitInfo)
    {
        std::cout << "multi on onTerminate do nothing" << std::endl;
    }

    virtual void on_order_update(StrategyAPI *sApi, const EtaOrderUpdate &orderUpdate)
    {

    }

    
    virtual void on_first_tick(StrategyAPI *sApi, const EtaQuote &quote)
    {

    }
};

class DEMO_DLL_API StrategyApplication
{
public:
    static void set_log_level(const std::string &level);

public:
    virtual void on_login_succ()
    {
    }

    virtual void on_back_test_result(const EtaBackTestResult &result)
    {
    }

public:
    StrategyApplication();

    virtual ~StrategyApplication() = default;

    int init(int argc, char *argv[]);

    int init(const std::string &configFile);

    void reg_analyzer(const std::string &analyzerName, StrategyAnalyzer *analyzer);

    void wait_for_shutdown();

    void release();
};

}

#if defined _WIN32 || defined _WIN64
#else
#pragma GCC visibility pop
#endif