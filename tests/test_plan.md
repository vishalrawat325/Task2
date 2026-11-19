# Manual / automated verification plan

The core library behavior is separated from the CLI so it can be unit-tested without terminal input.

Recommended test cases:
1. Empty collection search returns no results.
2. Add a valid book.
3. Reject duplicate book IDs.
4. Search is case-insensitive across ID/title/author/category.
5. Update existing book; reject missing IDs and invalid years.
6. Reject deleting a borrowed book.
7. Reject borrowing a missing book, missing member, or already-borrowed book.
8. Return an active loan and reject a second return.
9. Reject a return date earlier than the borrow date.
10. Save and reload books, members, and loan history.
11. Load a missing file without crashing.
12. Skip malformed rows while loading valid records.

The date comparison relies on canonical YYYY-MM-DD strings. The CLI asks for this format but does not yet perform full calendar validation; add a calendar-date validator before treating arbitrary user-entered dates as trusted.
