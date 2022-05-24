#include "OrgChart.hpp"
#include <stdexcept>

using namespace std;
using namespace ariel;

void OrgChart::deleteAllNodes(node *root) {
    if (root == nullptr) {
        return;
    }
    auto itr = root->subs.begin();
    while (itr != root->subs.end()) {
        deleteAllNodes(*itr);
        itr++;
    }
    delete root;
}

OrgChart::node *OrgChart::findNode(node *root, const string &value) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->data == value) {
        return root;
    }

    auto itr = root->subs.begin();
    while (itr != root->subs.end()) {
        node *found = findNode(*itr, value);
        if (found != nullptr) {
            return found;
        }
        itr++;
    }

    return nullptr;
}

OrgChart::OrgChart() : root(nullptr) {}

OrgChart::node *OrgChart::copyAllNodes(node *other) {
    if (other == nullptr) {
        return nullptr;
    }
    node *newRoot = new node(other->data);
    auto itr = other->subs.begin();
    while (itr != other->subs.end()) {
        node *copyNode = copyAllNodes(*itr);
        newRoot->subs.push_back(copyNode);
        ++itr;
    }
    return newRoot;
}

OrgChart::OrgChart(const OrgChart &other) : root(nullptr) {
    this->operator=(other);
}

OrgChart &OrgChart::operator=(OrgChart const &other) {
    if (&other != this) {
        if (root != nullptr) {
            deleteAllNodes(root);
            root = nullptr;
        }
        if (other.root != nullptr) {
            root = copyAllNodes(other.root);
        }
    }
    return *this;
}

OrgChart::OrgChart(OrgChart &&other) noexcept: root(nullptr) {
    this->operator=(other);
}

OrgChart &OrgChart::operator=(OrgChart &&other) noexcept {
    if (this != &other) {
        if (root != nullptr) {
            deleteAllNodes(root);
            root = nullptr;
        }
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}

OrgChart::~OrgChart() {
    deleteAllNodes(root);
}

OrgChart &OrgChart::add_root(const string &data) {
    if (root == nullptr) {
        root = new node(data);
    } else {
        root->data = data;
    }
    return *this;
}

OrgChart &OrgChart::add_sub(const string &parent, const string &son) {
    node *parentNode = findNode(root, parent);
    if (parentNode == nullptr) {
        throw std::logic_error("The parent element is not in the chart.");
    }
    parentNode->subs.push_back(new node(son));
    return *this;
}


void OrgChart::printChartLevels(ostream &ost, const node *ptr, int indent) {
    static const int LEVEL_INDENT = 4;
    int index = 0;
    if (ptr == nullptr) {
        return;
    }
    for (index = 0; index < indent - LEVEL_INDENT; index++) {
        ost << ' ';
    }
    for (index = 0; index < LEVEL_INDENT; index++) {
        ost << '-';
    }
    ost << ptr->data << "\n";
    auto itr = ptr->subs.begin();
    while (itr != ptr->subs.end()) {
        printChartLevels(ost, *itr, indent + LEVEL_INDENT);
        ++itr;
    }
}

ostream &ariel::operator<<(ostream &ost, const OrgChart &chart) {
    OrgChart::printChartLevels(ost, chart.root, 0);
    return ost;
}

OrgChart::iterator::iterator() :current_node(nullptr) {}

OrgChart::node **OrgChart::iterator::getCurrentNode() {
    return &this->current_node;
}

queue<OrgChart::node *> *OrgChart::iterator::getTraversalOrder() {
    return &this->traversal_order;
}

string &OrgChart::iterator::operator*() const {
    return current_node->data;
}

string *OrgChart::iterator::operator->() const {
    return &current_node->data;
}

bool OrgChart::iterator::operator==(const iterator &rhs) const {
    return current_node == rhs.current_node;
}

bool OrgChart::iterator::operator!=(const iterator &rhs) const {
    return current_node != rhs.current_node;
}

OrgChart::iterator &OrgChart::iterator::operator++() {
    if (!traversal_order.empty()) {
        current_node = traversal_order.front();
        traversal_order.pop();
    } else {
        current_node = nullptr;
    }
    return *this;
}

OrgChart::iterator OrgChart::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

OrgChart::level_order_iterator::level_order_iterator(node *ptr) {
    auto *current_node = getCurrentNode();
    auto *traversal_order = getTraversalOrder();
    if (ptr == nullptr) {
        *current_node = nullptr;
    } else {
        queue<node *> tmp;
        tmp.push(ptr);
        while (!tmp.empty()) {
            node *cur_node = tmp.front();
            tmp.pop();

            traversal_order->push(cur_node);
            for (auto itr = cur_node->subs.begin(); itr != cur_node->subs.end(); ++itr) {
                tmp.push(*itr);
            }
        }
        *current_node = traversal_order->front();
        traversal_order->pop();
    }
}

OrgChart::level_order_iterator OrgChart::begin_level_order() {
	if (root == nullptr){
		throw std::invalid_argument("Empty chart.");
	}
    return level_order_iterator(root);
}

OrgChart::level_order_iterator OrgChart::end_level_order() {
	if (root == nullptr){
		throw std::invalid_argument("Empty chart.");
	}
    return level_order_iterator(nullptr);
}

OrgChart::reverse_order_iterator::reverse_order_iterator(node *ptr) {
    auto *current_node = getCurrentNode();
    auto *traversal_order = getTraversalOrder();
    if (ptr == nullptr) {
        *current_node = nullptr;
    } else {
        queue<node *> reverse_order_per_level;
        reverse_order_per_level.push(ptr);
        stack<node *> stack;
        while (!reverse_order_per_level.empty()) {
            node *front = reverse_order_per_level.front();
            reverse_order_per_level.pop();
            stack.push(front);
            for (auto itr = front->subs.rbegin(); itr != front->subs.rend(); ++itr) {
                reverse_order_per_level.push(*itr);
            }
        }

        while (!stack.empty()) {
            traversal_order->push(stack.top());
            stack.pop();
        }

        *current_node = traversal_order->front();
        traversal_order->pop();
    }
}

OrgChart::reverse_order_iterator OrgChart::begin_reverse_order() {
	if (root == nullptr){
		throw std::invalid_argument("Empty chart.");
	}
    return reverse_order_iterator(root);
}

OrgChart::reverse_order_iterator OrgChart::reverse_order() {
	if (root == nullptr){
		throw std::invalid_argument("Empty chart.");
	}
    return reverse_order_iterator(nullptr);
}

void OrgChart::preorder_iterator::preorder_traversal(node *cur) {
    auto *traversal_order = getTraversalOrder();
    if (cur == nullptr) {
        return;
    } 
	traversal_order->push(cur);
	for (auto itr = cur->subs.begin(); itr != cur->subs.end(); ++itr) {
		preorder_traversal(*itr);
	}
}

OrgChart::preorder_iterator::preorder_iterator(node *ptr) {
    auto *current_node = getCurrentNode();
    auto *traversal_order = getTraversalOrder();
    if (ptr == nullptr) {
        current_node = nullptr;
    } else {
        preorder_traversal(ptr);
        *current_node = traversal_order->front();
        traversal_order->pop();
    }
}

OrgChart::preorder_iterator OrgChart::begin_preorder() {
	if (root == nullptr){
		throw std::invalid_argument("Empty chart.");
	}
    return preorder_iterator(root);
}

OrgChart::preorder_iterator OrgChart::end_preorder() {
	if (root == nullptr){
		throw std::invalid_argument("Empty chart.");
	}
    return preorder_iterator(nullptr);
}

OrgChart::level_order_iterator OrgChart::begin() {
	if (root == nullptr){
		throw std::invalid_argument("Empty chart.");
	}
    return level_order_iterator(root);
}

OrgChart::level_order_iterator OrgChart::end() {
	if (root == nullptr){
		throw std::invalid_argument("Empty chart.");
	}
    return level_order_iterator(nullptr);
}