#include <pybind11/pybind11.h>

#include <iostream>
#include <memory>
#include <stdexcept>

namespace py = pybind11;

constexpr uint32_t numEchos{10};
constexpr uint32_t numSensors{5};

template <typename T, uint32_t N> class TFixedVector
{
  public:
    void push_back(T t)
    {
        if (current >= capacity)
        {
            throw std::out_of_range("No free space in TFixedVector.");
        }
        members[current] = t;
        current++;
    }

    uint32_t size() const
    {
        return current;
    }

    uint32_t current{0};
    uint32_t capacity{N};
    T members[N];
};

struct Echo
{
    Echo() = default;
    Echo(int f_dst) : dst{f_dst} {};
    int dst{0};
};

struct Meas
{
    Meas() = default;
    Meas(TFixedVector<Echo, numEchos>& f_de, TFixedVector<Echo, numEchos>& f_ce, uint32_t f_sendingSensor)
        : de{f_de}, ce{f_ce}, sendingSensor{f_sendingSensor} {};
    TFixedVector<Echo, numEchos> de;
    TFixedVector<Echo, numEchos> ce;
    uint32_t sendingSensor;
};

struct EFIOut
{
    EFIOut() = default;
    EFIOut(uint32_t f_sendingMask, TFixedVector<Meas, numSensors>& f_measurements)
        : sendingMask{f_sendingMask}, measurements{f_measurements} {};
    uint32_t sendingMask;
    TFixedVector<Meas, numSensors> measurements;
};

void printInts(const Meas& meas)
{
    for (int i = 0; i < meas.de.size(); i++)
    {
        std::cout << meas.de.members[i].dst << std::endl;
    }
}

template <typename T, uint32_t N> void bindTFixedVector(py::module& m, const std::string name)
{
    py::class_<TFixedVector<T, N>>(m, name.c_str())
        .def(py::init<>())
        .def(py::init([](const py::iterable& it) {
            auto v = std::unique_ptr<TFixedVector<T, N>>(new TFixedVector<T, N>());
            for (py::handle h : it)
            {
                v->push_back(h.cast<T>());
            }
            return v.release();
        }))
        .def("append", [](TFixedVector<T, N>& v, const T& e) { v.push_back(e); })
        .def(
            "__getitem__",
            [](const TFixedVector<T, N>& v, size_t i) -> const T& {
                if (i >= v.current)
                {
                    throw std::out_of_range("Index out of range.");
                }
                return v.members[i];
            },
            py::return_value_policy::reference_internal)
        .def("__len__", &TFixedVector<T, N>::size)
        .def_readwrite("capacity", &TFixedVector<T, N>::capacity);
}

PYBIND11_MODULE(cppmain, m)
{
    bindTFixedVector<Echo, numEchos>(m, "EchoTFixedVector");
    bindTFixedVector<Meas, numSensors>(m, "MeasTFixedVector");

    py::class_<Echo>(m, "Echo").def(py::init<>()).def(py::init<int>()).def_readwrite("dst", &Echo::dst);

    py::class_<Meas>(m, "Meas")
        .def(py::init<>())
        .def(py::init<TFixedVector<Echo, numEchos>&, TFixedVector<Echo, numEchos>&, uint32_t>())
        .def_readwrite("de", &Meas::de)
        .def_readwrite("ce", &Meas::ce)
        .def_readwrite("sendingSensor", &Meas::sendingSensor);

    py::class_<EFIOut>(m, "EFIOut")
        .def(py::init<>())
        .def(py::init<uint32_t, TFixedVector<Meas, numSensors>&>())
        .def_readwrite("sendingMask", &EFIOut::sendingMask)
        .def_readwrite("measurements", &EFIOut::measurements);

    m.def("printInts", &printInts, "printInts");
}

int main()
{
    return 0;
}
