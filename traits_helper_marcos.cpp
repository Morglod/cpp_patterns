/*

Idea is that we have traits with single implementation for any type

We create static "callable" (lambda) of something from trait,
then use it in non-template method

Same approach may be used for getters/setters

*/


#include <concepts>
#include <stdio.h>

#define TRAIT_CALL_INDIRECTION(_NAME, _CONCEPT_NAME, _METHOD_NAME, ...) \
	void impl_trait_call_##_NAME(void* target, void (*method)(void* target, ##__VA_ARGS__)); \
	template<typename T, typename... ArgsT> \
	struct _LambdaCall_##_NAME { \
		static void call(void* target, ArgsT... args) { \
			T* t = (T*)target; \
			t->_METHOD_NAME(args...); \
		} \
	}; \
	template<_CONCEPT_NAME T> \
	void _NAME(T& x) { \
		impl_trait_call_##_NAME(&x, &_LambdaCall_##_NAME<T, ##__VA_ARGS__ >::call); \
	}

#define TRAIT_CALL_INDIRECTION_IMPL(_NAME, ...) \
	void impl_trait_call_##_NAME(void* target, void (*method)(void* target, ##__VA_ARGS__ ))


// Here we define our "trait"
// Something that has 2 methods on its instance; void draw() & void add(int)
template<typename T>
concept CDrawable = requires (T t) {
	{ t.draw() } -> std::convertible_to<void>;
	{ t.add(int{}) } -> std::convertible_to<void>;
};

struct Square {
	int what;

	void draw() {
		printf("Square: %i\n", what);
	}

    void add(int x) {
        what += x;
    }
};

struct Circle {
	int what;
	void draw() {
		printf("Circle: %i\n", what);
	}

    void add(int x) {
        what += x;
    }
};

TRAIT_CALL_INDIRECTION(add_10, CDrawable, add, int)

TRAIT_CALL_INDIRECTION_IMPL(add_10, int) {
    method(target, 10);
}

TRAIT_CALL_INDIRECTION(call_draw, CDrawable, draw)

TRAIT_CALL_INDIRECTION_IMPL(call_draw) {
    method(target);
}

int main(int argc, char **argv)
{
    Square s{ 123 };
    Circle c{ 446 };

    add_10(c);

	call_draw(s);
	call_draw(c);

    return 0;
}
