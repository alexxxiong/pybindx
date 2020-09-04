#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <sstream>
#include <algorithm>
#include <memory>

#if defined _WIN32 || defined _WIN64
#ifdef ETA_DLL_EXPORTS
#ifndef ETA_DLL_API
#define ETA_DLL_API __declspec(dllexport)
#endif
#else
#ifdef ETA_USE_DLL
#define ETA_DLL_API __declspec(dllimport)
#else
#define ETA_DLL_API
#endif
#endif

#else

#ifndef ETA_DLL_API
#define ETA_DLL_API
#endif

#pragma GCC visibility push(default)
#endif


namespace Demo
{

class ETA_DLL_API FactorDataFrame
{
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

typedef std::shared_ptr <FactorDataFrame> DataFramePtr;

#define SYMBOL_SIZE 24
#define EXCHAGE_SIZE 8
#define MARKET_SIZE 8
#define PRODUCT_SIZE 24
#define CURRENCY_SIZE 8
#define TIMEFORMAT_SIZE 24
#define TIME_SIZE 8
#define DATASTR_SIZE 8
#define UUID_SIZE 32
#define ERROR_SIZE 64
#define STRATEGYNAME_SIZE 128

//支持的实时Bar类型
enum ETA_DLL_API EtaBarTimespan
{
    EBAR_TICK = 0,
    EBAR_MIN_1 = 60, //1分钟K线
    EBAR_MIN_5 = 300, //5分钟K线
    EBAR_MIN_15 = 900, //15分钟K线
    EBAR_MIN_30 = 1800, //30分钟K线
    EBAR_MIN_60 = 3600, //60分钟K线
    EBAR_DAY_1 = 86400, //日K线
};

//报价列表
enum ETA_DLL_API EtaPriceList
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

enum ETA_DLL_API EtaExecType
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

//方向
enum ETA_DLL_API EtaPositionSide
{
    EPS_UNKNOWN = 0, //未知
    EPS_LONG = 1,   //long
    EPS_SHORT = 2,  //short
};

//策略信息
struct ETA_DLL_API EtaStrategyInfo
{
    char id[UUID_SIZE]{};
    char name[SYMBOL_SIZE]{};
};

//档位价钱
struct ETA_DLL_API EtaQtyPrice
{
    double quantity = 0;
    double price = 0;
};

enum ETA_DLL_API EtaOrderSide
{
    OS_UNKNOWN = 0,
    OS_BUY = 1,
    OS_SELL = 2,
};

enum ETA_DLL_API EtaPositionEffect
{
    PE_NONE = 0,
    PE_OPEN = 1,
    PE_CLOSE = 2,
    PE_CLOSE_TODAY = 3,
    PE_CLOSE_YESTERDAY = 4,
};

enum ETA_DLL_API EtaOrderType
{
    OT_NONE = 0,
    OT_MARKET = 1,
    OT_LIMIT = 2,
};

enum ETA_DLL_API EtaOrdStatus
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

enum ETA_DLL_API EtaTimeInForce
{
    TIF_NONE = 0,  //未指定，如果不指定;则限价单默认为TIF_DAY,市价单默认为 TIF_IMMEDIATE_OR_CANCEL
    TIF_DAY = 1,                  //订单在交易日内有效，会自动取消
    TIF_GOOD_TILL_CANCEL = 2,     //##暂时不支持—订单一直有效，直到投资者主动撤销订单（一般也有默认有效期30~90天）
    TIF_AT_THE_OPENING = 3,       //##暂时不支持-订单在开市的时候被执行，如果开盘内不能执行，则取消；通常是开盘前集合竞价或者开盘前的瞬间
    TIF_IMMEDIATE_OR_CANCEL = 4,  //订单必须立即执行，不能被立即执行的部分将被取消
    TIF_FILL_OR_KILL = 5,         //订单必须被完全执行或者完全不执行
    TIF_GOOD_TILL_CROSSING = 6,   //##暂时不支持-
    TIF_GOOD_TILL_DATE = 7,       //##暂时不支持-订单一直有效，直到指定日期
    TIF_AT_THE_CLOSE = 8,         //##暂时不支持-订单在交易最后执行（或者尽可能接近收盘价），通常是收拾撮合的时间段
};

//表数据
enum ETA_DLL_API EtaFieldValueType
{
    FVT_UNKNOWN = 0,
    FVT_INTEGER = 1, // 整型 int(11)
    FVT_FLOAT = 2, // 浮点型 decimal(20, 6)
    FVT_STRING = 3, // 字符串型, 对于数据库表为 varchar(30)
};

//复权模式
enum ETA_DLL_API EtaPriceMode
{
    ETA_PM_REAL = 0, //不复权
    ETA_PM_FORMER = 1, //前复权
};

//行情结构体
struct ETA_DLL_API EtaQuote
{
    char symbol[SYMBOL_SIZE]{};                //证券代码
    double bid = 0;    //买价1
    double ask = 0;    //卖价1
    double bidVol = 0;    //买量1-(实际证券数量，不是手数)
    double askVol = 0;    //卖量1
    double last = 0;    //最新成交价
    double lastVolume = 0;    //最新成交量
    double lastTurnover = 0;    //最新成交额
    double totalVolume = 0;    //交易日总成交量
    double totalTurnover = 0;    //交易日总成交额
    double high = 0;    //交易日最高价
    double low = 0;    //交易日最低价
    double open = 0;    //交易日开盘价
    double close = 0;    //交易日收盘价
    int tradeDate = 0;    //交易日-年月日(不要时分秒)(20170101)
    int64_t timeExch = 0;    //交易所的实时时间-年月日时分秒 (ms)
    double ceil = 0;    //涨停价(绝对值)
    double floor = 0;    //跌停价(绝对值)
    double position = 0;    //持仓量(黄金、期货)
    EtaQtyPrice bids[10];    //五档买价
    EtaQtyPrice asks[10];   //五档卖价
    double preClose = 0;    //昨日收盘价
    double preSettle = 0;       //昨日结算价
    char timeStr[TIMEFORMAT_SIZE]{};//标准时间
};

//K线
struct ETA_DLL_API EtaBar
{
    char symbol[SYMBOL_SIZE]{};
    int tradeDate = 0;      //交易日
    int64_t timeStop = 0;
    int64_t timeSpan = 0;   //K线区间的秒数，目前支持 60（1min），300（5min），1800(30min),3600(60min),86400(day)
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
    bool isSuspended = false;        //是否是填充的(默认不是填充)
    char timeStr[TIMEFORMAT_SIZE]{};        //标准时间
};

//DataList里面排序需要
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

//账号
struct ETA_DLL_API EtaAccount
{
    char id[UUID_SIZE]{};                            //账户 id(userId-market)
    char market[MARKET_SIZE]{};                        //市场
    char currency[CURRENCY_SIZE]{};                        //账户币种(CNY)
    double dailyPnL = 0;                        //当天盈亏 - 清算时候清零
    double urLastPnL = 0;                        //浮动盈亏 - 按最后价算
    double allTimePnL = 0;                        //盈亏
    double cashDeposited = 0;                    //存入现金(所有加入的现金)
    double cashAvailable = 0;                    //可用现金
    double unitValue = 0;                        //账户基金价值
    double margin = 0;                            //现金分红
    double bonus = 0;                            //持仓占用资金
    double marketValue = 0;                        //持仓市值
    double totAssets = 0;                        //总资产
    double totalCommission = 0;                    //总手续费
};

//参考数据
struct ETA_DLL_API EtaRefData
{
    char symbol[SYMBOL_SIZE]{};                     //标的
    char exchange[EXCHAGE_SIZE]{};                   //交易所
    char market[MARKET_SIZE]{};                 //市场名称
    char currency[CURRENCY_SIZE]{};                   //币种
    double lotSize = 0;                         //一手数目
    char name[SYMBOL_SIZE]{};                       //证券默认名字
    int32_t tplus{};                          //成交T+
    int32_t valuePerUnit{};                   //每股单位价值
    double marginRate = 0;                      //担保金%
    bool shortSellable{};                     //是否可以卖空
    double priceTick = 0;                       //标的价格浮动最小单位值
    char originSymbol[SYMBOL_SIZE]{};                 //交易所原始symbol名称
    bool isStandard = false;                        //是否是交易所标准合约, 比如主力合约就不是(量化平台自己算的)
    char tradeMarket[MARKET_SIZE]{};                //该标的对应的交易市场
    int listDate = 0;                //该标的对应的交易市场
    int lastTradeDate = 0;                //该标的对应的交易市场
    char productName[PRODUCT_SIZE]{};                //期货品种名
    double minOrderVolume = 0;              //最小下单数量
};

//仓位
struct ETA_DLL_API EtaOverallPosition
{
    char accountId[UUID_SIZE]{};        //账户ID
    char symbol[SYMBOL_SIZE]{};            //合约
    double buyPrice = 0;    //加权平均买价(开仓/平仓所有数据加权平均)
    double sellPrice = 0;    //加权平均卖价
    double buyQty = 0;    //买数量
    double sellQty = 0;    //卖数量
    double lastUrPnL = 0;    //最后成交价算的盈亏(以OpenPosition计算)
    double longAvailableQty = 0;   //多仓可以平仓数量(计算出来)
    double shortAvailableQty = 0;  //空仓可以平仓数量(计算出来)
    double longQty = 0;         //多仓数量
    double shortQty = 0;        //空仓数量
    double longPrice = 0;       //多仓开仓均价
    double shortPrice = 0;      //空仓开仓均价
    double longLastUrPnL = 0;        //多仓浮动盈亏
    double shortLastUrPnL = 0;        //空仓浮动盈亏
    double longMargin = 0;            //多仓占用保证金
    double shortMargin = 0;            //空仓占用保证金
    double longMarketValue = 0;        //多仓持仓市值
    double shortMarketValue = 0;    //空仓持仓市值
};

//策略盈亏
struct ETA_DLL_API EtaStrategyPnL
{
    char id[UUID_SIZE]{};    //策略id
    double overallPnL = 0;    //策略盈亏
    double urPnL = 0;        //策略的浮动盈亏
    double dailyPnL = 0;    //策略当日盈亏
    double totalCommission = 0; //总手续费
};


//策略中标的的仓位
struct ETA_DLL_API EtaSymbolPosition
{
    EtaPositionSide positionSide = EPS_UNKNOWN;
    char symbol[SYMBOL_SIZE]{}; //标的
    double posQty = 0; //仓位股数 (double)
    double posDailyQty = 0; //今日仓位股数 (double)
    double posPrice = 0; //仓位进仓价 (double)
    double posHigh = 0; // 持仓最高价 (double)
    double posLow = 0; // 持仓最低价 (double)
    int64_t posTime = 0; // 持仓时间 (double)
    double posUrPnL = 0; // 仓位浮动盈亏 (double)
    double availableQty = 0;        // 可平仓数量(计算出来,目前因为多账户问题意义不明确)
    double dailyUrPnL = 0;   // 今日浮动盈亏,现价-昨收价(结算价-期货),每日结算清零
    int posTradeDate = 0;   //持仓交易日(20170101)
    double posMargin = 0;       // 仓位保证金
    double posMarketValue = 0;  // 仓位市值
    double posDailyOverallPnL = 0;  // 当日盈亏
};

//标的策略仓位,包含两个方向
struct ETA_DLL_API EtaSymbolPositionList
{
    EtaSymbolPosition longPosition;
    EtaSymbolPosition shortPosition;
};

//symbol信息
struct ETA_DLL_API EtaInstrument
{
    char symbol[SYMBOL_SIZE]{};
    char accountId[UUID_SIZE]{};
    char tradeAccount[UUID_SIZE]{};
};

//回测信息
struct ETA_DLL_API EtaBackTestParam
{
    int startDate = 0;               //交易日 开始日期-20170629
    int endDate = 0;                    //交易日 结束日期-20170729
    int64_t startTimestamp = 0;    //绝对时间 开始日期-20170628-20:00:00 转换成ms
    int64_t endTimestamp = 0;    //绝对时间 结束日期-20170629-15:30:00 转换成ms
};


//开盘数据
struct ETA_DLL_API EtaMarketParam
{
    int tradeDate;//当前交易日
};

//字段数据
struct ETA_DLL_API EtaSymbolField
{
    int tradeDate;
    int64_t updateTime;
    double value;
};

//回测最终结果
struct ETA_DLL_API EtaBackTestResult
{
    char backTestId[UUID_SIZE]{};                      //回测ID
    double annualizedReturn = 0;    //年化收益率
    double maximumDrawdown = 0;    //最大回撤
    double sharpeRatio = 0;    //夏普比率
    double standardAnnualizedReturn = 0;    //基准年化收益率
    int dayCount = 0;    //回测天数
    double volatility = 0;    //策略波动率
    double strategyReturn = 0;    //策略收益
    double urAsset = 0;    //策略资产
    int64_t quoteCount = 0;    //回测行情数
    int64_t costTime = 0;    //回测耗时
    double winRatio = 0;    //胜率
};

struct ETA_DLL_API EtaStrategyExitInfo
{
    int32_t code = 0;
    char msg[ERROR_SIZE]{};
    EtaBackTestResult result; //回测结果
};



struct ETA_DLL_API EtaOrderStatusUpdate
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


struct ETA_DLL_API EtaOrderUpdate
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
    char subStrategyName[STRATEGYNAME_SIZE]{};
};

struct ETA_DLL_API EtaOrderExecReport
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
}

#if defined _WIN32 || defined _WIN64
#else
#pragma GCC visibility pop
#endif