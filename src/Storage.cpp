#include "Storage.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>

namespace {
std::string encode(const std::string& value) {
    std::string out = "\"";
    for (char c : value) { if (c == '"') out += "\"\""; else out += c; }
    return out + "\"";
}
std::vector<std::string> parseCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool quoted = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (quoted && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"'; ++i;
            } else quoted = !quoted;
        } else if (c == ',' && !quoted) {
            fields.push_back(field); field.clear();
        } else field += c;
    }
    if (quoted) return {};
    fields.push_back(field);
    return fields;
}
}

bool Storage::save(const Library& library, const std::string& path, std::string& message) {
    const std::string temp = path + ".tmp";
    {
        std::ofstream out(temp, std::ios::trunc);
        if (!out) { message = "Cannot open temporary storage file for writing."; return false; }
        out << "[BOOKS]\n";
        for (const auto& b : library.books())
            out << encode(b.getId()) << ',' << encode(b.getTitle()) << ','
                << encode(b.getAuthor()) << ',' << encode(b.getCategory()) << ','
                << b.getYear() << ',' << (b.isAvailable() ? 1 : 0) << '\n';
        out << "[MEMBERS]\n";
        for (const auto& m : library.members())
            out << encode(m.getId()) << ',' << encode(m.getName()) << ','
                << encode(m.getEmail()) << '\n';
        out << "[LOANS]\n";
        for (const auto& l : library.loans())
            out << encode(l.getBookId()) << ',' << encode(l.getMemberId()) << ','
                << encode(l.getBorrowDate()) << ',' << encode(l.getReturnDate()) << '\n';
        if (!out.good()) { message = "Write failed; original storage was not replaced."; return false; }
    }
    if (std::rename(temp.c_str(), path.c_str()) != 0) {
        // On Windows rename does not overwrite; remove old file only after temp is complete.
        std::remove(path.c_str());
        if (std::rename(temp.c_str(), path.c_str()) != 0) {
            message = "Could not replace storage file."; return false;
        }
    }
    message = "Data saved."; return true;
}

bool Storage::load(Library& library, const std::string& path, std::string& message) {
    std::ifstream in(path);
    if (!in) { message = "Storage file not found. Starting with an empty library."; return true; }

    std::vector<Book> books;
    std::vector<Member> members;
    std::vector<Loan> loans;
    std::string line, section;
    std::size_t skipped = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line == "[BOOKS]" || line == "[MEMBERS]" || line == "[LOANS]") {
            section = line; continue;
        }
        auto f = parseCsvLine(line);
        try {
            if (section == "[BOOKS]" && f.size() == 6) {
                int year = std::stoi(f[4]);
                if (f[0].empty() || f[1].empty() || f[2].empty() || f[3].empty() ||
                    year < 0 || year > 2100) { ++skipped; continue; }
                bool available = f[5] == "1";
                books.emplace_back(f[0], f[1], f[2], f[3], year, available);
            } else if (section == "[MEMBERS]" && f.size() == 3) {
                if (f[0].empty() || f[1].empty()) { ++skipped; continue; }
                members.emplace_back(f[0], f[1], f[2]);
            } else if (section == "[LOANS]" && f.size() == 4) {
                loans.emplace_back(f[0], f[1], f[2]);
                if (!f[3].empty()) loans.back().close(f[3]);
            } else ++skipped;
        } catch (...) { ++skipped; }
    }
    library.replaceData(std::move(books), std::move(members), std::move(loans));
    message = "Data loaded. Malformed rows skipped: " + std::to_string(skipped);
    return true;
}
