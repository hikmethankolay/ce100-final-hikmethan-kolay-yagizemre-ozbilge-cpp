/**
 * @brief a variable to mute warnings.
 *
 */
#define _CRT_SECURE_NO_WARNINGS

#include "../header/fitness_management_lib.h"
#include "../header/sha1.hpp"
#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <codecvt>
#include <random>
#include <sstream>
#include <iomanip>
#include <queue>
#include <unordered_map>
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <cstring>

using namespace std;

/**
*
* @brief variable to disable warnings.
*
*/
#define _CRT_SECURE_NO_WARNINGS

/**
 * @brief Struct for menu functions.
 *
 */
struct manin_menu_variables main_menu_choice;
/**
 * @brief Struct for menu functions.
 *
 */
struct sub_menu_variables sub_menu;

fstream myFile;  /**< File stream object for file operations. */

/**
 * @brief Calculates frequency of characters in the input text.
 *
 * @param text Input text.
 * @return unordered_map<char, int> Frequency map of characters.
 */
unordered_map<char, int> calculateFrequency(const string &text) {
  unordered_map<char, int> freqMap;

  for (char ch : text) {
    if (ch == ' ') {
      freqMap['_']++;
    } else {
      freqMap[ch]++;
    }
  }

  return freqMap;
}

/**
 * @brief Builds the Huffman tree based on character frequencies.
 *
 * @param freqMap Frequency map of characters.
 * @return Node* Root of the Huffman tree.
 */
Node *buildHuffmanTree(const unordered_map<char, int> &freqMap) {
  priority_queue<Node *, vector<Node *>, Compare> pq;

  // Create leaf nodes and add them to the priority queue
  for (auto &entry : freqMap) {
    pq.push(new Node(entry.first, entry.second));
  }

  // Merge nodes until there's only one node left in the queue
  while (pq.size() > 1) {
    Node *left = pq.top();
    pq.pop();
    Node *right = pq.top();
    pq.pop();
    // Create a new internal node with combined frequency
    Node *newNode = new Node('$', left->freq + right->freq);
    newNode->left = left;
    newNode->right = right;
    // Add the new node back to the priority queue
    pq.push(newNode);
  }

  // Return the root of the Huffman tree
  return pq.top();
}


/**
 * @brief Traverses the Huffman tree and builds the codewords.
 *
 * @param root Root of the Huffman tree.
 * @param code Current code.
 * @param codes Map to store character codes.
 */
void buildCodes(Node *root, string code, unordered_map<char, string> &codes) {
  if (root == nullptr) return;

  // If leaf node is reached, store the code
  if (root->data != '$') {
    codes[root->data] = code;
  }

  // Traverse left and right
  buildCodes(root->left, code + "0", codes);
  buildCodes(root->right, code + "1", codes);
}

/**
 * @brief Encodes the input text using Huffman codes.
 *
 * @param text Input text.
 * @param codes Map of character codes.
 * @return string Encoded text.
 */
string encode(const string &text, const unordered_map<char, string> &codes) {
  string encodedText = "";

  for (char ch : text) {
    if (ch == ' ') {
      encodedText += codes.at('_');
    } else {
      encodedText += codes.at(ch);
    }
  }

  return encodedText;
}

/**
 * @brief Decodes the encoded text using Huffman codes.
 *
 * @param encodedText Encoded text.
 * @param root Root of the Huffman tree.
 * @return string Decoded text.
 */
string decode(const string &encodedText, Node *root) {
  string decodedText = "";
  Node *current = root;

  for (char bit : encodedText) {
    if (bit == '0') {
      current = current->left;
    } else {
      current = current->right;
    }

    if (current->left == nullptr && current->right == nullptr) {
      decodedText += current->data;
      current = root; // Reset current to root for next character
    }
  }

  return decodedText;
}

/**
 * @brief Writes the Huffman tree to a file.
 *
 * @param outFile Output file stream.
 * @param node Current node being written.
 */
void writeTreeToFile(ofstream &outFile, Node *node) {
  if (node->left == nullptr && node->right == nullptr) {
    char data = node->data;

    if (data == '\n') {
      outFile << "L" << "\\n" << "|" << node->freq; // Special handling for newline character
    } else {
      outFile << "L" << data << "|" << node->freq;
    }
  } else {
    outFile << "I" << node->data << "|" << node->freq;
    writeTreeToFile(outFile, node->left);
    writeTreeToFile(outFile, node->right);
  }
}
/**
 * @brief Reads the Huffman tree from a file.
 *
 * @param inFile Input file stream.
 * @return Node* Root of the Huffman tree.
 */
Node *readTreeFromFile(ifstream &inFile) {
  char marker;

  if (!(inFile >> marker)) {
    cout << "End of file reached!" << endl;
    return nullptr;
  }

  if (marker == 'L') {
    char data;
    int freq;
    inFile >> data; // Read data

    if (data == '\\' && (inFile.peek() == 'n' || inFile.peek() == '\n')) {
      if (inFile.peek() == 'n') {
        inFile.ignore(); // Ignore the 'n' character
        data = '\n'; // Replace with newline character
      }
    }

    if (data == '_') {
      data = ' '; // Replace with space character
    }

    inFile.ignore(1, '|'); // Ignore the delimiter
    inFile >> freq; // Read frequency
    Node *leafNode = new Node(data, freq);
    return leafNode;
  } else if (marker == 'I') {
    char data;
    int freq;
    inFile >> data; // Read data
    inFile.ignore(1, '|'); // Ignore the delimiter
    inFile >> freq; // Read frequency
    Node *internalNode = new Node(data, freq);
    internalNode->left = readTreeFromFile(inFile);
    internalNode->right = readTreeFromFile(inFile);
    return internalNode;
  } else {
    cout << "Invalid marker in file!" << endl;
    return nullptr;
  }
}

/**
 * Finds the longest common subsequence (LCS) of two input strings.
 *
 *
 * @param text1 The first input string.
 * @param text2 The second input string.
 * @return The longest common subsequence of the input strings.
 */
string LCS(const string &text1, const string &text2) {
  int m = static_cast<int>(text1.length());
  int n = static_cast<int>(text2.length());
  // Create a 2D vector to store the length of the LCS for substrings of text1 and text2
  vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

  // Fill the dp table using bottom-up dynamic programming approach
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (text1[i - 1] == text2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }

  // Reconstruct the LCS from dp table
  int i = m, j = n;
  string lcs;

  while (i > 0 && j > 0) {
    if (text1[i - 1] == text2[j - 1]) {
      lcs = text1[i - 1] + lcs;
      --i;
      --j;
    } else if (dp[i - 1][j] > dp[i][j - 1]) {
      --i;
    } else {
      --j;
    }
  }

  return lcs;
}

/**
 * finds if file have a record that have a high LCS with text
 *
 *
 * @param text The input string.
 * @param file_name The file name.
 * @return 0 on success
 * @return -1 on fail
 */
int checkLCS(string text, string file_name) {
  ifstream myFile(file_name + ".bin", ios::binary);

  if (!myFile.is_open()) {
    return -1;
  }

  // Get the size of the file
  myFile.seekg(0, ios::end);
  streamoff fileSize = myFile.tellg();
  myFile.seekg(0, ios::beg);
  // Read the entire file into a string
  string content(fileSize, '\0');
  myFile.read(&content[0], fileSize);
  myFile.close(); // Close the file
  // Decode the content and return it
  ifstream inFile;
  inFile.open(file_name + "_huffman.bin", ios::binary);
  Node *root = readTreeFromFile(inFile);
  inFile.close();
  string decodedText = decode(content, root);
  vector<string> lines;
  string line;

  for(char i : decodedText) {
    if (i == '\n') {
      line = line + i;
      lines.push_back(line);
      line = "";
    } else {
      line = line + i;
    }
  }

  for(string i: lines) {
    string recordLCS = LCS(i, text);
    int m = static_cast<int>(i.length());
    int n = static_cast<int>(recordLCS.length());
    double similarity = (static_cast<double>(n) / static_cast<double>(m))*100;
    double limit = 85;

    if(similarity >= limit) {
      return 0;
    }
  }

  return -1;
}

/**
 * @brief Generates an HMAC digest using the given key and interval.
 *
 * @param key A pointer to the shared secret key.
 * @param kl The length of the shared secret key.
 * @param interval The counter value (time interval or event count).
 * @return A pointer to the generated HMAC digest.
 */
uint8_t *hmac(unsigned char *key, int kl, uint64_t interval) {
  return (uint8_t *)HMAC(EVP_sha1(), key, kl, (const unsigned char *)&interval, sizeof(interval), NULL, 0);
}

/**
 * @brief Extracts a 32-bit dynamic truncation value from the given HMAC digest.
 *
 * @param digest A pointer to the HMAC digest.
 * @return The 32-bit dynamic truncation value.
 */
uint32_t DT(uint8_t *digest) {
  uint64_t offset;
  uint32_t bin_code;
#ifdef DEBUG
  char mdString[40];

  for (int i = 0; i < 20; i++)
    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

  printf("HMAC digest: %s\n", mdString);
#endif
  // dynamically truncates hash
  offset = digest[19] & 0x0f;
  bin_code = (digest[offset] & 0x7f) << 24
             | (digest[offset+1] & 0xff) << 16
             | (digest[offset+2] & 0xff) << 8
             | (digest[offset+3] & 0xff);
#ifdef DEBUG
  printf("OFFSET: %d\n", offset);
  printf("\nDBC1: %d\n", bin_code);
#endif
  return bin_code;
}

/**
 * @brief Modifies the dynamically truncated value to fit the desired number of digits.
 *
 * @param bin_code The dynamically truncated value.
 * @param digits The number of digits for the generated OTP.
 * @return The OTP truncated to the specified number of digits.
 */
uint32_t mod_hotp(uint32_t bin_code, int digits) {
  int power = static_cast<int>(pow(10, digits));
  uint32_t otp = bin_code % power;
  return otp;
}

/**
 * @brief Generates a One-Time Password (OTP) using the HMAC-Based One-Time Password (HOTP) algorithm.
 *
 * @param key A pointer to the shared secret key.
 * @param kl The length of the shared secret key.
 * @param interval The counter value (time interval or event count).
 * @param digits The number of digits for the generated OTP.
 * @return The generated OTP.
 */
uint32_t HOTP(uint8_t *key, size_t kl, uint64_t interval, int digits) {
  uint8_t *digest;
  uint32_t result;
  uint32_t endianness;
#ifdef DEBUG
  printf("KEY IS: %s\n", key);
  printf("KEY LEN IS: %zu\n", kl); // Use %zu for size_t
  printf("COUNTER IS: %" PRIu64 "\n", interval); // Use PRIu64 for uint64_t
#endif
  endianness = 0xdeadbeef;

  if ((*(const uint8_t *)&endianness) == 0xef) {
    interval = ((interval & 0x00000000ffffffff) << 32) | ((interval & 0xffffffff00000000) >> 32);
    interval = ((interval & 0x0000ffff0000ffff) << 16) | ((interval & 0xffff0000ffff0000) >> 16);
    interval = ((interval & 0x00ff00ff00ff00ff) <<  8) | ((interval & 0xff00ff00ff00ff00) >>  8);
  }

  // First Phase: get the digest of the message using the provided key
  digest = (uint8_t *)hmac(key, static_cast<int>(kl), interval);
  // Second Phase: get the dbc from the algorithm
  uint32_t dbc = DT(digest);
  // Third Phase: calculate the mod_k of the dbc to get the correct number
  result = mod_hotp(dbc, digits);
  return result;
}

/**
 * @brief Calculates the current time interval since a reference time T0.
 *
 * @param t0 The reference time.
 * @return The current time interval since T0.
 */
time_t get_time(time_t t0) {
  return static_cast<time_t>(floor((time(NULL) - t0) / TS));
}

/**
 * @brief Generates a Time-Based One-Time Password (TOTP) using the given key and current time.
 *
 * @param key A pointer to the shared secret key.
 * @param kl The length of the shared secret key.
 * @param time The current time or time interval.
 * @param digits The number of digits for the generated OTP.
 * @return The generated TOTP.
 */
uint32_t TOTP(uint8_t *key, size_t kl, uint64_t time, int digits) {
  uint32_t totp;
  totp = HOTP(key, kl, time, digits);
  return totp;
}

/**
 * @brief Opens a binary file, deletes all of its content, and writes given text to it.
 *
 * @param file_name The name of the file to write.
 * @param text The text to write.
 * @param isFileNew check if a file new. if file is new it put "1-)" at start and "\n" at end
 * @return 0 on success.
 */
int file_write(string file_name, string text, bool isFileNew) {
  if (isFileNew) {
    text = "1-)" + text + "\n";
  }

  unordered_map<char, int> freqMap = calculateFrequency(text);
  Node *root = buildHuffmanTree(freqMap);
  unordered_map<char, string> codes;
  buildCodes(root, "", codes);
  string encodedText = encode(text, codes);
  // Open the binary file for writing
  ofstream outFile(file_name + ".bin", ios::binary);

  // Write the encoded binary data to the binary file
  for (char i : encodedText) {
    outFile.write(&i, sizeof(char));
  }

  outFile.close(); // Close the file
  // Open the Huffman file for writing
  ofstream outFileHuffman(file_name + "_huffman.bin", ios::binary);
  // Write the Huffman tree to the Huffman file
  writeTreeToFile(outFileHuffman, root);
  outFileHuffman.close(); // Close the Huffman file
  return 0;
}

/**
 * @brief Opens a binary file, reads all of its content, separates lines with "\n", and writes them to console. Also returns the contents of the file as a string for unit tests.
 *
 * @param file_name The name of the file to read from.
 * @param print_to_console It is a variable to disabling wrting content to console.
 * @return The contents of the file as a statically allocated string.
 */
string file_read(string file_name, const char print_to_console) {
  ifstream myFile(file_name + ".bin", ios::binary);

  if (!myFile.is_open()) {
    cout << "File operation failed, There is no record" << endl;
    return "-1";
  }

  // Get the size of the file
  myFile.seekg(0, ios::end);
  streamoff fileSize = myFile.tellg();
  myFile.seekg(0, ios::beg);
  // Read the entire file into a string
  string content(fileSize, '\0');
  myFile.read(&content[0], fileSize);
  myFile.close(); // Close the file
  // Decode the content and return it
  ifstream inFile;
  inFile.open(file_name + "_huffman.bin", ios::binary);
  Node *root = readTreeFromFile(inFile);
  inFile.close();
  string decodedText = decode(content, root);

  if (print_to_console == 'Y') {
    cout << decodedText; // Print the content to the console
  }

  return decodedText;
}

/**
 * @brief Opens a binary file, reads all of its content, separates lines with "\n", and writes them to console. Also returns the contents of the file as a string for unit tests.
 *
 * @param file_name The name of the file to read from.
 *
 * @return The contents of the file as a statically allocated string.
 */
string file_read_for_test(const string file_name) {
  string content;
  ifstream myFile(file_name+".bin", ios::binary);

  if (!myFile.is_open()) {
    std::cout << "File operation failed, There is no record" << std::endl;
    return "-1";
  }

  int ch;

  while ((ch = myFile.get()) != EOF) { // Ensure there's room for null terminator
    if (ch == '\r') continue; // Skip '\r'

    content+=ch;
  }

  myFile.close(); // Ensure the file is closed
  return content;
}

/**
 * @brief Appends given text to a binary file with a automatic calculated line number. Calcultes new lines line number by finding last lines line number.
 *
 *
 * @param file_name The name of the file to append to.
 * @param text The text to append to the file.
 * @return 0 on success.
 */
int file_append(string file_name, string text) {
  char mode = 'N';
  string file_content = file_read(file_name, mode);

  if(file_content == "-1") {
    return -1;
  }

  string lastLine;
  string currentLine;

  for (char i : file_content) {
    if (i == '\n') {
      currentLine = currentLine + i;
      lastLine = currentLine;
      currentLine = "";
      continue;
    }

    currentLine = currentLine + i;
  }

  size_t pos = lastLine.find("-)"); // Finds the location of "-)" in the last line
  int lineNumber = stoi(lastLine.substr(0, pos)) + 1; // Finds the number for the appended line
  text = to_string(lineNumber) + "-)" + text + "\n";
  file_content += text;
  file_write(file_name,file_content, false);
  return 0;
}

/**
 * @brief This function Opens a binary file, finds the line that user wants to edit and replace it wih new text.
 *
 *
 * @param file_name The name of the file to edit.
 * @param line_number_to_edit The line number to edit.
 * @param new_line The new text to replace the existing line.
 * @return 0 on success.
 */
int file_edit(string file_name, int line_number_to_edit, string new_line) {
  char mode = 'N';
  string file_content = file_read(file_name, mode);

  if(file_content == "-1") {
    return -1;
  }

  vector<string> lines;
  string line;
  int line_count = 0; // A variable for an if statement to check if the line that the user wants to edit exists

  for(char i : file_content) {
    if (i == '\n') {
      line = line + i;
      lines.push_back(line);
      line_count++;
      line = "";
      continue;
    }

    line = line + i;
  }

  if (line_number_to_edit > 0 && line_number_to_edit <= line_count) {
    lines[line_number_to_edit-1] = to_string(line_number_to_edit) + "-)" + new_line + "\n"; // Changes a member of Lines array to a new line with its line number
  } else {
    cout << "\nYou can only edit existing lines";
    return -1;
  }

  string new_file_content;

  for(string i : lines) {
    new_file_content += i;
  }

  file_write(file_name, new_file_content, false);
  cout << "\nData successfully edited";
  return 0;
}

/**
 * @brief This function Opens a binary file, deletes the line user wanted and make adjustments on line number acordingly.
 *
 *
 * @param file_name The name of the file to delete the line from.
 * @param line_number_to_delete The line number to delete.
 * @return 0 on success.
 */
int file_line_delete(string file_name, int line_number_to_delete) {
  char mode = 'N';
  string file_content = file_read(file_name, mode);

  if(file_content == "-1") {
    return -1;
  }

  vector<string> lines;
  string line;
  int line_count = 0; // A variable for an if statement to check if the line that the user wants to edit exists

  for(char i : file_content) {
    if (i == '\n') {
      line = line + i;
      lines.push_back(line);
      line_count++;
      line = "";
    } else {
      line = line + i;
    }
  }

  if (line_number_to_delete > 0 && line_number_to_delete <= line_count) {
    // Shift elements to "erase" the line at line_number_to_delete
    for (int i = line_number_to_delete - 1; i < line_count - 1; ++i) {
      lines[i] = lines[i + 1];
    }

    lines.pop_back(); // Clears the last element of lines so the same thing wouldn't write to the file twice
  } else {
    cout << "\nYou can only erase existing lines";
    myFile.close();
    return -1;
  }

  string new_file_content;

  for (const string &updated_lines : lines) {
    size_t pos = updated_lines.find("-)"); // Finds the position of "-)"
    int lineNumber = stoi(updated_lines.substr(0, pos)); // Finds each line's line number

    if (lineNumber > line_number_to_delete) { // Decreases a line's line number if it's bigger than the deleted line's line number
      string updated_line_with_new_number = to_string(lineNumber - 1) + updated_lines.substr(pos);
      new_file_content += updated_line_with_new_number;
    } else {
      new_file_content += updated_lines;
    }
  }

  file_write(file_name, new_file_content, false);
  cout << "\nData successfully deleted";
  return 0;
}

/**
 * @brief This function is for user register
 *
 * Function creates a user file in binary format and writes inputted username and password in it. Additionaly deletes all previous records.
 * @param new_username new username.
 * @param new_password new password.
 * @param new_recovery_key new recovery key
 * @param user_file file that contains user info.
 * @return 0 on success.
 * @return -1 on fail.
 */
int user_register(string new_username, string new_password, string new_recovery_key, string user_file) {
  new_username = sha1(new_username);
  new_password = sha1(new_password);
  new_recovery_key = sha1(new_recovery_key);
  string login_info = new_username + "/" + new_password + "/" + new_recovery_key;
  file_write(user_file,login_info, false);
  return 0;
}

/**
 * @brief This function is for user login
 *
 * Function read user.bin file and checks if username and password matchs with inputted username and password
 * @param username username.
 * @param password password.
 * @param user_file file that contains user info.
 * @return 0 on success.
 * @return -1 on fail.
 */
int user_login(string username, string password, string user_file) {
  string username_read;
  string password_read;
  string recovery_key_read;
  int count = 0;
  char mode = 'N';
  string file_content = file_read(user_file, mode);

  if(file_content == "-1") {
    return -1;
  }

  for(char i : file_content) {
    if (i == '/') {
      count++;
      continue;
    }

    if (count == 0) {
      username_read = username_read + i;
    } else if (count == 1) {
      password_read = password_read + i;
    } else if (count == 2) {
      break;
    }
  }

  if (sha1(username) == username_read && sha1(password) == password_read) {
    cout << "\nLogin Succesfull";
    return 0;
  } else {
    cout << "\nWrong username or password";
    return -1;
  }
}


/**
 * @brief This function changes the password of a user.
 * @param recovery_key recovery_key.
 * @param new_password new password.
 * @param user_file file that contains user info.
 * @return 0 on success.
 * @return -1 on fail.
 */
int user_change_password(string recovery_key, string new_password, string user_file) {
  string username_read;
  string password_read;
  string recovery_key_read;
  string new_login_info;
  int count = 0;
  char mode = 'N';
  string file_content = file_read(user_file, mode);

  if(file_content == "-1") {
    cout << "\nThere is no user info, Please register first.\n";
    return -1;
  }

  for(char i : file_content) {
    if (i == '/') {
      count++;
      continue;
    }

    if (count == 0) {
      username_read = username_read + i;
    } else if (count == 1) {
      continue;
    } else if (count == 2) {
      recovery_key_read = recovery_key_read + i;
    }
  }

  if (recovery_key_read == sha1(recovery_key)) {
    cout << "\nRecovey Key Approved\n";
    new_password = sha1(new_password);
    new_login_info = username_read + "/" + new_password + "/" + recovery_key_read;
    file_write(user_file,new_login_info, false);
    cout << "\nPassword Changed successfully";
    return 0;
  } else {
    cout << "\nWrong Recovery Key";
    return -1;
  }
}

/**
 * @brief Compute the SHA-1 hash of a given input string.
 *
 * @param input The input string to hash.
 * @return The SHA-1 hash value as a hexadecimal string.
 */
string sha1(string &input) {
  // Create an instance of the SHA1 class
  SHA1 sha1;
  // Update the SHA1 instance with the data
  sha1.update(input);
  // Get the final hash value
  string hash = sha1.final();
  return hash;
}

/**
 * @brief main menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int main_menu(bool isGuestMode) {
  while (true) {
    cout << "\n--------Main Menu--------";
    cout << "\n1-)Member Management";
    cout << "\n2-)Subscription Tracking";
    cout << "\n3-)Class Management";
    cout << "\n4-)Payment Processing";
    cout << "\n5-)Log out";
    cout << "\nPlease enter a choice:";
    int choice_main_menu;
    cin >> choice_main_menu;

    if (choice_main_menu == main_menu_choice.main_menu_member) {
      member_menu(isGuestMode);
    } else if (choice_main_menu == main_menu_choice.main_menu_subs) {
      subs_menu(isGuestMode);
    } else if (choice_main_menu == main_menu_choice.main_menu_class) {
      class_menu(isGuestMode);
    } else if (choice_main_menu == main_menu_choice.main_menu_payment) {
      payment_menu(isGuestMode);
    } else if (choice_main_menu == main_menu_choice.main_menu_log_out) {
      break;
    } else {
      cout << "\nPlease input a correct choice.";
      continue;
    }
  }

  return 0;
}

/**
 * @brief member_menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int member_menu(bool isGuestMode) {
  while (true) {
    cout << "\n--------Memberships Menu--------";
    cout << "\n1-)Show Memberships";
    cout << "\n2-)Add Membership";
    cout << "\n3-)Edit Memberships";
    cout << "\n4-)Delete Memberships";
    cout << "\n5-)Return to Main Menu";
    cout << "\nPlease enter a choice:";
    int choice_member;
    cin >> choice_member;

    if (choice_member == sub_menu.sub_menu_show) {
      cout << "\n--------------Membership Records--------------\n";
      file_read("member_records",'Y');
      continue;
    } else if (isGuestMode == true && (choice_member == sub_menu.sub_menu_add || choice_member == sub_menu.sub_menu_edit || choice_member == sub_menu.sub_menu_delete)) {
      cout << "\nYou can only see records while in guest mode.";
      continue;
    } else if (choice_member == sub_menu.sub_menu_add) {
      add_member_record();
      continue;
    } else if (choice_member == sub_menu.sub_menu_edit) {
      edit_member_record();
      continue;
    } else if (choice_member == sub_menu.sub_menu_delete) {
      delete_member_record();
      continue;
    } else if (choice_member == sub_menu.sub_menu_return) {
      break;
    } else {
      cout << "\nPlease input a correct choice.";
      continue;
    }
  }

  return 0;
}

/**
 * @brief a fucntion for adding member record.
 *
 * @return 0.
 */
int add_member_record() {
  memberRecord member;
  cout << "\nPlease enter memberID: ";
  cin >> member.memberID;
  cout << "\nPlease enter full name: ";
  cin >> member.fullName;
  cout << "\nPlease enter birth date: ";
  cin >> member.birthDate;
  cout << "\nPlease enter phone number: ";
  cin >> member.phoneNumber;
  cout << "\nPlease enter first registration date: ";
  cin >> member.firstRegistrationDate;
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << member.memberID
                  << " / Full name:" << member.fullName
                  << " / Birth date:" << member.birthDate
                  << " / Phone number:" << member.phoneNumber
                  << " / First registration date:" << member.firstRegistrationDate;
  string result = formattedRecord.str();

  if (checkLCS(result,"member_records") == 0) {
    char choice;
    cout << "\nThere is a very similar record, Dou you wish to add new record anayway?[Y/n]";
    cin >> choice;

    if(choice != 'Y') {
      return 0;
    }
  }

  FILE *myFile;
  myFile = fopen("member_records.bin", "rb");

  if (myFile == NULL) {
    file_write("member_records", result, true);
    return 0;
  } else {
    fclose(myFile);
    file_append("member_records", result);
    return 0;
  }
};

/**
 * @brief a fucntion for editing member record.
 *
 * @return 0 on success.
 * @return -1 on fail.
 */
int edit_member_record() {
  memberRecord member;
  int RecordNumberToEdit;
  cout << "\nPlease enter record number to edit: ";
  cin >> RecordNumberToEdit;
  cout << "\nPlease enter memberID: ";
  cin >> member.memberID;
  cout << "\nPlease enter full name: ";
  cin >> member.fullName;
  cout << "\nPlease enter birth date: ";
  cin >> member.birthDate;
  cout << "\nPlease enter phone number: ";
  cin >> member.phoneNumber;
  cout << "\nPlease enter first registration date: ";
  cin >> member.firstRegistrationDate;
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << member.memberID
                  << " / Full name:" << member.fullName
                  << " / Birth date:" << member.birthDate
                  << " / Phone number:" << member.phoneNumber
                  << " / First registration date:" << member.firstRegistrationDate;
  string result = formattedRecord.str();

  if (file_edit("member_records", RecordNumberToEdit, result) == 0) {
    return 0;
  } else {
    return -1;
  }
};

/**
 * @brief a fucntion for deleting member record.
 *
 * @return 0.
 */
int delete_member_record() {
  cout << "\nPlease enter record number to delete:";
  int RecordNumberToDelete;
  cin >> RecordNumberToDelete;

  if (file_line_delete("member_records", RecordNumberToDelete) == 0) {
    return 0;
  } else {
    return -1;
  }
};

/**
 * @brief subs menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int subs_menu(bool isGuestMode) {
  while (true) {
    cout << "\n--------Subscriptions Menu--------";
    cout << "\n1-)Show Subscriptions";
    cout << "\n2-)Add Subscription";
    cout << "\n3-)Edit Subscriptions";
    cout << "\n4-)Delete Subscriptions";
    cout << "\n5-)Return to Main Menu";
    cout << "\nPlease enter a choice:";
    int choice_sub;
    cin >> choice_sub;

    if (choice_sub == sub_menu.sub_menu_show) {
      cout << "\n--------------Membership Records--------------\n";
      file_read("subscription_records",'Y');
      continue;
    } else if (isGuestMode == true && (choice_sub == sub_menu.sub_menu_add || choice_sub == sub_menu.sub_menu_edit || choice_sub == sub_menu.sub_menu_delete)) {
      cout << "\nYou can only see records while in guest mode.";
      continue;
    } else if (choice_sub == sub_menu.sub_menu_add) {
      add_subs_record();
      continue;
    } else if (choice_sub == sub_menu.sub_menu_edit) {
      edit_subs_record();
      continue;
    } else if (choice_sub == sub_menu.sub_menu_delete) {
      delete_subs_record();
      continue;
    } else if (choice_sub == sub_menu.sub_menu_return) {
      break;
    } else {
      cout << "\nPlease input a correct choice.";
      continue;
    }
  }

  return 0;
}

/**
 * @brief a fucntion for adding subscription record.
 *
 * @return 0.
 */
int add_subs_record() {
  subsciprtionRecord sub;
  cout << "\nPlease enter memberID:";
  cin >> sub.memberID;
  cout << "\nPlease enter starting date:";
  cin >> sub.startingDate;
  cout << "\nPlease enter finishing date:";
  cin >> sub.finishingDate;
  cout << "\nPlease enter subcription tier:";
  cin >> sub.subscriptionTier;
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << sub.memberID
                  << " / Starting date:" << sub.startingDate
                  << " / Finishing date:" << sub.finishingDate
                  << " / Subcription tier:" << sub.subscriptionTier;
  string result = formattedRecord.str();

  if (checkLCS(result,"subscription_records") == 0) {
    char choice;
    cout << "\nThere is a very similar record, Dou you wish to add new record anayway?[Y/n]";
    cin >> choice;

    if(choice != 'Y') {
      return 0;
    }
  }

  FILE *myFile;
  myFile = fopen("subscription_records.bin", "rb");

  if (myFile == NULL) {
    file_write("subscription_records", result, true);
    return 0;
  } else {
    fclose(myFile);
    file_append("subscription_records", result);
    return 0;
  }
};

/**
 * @brief a fucntion for editing subscription record.
 *
 * @return 0 on success.
 * @return -1 on fail.
 */
int edit_subs_record() {
  subsciprtionRecord sub;
  int RecordNumberToEdit;
  cout << "\nPlease enter record number to edit:";
  cin >> RecordNumberToEdit;
  cout << "\nPlease enter memberID:";
  cin >> sub.memberID;
  cout << "\nPlease enter starting date:";
  cin >> sub.startingDate;
  cout << "\nPlease enter finishing date:";
  cin >> sub.finishingDate;
  cout << "\nPlease enter subcription tier:";
  cin >> sub.subscriptionTier;
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << sub.memberID
                  << " / Starting date:" << sub.startingDate
                  << " / Finishing date:" << sub.finishingDate
                  << " / Subcription tier:" << sub.subscriptionTier;
  string result = formattedRecord.str();

  if (file_edit("subscription_records", RecordNumberToEdit, result) == 0) {
    return 0;
  } else {
    return -1;
  }
};

/**
 * @brief a fucntion for deleting subscription record.
 *
 * @return 0.
 */
int delete_subs_record() {
  cout << "\nPlease enter record number to delete:";
  int RecordNumberToDelete;
  cin >> RecordNumberToDelete;

  if (file_line_delete("subscription_records", RecordNumberToDelete) == 0) {
    return 0;
  } else {
    return -1;
  }
};

/**
 * @brief class menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int class_menu(bool isGuestMode) {
  while (true) {
    cout << "\n--------Classes Menu--------";
    cout << "\n1-)Show Classes";
    cout << "\n2-)Add Class";
    cout << "\n3-)Edit Classes";
    cout << "\n4-)Delete Classes";
    cout << "\n5-)Return to Main Menu";
    cout << "\nPlease enter a choice:";
    int choice_class;
    cin >> choice_class;

    if (choice_class == sub_menu.sub_menu_show) {
      cout << "\n--------------Class Records--------------\n";
      file_read("class_records",'Y');
      continue;
    } else if (isGuestMode == true && (choice_class == sub_menu.sub_menu_add || choice_class == sub_menu.sub_menu_edit || choice_class == sub_menu.sub_menu_delete)) {
      cout << "\nYou can only see records while in guest mode.";
      continue;
    } else if (choice_class == sub_menu.sub_menu_add) {
      add_class_record();
      continue;
    } else if (choice_class == sub_menu.sub_menu_edit) {
      edit_class_record();
      continue;
    } else if (choice_class == sub_menu.sub_menu_delete) {
      delete_class_record();
      continue;
    } else if (choice_class == sub_menu.sub_menu_return) {
      break;
    } else {
      cout << "\nPlease input a correct choice.";
      continue;
    }
  }

  return 0;
}

/**
 * @brief a fucntion for adding class record.
 *
 * @return 0.
 */
int add_class_record() {
  classRecord classRecord;
  cout << "\nPlease enter tutor: ";
  cin >> classRecord.tutor;
  cout << "\nPlease enter date: ";
  cin >> classRecord.date;
  cout << "\nPlease enter starting hour: ";
  cin >> classRecord.startingHour;
  cout << "\nPlease enter finishing hour: ";
  cin >> classRecord.finishingHour;
  cout << "\nPlease enter student list: ";
  cin >> classRecord.studentList;
  ostringstream formattedRecord;
  formattedRecord << "Tutor:" << classRecord.tutor
                  << " / Class date:" << classRecord.date
                  << " / Starting Hour:" << classRecord.startingHour
                  << " / Finishing Hour:" << classRecord.finishingHour
                  << " / Student List:" << classRecord.studentList;
  string result = formattedRecord.str();

  if (checkLCS(result,"class_records") == 0) {
    char choice;
    cout << "\nThere is a very similar record, Dou you wish to add new record anayway?[Y/n]";
    cin >> choice;

    if(choice != 'Y') {
      return 0;
    }
  }

  FILE *myFile;
  myFile = fopen("class_records.bin", "rb");

  if (myFile == NULL) {
    file_write("class_records", result, true);
    return 0;
  } else {
    fclose(myFile);
    file_append("class_records", result);
    return 0;
  }
};

/**
 * @brief a fucntion for editing class record.
 *
 * @return 0.
 */
int edit_class_record() {
  classRecord classRecord;
  int RecordNumberToEdit;
  cout << "\nPlease enter a record number to edit: ";
  cin >> RecordNumberToEdit;
  cout << "\nPlease enter tutor: ";
  cin >> classRecord.tutor;
  cout << "\nPlease enter date: ";
  cin >> classRecord.date;
  cout << "\nPlease enter starting hour: ";
  cin >> classRecord.startingHour;
  cout << "\nPlease enter finishing hour: ";
  cin >> classRecord.finishingHour;
  cout << "\nPlease enter student list: ";
  cin >> classRecord.studentList;
  ostringstream formattedRecord;
  formattedRecord << "Tutor:" << classRecord.tutor
                  << " / Class date:" << classRecord.date
                  << " / Starting Hour:" << classRecord.startingHour
                  << " / Finishing Hour:" << classRecord.finishingHour
                  << " / Student List:" << classRecord.studentList;
  string result = formattedRecord.str();

  if (file_edit("class_records", RecordNumberToEdit, result) == 0) {
    return 0;
  } else {
    return -1;
  }
};

/**
 * @brief a fucntion for deleting class record.
 *
 * @return 0.
 */
int delete_class_record() {
  cout << "\nPlease enter record number to delete:";
  int RecordNumberToDelete;
  cin >> RecordNumberToDelete;

  if (file_line_delete("class_records", RecordNumberToDelete) == 0) {
    return 0;
  } else {
    return -1;
  }
};

/**
 * @brief payment menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int payment_menu(bool isGuestMode) {
  while (true) {
    cout << "\n--------Payments Menu--------";
    cout << "\n1-)Show Payments";
    cout << "\n2-)Add Payment";
    cout << "\n3-)Edit Payments";
    cout << "\n4-)Delete Payments";
    cout << "\n7-)Return to Main Menu";
    cout << "\nPlease enter a choice:";
    int choice_payment;
    cin >> choice_payment;

    if (choice_payment == sub_menu.sub_menu_show) {
      cout << "\n--------------Payment Records--------------\n";
      file_read("payment_records",'Y');
      continue;
    } else if (isGuestMode == true && (choice_payment == sub_menu.sub_menu_add || choice_payment == sub_menu.sub_menu_edit || choice_payment == sub_menu.sub_menu_delete)) {
      cout << "\nYou can only see records while in guest mode.";
      continue;
    } else if (choice_payment == sub_menu.sub_menu_add) {
      add_payment_record();
      continue;
    } else if (choice_payment == sub_menu.sub_menu_edit) {
      edit_payment_record();
      continue;
    } else if (choice_payment == sub_menu.sub_menu_delete) {
      delete_payment_record();
      continue;
    } else if (choice_payment == sub_menu.sub_menu_return) {
      break;
    } else {
      cout << "\nPlease input a correct choice.";
      continue;
    }
  }

  return 0;
}

/**
 * @brief a fucntion for adding payment record.
 *
 * @return 0.
 */
int add_payment_record() {
  paymentRecord payment;
  cout << "\nPlease enter memberID";
  cin >> payment.memberID;
  cout << "\nPlease enter paid amount";
  cin >> payment.paidAmount;
  cout << "\nPlease enter payment date";
  cin >> payment.paymentDate;
  cout << "\nPlease enter next payment date";
  cin >> payment.nextPaymentDate;
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << payment.memberID
                  << " / Paid amount:" << payment.paidAmount
                  << " / Payment date:" << payment.paymentDate
                  << " / Next payment date:" << payment.nextPaymentDate;
  string result = formattedRecord.str();

  if (checkLCS(result,"payment_records") == 0) {
    char choice;
    cout << "\nThere is a very similar record, Dou you wish to add new record anayway?[Y/n]";
    cin >> choice;

    if(choice != 'Y') {
      return 0;
    }
  }

  FILE *myFile;
  myFile = fopen("payment_records.bin", "rb");

  if (myFile == NULL) {
    file_write("payment_records", result, true);
    return 0;
  } else {
    fclose(myFile);
    file_append("payment_records", result);
    return 0;
  }
};

/**
 * @brief a function for editing payment record.
 *
 * @return 0.
 */
int edit_payment_record() {
  paymentRecord payment;
  int RecordNumberToEdit;
  cout << "\nPlease enter a record number to edit:";
  cin >> RecordNumberToEdit;
  cout << "\nPlease enter memberID";
  cin >> payment.memberID;
  cout << "\nPlease enter paid amount";
  cin >> payment.paidAmount;
  cout << "\nPlease enter payment date";
  cin >> payment.paymentDate;
  cout << "\nPlease enter next payment date";
  cin >> payment.nextPaymentDate;
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << payment.memberID
                  << " / Paid amount:" << payment.paidAmount
                  << " / Payment date:" << payment.paymentDate
                  << " / Next payment date:" << payment.nextPaymentDate;
  string result = formattedRecord.str();

  if (file_edit("payment_records", RecordNumberToEdit, result) == 0) {
    return 0;
  } else {
    return -1;
  }
};

/**
 * @brief a function for deleting payment record.
 *
 * @return 0.
 */
int delete_payment_record() {
  cout << "\nPlease enter record number to delete:";
  int RecordNumberToDelete;
  cin >> RecordNumberToDelete;

  if (file_line_delete("payment_records", RecordNumberToDelete) == 0) {
    return 0;
  } else {
    return -1;
  }
};

/**
 * @brief login menu.
 * @param isUnitTesting a bool to check if it is unit testing.
 * @return 0.
 */
int login_menu(bool isUnitTesting) {
  string user_name;
  string password;
  string user_file = "user";
  cout << "\nPlease enter your username:";
  cin >> user_name;
  cout << "\nPlease enter your password:";
  cin >> password;

  if (user_login(user_name,password,user_file) == 0) {
    // Define your shared secret key
    unsigned char key[] = "MZUXI3TFONZW2YLOMFTWK3LFNZ2HGZLDOJSXI33UOBVWK6I=";
    size_t key_len = strlen((char *)key);
    // Define the number of digits for the OTP
    int digits = 6;  // Typically 6 or 8
    // Get the current time interval since the epoch (T0)
    time_t t0 = 0;  // Reference time (epoch)
    uint64_t current_time_interval = get_time(t0);
    // Generate the TOTP
    uint32_t otp = TOTP(key, key_len, current_time_interval, digits);
    string user_input_otp;
    cout << "\nPlease enter single use code that we send you";

    if (isUnitTesting) {
      user_input_otp = to_string(otp);
    } else {
      cout << "\n" << otp << " is the code, this is just the simulation of scenario:";
      cin >> user_input_otp;
    }

    if (user_input_otp == to_string(otp)) {
      main_menu(false);
    }
  }

  return 0;
};

/**
 * @brief register menu.
 *
 * @return 0.
 */
int register_menu() {
  string user_name;
  string password;
  string recovery_key;
  string user_file = "user";
  string warning;
  cout << "\nPlease enter your new username:";
  cin >> user_name;
  cout << "\nPlease enter your new password:";
  cin >> password;
  cout << "\nPlease enter your new recovery key:";
  cin >> recovery_key;
  cout << "\n------------WARNING------------";
  cout << "\nThis process will delete all previous records, do you still wish to proceed?[Y/n]:";
  cin >> warning;

  if (warning == "Y") {
    user_register(user_name,password,recovery_key,user_file);
  } else {
    cout << "\nProcess terminated.";
  }

  return 0;
};

/**
 * @brief change pssword menu.
 *
 * @return 0.
 */
int change_password_menu() {
  string password;
  string recovery_key;
  string user_file = "user";
  cout << "\nPlease enter your recovery key:";
  cin >> recovery_key;
  cout << "\nPlease enter your new password:";
  cin >> password;
  user_change_password(recovery_key,password,user_file);
  return 0;
};
