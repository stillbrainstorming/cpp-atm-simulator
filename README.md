# CPP ATM Simulator

A console-based ATM simulation built with C++ that demonstrates core Object-Oriented Programming principles including encapsulation, data hiding, and class-based design. The application models a real-world ATM workflow — from secure login to transactional operations — entirely within a single-user session.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Class Design](#class-design)
- [Application Flow](#application-flow)
- [Default Test Credentials](#default-test-credentials)
- [Prerequisites](#prerequisites)
- [How to Compile](#how-to-compile)
- [How to Run](#how-to-run)
- [Sample Session](#sample-session)
- [Known Limitations](#known-limitations)

---

## Overview

This project is a mini ATM simulator written in standard C++ with a minimal dependency on the Windows-specific `conio.h` header (used solely for `_getch()` to pause the screen after output). The program simulates a terminal-based banking session where a single user can authenticate using an account number and PIN, then perform a set of standard ATM operations.

---

## Features

- Secure login with account number and PIN verification
- Check current account balance
- Withdraw cash with balance validation
- View full user profile details
- Update registered mobile number with old-number confirmation
- Continuous session loop — returns to the main menu after each operation
- Invalid input handling at both the login and menu levels

---

## Project Structure

```
cpp-atm-simulator/
    atm.cpp        - Full application source
    README.md      - Project documentation
```

---

## Class Design

The entire application is centered around a single class `atm`, which encapsulates all user data and operations.

### Private Member Variables

| Variable     | Type       | Description                |
| ------------ | ---------- | -------------------------- |
| `account_No` | `long int` | Unique account identifier  |
| `name`       | `string`   | Account holder's name      |
| `PIN`        | `int`      | 4-digit authentication PIN |
| `balance`    | `double`   | Current account balance    |
| `mobile_No`  | `string`   | Registered mobile number   |

### Public Member Functions

| Function                       | Return Type | Description                                                         |
| ------------------------------ | ----------- | ------------------------------------------------------------------- |
| `setData(...)`                 | `void`      | Initializes all private member variables with provided arguments    |
| `getAccountNo()`               | `long int`  | Returns the account number                                          |
| `getName()`                    | `string`    | Returns the account holder's name                                   |
| `getPIN()`                     | `int`       | Returns the PIN for authentication comparison                       |
| `getBalance()`                 | `double`    | Returns the current balance                                         |
| `getMobileNo()`                | `string`    | Returns the registered mobile number                                |
| `setMobile(mob_prev, mob_new)` | `void`      | Updates mobile number after verifying the old one matches on record |
| `cashWithDraw(amount_a)`       | `void`      | Deducts amount from balance if valid and sufficient funds exist     |

---

## Application Flow

```
Program Start
    |
    v
Login Screen
    |-- Enter Account No + PIN
    |-- Match against stored credentials
    |       |
    |       |-- Match: Proceed to ATM Menu
    |       |-- No Match: Show error, retry login
    |
    v
ATM Menu (loops until exit)
    |
    |-- 1. Check Balance       --> Display current balance
    |-- 2. Cash Withdraw       --> Enter amount, validate, deduct
    |-- 3. Show User Details   --> Display all account info
    |-- 4. Update Mobile No.   --> Verify old number, set new
    |-- 5. Exit                --> Terminate program
    |-- Invalid Input          --> Show error, return to menu
```

---

## Default Test Credentials

The application ships with a hardcoded test user. Use these credentials to log in:

| Field          | Value      |
| -------------- | ---------- |
| Account Number | `1234567`  |
| PIN            | `1111`     |
| Name           | Tim        |
| Balance        | 45000.90   |
| Mobile No.     | 9087654321 |

---

## Prerequisites

| Requirement      | Details                                                          |
| ---------------- | ---------------------------------------------------------------- |
| Operating System | Windows (required for `conio.h` / `_getch()`)                    |
| Compiler         | MinGW g++ (GCC for Windows) or MSVC (Visual Studio C++ compiler) |
| C++ Standard     | C++11 or later                                                   |

To check if `g++` is installed, run:

```
g++ --version
```

If not installed, download MinGW-w64 from: https://www.mingw-w64.org/downloads/

---

## How to Compile

Open a terminal (Command Prompt or PowerShell) in the project directory.

### Using g++ (MinGW)

```
g++ atm.cpp -o atm
```

### Using g++ with explicit C++11 standard

```
g++ -std=c++11 atm.cpp -o atm
```

### Using MSVC (Visual Studio Developer Command Prompt)

```
cl atm.cpp /Fe:atm.exe
```

A successful compile will produce `atm.exe` (or `atm` on MinGW) in the same directory.

---

## How to Run

After compiling, run the executable from the terminal:

```
.\atm.exe
```

Or, if compiled without `.exe` extension via MinGW:

```
.\atm
```

The program will clear the terminal and display the ATM login screen immediately.

---

## Sample Session

```
**** Welcome to ATM ****

Enter Your Account No: 1234567

Enter PIN: 1111

**** Welcome to ATM ****

Select an Option:
1. Check Balance
2. Cash Withdraw
3. Show User Details
4. Update Mobile No.
5. Exit

1

Your Bank Balance: 45000.9
[Press any key to continue]

**** Welcome to ATM ****
...
```

---

## Known Limitations

- **Single user only** — the application is hardcoded with one test user (`Tim`). There is no database, file storage, or multi-user support.
- **In-memory state** — all changes (balance deductions, mobile number updates) are lost when the program exits.
- **Windows only** — the use of `conio.h` and `_getch()` makes this non-portable to Linux or macOS without substitution.
- **No PIN masking** — the PIN is entered and displayed as plain text in the console.
- **Integer-only withdrawals** — the `cashWithDraw` function accepts `int` amounts; fractional amounts are not supported.
