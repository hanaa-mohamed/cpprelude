#include "catch.hpp"
#include <cpprelude/iterator.h>
#include <cpprelude/rb_tree.h>
#include <cpprelude/algorithm.h>
#include <cpprelude/dynamic_array.h>

#include<string>
#include <functional>
#include <iostream>
using namespace cpprelude;

TEST_CASE("rb_tree test", "[rb_tree]")
{
	using uPair_node = details::pair_node<usize, bool>;
	using bPair_node = details::pair_node<cpprelude::byte, bool>;
	using uRB_TREE = rb_tree<usize, bool>;
	using bRB_TREE = rb_tree<cpprelude::byte, bool>;
	using uIterator = rb_tree_iterator<uPair_node>;
	using bIterator = rb_tree_iterator<bPair_node>;
	using color_type = typename details::rb_node<uPair_node>::color_type;

	dynamic_array<usize> arr;
	std::function <void(uIterator it)> insert = [&arr](uIterator it) {
		arr.insert_back(it->data.key);
	};


	std::function <void(uIterator it)> print = [](uIterator it) {
		auto c = it->color == color_type::RED ? "RED" : "BLACK";
		std::cout << "key: " << it->data.key << " value: " << it->data.key << " color: " << c << std::endl;
		auto temp = it->parent != nullptr ? it->parent->data.key : 0;
		std::cout << "parent: " << temp << " ";
		temp = it->left != nullptr ? it->left->data.key : 0;
		std::cout << "left child: " << temp << " ";
		temp = it->right != nullptr ? it->right->data.key : 0;
		std::cout << "right child: " << temp << std::endl;
		std::cout << "==================================\n";
	};

	SECTION("Case 01")
	{
		tree_map<cpprelude::byte, bool> tree;
		tree.insert('e');
		CHECK(tree.is_rb_tree());
		tree.insert('c');
		CHECK(tree.is_rb_tree());
		tree.insert('b');
		CHECK(tree.is_rb_tree());
		tree.insert('d');
		CHECK(tree.is_rb_tree());
		tree.insert('g');
		CHECK(tree.is_rb_tree());
		tree.insert('f');
		CHECK(tree.is_rb_tree());
		tree.insert('h');
		CHECK(tree.is_rb_tree());

		/*auto it = tree.root();
		CHECK(it->data == 'e');
		auto suc = it++;
		CHECK(suc->data == 'e');
		CHECK(it->data == 'f');

		suc = ++it;
		CHECK(suc->data == 'g');
		CHECK(it->data == 'g');

		it = tree.root();
		auto pre = it--;
		CHECK(pre->data == 'e');
		CHECK(it->data == 'd');

		pre = --it;
		CHECK(pre->data == 'c');
		CHECK(it->data == 'c');*/
	}

	SECTION("Case 2")
	{
		uRB_TREE tree;

		tree.insert(359);
		CHECK(tree.is_rb_tree());
		tree.insert(966);
		CHECK(tree.is_rb_tree());
		tree.insert(105);
		CHECK(tree.is_rb_tree());
		tree.insert(115);
		CHECK(tree.is_rb_tree());
		tree.insert(81);
		CHECK(tree.is_rb_tree());
		tree.insert(255);
		CHECK(tree.is_rb_tree());
		tree.insert(74);
		CHECK(tree.is_rb_tree());
		tree.insert(236);
		CHECK(tree.is_rb_tree());
		tree.insert(809);
		CHECK(tree.is_rb_tree());
	}

	SECTION("Case 03")
	{
		uRB_TREE array;
		array.insert(1);
		CHECK(array.count() == 1);
		//array.inorder_traverse(fun);
		array.insert(2);
		CHECK(array.count() == 2);
		//array.inorder_traverse(fun);
		array.insert(3);
		CHECK(array.count() == 3);
		array.insert(3);
		array.inorder_traverse(insert);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	////Check right right case
	SECTION("Case 04")
	{
		uRB_TREE array;
		array.insert(1);
		CHECK(array.count() == 1);
		array.insert(2);
		CHECK(array.count() == 2);
		array.insert(3);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//Check left left case
	SECTION("Case 05")
	{
		uRB_TREE array;
		array.insert(3);
		CHECK(array.count() == 1);
		array.insert(2);
		CHECK(array.count() == 2);
		array.insert(1);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//Check right left case
	SECTION("Case 06")
	{
		uRB_TREE array;
		array.insert(1);
		CHECK(array.count() == 1);
		array.insert(3);
		CHECK(array.count() == 2);
		array.insert(2);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//Check left right case
	SECTION("Case 07")
	{
		uRB_TREE array;
		array.insert(3);
		CHECK(array.count() == 1);
		array.insert(1);
		CHECK(array.count() == 2);
		array.insert(2);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//node type is pair
	SECTION("Case 08")
	{
		using sPair_node = details::pair_node<usize, std::string>;
		using sIterator = rb_tree_iterator<sPair_node>;
		rb_tree<usize, std::string> tree;

		dynamic_array<usize> arr1;
		std::function <void(sIterator it)> insert = [&arr1](sIterator it) {
			arr1.insert_back(it->data.key);
		};

		sPair_node f(2, "22");
		CHECK(f == 2);
		CHECK(f.value == "22");
		tree.insert(sPair_node(3, "33"));
		CHECK(tree.count() == 1);
		tree.insert(sPair_node(1, "11"));
		CHECK(tree.count() == 2);
		tree.insert(sPair_node(2, "22"));
		CHECK(tree.count() == 3);
		tree.inorder_traverse(insert);
		CHECK(arr1.count() == 3);
		CHECK(cpprelude::is_sorted(arr1.begin(), arr1.count()));
		CHECK(tree.is_rb_tree());
		//search
		tree[1] = "11";
		auto h = tree.lookup(1);
		CHECK(h->data.key == 1);
		CHECK(h->data.value == "11");

	}

	SECTION("Case 09")
	{
		uRB_TREE tree;

		usize i = 3;
		tree.insert(i);
		tree.insert(5);
		//tree.inorder_traverse(print);
		//Copy constructor
		uRB_TREE tree2(tree);
		tree2.inorder_traverse(insert);
		CHECK(tree2.count() == 2);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(tree2.is_rb_tree());
		tree2.insert(4);
		//Assignment operator
		tree = tree2;
		CHECK(tree.count() == 3);
		arr.reset();
		tree.inorder_traverse(insert);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(tree2.is_rb_tree());
	}

	SECTION("Case 10")
	{
		uRB_TREE array;
		array.insert(3);
		CHECK(array.count() == 1);
		array.insert(1);
		CHECK(array.count() == 2);
		array.insert(2);
		CHECK(array.count() == 3);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
		CHECK(arr.count() == 3);
		//search
		CHECK(array.lookup(3) != array.end());

		CHECK(array.lookup(99) == nullptr);
		auto res = array.lookup(1);
		bool f = res != nullptr && res->data == 1;
		CHECK(f);
	}

	SECTION("Case 11")
	{
		uRB_TREE array;
		array.insert(3);
		CHECK(array.count() == 1);
		array.insert(1);
		CHECK(array.count() == 2);
		array.insert(2);
		//auto fn = print_fn<usize>;
		//array.inorder_traverse(fn);
		CHECK(array.is_rb_tree());
	}

	//insertion
	SECTION("Case 12")
	{
		uRB_TREE tree;
		tree.insert(15);
		tree.insert(1);
		tree.insert(20);
		tree.insert(3);
		tree.insert(13);
		tree.insert(14);
		tree.insert(45);
		tree.insert(34);
		CHECK(tree.count() == 8);
		tree.inorder_traverse(insert);
		CHECK(arr.count() == 8);
		CHECK(tree.is_rb_tree());
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	}

	//deleting root
	SECTION("Case 13")
	{
		uRB_TREE array;
		array.insert(3);
		CHECK(array.count() == 1);
		CHECK(array.is_rb_tree());

		//deleting non-existed element
		array.delete_rb_tree(0);
		CHECK(array.count() == 1);
		CHECK(array.is_rb_tree());

		//deleting root wuth no children
		array.delete_rb_tree(3);
		CHECK(array.count() == 0);
		CHECK(array.is_rb_tree());

	}

	//deleting root with one child
	SECTION("Case 14")
	{
		uRB_TREE array;

		//deleting root with left children
		array.insert(3);
		array.insert(1);
		CHECK(array.count() == 2);
		array.delete_rb_tree(3);
		CHECK(array.count() == 1);
		CHECK(array.root()->data == 1);
		//array.insert(3);
		//CHECK(array.count() == 2);
		CHECK(array.is_rb_tree());

		uRB_TREE array1;
		//deleting root with right children

		array1.insert(3);
		array1.insert(4);
		CHECK(array1.count() == 2);
		array1.delete_rb_tree(3);
		CHECK(array1.count() == 1);
		CHECK(array1.root()->data == 4);
		CHECK(array.is_rb_tree());
	}

	//deleting root with two children
	SECTION("Case 15")
	{
		uRB_TREE array;

		array.insert(3);
		array.insert(1);
		array.insert(6);
		CHECK(array.count() == 3);
		array.delete_rb_tree(3);
		CHECK(array.count() == 2);
		CHECK(array.root()->data == 6);
		array.insert(3);
		CHECK(array.count() == 3);
		CHECK(array.is_rb_tree());
	}

	//deleting node with two children ex 1
	//Case 3 & 4
	SECTION("Case 16")
	{
		uRB_TREE array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == 8);
		//array.inorder_traverse(print);
		array.delete_rb_tree(15);
		CHECK(array.count() == 7);
		//array.inorder_traverse(print);
		array.inorder_traverse(insert);
		CHECK(arr.count() == 7);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//deleting node with two children ex 2
	//Case 3 & 4
	SECTION("Case 17")
	{
		uRB_TREE array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.delete_rb_tree(15);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//deleting node with one children ex 1
	SECTION("Case 18")
	{
		uRB_TREE array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.delete_rb_tree(12);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//deleting node with one children ex 2
	SECTION("Case 19")
	{
		uRB_TREE array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.delete_rb_tree(20);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array.is_rb_tree());
	}

	//Case 2 ex 1
	SECTION("Case 20")
	{
		uRB_TREE array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.delete_rb_tree(20);
		CHECK(array.count() == --count);
		CHECK(array.is_rb_tree());
		array.delete_rb_tree(14);
		CHECK(array.count() == --count);
		CHECK(array.is_rb_tree());
		array.delete_rb_tree(12);
		CHECK(array.count() == --count);
		CHECK(array.is_rb_tree());
		//array.inorder_traverse(print);
	}


	SECTION("Case 21")
	{
		uRB_TREE array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//Case 2 ex 2
		array.delete_rb_tree(20);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
		array.delete_rb_tree(14);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
		//Case 2
		array.delete_rb_tree(19);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
		CHECK(array.is_rb_tree());
		array.delete_rb_tree(15);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
		//Case 4
		array.delete_rb_tree(12);
		CHECK(array.is_rb_tree());
		CHECK(array.count() == --count);
	}


}
