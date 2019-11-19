#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <op.h>
#include <string>

namespace py = pybind11;

OPResult<float, float> op2D_linker(vector<float> &x, vector<float> &y, float beta)
{
    return op2D(x, y, beta);
}

OPResult<float, float> op2Dcc_linker(vector<float> &x, vector<float> &y, float beta)
{
    return op2Dcc(x, y, beta);
}

OPResult<float, float> op1D_linker(vector<float> &x, vector<float> &y, float beta)
{
    return op1D(x, y, beta);
}

PYBIND11_MODULE(optimalpartitioning, m)
{
    py::class_<OPResult<float, float>>(m, "OPResult")
        .def(py::init([](vector<unsigned int> cp, vector<float> x, vector<float> y, double cost) {
            return new OPResult<float, float>(cp, x, y, cost);
        }))
        .def_readwrite("cp", &OPResult<float, float>::cp)
        .def_readwrite("x", &OPResult<float, float>::x)
        .def_readwrite("y", &OPResult<float, float>::y)
        .def_readwrite("cost", &OPResult<float, float>::cost);

    m.doc() = "Piece-wise linear Optimal Partitioning (OP) segmentation algorithm, with continuity constraint. "; // optional module docstring
    m.def("op2D", &op2D_linker, "op2D algorithm", py::arg("x"), py::arg("y"), py::arg("penalty"));
    m.def("op1D", &op1D_linker, "op1D algorithm", py::arg("x"), py::arg("y"), py::arg("penalty"));
    m.def("op2Dcc", &op2Dcc_linker, "op2D algorithm with continuity constraint", py::arg("x"), py::arg("y"), py::arg("penality"));
}