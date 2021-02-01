#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include <string>
#include <stdlib.h>
#include <time.h> 

using namespace std;


struct node{
	short number;
	struct node* prev = NULL;
	struct node* next = NULL;
};



void two_opt_func(struct node* left, struct node* right){
	struct node* right_next = right->next;
	struct node* cur_node = left->next;
	do{
		struct node* next_node = cur_node->next;
		cur_node->next = cur_node->prev;
		cur_node->prev = next_node;
		cur_node = next_node;
	}
	while(cur_node != right_next);
	
	left->next->next = right_next;
	right_next->prev = left->next;
	right->prev = left;
	left->next = right;
}

void three_opt_func(struct node* left, struct node* right){
	//cout << left->number << ' ' << right->number << endl;
	//swap(left, right);
	// struct node *left_prev = left->prev;
	// struct node *left_next = left->next;
	// left_prev->next = left_next;
	// left_next->prev = left_prev;
	// cout << left->prev->number << ' ' << left->number << ' ' << left->next->number << ' ' << right->number << ' ' << right->next->number << endl;
	
	
	left->prev->next = left->next;
	left->next->prev = left->prev;
	
	
	left->next = right->next;
	right->next->prev = left;
	
	right->next = left;
	left->prev = right;
	
	// cout << left->number << ' ' << right->number << endl;
	// left->next = right;
	// right->prev = left;
	// for(int i = 0; i < 2000000000; i++);
}

void three_opt_back(struct node* left, struct node* right, struct node* left_prev){
	right->next = left->next;
	left->next->prev = right;
	
	struct node* left_next = left_prev->next;
	left_prev->next = left;
	left->prev = left_prev;
	
	left_next->prev = left;
	left->next = left_next;
}


void swap(struct node* first, struct node* second){
	if(first != second && first->next != second && first->prev != second){
		first->prev->next = second;
		first->next->prev = second;
		second->prev->next = first;
		second->next->prev = first;		
		
		struct node* temp;
		
		temp = second->prev;
		second->prev = first->prev;
		first->prev = temp;
		
		temp = second->next;
		second->next = first->next;
		first->next = temp;	
	}
	
}

double get_sum(struct node *first_node, vector < vector <double> > &dist){
	double sum = 0;
	struct node *cur_node = first_node;
	do{
		sum += dist[cur_node->number][cur_node->next->number];
		// cout << cur_node->number << ' ';
		cur_node = cur_node->next;
	}
	while(cur_node != first_node);
	return sum;
}

void two_opt_alg(struct node *first_node, vector < vector <double> > &dist){
	bool flag = true;
		for(int i = 0; i < 10000 && flag; i++){ // int i = 0; i < 1000; i++
			flag = false;
			struct node *left_node = first_node;
			do{
				struct node *right_node = left_node->next->next;
				double best_dist = -1;
				struct node* best_right_node = NULL;
				double max_delta = -1;
				do{
					double  was, new_;
					was = dist[left_node->number][left_node->next->number] + dist[right_node->number][right_node->next->number];
					new_ = dist[left_node->number][right_node->number] + dist[left_node->next->number][right_node->next->number];
					if(new_ < was){
						if(max_delta == -1 || (was - new_) > max_delta){
							best_dist = new_;
							best_right_node = right_node;
							max_delta = (was - new_);
						}
					}
					right_node = right_node->next;
				}
				while(right_node->next != left_node); // !!!
				
				if(best_right_node != NULL){
					double was = dist[left_node->number][left_node->next->number] + dist[best_right_node->number][best_right_node->next->number];
					double new_ = dist[left_node->number][best_right_node->number] + dist[left_node->next->number][best_right_node->next->number];
					if(left_node != best_right_node){
						if(new_ >= was){
							cout << "AHTUNG" << endl;
							for(;;);
						}
						two_opt_func(left_node, best_right_node);
					}
					flag = true;
				}
				
				
				left_node = left_node->next;
			}
			while(left_node != first_node);
		}
}


bool my_opt(struct node *left, struct node *right, vector < vector <double> > &dist){
	bool flag = false;
	double was = dist[right->number][right->next->number] + dist[left->prev->number][left->number] + dist[left->number][left->next->number];
	double new_ = dist[right->number][left->number] + dist[left->number][right->next->number] + dist[left->prev->number][left->next->number];
	if(new_ < was){
		//cout << "yes" << was - new_ << endl;
		//cout << "left: " << left->number << " right: " << right->number << endl;
		//cout << "sum before my_opt: " << get_sum(left, dist) << endl;
		//cout << "was: " << was << " new_: " << new_ << endl;
//		
//		cout << "left->prev: " << left->prev << " left->prev->next: " << left->prev->next  << endl; 
//		cout << "left->next: " << left->next << " left->next->prev: " << left->next->prev  << endl;
		left->prev->next = left->next;
		left->next->prev = left->prev;
//		cout << "left->prev: " << left->prev << " left->prev->next: " << left->prev->next  << endl; 
//		cout << "left->next: " << left->next << " left->next->prev: " << left->next->prev  << endl;
		
		right->next->prev = left;
		left->next = right->next;
		
		left->prev = right;
		right->next = left;
		
		flag = true;
		
		//cout << "sum after my_opt: " << get_sum(left, dist) << endl << endl;
	}
	return flag;
}

int main(){
	ifstream in("./mona/457241_data.txt");
	// ifstream in("./525657_data.txt");
	short n; in >> n; n = n + 1; // ! n + 1
	// cout << n << endl;
	
	struct node *first_node = NULL;
	struct node *prev_node = NULL;
	short number; double x, y;
	
	vector <double> x_arr(n), y_arr(n);
	vector < vector <double> > dist(n); // int! not double
	for(short i = 1; i < n; i++){
		vector <double> dist_line(n); // int! not double
		dist[i] = dist_line;
	}
	
	for(short i = 1; i < n; i++){
		in >> number >> x >> y;
		x_arr[number] = x;
		y_arr[number] = y;
		struct node *cur_node = new struct node();
		cur_node->number = number;
		cur_node->prev = prev_node;
		if (prev_node != NULL){
			prev_node->next = cur_node;
		}
		if (i == 1){
			first_node = cur_node;
		}
		if (i == (n - 1)){
			first_node->prev = cur_node;
			cur_node->next = first_node;
		}
		prev_node = cur_node;
	}
	in.close();
	
	for(short i = 1; i < n; i++){
		for(short j = 1; j < n; j++){
			double dist_now = sqrt(pow(x_arr[j] - x_arr[i], 2) + pow(y_arr[j] - y_arr[i], 2));
			dist[i][j] = dist_now;
			dist[j][i] = dist_now;
		}
	}
	
	
	
	
	//cout << sum << endl;
	
	// for(;;);
	
	vector <struct node*> swaps(2);
	swaps[0] = first_node;
	swaps[1] = first_node;
	
	struct node* swap_first = first_node;
	do{	
		struct node* swap_second = first_node->next;
		do{
			if (true){
				srand (time(NULL));srand (time(NULL));
				int count = rand() % 600*n + 30*n;
				for(int i = 0; i < count; i++){
					int x = 100;
					if (count < 40*n){
						x = 1;
					}
					int number1 = rand() % n, number2 = rand() % n / x;
					struct node *node1 = first_node, *node2 = first_node;
					for(int j = 0; j <= number1; j++){
						node1 = node1 -> next;
					}
					node2 = node1;
					for(int j = 0; j <= number2; j++){
						node2 = node2 -> next;
					}
					//cout << "swap " << i << endl;
					swap(node1, node2);
				}	
			}
			
			if(false){
			//for(int k = 0; k < swaps.size(); k++r)
				// swap(swaps[0], swaps[1]);
				swap(swap_first, swap_second);
				swap(swap_first->next, swap_second->next);
				swap(swap_first->next->next, swap_second->prev);
				cout << "swapped" << endl;
				swaps[0] = swap_first;
				swaps[1] = swap_second;
			}
			
			// cout << 1 << endl;
			// for(int i = 0; i < 1000000000; i++);
			
			
			two_opt_alg(first_node, dist);
			
			
	
			cout << "sum: " << get_sum(first_node, dist) << endl;
			
			
			for(int i = 0; i < 2; i++){
				
				bool flag = true;
				double sum = 0, prev_sum = 1;
				for(int i = 0; i < 20; i++){
					prev_sum = sum;
					flag = false;
					struct node* left = first_node;
					do{
						struct node* right = left->next;
						do{
							bool flag2 = my_opt(left, right, dist);
							if(flag2){
								flag = true;
								// break;
							}
							right = right->next;
						}
						while(right != left);
						left = left->next;
					}
					while(left != first_node);
					sum = get_sum(first_node, dist);
					//cout << "... " << sum << " ..." << endl;sum < prev_sum
				}
			
			}
			
			double sum = get_sum(first_node, dist);
			cout << "sum after my_opt: " << sum << endl;
			
			
			if (sum < 455000){
			
			
				ofstream out;
				
				string file_name = "./mona/";
				int sum2 = sum;
				int sum_count = 0;
				while(sum2 > 0){
					sum_count += 1;
					sum2 /= 10;
				}
				int power = pow(10, (sum_count - 1));
				for(int i = 0; i < sum_count; i++){
					file_name += (char)((int)sum / power % 10 + '0');
					power /= 10;		
				}
				
				string file_name1 = file_name + "_path.txt";
			 
			    int file_name1_len = file_name1.length();
			 
			    char char_array1[file_name1_len + 1];
			 
			    strcpy(char_array1, file_name1.c_str());
			    out.open(char_array1); // îêðûâàåì ôàéë äëÿ çàïèñè
				
			    struct node *cur_node = first_node;
				do{
					out << cur_node->number << ' ';
					cur_node = cur_node->next;
				}
				while(cur_node != first_node);
			    out.close();
					
				
				string file_name2 = file_name + "_data.txt";
			 
			    int file_name2_len = file_name2.length();
			 
			    char char_array2[file_name2_len + 1];
			 
			    strcpy(char_array2, file_name2.c_str());
			    out.open(char_array2); // îêðûâàåì ôàéë äëÿ çàïèñè
			    
				out << (n - 1) << endl;
			    cur_node = first_node;
				do{
					int number = cur_node->number;
					out << number << ' ' << x_arr[number] << ' ' << y_arr[number] << endl;
					cur_node = cur_node->next;
				}
				while(cur_node != first_node);
			    out.close();
			
	
			}
			
			// vector < vector <double> > dist;
			
			/*
			struct node *cur_node = first_node;
			do{
				cout << cur_node->number << ' ' << cur_node->prev << ' ' <<  cur_node->next << endl;
				cur_node = cur_node->next;
			}
			while(cur_node != first_node);
			
			vector < vector <double> > dist;
			*/
			swap_second = swap_second->next;
		}
		while(swap_second != swap_first);
		swap_first = swap_first->next;
	}
	while(swap_first != first_node);
	
	
	return 0;
	
}
