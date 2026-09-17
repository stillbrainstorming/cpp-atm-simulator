# CPP ATM Simulator

A console-based ATM simulation built with C++ that demonstrates object-oriented design, authentication, account management, and transaction handling. The application models an ATM workflow with multiple in-memory accounts.

## Features

- Authentication with account number and PIN
- PIN masking on Windows
- Three-attempt login limit
- Multiple in-memory accounts
- Check account balance
- Withdraw cash with amount and balance validation
- Deposit cash
- Transfer funds between accounts
- View the authenticated user's profile
- Update the authenticated user's mobile number after old-number verification
- View transaction history for the authenticated account
- Windows-specific console behavior isolated with `_WIN32` guards

## Default Test Accounts

| Account Number | PIN  | Name | Balance | Mobile No. |
| --- | --- | --- | ---: | --- |
| `1234567` | `1111` | Tim | `45000.90` | `9087654321` |
| `7654321` | `2222` | Alex | `30000.00` | `9876543210` |

## Transaction History

Successful withdrawals, deposits, and transfers are recorded against the affected account. Each entry contains the transaction type, amount, resulting balance, and related account number for transfers.

Invalid or failed monetary operations do not create successful transaction-history entries.

Transaction history is currently held in memory and is reset when the application exits.

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
    |-- Deposit Cash
    |-- Transfer Funds
    |-- Show User Details
    |-- Update Mobile No.
    |-- Transaction History
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
- Transaction history does not yet include timestamps or persistent storage.
- The non-Windows PIN fallback does not provide console masking.

## Roadmap

The next planned stages are persistent account and transaction storage, stronger monetary/input validation, automated business-logic tests, and documentation refinement.
