#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/allocator.h"
#include "cpprelude/iterator.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/queue_array.h"

#undef min
#undef max

namespace cpprelude
{

	//red black tree implmenetion follows Introduction to Algorithms, Second Edition,” by Thomas H. Cormen, Charles E, Chapter 14
	template<typename KeyType, typename ValueType,
		typename Comparator = tmp::default_less_than<details::pair_node<KeyType, ValueType>>,
		typename AllocatorT = global_allocator>
	struct rb_tree
	{
		using T = details::pair_node<KeyType, ValueType>;
		using key_type = KeyType;
		using value_type = ValueType;
		using RB_Node = details::rb_node<T>;
		using iterator = rb_tree_iterator<T>;
		using const_iterator = const_rb_tree_iterator<T>;
		using COLOR = typename RB_Node::color_type;
		using data_type = T;

		RB_Node* _root;
		usize _count;
		AllocatorT _allocator;
		Comparator _less_than;

		rb_tree(Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT())
			:_root(nullptr), _count(0), _allocator(allocator), _less_than(compare_function)
		{}

		rb_tree(std::initializer_list<T> list, Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT())
			:_count(list.size()), _allocator(allocator), _less_than(compare_function)
		{
			auto it = list.begin();
			for (usize i = 0; i < _count; ++i)
			{
				insert(*it);
				it = std::next(it);
			}
		}

		rb_tree(const rb_tree& other)
			:_root(nullptr), _count(0), _allocator(other._allocator), _less_than(other._less_than)
		{
			_copy_content(other);
		}

		rb_tree(const rb_tree& other, Comparator compare_function)
			:_root(nullptr), _count(0), _allocator(other._allocator), _less_than(compare_function)
		{
			_copy_content(other);
		}

		rb_tree(const rb_tree& other, const AllocatorT& allocator)
			:_root(nullptr), _count(0), _allocator(allocator), _less_than(other._less_than)
		{
			_copy_content(other);
		}

		rb_tree(const rb_tree& other, const AllocatorT& allocator, Comparator compare_function)
			:_root(nullptr), _count(0), _allocator(allocator), _less_than(compare_function)
		{
			_copy_content(other);
		}

		rb_tree(rb_tree&& other)
			:_root(other._root), _count(other._count), _allocator(tmp::move(other._allocator)), _less_than(tmp::move(other._less_than))
		{
			other._root = nullptr;
			other._count = 0;
		}

		rb_tree(rb_tree&& other, Comparator compare_function)
			:_root(other._root), _count(other._count), _allocator(tmp::move(other._allocator)), _less_than(compare_function)
		{
			other._root = nullptr;
			other._count = 0;
		}

		rb_tree(rb_tree&& other, const AllocatorT& allocator)
			:_root(other._root), _count(other._count),
			_allocator(allocator),
			_less_than(tmp::move(other._less_than))
		{
			other._root = nullptr;
			other._count = 0;
		}

		rb_tree(rb_tree&& other, const AllocatorT& allocator, Comparator compare_function)
			:_root(other._root), _count(other._count), _allocator(allocator)
			, _less_than(compare_function)
		{
			other._root = nullptr;
			other._count = 0;
		}

		~rb_tree()
		{
			clear();
		}

		rb_tree&
		operator=(const rb_tree& other)
		{
			clear();
			_allocator = other._allocator;
			_less_than = other._less_than;
			_copy_content(other);
			return *this;
		}

		rb_tree&
		operator=(rb_tree&& other)
		{
			_reset(_root);
			_allocator = tmp::move(other._allocator);
			_less_than = tmp::move(other._less_than);
			_count = other._count;
			other._count = 0;
			_root = other._root;
			other._root = nullptr;
			return *this;
		}

		value_type&
		operator[](const key_type& key)
		{
			T node(key);
			return insert(node)->value;
		}

		const value_type&
		operator[](const key_type& key) const
		{
			T node(key);
			return insert(node)->value;
		}

		void
		clear()
		{
			_reset(_root);
			_root = nullptr;
			_count = 0;
		}

		//modify if exisits, and insert if not.
		iterator
		insert(const key_type& k)
		{
			RB_Node* new_node = _create_node(T(k));
			_rb_insert(new_node);
			++_count;
			return iterator(new_node);
		}

		iterator
		insert(const key_type& key, const value_type& value)
		{
			return insert(T(key, value));
		}

		iterator
		insert(key_type&& key, const value_type& value)
		{
			return insert(T(tmp::move(key), value));
		}

		iterator
		insert(const key_type& key, value_type&& value)
		{
			return insert(T(key, tmp::move(value)));
		}

		iterator
		insert(key_type&& key, value_type&& value)
		{
			return insert(T(tmp::move(key), tmp::move(value)));
		}

		iterator
		insert(const T& p)
		{
			RB_Node* new_node = _create_node(p);
			_rb_insert(new_node);
			++_count;
			return iterator(new_node);
		}

		iterator
		insert(key_type&& k)
		{
			RB_Node* new_node = _create_node(T(tmp::move(k)));
			_rb_insert(new_node);
			++_count;
			return iterator(new_node);
		}

		iterator
		insert(T&& p)
		{
			RB_Node* new_node = _create_node(tmp::move(p));
			_rb_insert(new_node);
			++_count;
			return iterator(new_node);
		}

		void
		delete_rb_tree(const key_type& k)
		{
			iterator node_to_delete = lookup(k);
			delete_rb_tree(node_to_delete);
		}

		void
		delete_rb_tree(const T&p)
		{
			iterator node_to_delete = lookup(p.key);
			delete_rb_tree(node_to_delete);
		}

		void
		delete_rb_tree(key_type&& k)
		{
			iterator node_to_delete = lookup(tmp::move(k));
			delete_rb_tree(node_to_delete);
		}

		void
		delete_rb_tree(T&& p)
		{
			iterator node_to_delete = lookup(tmp::move(p.key));
			delete_rb_tree(node_to_delete);
		}

		void
		delete_rb_tree(iterator node_to_delete)
		{
			if (node_to_delete == end()) return;

			RB_Node *x = nullptr, *x_parent = nullptr;
			iterator y;

			if (node_to_delete._node->left == nullptr || node_to_delete._node->right == nullptr)
				y = node_to_delete;
			else
			{
				y = node_to_delete;
				++y;
			}

			if (y._node->left != nullptr)
				x = y._node->left;
			else
				x = y._node->right;

			if (x != nullptr)
				x->parent = y._node->parent;

			x_parent = y._node->parent;

			if (y._node->parent == nullptr)
			{
				_root = x;
				if (x != nullptr)
					x->color = COLOR::BLACK;
			}
			else if (y._node == y._node->parent->left)
				y._node->parent->left = x;
			else
				y._node->parent->right = x;

			if (y != node_to_delete)
				*node_to_delete = *y;

			if (y._node->color == COLOR::BLACK)
				_rb_delete_fixup(x, x_parent);

			_free_mem(y._node);
		}

		iterator
		lookup(const key_type& key)
		{
			RB_Node* result = _root;
			if (result != nullptr)
				result = _lookup(key, _root);
			return iterator(result);
		}

		const_iterator
		lookup(const key_type& key) const
		{
			RB_Node* result = _root;
			if (result != nullptr)
				result = _lookup(key, _root);
			return const_iterator(result);
		}

		iterator
		lookup(key_type&& key)
		{
			RB_Node* result = _root;
			if (result != nullptr)
				result = _lookup(key, _root);
			return iterator(result);
		}

		const_iterator
		lookup(key_type&& key) const
		{
			RB_Node* result = _root;
			if (result != nullptr)
				result = _lookup(key, _root);
			return const_iterator(result);
		}

		template<typename function_type>
		void
		inorder_traverse(function_type&& FT)
		{
			_inorder_traverse(tmp::forward<function_type>(FT), _root);
		}

		template<typename function_type>
		void
		postorder_traverse(function_type&& FT)
		{
			_postorder_traverse(tmp::forward<function_type>(FT), _root);
		}

		template<typename function_type>
		void
		preorder_traverse(function_type&& FT)
		{
			_preorder_traverse(tmp::forward<function_type>(FT), _root);
		}

		template<typename function_type>
		void
		inorder_traverse(function_type&& FT) const
		{
			_inorder_traverse(tmp::forward<function_type>(FT), _root);
		}

		template<typename function_type>
		void
		postorder_traverse(function_type&& FT) const
		{
			_postorder_traverse(tmp::forward<function_type>(FT), _root);
		}

		template<typename function_type>
		void
		preorder_traverse(function_type&& FT) const
		{
			_preorder_traverse(tmp::forward<function_type>(FT), _root);
		}

		iterator
		root() const
		{
			return iterator(_root);
		}

		bool
		is_rb_tree()
		{
			return (-1 != _is_rb(_root));
		}

		void
		swap(rb_tree& other)
		{
			tmp::swap(_root, other._root);
			tmp::swap(_count, other._count);
			tmp::swap(_allocator, other._allocator);
			tmp::swap(_less_than, other._less_than);
		}

		iterator
		min() const
		{
			auto m = _root;
			if (m == nullptr) return iterator();

			while (m->left != nullptr)
			{
				m = m->left;
			}
			return m;
		}

		iterator
		max() const
		{
			auto m = _root;
			if (m == nullptr) return iterator();

			while (m->right != nullptr)
			{
				m = m->right;
			}
			return m;
		}

		iterator
		begin()
		{
			return min();
		}

		const_iterator
		begin() const
		{
			return min();
		}

		const_iterator
		cbegin() const
		{
			return min();
		}

		iterator
		end()
		{
			return nullptr;
		}

		const_iterator
		end() const
		{
			return nullptr;
		}

		const_iterator
		cend() const
		{
			return nullptr;
		}

		usize
		count() const
		{
			return _count;
		}

		usize
		empty() const
		{
			return _count == 0;
		}

		//inserts new node else if exists return the old value
		void
		_rb_insert(RB_Node*& z)
		{
			RB_Node* y = nullptr;
			RB_Node* x = _root;

			while (x != nullptr)
			{
				y = x;
				if (_less_than(z->data, x->data))
					x = x->left;
				else if (_less_than(x->data, z->data))
					x = x->right;
				else
				{
					_free_mem(z);
					z = x;
					return;
				}
			}
			z->parent = y;
			if (y == nullptr)
				_root = z;
			else
			{
				if (_less_than(z->data, y->data))
					y->left = z;
				else
					y->right = z;
			}
			z->left = nullptr;
			z->right = nullptr;
			z->color = COLOR::RED;
			_insert_fixup(z);
		}

		void
		_insert_fixup(RB_Node* z)
		{
			auto is_red = [](RB_Node* p) { return p != nullptr && p->is_red(); };

			while (is_red(z->parent))
			{
				if (z->parent == z->parent->parent->left)
				{
					RB_Node* y = z->parent->parent->right;
					if (is_red(y))
					{ //Casse 1
						z->parent->color = COLOR::BLACK;
						y->color = COLOR::BLACK;
						z->parent->parent->color = COLOR::RED;
						z = z->parent->parent;
					}
					else
					{
						if (z == z->parent->right)
						{ //case 2
							z = z->parent;
							_left_rotation(z);
						}
						//case 3
						z->parent->color = COLOR::BLACK;
						z->parent->parent->color = COLOR::RED;
						_right_rotation(z->parent->parent);
					}
				}
				else
				{// right instead of left

					RB_Node* y = z->parent->parent->left;
					if (is_red(y))
					{ //Casse 1
						z->parent->color = COLOR::BLACK;
						y->color = COLOR::BLACK;
						z->parent->parent->color = COLOR::RED;
						z = z->parent->parent;
					}
					else
					{
						if (z == z->parent->left)
						{ //case 2
							z = z->parent;
							_right_rotation(z);
						}
						//case 3
						z->parent->color = COLOR::BLACK;
						z->parent->parent->color = COLOR::RED;
						_left_rotation(z->parent->parent);
					}
				}
			}
			_root->color = COLOR::BLACK;
		}

		void
		_rb_delete_fixup(RB_Node* node, RB_Node* node_parent)
		{
			RB_Node* w = nullptr;
			bool is_node_black = true;
			if (node != nullptr)
				is_node_black = node->color == COLOR::BLACK;

			while (node != _root && is_node_black)
			{
				if (node == node_parent->left)
				{
					w = node_parent->right;
					if (w->color == COLOR::RED)
					{	//Case 1
						w->color = COLOR::BLACK;
						node_parent->color = COLOR::RED;
						_left_rotation(node_parent);
						w = node_parent->right;
					}
					bool is_w_left_black = true;
					bool is_w_right_black = true;
					if (w->left != nullptr)
						is_w_left_black = w->left->color == COLOR::BLACK;
					if (w->right != nullptr)
						is_w_right_black = w->right->color == COLOR::BLACK;

					if (is_w_left_black	&& is_w_right_black)
					{ //Case 2: black sibling with black childern
						w->color = COLOR::RED;
						node = node_parent;
						node_parent = node->parent;
						is_node_black = node->color == COLOR::BLACK;
					}
					else
					{
						if (is_w_right_black)
						{	//Case 3
							w->left->color = COLOR::BLACK;
							w->color = COLOR::RED;
							_right_rotation(w);
							w = node_parent->right;
						}
						//Case 4
						w->color = node_parent->color;
						node_parent->color = COLOR::BLACK;
						if (w->right != nullptr)
							w->right->color = COLOR::BLACK;
						_left_rotation(node_parent);
						node = _root;
						node_parent = nullptr;
					}

				}
				else
				{
					w = node_parent->left;
					if (w->color == COLOR::RED)
					{	//Case 1
						w->color = COLOR::BLACK;
						node_parent->color = COLOR::RED;
						_right_rotation(node_parent);
						w = node_parent->left;
					}

					bool is_w_left_black = true;
					bool is_w_right_black = true;
					if (w->left != nullptr)
						is_w_left_black = w->left->color == COLOR::BLACK;
					if (w->right != nullptr)
						is_w_right_black = w->right->color == COLOR::BLACK;

					if (is_w_left_black	&& is_w_right_black)
					{	//Case 2
						w->color = COLOR::RED;
						node = node_parent;
						node_parent = node->parent;
						is_node_black = node->color == COLOR::BLACK;
					}
					else
					{
						if (is_w_left_black)
						{	//Case 3
							w->right->color = COLOR::BLACK;
							w->color = COLOR::RED;
							_left_rotation(w);
							w = node_parent->left;
						}
						//Case 4
						w->color = node_parent->color;
						node_parent->color = COLOR::BLACK;
						if (w->left != nullptr)// check this
							w->left->color = COLOR::BLACK;
						_right_rotation(node_parent);
						node = _root;
						node_parent = nullptr;
					}
				}
			}
			if (node != nullptr)
				node->color = COLOR::BLACK;
		}

		void
		_left_rotation(RB_Node* x)
		{
			RB_Node* y = x->right;
			x->right = y->left;
			if (y->left != nullptr)
				y->left->parent = x;
			y->parent = x->parent;
			if (x->parent == nullptr)
				_root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			y->left = x;
			x->parent = y;
		}

		void
		_right_rotation(RB_Node* x)
		{
			RB_Node* y = x->left;
			x->left = y->right;
			if (y->right != nullptr)
				y->right->parent = x;
			y->parent = x->parent;
			if (x->parent == nullptr)
				_root = y;
			else if (x == x->parent->right)
				x->parent->right = y;
			else
				x->parent->left = y;
			y->right = x;
			x->parent = y;
		}

		RB_Node*
		_create_node(const T& k)
		{
			auto temp = _allocator.template alloc<RB_Node>();
			new (temp) RB_Node(k);
			return temp;
		}

		RB_Node*
		_create_node(T&& k)
		{
			auto temp = _allocator.template alloc<RB_Node>();
			new (temp) RB_Node(tmp::move(k));
			return temp;
		}

		void
		_free_mem(RB_Node* it)
		{
			if (it == nullptr) return;

			it->data.~T();
			_allocator.free(make_slice(it));
			--_count;
		}

		void
		_reset(RB_Node* it)
		{
			auto this_ptr = this;
			auto func = [&this_ptr](iterator it)
			{
				this_ptr->_free_mem(it._node);
			};
			/*
			if (it == nullptr)
				return;

			if (it->left != nullptr)
				_reset(it->left);

			if (it->right != nullptr)
				_reset(it->right);

			_free_mem(it);
			*/
		}

		isize
		_is_rb(RB_Node* it)
		{
			if (it == nullptr) return 0;

			isize left_count = _is_rb(it->left);
			isize right_count = _is_rb(it->right);

			if (left_count == -1 || right_count == -1 || right_count != left_count)
				return -1;
			else
				return (it->color == COLOR::RED) ? left_count : left_count + 1;
		}

		void
		_copy_content(const rb_tree& other)
		{
			auto this_ptr = this;
			auto func = [&this_ptr](const_iterator it)
			{
				this_ptr->insert(*it);
			};
			other.preorder_traverse(func);
			// queue_array<rb_iterator> queue;
			// queue.enqueue(other.root());
			//
			// while (!queue.empty())
			// {
			// 	auto it = queue.front();
			// 	queue.dequeue();
			// 	if (it == nullptr) continue;
			//
			// 	insert(it->data);
			// 	queue.enqueue(it->left);
			// 	queue.enqueue(it->right);
			// }
		}

		template<typename function_type>
		void
		_inorder_traverse(function_type&& fT, iterator it)
		{
			if (it._node->left != nullptr)
				_inorder_traverse(tmp::forward<function_type>(fT), it._node->left);

			fT(it);

			if (it._node->right != nullptr)
				_inorder_traverse(tmp::forward<function_type>(fT), it._node->right);
		}

		template<typename function_type>
		void
		_postorder_traverse(function_type&& fT, iterator it)
		{
			if (it._node->left != nullptr)
				_postorder_traverse(tmp::forward<function_type>(fT), it._node->left);

			if (it._node->right != nullptr)
				_postorder_traverse(tmp::forward<function_type>(fT), it._node->right);

			fT(it);
		}

		template<typename function_type>
		void
		_preorder_traverse(function_type&& fT, iterator it)
		{
			fT(it);

			if (it._node->left != nullptr)
				_preorder_traverse(tmp::forward<function_type>(fT), it._node->left);

			if (it._node->right != nullptr)
				_preorder_traverse(tmp::forward<function_type>(fT), it._node->right);
		}

		template<typename function_type>
		void
		_inorder_traverse(function_type&& fT, const_iterator it) const
		{
			if (it._node->left != nullptr)
				_inorder_traverse(tmp::forward<function_type>(fT), it._node->left);

			fT(it);

			if (it._node->right != nullptr)
				_inorder_traverse(tmp::forward<function_type>(fT), it._node->right);
		}

		template<typename function_type>
		void
		_postorder_traverse(function_type&& fT, const_iterator it) const
		{
			if (it._node->left != nullptr)
				_postorder_traverse(tmp::forward<function_type>(fT), it._node->left);

			if (it._node->right != nullptr)
				_postorder_traverse(tmp::forward<function_type>(fT), it._node->right);

			fT(it);
		}

		template<typename function_type>
		void
		_preorder_traverse(function_type&& fT, const_iterator it) const
		{
			fT(it);

			if (it._node->left != nullptr)
				_preorder_traverse(tmp::forward<function_type>(fT), it._node->left);

			if (it._node->right != nullptr)
				_preorder_traverse(tmp::forward<function_type>(fT), it._node->right);
		}

		RB_Node*
		_lookup(const key_type& key, RB_Node* it) const
		{
			if (_less_than(key , it->data.key))
			{
				if (it->left != nullptr)
					return _lookup(key, it->left);

				else
					return it->left;
			}
			else if (_less_than(it->data.key, key))
			{
				if (it->right != nullptr)
					return _lookup(key, it->right);
				else
					return it->right;
			}
			else
				return it;
		}

		RB_Node*
		_lookup(key_type&& key, RB_Node* it) const
		{
			return _lookup(key, it);
		}
	};

	template<typename key_type, typename value_type,
		typename Comparator = tmp::default_less_than<details::pair_node<key_type, value_type>>,
		typename AllocatorT = global_allocator>
		using tree_map = rb_tree<key_type, value_type, Comparator, AllocatorT>;
}
