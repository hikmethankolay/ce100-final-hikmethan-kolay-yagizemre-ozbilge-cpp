/**
 * @brief a variable to mute warnings.
 *
 */
#define _CRT_SECURE_NO_WARNINGS

#include "../header/fitness_management_lib.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <codecvt>
#include <random>
#include <sstream>
#include <iomanip>

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
 * @brief Opens a binary file, deletes all of its content, and writes given text to it.
 *
 * @param file_name The name of the file to write.
 * @param text The text to write.
 * @return 0 on success.
 */
int file_write(string file_name, string text) {
  text = "0-)" + text + "\n";
  myFile.open(file_name, ios::out | ios::binary | ios::trunc); // Opens file with output tag
  myFile.write(text.c_str(), text.length()); // Deletes everything inside file and writes text variable
  myFile.close();
  return 0;
}

/**
 * @brief Opens a binary file, reads all of its content, separates lines with "\n", and writes them to console. Also returns the contents of the file as a string for unit tests.
 *
 * @param file_name The name of the file to read from.
 * @param print_to_console It is a variable to disabling wrting content to console.
 * @return The contents of the file as a statically allocated string.
 */
char *file_read(const char *file_name, char print_to_console) {
  const int MAX_FILE_SIZE = 4096;
  static char content[MAX_FILE_SIZE]; // Static buffer for file content
  ifstream myFile(file_name, ios::binary);

  if (!myFile.is_open()) {
    std::cerr << "File operation failed, There is no record" << std::endl;
    return nullptr;
  }

  size_t length = 0;
  int ch;

  while ((ch = myFile.get()) != EOF && length < MAX_FILE_SIZE - 1) { // Ensure there's room for null terminator
    if (ch == '\r') continue; // Skip '\r'

    content[length++] = ch; // Append character to content
  }

  content[length] = '\0'; // Null-terminate the string

  if (print_to_console == 'Y') {
    cout << content; // Print the content to the console
  }

  myFile.close(); // Ensure the file is closed
  // Return a pointer to the static buffer.
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
  myFile.open(file_name, ios::in | ios::binary); // Opens file with input tag
  string lastLine;
  string currentLine;
  char i;

  if (myFile.is_open()) {
    while (myFile.get(i)) {
      if (i == '\n') {
        currentLine = currentLine + i;
        lastLine = currentLine;
        currentLine = "";
        continue;
      }

      currentLine = currentLine + i;
    }

    myFile.close();
  } else {
    cout << "\nFile operation failed";
    return -1;
  }

  size_t pos = lastLine.find("-)"); // Finds the location of "-)" in the last line
  int lineNumber = stoi(lastLine.substr(0, pos)) + 1; // Finds the number for the appended line
  text = to_string(lineNumber) + "-)" + text + "\n";
  myFile.open(file_name, ios::app | ios::binary); // Opens file with append tag
  myFile.write(text.c_str(), text.length()); // Appends text with its line number
  myFile.close();
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
  const int max_line_count = 100; // A variable for array
  char i;
  myFile.open(file_name, ios::in | ios::binary); // Opens file with read tag

  if (myFile.is_open()) {
    string lines[max_line_count]; // An array to store lines
    string line;
    int line_count = 0; // A variable for an if statement to check if the line that the user wants to edit exists

    while (myFile.get(i)) {
      if (i == '\n') {
        line = line + i;
        lines[line_count++] = line;
        line = "";
        continue;
      }

      line = line + i;
    }

    myFile.close();

    if (line_number_to_edit > 0 && line_number_to_edit <= line_count) {
      lines[line_number_to_edit] = to_string(line_number_to_edit) + "-)" + new_line + "\n"; // Changes a member of Lines array to a new line with its line number
    } else {
      cout << "\nYou can only edit existing lines";
      return -1;
    }

    myFile.open(file_name, ios::out | ios::binary); // Opens file in write mode

    for (const string &updated_lines : lines) {  // Writes every member of the lines array to the file
      if (updated_lines == "") {
        break; // Stops if there is nothing on the next line since arrays have fixed slots inside them from the start
      }

      myFile.write(updated_lines.c_str(), updated_lines.length());
    }

    myFile.close();
    cout << "\nData successfully edited";
    return 0;
  } else {
    cout << "\nFile operation failed";
    return -1;
  }
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
  const int max_line_count = 100; // A variable for an array to work properly
  char i;
  myFile.open(file_name, ios::in | ios::binary); // Opens file in read mode

  if (myFile.is_open()) {
    string lines[max_line_count]; // An array to store lines
    string line;
    int line_count = 0; // A variable for an if statement to check if there is a line that the user wants to delete

    while (myFile.get(i)) {// Gets lines one by one and assigns them to the line variable
      if (i == '\n') {
        line = line + i;
        lines[line_count++] = line; // Adds the line variable to the lines array and increases line_count after the operation
        line = "";
        continue;
      }

      line = line + i;
    }

    if (line_number_to_delete > 0 && line_number_to_delete < line_count) {
      // Shift elements to "erase" the line at line_number_to_delete
      for (int i = line_number_to_delete; i < line_count - 1; ++i) {
        lines[i] = lines[i + 1];
      }

      lines[line_count - 1] = ""; // Clears the last element of lines so the same thing wouldn't write to the file twice
    } else {
      cout << "\nYou can only erase existing lines";
      myFile.close();
      return -1;
    }

    myFile.close();
    myFile.open(file_name, ios::out | ios::binary); // Opens the file in write mode

    for (const string &updated_lines : lines) {
      if (updated_lines == "") {
        break; // Stops if there is nothing on the next line since arrays have fixed slots inside them from the start
      }

      size_t pos = updated_lines.find("-)"); // Finds the position of "-)"
      int lineNumber = stoi(updated_lines.substr(0, pos)); // Finds each line's line number

      if (lineNumber > line_number_to_delete) { // Decreases a line's line number if it's bigger than the deleted line's line number
        string updated_line_with_new_number = to_string(lineNumber - 1) + updated_lines.substr(pos);
        myFile.write(updated_line_with_new_number.c_str(), updated_line_with_new_number.length());
      } else {
        myFile.write(updated_lines.c_str(), updated_lines.length());
      }
    }

    cout << "\nData successfully deleted";
    myFile.close();
    return 0;
  } else {
    cout << "\nFile operation failed";
    return -1;
  }
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
  string login_info;
  login_info = new_username + "/" + new_password + "/" + new_recovery_key;
  myFile.open(user_file, ios::out | ios::binary | ios::trunc); // Opens file with output tag
  myFile.write(login_info.c_str(), login_info.length()); // Deletes everything inside file and writes login_info variable
  myFile.close();
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
  char i;
  myFile.open(user_file, ios::in | ios::binary); // Opens file with input tag

  if (!myFile.is_open()) {
    cout << "\nThere is no user info, Please register first.\n";
    return -1;
  }

  while (myFile.get(i)) {
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

  myFile.close();

  if (username == "None" && password == "None") {
    cout << "\nPlease enter username:";
    cin >> username;
    cout << "\nPlease enter password:";
    cin >> password;
  }

  if (username == username_read && password == password_read) {
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
  myFile.open(user_file, ios::in | ios::binary); // Opens file with input tag

  if (myFile.is_open()) {
    char i;

    while (myFile.get(i)) {
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

    myFile.close();
  } else {
    cout << "\nThere is no user info, Please register first.\n";
    return -1;
  }

  if (recovery_key == "None") {
    cout << "\nPlease enter your recovery key:";
    cin >> recovery_key;
  }

  if (recovery_key_read == recovery_key) {
    cout << "\nRecovey Key Approved\n";

    if (new_password == "None") {
      cout << "\nPlease enter a new password:";
      cin >> new_password;
    }

    new_login_info = username_read + "/" + new_password + "/" + recovery_key_read;
    myFile.open(user_file, ios::out | ios::binary | ios::trunc); // Opens file with output tag
    myFile.write(new_login_info.c_str(), new_login_info.length()); // Deletes everything inside file and writes login_info variable
    myFile.close();
    cout << "\nPassword changed succesfully";
    return 0;
  } else {
    cout << "\nWrong Recovery Key";
    return -1;
  }
}

/**
 * @brief generate secret keys for OTP algorithm.
 *
 * @return ss as string.
 */
string generateSecretKey() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, 255);
  stringstream ss;

  for (int i = 0; i < 16; ++i) {
    ss << setw(2) << setfill('0') << hex << dis(gen);
  }

  return ss.str();
}

/**
 * @brief OTP algorithm.
 * @param secretKey genereted secret key
 * @param length wanted length of the otp
 * @return otp.
 */
string generateOTP(const string &secretKey, int length) {
  string allowedCharacters = "0123456789";
  string otp;
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<size_t> dis(0, allowedCharacters.length() - 1);

  for (int i = 0; i < length; ++i) {
    size_t index = static_cast<size_t>(dis(gen));
    otp += allowedCharacters[index];
  }

  return otp;
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
    cout << "\n4-)Payemnt Processing";
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
      file_read("membership_records.bin",'Y');
      continue;
    } else if (isGuestMode == false) {
      if (choice_member == sub_menu.sub_menu_add) {
        add_member_record();
        continue;
      } else if (choice_member == sub_menu.sub_menu_edit) {
        edit_member_record();
        continue;
      } else if (choice_member == sub_menu.sub_menu_delete) {
        delete_member_record();
        continue;
      }
    } else if (choice_member == sub_menu.sub_menu_return) {
      break;
    } else if (isGuestMode == true) {
      cout << "\nYou can only see records while in guest mode.";
      continue;
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
  cout << "\nPlease enter memberID:";
  cin >> member.memberID;
  cout << "\nPlease enter full name:";
  cin >> member.fullName;
  cout << "\nPlease enter birth date:";
  cin >> member.birthDate;
  cout << "\nPlease enter phone number:";
  cin >> member.phoneNumber;
  cout << "\nPlease enter first registration date:";
  cin >> member.firstRegistrationDate;
  // Format the string first
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << member.memberID
                  << " / Full name:" << member.fullName
                  << " / Birth date:" << member.birthDate
                  << " / Phone number:" << member.phoneNumber
                  << " / First registration date:" << member.firstRegistrationDate;
  string result = formattedRecord.str();
  FILE *myFile;
  myFile = fopen("member_records.bin", "rb");

  if (myFile == NULL) {
    file_write("member_records.bin", result);
    return 0;
  } else {
    fclose(myFile);
    file_append("member_records.bin", result);
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
  cout << "\nPlease enter record number to edit:";
  cin >> RecordNumberToEdit;
  cout << "\nPlease enter memberID:";
  cin >> member.memberID;
  cout << "\nPlease enter full name:";
  cin >> member.fullName;
  cout << "\nPlease enter birth date:";
  cin >> member.birthDate;
  cout << "\nPlease enter phone number:";
  cin >> member.phoneNumber;
  cout << "\nPlease enter first registration date:";
  cin >> member.firstRegistrationDate;
  // Format the string first
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << member.memberID
                  << " / Full name:" << member.fullName
                  << " / Birth date:" << member.birthDate
                  << " / Phone number:" << member.phoneNumber
                  << " / First registration date:" << member.firstRegistrationDate;
  string result = formattedRecord.str();

  if (file_edit("member_records.bin", RecordNumberToEdit, result) == 0) {
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

  if (file_line_delete("member_records.bin", RecordNumberToDelete) == 0) {
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
      file_read("subscription_records.bin",'Y');
      continue;
    } else if (isGuestMode == false) {
      if (choice_sub == sub_menu.sub_menu_add) {
        add_subs_record();
        continue;
      } else if (choice_sub == sub_menu.sub_menu_edit) {
        edit_subs_record();
        continue;
      } else if (choice_sub == sub_menu.sub_menu_delete) {
        delete_subs_record();
        continue;
      }
    } else if (choice_sub == sub_menu.sub_menu_return) {
      break;
    } else if (isGuestMode == true) {
      cout << "\nYou can only see records while in guest mode.";
      continue;
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
  // Format the string first
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << sub.memberID
                  << " / Starting date:" << sub.startingDate
                  << " / Finishing date:" << sub.finishingDate
                  << " / Subcription tier:" << sub.subscriptionTier;
  string result = formattedRecord.str();
  FILE *myFile;
  myFile = fopen("subciption_records.bin", "rb");

  if (myFile == NULL) {
    file_write("subciption_records.bin", result);
    return 0;
  } else {
    fclose(myFile);
    file_append("subciption_records.bin", result);
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
  // Format the string first
  ostringstream formattedRecord;
  formattedRecord << "MemberID:" << sub.memberID
                  << " / Starting date:" << sub.startingDate
                  << " / Finishing date:" << sub.finishingDate
                  << " / Subcription tier:" << sub.subscriptionTier;
  string result = formattedRecord.str();

  if (file_edit("subciption_records.bin", RecordNumberToEdit, result) == 0) {
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

  if (file_line_delete("subciption_records.bin", RecordNumberToDelete) == 0) {
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
      file_read("class_records.bin",'Y');
      continue;
    } else if (isGuestMode == false) {
      if (choice_class == sub_menu.sub_menu_add) {
        add_class_record();
        continue;
      } else if (choice_class == sub_menu.sub_menu_edit) {
        edit_class_record();
        continue;
      } else if (choice_class == sub_menu.sub_menu_delete) {
        delete_class_record();
        continue;
      }
    } else if (choice_class == sub_menu.sub_menu_return) {
      break;
    } else if (isGuestMode == true) {
      cout << "\nYou can only see records while in guest mode.";
      continue;
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
    classRecord class;
    cout << "\nPlease enter tutor";
    cin >> class.tutor;
    cout << "\nPlease enter date";
    cin >> class.date;
    cout << "\nPlease enter starting hour";
    cin >> class.startingHour;
    cout << "\nPlease enter finishing hour";
    cin >> class.finishingHour;
    cout << "\nPlease enter student list";
    cin >> class.studentList[100];
    // Format the string first
    ostringstream formattedRecord;
    formattedRecord << "/ tutor:" << class.tutor;
                    << "/ date:" << class.date;
                    << "/ Starting Hour:" << class.startingHour;
                    << "/ Finishing Hour:" << class.finishingHour;
                    << "Student List" << class.studentList[100];
   string result = formattedRecord.str();
   FILE* myFile;
   myFile = fopen("class_records.bin", "rb");
   if (myFile == NULL) {
       file_write("class_records.bin", result);
       return 0;
   } else {
       fclose(myFile);
       file_append("class_records.bin", result);
       return 0;
   }
};

/**
 * @brief a fucntion for editing class record.
 *
 * @return 0.
 */
int edit_class_record() {
    classRecord class;
    int RecordNumberToEdit;
    cout << "\nPlease enter a record number to edit:";
    cin >> RecordNumberToEdit;
    cout << "\nPlease enter tutor";
    cin >> class.tutor;
    cout << "\nPlease enter date";
    cin >> class.date;
    cout << "\nPlease enter starting hour";
    cin >> class.startingHour;
    cout << "\nPlease enter finishing hour";
    cin >> class.finishingHour;
    cout << "\nPlease enter student list";
    cin >> class.studentList[100];
    // Format the string first
    ostringstream formattedRecord;
    formattedRecord << "/ tutor:" << class.tutor;
                    << "/ date:" << class.date;
                    << "/ Starting Hour:" << class.startingHour;
                    << "/ Finishing Hour:" << class.finishingHour;
                    << "Student List" << class.studentList[100];
    string result = formattedRecord.str();

    if (file_edit("class_records.bin", RecordNumberToEdit, result) == 0) {
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

    if (file_line_delete("class_records.bin", RecordNumberToDelete) == 0) {
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
      file_read("payment_records.bin",'Y');
      continue;
    } else if (isGuestMode == false) {
      if (choice_payment == sub_menu.sub_menu_add) {
        add_payment_record();
        continue;
      } else if (choice_payment == sub_menu.sub_menu_edit) {
        edit_payment_record();
        continue;
      } else if (choice_payment == sub_menu.sub_menu_delete) {
        delete_payment_record();
        continue;
      }
    } else if (choice_payment == sub_menu.sub_menu_return) {
      break;
    } else if (isGuestMode == true) {
      cout << "\nYou can only see records while in guest mode.";
      continue;
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
    // Format the string first
    ostringstream formattedRecord;
    formattedRecord << "/ memberID" << payment.memberID;
    << "/ paid amount" << payment.paidAmount;
    << "/ payment date:" << payment.paymentDate;
    << "/ next payment date:" << payment.nextPaymentDate;
    string result = formattedRecord.str();
    FILE* myFile;
    myFile = fopen("payment_records.bin", "rb");
    if (myFile == NULL) {
        file_write("payment_records.bin", result);
        return 0;
    }
    else {
        fclose(myFile);
        file_append("payment_records.bin", result);
        return 0;
    }
};

/**
 * @brief a fucntion for editing payment record.
 *
 * @return 0.
 */
int edit_payment_record() {
  return 0;
};

/**
 * @brief a fucntion for deleting payment record.
 *
 * @return 0.
 */
int delete_payment_record() {
  return 0;
};

/**
 * @brief login menu.
 * @param isUnitTesting a bool to check if it is unit testing.
 * @return 0.
 */
int login_menu(bool isUnitTesting) {
  string user_name;
  string password;
  string user_file = "user.bin";
  cout << "\nPlease enter your username:";
  cin >> user_name;
  cout << "\nPlease enter your password:";
  cin >> password;

  if (user_login(user_name,password,user_file) == 0) {
    string secretKey = generateSecretKey();
    string otp = generateOTP(secretKey, 6);
    string user_input_otp;
    cout << "\nPlease enter single use code that we send you";
    cout << "\n" << otp << " is the code, this is just the simulation of scenario:";

    if (isUnitTesting) {
      user_input_otp = otp;
    } else {
      cin >> user_input_otp;
    }

    if (user_input_otp == otp) {
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
  string user_file = "user.bin";
  char warning;
  cout << "Please enter your new username:";
  cin >> user_name;
  cout << "\nPlease enter your new password:";
  cin >> password;
  cout << "\nPlease enter your new recovery key:";
  cin >> recovery_key;
  cout << "\n------------WARNING------------";
  cout << "\nThis process will delete all previous records, do you still wish to proceed?[Y/n]:";
  scanf(" %c", &warning);

  if (warning == 'Y') {
    user_register(user_name,password,recovery_key,user_file);
  } else {
    cout << "Process terminated.";
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
  string user_file = "user.bin";
  cout << "Please enter your recovery key:";
  cin >> recovery_key;
  cout << "\nPlease enter your new password:";
  cin >> password;
  user_change_password(recovery_key,password,user_file);
  return 0;
};
