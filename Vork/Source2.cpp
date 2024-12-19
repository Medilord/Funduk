#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cstring>

bool flag_num(std::string num) {
    for (int i = 0; i < num.length(); i++) {
        if (not((num[i] >= 48) and (num[i] <= 57))) { // ASCII {0..9}
            return false;
        }
    }
    return true;
}

bool is_valid_key(std::string line) {
    /* Данная функция использует регулярные выражения для проверки ввода.
    Фамилия;Имя;Отчество;День;Месяц;Год;
    */
    static const std::regex LineRegExp("(([A-Z][a-z]+;)([A-Z][a-z]+;)([A-Z][a-z]+;)(0[1-9]|[12][0-9]|3[01]);(0[1-9]|1[012]);(19[0-9]{2}|20[01][0-9]|202[0-4]);)");

    return std::regex_match(line, LineRegExp);
}


struct key_info {
private:


public:
    std::string surname; // Первый символ - заглавная (если есть пробел, то и после него тоже)
    std::string name;
    std::string patronymic;
    unsigned short day;
    unsigned short month;
    unsigned short year;

    

    void string_to_key(std::string line) {

        char* LineSeparator = strtok(const_cast<char*>(line.c_str()), ";");
        surname = LineSeparator;
        LineSeparator = strtok(NULL, ";");
        name = LineSeparator;

        LineSeparator = strtok(NULL, ";");
        patronymic = LineSeparator;

        LineSeparator = strtok(NULL, ";");
        day = std::stoi(LineSeparator);

        LineSeparator = strtok(NULL, ";");
        month = std::stoi(LineSeparator);

        LineSeparator = strtok(NULL, ";");
        year = std::stoi(LineSeparator);

        LineSeparator = strtok(NULL, ";");
            
        return;
        
    }
};

bool operator < (const key_info& key1, const key_info& key2) {

    if (key1.surname < key2.surname) return true;
    else if (key1.surname > key2.surname) return false;

    if (key1.name < key2.name) return true;
    else if (key1.name > key2.name) return false;

    if (key1.patronymic < key2.patronymic) return true;
    else if (key1.patronymic > key2.patronymic) return false;

    if (key1.year < key2.year) return true;
    else if (key1.year > key2.year) return false;

    if (key1.month < key2.month) return true;
    else if (key1.month > key2.month) return false;

    if (key1.day < key2.day) return true;

    return false;
}

bool operator > (const key_info& key1, const key_info& key2) {
    if (key1.surname > key2.surname) return true;
    else if (key1.surname < key2.surname) return false;

    if (key1.name > key2.name) return true;
    else if (key1.name < key2.name) return false;

    if (key1.patronymic > key2.patronymic) return true;
    else if (key1.patronymic < key2.patronymic) return false;

    if (key1.year > key2.year) return true;
    else if (key1.year < key2.year) return false;

    if (key1.month > key2.month) return true;
    else if (key1.month < key2.month) return false;

    if (key1.day > key2.day) return true;

    return false;
}

std::ostream& operator << (std::ostream& os, const key_info& key) {
    return os << key.surname << " " << key.name[0] << "." << key.patronymic[0] << ". " << key.day << "." << key.month << "." << key.year;
}



struct list_node {
    unsigned int data;
    list_node* next = nullptr;
    list_node* prev = nullptr;

};

class list {

public:
    list_node* head;

    list() : head(nullptr) {} // Инициализация

    void to_delete_list() {
        if (head != nullptr) {
            while (head->next != nullptr) {
                head = head->next;
                delete head->prev;
            }
            delete head;
            head = nullptr;
        }
    }


    void append(unsigned int num) { // Добавление в конец списка
        if (head == nullptr) {
            head = new list_node;
            head->data = num;
        }
        else {
            list_node* end_node = head;
            list_node* new_node = new list_node();

            while (end_node->next != nullptr) { end_node = end_node->next; }

            new_node->data = num;
            new_node->prev = end_node;
            end_node->next = new_node;
        }
    }

    void print_list() {  // Показать список

        list_node* head_node = head;

        while (head_node->next != nullptr) {
            std::cout << " " << head_node->data << " <=>";
            head_node = head_node->next;
        }
        std::cout << " " << head_node->data;

    }

    void write_in_file(std::ofstream &outputFile) {
        list_node* head_node = head;

        while (head_node->next != nullptr) {
            outputFile << " " << head_node->data << " <=>";
            head_node = head_node->next;
        }
        outputFile << " " << head_node->data;
    }

};



class avl_tree {
private:

    const std::string Up_corner = "/-";
    const std::string L_corner = "\\-";
    const std::string Vertical_line = "| ";

	struct node {	
		key_info key;
		short balance_f = 0;
		node* left = nullptr;
		node* right = nullptr;
        list* value = nullptr;

        node(key_info add_key, unsigned int line_num) { 
            key = add_key; 
            value = new list;
            value->append(line_num); 
        }
	};

    node* turn_left(node* root) {
        node* right_node = root->right;
        root->right = right_node->left;
        right_node->left = root;

        //fixbalance_f(root);
        //fixbalance_f(right_node);
//        root->balance_f = 0;
        return right_node;
    }

    node* turn_right(node* root){
        node* left_node = root->left;
        root->left = left_node->right;
        left_node->right = root;

        //fixbalance_f(root);
        //fixbalance_f(left_node);
//       root->balance_f = 0;
        return left_node;
    }

    //node* balance_tree(node* root) {

    //    if (root->balance_f == 2) { // Правое поддерево перевешивает
    //        if (root->right->balance_f == -1) { 
    //            if (root->left->right->balance_f == -1) {
    //                root->left->balance_f = 1;
    //            }
    //            else { root->left->balance_f = 0; }

    //            if (root->left->right->balance_f == 1) {
    //                root->balance_f = -1;
    //            }
    //            else { root->balance_f = 0; }
    //            
    //            root->right = turn_right(root->right); 
    //        }
    //        root = turn_left(root);
    //        root->balance_f = 0;
    //    }
    //    if (root->balance_f == -2){ // Левое поддерево перевешивает
    //        if (root->left->balance_f == 1) { 
    //            if (root->left->right->balance_f == 1) {
    //                root->left->balance_f = -1;
    //            }
    //            else { root->left->balance_f = 0; }

    //            if (root->left->right->balance_f == -1) {
    //                root->balance_f = 1;
    //            }
    //            else { root->balance_f = 0; }
    //            root->left = turn_left(root->left);
    //        }
    //        root = turn_right(root);
    //        root->balance_f = 0;
    //    }
    //    return root;
    //}


    node* balanceL(node* root, bool& change_bf) {
        node* son, *grandson;

        if (root->balance_f == -1) {
            root->balance_f = 0;
            return root;
        }
        
        if (root->balance_f == 0) {
            root->balance_f = 1;
            change_bf = false;
            return root;
        }

        if (root->balance_f == 1){
            //root->balance_f = 2;
            son = root->right;
            if (son->balance_f >= 0) {
                root->right = son->left;
                son->left = root;

                if (son->balance_f == 0) {
                    root->balance_f = 1;
                    son->balance_f = -1;
                    change_bf = false;
                }
                else {
                    root->balance_f = 0;
                    son->balance_f = 0;
                }
                root = son;
            }
            else {
                grandson = son->left;
                son->left = grandson->right;
                grandson->right = son;
                root->right = grandson->left;
                grandson->left = root;

                if (grandson->balance_f == 1) { root->balance_f = -1; }
                else { root->balance_f = 0; }
                if (grandson->balance_f == -1) { son->balance_f = 1; }
                else { son->balance_f = 0; }

                grandson->balance_f = 0;
                root = grandson;
            }
        }
        return root;
    }

    node* balanceR(node* root, bool& change_bf) {
        node* son, * grandson;

        if (root->balance_f == 1) {
            root->balance_f = 0;
            return root;
        }

        if (root->balance_f == 0) {
            root->balance_f = -1;
            change_bf = false;
            return root;
        }

        if (root->balance_f == -1) {
            //root->balance_f = -2;
            son = root->left;
            if (son->balance_f <= 0) {
                root->left = son->right;
                son->right = root;

                if (son->balance_f == 0) {
                    root->balance_f = -1;
                    son->balance_f = 1;
                    change_bf = false;
                }
                else {
                    root->balance_f = 0;
                    son->balance_f = 0;
                }
                root = son;
            }
            else {
                grandson = son->right;
                son->right = grandson->left;
                grandson->left = son;
                root->left = grandson->right;
                grandson->right = root;

                if (grandson->balance_f == -1) { root->balance_f = 1; }
                else { root->balance_f = 0; }
                if (grandson->balance_f == 1) { son->balance_f = -1; }
                else { son->balance_f = 0; }

                grandson->balance_f = 0;
                root = grandson;
            }
        }
        return root;
    }
   
    node* find_min(node*& right, bool& change_bf) {
        node* min;
        if (right->left != nullptr) {
            min = find_min(right->left, change_bf);
            if (change_bf == true) { right = balanceL(right, change_bf); }
        }
        else {
            min = right;
            right = right->right;
            change_bf = true;
        }
        return min;
    }


    node* insert(node* root, key_info add_key, bool& change_bf) {
        node *son, *grandson;

        if (root == nullptr) {
            key_num++;
            change_bf = true;
            return new node(add_key, key_num);
        }
        if (add_key < root->key) {
            root->left = insert(root->left, add_key, change_bf);
            if (change_bf == true) {
                if (root->balance_f == 1) {
                    root->balance_f = 0;
                    change_bf = false;
                    return root;
                }
                if (root->balance_f == 0) {
                    root->balance_f = -1;
                    return root;
                }
                if (root->balance_f == -1) {
                    //root->balance_f = -2;

                    son = root->left;
                    if (son->balance_f == -1) {
                        root->left = son->right;
                        son->right = root;
                        root->balance_f = 0;
                        root = son;
                    }
                    else {
                        grandson = son->right;
                        son->right = grandson->left;
                        grandson->left = son;

                        root->left = grandson->right;
                        grandson->right = root;

                        if (grandson->balance_f == -1) { root->balance_f = 1; }
                        else { root->balance_f = 0; }
                        if (grandson->balance_f == 1) { son->balance_f = -1; }
                        else { son->balance_f = 0; }
                        root = grandson;
                    }
                    root->balance_f = 0;
                    change_bf = false;
                    return root;
                }
            }
        }
        else if (add_key > root->key) {
            root->right = insert(root->right, add_key, change_bf);
            if (change_bf == true) {
                if (root->balance_f == -1) {
                    root->balance_f = 0;
                    change_bf = false;
                    return root;
                }
                if (root->balance_f == 0) {
                    root->balance_f = 1;
                    return root;
                }
                if (root->balance_f == 1) {
                    //root->balance_f = 2;

                    son = root->right;
                    if (son->balance_f == 1) {
                        root->right = son->left;
                        son->left = root;
                        root->balance_f = 0;
                        root = son;
                    }
                    else {
                        grandson = son->left;
                        son->left = grandson->right;
                        grandson->right = son;

                        root->right = grandson->left;
                        grandson->left = root;

                        if (grandson->balance_f == 1) { root->balance_f = -1; }
                        else { root->balance_f = 0; }
                        if (grandson->balance_f == -1) { son->balance_f = 1; }
                        else { son->balance_f = 0; }
                        root = grandson;
                    }
                    root->balance_f = 0;
                    change_bf = false;
                    return root;
                }
            }
        }
        else {
            key_num++;
            root->value->append(key_num);
            change_bf = false;
            return root;
        }
        return root;
    }

    node* Remove(node* root, key_info del_key, bool& change_bf) {

        if (root == nullptr) {
            change_bf = false;
            return nullptr;
        }
        if (del_key < root->key) {
            root->left = Remove(root->left, del_key, change_bf);
            if (change_bf == true) {
                root = balanceL(root, change_bf);
            }
        }
        else if (del_key > root->key) {
            root->right = Remove(root->right, del_key, change_bf);
            if (change_bf == true) {
                root = balanceR(root, change_bf);
            }
        }
        else { // Случай равенства ключей 
            node* left = root->left;
            node* right = root->right;

            root->value->to_delete_list();
            delete root;
            

            if (left == nullptr) {
                change_bf = true;
                return right;
            }
            if (right == nullptr) {
                change_bf = true;
                return left;
            }

            node* min = find_min(right, change_bf);
            min->left = left;
            min->right = right;
            root = min;
            if (change_bf == true) {
                root = balanceR(root, change_bf);
            }
        }
        return root;
    }


public:
	node* top;
    unsigned int key_num = 0; 
	avl_tree() : top(nullptr) { } 

    node* insert(node* root, key_info add_key) {
        bool change_bf = false;
        return insert(root, add_key, change_bf);
    }

    node* Remove(node* root, key_info del_key) {
        bool change_bf = false;
        return Remove(root, del_key, change_bf);
    }

    list* find(node* root, key_info find_key) {

        if (root == nullptr) {
            return nullptr;
        }
        if (root->key < find_key) {
            return find(root->right, find_key);
        }
        if (root->key > find_key) {
            return find(root->left, find_key);
        }
        return root->value; // Случай равенства ключей
    }

    void Print_tree(const node *root, std::string Rpref = "", std::string Cpref = "", std::string Lpref = "") {
        if (root == nullptr) {
            return;
        }
        if (root->right != nullptr) {
            Print_tree(root->right, Rpref + "  ", Rpref + Up_corner, Rpref + Vertical_line);
        }
        std::cout << Cpref << root->key << " ";
        root->value->print_list();
        std::cout << "\n";

        if (root->left != nullptr) {
            Print_tree(root->left, Lpref + Vertical_line, Lpref + L_corner, Lpref + "  ");
        }
    }

    void Post_Order(node const* root, std::ofstream &outputFile) {

        if (root == nullptr) {
            return;
        }
         Post_Order(root->left, outputFile);
         Post_Order(root->right, outputFile);

         outputFile << root->key << " ";
         root->value->write_in_file(outputFile);
         outputFile << "\n";
    }

    void Clear(node* root) {
        if (root == nullptr) {
            return;
        }
        Clear(root->left);
        Clear(root->right);

        root->value->to_delete_list();
            delete root;
            root = nullptr;
    }

};

void draw_menu() {

	std::cout << "0. Exit \n";
    std::cout << "1. Init \n";
    std::cout << "2. Insert \n";
    std::cout << "3. Delete \n";
    std::cout << "4. Find \n";
    std::cout << "5. Print_tree \n";
    std::cout << "6. Post_order (Write to file) \n";
    std::cout << "7. Clear \n";
    std::cout << "8. ....... \n";
    std::cout << "9. Upload data from file \n";
}



void menu() {

	avl_tree* tree1 = nullptr;
	bool flag_exit = true;
    std::string menu_num_str;
    int menu_num;


	while (flag_exit) {
		draw_menu();

        do {
            std::cout << "Input num: ";
            std::cin >> menu_num_str;
            std::cout << "\n";
        } while (flag_num(menu_num_str) == false);

        menu_num = stoi(menu_num_str);

        switch (menu_num) {
        case 0:
            flag_exit = false;
            break;
        case 1:
            if (tree1 == nullptr) {
                tree1 = new avl_tree();
            }
            else {
                std::cout << "Tree already Init \n";
            }
            break;
        case 2:
            if (tree1 != nullptr) {

                std::string line_key;
                do {
                    std::cout << "Input Line of Key: \n";
                    std::cin >> line_key;
                    std::cout << "\n";
                } while (is_valid_key(line_key) == false);
                
                key_info new_key;
                new_key.string_to_key(line_key);

                tree1->top = tree1->insert(tree1->top, new_key);
            }
            else {
                std::cout << "Tree doesn't Init \n";
            }
            break;
        case 3:
            if (tree1 != nullptr) {
                
                std::string line_key;
                do {
                    std::cout << "Input Line of Key: \n";
                    std::cin >> line_key;
                    std::cout << "\n";
                } while (is_valid_key(line_key) == false);

                key_info del_key;
                del_key.string_to_key(line_key);

                tree1->top = tree1->Remove(tree1->top, del_key);
            }
            else {
                std::cout << "Tree doesn't Init \n";
            }

            break;
        case 4:
            if (tree1 != nullptr) {

                std::string line_key;
                do {
                    std::cout << "Input Line of Key: \n";
                    std::cin >> line_key;
                    std::cout << "\n";
                } while (is_valid_key(line_key) == false);

                key_info find_key;
                find_key.string_to_key(line_key);

                (tree1->find(tree1->top, find_key))->print_list();
                std::cout << "\n";

            }
            else {
                std::cout << "Tree doesn't Init \n";
            }
            break;
        case 5:
            if (tree1 != nullptr) {
                tree1->Print_tree(tree1->top);
                std::cout << "\n";
                
            }
            else {
                std::cout << "Tree doesn't Init \n";
            }
            break;
        case 6:
            if (tree1 != nullptr) {

                std::ofstream File("out.txt");

                if (File.is_open()) {
                    tree1->Post_Order(tree1->top, File);
                }
                File.close();
                std::cout << "File has been written \n";
            }
            else {
                std::cout << "Tree doesn't Init \n";
            }
            break;
        case 7:
            if (tree1 != nullptr) {
                tree1->Clear(tree1->top);
                delete tree1;
                tree1 = nullptr;
                std::cout << "Clear succesful \n";
            }
            else {
                std::cout << "Tree doesn't Init \n";
            }
            break;
        case 8:
            if (tree1 != nullptr) {

            }
            else {
                std::cout << "Tree doesn't Init \n";
            }
            break;
        case 9:
            if (tree1 != nullptr) {
                std::string line;

                std::ifstream file("data.txt"); // окрываем файл для чтения
                if (file.is_open())
                {

                    while (std::getline(file, line))
                    {
                        if (is_valid_key(line) == false) {
                            std::cout << "\n Input error \n";
                            return;
                        }

                        key_info new_key;
                        new_key.string_to_key(line);

                        tree1->top = tree1->insert(tree1->top, new_key);

                    }
                }
                file.close();     // закрываем файл
            }
            else {
                std::cout << "Tree doesn't Init \n";
            }
            
        }
	}


}

int main() {

    menu();

	return 0;
}