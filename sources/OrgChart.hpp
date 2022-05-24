#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

using namespace std;
namespace ariel {
    class OrgChart {
    private:

        class node {
        public:
            string data;
            vector<node *> subs;

            node(const string &data) : data(data) {}
        };

        node *root;

        void deleteAllNodes(node *root);

        node *copyAllNodes(node *other);

        node *findNode(node *root, const string &value);

        static void printChartLevels(ostream &ost, const node *ptr, int indent);

    public:
        OrgChart();

        OrgChart(const OrgChart &other);

        OrgChart &operator=(OrgChart const &other);

        OrgChart(OrgChart &&other) noexcept;

        OrgChart &operator=(OrgChart &&other) noexcept;

        ~OrgChart();

        OrgChart &add_root(const string &data);

        OrgChart &add_sub(const string &parent, const string &son);


        friend ostream &operator<<(ostream &ost, const OrgChart &chart);

        class iterator {
        private:
            node *current_node;
            queue<node *> traversal_order;
        public:
            iterator();

            virtual ~iterator() = default;

            iterator(iterator const &) = default;

            iterator &operator=(iterator const &) = default;

            iterator(iterator &&) = default;

            iterator &operator=(iterator &&) = default;

            node **getCurrentNode();

            queue<node *> *getTraversalOrder();

            string &operator*() const;

            string *operator->() const;

            bool operator==(const iterator &rhs) const;

            bool operator!=(const iterator &rhs) const;

            iterator &operator++();

            iterator operator++(int);
        };

        class level_order_iterator : public iterator {
        public:
            level_order_iterator(node *ptr);
        };

        level_order_iterator begin_level_order();

        level_order_iterator end_level_order();

        class reverse_order_iterator : public iterator {
        public:
            reverse_order_iterator(node *ptr);
        };

        reverse_order_iterator begin_reverse_order();

        reverse_order_iterator reverse_order();

        class preorder_iterator : public iterator {
        private:
            void preorder_traversal(node *cur);

        public:
            preorder_iterator(node *ptr);
        };

        preorder_iterator begin_preorder();

        preorder_iterator end_preorder();

        level_order_iterator begin();

        level_order_iterator end();
    };

    ostream &operator<<(ostream &ost, const OrgChart &chart);
}

