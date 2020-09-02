//头文件-utf8
#pragma once
//用于消除STL类导出DLL的warning

#include "DemoStruct.h"

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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//对外接口

//多标的分析器
class StrategyApi;
class ETA_DLL_API StrategyAnalyzer
{
public:
    virtual ~StrategyAnalyzer() = default;

    virtual void on_initialize(std::shared_ptr<StrategyApi> sApi)
    {
        std::cout << "multi do nothing" << std::endl;
    }

    //开盘前
    virtual void on_before_market_open(std::shared_ptr<StrategyApi> sApi, const EtaMarketParam &marketParam)
    {
        std::cout << " onBeforeMarketBegin do nothing" << std::endl;
    }

    //行情回调
    virtual void on_tick(std::shared_ptr<StrategyApi> sApi, const EtaQuote &quote)
    {
        std::cout << "multi on tick do nothing" << std::endl;
    }

    //K线回调
    virtual void on_bar(std::shared_ptr<StrategyApi> sApi, const EtaBar &bar)
    {
        std::cout << "multi on bar do nothing" << std::endl;
    }

    //一组K线回调
    virtual void on_handle_data(std::shared_ptr<StrategyApi> sApi, int64_t timeExch)
    {
        std::cout << "on handle bar do nothing" << std::endl;
    }

    //Timer回调
    virtual void on_timer(std::shared_ptr<StrategyApi> sApi)
    {
        std::cout << "multi on timer do nothing" << std::endl;
    }

    //Timer回调
    virtual void on_terminate(std::shared_ptr<StrategyApi> sApi, const EtaStrategyExitInfo &exitInfo)
    {
        std::cout << "multi on onTerminate do nothing" << std::endl;
    }

    virtual void on_order_update(std::shared_ptr<StrategyApi> sApi, const EtaOrderUpdate &orderUpdate)
    {

    }

    //模拟盘和实盘中的第一条行情
    virtual void on_first_tick(std::shared_ptr<StrategyApi> sApi, const EtaQuote &quote)
    {

    }
};

class ETA_DLL_API StrategyApi
{
    //读数据相关接口
public:
    /**
     * @brief 创建策略api
     * @param name
     * @return
     */
    static std::shared_ptr<StrategyApi> create_strategy_api(const std::string &name);


    /**
     * @brief 错误码转换为字符串
     * @param error_code
     * @return
     */
    static std::string error_code_to_string(int error_code);

    /**
     * @brief 设置日志级别
     * @param level
     */
    static void set_log_level(const std::string &level);

    /**
     * @brief 命令行直接创建
     * @param argc
     * @param argv --config=config_file
     * @return
     */
    virtual int init(int argc, char *argv[]) = 0;

    /**
     * @brief 从config.json文件创建
     * @param config_path
     * @return
     */
    virtual int init(const std::string &config_path) = 0;

    /**
     * @brief 注册回调
     * @param analyzerName
     * @param analyzer
     */
    virtual void reg_analyzer(StrategyAnalyzer *analyzer) = 0;

    /**
     * @brief 进入事件循环
     */
    virtual void wait_for_shutdown() = 0;


    /**
     * @brief 获取策略名称
     * @return
     */
    virtual const std::string &get_name() const = 0;

    /**
     * @brief 获取配置文件路径
     * @return
     */
    virtual std::string get_conf_path() = 0;

    /**
     * @brief 版本信息
     * @return
     */
    virtual std::string get_version() = 0;

    /**
     * @brief 退出时必须调用，用来释放资源
     */
    virtual void exit() = 0;

    /**
     * @brief 获取策略动态参数（double类型）
     * @param dynamic_param
     * @return
     */
    virtual double get_value_as_double(const std::string &dynamic_param) = 0;

    /**
     * @brief 获取策略动态参数（string类型）
     * @param dynamic_param
     * @return
     */
    virtual std::string get_value_as_string(const std::string &dynamic_param) = 0;

    /**
     * @brief 获取策略动态参数（int64_t类型）
     * @param dynamic_param
     * @return
     */
    virtual int64_t get_value_as_long(const std::string &dynamic_param) = 0;

    /**
     * @brief 推送自定义消息到web页, 可以在web页面上显示
     * @param msg
     */
    virtual void send_custom_message(const std::string &msg) = 0;

    /*
     * 初始化阶段函数
     */

    /**
     * @brief 初始化下载历史数据
     * @param table
     * @param start_date
     * @param end_date
     */
    virtual void sync_data(const std::string &table, int start_date, int end_date) = 0;

    /**
     * @brief 设置初始化参数
     * @param instset
     * @param symbols
     * @param fields
     * @param barSets
     */
    virtual void set_required_data(const std::vector<std::string> &instset, const std::vector<std::string> &symbols,
                                   const std::vector<std::string> &fields, const std::vector<std::pair<EtaBarTimespan, int>> &barSets) = 0;

    /**
     * @brief 设置需要的Bar类型，以及缓存K线的条数
     * @param timeSpan
     * @param count
     */
    virtual void set_require_bars(EtaBarTimespan timeSpan, int count) = 0;

    /**
     * @brief 设置需要的因子字段
     * @param fields
     */
    virtual void set_require_fields(const std::vector<std::string> &fields) = 0;
    /**
     * @brief 设置撮合参数
     * @param timeSpan 撮合频率
     * @param trade_time_ranges 可以交易的时间区间，参数格式为{{"09:00", "10:00"}, {"10:30", "11:30"}}
     */
    virtual void set_match_params(EtaBarTimespan time_span, const std::vector<std::pair<std::string, std::string>> &trade_time_ranges) = 0;

    /**
     * @brief 设置盘前事件响应时间
     * @param time 格式：09:00
     */
    virtual void set_pre_market_time(const std::string &time) = 0;

    /**
     * @brief 设置交易结束时间
     * @param time 格式：15:00
     */
    virtual void set_closing_time(const std::string &time) = 0;

    /**
     * @brief 设置回测日期区间（仅回测）
     * @param start_date 20180101
     * @param end_date 20190101
     */
    virtual void set_custom_back_test_param(int start_date, int end_date) = 0;

    /**
     * @brief 设置滑点
     * @param slippage
     */
    virtual void set_back_test_slippage(int slippage) = 0;

    /**
     * @brief 设置市场初始资金
     * @param market
     * @param value
     */
    virtual void set_back_test_cash(const std::string &market, double value) = 0;

    /**
     * @brief 设置手续费费率
     * @param commission
     */
    virtual void set_commission(double commission) = 0;


    /**
     * @brief 设置初始化股票池
     * @param instset
     * @param symbols
     */
    virtual void set_symbol_pool(const std::vector<std::string> &instset, const std::vector<std::string> &symbols) = 0;


    /**
     * @brief 设置Timer回调时钟,毫秒
     * @param cycle_ms
     */
    virtual void set_timer_cycle(uint32_t cycle_ms) = 0;

    /**
     * @brief 设置回调方式，目前支持组回调和单标的回调模式
     * @param timeout_ms 超时回调时间（ms），仅组回调模式
     * @param only_group 默认为true，为仅组回调模式
     */
    virtual void set_group_mode(uint32_t timeout_ms = 10000, bool only_group = true) = 0;

    /**
     * @brief 设置算法下单参数
     * @param on 是否开启算法交易
     * @param time_wait 算法下单时间间隔
     * @param avg_count 取历史N天的平均值
     * @param end_time 算法下单交易日结束时间
     */
    virtual void set_algo_param(bool on, int time_wait, int avg_count, const std::string &end_time = "145500") = 0;


    /*
     * 盘前调用函数
     */
    /**
     * @brief 设置当前分析器关注的标的
     * @param symbols
     */
    virtual void set_focus_symbols(const std::set<std::string> &symbols) = 0;

    /*
     * 以下是时间函数
     */
    /**
     * @brief 当前时间
     * @return
     */
    virtual int64_t time_now() = 0;

    /**
     * @brief 获取当前交易日
     * @return
     */
    virtual int get_curr_trade_date() = 0;

    /**
     * @brief 获取区间内交易日
     * @param start_date: 开始交易日(20170101)
     * @param end_date: 结束交易日
     * @param markets: 交易所列表
     * @return 返回交易日
     */
    virtual std::vector<int> get_trade_dates(int start_date, int end_date, const std::string &market) = 0;

    /**
     * @brief 获取市场前n个交易日
     * @param tradeDate
     * @param market
     * @param count
     * @return
     */
    virtual std::vector<int> get_prev_trade_dates(int trade_date, int count, const std::string& market) = 0;

    /**
     * @brief 获取市场后n个交易日，取未来还未发布法定交易日的日期有风险
     * @param trade_date
     * @param count
     * @param market
     * @return
     */
    virtual std::vector<int> get_next_trade_dates(int trade_date, int count, const std::string &market) = 0;

    /**
     * @brief 获取标的交易日区间包含交易天数
     * @param symbol 标的
     * @param start_date 含当日
     * @param end_date 含当日
     * @return
     */
    virtual int get_trade_day_interval(int start_date, int end_date, const std::string &symbol) = 0;


    /**
     * @brief 获取标的最近的[过去]交易日，不存在为0
     * @param symbol
     * @param date
     * @return
     */
    virtual int get_last_trade_date(int date, const std::string &symbol) = 0;

    /**
     * @brief 获取标的最近的[未来]交易日，不存在为0
     * @param symbol
     * @param date
     * @return
     */
    virtual int get_next_trade_date(int date, const std::string &symbol) = 0;

    /**
     * @brief 获取标的离下一次收盘时间的秒数间隔
     * @param symbol
     * @return
     */
    virtual int64_t get_seconds_to_next_close_time(const std::string &symbol) = 0;


    //
    /**
     * @brief 获取标的离下一次开盘时间的秒数间隔
     * @param symbol
     * @return
     */
    virtual int64_t get_seconds_to_next_start_time(const std::string &symbol) = 0;

    /**
     * @brief 是否交易时间
     * @param symbol
     * @return
     */
    virtual bool is_trading_now(const std::string &symbol) = 0;


    /*
     * 基础信息函数
     */
    /**
     * @brief 获取标的基础信息
     * @param symbol
     * @return
     */
    virtual EtaRefData get_ref_data(const std::string &symbol) = 0;

    /**
     * @brief 停牌判断
     * @param symbol
     * @param date
     * @return
     */
    virtual bool is_suspend(const std::string &symbol, int date) = 0;

    /**
     * @brief ST判断
     * @param symbol
     * @param date
     * @return
     */
    virtual bool is_ST(const std::string &symbol, int date) = 0;


    /**
     * @brief 上市判断
     * @param symbol
     * @param date
     * @return
     */
    virtual bool is_listed(const std::string &symbol, int date) = 0;

    /**
     * @brief 获取成分股的标的数据
     * @param instset 集合名称:指数代码、板块代码、指数代码列表或者行业板块代码列表
     * @param trade_date 交易日
     * @param res <集成名称, [标的集合]>
     */
    virtual std::map<std::string, std::vector<std::string>> get_constituent_symbols(const std::vector<std::string> &instset, int trade_date) = 0;

    /**
     * @brief 获取连续合约的真实标的
     * @param maincfs 连续合约代码
     * @param trade_date 交易日
     * @return 返回真实标的 
     */
    virtual std::string get_continuous_symbol(const std::string &maincfs, int trade_date) = 0;

    /**
     * @brief 获取某个成分股的标的数据
     * @param instset 标的集合名称
     * @param trade_date 交易日
     * @return
     */
    virtual std::vector<std::string> get_constituent_symbols_by_set(const std::string &instset, int trade_date) = 0;

    /**
     * @brief 期货特殊合约集合，
     * @param appoint_type Z0, Z1, M1-M12
     * @param trade_date
     * @return
     */
    virtual std::vector<std::string> get_appointed_symbols(const std::string &appoint_type, int trade_date) = 0;


    /*
     * k线数据接口
     */

    /**
     * @brief 取单个symbol前count条kBar
     * @param symbol 标的
     * @param time_span k线类型
     * @param count 取前count根
     * @param price_mode 复权模式
     * @param skip_suspended 是否跳过停牌
     * @return
     */
    virtual DataFramePtr get_bars_history(const std::string &symbol, EtaBarTimespan time_span, int count, EtaPriceMode price_mode , int skip_suspended = 1) = 0;

    /**
     * @brief 一组symbol取K线,
     * @param symbols
     * @param time_span
     * @param count
     * @param price_mode
     * @param fields
     * @return map<field, DATA> 数据结构按照[symbol, date]二维排列
     */
    virtual std::map<std::string, DataFramePtr> get_bars_history(const std::vector<std::string> &symbols, EtaBarTimespan time_span, int count, EtaPriceMode price_mode, std::vector<std::string> fields) = 0;

    /*
     * 策略下单接口
     */

    /**
     * @brief 调整标的仓位，即根据需要进行下撤单，无直接操作下单接口
     * @param symbol 标的
     * @param qty 量
     * @param price 价格，市价时为0
     * @param side 买卖方向
     * @param tif time in force
     * @param remark 备注，会在订单信息中显示
     * @param price_list 报价
     * @return orderId
     */
    virtual std::string target_position(const std::string &symbol, double qty, double price = 0.0, EtaPositionSide side = EPS_LONG, EtaTimeInForce tif = TIF_NONE,
                                const std::string &remark = "", EtaPriceList price_list = EPL_NONE) = 0;

    /**
     * @brief 目标比例下单,按照总资产的百分比进行下单
     * @param symbol
     * @param percent
     * @param price
     * @param side
     * @param tif
     * @param remark
     * @return
     */
    virtual std::string target_percent(const std::string &symbol, double percent, double price = 0.0, EtaPositionSide side = EPS_LONG, EtaTimeInForce tif = TIF_NONE,
                                       const std::string &remark = "") = 0;

    /**
     * @brief 调整标的仓位（算法模式）
     * @param mode
     * @param symbol
     * @param qty
     * @param side
     */
    virtual void target_position(const std::string &mode, const std::string &symbol, double qty, EtaPositionSide side = EPS_LONG) = 0;

    /**
     * @brief 撤单
     * @param symbol
     * @param side
     * @param remark
     */
    virtual void cancel_order(const std::string &symbol, EtaPositionSide side = EPS_LONG, const std::string &remark = "") = 0;

    /*
     * 策略信息接口
     */

    /**
     * @brief
     * @param symbol
     * @return
     */

    /**
     * @brief 获取所有持仓的标的
     * @return
     */
    virtual std::vector<std::string> get_position_symbols() const = 0;

    /**
     * @brief 获取所有持仓或者关注的标的
     * @return
     */
    virtual std::vector<std::string> get_focus_and_position_symbols() const = 0;

    /**
     * @brief 获取策略信息
     * @return
     */
    virtual EtaStrategyInfo get_strategy_info() = 0;

    /**
     * @brief 获取账户信息
     * @param symbol
     * @param market
     * @return
     */
    virtual const EtaAccount &get_account(const std::string &symbol, const std::string &market = "") = 0;

    /**
     * @brief 获取标的当前仓位
     * @param symbol
     * @param side
     * @return
     */
    virtual const EtaSymbolPosition &get_symbol_position(const std::string &symbol, EtaPositionSide side = EPS_LONG) = 0;

    /**
     * @brief 获取全部标的持仓量
     * @return
     */
    virtual std::vector<EtaSymbolPosition> get_symbol_positions() = 0;

    /**
     * @brief 获取标的汇总持仓量
     * @param symbol
     * @return
     */
    virtual const EtaOverallPosition &get_overall_position(const std::string &symbol) = 0;

    /**
     * @brief 获取策略盈亏
     * @return
     */
    virtual const EtaStrategyPnL &get_strategyPnL() = 0;

    /**
     * @brief 获取当前股票池标的
     * @return
     */
    virtual std::vector<std::string> get_symbol_pool() = 0;

    /**
     * @brief 是否关注了某只symbol
     * @param symbol
     * @return
     */
    virtual bool is_focus_symbol(const std::string &symbol) = 0;

    /**
     * @brief 判断是否回测模式
     * @return
     */
    virtual bool is_back_test() = 0;

    /**
     * @brief 判断是否模拟盘模式
     * @return
     */
    virtual bool is_real_env() = 0;

    /*
     * 获取因子数据函数
     */
    /**
     * @brief 获取一组标的, 某一个交易日的数据
     * @param symbols 标的
     * @param fields 因子
     * @param trade_date 交易日
     * @return
     */
    virtual DataFramePtr get_fields_one_day(const std::vector<std::string> &symbols, const std::vector<std::string> &fields, int trade_date) = 0;

    /**
     * @brief 获取一个标的, 某一段交易日的数据
     * @param symbol 标的
     * @param fields 因子
     * @param start_date 起始日期
     * @param end_date 截止日期
     * @return
     */
    virtual DataFramePtr get_fields_range_days(const std::string &symbol, const std::vector<std::string> &fields, int start_date, int end_date) = 0;

    /**
     * @brief 获取一个标的, 往前的n条数据(不包含当前交易日)
     * @param symbol 标的
     * @param fields 因子
     * @param trade_date 日期
     * @param count 向前取的数量
     * @return
     */
    virtual DataFramePtr get_fields_count_days(const std::string &symbol, const std::vector<std::string> &fields, int trade_date, int count) = 0;

    /**
     * @brief 获取一个标的, 某个交易日(不包含当前交易日)的财报数据(可往前回溯)
     * @param symbols 标的
     * @param fields 因子
     * @param trade_date 日期
     * @param report 报告期，格式 yyyyQ[1,2,3,4]，如2017Q1=2017年一季报
     * @return
     */
    virtual DataFramePtr get_finance_fields(const std::vector<std::string> &symbols, const std::vector<std::string> &fields, int trade_date,
                                    const std::string &report) = 0;

    /**
     * @brief 获取一天的表数据
     * @param symbol
     * @param field
     * @param trade_date
     * @param columns 表内字段
     */
    virtual DataFramePtr get_table_field_one_day(const std::string &symbol, const std::string &field, int trade_date, const std::vector<std::string> &columns) = 0;

    /**
     * @brief 获取指定天数的表数据
     * @param symbol
     * @param field
     * @param trade_date
     * @param count
     * @param columns 表内字段
     */
    virtual DataFramePtr get_table_field_count_days(const std::string &symbol, const std::string &field, int trade_date, int count, const std::vector<std::string> &columns) = 0;

    /**
     * @brief 获取时间范围的表数据
     * @param symbol
     * @param field
     * @param start_date
     * @param end_date
     * @param columns 表内字段
     */
    virtual DataFramePtr get_table_field_range_days(const std::string &symbol, const std::string &field, int start_date, int end_date, const std::vector<std::string> &columns) = 0;



private:
    std::string _analyzerName;
};

}

#if defined _WIN32 || defined _WIN64
#else
#pragma GCC visibility pop
#endif