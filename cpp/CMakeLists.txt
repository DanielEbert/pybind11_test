cmake_minimum_required(VERSION 3.18)
project(cppmain)

list(APPEND CMAKE_PREFIX_PATH "/home/user/P/pyright_test/venv/lib/python3.10/site-packages/pybind11/share/cmake/pybind11/")
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(cppmain main.cpp)

