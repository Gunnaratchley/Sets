#include <iostream>

#include "set.h"

int main()
{
	set<int> my_tree;
	const auto kMaxNumbers = 400000;
	const auto kGap = 3711;

	std::cout << "Checking... (no more output means success)" << std::endl;

	for (auto counter = kGap; counter != 0; counter = (counter + kGap) % kMaxNumbers) {
		my_tree.insert(counter);
	}

	for (auto counter = 1; counter < kMaxNumbers; counter += 2) {
		my_tree.remove(counter);
	}

	for (auto counter = 2; counter < kMaxNumbers; counter += 2) {
		if (!my_tree.contains(counter)) {
			std::cout << "Find error1!" << std::endl;
		}
	}

	for (auto counter = 1; counter < kMaxNumbers; counter += 2) {
		if (my_tree.contains(counter)) {
			std::cout << "Find error2!" << std::endl;
		}
	}

	set<int> tree_copy;
	tree_copy = my_tree;

	for (auto counter = 2; counter < kMaxNumbers; counter += 2) {
		if (!tree_copy.contains(counter)) {
			std::cout << "Find error1!" << std::endl;
		}
	}

	for (auto counter = 1; counter < kMaxNumbers; counter += 2) {
		if (tree_copy.contains(counter)) {
			std::cout << "Find error2!" << std::endl;
		}
	}

	std::cout << "Finished testing" << std::endl;

	set<int> print_tree;
	for (auto counter = 3; counter != 0; counter = (counter + 3) % 70) {
		print_tree.insert(counter);
	}

	print_tree.print();
	return 0;
}