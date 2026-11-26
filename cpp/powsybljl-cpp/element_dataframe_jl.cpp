#include "element_dataframe_jl.h"

void ElementDataframe::add_string_series(const std::string& name, bool index, const std::vector<std::string>& values) {
    names_.push_back(name);
    stringData_.push_back(values);
    std::vector<std::string>& stored = stringData_.back();
    stringPtrs_.emplace_back();
    std::vector<char*>& ptrs = stringPtrs_.back();
    ptrs.reserve(stored.size());
    for (std::string& s : stored) { ptrs.push_back(const_cast<char*>(s.c_str())); }
    appendSeries(index, 0, ptrs.data(), (int) ptrs.size());
}
void ElementDataframe::add_double_series(const std::string& name, bool index, const std::vector<double>& values) {
    names_.push_back(name);
    doubleData_.push_back(values);
    appendSeries(index, 1, doubleData_.back().data(), (int) doubleData_.back().size());
}
void ElementDataframe::add_int_series(const std::string& name, bool index, const std::vector<int>& values) {
    names_.push_back(name);
    intData_.push_back(values);
    appendSeries(index, 2, intData_.back().data(), (int) intData_.back().size());
}
void ElementDataframe::add_bool_series(const std::string& name, bool index, const std::vector<int>& values) {
    // Boolean series are marshalled as 4-byte ints (0/1), exactly like int
    // series: the Java side reads type 2 and 3 from an int* buffer.
    names_.push_back(name);
    intData_.push_back(values);
    appendSeries(index, 3, intData_.back().data(), (int) intData_.back().size());
}

void ElementDataframe::finish_dataframe() {
    frames_.push_back(current_);
    current_.clear();
}

dataframe ElementDataframe::build_dataframe() {
    return makeDataframe(current_);
}

std::vector<dataframe> ElementDataframe::build_dataframes() {
    std::vector<dataframe> result;
    if (!frames_.empty()) {
        for (std::vector<series>& frame : frames_) {
            result.push_back(makeDataframe(frame));
        }
    } else {
        result.push_back(makeDataframe(current_));
    }
    return result;
}

dataframe ElementDataframe::makeDataframe(std::vector<series>& frame) {
    dataframe df;
    df.series = frame.empty() ? &emptySeries_ : frame.data();
    df.series_count = (int) frame.size();
    return df;
}
void ElementDataframe::appendSeries(bool index, int type, void* ptr, int length) {
    series s;
    s.name = const_cast<char*>(names_.back().c_str());
    s.index = index ? 1 : 0;
    s.type = type;
    s.data.ptr = ptr;
    s.data.length = length;
    s.mask = nullptr;
    current_.push_back(s);
}
