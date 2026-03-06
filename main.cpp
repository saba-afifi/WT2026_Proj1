#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;


=======
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
 // TODO: Implement default constructor
 }

 Message(string sndr, string cntnt) {
 // TODO: Implement parameterized constructor
 }

 string getContent() const {
 // TODO: Implement getter
 return "";
 }

 string getSender() const {
 // TODO: Implement getter
 return "";
 }

 string getTimestamp() const {
 // TODO: Implement getter
 return "";
 }

 string getStatus() const {
 // TODO: Implement getter
 return "";
 }

 Message* getReplyTo() const {
 // TODO: Implement getter
 return nullptr;
 }

 void setStatus(string newStatus) {
 // TODO: Implement setter
 }

 void setReplyTo(Message* msg) {
 // TODO: Implement setter
 }

 void updateTimestamp() {
 // TODO: Implement timestamp update
 }

 void display() const {
 // TODO: Implement message display
 }

 void addEmoji(string emojiCode) {
 // TODO: Implement emoji support
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
 // TODO: Implement default constructor
 }

 Chat(vector<string> users, string name) {
 // TODO: Implement parameterized constructor
 }

 void addMessage(const Message& msg) {
 // TODO: Implement message addition
 }

 bool deleteMessage(int index, const string& username) {
 // TODO: Implement message deletion
 return false;
 }

 virtual void displayChat() const {
 // TODO: Implement chat display
 }

 vector<Message> searchMessages(string keyword) const {
 // TODO: Implement message search
 return {};
 }

 void exportToFile(const string& filename) const {
 // TODO: Implement export to file
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
 // TODO: Implement constructor
 }

 void displayChat() const override {
 // TODO: Implement private chat display
 }

 void showTypingIndicator(const string& username) const {
 // TODO: Implement typing indicator
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
 // TODO: Implement constructor
 }

 void addAdmin(string newAdmin) {
 // TODO: Implement add admin
 }

 bool removeParticipant(const string& admin, const string& userToRemove) {
 // TODO: Implement remove participant
 return false;
 }

 bool isAdmin(string username) const {
 // TODO: Implement admin check
 return false;
 }

 bool isParticipant(string username) const {
 // TODO: Implement participant check
 return false;
 }

 void setDescription(string desc) {
 // TODO: Implement set description
 }

 void displayChat() const override {
 // TODO: Implement group chat display
 }

 void sendJoinRequest(const string& username) {
 // TODO: Implement join request
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

 cout << "Login successful. Welcome "
      << users[index].getUsername() << endl;
}
 void startPrivateChat() {
 // TODO: Team Member 4: Implement private chat creation
 }

 void createGroup() {
 // TODO: Team Member 4: Implement group creation
 }

 void viewChats() const {
 // TODO: Team Member 4: Implement chat viewing
 }

void logout() {

 if (!isLoggedIn()) {
  cout << "No user is currently logged in.\n";
  return;
 }

 users[currentUserIndex].updateLastSeen();

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
>>>>>>> origin/teammate_2
