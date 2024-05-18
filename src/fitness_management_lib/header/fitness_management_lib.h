/**
 * @file fitness_management_lib.h
 *
 * @brief Provides functions for fitness managament app.
 */

#ifndef FITNESS_MANAGEMENT_LIB_H
#define FITNESS_MANAGEMENT_LIB_H

#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include<stdint.h>
#include<stdlib.h>

#define TS 30   /* time step in seconds, default value */

/**
 * @brief Using the standard C++ namespace to simplify code with standard library elements.
 *
 */
using namespace std;
/**
* @brief memberRecord
*
*/
struct memberRecord {

  /**
  * @brief memberID
  *
  */
  int memberID;
  /**
  * @brief fullName
  *
  */
  string fullName;
  /**
  * @brief birthDate
  *
  */
  string birthDate;
  /**
  * @brief phoneNumber
  *
  */
  string phoneNumber;
  /**
  * @brief firstRegistrationDate
  *
  */
  string firstRegistrationDate;

};
/**
* @brief subsciprtionRecord
*
*/
struct subsciprtionRecord {
  /**
  * @brief memberID
  *
  */
  int memberID;
  /**
  * @brief startingDate
  *
  */
  string startingDate;
  /**
  * @brief finishingDate
  *
  */
  string finishingDate;
  /**
  * @brief subscriptionTier
  *
  */
  string subscriptionTier;

};
/**
* @brief memberID
*
*/
struct classRecord {
  /**
  * @brief tutor
  *
  */
  string tutor;
  /**
  * @brief date
  *
  */
  string date;
  /**
  * @brief startingHour
  *
  */
  string startingHour;
  /**
  * @brief finishingHour
  *
  */
  string finishingHour;
  /**
  * @brief studentList
  *
  */
  string studentList;
};
/**
* @brief paymentRecord
*
*/
struct paymentRecord {
  /**
  * @brief memberID
  *
  */
  int memberID;
  /**
  * @brief paidAmount
  *
  */
  int paidAmount;
  /**
  * @brief paymentDate
  *
  */
  string paymentDate;
  /**
  * @brief nextPaymentDate
  *
  */
  string nextPaymentDate;

};

/**
 *
 * @brief Huffman tree struct.
 *
*/
struct Node {
  /**
  * @brief data
  *
  */
  char data;
  /**
  * @brief freq
  *
  */
  int freq;
  /**
  * @brief left, right
  *
  */
  Node *left, *right;

  /**
   * @brief Constructs a new Node object.
   *
   * @param data Character stored in the node.
   * @param freq Frequency of the character.
   */
  Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

/**
 *
 * @brief Comparison function for the priority queue.
 *
*/
struct Compare {
  /**
   * @brief Operator () overload for comparing two Node pointers based on their frequencies.
   *
   * @param a Pointer to the first Node.
   * @param b Pointer to the second Node.
   * @return true if frequency of 'a' is less than frequency of 'b', else false.
   */
  bool operator()(Node *a, Node *b) {
    if (a->freq == b->freq)
      return a->data > b->data; // If frequencies are equal, order by character

    return a->freq > b->freq;
  }
};

/**
 *
 * @brief Struct for login menu.
 *
*/
struct login_menu_variables {
  /**
   * @brief a varible to control app's running state.
  */
  bool run = true;
  /**
  * @brief a varible for menu navigation.
  */
  int login_menu_login = 1;
  /**
  * @brief a varible for menu navigation.
  */
  int login_menu_register = 2;
  /**
  * @brief a varible for menu navigation.
  */
  int login_menu_password_reset = 3;
  /**
  * @brief a varible for menu navigation.
  */
  int login_menu_guest = 4;
  /**
  * @brief a varible for menu navigation.
  */
  int login_menu_exit = 5;

};
/**
 *
 * @brief Struct for main menu.
 *
*/
struct main_menu_variables {
  /**
  * @brief a varible to control app's running state.
  */
  bool logged_in = true;
  /**
  * @brief a varible for menu navigation.
  */
  int main_menu_member = 1;
  /**
  * @brief a varible for menu navigation.
  */
  int main_menu_subs = 2;
  /**
  * @brief a varible for menu navigation.
  */
  int main_menu_class = 3;
  /**
  * @brief a varible for menu navigation.
  */
  int main_menu_payment = 4;
  /**
  * @brief a varible for menu navigation.
  */
  int main_menu_log_out = 5;
};
/**
 *
 * @brief Struct for sub menus.
 *
*/
struct sub_menu_variables {
  /**
  * @brief a varible for menu navigation.
  */
  int sub_menu_show = 1;
  /**
  * @brief a varible for menu navigation.
  */
  int sub_menu_add = 2;
  /**
  * @brief a varible for menu navigation.
  */
  int sub_menu_edit = 3;
  /**
  * @brief a varible for menu navigation.
  */
  int sub_menu_delete = 4;
  /**
  * @brief a varible for menu navigation.
  */
  int sub_menu_return = 5;
};

/**
 * @brief Opens a binary file, deletes all of its content, and writes given text to it.
 *
 * @param file_name The name of the file to write.
 * @param text The text to write.
 * @param isFileNew check if a file new. if file is new it put "1-)" at start and "\n" at end
 * @return 0 on success.
 */
int file_write(string file_name, string text, bool isFileNew);

/**
 * @brief Opens a binary file, reads all of its content, separates lines with "\n", and writes them to console. Also returns the contents of the file as a string for unit tests.
 *
 * @param file_name The name of the file to read from.
 * @param print_to_console It is a variable to disabling wrting content to console.
 * @return The contents of the file as a statically allocated string.
 */
string file_read(const string file_name, char print_to_console);

/**
 * @brief Opens a binary file, reads all of its content, separates lines with "\n", and writes them to console. Also returns the contents of the file as a string for unit tests.
 *
 * @param file_name The name of the file to read from.
 *
 * @return The contents of the file as a statically allocated string.
 */
string file_read_for_test(const string file_name);

/**
 * @brief Appends given text to a binary file with a automatic calculated line number. Calcultes new lines line number by finding last lines line number.
 *
 *
 * @param file_name The name of the file to append to.
 * @param text The text to append to the file.
 * @return 0 on success.
 */
int file_append(string file_name, string text);

/**
 * @brief This function Opens a binary file, finds the line that user wants to edit and replace it wih new text.
 *
 *
 * @param file_name The name of the file to edit.
 * @param line_number_to_edit The line number to edit.
 * @param new_line The new text to replace the existing line.
 * @return 0 on success.
 */
int file_edit(string file_name, int line_number_to_edit, string new_line);

/**
 * @brief This function Opens a binary file, deletes the line user wanted and make adjustments on line number acordingly.
 *
 *
 * @param file_name The name of the file to delete the line from.
 * @param line_number_to_delete The line number to delete.
 * @return 0 on success.
 */
int file_line_delete(string file_name, int line_number_to_delete);

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
int user_register(string new_username = "None", string new_password = "None", string new_recovery_key = "None", string user_file = "user.bin");

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
int user_login(string username = "None", string password = "None", string user_file = "user.bin");

/**
 * @brief This function changes the password of a user.
 * @param recovery_key recovery_key.
 * @param new_password new password.
 * @param user_file file that contains user info.
 * @return 0 on success.
 * @return -1 on fail.
 */
int user_change_password(string recovery_key = "None", string new_password = "None", string user_file = "user.bin");

/**
 * @brief generate secret keys for OTP algorithm.
 *
 * @return ss as string.
 */
string generateSecretKey();

/**
 * @brief OTP algorithm.
 * @param secretKey genereted secret key
 * @param length wanted length of the otp
 * @return otp.
 */
string generateOTP(const string &secretKey, int length);

/**
 * @brief Compute the SHA-1 hash of a given input string.
 *
 * @param input The input string to hash.
 * @return The SHA-1 hash value as a hexadecimal string.
 */
string sha1(string &input);

/**
 * @brief main menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int main_menu(bool isGuestMode);

/**
 * @brief login menu.
 * @param isUnitTesting a bool to check if it is unit testing.
 * @return 0.
 */
int login_menu(bool isUnitTesting);

/**
 * @brief register menu.
 *
 * @return 0.
 */
int register_menu();

/**
 * @brief change pssword menu.
 *
 * @return 0.
 */
int change_password_menu();

/**
 * @brief member_menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int member_menu(bool isGuestMode);

/**
 * @brief subs menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int subs_menu(bool isGuestMode);


/**
 * @brief class menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int class_menu(bool isGuestMode);

/**
 * @brief payment menu.
 * @param isGuestMode a bool to check if user entered with guest mode.
 * @return 0.
 */
int payment_menu(bool isGuestMode);

/**
 * @brief a fucntion for adding member record.
 *
 * @return 0.
 */
int add_member_record();

/**
 * @brief a fucntion for editing member record.
 *
 * @return 0 on success.
 * @return -1 on fail.
 */
int edit_member_record();

/**
 * @brief a fucntion for deleting member record.
 *
 * @return 0.
 */
int delete_member_record();

/**
 * @brief a fucntion for adding subscription record.
 *
 * @return 0.
 */
int add_subs_record();

/**
 * @brief a fucntion for editing subscription record.
 *
 * @return 0 on success.
 * @return -1 on fail.
 */
int edit_subs_record();

/**
 * @brief a fucntion for deleting subscription record.
 *
 * @return 0.
 */
int delete_subs_record();

/**
 * @brief a fucntion for adding class record.
 *
 * @return 0.
 */
int add_class_record();

/**
 * @brief a fucntion for editing class record.
 *
 * @return 0.
 */
int edit_class_record();

/**
 * @brief a fucntion for deleting class record.
 *
 * @return 0.
 */
int delete_class_record();

/**
 * @brief a fucntion for adding payment record.
 *
 * @return 0.
 */
int add_payment_record();

/**
 * @brief a fucntion for editing payment record.
 *
 * @return 0.
 */
int edit_payment_record();

/**
 * @brief a fucntion for deleting payment record.
 *
 * @return 0.
 */
int delete_payment_record();

/**
 * @brief Calculates frequency of characters in the input text.
 *
 * @param text Input text.
 * @return unordered_map<char, int> Frequency map of characters.
 */
unordered_map<char, int> calculateFrequency(const string &text);

/**
 * @brief Builds the Huffman tree based on character frequencies.
 *
 * @param freqMap Frequency map of characters.
 * @return Node* Root of the Huffman tree.
 */
Node *buildHuffmanTree(const unordered_map<char, int> &freqMap);

/**
 * @brief Traverses the Huffman tree and builds the codewords.
 *
 * @param root Root of the Huffman tree.
 * @param code Current code.
 * @param codes Map to store character codes.
 */
void buildCodes(Node *root, string code, unordered_map<char, string> &codes);

/**
 * @brief Decodes the encoded text using Huffman codes.
 *
 * @param encodedText Encoded text.
 * @param root Root of the Huffman tree.
 * @return string Decoded text.
 */
string decode(const string &encodedText, Node *root);

/**
 * @brief Writes the Huffman tree to a file.
 *
 * @param outFile Output file stream.
 * @param node Current node being written.
 */
void writeTreeToFile(ofstream &outFile, Node *node);

/**
 * @brief Reads the Huffman tree from a file.
 *
 * @param inFile Input file stream.
 * @return Node* Root of the Huffman tree.
 */
Node *readTreeFromFile(ifstream &inFile);

/**
 * Finds the longest common subsequence (LCS) of two input strings.
 *
 *
 * @param text1 The first input string.
 * @param text2 The second input string.
 * @return The longest common subsequence of the input strings.
 */
string LCS(const string &text1, const string &text2);

/**
 * finds if file have a record that have a high LCS with text
 *
 *
 * @param text The input string.
 * @param file_name The file name.
 * @return 0 on success
 * @return -1 on fail
 */
int checkLCS(string text, string file_name);


/**
 * @brief Generates a One-Time Password (OTP) using the HMAC-Based One-Time Password (HOTP) algorithm.
 *
 * @param key A pointer to the shared secret key.
 * @param kl The length of the shared secret key.
 * @param interval The counter value (time interval or event count).
 * @param digits The number of digits for the generated OTP.
 * @return The generated OTP.
 */
uint32_t HOTP(uint8_t *key, size_t kl, uint64_t interval, int digits);

/**
 * @brief Generates an HMAC digest using the given key and interval.
 *
 * @param key A pointer to the shared secret key.
 * @param kl The length of the shared secret key.
 * @param interval The counter value (time interval or event count).
 * @return A pointer to the generated HMAC digest.
 */
uint8_t *hmac(unsigned char *key, int kl, uint64_t interval);

/**
 * @brief Extracts a 32-bit dynamic truncation value from the given HMAC digest.
 *
 * @param digest A pointer to the HMAC digest.
 * @return The 32-bit dynamic truncation value.
 */
uint32_t DT(uint8_t *digest);

/**
 * @brief Generates a Time-Based One-Time Password (TOTP) using the given key and current time.
 *
 * @param key A pointer to the shared secret key.
 * @param kl The length of the shared secret key.
 * @param time The current time or time interval.
 * @param digits The number of digits for the generated OTP.
 * @return The generated TOTP.
 */
uint32_t TOTP(uint8_t *key, size_t kl, uint64_t time, int digits);

/**
 * @brief Gets the current time since a reference time T0.
 *
 * @param T0 The reference time.
 * @return The current time since T0.
 */
time_t get_time(time_t T0);

#endif // FITNESS_MANAGEMENT_LIB_H
