#include <bits/stdc++.h>

using namespace std;

// Book structure
struct Book {
    string title;
    string author;
    string genre;
    double price;
    string seller;
    string dateAdded;
};

// Review structure
struct Review {
    string bookTitle;
    string user;
    int rating;
    string reviewText;
    string date;
};

// User structure
struct User {
    string username;
    string password;
};

class BookMarketplace {
private:
    string currentUser;
    vector<User> users;
    vector<Book> books;
    vector<Review> reviews;

    // File names
    const string USERS_FILE = "users.txt";
    const string BOOKS_FILE = "books.txt";
    const string REVIEWS_FILE = "reviews.txt";

public:
    BookMarketplace() : currentUser("") {
        loadData();
    }

    // Load data from files
    void loadData() {
        loadUsers();
        loadBooks();
        loadReviews();
    }

    void loadUsers() {
        ifstream file(USERS_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                User user;
                user.username = line.substr(0, pos);
                user.password = line.substr(pos + 1);
                users.push_back(user);
            }
        }
        file.close();
    }

    void loadBooks() {
        ifstream file(BOOKS_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            Book book;
            size_t pos1 = line.find('|');
            if (pos1 == string::npos) continue;

            book.title = line.substr(0, pos1);

            size_t pos2 = line.find('|', pos1 + 1);
            if (pos2 == string::npos) continue;
            book.author = line.substr(pos1 + 1, pos2 - pos1 - 1);

            size_t pos3 = line.find('|', pos2 + 1);
            if (pos3 == string::npos) continue;
            book.genre = line.substr(pos2 + 1, pos3 - pos2 - 1);

            size_t pos4 = line.find('|', pos3 + 1);
            if (pos4 == string::npos) continue;
            book.price = stod(line.substr(pos3 + 1, pos4 - pos3 - 1));

            size_t pos5 = line.find('|', pos4 + 1);
            if (pos5 == string::npos) continue;
            book.seller = line.substr(pos4 + 1, pos5 - pos4 - 1);

            book.dateAdded = line.substr(pos5 + 1);
            books.push_back(book);
        }
        file.close();
    }

    void loadReviews() {
        ifstream file(REVIEWS_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            Review review;
            size_t pos1 = line.find('|');
            if (pos1 == string::npos) continue;

            review.bookTitle = line.substr(0, pos1);

            size_t pos2 = line.find('|', pos1 + 1);
            if (pos2 == string::npos) continue;
            review.user = line.substr(pos1 + 1, pos2 - pos1 - 1);

            size_t pos3 = line.find('|', pos2 + 1);
            if (pos3 == string::npos) continue;
            review.rating = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));

            size_t pos4 = line.find('|', pos3 + 1);
            if (pos4 == string::npos) continue;
            review.reviewText = line.substr(pos3 + 1, pos4 - pos3 - 1);

            review.date = line.substr(pos4 + 1);
            reviews.push_back(review);
        }
        file.close();
    }

    // Save data to files
    void saveUsers() {
        ofstream file(USERS_FILE);
        for (const auto& user : users) {
            file << user.username << ":" << user.password << endl;
        }
        file.close();
    }

    void saveBooks() {
        ofstream file(BOOKS_FILE);
        for (const auto& book : books) {
            file << book.title << "|" << book.author << "|" << book.genre << "|"
                 << book.price << "|" << book.seller << "|" << book.dateAdded << endl;
        }
        file.close();
    }

    void saveReviews() {
        ofstream file(REVIEWS_FILE);
        for (const auto& review : reviews) {
            file << review.bookTitle << "|" << review.user << "|" << review.rating << "|"
                 << review.reviewText << "|" << review.date << endl;
        }
        file.close();
    }

    // Get current date and time
    string getCurrentDateTime() {
        time_t now = time(0);
        tm* localTime = localtime(&now);
        stringstream ss;
        ss << 1900 + localTime->tm_year << "-"
           << setw(2) << setfill('0') << 1 + localTime->tm_mon << "-"
           << setw(2) << setfill('0') << localTime->tm_mday << " "
           << setw(2) << setfill('0') << localTime->tm_hour << ":"
           << setw(2) << setfill('0') << localTime->tm_min << ":"
           << setw(2) << setfill('0') << localTime->tm_sec;
        return ss.str();
    }

    // User registration
    void registerUser() {
        cout << "\n=== User Registration ===" << endl;
        string username, password;

        cout << "Enter username: ";
        cin.ignore();
        getline(cin, username);

        // Check if username already exists
        for (const auto& user : users) {
            if (user.username == username) {
                cout << "Username already exists! Please choose another." << endl;
                return;
            }
        }

        cout << "Enter password: ";
        getline(cin, password);

        User newUser;
        newUser.username = username;
        newUser.password = password;
        users.push_back(newUser);
        saveUsers();

        cout << "Registration successful! You can now login." << endl;
    }

    // User login
    bool login() {
        cout << "\n=== User Login ===" << endl;
        string username, password;

        cout << "Username: ";
        cin.ignore();
        getline(cin, username);

        cout << "Password: ";
        getline(cin, password);

        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                currentUser = username;
                cout << "Login successful! Welcome " << username << "!" << endl;
                return true;
            }
        }

        cout << "Invalid username or password!" << endl;
        return false;
    }

    // Search books
    void searchBooks() {
        cout << "\n=== Search Books ===" << endl;
        string searchTerm;

        cout << "Enter book title, author, or genre to search: ";
        cin.ignore();
        getline(cin, searchTerm);

        // Convert to lowercase for case-insensitive search
        transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

        vector<Book> foundBooks;
        for (const auto& book : books) {
            string titleLower = book.title;
            string authorLower = book.author;
            string genreLower = book.genre;

            transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);
            transform(authorLower.begin(), authorLower.end(), authorLower.begin(), ::tolower);
            transform(genreLower.begin(), genreLower.end(), genreLower.begin(), ::tolower);

            if (titleLower.find(searchTerm) != string::npos ||
                authorLower.find(searchTerm) != string::npos ||
                genreLower.find(searchTerm) != string::npos) {
                foundBooks.push_back(book);
            }
        }

        if (!foundBooks.empty()) {
            cout << "\nFound Books:" << endl;
            for (size_t i = 0; i < foundBooks.size(); i++) {
                cout << i + 1 << ". " << foundBooks[i].title << " by " << foundBooks[i].author
                     << " - $" << fixed << setprecision(2) << foundBooks[i].price << endl;
                cout << "   Genre: " << foundBooks[i].genre << ", Seller: " << foundBooks[i].seller << endl;
            }
        } else {
            cout << "No books found matching your search." << endl;
        }
    }

    // Sell book
    void sellBook() {
        if (currentUser.empty()) {
            cout << "Please login first!" << endl;
            return;
        }

        cout << "\n=== Sell a Book ===" << endl;
        Book newBook;

        cout << "Book title: ";
        cin.ignore();
        getline(cin, newBook.title);

        cout << "Author: ";
        getline(cin, newBook.author);

        cout << "Genre: ";
        getline(cin, newBook.genre);

        cout << "Price: $";
        cin >> newBook.price;

        newBook.seller = currentUser;
        newBook.dateAdded = getCurrentDateTime();

        books.push_back(newBook);
        saveBooks();

        cout << "Book '" << newBook.title << "' listed for sale successfully!" << endl;
    }

    // Buy book
    void buyBook() {
        if (currentUser.empty()) {
            cout << "Please login first!" << endl;
            return;
        }

        cout << "\n=== Buy a Book ===" << endl;

        if (books.empty()) {
            cout << "No books available for purchase." << endl;
            return;
        }

        // Display available books (excluding current user's books)
        vector<Book> availableBooks;
        int index = 1;

        cout << "\nAvailable Books:" << endl;
        for (const auto& book : books) {
            if (book.seller != currentUser) {
                cout << index << ". " << book.title << " by " << book.author
                     << " - $" << fixed << setprecision(2) << book.price
                     << " (Seller: " << book.seller << ")" << endl;
                availableBooks.push_back(book);
                index++;
            }
        }

        if (availableBooks.empty()) {
            cout << "No books available from other sellers." << endl;
            return;
        }

        cout << "\nEnter book number to purchase (0 to cancel): ";
        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "Purchase cancelled." << endl;
            return;
        }

        if (choice < 1 || choice > availableBooks.size()) {
            cout << "Invalid selection!" << endl;
            return;
        }

        Book bookToBuy = availableBooks[choice - 1];

        cout << "Confirm purchase of '" << bookToBuy.title << "' for $"
             << fixed << setprecision(2) << bookToBuy.price << "? (y/n): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            // Remove book from vector
            auto it = find_if(books.begin(), books.end(), [&](const Book& b) {
                return b.title == bookToBuy.title && b.author == bookToBuy.author && b.seller == bookToBuy.seller;
            });

            if (it != books.end()) {
                books.erase(it);
                saveBooks();
                cout << "Purchase successful! The book has been added to your collection." << endl;
            }
        } else {
            cout << "Purchase cancelled." << endl;
        }
    }

    // Add review
    void addReview() {
        if (currentUser.empty()) {
            cout << "Please login first!" << endl;
            return;
        }

        cout << "\n=== Add Review ===" << endl;
        Review newReview;

        cout << "Enter book title you want to review: ";
        cin.ignore();
        getline(cin, newReview.bookTitle);

        cout << "Note: In this demo, we assume you've purchased the book." << endl;

        cout << "Enter rating (1-5 stars): ";
        cin >> newReview.rating;

        if (newReview.rating < 1 || newReview.rating > 5) {
            cout << "Rating must be between 1 and 5!" << endl;
            return;
        }

        cout << "Enter your review: ";
        cin.ignore();
        getline(cin, newReview.reviewText);

        newReview.user = currentUser;
        newReview.date = getCurrentDateTime();

        reviews.push_back(newReview);
        saveReviews();

        cout << "Review added successfully!" << endl;
    }

    // View reviews
    void viewReviews() {
        cout << "\n=== Book Reviews ===" << endl;
        string bookTitle;

        cout << "Enter book title to view reviews: ";
        cin.ignore();
        getline(cin, bookTitle);

        // Convert to lowercase for case-insensitive search
        transform(bookTitle.begin(), bookTitle.end(), bookTitle.begin(), ::tolower);

        vector<Review> bookReviews;
        double totalRating = 0;

        for (const auto& review : reviews) {
            string reviewTitleLower = review.bookTitle;
            transform(reviewTitleLower.begin(), reviewTitleLower.end(), reviewTitleLower.begin(), ::tolower);

            if (reviewTitleLower == bookTitle) {
                bookReviews.push_back(review);
                totalRating += review.rating;
            }
        }

        if (!bookReviews.empty()) {
            cout << "\nReviews for '" << bookTitle << "':" << endl;
            for (const auto& review : bookReviews) {
                cout << "User: " << review.user << endl;
                cout << "Rating: ";
                for (int i = 0; i < review.rating; i++) cout << "★";
                for (int i = review.rating; i < 5; i++) cout << "☆";
                cout << endl;
                cout << "Review: " << review.reviewText << endl;
                cout << "Date: " << review.date << endl;
                cout << string(30, '-') << endl;
            }

            double avgRating = totalRating / bookReviews.size();
            cout << "Average Rating: " << fixed << setprecision(1) << avgRating << "/5" << endl;
        } else {
            cout << "No reviews found for '" << bookTitle << "'" << endl;
        }
    }

    // Logout
    void logout() {
        currentUser = "";
        cout << "Logged out successfully!" << endl;
    }

    // Main menu
    void run() {
        while (true) {
            cout << "\n=== Book Marketplace ===" << endl;
            if (!currentUser.empty()) {
                cout << "Logged in as: " << currentUser << endl;
                cout << "1. Search Books" << endl;
                cout << "2. Sell Book" << endl;
                cout << "3. Buy Book" << endl;
                cout << "4. Add Review" << endl;
                cout << "5. View Reviews" << endl;
                cout << "6. Logout" << endl;
                cout << "7. Exit" << endl;
            } else {
                cout << "1. Register" << endl;
                cout << "2. Login" << endl;
                cout << "3. Search Books" << endl;
                cout << "4. View Reviews" << endl;
                cout << "5. Exit" << endl;
            }

            cout << "Enter your choice: ";
            int choice;
            cin >> choice;

            if (!currentUser.empty()) {
                switch (choice) {
                    case 1: searchBooks(); break;
                    case 2: sellBook(); break;
                    case 3: buyBook(); break;
                    case 4: addReview(); break;
                    case 5: viewReviews(); break;
                    case 6: logout(); break;
                    case 7:
                        cout << "Thank you for using Book Marketplace!" << endl;
                        return;
                    default:
                        cout << "Invalid choice!" << endl;
                        break;
                }
            } else {
                switch (choice) {
                    case 1: registerUser(); break;
                    case 2: login(); break;
                    case 3: searchBooks(); break;
                    case 4: viewReviews(); break;
                    case 5:
                        cout << "Thank you for using Book Marketplace!" << endl;
                        return;
                    default:
                        cout << "Invalid choice!" << endl;
                        break;
                }
            }
        }
    }
};

int main() {
    BookMarketplace marketplace;
    marketplace.run();
    return 0;
}
