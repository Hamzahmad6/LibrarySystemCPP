#include <iostream>
#include <string>
using namespace std;

class MemberList;
class BookBST;

struct MemberNode {
    int id;
    string name;
    string email;
    MemberNode* next;
};

class MemberList {
private:
    MemberNode* head;
public:
    MemberList() {head=nullptr;}

    bool exists(int id) {
        MemberNode* temp = head;
        while (temp) {
            if (temp->id == id) return true;
            temp = temp->next;
        }
        return false;
    }

    void addMember(int id, string name, string email) {
        if (exists(id)) {
            cout<<"Error! Member ID: "<<id<<" Already Exists!"<<endl;
            return;
        }
        MemberNode* newNode = new MemberNode{id, name, email, nullptr};
        if (head==nullptr) {
            head = newNode;
        } else {
            MemberNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        cout <<"Member Added: "<<name<<endl;
    }

    void removeMember(int id) {
        if (head==nullptr) {
            cout<<"List Is Empty."<<endl;
            return;
        }
        if (head->id == id) {
            MemberNode* temp = head;
            head = head->next;
            delete temp;
            cout<<"Member "<<id<<" Removed."<<endl;
            return;
        }
        MemberNode* current = head;
        MemberNode* prev = nullptr;
        while (current && current->id != id) {
            prev = current;
            current = current->next;
        }
        if (!current) {
            cout<<"Member Not Found."<< endl;
            return;
        }
        prev->next = current->next;
        delete current;
        cout << "Member "<<id<<" removed."<< endl;
    }

    void searchMember(int id) {
        MemberNode* temp = head;
        while (temp) {
            if (temp->id == id) {
                cout<<"Found Member: "<<temp->name<<endl;
                cout<<"Email: "<<temp->email<<endl;
                return;
            }
            temp = temp->next;
        }
        cout<<"Member Not Found."<<endl;
    }

    void displayMembers() {
        MemberNode* temp = head;
        cout << "\n---Member List---" <<endl;
        while (temp) {
            cout<<"ID: "<< temp->id <<endl;
            cout<<"Name: "<< temp->name<<endl;
            temp = temp->next;
        }
    }
};

struct BookNode {
    int id;
    string title;
    string author;
    int copies;
    BookNode* left;
    BookNode* right;
};

class BookBST {
private:
    BookNode* root;

    BookNode* insert(BookNode* node, int id, string title, string author, int copies) {
        if (!node) return new BookNode{id, title, author, copies, nullptr, nullptr};
        if (id < node->id) node->left = insert(node->left, id, title, author, copies);
        else if (id > node->id) node->right = insert(node->right, id, title, author, copies);
        return node;
    }

    BookNode* findMin(BookNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    BookNode* remove(BookNode* node, int id) {
        if (!node) return nullptr;
        if (id < node->id) node->left = remove(node->left, id);
        else if (id > node->id) node->right = remove(node->right, id);
        else {
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            }
            if (!node->left) {
                BookNode* temp = node->right;
                delete node;
                return temp;
            }
            if (!node->right) {
                BookNode* temp = node->left;
                delete node;
                return temp;
            }
            BookNode* temp = findMin(node->right);
            node->id = temp->id;
            node->title = temp->title;
            node->author = temp->author;
            node->copies = temp->copies;
            node->right = remove(node->right, temp->id);
        }
        return node;
    }

    BookNode* search(BookNode* node, int id) {
        if (!node || node->id == id) return node;
        if (id < node->id) return search(node->left, id);
        return search(node->right, id);
    }

    void inorder(BookNode* node) {
        if (!node) return;
        inorder(node->left);
        cout<<"ID: "<< node->id<< " | "<<node->title<<endl;
        inorder(node->right);
    }

public:
    BookBST() { root = nullptr; }

    // Helper to check if book exists
    bool exists(int id) {
        return search(root, id) != nullptr;
    }

    void addBook(int id, string title, string author, int copies) {
        if (exists(id)) {
            cout <<"Error: Book ID "<<id<<" Already Exists!"<<endl;
            return;
        }
        root = insert(root, id, title, author, copies);
        cout <<"Book Added: "<<title<< endl;
    }

    void removeBook(int id) {
        if (!exists(id)) {
            cout <<"Error: Book ID "<<id<<" Not Found."<<endl;
            return;
        }
        root = remove(root, id);
        cout<<"Book removed."<<endl;
    }

    void searchBook(int id) {
        BookNode* result = search(root, id);
        if (result) cout <<"Found: "<<result->title<<" By "<<result->author<<endl;
        else cout <<"Book not found!"<<endl;
    }

    void displayBooksSorted() {
        cout<<"\n--- Book Inventory (Sorted by ID) ---"<<endl;
        inorder(root);
    }
};

struct HistoryNode {
    int memberId;
    int bookId;
    string date;
    HistoryNode* prev;
    HistoryNode* next;
};

class BorrowHistory {
private:
    HistoryNode* head;
public:
    BorrowHistory() { head = nullptr; }
    void addRecord(int mId, int bId, string date, MemberList& members, BookBST& books) {
        if (!members.exists(mId)) {
            cout<<"Error: Member ID "<<mId<<" not found. Cannot borrow."<<endl;
            return;
        }
        if (!books.exists(bId)) {
            cout<<"Error: Book ID "<<bId<< " not found. Cannot borrow."<<endl;
            return;
        }

        HistoryNode* newNode = new HistoryNode{mId, bId, date, nullptr, nullptr};
        if (!head) {
            head = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        cout<<"Borrow record added for Member "<<mId<<", Book "<<bId<<endl;
    }
    void viewHistory() {
        HistoryNode* temp = head;
        cout<<"\n---Borrow History---"<<endl;
        if (!temp) {
            cout<<"No borrow history."<<endl;
            return;
        }
        while (temp) {
            cout<<"Member "<<temp->memberId<<" borrowed Book "<<temp->bookId<<" on "<<temp->date<<endl;
            temp = temp->next;
        }
    }

    void viewMemberHistory(int memberId, MemberList& members) {
        if (!members.exists(memberId)) {
            cout<<"Error: Member ID "<<memberId<<" does not exist!"<<endl;
            return;
        }

        HistoryNode* temp = head;
        cout<<"\n--- Borrow History for Member "<<memberId<<" ---"<< endl;
        bool found = false;
        while (temp) {
            if (temp->memberId == memberId) {
                cout<<"Book "<<temp->bookId<<" borrowed on "<<temp->date<<endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout<<"No borrow history for Member "<<memberId<<endl;
        }
    }
};

struct ReturnStackNode {
    int bookId;
    ReturnStackNode* next;
};

class ReturnStack {
private:
    ReturnStackNode* top;
public:
    ReturnStack() { top = nullptr; }

    void pushReturn(int bookId, BookBST& books) {
        if (!books.exists(bookId)) {
             cout<<"Error: Book ID "<<bookId<<" does not belong to this library!"<<endl;
             return;
        }

        ReturnStackNode* newNode = new ReturnStackNode{bookId, top};
        top = newNode;
        cout<<"Book "<<bookId<<" pushed to return stack."<<endl;
    }

    void processReturn() {
        if (!top) {
            cout<<"No returns to process."<<endl;
            return;
        }
        ReturnStackNode* temp = top;
        cout<<"Processing return for Book ID: "<<top->bookId<<endl;
        top = top->next;
        delete temp;
    }
};
struct QueueNode {
    int memberId;
    int bookId;
    QueueNode* next;
};

class ReservationQueue {
private:
    QueueNode *front, *rear;
public:
    ReservationQueue() { front = rear = nullptr; }

    void enqueue(int mId, int bId, MemberList& members, BookBST& books) {
        if (!members.exists(mId)) {
            cout<<"Error: Member ID "<<mId<<" not found. Cannot reserve."<<endl;
            return;
        }
        if (!books.exists(bId)) {
            cout<<"Error: Book ID "<<bId<<" not found. Cannot reserve."<<endl;
            return;
        }

        QueueNode* newNode = new QueueNode{mId, bId, nullptr};
        if (!rear) {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
        cout<<"Reservation added for Member "<<mId<<" Book "<<bId<<endl;
    }

    void dequeue() {
        if (!front) {
            cout<<"No reservations."<<endl;
            return;
        }
        QueueNode* temp = front;
        cout<<"Processing reservation for Member "<<front->memberId<<endl;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
    }
};

int main() {
    MemberList members;
    BookBST books;
    BorrowHistory history;
    ReturnStack returns;
    ReservationQueue reservations;
    int choice;

    while(true){
        cout<<"\n=== LIBRARY SYSTEM MENU ==="<<endl;
        cout<<"1. Add Member"<<endl;
        cout<<"2. Remove Member"<<endl;
        cout<<"3. Search Member"<<endl;
        cout<<"4. Display Members"<<endl;
        cout<<"---------------------------"<<endl;
        cout<<"5. Add Book"<<endl;
        cout<<"6. Remove Book"<<endl;
        cout<<"7. Search Book"<<endl;
        cout<<"8. Display Books"<<endl;
        cout<<"---------------------------"<<endl;
        cout<<"9. Borrow Book"<<endl;
        cout<<"10. View All Borrow History"<<endl;
        cout<<"11. View Member Borrow History"<<endl;
        cout<<"---------------------------"<<endl;
        cout<<"12. Return Book"<<endl;
        cout<<"13. Process Return"<<endl;
        cout<<"---------------------------"<<endl;
        cout<<"14. Reserve Book"<<endl;
        cout<<"15. Process Reservation"<<endl;
        cout<<"0. Exit"<<endl;
        cout<<"Enter choice:";
        cin>>choice;

        int id,id2,copies;
        string s1,s2;

        switch(choice){
            case 1:
                cout<<"ID:";
                cin>>id;
                cout<<"Name:";
                cin.ignore();
                getline(cin,s1);
                cout<<"Email:";
                getline(cin,s2);
                members.addMember(id,s1,s2);
                break;
            case 2:
                cout<<"ID to remove:";
                cin>>id;
                members.removeMember(id);
                break;
            case 3:
                cout<<"ID to search:";
                cin>>id;
                members.searchMember(id);
                break;
            case 4:
                members.displayMembers();
                break;
            case 5:
                cout<<"ID:";
                cin>>id;
                cout<<"Title:";
                cin.ignore();
                getline(cin,s1);
                cout<<"Author:";
                getline(cin,s2);
                cout<<"Copies:";
                cin>>copies;
                books.addBook(id,s1,s2,copies);
                break;
            case 6:
                cout<<"ID to remove:";
                cin>>id;
                books.removeBook(id);
                break;
            case 7:
                cout<<"ID to search:";
                cin>>id;
                books.searchBook(id);
                break;
            case 8:
                books.displayBooksSorted();
                break;
            case 9:
                cout<<"Member ID:";
                cin>>id;
                cout<<"Book ID:";
                cin>>id2;
                history.addRecord(id,id2,"Today",members,books);
                break;
            case 10:
                history.viewHistory();
                break;
            case 11:
                cout<<"Member ID:";
                cin>>id;
                history.viewMemberHistory(id,members);
                break;
            case 12:
                cout<<"Book ID to return:";
                cin>>id;
                returns.pushReturn(id,books);
                break;
            case 13:
                returns.processReturn();
                break;
            case 14:
                cout<<"Member ID:";
                cin>>id;
                cout<<"Book ID:";
                cin>>id2;
                reservations.enqueue(id,id2,members,books);
                break;
            case 15:
                reservations.dequeue();
                break;
            case 0:
                return 0;
            default:
                cout<<"Invalid choice!"<<endl;
        }
    }
}
