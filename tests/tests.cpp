#include "gtest/gtest.h"

extern "C" {
#include "../include/employee.h"
}

TEST(array, Initializing) {
    employee_array t;
    size_t init_size = 20;
    init_array(&t, init_size);
    ASSERT_NE(t.array, nullptr);
    ASSERT_EQ(t.used, 0);
    ASSERT_EQ(t.size, init_size);
}

TEST(array, Inserting) {
    employee_array arr;
    size_t init_size = 20;
    init_array(&arr, init_size);

    size_t arr_size = arr.size;
    size_t arr_used = arr.used;

    employee empl = {
            "Junior Python",
            23,
            3,
            85000,
            "Kucheryavenko",
            true,
            "Aleksey"
    };
    insert_array(&arr, empl);

    ASSERT_EQ(arr.used, arr_used + 1);
    ASSERT_EQ(arr.size, arr_size);
    ASSERT_STREQ(empl.position, arr.array[arr_used].position);
    ASSERT_EQ(empl.age, arr.array[arr_used].age);
    ASSERT_EQ(empl.work_experience, arr.array[arr_used].work_experience);
    ASSERT_EQ(empl.salary, arr.array[arr_used].salary);
    ASSERT_STREQ(empl.second_name, arr.array[arr_used].second_name);
    ASSERT_EQ(empl.is_male, arr.array[arr_used].is_male);
    ASSERT_STREQ(empl.first_name, arr.array[arr_used].first_name);
}

TEST(array, InsertingWithReassigning) {
    employee_array arr;
    size_t init_size = 2;
    init_array(&arr, init_size);
    employee empl = {
            "Junior Python",
            23,
            3,
            85000,
            "Kucheryavenko",
            true,
            "Aleksey"
    };
    insert_array(&arr, empl);
    insert_array(&arr, empl);

    size_t arr_size = arr.size;
    size_t arr_used = arr.used;
    insert_array(&arr, empl);

    ASSERT_EQ(arr.used, arr_used + 1);
    ASSERT_EQ(arr.size, arr_size * 2);
}

TEST(array, Slicing) {
    employee_array input_arr;
    size_t init_size = 10;
    init_array(&input_arr, init_size);
    employee empl = {
            "Junior Python",
            23,
            3,
            85000,
            "Kucheryavenko",
            true,
            "Aleksey"
    };
    insert_array(&input_arr, empl);
    insert_array(&input_arr, empl);
    insert_array(&input_arr, empl);

    employee_array output_arr;
    init_array(&output_arr, init_size);
    slice_array(&input_arr, &output_arr, 1, 2);

    ASSERT_STREQ(input_arr.array[1].second_name, output_arr.array[0].second_name);
    ASSERT_EQ(output_arr.used, 2);
}

TEST(array, Free) {
    employee_array arr;
    size_t init_size = 10;
    init_array(&arr, init_size);
    employee empl = {
            "Junior Python",
            23,
            3,
            85000,
            "Kucheryavenko",
            true,
            "Aleksey"
    };
    insert_array(&arr, empl);

    free_array(&arr);
    ASSERT_EQ(arr.array, nullptr);
    ASSERT_EQ(arr.used, 0);
    ASSERT_EQ(arr.size, 0);
}

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
