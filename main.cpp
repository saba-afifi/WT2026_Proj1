#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>
using namespace std;

// Forward declarations
class Message;

// ========================
// USER CLASS
// ========================
class User {
private:
    string username;
    string password;
    string phoneNumber;
    string status;
    string lastSeen;

    string getCurrentTimestamp() const {
        time_t now = time(0);
        char* dt = ctime(&now);
        string ts(dt);
        ts.pop_back(); // Remove newline
        return ts;
    }

public:
    User() : username(""), password(""), phoneNumber(""), status("Offline") {
        updateLastSeen();
    }

    User(string uname, string pwd, string phone) : username(uname), phoneNumber(phone) {
        if (pwd.length() >= 6) {
            password = pwd;
        } else {
            password = "";
            cout << "Password must be at least 6 characters long!" << endl;
        }
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
        lastSeen = getCurrentTimestamp();
    }

    bool checkPassword(string pwd) const {
        return password == pwd;
    }

    void changePassword(string newPwd) {
        if (newPwd.length() >= 6) {
            password = newPwd;
            updateLastSeen();
            cout << "Password changed successfully!" << endl;
        } else {
            cout << "Password must be at least 6 characters long!" << endl;
        }
    }

    void display() const {
        cout << "Username: " << username << endl;
        cout << "Phone: " << phoneNumber << endl;
        cout << "Status: " << status << endl;
        cout << "Last Seen: " << lastSeen << endl;
    }
};

// ========================
// MESSAGE CLASS
// ========================
class Message {
private:
    string sender;
    string content;
    string timestamp;
    string status;
    Message* replyTo;

    string getCurrentTimestamp() const {
        time_t now = time(0);
        char* dt = ctime(&now);
        string ts(dt);
        ts.pop_back(); // Remove newline
        return ts;
    }

    string replaceEmoji(string text) const {
        // Emoji mapping based on SRS
        size_t pos = 0;
        while ((pos = text.find(":)", pos)) != string::npos) {
            text.replace(pos, 2, "😊");
            pos += 2;
        }
        pos = 0;
        while ((pos = text.find(":(", pos)) != string::npos) {
            text.replace(pos, 2, "😢");
            pos += 2;
        }
        pos = 0;
        while ((pos = text.find(":D", pos)) != string::npos) {
            text.replace(pos, 2, "😃");
            pos += 2;
        }
        pos = 0;
        while ((pos = text.find(";)", pos)) != string::npos) {
            text.replace(pos, 2, "😉");
            pos += 2;
        }
        pos = 0;
        while ((pos = text.find("<3", pos)) != string::npos) {
            text.replace(pos, 2, "❤️");
            pos += 2;
        }
        return text;
    }

public:
    Message() : sender(""), content(""), status("sent"), replyTo(nullptr) {
        updateTimestamp();
    }

    Message(string sndr, string cntnt) : sender(sndr), content(cntnt), status("sent"), replyTo(nullptr) {
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
        timestamp = getCurrentTimestamp();
    }

    void display() const {
        cout << "[" << timestamp << "] " << sender << ": ";
        cout << replaceEmoji(content);
        if (replyTo != nullptr) {
            cout << " (Replying to: " << replyTo->getSender() << ": "
                 << replyTo->getContent().substr(0, 20) << "...)";
        }
        cout << " [" << status << "]" << endl;
    }

    void addEmoji(string emojiCode) {
        content += " " + emojiCode;
        content = replaceEmoji(content);
    }

    bool containsKeyword(const string& keyword) const {
        return content.find(keyword) != string::npos;
    }
};

// ========================
// CHAT CLASS (BASE)
// ========================
class Chat {
protected:
    vector<string> participants;
    vector<Message> messages;
    string chatName;

public:
    Chat() : chatName("") {}

    Chat(vector<string> users, string name) : participants(users), chatName(name) {}

    virtual ~Chat() {}

    void addMessage(const Message& msg) {
        messages.push_back(msg);
        // Update status of previous messages to delivered
        for (auto& m : messages) {
            if (m.getStatus() == "sent") {
                const_cast<Message&>(m).setStatus("delivered");
            }
        }
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
        cout << "\n=== " << chatName << " ===" << endl;
        cout << "Participants: ";
        for (const auto& p : participants) {
            cout << p << " ";
        }
        cout << "\n\nMessages:" << endl;
        for (const auto& msg : messages) {
            msg.display();
        }
    }

    vector<Message> searchMessages(string keyword) const {
        vector<Message> results;
        for (const auto& msg : messages) {
            if (msg.containsKeyword(keyword)) {
                results.push_back(msg);
            }
        }
        return results;
    }

    string getChatName() const {
        return chatName;
    }

    bool isParticipant(const string& username) const {
        return find(participants.begin(), participants.end(), username) != participants.end();
    }
};

// ========================
// PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat {
private:
    string user1;
    string user2;
    bool typingStatus;

public:
    PrivateChat(string u1, string u2) : user1(u1), user2(u2), typingStatus(false) {
        participants = {u1, u2};
        chatName = "Chat between " + u1 + " and " + u2;
    }

    void displayChat() const override {
        Chat::displayChat();
        if (typingStatus) {
            cout << "\n[User is typing...]" << endl;
        }
    }

    void showTypingIndicator(const string& username) const {
        if (username == user1 || username == user2) {
            cout << "\n[" << username << " is typing...]" << endl;
        }
    }

    void setTyping(bool isTyping) {
        typingStatus = isTyping;
    }
};

// ========================
// GROUP CHAT CLASS
// ========================
class GroupChat : public Chat {
private:
    vector<string> admins;
    string description;
    vector<string> joinRequests;

public:
    GroupChat(vector<string> users, string name, string creator) : Chat(users, name) {
        admins.push_back(creator);
        description = "Group created by " + creator;
    }

    void addAdmin(string newAdmin) {
        if (isParticipant(newAdmin) && !isAdmin(newAdmin)) {
            admins.push_back(newAdmin);
            cout << newAdmin << " is now an admin." << endl;
        }
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        if (!isAdmin(admin)) {
            cout << "Only admins can remove participants!" << endl;
            return false;
        }

        if (admin == userToRemove) {
            cout << "Admins cannot remove themselves!" << endl;
            return false;
        }

        auto it = find(participants.begin(), participants.end(), userToRemove);
        if (it != participants.end()) {
            participants.erase(it);

            // Remove from admins if they were an admin
            auto adminIt = find(admins.begin(), admins.end(), userToRemove);
            if (adminIt != admins.end()) {
                admins.erase(adminIt);
            }

            cout << userToRemove << " removed from group." << endl;
            return true;
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
        cout << "\n=== " << chatName << " ===" << endl;
        cout << "Description: " << description << endl;
        cout << "\nAdmins: ";
        for (const auto& admin : admins) {
            cout << admin << " ";
        }
        cout << "\nParticipants: ";
        for (const auto& p : participants) {
            cout << p << " ";
        }
        cout << "\n\nMessages:" << endl;
        for (const auto& msg : messages) {
            msg.display();
        }
    }

    void sendJoinRequest(const string& username) {
        if (!isParticipant(username)) {
            joinRequests.push_back(username);
            cout << "Join request sent to group: " << chatName << endl;
        }
    }

    void acceptJoinRequest(const string& admin, const string& username) {
        if (isAdmin(admin)) {
            auto it = find(joinRequests.begin(), joinRequests.end(), username);
            if (it != joinRequests.end()) {
                participants.push_back(username);
                joinRequests.erase(it);
                cout << username << " joined the group." << endl;
            }
        }
    }
};

// ========================
// WHATSAPP APP CLASS
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

    // FIXED: Proper input clearing function
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // FIXED: Safe string input function
    string getStringInput(const string& prompt) {
        cout << prompt;
        string input;
        getline(cin, input);
        return input;
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    ~WhatsApp() {
        for (auto chat : chats) {
            delete chat;
        }
    }

    void signUp() {
        string username, password, phone;

        // FIXED: Clear buffer before string input
        clearInputBuffer();

        cout << "Enter username: ";
        getline(cin, username);

        // Check uniqueness
        if (findUserIndex(username) != -1) {
            cout << "Username already exists!" << endl;
            return;
        }

        cout << "Enter password (min 6 characters): ";
        getline(cin, password);

        if (password.length() < 6) {
            cout << "Password must be at least 6 characters!" << endl;
            return;
        }

        cout << "Enter phone number: ";
        getline(cin, phone);

        users.push_back(User(username, password, phone));
        cout << "Registration successful!" << endl;
    }

    void login() {
        string username, password;

        // FIXED: Clear buffer before string input
        clearInputBuffer();

        cout << "Enter username: ";
        getline(cin, username);

        int index = findUserIndex(username);
        if (index == -1) {
            cout << "User not found!" << endl;
            return;
        }

        cout << "Enter password: ";
        getline(cin, password);

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

        string otherUsername;

        // FIXED: Clear buffer before string input
        clearInputBuffer();

        cout << "Enter username to chat with: ";
        getline(cin, otherUsername);

        int otherIndex = findUserIndex(otherUsername);
        if (otherIndex == -1) {
            cout << "User not found!" << endl;
            return;
        }

        if (otherUsername == getCurrentUsername()) {
            cout << "Cannot chat with yourself!" << endl;
            return;
        }

        // Check if chat already exists
        for (auto chat : chats) {
            PrivateChat* pc = dynamic_cast<PrivateChat*>(chat);
            if (pc) {
                if (pc->isParticipant(getCurrentUsername()) && pc->isParticipant(otherUsername)) {
                    cout << "Chat already exists! Viewing existing chat..." << endl;
                    pc->displayChat();
                    return;
                }
            }
        }

        // Create new private chat
        PrivateChat* newChat = new PrivateChat(getCurrentUsername(), otherUsername);
        chats.push_back(newChat);
        cout << "Private chat created with " << otherUsername << endl;

        // Messaging loop
        string message;
        cout << "Start typing messages (type '/exit' to stop):" << endl;

        while (true) {
            cout << "You: ";
            getline(cin, message);

            if (message == "/exit") break;

            if (!message.empty()) {
                // Add message
                Message msg(getCurrentUsername(), message);
                newChat->addMessage(msg);

                // Show typing indicator for other user
                newChat->showTypingIndicator(otherUsername);

                // Display the message
                cout << "Message sent!" << endl;
            }
        }
    }

    void createGroup() {
        if (!isLoggedIn()) return;

        string groupName;
        vector<string> members;
        string member;

        // FIXED: Clear buffer before string input
        clearInputBuffer();

        cout << "Enter group name: ";
        getline(cin, groupName);

        cout << "Add members (type 'done' when finished):" << endl;
        members.push_back(getCurrentUsername()); // Creator is automatically a member
        cout << "Added: " << getCurrentUsername() << " (creator)" << endl;

        while (true) {
            cout << "Enter username (or 'done'): ";
            getline(cin, member);

            if (member == "done") break;

            if (member.empty()) continue;

            if (findUserIndex(member) != -1) {
                if (find(members.begin(), members.end(), member) == members.end()) {
                    members.push_back(member);
                    cout << member << " added to group." << endl;
                } else {
                    cout << "User already added!" << endl;
                }
            } else {
                cout << "User not found!" << endl;
            }
        }

        if (members.size() < 2) {
            cout << "Group must have at least 2 participants!" << endl;
            return;
        }

        GroupChat* newGroup = new GroupChat(members, groupName, getCurrentUsername());
        chats.push_back(newGroup);
        cout << "Group '" << groupName << "' created successfully!" << endl;

        // Add description option
        char addDesc;
        cout << "Add description? (y/n): ";
        cin >> addDesc;

        if (addDesc == 'y' || addDesc == 'Y') {
            clearInputBuffer(); // FIXED: Clear after cin
            string desc;
            cout << "Enter description: ";
            getline(cin, desc);
            newGroup->setDescription(desc);
        } else {
            clearInputBuffer(); // FIXED: Clear buffer even if not adding description
        }
    }

    void viewChats() {
        if (!isLoggedIn()) return;

        cout << "\n=== Your Chats ===" << endl;
        int chatCount = 0;
        vector<int> userChatIndices;

        for (int i = 0; i < chats.size(); i++) {
            if (chats[i]->isParticipant(getCurrentUsername())) {
                cout << i + 1 << ". " << chats[i]->getChatName() << endl;
                userChatIndices.push_back(i);
                chatCount++;
            }
        }

        if (chatCount == 0) {
            cout << "No chats available." << endl;
            return;
        }

        cout << "Select chat (0 to cancel): ";
        int choice;
        cin >> choice;

        if (choice > 0 && choice <= chatCount) {
            int chatIndex = userChatIndices[choice - 1];
            chats[chatIndex]->displayChat();

            // Menu for selected chat
            cout << "\n1. Send Message\n2. Search Messages\n3. Delete Message\nChoice: ";
            int action;
            cin >> action;

            clearInputBuffer(); // FIXED: Clear after cin

            if (action == 1) {
                // Send message
                string message;
                cout << "Enter message: ";
                getline(cin, message);

                if (!message.empty()) {
                    Message msg(getCurrentUsername(), message);
                    chats[chatIndex]->addMessage(msg);
                    cout << "Message sent!" << endl;

                    // Show typing indicator for private chats
                    PrivateChat* pc = dynamic_cast<PrivateChat*>(chats[chatIndex]);
                    if (pc) {
                        string otherUser = (pc->isParticipant(getCurrentUsername()) ?
                                            (pc->getChatName().find(getCurrentUsername()) != string::npos ? "other user" : "other user") : "other user");
                        pc->showTypingIndicator(otherUser);
                    }
                }
            }
            else if (action == 2) {
                // Search messages
                string keyword;
                cout << "Enter keyword to search: ";
                getline(cin, keyword);

                vector<Message> results = chats[chatIndex]->searchMessages(keyword);
                cout << "\nSearch Results:" << endl;
                if (results.empty()) {
                    cout << "No messages found." << endl;
                } else {
                    for (const auto& msg : results) {
                        msg.display();
                    }
                }
            }
            else if (action == 3) {
                // Delete message
                int msgIndex;
                cout << "Enter message number to delete: ";
                cin >> msgIndex;
                clearInputBuffer(); // FIXED: Clear after cin

                if (chats[chatIndex]->deleteMessage(msgIndex - 1, getCurrentUsername())) {
                    cout << "Message deleted!" << endl;
                } else {
                    cout << "Cannot delete message! (Make sure it's your message and index is valid)" << endl;
                }
            }
        } else {
            clearInputBuffer(); // FIXED: Clear buffer even if choice is 0
        }
    }

    void logout() {
        if (isLoggedIn()) {
            users[currentUserIndex].setStatus("Offline");
            users[currentUserIndex].updateLastSeen();
            cout << "Goodbye, " << users[currentUserIndex].getUsername() << "!" << endl;
            currentUserIndex = -1;
        }
    }

    void run() {
        cout << "=== WhatsApp-like Messaging System ===" << endl;
        cout << "Based on SRS Requirements" << endl;

        while (true) {
            try {
                if (!isLoggedIn()) {
                    cout << "\n=== Main Menu ===" << endl;
                    cout << "1. Login" << endl;
                    cout << "2. Sign Up" << endl;
                    cout << "3. Exit" << endl;
                    cout << "Choice: ";

                    int choice;
                    cin >> choice;

                    if (choice == 1) {
                        login();
                    }
                    else if (choice == 2) {
                        signUp();
                    }
                    else if (choice == 3) {
                        cout << "Thank you for using WhatsApp-like Messaging System!" << endl;
                        break;
                    }
                    else {
                        cout << "Invalid choice! Please enter 1, 2, or 3." << endl;
                        clearInputBuffer(); // FIXED: Clear invalid input
                    }
                }
                else {
                    cout << "\n=== Main Menu (Logged in as: " << getCurrentUsername() << ") ===" << endl;
                    cout << "1. Start Private Chat" << endl;
                    cout << "2. Create Group" << endl;
                    cout << "3. View Chats" << endl;
                    cout << "4. Logout" << endl;
                    cout << "Choice: ";

                    int choice;
                    cin >> choice;

                    if (choice == 1) {
                        startPrivateChat();
                    }
                    else if (choice == 2) {
                        createGroup();
                    }
                    else if (choice == 3) {
                        viewChats();
                    }
                    else if (choice == 4) {
                        logout();
                    }
                    else {
                        cout << "Invalid choice! Please enter 1, 2, 3, or 4." << endl;
                        clearInputBuffer(); // FIXED: Clear invalid input
                    }
                }
            }
            catch (const exception& e) {
                cout << "An error occurred: " << e.what() << endl;
                clearInputBuffer();
            }
        }
    }
};

// ========================
// MAIN
// ========================
int main() {
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}