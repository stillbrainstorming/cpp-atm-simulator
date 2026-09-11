# CPP ATM Simulator

A console-based ATM simulation built with C++ that demonstrates core Object-Oriented Programming principles including encapsulation, data hiding, authentication, and transaction handling. The application models an ATM workflow with multiple in-memory accounts.

---

## Features

- Authentication with account number and PIN
- PIN masking on Windows
- Three-attempt login limit
- Multiple in-memory accounts
- Check account balance
- Withdraw cash with amount and balance validation
- View the authenticated user's profile
- Update the authenticated user's mobile number after old-number verification
- Continuous session loop
- Windows-specific console behavior isolated with `_WIN32` guards

## Project Structure

```
cpp-atm-simulator/
    atm.cpp        - Application, domain, authentication, and transaction services
    README.md      - Project documentation
```

## Default Test Accounts

| Account Number | PIN  | Name | Balance | Mobile No. |
| --- | --- | --- | ---: | --- |
| `1234567` | `1111` | Tim | `45000.90` | `9087654321` |
| `7654321` | `2222` | Alex | `30000.00` | `9876543210` |

Each account has an independent session state. A withdrawal or mobile-number update is applied only to the account that successfully authenticated.

## Application Flow

```
Program Start
    |
    v
Initialize account collection
    |
    v
Login Screen
    |-- Enter Account No + PIN
    |-- Search configured accounts
    |       |
    |       |-- Match: Bind authenticated session to that account
    |       |-- No Match: Retry up to 3 attempts
    |
    v
ATM Menu
    |-- Check Balance
    |-- Cash Withdraw
    |-- Show User Details
    |-- Update Mobile No.
    |-- Exit
```

## Prerequisites

| Requirement | Details |
| --- | --- |
| Operating System | Windows for masked PIN input; other platforms use the portable fallback |
| Compiler | MinGW g++ or MSVC |
| C++ Standard | C++11 or later |

## How to Compile

Using g++:

```bash
g++ -std=c++11 atm.cpp -o atm
```

Using MSVC:

```text
cl atm.cpp /Fe:atm.exe
```

## How to Run

Windows:

```text
.\\atm.exe
```

## Known Limitations

- Accounts are configured in source code and are not yet loaded from persistent storage.
- Account and transaction state is held in memory and resets when the program exits.
- The project does not yet provide deposits, transfers, or transaction history.
- The non-Windows PIN fallback does not provide console masking.

## Roadmap

The next planned stages are persistent account storage, deposits/transfers, transaction history, stronger monetary/input validation, automated business-logic tests, and documentation refinement.
