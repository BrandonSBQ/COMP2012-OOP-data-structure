#include "bst.h"
using namespace std;

/**
 * Returns true if "src" contains *any* words in "filterWords" (case-insensitive).
 * e.g.: 
 *   containAnyWords("Example string", "example") == true
 *   containAnyWords("Example string", "amp") == true
 *   containAnyWords("Example string", "this is a long sentence") == true  [contains 'a']
 *   containAnyWords("Example string", "no occurence") == false
*/
bool containAnyWords(string src, string filterWords) {
    auto strToLower = [](string& str) {
        for (char& c: str) c = tolower(c);
    };
    strToLower(src);
    strToLower(filterWords);
    size_t found = filterWords.find(' ');
    while (found != string::npos) {
        if (src.find(filterWords.substr(0, found)) != string::npos)
            return true;
        filterWords = filterWords.substr(found+1);
        found = filterWords.find(' ');
    }
    return src.find(filterWords) != string::npos;
}


// TASK 2.1: Filter::match(const Animal&) const
// Returns true if the animal satisfies ALL 3 conditions:
// - species (including breed) contains *any* of the words in speciesMatch
// - health description contains *any* of the words in healthMatch
// - vaccine status contains *all* of the non-empty strings in vaccineMatch
bool Filter::match(const Animal& a) const
{
    if(!containAnyWords(a.getSpecies(),speciesFilter)){
        return false;
    }

    if(!containAnyWords(a.getHealthCondition().description, healthFilter)){
        return false;
    }

    for (int i = 0; i < VACCINE_TABLE_SIZE; ++i) {
            if (!a.getVaccinationStatus().hasVaccine(vaccineFilter[i])) {
                return false;
            }
        }
    
    return true;
}

// TASK 2.2: AnimalLLnode::print(unsigned int&, unsigned int&, const Filter&) const
// Print the animals in this linked list if it matches with the filter.
// The linked list should be maintained such that it is in decreasing ID order.
//
// E.g. (each node is shown as ID[Name]):
//      7[Cat] -> 6[Cat] -> 5[Dog] -> 3[Cat] -> 1[Bird] -> 0[Cat]
// should be printed in the following order using Filter{Name = "Cat"}: 
//      0[Cat]
//      3[Cat]
//      6[Cat]
//      7[Cat]
//
// The parameters ignoreCount and displayCount should be passed to the animal's display() function
void AnimalLLnode::print(unsigned int& ignoreCount, unsigned int& displayCount, const Filter& filter) const
{
    if(next){
        next->print(ignoreCount,displayCount,filter);
    }

    if(filter.match(*animal)){
        animal->display(ignoreCount,displayCount);
    }
}

// TASK 2.3: BSTnode destructor
BSTnode::~BSTnode()  {
    AnimalLLnode* current = head;
    while(current != nullptr){
        AnimalLLnode* next = current->next;
        delete current;
        current = next;
    }
}

// TASK 2.4: BSTnode::addAnimal(const Animal* a)
// Add an animal to the linked list.
// Ensure the order is *decreasing ID*, e.g.: 7[Cat] -> 6[Cat] -> 5[Dog] -> 3[Cat] -> 1[Bird] -> 0[Cat]
// You may assume no two animals with the same ID will be added to a node
// (this also means you should not add the same animal to a node twice!)
// void BSTnode::addAnimal(const Animal* a) {
//     AnimalLLnode* newNode = new AnimalLLnode(a);
    
//     if(head == nullptr || a->getID() > head->animal->getID()){
//         newNode->next = head;
//         head = newNode;
//     }else{
//         AnimalLLnode* current = head;
//         while(current->next != nullptr && current->next->animal->getID() > a->getID()){
//             current = current->next;
//         }

//         newNode->next = current->next;
//         current->next = newNode;
//     }
// }
void BSTnode::addAnimal(const Animal* a) {
    AnimalLLnode* newNode = new AnimalLLnode(a);
    AnimalLLnode* current = head;
    AnimalLLnode* prev = nullptr;

    while (current != nullptr && current->animal->getID() > a->getID()) {
        prev = current;
        current = current->next;
    }

    if (prev == nullptr) {
        // Insert at the beginning of the list
        newNode->next = head;
        head = newNode;
    } else {
        // Insert after the previous node
        newNode->next = prev->next;
        prev->next = newNode;
    }
}

// TASK 2.5: BSTnode::addAnimal(const Animal* a)
// Remove an animal from the linked list.
// Ensure the order of the other animals are kept.
// If the animal does not exist, do nothing.
// If there are no animals left after removing, set head to nullptr.
// void BSTnode::removeAnimal(const Animal* a) {
//     AnimalLLnode** current = &head;
//     while (*current) {
//         if ((*current)->animal == a) {
//             // Found the animal to remove
//             AnimalLLnode* nodeToRemove = *current;
//             *current = (*current)->next; // Bypass the node being removed
//             delete nodeToRemove; // Delete the node
//             return;
//         } else {
//             current = &((*current)->next);
//         }
//     }
//     // No need to set head to nullptr here, as it's already either nullptr or pointing to the next node
// }
void BSTnode::removeAnimal(const Animal* a) {
    AnimalLLnode* current = head;
    AnimalLLnode* prev = nullptr;

    while (current != nullptr && current->animal != a) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        return;
    }

    if (prev == nullptr) {
        head = current->next;
    } else {
        prev->next = current->next;
    }

    delete current;

    if (head == nullptr) {
        head = nullptr;
    }
}


// TASK 3.1: BST destructor
BST::~BST() {
    delete root;
}

// TASK 3.2: BST::findMinNode()
// Optional task, but may be needed for BST::remove().
// Return a reference to the BSTnode* of the min node in this BST.

BSTnode*& BST::findMinNode() {
    // BSTnode** node = &root;  
    // while ((*node)->left.root != nullptr) { 
    //     node = &((*node)->left.root);
    // }
    // return *node;  
    if(root->left.root){
        return root->left.findMinNode();
    }else{
        return root;
    }
}
// TASK 3.3: BST::insert(const Animal* a)
// Insert an animal 'a' to the BST.
// Use the comparator "data member function" to compare the animal with the current node:
// - If 'a' is "less than" the current node, insert it to the left subtree.
// - If 'a' is "more than" the current node, insert it to the right subtree.
// - If 'a' "equals" the current node, insert it into this node's linked list.
// - Otherwise, if the node is empty, create a new node using 'a'.
// void BST::insert(const Animal* a)
// {
//     BSTnode** current = &root;

//     while (*current != nullptr) {
//         int compareResult = comparator(a, (*current)->head->animal);

//         if (compareResult == 0) {
//             // Animal matches the value at the current node, add to the linked list
//             (*current)->addAnimal(a);
//             return;
//         } else if (compareResult < 0) {
//             // Animal is less than the current node's animal, go left
//             current = &((*current)->left.root);
//         } else {
//             // Animal is greater than the current node's animal, go right
//             current = &((*current)->right.root);
//         }
//     }

//     // If we reach a nullptr, it means we've found our insertion point
//     *current = new BSTnode(a, comparator);
// }
void BST::insert(const Animal* a){
    if(root == nullptr){
        root = new BSTnode(a,comparator);
    }else{
        if(comparator(a,root->head->animal) == 0){
            root->addAnimal(a);
        }else if(comparator(a,root->head->animal)<0){
            root->left.insert(a);
        }else{
            root->right.insert(a);
        }
    }
}

// void BST::insert(const Animal* a) {
//     if (root == nullptr) {
//         // BST is empty, create a new node as the root
//         root = new BSTnode(a, comparator);
//     } else {
//         BSTnode* current = root;
//         while (true) {
//             int compareResult = comparator(a, current->head->animal);
//             if (compareResult < 0) {
//                 // 'a' is less than the current node, insert it to the left subtree
//                 if (current->left.root == nullptr) {
//                     current->left.root = new BSTnode(a, comparator);
//                     break;
//                 } else {
//                     current = current->left.root;
//                 }
//             } else if (compareResult > 0) {
//                 // 'a' is more than the current node, insert it to the right subtree
//                 if (current->right.root == nullptr) {
//                     current->right.root = new BSTnode(a, comparator);
//                     break;
//                 } else {
//                     current = current->right.root;
//                 }
//             } else {
//                 // 'a' equals the current node, insert it into the linked list
//                 current->addAnimal(a);
//                 break;
//             }
//         }
//     }
// }

// TASK 3.4: BST::remove(const Animal* a)
// Remove an animal 'a' from the BST
// Follow the same steps in BST::insert() to locate the node to remove.
// Removal strategy is similar to lecture notes example with a few differences:
// - If the node is not found, do nothing.
// - If the node is found, remove the animal from its linked list. If it makes the linked list become empty,
//   remove the node:
//   + If the node contains 0 or 1 child, move the children node to current root, and deallocate the old root.
//   + Else, *move* the linked list from the right subtree's min node to current root, and deallocate right subtree's min node.



void BST::remove(const Animal* a){
    if(isEmpty()){
        return;
    }

    if(comparator(a,root->head->animal)<0){
        root->left.remove(a);
    }else if(comparator(a,root->head->animal)>0){
        root->right.remove(a);
    }// }else{
    //     root->removeAnimal(a);
    //     if(root->head == nullptr){
    //         if(root->left.root && root->right.root){
    //             root->head = root->right.findMinNode()->head;
    //             root->right.findMinNode()->head = nullptr;
    //             BSTnode* temp = root->right.findMinNode();
    //             root->right.findMinNode() = (root->right.findMinNode())->right.root;
    //             temp->right.root = nullptr;
    //             delete temp;
    //         }else{
    //             BSTnode* dele_n = root;
    //             root = (root->left.isEmpty())?root->right.root:root->left.root;
    //             dele_n->left.root = dele_n->right.root = nullptr;
    //             delete dele_n;
    //         }
    //     }
    // }
    this->root->removeAnimal(a);

    if(root->head){
        return;
    }

    else if(root->left.root && root->right.root){
        BSTnode* copy = this->root->right.findMinNode();
        int count = 0;

        for(AnimalLLnode* copying = copy->head; copying; copying = copying->next){
            this->root->addAnimal(copying->animal);
            count++;
        }

        for(AnimalLLnode* deleting = this->root->head; count != 0; deleting = deleting->next){
            this->root->right.remove(deleting->animal);
            count--;
        }
    }
}





// TASK 3.5: BST::print(unsigned int&, unsigned int&, const Filter&) const
// Print the BST using in-order traversal.
//
// E.g.: Consider a BST containing animals sorted by species name:
//                      8[Reptile, Healthy] -> 6[Reptile, Bad]
//              5[Rabbit, Critical]
// 7[Dog, Healthy] -> 3[Dog, Healthy]
//                      9[Cat, Bad] -> 4[Cat, Healthy] -> 2[Cat, Very poor]
//              1[Bird, Healthy]
// 
// should print in the following order using Filter{Health = "Healthy"}:
//      1[Bird, Healthy]
//      4[Cat, Healthy]
//      3[Dog, Healthy]
//      7[Dog, Healthy]
//      8[Reptile, Healthy]
void BST::print(unsigned int& ignoreCount, unsigned int& displayCount, const Filter& filter) const {
    // In-order traversal: left, node, right
    if (root == nullptr) {
        return; // Base case: if the current subtree is empty, return.
    }

    // First, visit the left subtree
    root->left.print(ignoreCount, displayCount, filter);


    // Process the current node's linked list of animals
    // The linked list is in decreasing ID order; we need to reverse the order as we print
    // This is done by recursively calling print on the next node before printing the current node's animal
    if (root->head != nullptr) {
        root->head->print(ignoreCount, displayCount, filter);
    }

   
    root->right.print(ignoreCount, displayCount, filter);
}
