#include <sstream>
#include "School.h"

// init the student number counter
unsigned int School::Student::nextStudentNumber = 9312u;

// adds a student to the course
// return false if the student is already enrolled in the course
bool School::Course::addStudent(std::weak_ptr<Student> wp) {
    // check that the student isn't already in the course
    auto result = std::find_if(studentsInCourse.begin(), studentsInCourse.end(), [wp](std::weak_ptr<Student> s) {
        if (wp.lock() == s.lock()) return true;
        return false;
    });
    if (result != studentsInCourse.end()) return false;
    // add the student to the course.
    studentsInCourse.push_back(wp);
    return true;
}

// method to print the students in a course
void School::Course::printRoll() const {
    std::cout << courseCode << " Roll" << std::endl;
    for (auto wp : studentsInCourse) {
        auto sp = wp.lock();
        /*weak pointers must be converted to std::shared_ptr
        in order to access the referenced object.*/
        if (sp != nullptr) {
            std::cout << sp->getName() << std::endl;
        }
    }
}

// method to add a course that this student is taking.
// Pre-Condition: The student has already been enrolled in the course.
// Pre-Condition: The student hasn't already had this course added to their timetable
bool School::Student::addCourse(std::weak_ptr<Course> wp) {
    coursesEnrolledIn.push_back(wp);
    return true;
}

// method to print a student's timetable
void School::Student::printTimetable() const {
    std::cout << "Timetable for " << name << " " << studentNumber << std::endl;
    for (auto c : coursesEnrolledIn) {
        auto sp = c.lock();    // convert the weak_ptr to a shared_ptr
        if (sp != nullptr) {    // if the convert was successful then print the course
            std::cout << sp->getCourseCode() << std::endl;
        }
    }
}

// create a course object and add it to the list of courses
// return false if the course already exists
bool School::addCourse(const std::string &courseCode) {
    // TODO: double check that the course doesn't already exist.
    // create a new course object
    auto result = std::find_if(courses.begin(), courses.end(),
                               [&courseCode](std::shared_ptr<Course> c) {
        if (c->getCourseCode() == courseCode) return true;
        return false;
    });

    if (result != courses.end()) return false; //course already exists.
    courses.push_back(std::make_shared<Course>(courseCode));
    return true;
}

// method to add a student to a course.
void School::addStudentToCourse(unsigned int id, const std::string &courseCode) {
    auto courseObject = findCourse(courseCode);    // find the course object
    // find student
    auto studentObject = findStudent(id);

    //TODO: try to add the student to the course.
    // if the student can't be added to the course, e.g. they are already enrolled
    // then return false.
    // courseObject is an iterator of std::vector<std::shared_ptr<Course>>
    if (!(*courseObject)->addStudent(studentObject)) {
        std::stringstream ss;
        ss << "Student " << id << " is already enrolled in course " << courseCode;
        throw std::runtime_error{ss.str()};
    }

    // add the course object to a student's object
    studentObject->addCourse(*courseObject);
}

// method to transfer ownership of a locker to a student if
// there are enough lockers available.
bool School::assignLocker(unsigned int id) {
    auto studentObject = findStudent(id);
    if (lockers.size() > 0u) {
        // TODO: assign the locker to the student
        studentObject->assignLocker(std::move(lockers.back()));
        lockers.pop_back();
        return true;
    }
    return false;
}

// method to find a course object.
// showcasing a trailing return type, using the type of iterator that courses returns.
// this is used as trying to determine the type of this iterator is difficult.
auto School::findCourse(const std::string &courseCode) const -> decltype(courses.begin()) {
    // find course.
    auto courseObject = std::find_if(courses.begin(), courses.end(), [&courseCode](std::shared_ptr<Course> c) {
        if (c->getCourseCode().compare(courseCode) == 0) return true;
        return false;
    });
    if (courseObject == courses.end()) throw std::runtime_error("Can't find course " + courseCode);
    return courseObject;
}
