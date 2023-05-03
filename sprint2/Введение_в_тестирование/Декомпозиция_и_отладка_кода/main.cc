#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    std::string operation_code;
    is >> operation_code;

    if (operation_code == "NEW_BUS"s) {
        q.stops.clear();
        q.type = QueryType::NewBus;
        is >> q.bus;

        size_t stop_count = 0;
        is >> stop_count;
        std::vector<std::string> stops;
        stops.resize(stop_count);

        for (std::string& stop : stops) {
            is >> stop;
            q.stops.push_back(stop);
        }
    } else if (operation_code == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (operation_code == "STOPS_FOR_BUS"s) {
        q.type =  QueryType::StopsForBus;
        is >> q.bus;
    } else {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    std::map<std::string, std::vector<std::string>> stops_to_buses;
    std::string                                     stop;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if (r.stops_to_buses.count(r.stop) == 0) {
        os << "No stop"s;
        return os;
    }

    bool is_first = true;
    for (const std::string& bus : r.stops_to_buses.at(r.stop)) {
        if (is_first) {
            os << bus;
            is_first = false;
        } else {
            os << " "s << bus;
        }
    }
    return os;
}

struct StopsForBusResponse {
    std::map<std::string, std::vector<std::string>> buses_to_stops;
    std::map<std::string, std::vector<std::string>> stops_to_buses;
    std::string                                     bus;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if (r.buses_to_stops.count(r.bus) == 0) {
        os << "No bus"s;
        return os;
    }

    size_t count = 0;
    for (const std::string& stop : r.buses_to_stops.at(r.bus)) {
        ++count;
        os << "Stop "s << stop << ": "s;
        if (r.stops_to_buses.at(stop).size() == 1) {
            os << "no interchange"s;
        } else {
            bool is_first = true;
            for (const std::string& transfer_bus : r.stops_to_buses.at(stop)) {
                if (transfer_bus != r.bus) {
                    if (!is_first) {
                        os << " ," << transfer_bus;
                    } else {
                        is_first = false;
                        os << transfer_bus;
                    }
                }
            }
        }
        if (r.buses_to_stops.at(r.bus).size() != count) {
            os << std::endl;
        }
    }
    return os;
}

struct AllBusesResponse {
    std::map<std::string, std::vector<std::string>> buses_to_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if (r.buses_to_stops.empty()) {
        os << "No buses"s;
        return os;
    }

    size_t count = 0;
    for (const auto& [bus, stops] : r.buses_to_stops) {
        ++count;
        os << "Bus "s << bus << ": "s;
        bool is_first = true;
        for (const auto& stop : stops) {
            if (!is_first) {
                os << " "s << stop;
            } else {
                is_first = false;
                os << stop;
            }
        }
        if (count != r.buses_to_stops.size()) {
            os << std::endl;
        }
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops_[bus] = stops;
        for (const auto& stop : stops) {
            stops_to_buses_[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse result;
        result.stops_to_buses = stops_to_buses_;
        result.stop = stop;
        return result;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse result;
        result.buses_to_stops = buses_to_stops_;
        result.stops_to_buses = stops_to_buses_;
        result.bus = bus;
        return result;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse result;
        result.buses_to_stops = buses_to_stops_;
        return result;
    }

private:
    std::map<std::string, std::vector<std::string>> buses_to_stops_, stops_to_buses_;
};

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
    return 0;
}