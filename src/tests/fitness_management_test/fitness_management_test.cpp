#define _CRT_SECURE_NO_WARNINGS
//#define ENABLE_FITNESS_TEST  // Uncomment this line to enable the Calculator tests

#include "gtest/gtest.h"
#include "../../fitness_management_lib/header/fitness_management_lib.h"  // Adjust this include path based on your project structure
#include <unordered_set>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <cstring>

class FitnessTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Setup test data
  }

  void TearDown() override {
    // Clean up test data
  }
};

/**
 * @brief Variables for tests.
 */
int fail = -1;
/**
 * @brief Variables for tests.
 */
int success = 0;

/**
 * @brief Tests the file_read function.
 */
TEST_F(FitnessTest, TestFileRead) {
  string testString = "1-)TEXT STRING1\n2-)TEXT STRING2\n3-)TEXT STRING3\n4-)TEXT STRING4\n5-)TEXT STRING5\n";
  EXPECT_EQ(testString, file_read("test1", 'N'));
}

/**
 * @brief Tests the file_append function.
 */
TEST_F(FitnessTest, TestFileAppend) {
  string testString = "1-)TEXT STRING1\n2-)TEXT STRING2\n3-)TEXT STRING3\n4-)TEXT STRING4\n5-)TEXT STRING5\n6-)TEXT STRING6\n";
  string appendString = "TEXT STRING6";
  file_append("test2", appendString);
  EXPECT_EQ(testString, file_read("test2", 'N'));
}

/**
 * @brief Tests the file_edit function.
 */
TEST_F(FitnessTest, TestFileEdit) {
  string testString = "1-)TEXT STRING1\n2-)TEXT STRING2\n3-)TEXT STRING EDIT\n4-)TEXT STRING4\n5-)TEXT STRING5\n";
  string editString = "TEXT STRING EDIT";
  file_edit("test3", 3, editString);
  EXPECT_EQ(testString, file_read("test3", 'N'));
}

/**
 * @brief Tests the file_line_delete function.
 */
TEST_F(FitnessTest, TestFileDelete) {
  string testString = "1-)TEXT STRING2\n2-)TEXT STRING3\n3-)TEXT STRING4\n4-)TEXT STRING5\n";
  file_line_delete("test4", 1);
  EXPECT_EQ(testString, file_read("test4", 'N'));
}

/**
 * @brief Tests the file_write function.
 */
TEST_F(FitnessTest, TestFileWrite) {
  string testString = "1-)TEXT STRING WRITE\n";
  string writeString = "TEXT STRING WRITE";
  file_write("test5", writeString, true);
  EXPECT_EQ(testString, file_read("test5", 'N'));
}

/**
 * @brief Tests the file_read function in a fail case.
 */
TEST_F(FitnessTest, TestFileReadFail) {
  ASSERT_EQ("-1", file_read("test1f", 'N'));
}

/**
 * @brief Tests the file_append function in a fail case.
 */
TEST_F(FitnessTest, TestFileAppendFail) {
  string appendString = "TEXT STRING5";
  ASSERT_EQ(fail, file_append("test2f", appendString));
}

/**
 * @brief Tests the file_edit function in a fail case.
 */
TEST_F(FitnessTest, TestFileEditFail) {
  string editString = "TEXT STRING EDIT";
  ASSERT_EQ(fail, file_edit("test3f", 3, editString));
}

/**
 * @brief Tests the file_edit function in a fail case (line number out of range).
 */
TEST_F(FitnessTest, TestFileEditFail_2) {
  string editString = "TEXT STRING EDIT";
  ASSERT_EQ(fail, file_edit("test3", 100, editString));
}

/**
 * @brief Tests the file_line_delete function in a fail case.
 */
TEST_F(FitnessTest, TestFileDeleteFail) {
  ASSERT_EQ(fail, file_line_delete("test4f", 2));
}

/**
 * @brief Tests the file_line_delete function in a fail case (line number out of range).
 */
TEST_F(FitnessTest, TestFileDeleteFail_2) {
  ASSERT_EQ(fail, file_line_delete("test4", 100));
}

/**
 * @brief Tests the user_register function.
 */
TEST_F(FitnessTest, TestUserRegister) {
  string testString = "249ba36000029bbe97499c03db5a9001f6b734ec/5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8/29773a093e4c80375be4dd8738f4c6afb993b319";
  user_register("username", "password", "recoverykey", "usertest");
  EXPECT_EQ(testString, file_read("usertest", 'N'));
}

/**
 * @brief Tests the user_change_password function.
 */
TEST_F(FitnessTest, TestUserChangePassword) {
  string testString = "249ba36000029bbe97499c03db5a9001f6b734ec/f2c57870308dc87f432e5912d4de6f8e322721ba/29773a093e4c80375be4dd8738f4c6afb993b319";
  user_change_password("recoverykey", "newpassword", "usertest2");
  EXPECT_EQ(testString, file_read("usertest2", 'N'));
}

/**
 * @brief Tests the user_change_password function in a fail case
 */
TEST_F(FitnessTest, TestUserChangePasswordFail) {
  EXPECT_EQ(fail, user_change_password("recoverykey", "newpassword", "usertestttt"));
}

/**
 * @brief Tests the user_change_password function in a fail case
 */
TEST_F(FitnessTest, TestUserChangePasswordFail_2) {
  EXPECT_EQ(fail, user_change_password("recoverykeyaa", "newpassword", "usertest2"));
}

/**
 * @brief Tests the user_login function.
 */
TEST_F(FitnessTest, TestUserLogin) {
  EXPECT_EQ(success, user_login("username", "password", "usertest3"));
}

/**
 * @brief Tests the user_login function in a fail case
 */
TEST_F(FitnessTest, TestUserLoginFail) {
  EXPECT_EQ(fail, user_login("username", "passwordaa", "usertest3"));
}

/**
 * @brief Tests the user_login function in a fail case
 */
TEST_F(FitnessTest, TestUserLoginFail_2) {
  EXPECT_EQ(fail, user_login("usernameaa", "password", "usertest3"));
}

/**
 * @brief Tests the user_login function in a fail case
 */
TEST_F(FitnessTest, TestUserLoginFail_3) {
  EXPECT_EQ(fail, user_login("username", "password", "usertestttt"));
}

/**
 * @brief Test case to verify OTP length
 */
TEST_F(FitnessTest, TestOTPLength) {
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
  string OTP = to_string(otp);
  EXPECT_EQ(OTP.length(), digits);
}

/**
 * @brief Test case to verify OTP uniqueness
 */
TEST_F(FitnessTest, TestOTPUniqueness) {
  unsigned char key[] = "MZUXI3TFONZW2YLOMFTWK3LFNZ2HGZLDOJSXI33UOBVWK6I=";
  size_t key_len = strlen((char *)key);
  // Define the number of digits for the OTP
  int digits = 6;  // Typically 6 or 8
  // Get the current time interval since the epoch (T0)
  time_t t0 = 0;  // Reference time (epoch)
  unordered_set<string> otpSet;
  const int numIterations = 100;

  for (int i = 0; i < numIterations; ++i) {
    // Update the current time interval for each iteration
    uint64_t current_time_interval = get_time(t0) + i;
    // Generate the TOTP
    uint32_t otp = TOTP(key, key_len, current_time_interval, digits);
    string OTP = to_string(otp);
    otpSet.insert(OTP);
  }

  EXPECT_EQ(otpSet.size(), numIterations);
}

/**
 * @brief Tests the SHA1 function for same strings to look if they output same hexadecimel code.
 */
TEST_F(FitnessTest, TestSHA1) {
  string testString = "TEST STRING";
  string testString2 = "TEST STRING";
  EXPECT_EQ(sha1(testString),sha1(testString2));
}

/**
 * @brief Tests the SHA1 function for diffirent strings to look if they output diffirent hexadecimel code.
 */
TEST_F(FitnessTest, TestSHA1Fail) {
  string testString = "TEST STRING";
  string testString2 = "test string";
  EXPECT_NE(sha1(testString),sha1(testString2));
}

/**
 * @brief Tests the checkLCS function for its success case
 */
TEST_F(FitnessTest, TestLCS) {
  string testString = "3-)TEXT STRING3";
  EXPECT_EQ(success, checkLCS(testString,"test1"));
}

/**
 * @brief Tests the checkLCS function for its faill case
 */
TEST_F(FitnessTest, TestLCSFail) {
  string testString = "TEXT";
  EXPECT_EQ(fail, checkLCS(testString,"test1"));
}

/**
 * @brief Test for login menu
 */
TEST_F(FitnessTest, TestLoginMenu) {
  fflush(stdout);
  freopen("login_menu_output_test.bin", "wb", stdout);
  freopen("login_menu_input_test.bin", "rb", stdin);
  login_menu(true);
#ifdef _WIN32
  freopen("CON", "w", stdout);
  fflush(stdout);
  freopen("CON", "r", stdin);
#else
  freopen("/dev/tty", "w", stdout);
  fflush(stdout);
  freopen("/dev/tty", "r", stdin);
#endif
  EXPECT_EQ(file_read_for_test("login_menu_expected_output"), file_read_for_test("login_menu_output_test"));
}

/**
 * @brief Test for register menu
 */
TEST_F(FitnessTest, TestRegisterMenu) {
  fflush(stdout);
  freopen("register_menu_output_test.bin", "wb", stdout);
  freopen("register_menu_input_test.bin", "rb", stdin);
  register_menu();
#ifdef _WIN32
  freopen("CON", "w", stdout);
  fflush(stdout);
  freopen("CON", "r", stdin);
#else
  freopen("/dev/tty", "w", stdout);
  fflush(stdout);
  freopen("/dev/tty", "r", stdin);
#endif
  EXPECT_EQ(file_read_for_test("register_menu_expected_output"), file_read_for_test("register_menu_output_test"));
}

/**
 * @brief Test for Change password menu
 */
TEST_F(FitnessTest, TestChangePasswordMenu) {
  fflush(stdout);
  freopen("password_menu_output_test.bin", "wb", stdout);
  freopen("password_menu_input_test.bin", "rb", stdin);
  change_password_menu();
#ifdef _WIN32
  freopen("CON", "w", stdout);
  fflush(stdout);
  freopen("CON", "r", stdin);
#else
  freopen("/dev/tty", "w", stdout);
  fflush(stdout);
  freopen("/dev/tty", "r", stdin);
#endif
  EXPECT_EQ(file_read_for_test("password_menu_expected_output"), file_read_for_test("password_menu_output_test"));
}

/**
 * @brief Test for guest mode
 */
TEST_F(FitnessTest, TestGuestMode) {
  fflush(stdout);
  freopen("guest_menu_output_test.bin", "wb", stdout);
  freopen("guest_menu_input_test.bin", "rb", stdin);
  main_menu(true);
#ifdef _WIN32
  freopen("CON", "w", stdout);
  fflush(stdout);
  freopen("CON", "r", stdin);
#else
  freopen("/dev/tty", "w", stdout);
  fflush(stdout);
  freopen("/dev/tty", "r", stdin);
#endif
  EXPECT_EQ(file_read_for_test("guest_menu_expected_output"), file_read_for_test("guest_menu_output_test"));
}

/**
 * @brief Test for member menu
 */
TEST_F(FitnessTest, TestMemberMenu) {
  fflush(stdout);
  freopen("member_menu_output_test.bin", "wb", stdout);
  freopen("member_menu_input_test.bin", "rb", stdin);
  member_menu(false);
#ifdef _WIN32
  freopen("CON", "w", stdout);
  fflush(stdout);
  freopen("CON", "r", stdin);
#else
  freopen("/dev/tty", "w", stdout);
  fflush(stdout);
  freopen("/dev/tty", "r", stdin);
#endif
  EXPECT_EQ(file_read_for_test("member_menu_expected_output"), file_read_for_test("member_menu_output_test"));
}

/**
 * @brief Test for subs menu
 */
TEST_F(FitnessTest, TestSubsMenu) {
  fflush(stdout);
  freopen("subs_menu_output_test.bin", "wb", stdout);
  freopen("subs_menu_input_test.bin", "rb", stdin);
  subs_menu(false);
#ifdef _WIN32
  freopen("CON", "w", stdout);
  fflush(stdout);
  freopen("CON", "r", stdin);
#else
  freopen("/dev/tty", "w", stdout);
  fflush(stdout);
  freopen("/dev/tty", "r", stdin);
#endif
  EXPECT_EQ(file_read_for_test("subs_menu_expected_output"), file_read_for_test("subs_menu_output_test"));
}

/**
 * @brief Test for class menu
*/
TEST_F(FitnessTest, TestClassMenu) {
  freopen("class_menu_output_test.bin", "wb", stdout);
  freopen("class_menu_input_test.bin", "rb", stdin);
  class_menu(false);
#ifdef _WIN32
  freopen("CON", "w", stdout);
  freopen("CON", "r", stdin);
#else
  freopen("/dev/tty", "w", stdout);
  freopen("/dev/tty", "r", stdin);
#endif
  EXPECT_EQ(file_read_for_test("class_menu_expected_output"), file_read_for_test("class_menu_output_test"));
}

/**
 * @brief Test for payment menu
 */
TEST_F(FitnessTest, TestPaymentMenu) {
  fflush(stdout);
  freopen("payment_menu_output_test.bin", "wb", stdout);
  freopen("payment_menu_input_test.bin", "rb", stdin);
  payment_menu(false);
#ifdef _WIN32
  freopen("CON", "w", stdout);
  fflush(stdout);
  freopen("CON", "r", stdin);
#else
  freopen("/dev/tty", "w", stdout);
  fflush(stdout);
  freopen("/dev/tty", "r", stdin);
#endif
  EXPECT_EQ(file_read_for_test("payment_menu_expected_output"), file_read_for_test("payment_menu_output_test"));
}

/**
 * @brief The main function of the test program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument std::strings.
 * @return int The exit status of the program.
 */
int main(int argc, char **argv) {
#ifdef ENABLE_FITNESS_TEST
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(color) = "no";
  return RUN_ALL_TESTS();
#else
  return 0;
#endif
}
