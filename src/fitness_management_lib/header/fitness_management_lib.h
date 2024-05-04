/**
 * @file fitness_management_lib.h
 *
 * @brief Provides functions for fitness managament app.
 */

#ifndef FITNESS_MANAGEMENT_LIB_H
#define FITNESS_MANAGEMENT_LIB_H

#include <string>
/**
 * @brief Using the standard C++ namespace to simplify code with standard library elements.
 *
 */
using namespace std;

struct member_record {
  int memberID;
  string fullName;
  int birthDate;
  string phoneNumber;
  string firstRegistrationDate;

};

struct subsciprtion_record {
  int memberID;
  string startingDate;
  string finishingDate;
  string subscriptionTier;

};

struct class_record {
  string tutor;
  string date;
  string startingHour;
  string finishingHour;
  int studentList[100];
};

struct payment_record {
  int memberID;
  int paidAmount;
  string paymentDate;
  string nextPaymentDate;

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
struct manin_menu_variables {
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
 * @brief Opens a binary file, deletes all of its content and write given text to it.
 *
 *
 * @param file_name The name of the file to write.
 * @param text The text to write.
 * @return 0 on success.
 */
int file_write(string file_name, string text);

/**
 * @brief Opens a binary file, reads all of its content, separates lines with "\n", and writes them to console. Also returns the contents of the file as a string for unit tests.
 *
 * @param file_name The name of the file to read from.
 * @param print_to_console It is a variable to disabling wrting content to console.
 * @return The contents of the file as a statically allocated string.
 */
char *file_read(const char *file_name, char print_to_console);

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
 * @brief main menu.
 *
 * @return 0.
 */
int main_menu();

/**
 * @brief login menu.
 *
 * @return 0.
 */
int login_menu();

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
 *
 * @return 0.
 */
int member_menu();

/**
 * @brief subs menu.
 *
 * @return 0.
 */
int subs_menu();


/**
 * @brief class menu.
 *
 * @return 0.
 */
int class_menu();

/**
 * @brief payment menu.
 *
 * @return 0.
 */
int payment_menu();

#endif // FITNESS_MANAGEMENT_LIB_H
