#include "tree.hh"

#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>

#include <Eigen/Dense>

int main() {
    tree::tree<2, size_t, uint32_t> t{};

    {
        std::mt19937_64 rng(0xfeed);
        std::normal_distribution<float> normal_dist(0, 1024);
     
        size_t n = 1000 * 1000 * 10;
        std::vector<std::pair<uint32_t, std::array<size_t, 2>>> to_insert;
        for (size_t i = 0; i < n; i++) {
            std::array<size_t, 2> pos = {
                static_cast<size_t>(1024 * 10 + normal_dist(rng)),
                static_cast<size_t>(1024 * 10 + normal_dist(rng))
            };
            to_insert.push_back({{}, pos});
        }
        if (true) {
            std::sort(to_insert.begin(), to_insert.end(),
                [](const auto& a, const auto& b) -> bool {
                    return decltype(t)::morton_compare(a.second, b.second);
                }
            );
        }
        t.reserve(n);
        {
            using time_type = std::chrono::time_point<std::chrono::high_resolution_clock>;
            time_type start_time = std::chrono::high_resolution_clock::now();
            for (auto &pos: to_insert) {
                t.insert_item(pos.first, pos.second);
            }
            time_type stop_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = stop_time - start_time;
            std::cout << diff.count() << std::endl;
        }

        t.check_items_in_tree();
    }

    {
        std::mt19937_64 rng(0xfeed);
        std::normal_distribution<float> normal_dist(0, 100);

        struct entity {
            Eigen::Vector2f pos;
            Eigen::Vector2f target;
            float max_speed = 4.0;
        };

        std::vector<struct entity> entities (1000 * 1000);

        for (auto& e: entities) {
            e.pos = {
                normal_dist(rng),
                normal_dist(rng),
            };
            e.target = {
                normal_dist(rng),
                normal_dist(rng),
            };
        }

        for (auto& e: entities) {
            Eigen::Vector2f direction = (e.target - e.pos);
            float d = direction.norm();
            if (d > e.max_speed) {
                direction = direction / d * e.max_speed;
            }
            e.pos += direction;

            if (e.pos == e.target) {
                e.target = {
                    normal_dist(rng),
                    normal_dist(rng),
                };
            }
        }

    }

    return 0;
}
