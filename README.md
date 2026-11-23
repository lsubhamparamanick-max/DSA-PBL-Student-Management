# Structure-oriented Persistent Data Management 

**Experiment 7 | Data Structure & Algorithm Lab 
**Student Name:** Subham Paramanick  
**Student ID:** BWU_BTD_25_68  
**Class:** B.Tech CSE - DS III A  
**Faculty:** Dr. Supriya Chakraborty & Ms. Paulomy Majee  

---

## 📌 Problem Statement
Design and implement a structure-oriented persistent data management system in C for an educational institute to manage registered students (`ID`, `Name`, `Fees`), allot courses (`DS`, `AI`, `ML`, `Web`) upon fee payment, and declare final course grades (`Excellent`, `Good`, `Fail`). The system must include user authentication, a menu-driven interface, and persistent file storage.

---

## 🛠️ Data Structures & Design
- **Singly Linked List (`struct Node`):** Used for dynamic in-memory management of student records (insertion, traversal, search, update, and deletion without fixed array limits).
- **Binary File Persistence (`students.dat`):** All records are automatically saved to and loaded from a binary file using `fread()` and `fwrite()`, ensuring data persists across program restarts.
- **Authentication:** Requires valid `User ID` (`admin`) and `Password` (`admin123`) with a 3-attempt limit before granting access.

---

## ⚙️ How to Compile & Run
```bash
gcc student_management.c -o student_management
./student_management

Test Cases & Sample Output
Test Case 1: Authentication (Invalid & Valid Login)

==================================================
               SYSTEM AUTHENTICATION              
==================================================
User ID  : admin
Password : wrong
[ERROR] Invalid credentials. Attempts left: 2

User ID  : admin
Password : admin123

[SUCCESS] Login successful! Welcome, admin.

Test Case 2: Register Student, Pay Fees & Allot Course

Enter your choice: 1
Enter Student ID (numeric): 101
Enter Student Name: Subham Paramanick
[SUCCESS] Student 'Subham Paramanick' (ID: 101) registered successfully.

Enter your choice: 2
Enter Student ID: 101
Enter Fee Amount Paid (INR): 15000

Select Course to Allot:
1. DS (Data Science)
2. AI (Artificial Intelligence)
3. ML (Machine Learning)
4. Web (Web Development)
Choice (1-4): 1
[SUCCESS] Fees (Rs. 15000.00) recorded and course 'DS' allotted to Subham Paramanick.

Test Case 3: Declare Result & Display All Records

Enter your choice: 3
Enter Student ID: 101

Select Grade for Subham Paramanick (Course: DS):
1. Excellent
2. Good
3. Fail
Choice (1-3): 1
[SUCCESS] Result 'Excellent' declared for Subham Paramanick.

Enter your choice: 4

==============================================================================
ID     Name                   Fees (INR)   Status     Course     Grade       
------------------------------------------------------------------------------
101    Subham Paramanick      15000.00     Paid       DS         Excellent   
==============================================================================

Test Case 4: Persistent Storage Verification

Choose option 7 (Save & Exit).

Restart the program, log in again, and select option 4 (Display All Students).

Record 101 (Subham Paramanick | DS | Excellent) is automatically loaded from students.dat.

