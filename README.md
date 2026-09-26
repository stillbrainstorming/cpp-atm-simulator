# CPP ATM Simulator

A console-based ATM simulation built with C++ that demonstrates authentication, multi-account handling, transactional operations, local persistence, validation, and business-logic testing.

## Current capabilities

- Authenticate against multiple configured demo accounts with a three-attempt login limit.
- Mask PIN entry on Windows and use a portable fallback elsewhere.
- Check balances and maintain an authenticated account session.
- Withdraw, deposit, and transfer funds with monetary validation.
- View the authenticated user's profile.
- Update the mobile number after verifying the existing number.
- Record successful withdrawals, deposits, and transfers in per-account transaction history.
- Persist balances, mobile numbers, and successful transaction history to `atm_data.txt`.
- Ignore malformed persistence records instead of terminating the application.
- Reject empty, non-positive, non-finite, or otherwise invalid monetary input.
- Validate mobile-number updates as exactly 10 numeric digits.
- Keep Windows-specific console behavior behind `_WIN32` guards.

## Demo accounts

| Account Number | PIN  | Name | Balance | Mobile No. |
| --- | --- | --- | ---: | --- |
| `1234567` | `1111` | Tim | `45000.90` | `9087654321` |
| `7654321` | `2222` | Alex | `30000.00` | `9876543210` |

These credentials are intentionally source-controlled demo data for the simulator. They must not be treated as production authentication material.

## Application flow

```
Program Start
    |
    v
Initialize account collection
    |
    v
Load persisted account and transaction state
    |
    v
Login
    |-- Account number + PIN
    |-- Up to 3 failed attempts
    |
    v
Authenticated ATM session
    |-- Check balance
    |-- Withdraw
    |-- Deposit
    |-- Transfer
    |-- User details
    |-- Update mobile number
    |-- Transaction history
    |-- Exit
```

## Persistence

The application uses a local `atm_data.txt` file in the working directory.

Persisted state includes:

- account balances;
- mobile numbers;
- successful transaction-history entries.

The PIN remains part of the demo account configuration in source code and is not intended as a secure credential store.

If the storage file is missing, the default demo accounts are used. Malformed persistence records are ignored so a bad record does not terminate the application.

## Transaction history

Successful withdrawals, deposits, and transfers are recorded against the affected account. Transfer entries retain the related account number and the resulting balance.

Invalid or failed monetary operations do not create successful history entries.

## Build

### g++

```bash
g++ -std=c++11 atm.cpp -o atm
```

### MSVC

```text
cl atm.cpp /Fe:atm.exe
```

## Run

Windows:

```text
.\\atm.exe
```

On other platforms, run the generated executable using the platform's normal executable invocation.

## Automated tests

The focused business-logic suite is in `tests/test_core.cpp`. It covers mobile validation, authentication, withdrawal/deposit validation, transfers, transaction-history recording, and mobile-number updates.

### g++

```bash
g++ -std=c++11 tests/test_core.cpp -o atm-tests
./atm-tests
```

The test file renames the production `main` entry point while compiling against `atm.cpp`, so it exercises the production classes and services without starting the interactive CLI.

## Platform notes

| Area | Behavior |
| --- | --- |
| Windows PIN entry | Masked input through `_getch()` |
| Non-Windows PIN entry | Portable visible-input fallback |
| Persistence | Local text file |
| Currency | Floating-point demo values |
| Credentials | Source-controlled demo configuration |

## Known limitations

- Account identity and PIN configuration are source-controlled demo data.
- Persistence uses a local text file rather than a database.
- Persistence records do not include timestamps.
- Storage write failures are not surfaced through a dedicated recovery flow.
- The non-Windows PIN fallback does not provide console masking.
- This project is a simulator, not a production banking system.

## Development notes

The project is intentionally dependency-free and keeps its implementation in a single production source file. The automated test suite focuses on the business and transaction layer while the CLI remains manually exercised through the documented commands.

## Remaining roadmap

Future work can focus on richer persistence formats, transaction timestamps, stronger credential handling, broader CLI/integration tests, and clearer reporting of storage failures.