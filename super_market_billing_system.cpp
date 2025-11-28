#include <bits/stdc++.h>
using namespace std;

struct User {
    string username, password, role;
};

struct Product {
    int id;
    string name;
    double price;
    int quantity;
};

struct CartItem {
    Product product;
    int quantity;
};

bool login(string role, User &loggedUser);
void ownerMenu();
void salesmanMenu();
void customerMenu();
void addProduct();
void viewProducts();
void editProduct();
void deleteProduct();
void billing();
void customerBilling();
void viewCart(vector<CartItem> &cart);
double calculateTotal(vector<CartItem> &cart);
void updateProductQuantity(int productId, int soldQuantity);

//================= LOGIN =================//
bool login(string role, User &loggedUser) {
    string username, password;
    cout << "\nEnter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    ifstream file("users.txt");
    if (!file.is_open()) {
        cout << "Error: users.txt not found!\n";
        return false;
    }

    User user;
    bool success = false;

    while (file >> user.username >> user.password >> user.role) {
        if (user.username == username && user.password == password && user.role == role) {
            loggedUser = user;
            success = true;
            break;
        }
    }
    file.close();

    if (success) {
        cout << "Login Successful! (" << role << ")\n";
        return true;
    } else {
        cout << "Invalid Username/Password or Role.\n";
        return false;
    }
}

//================= OWNER MENU =================//
void ownerMenu() {
    int choice;
    do {
        cout << "\n--- Owner Menu ---\n";
        cout << "1. Add Product\n2. View Products\n3. Edit Product\n4. Delete Product\n5. Logout\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addProduct(); break;
            case 2: viewProducts(); break;
            case 3: editProduct(); break;
            case 4: deleteProduct(); break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid Option!\n";
        }
    } while (choice != 5);
}

//================= SALESMAN MENU =================//
void salesmanMenu() {
    int choice;
    do {
        cout << "\n--- Salesman Menu ---\n";
        cout << "1. View Products\n2. Billing\n3. Logout\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewProducts(); break;
            case 2: billing(); break;
            case 3: cout << "Logging out...\n"; break;
            default: cout << "Invalid Option!\n";
        }
    } while (choice != 3);
}

//================= CUSTOMER MENU =================//
void customerMenu() {
    int choice;
    do {
        cout << "\n--- Customer Menu ---\n";
        cout << "1. View Products\n2. Purchase Products\n3. Logout\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewProducts(); break;
            case 2: customerBilling(); break;
            case 3: cout << "Logging out...\n"; break;
            default: cout << "Invalid Option!\n";
        }
    } while (choice != 3);
}

//================= PRODUCT MANAGEMENT =================//
void addProduct() {
    Product p;
    cout << "\nEnter Product ID: ";
    cin >> p.id;
    cout << "Enter Product Name: ";
    cin >> p.name;
    cout << "Enter Price: ";
    cin >> p.price;
    cout << "Enter Quantity: ";
    cin >> p.quantity;

    ofstream file("products.txt", ios::app);
    file << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
    file.close();
    cout << "Product Added Successfully!\n";
}

void viewProducts() {
    ifstream file("products.txt");
    if (!file.is_open()) {
        cout << "No products found.\n";
        return;
    }

    Product p;
    cout << "\n" << left << setw(10) << "ID" << setw(20) << "Name"
         << setw(10) << "Price" << setw(10) << "Qty" << endl;
    cout << "--------------------------------------------------\n";
    while (file >> p.id >> p.name >> p.price >> p.quantity) {
        cout << left << setw(10) << p.id << setw(20) << p.name
             << setw(10) << p.price << setw(10) << p.quantity << endl;
    }
    file.close();
}

void editProduct() {
    int id;
    cout << "\nEnter Product ID to Edit: ";
    cin >> id;

    ifstream file("products.txt");
    ofstream temp("temp.txt");
    Product p;
    bool found = false;

    while (file >> p.id >> p.name >> p.price >> p.quantity) {
        if (p.id == id) {
            cout << "Enter New Name: ";
            cin >> p.name;
            cout << "Enter New Price: ";
            cin >> p.price;
            cout << "Enter New Quantity: ";
            cin >> p.quantity;
            found = true;
        }
        temp << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
    }

    file.close();
    temp.close();

    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (found) cout << "Product Updated Successfully!\n";
    else cout << "Product Not Found!\n";
}

void deleteProduct() {
    int id;
    cout << "\nEnter Product ID to Delete: ";
    cin >> id;

    ifstream file("products.txt");
    ofstream temp("temp.txt");
    Product p;
    bool found = false;

    while (file >> p.id >> p.name >> p.price >> p.quantity) {
        if (p.id == id) {
            found = true;
            continue;
        }
        temp << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
    }

    file.close();
    temp.close();

    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (found) cout << "Product Deleted Successfully!\n";
    else cout << "Product Not Found!\n";
}

//================= BILLING (Salesman) =================//
void billing() {
    int id, qty;
    double total = 0;
    Product p;

    cout << "\nEnter Product ID to Buy (0 to stop): ";
    while (cin >> id && id != 0) {
        ifstream file("products.txt");
        bool found = false;
        while (file >> p.id >> p.name >> p.price >> p.quantity) {
            if (p.id == id) {
                cout << "Enter Quantity: ";
                cin >> qty;
                if (qty <= p.quantity) {
                    total += qty * p.price;
                    cout << "Added to bill: " << p.name << " x" << qty
                         << " = " << qty * p.price << " Taka\n";
                    // Update product quantity
                    updateProductQuantity(id, qty);
                } else {
                    cout << "Not enough stock! Available: " << p.quantity << endl;
                }
                found = true;
                break;
            }
        }
        file.close();
        if (!found) cout << "Product Not Found!\n";
        cout << "\nEnter Next Product ID (0 to stop): ";
    }
    cout << "\nTotal Bill = " << total << " Taka\n";
}

//================= CUSTOMER BILLING =================//
void customerBilling() {
    vector<CartItem> cart;
    int choice;

    do {
        cout << "\n--- Customer Shopping ---\n";
        cout << "1. View Products\n";
        cout << "2. Add to Cart\n";
        cout << "3. View Cart\n";
        cout << "4. Checkout\n";
        cout << "5. Back to Menu\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewProducts();
                break;

            case 2: {
                int id, qty;
                cout << "Enter Product ID to add to cart: ";
                cin >> id;
                cout << "Enter Quantity: ";
                cin >> qty;

                ifstream file("products.txt");
                Product p;
                bool found = false;
                bool inCart = false;

                // Check if product exists and has sufficient quantity
                while (file >> p.id >> p.name >> p.price >> p.quantity) {
                    if (p.id == id) {
                        found = true;
                        if (qty <= p.quantity) {
                            // Check if product already in cart
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
                }
                file.close();
                if (!found) {
                    cout << "Product Not Found!\n";
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
                    // Update product quantities in file
                    for (auto &item : cart) {
                        updateProductQuantity(item.product.id, item.quantity);
                    }
                    cout << "Purchase completed successfully!\n";
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
    cout << left << setw(20) << "Product" << setw(10) << "Price"
         << setw(10) << "Qty" << setw(15) << "Subtotal" << endl;
    cout << "--------------------------------------------------\n";

    for (auto &item : cart) {
        double subtotal = item.product.price * item.quantity;
        cout << left << setw(20) << item.product.name
             << setw(10) << item.product.price
             << setw(10) << item.quantity
             << setw(15) << subtotal << endl;
    }

    double total = calculateTotal(cart);
    cout << "--------------------------------------------------\n";
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
void updateProductQuantity(int productId, int soldQuantity) {
    ifstream file("products.txt");
    ofstream temp("temp.txt");
    Product p;
    bool found = false;

    while (file >> p.id >> p.name >> p.price >> p.quantity) {
        if (p.id == productId) {
            p.quantity -= soldQuantity;
            found = true;
        }
        temp << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
    }

    file.close();
    temp.close();

    remove("products.txt");
    rename("temp.txt", "products.txt");
}

//================= MAIN =================//
int main() {
    int choice;
    User user;

    while (true) {
        cout << "\n===== Super Market Billing System =====\n";
        cout << "1. Owner Login\n2. Salesman Login\n3. Customer Login\n4. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: if (login("Owner", user)) ownerMenu(); break;
            case 2: if (login("Salesman", user)) salesmanMenu(); break;
            case 3: if (login("Customer", user)) customerMenu(); break;
            case 4: cout << "Exiting Program...\n"; return 0;
            default: cout << "Invalid Choice!\n";
        }
    }
}
