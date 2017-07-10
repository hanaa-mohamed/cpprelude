#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/allocator.h"
#include "cpprelude/iterator.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/queue_array.h"

namespace cpprelude {

	//Lean left red black tree
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
			if (_root == nullptr)
				new_node->color = COLOR::BLACK;
			_root = _insert(_root, new_node);
			++_count;
			return rb_iterator(new_node);
		}

		rb_iterator
		insert(T&& key)
		{
			RB_Node* new_node = _create_node(tmp::move(key));
			if (_root == nullptr)
				new_node->color = COLOR::BLACK;
			_root = _insert(_root, new_node);
			++_count;
			return rb_iterator(new_node);
		}

		rb_iterator
		lookup(const T& key)
		{
			RB_Node* result = _root;
			if (result != nullptr)
				result = _lookup(key, _root);
			return rb_iterator(result);
		}

		const_rb_iterator
		lookup(const T& key) const
		{
			RB_Node* result = _root;
			if (result != nullptr)
				result = _lookup(key, _root);
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
			return (-1 != _is_llrb(_root, false));
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
		RB_Node*
		_insert(RB_Node* parent, RB_Node*& node)
		{
			if (parent == nullptr)
				return node;
			
			if (_less_than(node->data, parent->data)) //left branch
			{
				if(parent->left == nullptr)
					node->parent = parent;
				parent->left = _insert(parent->left, node);
				
			}
			else if (_less_than(parent->data, node->data)) // right branch
			{
				if (parent->right == nullptr)
					node->parent = parent;
				parent->right = _insert(parent->right, node);
				
			}
			else //node points to the old value in the tree
				node = parent;

			//fix the tree after insertion
			auto is_red = [](RB_Node* p) {return (p != nullptr && p->is_red()) ? true : false; };

			if (is_red(parent->right) && !is_red(parent->left))
				parent = _rotate_left(parent);
			if (is_red(parent->left) && is_red(parent->left->left))
				parent = _rotate_right(parent);
			if (is_red(parent->left) && is_red(parent->right))
				_flip_colors(parent);

			return parent;
		}

		RB_Node*
		_rotate_left(RB_Node* node)
		{
			RB_Node* temp = node->right;
			node->right = temp->left;
			if (temp->left != nullptr)
				temp->left->parent = node;
			temp->parent = node->parent;
			temp->left = node;
			node->parent = temp;
			temp->color = temp->left->color;
			temp->left->color = COLOR::RED;
			return temp;
		}

		RB_Node*
		_rotate_right(RB_Node* node)
		{
			RB_Node* temp = node->left;
			node->left = temp->right;
			if (temp->right != nullptr)
				temp->right->parent = node;
			temp->parent = node->parent;
			temp->right = node;
			node->parent = temp;
			temp->color = temp->right->color;
			temp->right->color = COLOR::RED;
			return temp;
		}

		void
		_flip_colors(RB_Node* node)
		{
			node->color = COLOR::RED;
			node->left->color = COLOR::BLACK;
			node->right->color = COLOR::BLACK;
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
		_is_llrb(RB_Node* it, bool right)
		{
			if (it == nullptr) return 0;

			isize left_count = _is_llrb(it->left, false);
			isize right_count = _is_llrb(it->right, true);

			if (left_count == -1 || right_count == -1 || right_count != left_count || (right&& it->color == COLOR::RED))
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
		_lookup(const T& k, RB_Node* it) 
		{
			if (_less_than(k , it->data))
			{
				if (it->left != nullptr)
					return _lookup(k, it->left);

				else
					return it->left;
			}
			else if (_less_than(it->data, k ))
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
}