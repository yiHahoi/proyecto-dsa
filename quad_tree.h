#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

namespace dsa {

using point_t = pair<key_t, val_t>;
using zone_t = tuple<double, double, double, double>;
					// xmin    xmax    ymin    ymax

class quad_tree
{
	vector<pair<key_t, val_t>> data;

	struct node {
		enum quadrant_type {
			ne,
			nw,
			se,
			sw
		};

		enum node_type {
			grey,
			black
		};

		node_type type = grey;
		point_t* point = nullptr;
		zone_t zone;

		node* quadrants[4] = { nullptr, nullptr, nullptr, nullptr };

		node (std::pair<key_t, val_t>* p = nullptr)
		: point (p)
		{}
		~node() { 
			if (point != nullptr)
				delete point; 
		}

		node* quadrant() {
			node* n = new node();
			n->zone = zone;
			return n;
		}

		const tuple<double, double> center() {
			return { (get<0>(zone) + get<1>(zone))/2, 
					 (get<2>(zone) + get<3>(zone))/2 };
		}

		const tuple<double, double> coords() { 
			if (point)
				return { get<0>(point->first), get<1>(point->first) };
			else
				return center();
		}

		const val_t val() { return point->second; }
	};

	node* root = nullptr;

	double dist = 360;

	double xf[4] = {-0.25, 0.25, -0.25, 0.25};
	double yf[4] = {0.25, 0.25, -0.25, -0.25};

	node* create_node() {
		return new node();
	}

	node* create_quadrant() {
		return new node();
	}

	node::quadrant_type point_compare(node* p, node* n) {
		auto [p_lat, p_lon] = p->coords();
		auto [n_lat, n_lon] = n->coords();

		if (n_lat > p_lat)
			if (n_lon > p_lon)
				return node::ne;
			else
				return node::nw;
		else
			if (n_lon > p_lon)
				return node::se;
			else
				return node::sw;
	}

	void subdivide(node* quad, node* n) {
		auto q = point_compare(quad, n);

		cout << "reinsert: " << get<1>(n->val()) << endl;
		quad->quadrants[q] = new node(n->point);

		quad = quad->quadrants[q];
		quad->zone = { get<0>(quad->zone)*xf[q], 
					   get<1>(quad->zone)*xf[q], 
					   get<2>(quad->zone)*yf[q], 
					   get<3>(quad->zone)*yf[q] };
		quad->type = node::black;

		// cout << quad->coords() << endl;
	}

public:
	quad_tree() {}
	quad_tree(vector<pair<key_t, val_t>>& data) {
		for (auto& record: data) 
			insert(&record);
	}
	size_t size = 0;

	bool insert(pair<key_t, val_t>* point) {
		// cout << "************** insert**************" << endl;
		// nodo a ingresar
		node* n = new node(point);
		
		// nodo actual
		node* curr = root;

		// nodo padre del actual
		node* prev = nullptr;

		if (root == nullptr) {
			root = n;
			root->zone = { -dist, dist, -dist, dist };
			root->type = node::black;
			// cout << "root: " << get<1>(n->val()) << endl << endl;
			size++;
			return true;
		}

		if (curr->type == node::black) {
			curr->type = node::grey;

			if (n->coords() == curr->coords()){
				delete n;
				return false;
			}
			else {
				subdivide(curr->quadrant(), curr);
				curr->point = nullptr; // Lo convierte en cuadrante
			}
		}

		// cout << root->coords() << endl;

		while((curr != nullptr) && (curr->type == node::grey)) {
			auto q = point_compare(curr, n);
			prev = curr;
			curr = curr->quadrants[q];

			// nodo blanco
			if (curr == nullptr) {
				prev->quadrants[q] = n;
				prev->quadrants[q]->type = node::black;
				// cout << "white: " << get<1>(prev->quadrants[q]->val());
				// cin.get();
			}
			else if (curr->type == node::black) {
				// cout << "black: " <<  get<1>(curr->val());
				// cin.get();
				node* r = curr;
				subdivide(curr->quadrant(), r);
				// curr->point = nullptr; // Lo convierte en cuadrante
				curr = prev;
				auto q = point_compare(curr->quadrant(), n);
				curr->quadrants[q] = n;
				curr->quadrants[q]->type = node::black;
				curr = curr->quadrants[q];
			}
		}

		// cout << endl << endl;

		size++;
		return true;
	}

	void print() {
		inorder_walk(root);
		cout << endl;
	}

	void inorder_walk(node* n) {
		if (n != nullptr) {
			if (n->type == node::black)
				cout << n->coords() << endl;
			for (int i = 0; i < 4; ++i)
				inorder_walk(n->quadrants[i]);
		}
	}

	void bfs() {
		queue<node*> Q;
		node* v = root;

		cout << v->coords() << ": "
			 << v->val() << endl;


		Q.push(v);

		while(!Q.empty()) {
			for (auto u: v->quadrants) {
				if (u != nullptr)
					Q.push(u);
			}
			if (Q.front() != nullptr)
				cout << Q.front()->coords() << endl;
			Q.pop();
		}
	}
};
	
} // namespace dsa

#endif
