#pragma once
#include <cstdint>

namespace utl {
	template<class T, size_t max_count = 512>
	class static_queue {
	private:
		T elements[max_count];
		int elements_added = 0;
	public:
		int contains(T& element) {
			for (int i = 0; i < elements_added; i++) {
				if (elements[i] == element)
					return i + 1;
			}
			return 0;
		}

		void add(T element) {
			if (elements_added < max_count) {
				elements[elements_added] = element;
				elements_added++;
			}
		}

		void add_unique(T element) {
			if (contains(element)) {
				return;
			}

			add(element);
		}

		void pop_back() {
			if (elements_added == 0)
				return;

			elements_added--;
		}

		void pop_front() {
			if (elements_added == 0)
				return;

			for (int i = 1; i < elements_added; i++) {
				elements[i - 1] = elements[i];
			}

			elements_added--;
		}

		void clear() {
			elements_added = 0;
		}

		T& front() {
			return elements[0];
		}

		T& back() {
			return elements[elements_added - 1];
		}


		int size() {
			return elements_added;
		}
	};
}