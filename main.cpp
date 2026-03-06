#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <algorithm>
using namespace std;


// ========================
// USER CLASS
// ========================
// TEAM MEMBER 1: Implement all methods in this class
class User {
private:
    string username;
    string password;
    string phoneNumber;
    string status;
    string lastSeen;

public:
    User() {
        username = "";
        password = "";
        phoneNumber = "";
        status = "Online";
        lastSeen = "Never";
    }

    User(string uname, string pwd, string phone) {
        username = uname;
        password = pwd;
        phoneNumber = phone;
        status = "Online";
        updateLastSeen();
    }

    string getUsername() const {
        return username;
    }

    string getPhoneNumber() const {
        return phoneNumber;
    }

    string getStatus() const {
        return status;
    }

    string getLastSeen() const {
        return lastSeen;
    }

    void setStatus(string newStatus) {
        status = newStatus;
    }

    void setPhoneNumber(string phone) {
        phoneNumber = phone;
    }

    void updateLastSeen() {
        time_t now = time(0);
        char* dt = ctime(&now);
        lastSeen = string(dt);
    }

    bool checkPassword(string pwd) const {
        return pwd == password;
    }

    void changePassword(string newPwd) {
        password = newPwd;
    }
};

// ========================
// MESSAGE CLASS
// ========================
// TEAM MEMBER 2: Implement all methods in this class
class Message {
private:
    string sender;
    string content;
    string timestamp;
    string status;
    Message* replyTo;

public:
    Message() {
        sender = "";
        content = "";
        timestamp = "";
        status = "sent";
        replyTo = nullptr;
    }

    Message(string sndr, string cntnt) {
        sender = sndr;
        if (cntnt.empty()) {
            content = "Message is empty";
        }
        else {
            content = cntnt;
        }
        status = "sent";
        replyTo = nullptr;
        updateTimestamp();
    }

    string getContent() const {
        return content;
    }

    string getSender() const {
        return sender;
    }

    string getTimestamp() const {
        return timestamp;
    }

    string getStatus() const {
        return status;
    }

    Message* getReplyTo() const {
        return replyTo;
    }

    void setStatus(string newStatus) {
        status = newStatus;
    }

    void setReplyTo(Message* msg) {
        replyTo = msg;
    }

    void updateTimestamp() {
        time_t now = time(0);
        char* dt = ctime(&now);
        timestamp = string(dt);
        timestamp.pop_back(); // Remove newline
    }

    void display() const {
        cout << "[" << sender << " @ " << timestamp << "]: " << content;
        if (replyTo != nullptr) {
            cout << " (Reply to: " << replyTo->getContent() << ")";
        }
        cout << " [" << status << "]" << endl;
    }

    void addEmoji(string emojiCode) {
        if (emojiCode == ":)") {
            content += " 😊";
        }
        else if (emojiCode == ":(") {
            content += " 😞";
        }
        else if (emojiCode == ":D") {
            content += " 😂";
        }
        else if (emojiCode == "<3") {
            content += " ❤️";
        }
        else if (emojiCode == ":thumbsup:") {
            content += " 👍";
        }
    }
};

// ========================
// CHAT CLASS (BASE)
// ========================
// TEAM MEMBER 3: Implement all methods in this class
class Chat {
protected:
    vector<string> participants;
    vector<Message> messages;
    string chatName;

public:
    Chat() {
        chatName = "";
    }

    Chat(vector<string> users, string name) {
        participants = users;
        chatName = name;
    }

    void addMessage(const Message& msg) {
        messages.push_back(msg);
    }

    bool deleteMessage(int index, const string& username) {
        if (index >= 0 && index < messages.size()) {
            // Check if user is sender or admin (simplified)
            if (messages[index].getSender() == username) {
                messages.erase(messages.begin() + index);
                return true;
            }
        }
        return false;
    }

    virtual void displayChat() const {
        cout << "\n=== " << chatName << " ===\n";
        cout << "Participants: ";
        for (const auto& p : participants) {
            cout << p << " ";
        }
        cout << "\n\nMessages:\n";
        for (const auto& msg : messages) {
            msg.display();
        }
    }

    vector<Message> searchMessages(string keyword) const {
        vector<Message> results;
        for (const auto& msg : messages) {
            if (msg.getContent().find(keyword) != string::npos) {
                results.push_back(msg);
            }
        }
        return results;
    }

    void exportToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "Chat: " << chatName << "\n";
            file << "Participants: ";
            for (const auto& p : participants) {
                file << p << " ";
            }
            file << "\n\nMessages:\n";
            for (const auto& msg : messages) {
                file << "[" << msg.getSender() << " @ " << msg.getTimestamp() << "]: "
                     << msg.getContent() << "\n";
            }
            file.close();
            cout << "Chat exported to " << filename << endl;
        }
    }
};

// ========================
// PRIVATE CHAT CLASS
// ========================
// TEAM MEMBER 3: Implement all methods in this class
class PrivateChat : public Chat {
private:
    string user1;
    string user2;

public:
    PrivateChat(string u1, string u2) {
        user1 = u1;
        user2 = u2;
        participants.push_back(u1);
        participants.push_back(u2);  // Fixed: was pushing u1 twice
        chatName = "Chat between " + u1 + " and " + u2;
    }

    void displayChat() const override {
        cout << "\n*** Private Chat: " << user1 << " & " << user2 << " ***\n";
        cout << "Participants: " << user1 << ", " << user2 << "\n\n";
        cout << "Messages:\n";
        for (const auto& msg : messages) {
            msg.display();
        }
    }

    void showTypingIndicator(const string& username) const {
        cout << username << " is typing..." << endl;
    }
};

// ========================
// GROUP CHAT CLASS
// ========================
// TEAM MEMBER 4: Implement all methods in this class
class GroupChat : public Chat {
private:
    vector<string> admins;
    string description;

public:
    GroupChat(vector<string> users, string name, string creator) {
        chatName = name;
        participants = users;
        participants.push_back(creator);  // Add creator to participants
        admins.push_back(creator);
        description = "Group description not set";
    }

    void addAdmin(string newAdmin) {
        if (find(admins.begin(), admins.end(), newAdmin) == admins.end()) {
            admins.push_back(newAdmin);
        }
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        if (!isAdmin(admin)) {
            cout << "Only admins can remove participants.\n";
            return false;
        }

        auto it = find(participants.begin(), participants.end(), userToRemove);
        if (it != participants.end()) {
            participants.erase(it);
            // Also remove from admins if they were an admin
            auto adminIt = find(admins.begin(), admins.end(), userToRemove);
            if (adminIt != admins.end()) {
                admins.erase(adminIt);
            }
            return true;
        }
        return false;
    }

    bool isAdmin(string username) const {
        return find(admins.begin(), admins.end(), username) != admins.end();
    }

    bool isParticipant(string username) const {
        for (string user : participants) {
            if (user == username) {
                return true;
            }
        }
        return false;
    }

    void setDescription(string desc) {
        description = desc;
    }

    void displayChat() const override {
        cout << "\n=== Group: " << chatName << " ===\n";
        cout << "Description: " << description << "\n";
        cout << "Admins: ";
        for (const auto& admin : admins) {
            cout << admin << " ";
        }
        cout << "\nParticipants: ";
        for (const auto& p : participants) {
            cout << p << " ";
        }
        cout << "\n\nMessages:\n";
        for (const auto& msg : messages) {
            msg.display();
        }
    }

    void sendJoinRequest(const string& username) {
        cout << username << " has requested to join " << chatName << endl;
    }
};

// ========================
// WHATSAPP APP CLASS
// ========================
// TEAM MEMBER 1: Implement findUserIndex, isLoggedIn, getCurrentUsername, signUp, login, logout
// TEAM MEMBER 4: Implement startPrivateChat, createGroup, viewChats, and manage integration
class WhatsApp {
private:
    vector<User> users;
    vector<Chat*> chats;
    int currentUserIndex;

    int findUserIndex(string username) const {
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) {
                return i;
            }
        }
        return -1;
    }

    bool isLoggedIn() const {
        return currentUserIndex != -1;
    }

    string getCurrentUsername() const {
        if (currentUserIndex == -1)
            return "";
        return users[currentUserIndex].getUsername();
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    ~WhatsApp() {
        for (auto chat : chats) {
            delete chat;
        }
    }

    void signUp() {
        string username;
        string password;
        string phone;

        cout << "\n--- Sign Up ---\n";
        cout << "Enter username: ";
        cin.ignore();
        getline(cin, username);

        if (findUserIndex(username) != -1) {
            cout << "Username already exists.\n";
            return;
        }

        cout << "Enter password: ";
        getline(cin, password);
        cout << "Enter phone number: ";
        getline(cin, phone);

        User newUser(username, password, phone);
        users.push_back(newUser);
        cout << "Signup successful.\n";
    }

    void login() {
        string username;
        string password;

        cout << "\n--- Login ---\n";
        cout << "Enter username: ";
        cin.ignore();
        getline(cin, username);

        int index = findUserIndex(username);
        if (index == -1) {
            cout << "User not found.\n";
            return;
        }

        cout << "Enter password: ";
        getline(cin, password);

        if (!users[index].checkPassword(password)) {
            cout << "Incorrect password.\n";
            return;
        }

        currentUserIndex = index;
        users[index].setStatus("Online");
        cout << "Login successful. Welcome " << users[index].getUsername() << endl;
    }

    void startPrivateChat() {
        if (!isLoggedIn()) {
            cout << "You must login first." << endl;
            return;
        }

        string username;
        cout << "Enter username to chat with: ";
        cin >> username;

        int index = findUserIndex(username);
        if (index == -1) {
            cout << "User not found." << endl;
            return;
        }

        string currentUser = getCurrentUsername();
        if (username == currentUser) {
            cout << "You cannot chat with yourself." << endl;
            return;
        }

        Chat* newChat = new PrivateChat(currentUser, username);
        chats.push_back(newChat);

        cout << "Private chat created with " << username << endl;

        // Simple message sending demo
        string message;
        cout << "Enter your first message (or 'quit' to skip): ";
        cin.ignore();
        getline(cin, message);
        if (message != "quit") {
            Message msg(getCurrentUsername(), message);
            newChat->addMessage(msg);
        }

        newChat->displayChat();
    }

    void createGroup() {
        if (!isLoggedIn()) {
            cout << "You must login first." << endl;
            return;
        }

        string groupName;
        cout << "Enter group name: ";
        cin >> groupName;

        int numOfPart;
        cout << "Enter the number of participants: ";
        cin >> numOfPart;

        vector<string> members;
        for (int i = 0; i < numOfPart; i++) {
            string username;
            cout << "Enter username " << i + 1 << ": ";
            cin >> username;

            if (findUserIndex(username) == -1) {
                cout << "User not found." << endl;
                i--;
                continue;
            }

            // Check if user is already added
            if (find(members.begin(), members.end(), username) != members.end()) {
                cout << "User already added to group." << endl;
                i--;
                continue;
            }

            members.push_back(username);
        }

        string creator = getCurrentUsername(); //the one that logged in

        // Check if creator is already in members list
        if (find(members.begin(), members.end(), creator) == members.end()) {
            members.push_back(creator);
        }

        Chat* group = new GroupChat(members, groupName, creator);
        chats.push_back(group);
        cout << "Group created successfully!" << endl;
    }

    void viewChats() const {
        if (chats.empty()) {
            cout << "No chats available.\n";
            return;
        }

        cout << "\n=== Your Chats ===\n";
        for (int i = 0; i < chats.size(); i++) {
            cout << i + 1 << ". ";
            chats[i]->displayChat();
            cout << "-------------------\n";
        }
    }

    void logout() {
        if (!isLoggedIn()) {
            cout << "No user is currently logged in.\n";
            return;
        }

        users[currentUserIndex].updateLastSeen();
        users[currentUserIndex].setStatus("Offline");
        cout << "User " << users[currentUserIndex].getUsername() << " logged out.\n";
        currentUserIndex = -1;
    }

    void run() {
        while (true) {
            if (!isLoggedIn()) {
                cout << "\n1. Login\n2. Sign Up\n3. Exit\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) login();
                else if (choice == 2) signUp();
                else if (choice == 3) break;
            }
            else {
                cout << "\n1. Start Private Chat\n2. Create Group\n3. View Chats\n4. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) startPrivateChat();
                else if (choice == 2) createGroup();
                else if (choice == 3) viewChats();
                else if (choice == 4) logout();
            }
        }
    }
};

// ========================
// MAIN
// ========================
// TEAM MEMBER 4: Ensure main function works with integrated code
int main() {
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}