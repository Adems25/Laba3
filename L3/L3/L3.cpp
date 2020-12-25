#include <iostream>
#include <stdexcept>

using namespace std;

class Iterator
{
public:
    virtual int next() = 0;
    virtual bool has_next() = 0;
};

class BiniarySearchTree
{
    class Node
    {
    public:

        int key;
        Node* left;
        Node* right;
        Node* parent;
        
        Node(int Key, Node* Parent=nullptr)
        {
            key = Key;
            parent = Parent;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* head;
    Iterator* dft;
    Iterator* bft;

    void rec_insert(Node* Cur, int key)
    {
        if (key >= Cur->key)
        {
            if (Cur->right == nullptr)
                Cur->right = new Node(key, Cur);
            else
                rec_insert(Cur->right, key);
        }
        else
        {
            if (Cur->left == nullptr)
                Cur->left = new Node(key, Cur);
            else
                rec_insert(Cur->left, key);
        }

    }

    Node* find(Node* Cur, int key)
    {
        if (Cur == nullptr)
            return nullptr;
        else if (Cur->key == key)
            return Cur;
        else if (key > Cur->key)
            return find(Cur->right, key);
        else
            return find(Cur->left, key);
    }

    Node* find_min(Node* Cur)
    {
        if (Cur->left == nullptr)
            return Cur;
        else
            return find_min(Cur->left);
    }

public:

    BiniarySearchTree()
    {
        head = nullptr;
        dft = nullptr;
        bft = nullptr;
    }

    class DFT :public Iterator
    {
        class Stack
        {
        public:
            Node* inf;
            Stack* next;
            Stack(Node* Inf, Stack* Next)
            {
                inf = Inf;
                next = Next;
            }
        };
        Stack* Head;
        Node* Cur;
    public:
        DFT(Node* Start)
        {
            Head = nullptr;
            Cur = Start;
        }

        int next() override
        {
            if (!has_next())
                throw out_of_range("Tree is over\n");
            int ret = Cur->key;
            if (Cur->right != nullptr)
            {
                Stack* New = new Stack(Cur->right, Head);
                Head = New;
            }
            if (Cur->left != nullptr)
            {
                Cur = Cur->left;
            }
            else
            {
                if (Head != nullptr)
                {
                    Cur = Head->inf;
                    Stack* Next;
                    Next = Head->next;
                    delete Head;
                    Head = Next;
                }
                else
                    Cur = nullptr;
            }
            return ret;
        }

        bool has_next() override
        {
            return (Cur != nullptr);
        }
    };

    Iterator* create_dft_iterator()
    {
        if (dft != nullptr)
            delete dft;
        dft = new DFT(head);
        return dft;
    }

    class BFT :public Iterator
    {
        class Queue
        {
        public:
            Node* inf;
            Queue* next;
            Queue(Node* Inf, Queue* Next)
            {
                inf = Inf;
                next = Next;
            }
        };
        Queue* First;
        Queue* Last;
        Node* Cur;
    public:
        BFT(Node* Start)
        {
            First = nullptr;
            Last = nullptr;
            Cur = Start;
        }

        int next() override
        {
            if (!has_next())
                throw out_of_range("Tree is over\n");
            int ret = Cur->key;
            if (Cur->left != nullptr)
            {
                Queue* New = new Queue(Cur->left, Last);
                if (First == nullptr)
                {
                    First = New;
                }
                Last = New;
            }
            if (Cur->right != nullptr)
            {
                Queue* New = new Queue(Cur->right, Last);
                if (First == nullptr)
                {
                    First = New;
                }
                Last = New;
            }
            Queue* CUR = Last;
            if (CUR != nullptr)
            {
                if (CUR == First)
                {
                    Cur = CUR->inf;
                    delete CUR;
                    Last = nullptr;
                    First = nullptr;
                }
                else
                {
                    while (CUR->next != First)
                    {
                        CUR = CUR->next;
                    }
                    Cur = CUR->next->inf;
                    delete First;
                    First = CUR;
                    CUR->next = nullptr;
                }
            }
            else
                Cur = nullptr;
            return ret;
        }

        bool has_next() override
        {
            return (Cur != nullptr);
        }
    };

    Iterator* create_bft_iterator()
    {
        if (bft != nullptr)
            delete bft;
        bft = new BFT(head);
        return bft;
    }

    void insert(int key)
    {
        if (bft != nullptr)
        {
            delete bft;
            bft = nullptr;
        }
        if (dft != nullptr)
        {
            delete dft;
            dft = nullptr;
        }
        if (head == nullptr)
            head = new Node(key);
        else
            rec_insert(head, key);
    }

    bool contains(int key)
    {
        return (find(head, key) != nullptr);
    }

    void remove(int key)
    {
        if (bft != nullptr)
        {
            delete bft;
            bft = nullptr;
        }
        if (dft != nullptr)
        {
            delete dft;
            dft = nullptr;
        }
        Node* Del = find(head, key);
        if (Del == nullptr)
            cout << "fuck\n";
        if (Del == nullptr)
            throw invalid_argument("Element is not included in tree\n");
        if ((Del->left == nullptr) && (Del->right == nullptr))
        {
            if (Del->parent == nullptr)
                head = nullptr;
            else if (Del->parent->left == Del)
                Del->parent->left = nullptr;
            else
                Del->parent->right = nullptr;
            delete Del;
        }
        else if ((Del->left == nullptr) && (Del->right != nullptr))
        {
            if (Del->parent == nullptr)
            {
                head = Del->right;
                Del->right->parent = nullptr;
            }
            else if (Del->parent->left == Del)
            {
                Del->parent->left = Del->right;
                Del->right->parent = Del->parent;
            }
            else
            {
                Del->parent->right = Del->right;
                Del->right->parent = Del->parent;
            }
            delete Del;
        }
        else if ((Del->left != nullptr) && (Del->right == nullptr))
        {
            if (Del->parent == nullptr)
            {
                head = Del->left;
                Del->left->parent = nullptr;
            }
            else if (Del->parent->left == Del)
            {
                Del->parent->left = Del->left;
                Del->left->parent = Del->parent;
            }
            else
            {
                Del->parent->right = Del->left;
                Del->left->parent = Del->parent;
            }
            delete Del;
        }
        else if ((Del->left != nullptr) && (Del->right != nullptr))
        {
            Node* Min = find_min(Del->right);
            if (Del->parent == nullptr)
                head = Min;
            if (Min->parent->left == Min)
                Min->parent->left = Min->right;
            else
                Min->parent->right = Min->right;
            if(Min->right!=nullptr)
                Min->right->parent = Min->parent;
            Min->parent = Del->parent;
            if (Min->parent != nullptr)
            {
                if (Min->parent->right == Del)
                    Min->parent->right = Min;
                else
                    Min->parent->left = Min;
            }
            Min->left = Del->left;
            Min->right = Del->right;
            Min->left->parent = Min;
            if (Min->right != nullptr)
                Min->right->parent = Min;
            delete Del;
        }
    }

    ~BiniarySearchTree()
    {
        if (bft != nullptr)
        {
            delete bft;
            bft = nullptr;
        }
        if (dft != nullptr)
        {
            delete dft;
            dft = nullptr;
        }
        while (head != nullptr)
            remove(head->key);
    }
};

int main()
{
    /*
    Tree:
          8
       4     12
      2 6   9  14
    */
    //bft: 8 4 12 2 6 9 14
    //dft: 8 4 2 6 12 9 14
    BiniarySearchTree T;
    T.insert(8);
    T.insert(4);
    T.insert(12);
    T.insert(2);
    T.insert(6);
    T.insert(9);
    T.insert(14);
    Iterator* B = T.create_bft_iterator();
    Iterator* D = T.create_dft_iterator();
    while (B->has_next())
    {
        cout << B->next();
        cout << ' ';
    }
    cout << endl;
    while (D->has_next())
    {
        cout << D->next();
        cout << ' ';
    }
    cout << endl;
}

