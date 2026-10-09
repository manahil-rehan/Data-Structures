#include <iostream>
#include <string>
using namespace std;

struct Message
{
    int id;
    string role;
    string text;
    Message *prev;
    Message *next;

    Message(int _id, string _role, string _text)
    {
        id = _id;
        role = _role;
        text = _text;
        prev = nullptr;
        next = nullptr;
    }
};

class ChatHistory
{
private:
    Message *head;
    Message *tail;
    int count;

public:
    ChatHistory()
    {
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    ~ChatHistory()
    {
        // TODO: properly delete the Message Data Structure
        Message *cur = head;
        while (cur != nullptr) {
            Message *nextNode = cur->next;
            delete cur;
            cur = nextNode;
        }
    }

    int size() { return count; }

    void addMessage(int id, string role, string text)
    {
        // TODO: create a new Message node and append it at the tail.
        
        Message *message = new Message(id, role, text);
        //if list is empty, insert at head
        if (head == nullptr) {
          head = message;
          tail = message;
        }
        //else insert at the end
        else {
            message->prev = tail;
            tail->next = message;
            tail = message;
        }
        //count upgradation
        count++;
    }

    Message *findById(int id)
    {
        // TODO: linear search from head, return node pointer or nullptr.
        Message *temp = head;
        //traverses through the whole list to check 
        //if a message node matches the required id
        //terminates if reaches the end
        while (temp != nullptr) {
            //return the required message if id matches required
            if (temp->id == id)
                return temp;
            //loop iterator
            temp = temp->next;
        }
        //if nothing matches return nullptr
        return nullptr;
    }

    void deleteMessage(int id)
    {
        // TODO: find the node by id, then relink prev/next around it.
        Message *temp = head;
        
        if (head == nullptr) {
            cout << "List is empty!!" << endl;
            return;
        }
        //traverses
        while (temp != nullptr) {
            //if id matches the req id
            if (temp->id == id) {
                //if its the only node
                if (temp == head && temp == tail) {
                    head = nullptr;
                    tail = nullptr;
                    delete temp;
                    count--;
                    break;
                }
                //if its the head
                else if (temp == head) {
                    temp->next->prev = nullptr;
                    head = temp-> next;
                    delete temp;
                    count--;
                    return;
                }
                //if its the tail
                else if(temp == tail) {
                    temp->prev->next = nullptr;
                    tail = temp->prev;
                    delete temp;
                    count--;
                    return;
                }
                //if its any middle node/message
                else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                    delete temp;
                    count--;
                    return;
                }
            }
            
            temp = temp->next;
        }    
    }

    void editMessage(int id, string newText)
    {
        // TODO: find the node by id and overwrite its text field.
        Message *temp = findById(id);
        if (temp == nullptr) {
            cout << "Message not found!!!" << endl;
        }
        else
            temp->text = newText;
    }

    void evictOldestIfOverLimit(int maxMessages)
    {
        // TODO: while count > maxMessages, remove the head node
        //if no of messages exceed max
        while (count > maxMessages) {
            //delete head till count is less than messages
            Message* temp = head;
            //if heads the only node
            if (head == tail) {
                head = nullptr;
                tail = nullptr;
            }
            else {
                head = temp->next;
                head->prev = nullptr;
            }
            delete temp;
            //count decreased
            count--;      
        }
    }

    void printConversation()
    {
        Message *cur = head;
        while (cur != nullptr)
        {
            cout << "[" << cur->id << "] " << cur->role << ": " << cur->text << endl;
            cur = cur->next;
        }
    }
};

int main()
{
    ChatHistory chat;
    string cmd;
    cout << "Enter a command:\n- ADD/DELETE/EDIT/LIMIT/PRINT\n- EXIT to stop." << endl;
    while (cin >> cmd)
    {
        if (cmd == "ADD")
        {
            int id;
            string role, text;
            cin >> id >> role;
            cin.ignore();
            getline(cin, text);
            chat.addMessage(id, role, text);
        }
        else if (cmd == "DELETE")
        {
            int id;
            cin >> id;
            chat.deleteMessage(id);
        }
        else if (cmd == "EDIT")
        {
            int id;
            string newText;
            cin >> id;
            cin.ignore();
            getline(cin, newText);
            chat.editMessage(id, newText);
        }
        else if (cmd == "LIMIT")
        {
            int n;
            cin >> n;
            chat.evictOldestIfOverLimit(n);
            chat.printConversation();
        }
        else if (cmd == "PRINT")
        {
            chat.printConversation();
        }
        else if (cmd == "EXIT")
        {
            break;;
        }
        // TODO (optional): handle malformed/unknown commands gracefully.
        else {
            cout << "UNKNOWN COMMAND -_- -_-" << endl;
        }
    }

    return 0;
}
