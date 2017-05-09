//
// Created by zts on 17-3-30.
//

#ifndef SSDB_T_SCAN_H
#define SSDB_T_SCAN_H

#include <string>

template<typename T>
class ScanResultProcessor {
public:
    static void process(std::vector<std::string> &resp, T *mit) {
        resp.push_back(mit->key);
    }
};

template<>
class ScanResultProcessor<ZIterator> {
public:
    static void process(std::vector<std::string> &resp, ZIterator *mit) {
        resp.push_back(mit->key);
        resp.push_back(str(mit->score));
    }

};

template<>
class ScanResultProcessor<HIterator> {
public:
    static void process(std::vector<std::string> &resp, HIterator *mit) {
        resp.push_back(mit->key);
        resp.push_back(mit->val);
    }
};


template<typename T>
bool doScanGeneric(T *mit, const std::string &pattern, uint64_t limit, std::vector<std::string> &resp) {

    bool end = false; //scan end

    bool fulliter = (pattern == "*");
    while (mit->next()) {
        if (limit == 0) {
            break; //check limit
        }

        if (fulliter || stringmatchlen(pattern.data(), pattern.length(), mit->key.data(), mit->key.length(), 0)) {
            ScanResultProcessor<T>::process(resp, mit);
        } else {
            //skip
        }
        limit--;
        if (limit == 0) {
            break; //stop now
        }
    }

    if (!mit->next()) { // check iter , and update next as last key
        //scan end
        end = true;
    } else if (limit != 0) {
        //scan end
        end = true;
    }

    return end;
}

#endif //SSDB_T_SCAN_H
