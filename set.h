#ifndef SET_H_
#define SET_H_


#include <algorithm>
#include <iostream>

template <typename T>
class set {
public:

	set() : root{ nullptr } { }

	set(const set& rhs) : root{ nullptr }
	{
		root = clone(rhs.root);
	}

	set(set&& rhs) : root{ rhs.root }
	{
		rhs.root = nullptr;
	}

	~set()
	{
		this->clear();
	}

	set& operator=(const set& rhs)
	{
		auto copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	set& operator=(set&& rhs)
	{
		std::swap(root, rhs.root);
		return *this;
	}

	bool contains(const T& value) const
	{
		return this->contains(value, this->root);
	}

	bool empty() const
	{
		return this->root == nullptr;
	}

	void print(std::ostream& out = std::cout) const
	{
		if (this->empty()) {
			out << "Empty tree" << std::endl;
		}
		else {
			this->print(root, out);
		}
	}

	void clear()
	{
		this->clear(this->root);
	}

	void insert(const T& value)
	{
		this->insert(value, this->root, root);
	}

	void insert(T&& value)
	{
		this->insert(std::move(value), this->root, root);
	}

	void remove(const T& value)
	{
		this->remove(value, this->root);
	}


private:
	struct node
	{
		T element;
		node* left;
		node* right;
		node* parent;

		node(const T& the_element, node* left_node, node* right_node, node* parent)
			: element{ the_element }, left{ left_node }, right{ right_node }, parent{ parent } { }

		node(T&& the_element, node* left_node, node* right_node, node* parent)
			: element{ std::move(the_element) }, left{ left_node }, right{ right_node }, parent{ parent } { }
	};

	node* root;

	class const_iterator {
	public:

		// Public constructor for const_iterator.
		// Notice we use nullptr not NULL
		const_iterator() : current{ nullptr }
		{ }

		// Return the T stored at the current position.
		// For const_iterator, this is an accessor with a
		// const reference return type.
		const T& operator*() const
		{
			// de-reference operator we use to get the value
			// of the node to which we are pointing.
			return this->retrieve();
		}

		const_iterator& operator++()
		{
			// remember we have value++ and ++value
			// We have two method for pre or post increment.
			this->current = this->current->next;
			return *this;
		}

		const_iterator operator++(int)
		{
			auto old = *this;
			++(*this);
			return old;
		}

		const_iterator& operator--()
		{
			this->current = this->current->previous;
			return *this;
		}

		const_iterator operator--(int)
		{
			auto old = *this;
			--(*this);
			return old;
		}

		bool operator==(const const_iterator& rhs) const
		{
			return this->current == rhs.current;
		}

		bool operator!=(const const_iterator& rhs) const
		{
			return !(*this == rhs);
		}

	protected:
		node* current;

		// Protected helper in const_iterator that returns the T
		// stored at the current position. Can be called by all
		// three versions of operator* without any type conversions.
		T& retrieve() const
		{
			return this->current->data;
		}

		// Protected constructor for const_iterator.
		// Expects a pointer that represents the current position.
		const_iterator(node* position) : current{ position }
		{ }

		friend class set<T>;
	};

	class iterator : public const_iterator {
	public:

		// Public constructor for iterator.
		// Calls the base-class constructor.
		// Must be provided because the private constructor
		// is written; otherwise zero-parameter constructor
		// would be disabled.
		iterator()
		{ }

		T& operator*()
		{
			return const_iterator::retrieve();
		}

		// Return the T stored at the current position.
		// For iterator, there is an accessor with a
		// const reference return type and a mutator with
		// a reference return type. The accessor is shown first.
		const T& operator*() const
		{
			return const_iterator::operator*();
		}
		// Increments iterator to next
		iterator& operator++()
		{
			node* temp;
			if (this->current == nullptr) {
				this->current = this->root;

				while (this->current->left != nullptr) {
					this->current = this->current->left;
				}
			}
			else if (this->current->right != nullptr) {
				this->current = this->current->right;
				while (this->current->left != nullptr) {
					this->current = this->current->left;
				}
			}
			else {
				temp = this->current->parent;
				while (temp != nullptr && this->current == temp->right) {
					this->current = temp;
					temp = temp->parent;
				}

				this->current = temp;
			}
			return *this;
		}
		// Increments iterator to next
		iterator operator++(int)
		{
			auto old = *this;
			++(*this);
			return old;
		}
		// Increments iterator to previous
		iterator& operator--()
		{
			node* temp;
			if (this->current == nullptr) {
				this->current = this->root;

				while (this->current->right != nullptr) {
					this->current = this->current->right;
				}
			}
			else if (this->current->left != nullptr) {
				this->current = this->current->left;
				while (this->current->right != nullptr) {
					this->current = this->current->right;
				}
			}
			else {
				temp = this->current->parent;
				while (temp != nullptr && this->current == temp->left) {
					this->current = temp;
					temp = temp->parent;
				}

				this->current = temp;
			}
			return *this;
		}
		// Increments iterator to previous
		iterator operator--(int)
		{
			auto old = *this;
			--(*this);
			return old;
		}

	protected:
		// Protected constructor for iterator.
		// Expects the current position.
		iterator(node* position) : const_iterator{ position }
		{ }

		friend class set<T>;
	};

	void insert(const T& value, node*& position, node*& parent)
	{
		if (position == nullptr) {
			position = new node{ value, nullptr, nullptr, parent };
		}
		else if (value < position->element) {
			this->insert(value, position->left, position);
		}
		else if (position->element < value) {
			this->insert(value, position->right, position);
		} // else, we have a duplicate, do_nothing();
	}

	void insert(T&& value, node*& position, node*& parent)
	{
		if (position == nullptr) {
			position = new node{ std::move(value), nullptr, nullptr, parent };
		}
		else if (value < position->element) {
			this->insert(std::move(value), position->left, position);
		}
		else if (position->element < value) {
			this->insert(std::move(value), position->right, position);
		} // else, we have a duplicate, do_nothing();
	}

	node* find_min(node* position) const
	{
		if (position == nullptr) {
			return nullptr;
		} // else, we are still looking, do_nothing();

		if (position->left == nullptr) {
			return position;
		} // else, we have no more left children, do_nothing();

		return this->find_min(position->left);
	}

	node* find_max(node* position) const
	{
		if (position == nullptr) {
			return nullptr;
		} // else, we are still looking, do_nothing();

		if (position->right == nullptr) {
			return position;
		} // else, we have no more right children, do_nothing();

		return this->find_max(position->right);
	}

	void remove(const T& value, node*& position)
	{
		if (position == nullptr) {
			return;   // Item not found; do nothing
		} //else, we still have nodes to check, do_nothing();

		if (value < position->element) {
			this->remove(value, position->left);
		}
		else if (position->element < value) {
			this->remove(value, position->right);
		}
		else if (position->left != nullptr && position->right != nullptr) {
			position->element = this->find_min(position->right)->element;
			this->remove(position->element, position->right);
		}
		else {
			auto* oldNode = position;
			position = (position->left != nullptr) ? position->left : position->right;
			delete oldNode;
		}
	}

	bool contains(const T& value, node* position) const
	{
		while (position != nullptr) {
			if (value < position->element) {
				position = position->left;
			}
			else if (position->element < value) {
				position = position->right;
			}
			else {
				return true;    // Match
			}
		}

		return false;   // No match
	}

	void clear(node*& position)
	{
		if (position != nullptr)
		{
			this->clear(position->left);
			this->clear(position->right);
			delete position;
		} // else, the current position is null, do_nothing();

		position = nullptr;
	}

	void print(node* position, std::ostream& out) const
	{
		if (position != nullptr)
		{
			this->print(position->left, out);
			out << position->element << std::endl;
			this->print(position->right, out);
		} // else, we reached an end, do_nothing();
	}

	node* clone(node* value) const
	{
		if (value == nullptr) {
			return nullptr;
		}
		else {
			return new node{ value->element, clone(value->left), clone(value->right), value->parent };
		}
	}

	// Return iterator representing beginning of list.
	   // Mutator version is first, then accessor version.
	iterator begin()
	{
		return iterator(find_min(root));
	}

	// Remember an iterator always begins with the first element and ends with the value PAST
	// the end. 
	const_iterator begin() const
	{
		return const_iterator(find_min(root));
	}

	iterator end()
	{
		return iterator(find_max(root));
	}

	//const_iterator end()
	//{
	//	return const_iterator(nullptr, this);
	//}
};

#endif