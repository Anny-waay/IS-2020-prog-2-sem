#include <fstream>
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

#pragma execution_character_set("utf-8")
#define PI 3.14159265358979323846

struct Station {
    int num;
    std::string type_vehicle;
    std::vector<std::string> streets;
    std::vector<std::string> routes;
    double coord1;
    double coord2;
};

double get_dist_km(double lat1, double lon1, double lat2, double lon2) {
    int r = 6371; 
    double dLat = (lat2 - lat1) * (PI / 180);  
    double dLon = (lon2 - lon1) * (PI / 180);
    double a =std::sin(dLat / 2) * std::sin(dLat / 2) + std::cos(lat1 * (PI / 180)) * std::cos(lat2 * (PI / 180)) * std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    double d = r * c; 
    return d;
}

class Transport {
private:
    std::vector<Station> stations;
public:
    Transport(std::string file_name) {
        pugi::xml_document doc;
        std::ifstream stream(file_name);
        pugi::xml_parse_result result = doc.load(stream);
        pugi::xml_node data = doc.child("dataset").child("transport_station");

        std::string str;

        while (data) {
            str = data.child_value("number");
            int num = atoi(str.c_str());
            std::vector <std::string> streets;
            std::string loc = data.child_value("location");
            str.clear();
            int f = 0;
            for (char c : loc) {
                if (f == 1) 
                    f = 0; 
                else
                    if (c == ',') {
                        streets.push_back(str);
                        str.clear();
                        f = 1;
                    }
                    else {
                        str.append(1, c);
                    }
            }
            streets.push_back(str);

            std::vector <std::string> routes;
            std::string r = data.child_value("routes");
            str.clear();
            for (char c : r) {
                if (c == ',' || c == '.') {
                    routes.push_back(str);
                    str.clear();
                }
                else str.append(1, c);
            }
            routes.push_back(str);

            std::string coord = data.child_value("coordinates");
            double coord1 = atof(coord.substr(0, 9).c_str());
            double coord2 = atof(coord.substr(10, 9).c_str());

            data = data.next_sibling();
            Station st = {
                num,
                data.child_value("type_of_vehicle"),
                streets,
                routes,
                coord1,
                coord2
            };

            stations.push_back(st);
        }
    }

    Transport(const Transport& other) {
        stations = other.stations;
    }

    Transport& operator=(const Transport& other) {
        if (&other == this)
            return *this;
        stations.clear();
        stations = other.stations;
        return *this;
    }

    void max_stations(std::string type) {
        std::vector<std::string> res_r;
        std::vector<int> res_num;
        for (auto st : stations) {
            if (st.type_vehicle == type) {
                for (auto r : st.routes) {
                    int index = -1;
                    for (size_t i = 0; i < res_r.size(); ++i)
                        if (r == res_r[i])
                            index = i;
                    if (index == -1) {
                        res_r.push_back(r);
                        res_num.push_back(1);
                    }
                    else
                        res_num[index] += 1;
                }
            }
        }
        int max = -1;
        std::string ans;
        for (size_t i = 0; i < res_num.size(); ++i) {
            if (res_num[i] > max) {
                max = res_num[i];
                ans = res_r[i];
            }
        }
        std::cout << type << ": маршрут №" << ans << " остановок " << max << std::endl;
    }

    void max_dist(std::string type) {
        std::vector<std::string> res_r;
        std::vector<std::vector<int>> res_num;
        for (auto st : stations) {
            if (st.type_vehicle == type) {
                for (auto r : st.routes) {
                    int index = -1;
                    for (size_t i = 0; i < res_r.size(); ++i)
                        if (r == res_r[i])
                            index = i;
                    if (index == -1) {
                        res_r.push_back(r);
                        std::vector<int> temp;
                        temp.push_back(st.num-1);
                        res_num.push_back(temp);
                    }
                    else
                        res_num[index].push_back(st.num-1);
                }
            }
        }
        std::string ans;
        double max_d = -1;
        for (size_t i = 0; i < res_num.size(); ++i) {
            std::vector<std::vector<double>> distanse(res_num[i].size());
            for (int j = 0; j < res_num[i].size(); j++)
                distanse[j].assign(res_num[i].size(), 0);
            for (int j = 0; j < res_num[i].size(); ++j) {
                for (int k = 0; k < res_num[i].size(); ++k) {
                    if(j!=k)
                    distanse[j][k] = get_dist_km(stations[res_num[i][j]].coord1, stations[res_num[i][j]].coord2, stations[res_num[i][k]].coord1, stations[res_num[i][k]].coord2);
                }
            }

            std::vector <double> dist(res_num[i].size(), 1000000000);
            dist[0] = 0;
            std::vector <bool> visited(res_num[i].size(), 0);
            double res = 0;
            for (int k = 0; k < res_num[i].size(); ++k)
            {
                double min_dist = 1000000000;
                int ver = 0;
                for (int j = 0; j < res_num[i].size(); ++j)
                    if (!visited[j] && dist[j] < min_dist)
                    {
                        min_dist = dist[j];
                        ver = j;
                    }
                res += min_dist;
                visited[ver] = 1;
                for (int v = 0; v < res_num[i].size(); ++v)
                    dist[v] = std::min(dist[v], distanse[ver][v]);
            }
            if (res > max_d) {
                max_d = res;
                ans = res_r[i];
            }
        }
        std::cout << type << ": маршрут №" << ans << " - " << max_d << "км"<<std::endl;
    }

    void max_street() {
        std::vector<std::string> res_str;
        std::vector<int> res_num;
        for (auto st : stations) {
            for (auto str : st.streets) {
                int index = -1;
                for (size_t i = 0; i < res_str.size(); ++i)
                    if (str == res_str[i])
                        index = i;
                if (index == -1) {
                    res_str.push_back(str);
                    res_num.push_back(1);
                }
                else
                    res_num[index] += 1;
            }
        }
        int max = -1;
        std::string ans;
        for (size_t i = 0; i < res_num.size(); ++i) {
            if ((res_num[i] > max)&&(!res_str[i].empty())) {
                max = res_num[i];
                ans = res_str[i];
            }
        }
        std::cout << "Улица с наибольшим числом остановок - " << ans << " остановок " << max << std::endl;
    }
};

int main() {
    system("chcp 65001");
    Transport ans("data.xml");
    
    std::cout << "Максимальное количество остановок" << std::endl;
    ans.max_stations("Автобус");
    ans.max_stations("Трамвай");
    ans.max_stations("Троллейбус");

    std::cout << std::endl << "Максимально длинные маршруты"<<std::endl;
    ans.max_dist("Автобус");
    ans.max_dist("Трамвай");
    ans.max_dist("Троллейбус");

    std::cout <<  std::endl;
    ans.max_street();
    return 0;
}
