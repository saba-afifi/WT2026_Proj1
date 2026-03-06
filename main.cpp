#include <iostream>
#include <vector>
#include <string>
#include <ctime>
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
 // TODO: Implement default constructor
 }

 User(string uname, string pwd, string phone) {
 // TODO: Implement parameterized constructor
 }

 string getUsername() const {
 // TODO: Implement getter
 return "";
 }

 string getPhoneNumber() const {
 // TODO: Implement getter
 return "";
 }

 string getStatus() const {
 // TODO: Implement getter
 return "";
 }

 string getLastSeen() const {
 // TODO: Implement getter
 return "";
 }

 void setStatus(string newStatus) {
 // TODO: Implement setter
 }

 void setPhoneNumber(string phone) {
 // TODO: Implement setter
 }

 void updateLastSeen() {
 // TODO: Implement last seen update
 }

 bool checkPassword(string pwd) const {
 // TODO: Implement password check
 return false;
 }

 void changePassword(string newPwd) {
 // TODO: Implement password change
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
  sender="";
  content="";
  timestamp="";
  status="";
 }

 Message(string sndr, string cntnt) {
 // TODO: Implement parameterized constructor
  sender=sndr;
  if (cntnt.empty()) {
   content = "Message is empty";
  }
  else {
   content = cntnt;
  }
  status="sent";
  updateTimestamp();
 }

 string getContent() const {
 // TODO: Implement getter

 return content;
 }

 string getSender() const {
 // TODO: Implement getter
 return sender ;
 }

 string getTimestamp() const {
 // TODO: Implement getter
 return timestamp;
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
  time_t now = time(0);
  timestamp=ctime(&now);

 }

 void display() const {
 // TODO: Implement message display
  cout<<"["<<sender<<"@"<<timestamp<<"]:"<<content<<endl;
 }

 void addEmoji(string emojiCode) {
 // TODO: Implement emoji support
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
  messages.push_back(msg);
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
  user1=u1;
  user2=u2;
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
 // TODO: Team Member 1: Implement user search
 return -1;
 }

 bool isLoggedIn() const {
 // TODO: Team Member 1: Implement login check
 return false;
 }

 string getCurrentUsername() const {
 // TODO: Team Member 1: Implement get current user
 return "";
 }

public:
 WhatsApp() : currentUserIndex(-1) {}

 void signUp() {
 // TODO: Team Member 1: Implement user registration
 }

 void login() {
 // TODO: Team Member 1: Implement user login
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
 // TODO: Team Member 1: Implement logout
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