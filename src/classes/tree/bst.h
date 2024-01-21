#ifdef __cplusplus
#include <functional>
#include <queue>
#include <vector>
#include <string>
#include <type_traits>
#include "../../visualization/tree_visual/tree_visualization.h"
#endif

template <typename T> class bst {
public:
  explicit bst(std::vector<T> __elements = {}) noexcept : root(nullptr) {
    if (!__elements.empty()) {
      for (T &x : __elements) {
        this->insert(x);
      }
    }
  }
  ~bst() noexcept {}

  void insert(T key) { root = __insert(root, key); }

  void remove(T key) { root = __remove(root, key); }

  std::vector<std::vector<T>> level_order() {
    std::vector<std::vector<T>> levels;
    if (!root) {
      return levels;
    }
    std::queue<T> q;
    q.push(root);
    while (!q.empty()) {
      int64_t size = q.size();
      std::vector<T> row;
      for (int64_t i = 0; i < size; i++) {
        T current = q.front();
        row.push_back(current);
        q.pop();
        if (current->right) {
          q.push(current->right);
        }
        if (current->left) {
          q.push(current->left);
        }
      }
      levels.push_back(row);
    }
    return levels;
  }

  std::vector<T> inorder() {
    std::vector<T> path;
    __inorder([&](node *callbacked) { path.push_back(callbacked->info); },
              root);
    return path;
  }

  std::vector<T> preorder() {
    std::vector<T> path;
    __preorder([&](node *callbacked) { path.push_back(callbacked->info); },
               root);
    return path;
  }

  std::vector<T> postorder() {
    std::vector<T> path;
    __postorder([&](node *callbacked) { path.push_back(callbacked->info); },
                root);
    return path;
  }

  void visualize(){
    std::string __generated = generate_visualization();
    visualization::visualize(__generated);
  }

private:
  struct node {
    T info;
    node *right, *left;
  };
  node *root;
  node *new_node(T &key) {
    node *p = new node;
    p->info = key;
    p->right = p->left = nullptr;
    return p;
  }

  node *__insert(node *root, T &key) {
    if (!root) {
      return new_node(key);
    } else {
      if (root->info < key) {
        root->right = __insert(root->right, key);
      } else {
        root->left = __insert(root->left, key);
      }
    }
    return root;
  }

  node *__remove(node *root, T &key) {
    if (!root) {
      return root;
    }

    if (root->info < key) {
      root->right = __remove(root->right, key);
    } else if (root->info > key) {
      root->left = __remove(root->left, key);
    } else {
      if (!root->left && !root->right) {
        free(root);
        root = nullptr;
      } else if (!root->left) {
        node *temp = root->right;
        free(root);
        return temp;
      } else if (!root->right) {
        node *temp = root->left;
        free(root);
        return temp;
      } else {
        node *temp = root->right;
        while (temp->left) {
          temp = temp->left;
        }
        root->info = temp->info;
        root->right = __remove(root->right, temp->info);
      }
    }
    return root;
  }

  void __inorder(std::function<void(node *)> callback, node *root) {
    if (root) {
      __inorder(callback, root->left);
      callback(root);
      __inorder(callback, root->right);
    }
  }

  void __postorder(std::function<void(node *)> callback, node *root) {
    if (root) {
      __inorder(callback, root->left);
      __inorder(callback, root->right);
      callback(root);
    }
  }

  void __preorder(std::function<void(node *)> callback, node *root) {
    if (root) {
      callback(root);
      __inorder(callback, root->left);
      __inorder(callback, root->right);
    }
  }

  std::string generate_visualization(){
    std::string __generate = __inorder_gen(root);
    return __generate;
  }

  std::string __inorder_gen(node *root){
    std::string __s;
    if(std::is_same_v<T, char> || std::is_same_v<T, std::string>){
      if(root -> left){
        __s += root->info;
        __s +=  "->";
        __s += root->left->info;
        __s += "\n";
        __s +=  __inorder_gen(root->left);
      }
      if(root -> right){
        __s += root->info;
        __s += "->"; 
        __s += root->right->info;
        __s += "\n";
        __s += __inorder_gen(root->right);
      }
    }
    else{
      if(root -> left){__s += std::to_string(root->info) + "->" + std::to_string(root->left->info) + "\n" + __inorder_gen(root->left);}
      if(root -> right){__s += std::to_string(root->info) + "->" + std::to_string(root->right->info) + "\n" + __inorder_gen(root->right);}
    }
    return __s;
  }
};