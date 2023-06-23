#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <iostream>
#include <vector>

namespace py = pybind11;

struct Echo
{
    int echoDist{0};
};

struct Trace
{
    std::vector<Echo> echos;
};

void printInts(const Trace& trace)
{
    for (const Echo& i : trace.echos)
    {
        std::cout << i.echoDist << std::endl;
    }
}

PYBIND11_MAKE_OPAQUE(std::vector<Echo>);

PYBIND11_MODULE(cppmain, m) {
    py::bind_vector<std::vector<Echo>>(m, "VectorEcho");

    py::class_<Echo>(m, "Echo")
        .def(py::init<>())
        .def_readwrite("echoDist", &Echo::echoDist);

    py::class_<Trace>(m, "Trace")
        .def(py::init<>())
        .def_readwrite("echos", &Trace::echos);
    
    m.def("printInts", &printInts, "printInts");
}


int main()
{
    return 0;
}
