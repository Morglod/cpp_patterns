// Problem:
// We need some templated functionality in base class but without templating whole Base

class _Base {
  // a lot of stuff
};

template<typename T>
class BaseT : public _Base {
  // some templated stuff
};

class Foo : BaseT<Foo> {
};
