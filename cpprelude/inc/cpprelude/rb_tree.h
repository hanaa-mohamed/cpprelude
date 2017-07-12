#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/allocator.h"
#include "cpprelude/iterator.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/queue_array.h"

namespace cpprelude {

	template<typename T, typename Comparator = tmp::default_less_than<T>, typename AllocatorT = global_allocator>
	struct rb_tree
	{
		using RB_Node = details::rb_node<T>;
		using rb_iterator = rb_tree_iterator<T>;
		using const_rb_iterator = const rb_tree_iterator<T>;
		using COLOR = details::color_type;
		using data_type = T;

		RB_Node* _root;
		usize _count;
		AllocatorT _allocator;
		Comparator _less_than;

		rb_tree(Comparator compare_function = Comparator(), const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator), _root(nullptr) , _less_than(compare_function)
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

		rb_tree(const rb_tree<T>& other)
			:_root(nullptr), _count(0), _allocator(other._allocator), _less_than(other._less_than)
		{
			_copy_content(other);
		}

		rb_tree(const rb_tree<T>& other, Comparator compare_function)
			:_root(nullptr), _count(0), _allocator(other._allocator), _less_than(compare_function)
		{
			_copy_content(other);
		}

		rb_tree(const rb_tree<T>& other, const AllocatorT& allocator)
			:_root(nullptr), _count(0), _allocator(allocator), _less_than(other._less_than)
		{
			_copy_content(other);
		}

		rb_tree(const rb_tree<T>& other, const AllocatorT& allocator, Comparator compare_function)
			:_root(nullptr), _count(0), _allocator(allocator), _less_than(compare_function)
		{
			_copy_content(other);
		}

		rb_tree(rb_tree<T>&& other)
			:_root(nullptr), _count(0), _allocator(tmp::move(other._allocator)), _less_than(other._less_than)
		{
			_copy_content(tmp::move(other));
		}

		rb_tree(rb_tree<T>&& other, Comparator compare_function)
			:_root(nullptr), _count(0), _allocator(tmp::move(other._allocator)), _less_than(compare_function)
		{
			_copy_content(tmp::move(other));
		}

		rb_tree(rb_tree<T>&& other, const AllocatorT& allocator)
			:_root(nullptr), _count(0), _allocator(allocator), _less_than(other._less_than)
		{
			_copy_content(tmp::move(other));
		}

		rb_tree(rb_tree<T>&& other, const AllocatorT& allocator, Comparator compare_function)
			:_root(nullptr), _count(0), _allocator(allocator), _less_than(compare_function)
		{
			_copy_content(tmp::move(other));
		}

		~rb_tree()
		{
			clear();
		}

		rb_tree<T>&
		operator=(const rb_tree<T>& other)
		{
			clear();
			_allocator = other._allocator;
			_less_than = other._less_than;
			_copy_content(other);
			return *this;
		}

		rb_tree<T>&
		operator=(rb_tree<T>&& other)
		{
			clear();
			_allocator = tmp::move(other._allocator);
			_less_than = other._less_than;
			_copy_content(tmp::move(other));
			return *this;
		}

		rb_iterator
		operator[](const T& key)
		{	//this function either returns the existed node or the new inserted node
			return insert(key);
		}

		rb_iterator
		operator[](T&& key)
		{	//this function either returns the existed node or the new inserted node
			return insert(key);
		}

		void
		clear()
		{
			_reset(_root);
			_root = nullptr;
			_count = 0;
			_allocator = AllocatorT();
			_less_than = Comparator();
		}

		//modify if exisits, and insert if not.
		rb_iterator
		insert(const T& key)
		{
			RB_Node* new_node = _create_node(key);
			_rb_insert(new_node);
			++_count;
			return rb_iterator(new_node);
		}

		rb_iterator
		insert(T&& key)
		{
			RB_Node* new_node = _create_node(tmp::move(key));
			_rb_insert(new_node);
			++_count;
			return rb_iterator(new_node);
		}

		void
		delete_rb_tree(const T&k)
		{
			rb_iterator node_to_delete = lookup(k);
			delete_rb_tree(node_to_delete);
		}

		void
		delete_rb_tree(T&&k)
		{
			rb_iterator node_to_delete = lookup(k);
			delete_rb_tree(node_to_delete);
		}

		void
		delete_rb_tree(rb_iterator node_to_delete)
		{
			if (node_to_delete == nullptr) return;

			RB_Node *x = nullptr, *x_parent = nullptr;
			rb_iterator y;

			if (node_to_delete->left == nullptr || node_to_delete->right == nullptr)
				y = node_to_delete;
			else
			{
				y = ++node_to_delete;
				--node_to_delete;
			}

			if (y->left != nullptr)
				x = y->left;
			else
				x = y->right;

			if (x != nullptr)
				x->parent = y->parent;

			x_parent = y->parent;

			if (y->parent == nullptr)
			{
				_root = x;
				if (x != nullptr)
					x->color = COLOR::BLACK;
			}
			else if (y == y->parent->left)
				y->parent->left = x;
			else
				y->parent->right = x;

			if (y != node_to_delete)
				node_to_delete->data = y->data;

			if (y->color == COLOR::BLACK)
				_rb_delete_fixup(x, x_parent);

			_free_mem(y._node);
		}

		rb_iterator
		lookup(const T& key)
		{
			RB_Node* result = _root;
			RB_Node* key_it = _create_node(key);
			if (result != nullptr)
				result = _lookup(key_it, _root);
			return rb_iterator(result);
		}

		const_rb_iterator
		lookup(const T& key) const
		{
			RB_Node* result = _root;
			RB_Node* key_it = _create_node(key);
			if (result != nullptr)
				result = _lookup(key_it, _root);
			return const_rb_iterator(result);
		}

		rb_iterator
		lookup(T&& key)
		{
			RB_Node* result = _root;
			RB_Node* key_it = _create_node(key);
			if (result != nullptr)
				result = _lookup(key_it, _root);
			return rb_iterator(result);
		}

		const_rb_iterator
		lookup(T&& key) const
		{
			RB_Node* result = _root;
			RB_Node* key_it = _create_node(key);
			if (result != nullptr)
				result = _lookup(key_it, _root);
			return const_rb_iterator(result);
		}

		template<typename function_type>
		void
		inorder_traverse(function_type FT)
		{
			_inorder_traverse(FT, _root);
		}

		template<typename function_type>
		void
		postorder_traverse(function_type FT)
		{
			_postorder_traverse(FT, _root);
		}

		template<typename function_type>
		void
		preorder_traverse(function_type FT)
		{
			_preorder_traverse(FT, _root);
		}

		rb_iterator
		root() const
		{
			return rb_iterator(_root);
		}

		bool
		is_rb_tree()
		{
			return (-1 != _is_rb(_root, false));
		}

		void
		swap(rb_tree& other)
		{
			tmp::swap(_root, other._root);
			tmp::swap(_count, other._count);
			tmp::swap(_allocator, other._allocator);
			tmp::swap(_less_than, other._less_than);
		}

		void
		swap(rb_tree&& other)
		{
			tmp::swap(_root, other._root);
			tmp::swap(_count, tmp::move(other._count));
			tmp::swap(_allocator, tmp::move(other._allocator));
			tmp::swap(_less_than, other._less_than);
		}

		rb_iterator
		get_min() const
		{
			auto m = _root;
			if (m == nullptr) return rb_iterator();

			while (m->left != nullptr)
			{
				m = m->left;
			}
			return m;
		}

		rb_iterator
		get_max() const
		{
			auto m = _root;
			if (m == nullptr) return rb_iterator();

			while (m->right != nullptr)
			{
				m = m->right;
			}
			return m;
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
			new (temp) RB_Node(k);
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
			if (it == nullptr)
				return;

			if (it->left != nullptr)
				_reset(it->left);

			if (it->right != nullptr)
				_reset(it->right);

			_free_mem(it);
		}

		isize
		_is_rb(RB_Node* it, bool right)
		{
			if (it == nullptr) return 0;

			isize left_count = _is_rb(it->left, false);
			isize right_count = _is_rb(it->right, true);

			if (left_count == -1 || right_count == -1 || right_count != left_count)
				return -1;
			else
				return (it->color == COLOR::RED) ? left_count : left_count + 1;
		}

		void
		_copy_content(const rb_tree<T>& other)
		{
			queue_array<rb_iterator> queue;
			queue.enqueue(other.root());

			while (!queue.empty())
			{
				auto it = queue.front();
				queue.dequeue();
				if (it == nullptr) continue;

				insert(it->data);
				queue.enqueue(it->left);
				queue.enqueue(it->right);
			}
		}

		void
		_copy_content(rb_tree<T>&& other)
		{
			queue_array<rb_iterator> queue;
			queue.enqueue(tmp::move(other.root()));

			while (!queue.empty())
			{
				auto it = queue.front();
				queue.dequeue();
				if (it == nullptr) continue;

				insert(tmp::move(it->data));
				queue.enqueue(tmp::move(it->left));
				queue.enqueue(tmp::move(it->right));
			}
		}	

		template<typename function_type>
		void
		_inorder_traverse(function_type fT, rb_iterator it)
		{
			if (it->left != nullptr)
				_inorder_traverse(fT, it->left);

			fT(it);

			if (it->right != nullptr)
				_inorder_traverse(fT, it->right);
		}

		template<typename function_type>
		void
		_postorder_traverse(function_type fT, rb_iterator it)
		{
			if (it->left != nullptr)
				_postorder_traverse(fT, it->left);

			if (it->right != nullptr)
				_postorder_traverse(fT, it->right);

			fT(it);
		}

		template<typename function_type>
		void
		_preorder_traverse(function_type fT, rb_iterator it)
		{
			fT(it);

			if (it->left != nullptr)
				_preorder_traverse(fT, it->left);

			if (it->right != nullptr)
				_preorder_traverse(fT, it->right);
		}

		RB_Node*
		_lookup(RB_Node* k, RB_Node* it) 
		{
			if (_less_than(k->data , it->data))
			{
				if (it->left != nullptr)
					return _lookup(k, it->left);

				else
					return it->left;
			}
			else if (_less_than(it->data, k->data ))
			{
				if (it->right != nullptr)
					return _lookup(k, it->right);
				else
					return it->right;
			}
			else
				return it;
		}
	};

	template<typename key, typename value, typename Comparator = tmp::default_less_than<details::pair_node<key, value>>, typename AllocatorT = global_allocator>
	using ordered_map = rb_tree<details::pair_node<key, value>, Comparator, AllocatorT>;
}