#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/vector.h>
#include "point2d.hpp"
#include "chull.cpp"

namespace nb = nanobind;

template<typename T>
void bind_point2d(nb::module_& m, const char* name) {
    using pt = point<T>;
    nb::class_<pt>(m, name)
        .def("__init__", [](pt* self, T x, T y, int id) { new (self) pt{x, y, id}; },
             nb::arg("x"), nb::arg("y"), nb::arg("id") = 0)
        .def_rw("x", &pt::x)
        .def_rw("y", &pt::y)
        .def_rw("id", &pt::id)
        .def(nb::self == nb::self)
        .def(nb::self < nb::self)
        .def(nb::self > nb::self)
        .def(nb::self += nb::self)
        .def(nb::self + nb::self)
        .def(nb::self -= nb::self)
        .def(nb::self - nb::self)
        .def("__mul__", &pt::operator*, nb::arg("other"))
        .def("__xor__", &pt::operator^, nb::arg("other"))
        .def("belongs", &pt::belongs, nb::arg("a"), nb::arg("b"))
        .def("inside", &pt::inside, nb::arg("a"), nb::arg("b"))
        .def("left", &pt::left, nb::arg("a"), nb::arg("b"));
}

NB_MODULE(point2d, m) {
    bind_point2d<long>(m, "point");
    m.def("chull", &chull<long>, nb::arg("points"));
}
