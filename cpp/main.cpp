#include <pybind11/pybind11.h>

#include <iostream>
#include <memory>

namespace py = pybind11;

constexpr uint32_t numEchos{10};

template <typename T, uint32_t N> class TFixedVector
{
  public:
    bool push_back(T t)
    {
        if (current >= capacity)
        {
            return false;
        }
        members[current] = t;
        current++;
        return true;
    }
    uint32_t size()
    {
        return current;
    }
    uint32_t current{0};
    uint32_t capacity{N};
    T members[N];
};

struct Echo
{
    int echoDist{0};
};

struct Trace
{
    TFixedVector<Echo, 10> echos;
    uint32_t numActiveSensors;
};

void printInts(const Trace& trace)
{
    for (int i = 0; i < trace.echos.current; i++)
    {
        std::cout << trace.echos.members[i].echoDist << std::endl;
    }
}

PYBIND11_MODULE(cppmain, m)
{
    py::class_<TFixedVector<Echo, numEchos>>(m, "EchoTFixedVector")
        .def(py::init<>())
        .def(py::init([](const py::iterable& it) {
            auto v = std::unique_ptr<TFixedVector<Echo, numEchos>>(new TFixedVector<Echo, numEchos>());
            for (py::handle h : it)
            {
                v->push_back(h.cast<Echo>());
            }
            return v.release();
        }))
        .def("append", [](TFixedVector<Echo, numEchos>& v, const Echo& e) { v.push_back(e); })
        .def(
            "__getitem__",
            [](const TFixedVector<Echo, numEchos>& v, size_t i) -> const Echo& {
                return v.members[static_cast<size_t>(i)];
            },
            py::return_value_policy::reference_internal)
        .def("__len__", &TFixedVector<Echo, numEchos>::size)
        .def_readwrite("capacity", &TFixedVector<Echo, numEchos>::capacity);

    py::class_<Echo>(m, "Echo")
        .def(py::init<>())
        .def(py::init([](uint32_t f_echoDist) {
            Echo e;
            e.echoDist = f_echoDist;
            return e;
        }))
        .def_readwrite("echoDist", &Echo::echoDist);

    py::class_<Trace>(m, "Trace").def(py::init<>()).def_readwrite("echos", &Trace::echos);

    m.def("printInts", &printInts, "printInts");
}

int main()
{
    return 0;
}
