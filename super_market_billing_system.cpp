#include <bits/stdc++.h>
#include <ctime>
using namespace std;

// Category names
vector<string> categories = {
    "Desktop", "Laptop", "Phone", "Tablet",
    "Networking", "Software_Accessories", "Gaming", "Gadget"
};

struct Salesman {
    int id;
    string username, password, name, mobile, email;
};

struct Customer {
    string name, username, password, phone;
};

struct Product {
    int id;
    string name;
    double price;
    int quantity;
    string category;
};

struct CartItem {
    Product product;
    int quantity;
};

struct PurchaseHistory {
    int serialNumber;
    string username;
    string customerName;
    string customerPhone;
    string dateTime;
    vector<CartItem> items;
    double totalAmount;
};

// Function declarations
bool ownerLogin();
bool salesmanLogin(Salesman &loggedSalesman);
bool customerLogin(Customer &loggedCustomer);
void ownerMenu();
void salesmanMenu(Salesman &salesman);
void customerMenu(Customer &customer);
void addSalesman();
void viewSalesmen();
void editSalesman();
void deleteSalesman();
void addProduct();
void viewProductsByCategory();
void viewAllProducts();
void editProduct();
void deleteProduct();
void salesmanBilling();
void customerBilling(Customer &customer);
void viewCart(vector<CartItem> &cart);
double calculateTotal(vector<CartItem> &cart);
void updateProductQuantity(int productId, int soldQuantity, const string &category);
void savePurchaseHistory(const PurchaseHistory &history);
int getNextSerialNumber();
void viewPurchaseHistory(const string &username = "");
void viewAllPurchaseHistory();
void customerRegistration();
bool checkCustomerExists(const string &phone);
string getCategoryFilename(const string &category);
void loadProductsFromCategory(const string &category, vector<Product> &products);
void saveProductToCategory(const Product &product);
vector<Product> getAllProducts();
Product findProductById(int productId);

//================= OWNER LOGIN =================//
bool ownerLogin() {
    string username, password;
    cout << "\n=== Owner Login ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    // Hardcoded owner credentials
    if (username == "owner" && password == "123") {
        cout << "Owner login successful!\n";
        return true;
    } else {
        cout << "Invalid owner credentials!\n";
        return false;
    }
}

//================= SALESMAN LOGIN =================//
bool salesmanLogin(Salesman &loggedSalesman) {
    string username, password;
    cout << "\n=== Salesman Login ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    ifstream file("salesmen.txt");
    if (!file.is_open()) {
        cout << "No salesmen accounts found!\n";
        return false;
    }

    Salesman salesman;
    bool success = false;

    while (file >> salesman.id >> salesman.username >> salesman.password
                >> salesman.name >> salesman.mobile >> salesman.email) {
        if (salesman.username == username && salesman.password == password) {
            loggedSalesman = salesman;
            success = true;
            break;
        }
    }
    file.close();

    if (success) {
        cout << "Salesman login successful! Welcome " << salesman.name << "\n";
        return true;
    } else {
        cout << "Invalid salesman credentials!\n";
        return false;
    }
}

//================= CUSTOMER LOGIN =================//
bool customerLogin(Customer &loggedCustomer) {
    string username, password;
    cout << "\n=== Customer Login ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    ifstream file("customers.txt");
    if (!file.is_open()) {
        cout << "No customer accounts found!\n";
        return false;
    }

    Customer customer;
    bool success = false;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, customer.name, ',');
        getline(ss, customer.username, ',');
        getline(ss, customer.password, ',');
        getline(ss, customer.phone, ',');

        if (customer.username == username && customer.password == password) {
            loggedCustomer = customer;
            success = true;
            break;
        }
    }
    file.close();

    if (success) {
        cout << "Customer login successful! Welcome " << customer.name << "\n";
        return true;
    } else {
        cout << "Invalid customer credentials!\n";
        return false;
    }
}

//================= CUSTOMER REGISTRATION =================//
void customerRegistration() {
    Customer newCustomer;

    cout << "\n=== Customer Registration ===\n";
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, newCustomer.name);
    cout << "Enter Phone: ";
    cin >> newCustomer.phone;

    // Check if phone already exists
    if (checkCustomerExists(newCustomer.phone)) {
        cout << "Phone number already registered! Please login instead.\n";
        return;
    }

    cout << "Enter Username: ";
    cin >> newCustomer.username;
    cout << "Enter Password: ";
    cin >> newCustomer.password;

    // Save to customers file
    ofstream file("customers.txt", ios::app);
    file << newCustomer.name << "," << newCustomer.username << ","
         << newCustomer.password << "," << newCustomer.phone << endl;
    file.close();

    cout << "Registration Successful! You can now login.\n";
}

bool checkCustomerExists(const string &phone) {
    ifstream file("customers.txt");
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, username, password, custPhone;
        getline(ss, name, ',');
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, custPhone, ',');

        if (custPhone == phone) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

//================= SALESMAN MANAGEMENT =================//
void addSalesman() {
    Salesman newSalesman;
    cout << "\n=== Add Salesman ===\n";
    cout << "Enter Salesman ID: ";
    cin >> newSalesman.id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, newSalesman.name);
    cout << "Enter Mobile: ";
    cin >> newSalesman.mobile;
    cout << "Enter Email: ";
    cin >> newSalesman.email;
    cout << "Enter Username: ";
    cin >> newSalesman.username;
    cout << "Enter Password: ";
    cin >> newSalesman.password;

    ofstream file("salesmen.txt", ios::app);
    file << newSalesman.id << " " << newSalesman.username << " " << newSalesman.password
         << " " << newSalesman.name << " " << newSalesman.mobile << " " << newSalesman.email << endl;
    file.close();

    cout << "Salesman added successfully!\n";
}

void viewSalesmen() {
    ifstream file("salesmen.txt");
    if (!file.is_open()) {
        cout << "No salesmen found!\n";
        return;
    }

    Salesman salesman;
    cout << "\n=== Salesmen List ===\n";
    cout << left << setw(5) << "ID" << setw(15) << "Username" << setw(20) << "Name"
         << setw(15) << "Mobile" << setw(20) << "Email" << endl;
    cout << "-------------------------------------------------------------------\n";

    while (file >> salesman.id >> salesman.username >> salesman.password
                >> salesman.name >> salesman.mobile >> salesman.email) {
        cout << left << setw(5) << salesman.id << setw(15) << salesman.username
             << setw(20) << salesman.name << setw(15) << salesman.mobile
             << setw(20) << salesman.email << endl;
    }
    file.close();
}

void editSalesman() {
    int id;
    cout << "\nEnter Salesman ID to edit: ";
    cin >> id;

    ifstream file("salesmen.txt");
    ofstream temp("temp_salesmen.txt");
    Salesman salesman;
    bool found = false;

    while (file >> salesman.id >> salesman.username >> salesman.password
                >> salesman.name >> salesman.mobile >> salesman.email) {
        if (salesman.id == id) {
            found = true;
            cout << "Enter New Name: ";
            cin.ignore();
            getline(cin, salesman.name);
            cout << "Enter New Mobile: ";
            cin >> salesman.mobile;
            cout << "Enter New Email: ";
            cin >> salesman.email;
            cout << "Enter New Username: ";
            cin >> salesman.username;
            cout << "Enter New Password: ";
            cin >> salesman.password;
        }
        temp << salesman.id << " " << salesman.username << " " << salesman.password
             << " " << salesman.name << " " << salesman.mobile << " " << salesman.email << endl;
    }

    file.close();
    temp.close();
    remove("salesmen.txt");
    rename("temp_salesmen.txt", "salesmen.txt");

    if (found) cout << "Salesman updated successfully!\n";
    else cout << "Salesman not found!\n";
}

void deleteSalesman() {
    int id;
    cout << "\nEnter Salesman ID to delete: ";
    cin >> id;

    ifstream file("salesmen.txt");
    ofstream temp("temp_salesmen.txt");
    Salesman salesman;
    bool found = false;

    while (file >> salesman.id >> salesman.username >> salesman.password
                >> salesman.name >> salesman.mobile >> salesman.email) {
        if (salesman.id == id) {
            found = true;
            continue;
        }
        temp << salesman.id << " " << salesman.username << " " << salesman.password
             << " " << salesman.name << " " << salesman.mobile << " " << salesman.email << endl;
    }

    file.close();
    temp.close();
    remove("salesmen.txt");
    rename("temp_salesmen.txt", "salesmen.txt");

    if (found) cout << "Salesman deleted successfully!\n";
    else cout << "Salesman not found!\n";
}

//================= OWNER MENU =================//
void ownerMenu() {
    int choice;
    do {
        cout << "\n--- Owner Menu ---\n";
        cout << "1. Salesman Management\n";
        cout << "2. View All Purchase History\n";
        cout << "3. View Products by Category\n";
        cout << "4. View All Products\n";
        cout << "5. Logout\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int smChoice;
                cout << "\n--- Salesman Management ---\n";
                cout << "1. Add Salesman\n2. View Salesmen\n3. Edit Salesman\n4. Delete Salesman\n5. Back\n";
                cout << "Enter Choice: ";
                cin >> smChoice;
                switch (smChoice) {
                    case 1: addSalesman(); break;
                    case 2: viewSalesmen(); break;
                    case 3: editSalesman(); break;
                    case 4: deleteSalesman(); break;
                    case 5: break;
                    default: cout << "Invalid option!\n";
                }
                break;
            }
            case 2: viewAllPurchaseHistory(); break;
            case 3: viewProductsByCategory(); break;
            case 4: viewAllProducts(); break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid Option!\n";
        }
    } while (choice != 5);
}

//================= SALESMAN MENU =================//
void salesmanMenu(Salesman &salesman) {
    int choice;
    do {
        cout << "\n--- Salesman Menu ---\n";
        cout << "1. Add Product\n";
        cout << "2. View Products by Category\n";
        cout << "3. View All Products\n";
        cout << "4. Edit Product\n";
        cout << "5. Checkout (Billing)\n";
        cout << "6. Logout\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addProduct(); break;
            case 2: viewProductsByCategory(); break;
            case 3: viewAllProducts(); break;
            case 4: editProduct(); break;
            case 5: salesmanBilling(); break;
            case 6: cout << "Logging out...\n"; break;
            default: cout << "Invalid Option!\n";
        }
    } while (choice != 6);
}

//================= CUSTOMER MENU =================//
void customerMenu(Customer &customer) {
    int choice;
    do {
        cout << "\n--- Customer Menu ---\n";
        cout << "1. View Products by Category\n";
        cout << "2. View All Products\n";
        cout << "3. Purchase Products\n";
        cout << "4. View Purchase History\n";
        cout << "5. Logout\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewProductsByCategory(); break;
            case 2: viewAllProducts(); break;
            case 3: customerBilling(customer); break;
            case 4: viewPurchaseHistory(customer.username); break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid Option!\n";
        }
    } while (choice != 5);
}

//================= PRODUCT CATEGORY MANAGEMENT =================//
string getCategoryFilename(const string &category) {
    return category + ".txt";
}

void loadProductsFromCategory(const string &category, vector<Product> &products) {
    string filename = getCategoryFilename(category);
    ifstream file(filename);

    if (!file.is_open()) {
        return; // File doesn't exist yet
    }

    Product p;
    p.category = category;
    while (file >> p.id >> p.name >> p.price >> p.quantity) {
        products.push_back(p);
    }
    file.close();
}

void saveProductToCategory(const Product &product) {
    string filename = getCategoryFilename(product.category);
    ofstream file(filename, ios::app);
    file << product.id << " " << product.name << " " << product.price << " " << product.quantity << endl;
    file.close();
}

vector<Product> getAllProducts() {
    vector<Product> allProducts;
    for (const auto &category : categories) {
        loadProductsFromCategory(category, allProducts);
    }
    return allProducts;
}

Product findProductById(int productId) {
    Product notFound;
    notFound.id = -1;

    for (const auto &category : categories) {
        vector<Product> products;
        loadProductsFromCategory(category, products);
        for (const auto &product : products) {
            if (product.id == productId) {
                return product;
            }
        }
    }
    return notFound;
}

//================= PRODUCT MANAGEMENT =================//
void addProduct() {
    Product p;
    cout << "\n=== Add Product ===\n";

    // Display categories
    cout << "Available Categories:\n";
    for (int i = 0; i < categories.size(); i++) {
        cout << i+1 << ". " << categories[i] << endl;
    }

    int catChoice;
    cout << "Select Category: ";
    cin >> catChoice;

    if (catChoice < 1 || catChoice > categories.size()) {
        cout << "Invalid category selection!\n";
        return;
    }

    p.category = categories[catChoice - 1];

    cout << "Enter Product ID: ";
    cin >> p.id;
    cout << "Enter Product Name: ";
    cin.ignore();
    getline(cin, p.name);
    cout << "Enter Price: ";
    cin >> p.price;
    cout << "Enter Quantity: ";
    cin >> p.quantity;

    saveProductToCategory(p);
    cout << "Product Added Successfully to " << p.category << " category!\n";
}

void viewProductsByCategory() {
    cout << "\nAvailable Categories:\n";
    for (int i = 0; i < categories.size(); i++) {
        cout << i+1 << ". " << categories[i] << endl;
    }

    int catChoice;
    cout << "Select Category: ";
    cin >> catChoice;

    if (catChoice < 1 || catChoice > categories.size()) {
        cout << "Invalid category selection!\n";
        return;
    }

    string category = categories[catChoice - 1];
    vector<Product> products;
    loadProductsFromCategory(category, products);

    if (products.empty()) {
        cout << "No products found in " << category << " category.\n";
        return;
    }

    cout << "\n=== " << category << " Products ===\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name"
         << setw(10) << "Price" << setw(10) << "Qty" << endl;
    cout << "--------------------------------------------------\n";

    for (const auto &p : products) {
        cout << left << setw(10) << p.id << setw(20) << p.name
             << setw(10) << p.price << setw(10) << p.quantity << endl;
    }
}

void viewAllProducts() {
    vector<Product> allProducts = getAllProducts();

    if (allProducts.empty()) {
        cout << "No products found in any category.\n";
        return;
    }

    cout << "\n=== All Products ===\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name"
         << setw(12) << "Category" << setw(10) << "Price" << setw(10) << "Qty" << endl;
    cout << "----------------------------------------------------------------\n";

    for (const auto &p : allProducts) {
        cout << left << setw(10) << p.id << setw(20) << p.name
             << setw(12) << p.category << setw(10) << p.price << setw(10) << p.quantity << endl;
    }
}

void editProduct() {
    int id;
    cout << "\nEnter Product ID to Edit: ";
    cin >> id;

    // Find the product first
    Product foundProduct;
    string foundCategory;
    bool productFound = false;

    for (const auto &category : categories) {
        vector<Product> products;
        loadProductsFromCategory(category, products);

        for (const auto &product : products) {
            if (product.id == id) {
                foundProduct = product;
                foundCategory = category;
                productFound = true;
                break;
            }
        }
        if (productFound) break;
    }

    if (!productFound) {
        cout << "Product not found!\n";
        return;
    }

    // Load all products from the category
    vector<Product> categoryProducts;
    loadProductsFromCategory(foundCategory, categoryProducts);

    // Create temp file and update the product
    string tempFilename = "temp_" + foundCategory + ".txt";
    ofstream temp(tempFilename);

    for (auto &p : categoryProducts) {
        if (p.id == id) {
            cout << "Enter New Name: ";
            cin.ignore();
            getline(cin, p.name);
            cout << "Enter New Price: ";
            cin >> p.price;
            cout << "Enter New Quantity: ";
            cin >> p.quantity;
        }
        temp << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
    }

    temp.close();

    // Replace the original file
    string originalFilename = getCategoryFilename(foundCategory);
    remove(originalFilename.c_str());
    rename(tempFilename.c_str(), originalFilename.c_str());

    cout << "Product updated successfully!\n";
}

void deleteProduct() {
    int id;
    cout << "\nEnter Product ID to Delete: ";
    cin >> id;

    // Find which category the product belongs to
    string foundCategory;
    bool productFound = false;

    for (const auto &category : categories) {
        vector<Product> products;
        loadProductsFromCategory(category, products);

        for (const auto &product : products) {
            if (product.id == id) {
                foundCategory = category;
                productFound = true;
                break;
            }
        }
        if (productFound) break;
    }

    if (!productFound) {
        cout << "Product not found!\n";
        return;
    }

    // Load all products from the category except the one to delete
    vector<Product> categoryProducts;
    loadProductsFromCategory(foundCategory, categoryProducts);

    // Create temp file without the deleted product
    string tempFilename = "temp_" + foundCategory + ".txt";
    ofstream temp(tempFilename);

    for (const auto &p : categoryProducts) {
        if (p.id != id) {
            temp << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
        }
    }

    temp.close();

    // Replace the original file
    string originalFilename = getCategoryFilename(foundCategory);
    remove(originalFilename.c_str());
    rename(tempFilename.c_str(), originalFilename.c_str());

    cout << "Product deleted successfully!\n";
}

//================= BILLING SYSTEM =================//
Customer findCustomerByPhone(const string &phone) {
    ifstream file("customers.txt");
    Customer customer;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, customer.name, ',');
        getline(ss, customer.username, ',');
        getline(ss, customer.password, ',');
        getline(ss, customer.phone, ',');

        if (customer.phone == phone) {
            file.close();
            return customer;
        }
    }

    file.close();
    Customer notFound;
    notFound.name = "";
    return notFound;
}

void salesmanBilling() {
    vector<CartItem> cart;
    string customerPhone;
    Customer customer;

    cout << "\n=== Salesman Billing ===\n";
    cout << "Enter Customer Phone: ";
    cin >> customerPhone;

    customer = findCustomerByPhone(customerPhone);
    if (customer.name == "") {
        cout << "Customer not found! Please register first.\n";
        return;
    }

    cout << "Customer Found: " << customer.name << endl;

    int choice;
    do {
        cout << "\n--- Billing for " << customer.name << " ---\n";
        cout << "1. View All Products\n";
        cout << "2. Add to Cart\n";
        cout << "3. View Cart\n";
        cout << "4. Checkout\n";
        cout << "5. Cancel\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewAllProducts();
                break;

            case 2: {
                int id, qty;
                cout << "Enter Product ID to add to cart: ";
                cin >> id;
                cout << "Enter Quantity: ";
                cin >> qty;

                Product p = findProductById(id);
                if (p.id == -1) {
                    cout << "Product Not Found!\n";
                    break;
                }

                if (qty <= p.quantity) {
                    bool inCart = false;
                    for (auto &item : cart) {
                        if (item.product.id == id) {
                            item.quantity += qty;
                            inCart = true;
                            cout << "Added " << qty << " more of " << p.name << " to cart\n";
                            break;
                        }
                    }
                    if (!inCart) {
                        CartItem newItem;
                        newItem.product = p;
                        newItem.quantity = qty;
                        cart.push_back(newItem);
                        cout << "Added " << p.name << " to cart\n";
                    }
                } else {
                    cout << "Not enough stock! Available: " << p.quantity << endl;
                }
                break;
            }

            case 3:
                viewCart(cart);
                break;

            case 4: {
                if (cart.empty()) {
                    cout << "Cart is empty!\n";
                    break;
                }

                viewCart(cart);
                double total = calculateTotal(cart);
                cout << "\nTotal Amount: " << total << " Taka\n";

                char confirm;
                cout << "Confirm purchase? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    PurchaseHistory history;
                    history.serialNumber = getNextSerialNumber();
                    history.username = "Salesman_Transaction";
                    history.customerName = customer.name;
                    history.customerPhone = customer.phone;
                    history.totalAmount = total;
                    history.items = cart;

                    time_t now = time(0);
                    char* dt = ctime(&now);
                    history.dateTime = dt;
                    history.dateTime.erase(history.dateTime.find('\n'));

                    savePurchaseHistory(history);

                    // Update product quantities
                    for (auto &item : cart) {
                        updateProductQuantity(item.product.id, item.quantity, item.product.category);
                    }

                    cout << "Checkout completed successfully!\n";
                    cout << "Invoice Number: " << history.serialNumber << endl;
                    cout << "Customer: " << customer.name << endl;
                    cout << "Phone: " << customer.phone << endl;
                    cout << "Total: " << total << " Taka\n";
                    cart.clear();
                } else {
                    cout << "Purchase cancelled.\n";
                }
                break;
            }

            case 5:
                cout << "Billing cancelled.\n";
                break;

            default:
                cout << "Invalid Option!\n";
        }
    } while (choice != 4 && choice != 5);
}

void customerBilling(Customer &customer) {
    vector<CartItem> cart;
    int choice;

    do {
        cout << "\n--- Shopping Cart ---\n";
        cout << "1. View All Products\n";
        cout << "2. Add to Cart\n";
        cout << "3. View Cart\n";
        cout << "4. Checkout\n";
        cout << "5. Back to Menu\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewAllProducts();
                break;

            case 2: {
                int id, qty;
                cout << "Enter Product ID to add to cart: ";
                cin >> id;
                cout << "Enter Quantity: ";
                cin >> qty;

                Product p = findProductById(id);
                if (p.id == -1) {
                    cout << "Product Not Found!\n";
                    break;
                }

                if (qty <= p.quantity) {
                    bool inCart = false;
                    for (auto &item : cart) {
                        if (item.product.id == id) {
                            item.quantity += qty;
                            inCart = true;
                            cout << "Added " << qty << " more of " << p.name << " to cart\n";
                            break;
                        }
                    }
                    if (!inCart) {
                        CartItem newItem;
                        newItem.product = p;
                        newItem.quantity = qty;
                        cart.push_back(newItem);
                        cout << "Added " << p.name << " to cart\n";
                    }
                } else {
                    cout << "Not enough stock! Available: " << p.quantity << endl;
                }
                break;
            }

            case 3:
                viewCart(cart);
                break;

            case 4: {
                if (cart.empty()) {
                    cout << "Cart is empty!\n";
                    break;
                }

                viewCart(cart);
                double total = calculateTotal(cart);
                cout << "\nTotal Amount: " << total << " Taka\n";

                char confirm;
                cout << "Confirm purchase? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    PurchaseHistory history;
                    history.serialNumber = getNextSerialNumber();
                    history.username = customer.username;
                    history.customerName = customer.name;
                    history.customerPhone = customer.phone;
                    history.totalAmount = total;
                    history.items = cart;

                    time_t now = time(0);
                    char* dt = ctime(&now);
                    history.dateTime = dt;
                    history.dateTime.erase(history.dateTime.find('\n'));

                    savePurchaseHistory(history);

                    for (auto &item : cart) {
                        updateProductQuantity(item.product.id, item.quantity, item.product.category);
                    }

                    cout << "Purchase completed successfully!\n";
                    cout << "Invoice Number: " << history.serialNumber << endl;
                    cout << "Thank you for your purchase!\n";
                    cart.clear();
                } else {
                    cout << "Purchase cancelled.\n";
                }
                break;
            }

            case 5:
                cout << "Returning to menu...\n";
                break;

            default:
                cout << "Invalid Option!\n";
        }
    } while (choice != 5);
}

//================= CART MANAGEMENT =================//
void viewCart(vector<CartItem> &cart) {
    if (cart.empty()) {
        cout << "Cart is empty!\n";
        return;
    }

    cout << "\n--- Your Cart ---\n";
    cout << left << setw(20) << "Product" << setw(15) << "Category"
         << setw(10) << "Price" << setw(10) << "Qty" << setw(15) << "Subtotal" << endl;
    cout << "-------------------------------------------------------------------\n";

    for (auto &item : cart) {
        double subtotal = item.product.price * item.quantity;
        cout << left << setw(20) << item.product.name
             << setw(15) << item.product.category
             << setw(10) << item.product.price
             << setw(10) << item.quantity
             << setw(15) << subtotal << endl;
    }

    double total = calculateTotal(cart);
    cout << "-------------------------------------------------------------------\n";
    cout << "Total: " << total << " Taka\n";
}

double calculateTotal(vector<CartItem> &cart) {
    double total = 0;
    for (auto &item : cart) {
        total += item.product.price * item.quantity;
    }
    return total;
}

//================= UPDATE PRODUCT QUANTITY =================//
void updateProductQuantity(int productId, int soldQuantity, const string &category) {
    vector<Product> products;
    loadProductsFromCategory(category, products);

    string tempFilename = "temp_" + category + ".txt";
    ofstream temp(tempFilename);

    for (auto &p : products) {
        if (p.id == productId) {
            p.quantity -= soldQuantity;
        }
        temp << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
    }

    temp.close();

    string originalFilename = getCategoryFilename(category);
    remove(originalFilename.c_str());
    rename(tempFilename.c_str(), originalFilename.c_str());
}

//================= PURCHASE HISTORY MANAGEMENT =================//
void savePurchaseHistory(const PurchaseHistory &history) {
    ofstream file("purchase_history.txt", ios::app);

    if (!file.is_open()) {
        cout << "Error creating purchase history file!\n";
        return;
    }

    file << "=== Invoice: " << history.serialNumber << " ===\n";
    file << "Customer: " << history.customerName << "\n";
    file << "Phone: " << history.customerPhone << "\n";
    file << "Username: " << history.username << "\n";
    file << "Date: " << history.dateTime << "\n";
    file << "Items:\n";

    for (const auto &item : history.items) {
        file << item.product.id << " " << item.product.name << " "
             << item.product.category << " " << item.product.price << " "
             << item.quantity << " " << (item.product.price * item.quantity) << "\n";
    }

    file << "Total: " << history.totalAmount << "\n";
    file << "=== End Invoice ===\n\n";
    file.close();
}

int getNextSerialNumber() {
    ifstream file("purchase_counter.txt");
    int counter = 1;

    if (file.is_open()) {
        file >> counter;
        file.close();
    }

    ofstream outFile("purchase_counter.txt");
    outFile << (counter + 1);
    outFile.close();

    return counter;
}

void viewPurchaseHistory(const string &username) {
    ifstream file("purchase_history.txt");

    if (!file.is_open()) {
        cout << "No purchase history found.\n";
        return;
    }

    string line;
    bool found = false;
    bool showInvoice = false;

    cout << "\n--- Purchase History ---\n";

    while (getline(file, line)) {
        if (username.empty() || line.find("Username: " + username) != string::npos) {
            if (line.find("=== Invoice:") != string::npos) {
                found = true;
                showInvoice = true;
            }
        }

        if (showInvoice) {
            cout << line << endl;
        }

        if (line.find("=== End Invoice ===") != string::npos) {
            showInvoice = false;
            cout << endl;
        }
    }

    file.close();

    if (!found) {
        cout << "No purchase history found";
        if (!username.empty()) {
            cout << " for user: " << username;
        }
        cout << endl;
    }
}

void viewAllPurchaseHistory() {
    viewPurchaseHistory("");
}

//================= CUSTOMER LOGIN FLOW =================//
void customerLoginFlow() {
    char hasAccount;
    cout << "\nDo you have an existing account? (y/n): ";
    cin >> hasAccount;

    if (hasAccount == 'y' || hasAccount == 'Y') {
        Customer customer;
        if (customerLogin(customer)) {
            customerMenu(customer);
        }
    } else {
        customerRegistration();
    }
}

//================= MAIN =================//
int main() {
    int choice;

    while (true) {
        cout << "\n===== Super Market Billing System =====\n";
        cout << "1. Owner Login\n2. Salesman Login\n3. Customer Login\n4. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (ownerLogin()) ownerMenu();
                break;
            case 2: {
                Salesman salesman;
                if (salesmanLogin(salesman)) salesmanMenu(salesman);
                break;
            }
            case 3:
                customerLoginFlow();
                break;
            case 4:
                cout << "Exiting Program...\n";
                return 0;
            default:
                cout << "Invalid Choice!\n";
        }
    }
}
