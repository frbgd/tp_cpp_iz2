#include "gtest/gtest.h"

extern "C" {
#include "../include/employee.h"
}

// TODO тесты для array

TEST(number_reading, CorrectFileReading) {
    int employees_number = read_employees_number_from_file(const_cast<char*>("test_data1k.bin"));
    ASSERT_NE(employees_number, -1);
}

TEST(number_reading, IncorrectFileOpening) {
    int employees_number = read_employees_number_from_file(const_cast<char*>("wrong_path"));
    ASSERT_EQ(employees_number, -1);
}

TEST(number_reading, IncorrectFileReading) {
    int employees_number = read_employees_number_from_file(const_cast<char*>("test_data_invalid.bin"));
    ASSERT_EQ(employees_number, -1);
}

TEST(employees_reading, CorrectFileReading) {
    employee_array employee_list;
    int employees_number = 1000;
    init_array(&employee_list, employees_number);
    int read_result = read_employees_from_file(const_cast<char*>("test_data1k.bin"), employees_number, &employee_list);
    ASSERT_NE(employee_list.used, 0);
    ASSERT_EQ(read_result, 0);
}

TEST(employees_reading, IncorrectFileOpening) {
    employee_array employee_list;
    int employees_number = 1000;
    init_array(&employee_list, employees_number);
    int read_result = read_employees_from_file(const_cast<char*>("wrong_path"), employees_number, &employee_list);
    ASSERT_EQ(employee_list.used, 0);
    ASSERT_EQ(read_result, -1);
}

TEST(employees_reading, IncorrectFileReading) {
    employee_array employee_list;
    int employees_number = 1000;
    init_array(&employee_list, employees_number);
    int read_result = read_employees_from_file(const_cast<char*>("wrong_path"), employees_number, &employee_list);
    ASSERT_EQ(employee_list.used, 0);
    ASSERT_EQ(read_result, -1);
}

TEST(employees, FirstPositionMinAgeEmployee) {

}

TEST(employees, FirstPositionMaxAgeEmployee) {

}

TEST(employees, SecondPositionMinAgeEmployee) {

}

TEST(employees, SecondPositionMaxAgeEmployee) {

}

TEST(employees, ThirdPositionMinAgeEmployee) {

}

TEST(employees, ThirdPositionMaxAgeEmployee) {

}

TEST(employees, CorrectSecondNamesSort) {

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
