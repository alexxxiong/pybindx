# pybindx
autogenerate for pybind11 with just head file

## required package

based on castxml and pygccxml, for parse C++

based on pybind11, for dealing with C++ Python Wrap

## support

1. support feature
* common feature

  * import namespace
  * module and sub module
  * smart_ptr
  * class public arg
  * class public method, include virtual and pure virtual
  * global function
  * enum

* class array arg like xxx[] get/set, include char[]
    ```
    .def_property("symbol", 
                    [](EtaOverallPosition &self) { return self.symbol; }, 
                    [](EtaOverallPosition &self, ::std::vector<char>& arg) { std::copy(arg.begin(), arg.end(), self.symbol); })
    ```

    ```
    .def_property("bids", 
                    [](EtaQuote &self) { return self.bids; }, 
                    [](EtaQuote &self, ::std::vector<::Demo::EtaQtyPrice>& arg) { std::copy(arg.begin(), arg.end(), self.bids); })
    ```

* class method like
    ```
    int test(int argc, char ** argv)
    ```
  will transform this args to list in python

* global class method transform to any other method name, like __repr__
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
