#ifndef ELEMENT_DATAFRAME_JL_H
#define ELEMENT_DATAFRAME_JL_H

#include <string>
#include <list>
#include <vector>

#include "powsybl-cpp.h"

// Accumulates typed columns and exposes them as a C `dataframe` for element
// creation/update. Owns all the column storage so the pointers held by the C
// series stay valid for the duration of the (synchronous) Java call.
class ElementDataframe {
public:
    void add_string_series(const std::string& name, bool index, const std::vector<std::string>& values);
    void add_double_series(const std::string& name, bool index, const std::vector<double>& values);
    void add_int_series(const std::string& name, bool index, const std::vector<int>& values);
    void add_bool_series(const std::string& name, bool index, const std::vector<int>& values);
    void finish_dataframe();
    dataframe build_dataframe();
    std::vector<dataframe> build_dataframes();

private:
    dataframe makeDataframe(std::vector<series>& frame);
    void appendSeries(bool index, int type, void* ptr, int length);

    std::list<std::string> names_;
    std::list<std::vector<std::string>> stringData_;
    std::list<std::vector<char*>> stringPtrs_;
    std::list<std::vector<double>> doubleData_;
    std::list<std::vector<int>> intData_;
    std::vector<series> current_;
    std::list<std::vector<series>> frames_;
    series emptySeries_{};
};

#endif