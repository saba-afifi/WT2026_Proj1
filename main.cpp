#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <sstream>
using namespace std;

// ========================
//       USER CLASS
// ========================
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
        status = "Offline";
        updateLastSeen();
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
        updateLastSeen();
    }

    void setPhoneNumber(string phone) {
        phoneNumber = phone;
        updateLastSeen();
    }

    void updateLastSeen() {
        time_t now = time(0);
        lastSeen = ctime(&now);
        lastSeen.pop_back(); // Remove newline
    }

    bool checkPassword(string pwd) const {
        return password == pwd;
    }

    void changePassword(string newPwd) {
        if (newPwd.length() >= 6) {
            password = newPwd;
            updateLastSeen();
        }
    }
};

// ========================
//      MESSAGE CLASS
// ========================
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
        status = "sent";
        replyTo = nullptr;
        updateTimestamp();
    }

    Message(string sndr, string cntnt) {
        sender = sndr;
        content = cntnt;
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
        timestamp = ctime(&now);
        timestamp.pop_back(); // Remove newline
    }

    void display() const {
        cout << "[" << timestamp << "] " << sender << ": ";

        if (replyTo) {
            cout << "[Replying to: " << replyTo->getSender() << ": "
                 << replyTo->getContent().substr(0, 20) << "...] ";
        }

        cout << content << " (" << status << ")" << endl;
    }

    void addEmoji(string emojiCode) {
        if (emojiCode==":)") {
            content+="😊";
        }
        else if (emojiCode==":(") {
            content+="😞";
        }
        else if (emojiCode==":D") {
            content+="😂";
        }
        else if (emojiCode=="<3") {
            content+="❤️";
        }
        else if (emojiCode==":thumpsup:") {
            content+="👍";
        }
    }
};

// ========================
//       CHAT CLASS (BASE)
// ========================
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
            if (messages[index].getSender() == username) {
                messages.erase(messages.begin() + index);
                return true;
            }
        }
        return false;
    }

    virtual void displayChat() const {
        cout << "=== " << chatName << " ===" << endl;
        cout << "Participants: ";
        for (const auto& p : participants) {
            cout << p << " ";
        }
        cout << "\n" << endl;

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

    string getChatName() const {
        return chatName;
    }

    vector<string> getParticipants() const {
        return participants;
    }
};

// ========================
//     PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat {
private:
    string user1;
    string user2;

public:
    PrivateChat(string u1, string u2) : Chat({u1, u2}, "Chat between " + u1 + " and " + u2) {
        user1 = u1;
        user2 = u2;
    }

    void displayChat() const override {
        cout << "\n=== Private Chat ===" << endl;
        cout << user1 << " and " << user2 << endl;
        cout << "===================" << endl;

        if (messages.empty()) {
            cout << "No messages yet." << endl;
        } else {
            for (const auto& msg : messages) {
                msg.display();
            }
        }
    }

    void showTypingIndicator(const string& username) const {
        cout << "[" << username << " is typing...]" << endl;
    }
};

// ========================
//      GROUP CHAT CLASS
// ========================
class GroupChat : public Chat {
private:
    vector<string> admins;
    string description;

public:
    GroupChat(vector<string> users, string name, string creator) : Chat(users, name) {
        admins.push_back(creator);
        description = "";
    }

    void addAdmin(string newAdmin) {
        if (isParticipant(newAdmin) && find(admins.begin(), admins.end(), newAdmin) == admins.end()) {
            admins.push_back(newAdmin);
            cout << newAdmin << " is now an admin." << endl;
        }
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        if (isAdmin(admin)) {
            auto it = find(participants.begin(), participants.end(), userToRemove);
            if (it != participants.end()) {
                participants.erase(it);

                // Also remove from admins if they were admin
                auto adminIt = find(admins.begin(), admins.end(), userToRemove);
                if (adminIt != admins.end()) {
                    admins.erase(adminIt);
                }
                cout << userToRemove << " removed from group." << endl;
                return true;
            }
        }
        return false;
    }

    bool isAdmin(string username) const {
        return find(admins.begin(), admins.end(), username) != admins.end();
    }

    bool isParticipant(string username) const {
        return find(participants.begin(), participants.end(), username) != participants.end();
    }

    void setDescription(string desc) {
        description = desc;
    }

    void displayChat() const override {
        cout << "\n=== Group: " << chatName << " ===" << endl;
        if (!description.empty()) {
            cout << "Description: " << description << endl;
        }

        cout << "Admins: ";
        if (admins.empty()) {
            cout << "None";
        } else {
            for (const auto& admin : admins) {
                cout << admin << " ";
            }
        }

        cout << "\nParticipants: ";
        for (const auto& p : participants) {
            cout << p << " ";
        }
        cout << "\n" << endl;

        if (messages.empty()) {
            cout << "No messages yet." << endl;
        } else {
            for (const auto& msg : messages) {
                msg.display();
            }
        }
    }

    void sendJoinRequest(const string& username) {
        cout << username << " requested to join group " << chatName << endl;
    }
};

// ========================
//    WHATSAPP APP CLASS
// ========================
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
        if (isLoggedIn()) {
            return users[currentUserIndex].getUsername();
        }
        return "";
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    ~WhatsApp() {
        // Clean up dynamically allocated chats
        for (Chat* chat : chats) {
            delete chat;
        }
    }

    void signUp() {
        string username, password, phone;

        cout << "Enter username: ";
        cin >> username;

        // Check if username exists
        if (findUserIndex(username) != -1) {
            cout << "Username already exists!" << endl;
            return;
        }

        cout << "Enter password (min 6 characters): ";
        cin >> password;

        if (password.length() < 6) {
            cout << "Password must be at least 6 characters!" << endl;
            return;
        }

        cout << "Enter phone number: ";
        cin >> phone;

        users.push_back(User(username, password, phone));
        cout << "User created successfully!" << endl;
    }

    void login() {
        string username, password;

        cout << "Enter username: ";
        cin >> username;

        int index = findUserIndex(username);
        if (index == -1) {
            cout << "User not found!" << endl;
            return;
        }

        cout << "Enter password: ";
        cin >> password;

        if (users[index].checkPassword(password)) {
            currentUserIndex = index;
            users[index].setStatus("Online");
            cout << "Login successful! Welcome " << username << endl;
        } else {
            cout << "Incorrect password!" << endl;
        }
    }

    void startPrivateChat() {
        if (!isLoggedIn()) return;

        string otherUser;
        cout << "Enter username to chat with: ";
        cin >> otherUser;

        int otherIndex = findUserIndex(otherUser);
        if (otherIndex == -1) {
            cout << "User not found!" << endl;
            return;
        }

        if (otherUser == getCurrentUsername()) {
            cout << "You cannot chat with yourself!" << endl;
            return;
        }

        // Check if chat already exists
        for (Chat* chat : chats) {
            PrivateChat* priv = dynamic_cast<PrivateChat*>(chat);
            if (priv) {
                vector<string> parts = priv->getParticipants();
                if ((parts[0] == getCurrentUsername() && parts[1] == otherUser) ||
                    (parts[0] == otherUser && parts[1] == getCurrentUsername())) {
                    cout << "Chat already exists!" << endl;
                    enterChat(priv);
                    return;
                }
            }
        }

        // Create new private chat
        PrivateChat* chat = new PrivateChat(getCurrentUsername(), otherUser);
        chats.push_back(chat);

        cout << "Private chat started with " << otherUser << endl;
        enterChat(chat);
    }

    void enterChat(Chat* chat) {
        string message;
        cout << "\nEntering chat: " << chat->getChatName() << endl;
        cout << "Commands: type 'exit' to leave, 'emoji' to add emoji, 'typing' to show typing" << endl;
        cout << "Emoji codes: :) :( :D <3 :thumpsup:" << endl;

        while (true) {
            cout << "\nYou: ";
            cin.ignore();
            getline(cin, message);

            if (message == "exit") {
                break;
            }
            else if (message == "typing") {
                PrivateChat* priv = dynamic_cast<PrivateChat*>(chat);
                if (priv) {
                    priv->showTypingIndicator(getCurrentUsername());
                } else {
                    cout << "[" << getCurrentUsername() << " is typing...]" << endl;
                }
            }
            else if (message == "emoji") {
                cout << "Enter emoji code: ";
                string emojiCode;
                getline(cin, emojiCode);

                Message msg(getCurrentUsername(), "");
                msg.addEmoji(emojiCode);
                chat->addMessage(msg);
                chat->displayChat();
            }
            else if (message == "admins" && dynamic_cast<GroupChat*>(chat)) {
                GroupChat* group = dynamic_cast<GroupChat*>(chat);
                if (group->isAdmin(getCurrentUsername())) {
                    cout << "Enter username to make admin: ";
                    string newAdmin;
                    getline(cin, newAdmin);
                    if (findUserIndex(newAdmin) != -1) {
                        group->addAdmin(newAdmin);
                    } else {
                        cout << "User not found!" << endl;
                    }
                } else {
                    cout << "Only admins can add admins!" << endl;
                }
            }
            else if (message == "remove" && dynamic_cast<GroupChat*>(chat)) {
                GroupChat* group = dynamic_cast<GroupChat*>(chat);
                if (group->isAdmin(getCurrentUsername())) {
                    cout << "Enter username to remove: ";
                    string userToRemove;
                    getline(cin, userToRemove);
                    group->removeParticipant(getCurrentUsername(), userToRemove);
                } else {
                    cout << "Only admins can remove participants!" << endl;
                }
            }
            else {
                // Regular message
                Message msg(getCurrentUsername(), message);
                chat->addMessage(msg);
                chat->displayChat();
            }
        }
    }

    void createGroup() {
        if (!isLoggedIn()) return;

        string groupName;
        cout << "Enter group name: ";
        cin.ignore();
        getline(cin, groupName);

        vector<string> members;
        members.push_back(getCurrentUsername());

        cout << "Add members (type 'done' to finish):" << endl;
        while (true) {
            string member;
            cout << "Enter username: ";
            cin >> member;

            if (member == "done") break;

            if (findUserIndex(member) != -1) {
                if (find(members.begin(), members.end(), member) == members.end()) {
                    members.push_back(member);
                    cout << member << " added to group" << endl;
                } else {
                    cout << member << " is already in the group!" << endl;
                }
            } else {
                cout << "User not found!" << endl;
            }
        }

        if (members.size() < 2) {
            cout << "Group must have at least 2 participants!" << endl;
            return;
        }

        GroupChat* group = new GroupChat(members, groupName, getCurrentUsername());
        chats.push_back(group);

        cout << "Group '" << groupName << "' created successfully!" << endl;

        // Ask for description
        cout << "Enter group description (or press enter to skip): ";
        cin.ignore();
        string desc;
        getline(cin, desc);
        if (!desc.empty()) {
            group->setDescription(desc);
        }

        // Enter the group chat
        enterChat(group);
    }

    void viewChats() {
        if (!isLoggedIn()) return;

        cout << "\n=== Your Chats ===" << endl;

        vector<Chat*> userChats;
        for (Chat* chat : chats) {
            PrivateChat* priv = dynamic_cast<PrivateChat*>(chat);
            GroupChat* group = dynamic_cast<GroupChat*>(chat);

            if (priv) {
                vector<string> parts = priv->getParticipants();
                if (parts[0] == getCurrentUsername() || parts[1] == getCurrentUsername()) {
                    userChats.push_back(chat);
                }
            }
            else if (group && group->isParticipant(getCurrentUsername())) {
                userChats.push_back(chat);
            }
        }

        if (userChats.empty()) {
            cout << "You have no chats yet." << endl;
            return;
        }

        for (int i = 0; i < userChats.size(); i++) {
            cout << i + 1 << ". " << userChats[i]->getChatName() << endl;
        }

        cout << "Enter chat number to open (0 to cancel): ";
        int choice;
        cin >> choice;

        if (choice > 0 && choice <= userChats.size()) {
            enterChat(userChats[choice - 1]);
        }
    }

    void logout() {
        if (isLoggedIn()) {
            users[currentUserIndex].setStatus("Offline");
            users[currentUserIndex].updateLastSeen();
            currentUserIndex = -1;
            cout << "Logged out successfully!" << endl;
        }
    }

    void run() {
        while (true) {
            if (!isLoggedIn()) {
                cout << "\n=== WhatsApp ===" << endl;
                cout << "1. Login" << endl;
                cout << "2. Sign Up" << endl;
                cout << "3. Exit" << endl;
                cout << "Choice: ";

                int choice;
                cin >> choice;

                if (choice == 1) login();
                else if (choice == 2) signUp();
                else if (choice == 3) {
                    cout << "Goodbye!" << endl;
                    break;
                }
            }
            else {
                cout << "\n=== Welcome, " << getCurrentUsername() << " ===" << endl;
                cout << "1. Start Private Chat" << endl;
                cout << "2. Create Group" << endl;
                cout << "3. View Chats" << endl;
                cout << "4. Logout" << endl;
                cout << "Choice: ";

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
//          MAIN
// ========================
int main() {
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}