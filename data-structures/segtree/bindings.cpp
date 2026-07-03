#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/function.h>
#include <nanobind/make_iterator.h>
#include "segtree.hpp"

namespace nb = nanobind;

template<typename T>
void bind_segtree(nb::module_& m, const char* name) {
    using st = segtree<T>;
    nb::class_<st>(m, name)
        .def(nb::init<size_t, T, std::function<T(T,T)>>(),
             nb::arg("size"), nb::arg("id"), nb::arg("op"))
        .def("build", &st::build, nb::arg("leaves"))
        .def("set", &st::set, nb::arg("i"), nb::arg("val"))
        .def("query",  &st::query,  nb::arg("l"), nb::arg("r"))
        .def("__len__", &st::len)
        .def("__getitem__", &st::getitem, nb::arg("i"))
        .def("__setitem__", &st::setitem, nb::arg("i"), nb::arg("val"))
        .def("__iter__",
             [](st& s) {
                 return nb::make_iterator(nb::type<st>(), "Iterator",
                                           s.begin(), s.end());
             }, nb::keep_alive<0, 1>());
}

NB_MODULE(segtree, m) {
    bind_segtree<nb::object>(m, "segtree");
}
