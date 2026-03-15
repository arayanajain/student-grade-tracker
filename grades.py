import json
import os

FILENAME = "grades.json"

def load_data():
    if os.path.exists(FILENAME):
        with open(FILENAME, "r") as f:
            return json.load(f)
    return {}

def save_data(students):
    with open(FILENAME, "w") as f:
        json.dump(students, f, indent=4)

def add_student(students):
    name = input("Enter student name: ").strip()
    if name in students:
        print("Student already exists!")
        return
    students[name] = []
    print(f"{name} added successfully!")
    save_data(students)

def add_grades(students):
    name = input("Enter student name: ").strip()
    if name not in students:
        print("Student not found!")
        return
    subject = input("Enter subject: ").strip()
    grade = float(input("Enter grade (0-100): "))
    students[name].append({"subject": subject, "grade": grade})
    print(f"Grade added for {name} in {subject}!")
    save_data(students)

def view_student(students):
    name = input("Enter student name: ").strip()
    if name not in students:
        print("Student not found!")
        return
    print(f"\n--- {name}'s Grades ---")
    if not students[name]:
        print("No grades yet!")
        return
    total = 0
    for entry in students[name]:
        print(f"{entry['subject']}: {entry['grade']}")
        total += entry['grade']
    avg = total / len(students[name])
    print(f"Average: {avg:.2f}")
    if avg >= 90:
        print("Grade: A")
    elif avg >= 80:
        print("Grade: B")
    elif avg >= 70:
        print("Grade: C")
    elif avg >= 60:
        print("Grade: D")
    else:
        print("Grade: F")

def view_all(students):
    if not students:
        print("No students found!")
        return
    print("\n--- All Students ---")
    for name in students:
        grades = students[name]
        if grades:
            avg = sum(e['grade'] for e in grades) / len(grades)
            print(f"{name} — Average: {avg:.2f}")
        else:
            print(f"{name} — No grades yet")

def delete_student(students):
    name = input("Enter student name to delete: ").strip()
    if name not in students:
        print("Student not found!")
        return
    del students[name]
    print(f"{name} deleted!")
    save_data(students)

def main():
    students = load_data()
    while True:
        print("\n=== Student Grade Tracker ===")
        print("1. Add Student")
        print("2. Add Grade")
        print("3. View Student Report")
        print("4. View All Students")
        print("5. Delete Student")
        print("6. Exit")
        choice = input("Enter choice: ").strip()
        if choice == "1":
            add_student(students)
        elif choice == "2":
            add_grades(students)
        elif choice == "3":
            view_student(students)
        elif choice == "4":
            view_all(students)
        elif choice == "5":
            delete_student(students)
        elif choice == "6":
            print("Exiting...")
            break
        else:
            print("Invalid choice!")

if __name__ == "__main__":
    main()
