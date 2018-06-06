#include <iostream>

#include "doctest.h"
#include "tapp-mesh.hpp"

TEST_CASE("Bounds check on Mesh when using Mesh::at") {
    SUBCASE("An empty mesh should always return std::nullopt") {
        auto mesh = Mesh();
        CHECK(mesh.at(0, 0) == std::nullopt);
        CHECK(mesh.at(0, 1) == std::nullopt);
        CHECK(mesh.at(1, 0) == std::nullopt);
        CHECK(mesh.at(1, 1) == std::nullopt);
    };
    SUBCASE(
        "An mesh should return it's value if inside bounds or std::nullopt") {
        auto mesh = Mesh({4, 4}, {0.0, 60.0, 80.5, 1000.0});
        CHECK(mesh.at(0, 0) == 0.0);
        CHECK(mesh.at(0, 0) == 0.0);
        CHECK(mesh.at(0, 1) == 0.0);
        CHECK(mesh.at(1, 0) == 0.0);
        CHECK(mesh.at(1, 1) == 0.0);
        CHECK(mesh.at(3, 3) == 0.0);
        CHECK(mesh.at(3, 4) == std::nullopt);
        CHECK(mesh.at(4, 3) == std::nullopt);
    };
}

TEST_CASE("Real world dimensions fetching via Mesh::mz and Mesh::rt") {
    SUBCASE("An empty mesh should always return std::nullopt") {
        auto mesh = Mesh();
        CHECK(mesh.mz(0) == std::nullopt);
        CHECK(mesh.mz(1) == std::nullopt);
        CHECK(mesh.mz(2) == std::nullopt);
        CHECK(mesh.rt(0) == std::nullopt);
        CHECK(mesh.rt(1) == std::nullopt);
        CHECK(mesh.rt(2) == std::nullopt);
    };
    SUBCASE("Proper interpolation if inside bounds or std::nullopt") {
        auto mesh = Mesh({4, 4}, {0.0, 75.0, 200.0, 800.0});
        CHECK(mesh.mz(0) == 200.0);
        CHECK(mesh.mz(1) == 400.0);
        CHECK(mesh.mz(2) == 600.0);
        CHECK(mesh.mz(3) == 800.0);
        CHECK(mesh.mz(4) == std::nullopt);
        CHECK(mesh.rt(0) == 0.0);
        CHECK(mesh.rt(1) == 25.0);
        CHECK(mesh.rt(2) == 50.0);
        CHECK(mesh.rt(3) == 75.0);
        CHECK(mesh.rt(4) == std::nullopt);
    };
}
