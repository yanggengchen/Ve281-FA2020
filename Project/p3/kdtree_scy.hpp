#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <iostream>

/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");
protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            // TODO: implement this function
            // case 0: this is the end iterator
            //  effect: throw an error
            if (!node) {
                std::cout << "error: increment on the end iterator" << std::endl;
                throw "error: increment on the end iterator";
            }
            // case 1: current node has a right subtree
            //  effect: return the left most leaf in the right subtree
            if (node->right) {
                Node *temp = node->right;
                // go to the left most leaf
                while (temp->left) temp = temp->left;
                node = temp;
                return;
            }
            // case 2: current node doesn't have a right subtree
            //  effect: move up until a "right turn"
            while (node->parent) {
                // case 2-1: current node is in the left subtree of the ancestor node
                if (node == (node->parent)->left) { 
                    node = node->parent;
                    return;
                }
                node = node->parent; // move up current node
            }
            // case 2-2: not found, node == nullptr
            node = nullptr;
            return;
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            // TODO: implement this function
            // case 0: decrement the end iterator
            //  effect: return the right most leaf node
            if (!node) {
                Node *temp = tree->root;
                while (temp->right) temp = temp->right;
                node = temp;
                return;
            }
            // case 1: current node has a left subtree
            //  effect: return the right most leaf in the left subtree
            if (node->left) {
                Node *temp = node->left;
                // go to the right most leaf
                while (temp->right) temp = temp->right;
                node = temp;
                return;
            }
            // case 2: current node doesn't have a left subtree
            //  effect: move up until a "left turn"
            while (node->parent) {
                // case 2-1: current node is in the right subtree of the ancestor node
                if (node == (node->parent)->right) { 
                    node = node->parent;
                    return;
                }
                node = node->parent; // move up current node
            }
            // case 2-2: this is the begin iterator
            //  effect: throw an error
            std::cout << "error: decrement on the begin iterator" << std::endl;
            throw "error: decrement on the begin iterator";
            return;
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            return node == that.node;
        }

        bool operator!=(const Iterator &that) const {
            return node != that.node;
        }

        Data *operator->() {
            return &(node->data);
        }

        Data &operator*() {
            return node->data;
        }
    };

protected:                      // DO NOT USE private HERE!
    Node *root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (!node) return nullptr;
        if (key == node->key()) {
            return node;
        }
        if (compareKey<DIM, std::less<> >(key, node->key())) {
            return find<DIM_NEXT>(key, node->left);
        }
        else {
            return find<DIM_NEXT>(key, node->right);
        }
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        // NOTE: update treeSize
        // case 1: key doesn't exist
        //  effect: insert new Node; update treeSize
        if (node == nullptr) {
            node = new Node(key, value, parent);
            treeSize++; // update treeSize
            return true;
        }
        // case 2: key already exists
        if (key == node->key()) {
            node->value() = value; // update the value only
            return false;
        }
        // case 3: recursive call
        if (compareKeyAtDIM<DIM, std::less<>>(key, node->key())) // key[DIM] < node->key()[DIM]
            return insert<DIM_NEXT>(key, value, node->left, node); // node is the parent of its lchild
        else
            return insert<DIM_NEXT>(key, value, node->right, node); // node is the parent of its rchild
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)){
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (!node) return nullptr;
        Node *min = findMin<DIM_CMP, DIM_NEXT>(node->left);
        if (DIM_CMP != DIM) {
            Node *rightMin = findMin<DIM_CMP, DIM_NEXT>(node->right);
            min = compareNode<DIM_CMP, std::less<> >(min, rightMin);
        }
        return compareNode<DIM_CMP, std::less<> >(min, node);
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (!node) return nullptr;
        Node *max = findMax<DIM_CMP, DIM_NEXT>(node->right);
        if (DIM_CMP != DIM) {
            Node *leftMax = findMax<DIM_CMP, DIM_NEXT>(node->left);
            max = compareNode<DIM_CMP, std::greater<> >(max, leftMax);
        }
        return compareNode<DIM_CMP, std::greater<> >(max, node);
    }

    template<size_t DIM>
    Node *findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node *findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        // NOTE: update treeSize; release unused memory;
        if (!node) return nullptr;
        if (key == node->key()) {
            if ((!node->left) && (!node->right)) { // node is a leaf
                delete node;
                treeSize--; // update treeSize for successful erasion
                return nullptr;
            }
            else if (node->right) { // search for the mininum element in the right subtree
                Node *minNode = findMin<DIM, DIM_NEXT>(node->right);
                // update the  "key", "value" & "right" fields of "node"
                updateNodeData(node, minNode->key(), minNode->value());
                node->right = erase<DIM_NEXT>(node->right, minNode->key());
            }
            else if (node->left) { // search for the maximum element in the left subtree
                Node *maxNode = findMax<DIM, DIM_NEXT>(node->left);
                // update the  "key", "value" & "left" fields of "node"
                updateNodeData(node, maxNode->key(), maxNode->value());
                node->left = erase<DIM_NEXT>(node->left, maxNode->key());
            }
        }
        else { // recursive call
            if (compareKeyAtDIM<DIM, std::less<>>(key, node->key()))
                node->left = erase<DIM_NEXT>(node->left, key);
            else
                node->right = erase<DIM_NEXT>(node->right, key);
        }
        return node;
    }

    template<size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    // TODO: define your helper functions here if necessary

    /**
     * Return the relationship between two Key at DIM
     */
    template<size_t DIM, typename Compare>
    static bool compareKeyAtDIM(const Key &a, const Key &b, Compare compare = Compare()) {
        return compare(std::get<DIM>(a), std::get<DIM>(b));
    }

    /**
     * Erase a node and all its child nodes; 
     * Update treeSize. 
     * Time Complexity: O(n), where n is the # of nodes deleted. 
     * @param node
     */    
    void eraseNodes(Node *node) {
        // NOTE: update treeSize
        if (!node) return;
        if (node->left) eraseNodes(node->left);
        if (node->right) eraseNodes(node->right);
        delete node;
        treeSize--;
        return;
    }
    
    /**
     * REQUIRE: t_node is a nullptr; 
     * EFFECT: deep copy s_node and its child nodes to t_node; 
     * Time Complexity: O(n), where n is the # of nodes of the tree specified by s_node.
     * @param t_node target node
     * @param s_node source source
     * @param t_parent the parent of the target node
     */
    void copyNodes(Node *&t_node, Node *s_node, Node *t_parent) {
        // NOTE: do dynamic allocation for  new nodes
        if (!s_node) return;
        t_node = new Node(s_node->key(), s_node->value(), t_parent);
        copyNodes(t_node->left, s_node->left, t_node);
        copyNodes(t_node->right, s_node->right, t_node);
    }

    /**
     * Replace the "key" & "value" of node, while maintaining all other attributes.
     * @param node
     * @param key
     * @param value
     */
    void updateNodeData(Node *&node, const Key &key, const Value &value) {
        // case 1: allocate a new node, if "node" is nullptr
        if (!node) {
            Node *tempNode = new Node(key, value, nullptr);
            tempNode->left = nullptr;
            tempNode->right = nullptr;
            node = tempNode;
            return;
        }
        // case 2: "node" exists, update key & value
        const_cast<Key &>(node->key()) = key;
        node->value() = value;
    }

    /**
     * Called by the copy constructor & the assignment operator. 
     * Time complexity: O(n), where n is the # of nodes of "that". 
     * @param that
     */
    void copyFrom(const KDTree &that) {
        // NOTE: deep copy; remember to allocate memory; remember to update data members;
        root = nullptr;
        treeSize = 0;
        // deep copy each node of "that"
        copyNodes(root, const_cast<Node *>(that.root), nullptr);
        // update data members
        treeSize = that.treeSize;
    }

    // Nov.29: this function is unnecessary now, since "compareKey" has been updated
    /**
     * Function object used to stable_sort the vector of key-value pair.
     */
    
    template<size_t DIM, typename Compare>
    static bool total_compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // case 1: two keys are equal in all demensions
        if (DIM == KeySize - 1) {
            if (!compare(std::get<DIM>(a), std::get<DIM>(b)) && !compare(std::get<DIM>(b), std::get<DIM>(a)))
                return false;
        }
        // case 2: two keys differ at a certain demension
        if (compare(std::get<DIM>(a), std::get<DIM>(b)) || compare(std::get<DIM>(b), std::get<DIM>(a))) {
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        // case 3: two keys equal for the first DIM demensions, recursively compare the next demension
        else
        {
            return total_compareKey<DIM_NEXT, Compare>(a, b);
        }
    }


    /**
     * Recursively called by the vector-version initializer
     * @tparam DIM current dimension of node
     * @param v_begin the begin iterator of a vector of data
     * @param v_end the end iterator of a vector of data
     * @param node 
     * @param parent 
     */
    template<size_t DIM>
    void KDTree_Helper(std::vector<std::pair<Key, Value>> &v, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // terminal case: "v" is empty
        if (!v.size()) return;
        // find the median and partition the vector "v"
        auto mid_it = v.begin() + v.size()/2;
        std::nth_element(v.begin(), mid_it, v.end(), [](const auto& a, const auto& b) { return compareKey<DIM, std::less<>>(a.first, b.first); });
        // update key, value, parent of "node"
        updateNodeData(node, v[v.size()/2].first, v[v.size()/2].second);
        node->parent = parent;
        // increase the treeSize for each copied node
        treeSize++;
        // recursively deal with left/right subtree of "node"
        std::vector<std::pair<Key, Value>> v_left(v.begin(), mid_it);
        std::vector<std::pair<Key, Value>> v_right(mid_it + 1, v.end());
        KDTree_Helper<DIM_NEXT>(v_left, node->left, node);
        KDTree_Helper<DIM_NEXT>(v_right, node->right, node);
    }

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        // TODO: implement this function
        // remove duplicates in "v"
        std::stable_sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return total_compareKey<0, std::less<>>(a.first, b.first); });
        auto end_it = std::unique(v.begin(), v.end(), [](const auto& a, const auto& b) {return a.first == b.first;});
        v.erase(end_it, v.end());

        root = nullptr;
        treeSize = 0;
        // call helper to recursively initialize the tree
        KDTree_Helper<0>(v, root, nullptr);
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) {
        // TODO: implement this function
        root = nullptr;
        treeSize = 0;
        copyFrom(that);
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        // TODO: implement this function
        if (this != &that)
            copyFrom(that);
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        // TODO: implement this function
        // effect: release dynamically allocated storage
        eraseNodes(root);
    }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key &key) {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key &key, const Value &value) {
        insert<0>(key, value, root, nullptr);
    }

    template<size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template<size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};