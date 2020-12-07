# pybindx
自动化构建python转换函数的转换函数

## 需要安装的包和库

1. 依赖基于castxml的pygccxml解析C++头文件
2. 依赖pybind11转换python

## 支持的形式

* 基本

  * 可引入特定的namespace
  * 多模块引入
  * 智能指针，包括自定义智能指针
  * 类公共成员变量
  * 类公共函数，包括虚函数和纯虚函数，模版类需要实例化
  * 全局（命名空间）函数
  * （类）枚举

* 特殊
  * 类成员变量 为数组 的get/set。
    ```
    .def_property("bids", 
                  [](CLASS &self) { return self.bids; }, 
                  [](CLASS &self, ::std::vector<::Demo::EtaQtyPrice>& arg) { std::copy(arg.begin(), arg.end(), self.bids); })
    ```

    注: char[] 被转换为str来处理
    ```
    .def_property("symbol", 
                  [](CLASS &self) { return self.symbol; }, 
                  [](CLASS &self, ::std::string& arg) { std::copy(arg.begin(), arg.end(), self.symbol); })
    ```

  * 命令行可变参数输入改变为python的list输入
    ```
    int test(int argc, char ** argv) -----> def test(arg: list)
    ```

  * 将类成员函数改名转换为另一些函数，如将print改为__repr__，方便python打印

    ![repr](repr.gif)
    ```
    global_function_mapping:
    - mapping: [ print, __repr__ ]
    ```

    * global free function for display c struct trans to __repr__
        ```
        struct_repr: [writeToJsonString]
        ```
    * class method with no-const-reference arguments
        ```
        int test(int arg1, vector<string> &arg2)    
        ```
        will return arg reference in return arguments
