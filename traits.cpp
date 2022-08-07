/*

Idea is that we have traits with single implementation for any type

We create static "callable" (lambda) of something from trait,
then use it in non-template method

Same approach may be used for getters/setters

*/


#include <concepts>
#include <stdio.h>

// Here we define our "trait"
// Something that has 2 methods on its instance; void draw() & void add(int)
template<typename T>
concept CDrawable = requires (T t) {
	{ t.draw() } -> std::convertible_to<void>;
	{ t.add(int{}) } -> std::convertible_to<void>;
};

void call_draw_impl(void* target, void (*t_draw)(void* target)) {
	t_draw(target);
}

// CDrawable.draw call indirection
template<CDrawable T>
void call_draw(T x) {
	call_draw_impl(&x, [](void* target) {
		T* t = (T*)target;
		t->draw();
	});
}

void add_10_impl(void* target, void (*t_add)(void* target, int x)) {
	t_add(target, 10);
}

// CDrawable.add call indirection
template<CDrawable T>
void add_10(T& x) {
	add_10_impl(&x, [](void* target, int x) {
		T* t = (T*)target;
		t->add(x);
	});
}

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


int main(int argc, char **argv)
{
    Square s{ 123 };
    Circle c{ 446 };

    add_10(c);

	  call_draw(s);
	  call_draw(c);

    return 0;
}
