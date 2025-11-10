

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

// ==================== COLOR CODES ====================
class Color {
public:
    static const string RESET;
    static const string RED;
    static const string GREEN;
    static const string YELLOW;
    static const string BLUE;
    static const string MAGENTA;
    static const string CYAN;
    static const string WHITE;
    static const string BOLD;
    static const string UNDERLINE;
};

const string Color::RESET = "\033[0m";
const string Color::RED = "\033[31m";
const string Color::GREEN = "\033[32m";
const string Color::YELLOW = "\033[33m";
const string Color::BLUE = "\033[34m";
const string Color::MAGENTA = "\033[35m";
const string Color::CYAN = "\033[36m";
const string Color::WHITE = "\033[37m";
const string Color::BOLD = "\033[1m";
const string Color::UNDERLINE = "\033[4m";


void consumeNewline() {
    if (cin.peek() == '\n') cin.get();
}


double getValidatedDouble(const string& prompt) {
    double value;
    cout << prompt;

    while (!(cin >> value)) {
        cin.clear();
        consumeNewline();
        cout << Color::RED << "Invalid input! Please enter a valid number: " << Color::RESET;
    }
    consumeNewline();
    return value;
}


int getValidatedInt(const string& prompt) {
    int value;
    cout << prompt;

    while (!(cin >> value)) {
        cin.clear();
        consumeNewline();
        cout << Color::RED << "Invalid input! Please enter a valid number: " << Color::RESET;
    }
    consumeNewline();
    return value;
}

bool equalsIgnoreCaseASCII(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        char ca = a[i];
        char cb = b[i];
        if (ca == cb) continue;
        if (ca + 32 == cb || ca - 32 == cb) continue;
        return false;
    }
    return true;
}

bool isValidEmail(const string& mail) {
    static const regex pattern(
        "^(?![.-])[A-Za-z0-9]+([._%+-]?[A-Za-z0-9]+)*@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"
    );
    return regex_match(mail, pattern);
}

bool isAlphaNumeric(const string& s) {
    for (size_t i = 0; i < s.size(); i++)
        if (!((s[i] >= 'A' && s[i] <= 'Z') ||
            (s[i] >= 'a' && s[i] <= 'z') ||
            (s[i] >= '0' && s[i] <= '9')))
            return false;
    return true;
}

bool isAlphaOnly(const string& s) {
    for (size_t i = 0; i < s.size(); i++)
        if (!((s[i] >= 'A' && s[i] <= 'Z') ||
            (s[i] >= 'a' && s[i] <= 'z') ||
            (s[i] == ' ')))
            return false;
    return true;
}

// ==================== DATE-TIME STRUCT ====================
struct DateTime {
    int day, month, year, hour, minute;
    DateTime() {
        time_t now = time(0);
        tm localTime;
        localtime_s(&localTime, &now);
        day = localTime.tm_mday;
        month = localTime.tm_mon + 1;
        year = localTime.tm_year + 1900;
        hour = localTime.tm_hour;
        minute = localTime.tm_min;
    }
    string toFileFormat() const {
        stringstream ss;
        ss << day << "|" << month << "|" << year << "|" << hour << "|" << minute;
        return ss.str();
    }
    string toString() const {
        stringstream ss;
        ss << (day < 10 ? "0" : "") << day << "."
            << (month < 10 ? "0" : "") << month << "."
            << year << " - "
            << (hour < 10 ? "0" : "") << hour << ":"
            << (minute < 10 ? "0" : "") << minute;
        return ss.str();
    }
};

// ==================== PERSON ====================
class Person {
private:
    string username;
    string password;
    string email;
public:
    Person() {}
    Person(string username, string password, string email) {
        setUsername(username);
        setPassword(password);
        setEmail(email);
    }
    void setUsername(string username) {
        if (username.length() < 4)
            throw string(Color::RED + "Username must be at least 4 characters!" + Color::RESET);
        if (!isAlphaNumeric(username))
            throw string(Color::RED + "Username can only contain letters and numbers!" + Color::RESET);
        this->username = username;
    }
    void setPassword(string password) {
        if (password.length() < 6)
            throw string(Color::RED + "Password must be at least 6 characters!" + Color::RESET);
        this->password = password;
    }
    void setEmail(string email) {
        if (!isValidEmail(email))
            throw string(Color::RED + "Invalid email format!" + Color::RESET);
        this->email = email;
    }
    string getUsername() const noexcept { return username; }
    string getPassword() const noexcept { return password; }
    string getEmail() const noexcept { return email; }
};

// ==================== INGREDIENT ====================
class Ingredient {
private:
    string name;
    double pricePerKg;
    double quantity;
public:
    Ingredient() : name(""), pricePerKg(0), quantity(0) {}
    Ingredient(string name, double price, double qty) {
        setName(name);
        setPrice(price);
        setQuantity(qty);
    }
    void setName(string n) {
        if (n.empty()) throw string(Color::RED + "Ingredient name cannot be empty!" + Color::RESET);
        name = n;
    }
    void setPrice(double p) {
        if (p <= 0) throw string(Color::RED + "Price must be positive!" + Color::RESET);
        pricePerKg = p;
    }
    void setQuantity(double q) {
        if (q <= 0) throw string(Color::RED + "Quantity must be positive!" + Color::RESET);
        quantity = q;
    }
    string getName() const { return name; }
    double getPrice() const { return pricePerKg; }
    double getQuantity() const { return quantity; }
    void increaseQuantity(double q) {
        if (q <= 0) throw string(Color::RED + "Increase amount must be positive!" + Color::RESET);
        quantity += q;
    }
    void decreaseQuantity(double q) {
        if (q <= 0) throw string(Color::RED + "Decrease amount must be positive!" + Color::RESET);
        if (q > quantity) throw string(Color::RED + "Not enough quantity in stock!" + Color::RESET);
        quantity -= q;
    }
    void showShort() const {
        cout << Color::CYAN << name << Color::RESET << " | "
            << Color::YELLOW << quantity << " kg" << Color::RESET << " | "
            << Color::GREEN << pricePerKg << " AZN/kg" << Color::RESET << endl;
    }
};

// ==================== STOCK ====================
class Stock {
private:
    vector<Ingredient> ingredients;
public:
    void addIngredient(const Ingredient& ing) {
        for (size_t i = 0; i < ingredients.size(); i++) {
            if (equalsIgnoreCaseASCII(ingredients[i].getName(), ing.getName())) {
                ingredients[i].increaseQuantity(ing.getQuantity());
                return;
            }
        }
        ingredients.push_back(ing);
    }
    Ingredient* findByName(const string& n) {
        for (size_t i = 0; i < ingredients.size(); i++)
            if (equalsIgnoreCaseASCII(ingredients[i].getName(), n))
                return &ingredients[i];
        return NULL;
    }
    void show() const {
        cout << Color::BOLD << Color::BLUE << "\n--- STOCK ---" << Color::RESET << endl;
        if (ingredients.empty()) {
            cout << Color::YELLOW << "Stock is empty!" << Color::RESET << endl;
            return;
        }
        for (size_t i = 0; i < ingredients.size(); i++)
            ingredients[i].showShort();
    }
    void saveToFile() {
        ofstream f("ingredients.txt");
        if (!f.is_open()) {
            cout << Color::RED << "Could not open ingredients.txt file!" << Color::RESET << endl;
            return;
        }
        for (size_t i = 0; i < ingredients.size(); i++) {
            f << ingredients[i].getName() << "|"
                << ingredients[i].getPrice() << "|"
                << ingredients[i].getQuantity() << endl;
        }
        f.close();
    }
    void loadFromFile() {
        ifstream f("ingredients.txt");
        if (!f.is_open()) {
            cout << Color::YELLOW << "ingredients.txt file not found!" << Color::RESET << endl;
            return;
        }
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string n; double price, qty;
            getline(ss, n, '|');
            ss >> price; ss.ignore(1, '|');
            ss >> qty;
            if (!ss.fail())
                ingredients.push_back(Ingredient(n, price, qty));
        }
        f.close();
    }
};

// ==================== INGREDIENT USAGE ====================
struct IngredientUsage {
    Ingredient* ingredient;
    double usedKg;
};

// ==================== DISH ====================
class Dish {
private:
    string name;
    double price;
    string info;
    vector<IngredientUsage> ingredients;
public:
    Dish() {}
    Dish(string name, double price, string info, const vector<IngredientUsage>& ingList) {
        setName(name);
        setPrice(price);
        setInfo(info);
        setIngredients(ingList);
    }
    void setName(string n) {
        if (n.empty()) throw string(Color::RED + "Dish name cannot be empty!" + Color::RESET);
        name = n;
    }
    void setPrice(double p) {
        if (p <= 0) throw string(Color::RED + "Price must be positive!" + Color::RESET);
        price = p;
    }
    void setInfo(string i) {
        if (i.length() < 5) throw string(Color::RED + "Info must contain at least 5 characters!" + Color::RESET);
        info = i;
    }
    void setIngredients(const vector<IngredientUsage>& list) {
        if (list.size() < 2) throw string(Color::RED + "Each dish must contain at least 2 ingredients!" + Color::RESET);
        ingredients = list;
    }
    string getName() const { return name; }
    double getPrice() const { return price; }
    string getInfo() const { return info; }
    vector<IngredientUsage> getIngredients() const { return ingredients; }
    void show() const {
        cout << Color::BOLD << Color::MAGENTA << "\n" << name << Color::RESET
            << " - " << Color::GREEN << price << " AZN" << Color::RESET << endl;
        cout << Color::CYAN << "Info: " << Color::RESET << info << endl;
        cout << Color::YELLOW << "Ingredients:" << Color::RESET << endl;
        for (size_t i = 0; i < ingredients.size(); i++)
            cout << " - " << Color::CYAN << ingredients[i].ingredient->getName() << Color::RESET
            << " (" << Color::YELLOW << ingredients[i].usedKg << " kg" << Color::RESET << ")\n";
    }
};

// ==================== ORDER HISTORY ====================
struct OrderRecord {
    string username;
    string dishName;
    double totalPrice;
    DateTime date;
};

class OrderHistory {
private:
    vector<OrderRecord> history;
public:
    void addOrder(const string& username, const string& dish, double price) {
        OrderRecord rec;
        rec.username = username;
        rec.dishName = dish;
        rec.totalPrice = price;
        rec.date = DateTime();
        history.push_back(rec);
        saveToFile(rec);
    }

    void saveToFile(const OrderRecord& rec) {
        ofstream f("orders.txt", ios::app);
        if (!f.is_open()) {
            cout << Color::RED << "Could not open orders.txt file!" << Color::RESET << endl;
            return;
        }
        f << rec.username << "|" << rec.dishName << "|" << rec.totalPrice << "|"
            << rec.date.toFileFormat() << endl;
        f.close();
    }

    void showUserHistory(const string& username) {
        ifstream f("orders.txt");
        if (!f.is_open()) {
            cout << Color::YELLOW << "orders.txt file not found!" << Color::RESET << endl;
            return;
        }
        string line;
        cout << Color::BOLD << Color::BLUE << "\nOrder history for " << username << ":" << Color::RESET << endl;
        bool found = false;
        while (getline(f, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string u, d; double p; int day, month, year, hour, minute; char sep;
            getline(ss, u, '|'); getline(ss, d, '|');
            ss >> p; ss.ignore(1, '|');
            ss >> day >> sep >> month >> sep >> year >> sep >> hour >> sep >> minute;
            if (equalsIgnoreCaseASCII(u, username)) {
                found = true;
                DateTime dt; dt.day = day; dt.month = month; dt.year = year; dt.hour = hour; dt.minute = minute;
                cout << " - " << Color::CYAN << d << Color::RESET << " (" << Color::GREEN << p << " AZN"
                    << Color::RESET << ") [" << Color::YELLOW << dt.toString() << Color::RESET << "]\n";
            }
        }
        f.close();
        if (!found) cout << Color::YELLOW << "No orders found!" << Color::RESET << endl;
    }
};

// ==================== ADMIN ====================
class Admin : public Person {
private:
    Stock stock;
    vector<Dish> menu;
    double budget;
public:
    Admin() : Person("admin", "admin123", "admin@gmail.com"), budget(15000) {
        stock.loadFromFile();
        loadBudget();
        loadMenuFromFile();
    }

    void saveBudget() {
        ofstream f("budget.txt");
        if (f.is_open()) f << budget;
        f.close();
    }

    void loadBudget() {
        ifstream f("budget.txt");
        if (f.is_open()) {
            f >> budget;
            f.close();
        }
        else {
            budget = 15000;
            saveBudget();
            cout << Color::GREEN << "New budget.txt file created (15000 AZN)." << Color::RESET << endl;
        }
    }

    void increaseBudget(double amount) {
        if (amount <= 0) throw string(Color::RED + "Amount to increase must be positive!" + Color::RESET);
        budget += amount;
        saveBudget();
    }

    Stock& getStockRef() { return stock; }
    Ingredient* findStockIngredient(const string& name) { return stock.findByName(name); }

    void showStock() const { stock.show(); }
    void showBudget() const {
        cout << Color::BOLD << Color::GREEN << "\nBudget: " << budget << " AZN" << Color::RESET << endl;
    }

    
    void addIngredientToStock() {
        string n; double price = 0, qty = 0;
        cout << Color::CYAN << "Ingredient name: " << Color::RESET;
        consumeNewline(); getline(cin, n);

        
        Ingredient* existingIng = stock.findByName(n);
        if (existingIng != NULL) {
            throw string(Color::RED + "Ingredient '" + n + "' already exists in stock!" + Color::RESET);
        }

        
        for (size_t i = 0; i < n.size(); i++) {
            if (!((n[i] >= 'A' && n[i] <= 'Z') ||
                (n[i] >= 'a' && n[i] <= 'z') ||
                n[i] == ' ')) {
                throw string(Color::RED + "Ingredient name must contain only letters!" + Color::RESET);
            }
        }

        
        price = getValidatedDouble(Color::CYAN + "Price per kg (AZN): " + Color::RESET);

        
        qty = getValidatedDouble(Color::CYAN + "Quantity (kg): " + Color::RESET);

        
        if (price <= 0) throw string(Color::RED + "Price must be positive!" + Color::RESET);
        if (qty <= 0) throw string(Color::RED + "Quantity must be positive!" + Color::RESET);

        double cost = price * qty;
        if (cost > budget) throw string(Color::RED + "Not enough funds in budget!" + Color::RESET);

        
        stock.addIngredient(Ingredient(n, price, qty));
        budget -= cost;
        saveBudget();
        stock.saveToFile();
        updateMenuAvailability();
        saveMenuToFile();

        cout << Color::GREEN << n << " added and " << cost << " AZN deducted from budget!" << Color::RESET << endl;
    }

    
    void increaseExistingIngredient() {
        string name; double qty = 0;
        cout << Color::CYAN << "Ingredient name to increase: " << Color::RESET;
        consumeNewline(); getline(cin, name);

        Ingredient* ing = stock.findByName(name);
        if (ing == NULL) throw string(Color::RED + "No such ingredient found!" + Color::RESET);

        
        qty = getValidatedDouble(Color::CYAN + "Quantity to add (kg): " + Color::RESET);

        if (qty <= 0) throw string(Color::RED + "Quantity must be positive!" + Color::RESET);
        double cost = qty * ing->getPrice();
        if (cost > budget) throw string(Color::RED + "Not enough funds in budget!" + Color::RESET);

        ing->increaseQuantity(qty);
        stock.saveToFile();
        budget -= cost;
        saveBudget();
        updateMenuAvailability();
        saveMenuToFile();
        cout << Color::GREEN << name << " increased and " << cost << " AZN deducted from budget!" << Color::RESET << endl;
    }

    
    Dish createDish() {
        string n, info; double p = 0; int count = 0;
        cout << Color::CYAN << "Dish name: " << Color::RESET;
        consumeNewline(); getline(cin, n);

        
        p = getValidatedDouble(Color::CYAN + "Price (AZN): " + Color::RESET);

        cout << Color::CYAN << "Additional info: " << Color::RESET;
        consumeNewline(); getline(cin, info);
        if (info.length() < 5) throw string(Color::RED + "Info must contain at least 5 characters!" + Color::RESET);

        
        count = getValidatedInt(Color::CYAN + "How many ingredients?: " + Color::RESET);

        if (count < 2) throw string(Color::RED + "At least 2 ingredients required!" + Color::RESET);
        vector<IngredientUsage> list;
        for (int i = 0; i < count; i++) {
            string name; double used = 0;
            cout << Color::YELLOW << "[" << i + 1 << "] Ingredient name: " << Color::RESET;
            consumeNewline(); getline(cin, name);
            for (size_t j = 0; j < list.size(); j++)
                if (equalsIgnoreCaseASCII(list[j].ingredient->getName(), name))
                    throw string(Color::RED + "This ingredient is already added!" + Color::RESET);
            Ingredient* ing = stock.findByName(name);
            if (ing == NULL) throw string(Color::RED + "This ingredient is not in stock!" + Color::RESET);

            
            used = getValidatedDouble(Color::CYAN + "Quantity (kg): " + Color::RESET);

            if (used <= 0) throw string(Color::RED + "Quantity must be positive!" + Color::RESET);
            IngredientUsage iu; iu.ingredient = ing; iu.usedKg = used; list.push_back(iu);
        }
        return Dish(n, p, info, list);
    }

    void addDishToMenu() {
        Dish d = createDish();
        for (size_t i = 0; i < menu.size(); i++)
            if (equalsIgnoreCaseASCII(menu[i].getName(), d.getName()))
                throw string(Color::RED + "Dish with this name already exists!" + Color::RESET);
        menu.push_back(d);
        saveMenuToFile();
        cout << Color::GREEN << d.getName() << " added to menu!" << Color::RESET << endl;
    }

    void saveMenuToFile() {
        ofstream f("menu.txt");
        if (!f.is_open()) {
            cout << Color::RED << "Could not open menu.txt file!" << Color::RESET << endl;
            return;
        }
        for (size_t i = 0; i < menu.size(); i++) {
            f << menu[i].getName() << "|" << menu[i].getPrice() << "|" << menu[i].getInfo() << endl;
            vector<IngredientUsage> list = menu[i].getIngredients();
            for (size_t j = 0; j < list.size(); j++)
                f << "ING|" << list[j].ingredient->getName() << "|" << list[j].usedKg << endl;
            f << "END_DISH" << endl;
        }
        f.close();
    }

    void loadMenuFromFile() {
        ifstream f("menu.txt");
        if (!f.is_open()) {
            cout << Color::YELLOW << "Menu file not found. Starting with empty menu." << Color::RESET << endl;
            menu.clear();
            return;
        }
        menu.clear();
        string line, name, info; double price; vector<IngredientUsage> list;
        while (getline(f, line)) {
            if (line.empty()) continue;
            if (line.rfind("ING|", 0) == 0) {
                stringstream ss(line.substr(4)); string ingName; double used;
                getline(ss, ingName, '|'); ss >> used;
                IngredientUsage iu; Ingredient* ref = stock.findByName(ingName);
                if (ref != NULL) { iu.ingredient = ref; iu.usedKg = used; list.push_back(iu); }
            }
            else if (line == "END_DISH") {
                if (!name.empty()) menu.push_back(Dish(name, price, info, list));
                list.clear(); name.clear(); info.clear(); price = 0;
            }
            else {
                stringstream ss(line);
                getline(ss, name, '|');
                ss >> price; ss.ignore(); getline(ss, info);
            }
        }
        f.close();
    }

    void updateMenuAvailability() {
        loadMenuFromFile();
        vector<Dish> availableDishes;
        for (size_t i = 0; i < menu.size(); i++) {
            bool available = true;
            vector<IngredientUsage> list = menu[i].getIngredients();
            for (size_t j = 0; j < list.size(); j++) {
                Ingredient* ing = stock.findByName(list[j].ingredient->getName());
                if (ing == NULL) {
                    available = false;
                    break;
                }
                if (ing->getQuantity() < list[j].usedKg) {
                    available = false;
                    break;
                }
            }
            if (available) availableDishes.push_back(menu[i]);
        }
        menu = availableDishes;
        saveMenuToFile();
    }

    void showMenu() const {
        if (menu.empty()) cout << Color::YELLOW << "Menu is empty!" << Color::RESET << endl;
        else {
            cout << Color::BOLD << Color::MAGENTA << "\n=== MENU ===" << Color::RESET << endl;
            for (size_t i = 0; i < menu.size(); i++)
                cout << i + 1 << ". " << Color::CYAN << menu[i].getName() << Color::RESET
                << " - " << Color::GREEN << menu[i].getPrice() << " AZN" << Color::RESET << endl;
        }
    }

    vector<Dish>& getMenu() { return menu; }
};

// ==================== CART STRUCT ====================
struct CartItem {
    Dish dish;
    vector<IngredientUsage> modifiedIngredients;
    double totalPrice;
    CartItem(const Dish& d) {
        dish = d;
        modifiedIngredients = d.getIngredients();
        totalPrice = d.getPrice();
    }
};

// ==================== CART CLASS ====================
class Cart {
private:
    vector<CartItem> items;
    OrderHistory& history;
public:
    Cart(OrderHistory& hist) : history(hist) {}

    void addDish(const Dish& d) {
        for (size_t i = 0; i < items.size(); i++)
            if (equalsIgnoreCaseASCII(items[i].dish.getName(), d.getName()))
                throw string(Color::RED + "This dish is already in cart!" + Color::RESET);
        items.push_back(CartItem(d));
        cout << Color::GREEN << d.getName() << " added to cart!" << Color::RESET << endl;
    }

    void removeDish(const string& name) {
        for (size_t i = 0; i < items.size(); i++) {
            if (equalsIgnoreCaseASCII(items[i].dish.getName(), name)) {
                items.erase(items.begin() + i);
                cout << Color::RED << name << " removed from cart!" << Color::RESET << endl;
                return;
            }
        }
        throw string(Color::RED + "No such dish in cart!" + Color::RESET);
    }

    void modifyIngredient(const string& dishName, const string& ingName, double newKg) {
        if (newKg <= 0) throw string(Color::RED + "New quantity must be positive!" + Color::RESET);
        for (size_t i = 0; i < items.size(); i++) {
            if (equalsIgnoreCaseASCII(items[i].dish.getName(), dishName)) {
                for (size_t j = 0; j < items[i].modifiedIngredients.size(); j++) {
                    if (equalsIgnoreCaseASCII(items[i].modifiedIngredients[j].ingredient->getName(), ingName)) {
                        double oldKg = items[i].modifiedIngredients[j].usedKg;
                        double ratio = newKg / oldKg;
                        items[i].modifiedIngredients[j].usedKg = newKg;
                        items[i].totalPrice *= ratio;
                        cout << Color::YELLOW << ingName << " changed: " << oldKg << " -> " << newKg
                            << " kg. New price: " << items[i].totalPrice << " AZN" << Color::RESET << endl;
                        return;
                    }
                }
            }
        }
        throw string(Color::RED + "Ingredient not found!" + Color::RESET);
    }

    void showCart() const {
        if (items.empty()) {
            cout << Color::YELLOW << "Cart is empty!" << Color::RESET << endl;
            return;
        }
        double total = 0;
        cout << Color::BOLD << Color::BLUE << "\n=== CART ===" << Color::RESET << endl;
        for (size_t i = 0; i < items.size(); i++) {
            cout << i + 1 << ". " << Color::CYAN << items[i].dish.getName() << Color::RESET
                << " - " << Color::GREEN << items[i].totalPrice << " AZN" << Color::RESET << endl;
            total += items[i].totalPrice;
        }
        cout << Color::BOLD << "Total: " << Color::GREEN << total << " AZN" << Color::RESET << endl;
    }

    double calculateTotal() const {
        double total = 0;
        for (size_t i = 0; i < items.size(); i++)
            total += items[i].totalPrice;
        return total;
    }

    void confirmOrder(Admin& admin, const string& username) {
        if (items.empty()) throw string(Color::RED + "Cart is empty!" + Color::RESET);
        double totalAmount = calculateTotal();

        for (size_t i = 0; i < items.size(); i++)
            for (size_t j = 0; j < items[i].modifiedIngredients.size(); j++) {
                Ingredient* stockIng = admin.findStockIngredient(items[i].modifiedIngredients[j].ingredient->getName());
                if (stockIng == NULL) throw string(Color::RED + "'" + items[i].modifiedIngredients[j].ingredient->getName() + "' not in stock!" + Color::RESET);
                if (stockIng->getQuantity() < items[i].modifiedIngredients[j].usedKg)
                    throw string(Color::RED + "Not enough '" + items[i].modifiedIngredients[j].ingredient->getName() + "' in stock!" + Color::RESET);
            }

        for (size_t i = 0; i < items.size(); i++)
            for (size_t j = 0; j < items[i].modifiedIngredients.size(); j++) {
                Ingredient* stockIng = admin.findStockIngredient(items[i].modifiedIngredients[j].ingredient->getName());
                stockIng->decreaseQuantity(items[i].modifiedIngredients[j].usedKg);
            }

        admin.getStockRef().saveToFile();
        admin.increaseBudget(totalAmount);
        admin.updateMenuAvailability();

        for (size_t i = 0; i < items.size(); i++) {
            history.addOrder(username, items[i].dish.getName(), items[i].totalPrice);
        }

        cout << Color::GREEN << "Payment completed. Budget updated." << Color::RESET << endl;
        items.clear();
    }
};

// ==================== USER ====================
class User : public Person {
private:
    string name;
    int age;
public:
    User() : name(""), age(0) {}
    User(string u, string p, string m, string n, int a)
        : Person(u, p, m) {
        setName(n);
        setAge(a);
    }

    void setName(string n) {
        if (!isAlphaOnly(n)) throw string(Color::RED + "Name must contain only letters!" + Color::RESET);
        name = n;
    }
    void setAge(int a) {
        if (a <= 0 || a > 120)
            throw string(Color::RED + "Age must be between 1-120!" + Color::RESET);
        age = a;
    }
    string getName() const { return name; }
    int getAge() const { return age; }
};

// ==================== USER MANAGER ====================
class UserManager {
private:
    vector<User> users;
    Admin admin;
    OrderHistory history;
public:
    void saveUsersToFile() {
        ofstream f("users.txt");
        if (!f.is_open()) {
            cout << Color::RED << "Could not open users.txt file!" << Color::RESET << endl;
            return;
        }
        for (size_t i = 0; i < users.size(); i++)
            f << users[i].getUsername() << "|" << users[i].getPassword() << "|"
            << users[i].getEmail() << "|" << users[i].getName() << "|"
            << users[i].getAge() << endl;
        f.close();
    }

    void loadUsersFromFile() {
        ifstream f("users.txt");
        if (!f.is_open()) {
            cout << Color::YELLOW << "users.txt file not found!" << Color::RESET << endl;
            return;
        }
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string u, p, m, n; int a;
            getline(ss, u, '|'); getline(ss, p, '|'); getline(ss, m, '|'); getline(ss, n, '|'); ss >> a;
            users.push_back(User(u, p, m, n, a));
        }
        f.close();
    }

    void signUp() {
        string u, p, m, n; int a;
        cout << Color::CYAN << "Username: " << Color::RESET; cin >> u;
        cout << Color::CYAN << "Password: " << Color::RESET; cin >> p;
        cout << Color::CYAN << "Email: " << Color::RESET; cin >> m;
        cout << Color::CYAN << "Name: " << Color::RESET; consumeNewline(); getline(cin, n);

        
        a = getValidatedInt(Color::CYAN + "Age: " + Color::RESET);

        for (size_t i = 0; i < users.size(); i++)
            if (equalsIgnoreCaseASCII(users[i].getUsername(), u))
                throw string(Color::RED + "This username already exists!" + Color::RESET);
        users.push_back(User(u, p, m, n, a));
        cout << Color::GREEN << "Registration completed!" << Color::RESET << endl;
        saveUsersToFile();
    }

    void signIn() {
        string u, p, m;
        cout << Color::CYAN << "Username: " << Color::RESET; cin >> u;
        cout << Color::CYAN << "Password: " << Color::RESET; cin >> p;
        cout << Color::CYAN << "Email: " << Color::RESET; cin >> m;

        // ==================== ADMIN PANEL ====================
        if (u == admin.getUsername() && p == admin.getPassword() && m == admin.getEmail()) {
            cout << Color::GREEN << "\nLogged in as Admin!" << Color::RESET << endl;
            int choice;
            while (true) {
                cout << Color::BOLD << Color::MAGENTA << "\n=== ADMIN PANEL ===" << Color::RESET << endl
                    << "1. Add new ingredient\n"
                    << "2. Add new dish\n"
                    << "3. Show stock\n"
                    << "4. Show menu\n"
                    << "5. Increase existing ingredient\n"
                    << "6. Show budget\n"
                    << "0. Exit\n"
                    << Color::CYAN << "Choice: " << Color::RESET;
                if (!(cin >> choice)) {
                    cin.clear(); consumeNewline();
                    cout << Color::RED << "Please enter a number!" << Color::RESET << endl;
                    continue;
                }
                try {
                    if (choice == 1) admin.addIngredientToStock();
                    else if (choice == 2) admin.addDishToMenu();
                    else if (choice == 3) admin.showStock();
                    else if (choice == 4) admin.showMenu();
                    else if (choice == 5) admin.increaseExistingIngredient();
                    else if (choice == 6) admin.showBudget();
                    else if (choice == 0) break;
                    else cout << Color::RED << "Wrong choice!" << Color::RESET << endl;
                }
                catch (string e) { cout << e << endl; }
            }
            return;
        }

        // ==================== USER PANEL ====================
        for (size_t i = 0; i < users.size(); i++) {
            if (equalsIgnoreCaseASCII(users[i].getUsername(), u)
                && users[i].getPassword() == p
                && equalsIgnoreCaseASCII(users[i].getEmail(), m)) {
                cout << Color::GREEN << "\nWelcome, " << users[i].getName() << "!" << Color::RESET << endl;
                Cart cart(history); int choice;
                while (true) {
                    cout << Color::BOLD << Color::BLUE << "\n=== USER PANEL ===" << Color::RESET << endl
                        << "1. Show menu\n"
                        << "2. Add dish to cart\n"
                        << "3. Show cart\n"
                        << "4. Remove dish from cart\n"
                        << "5. Modify ingredient quantity\n"
                        << "6. Confirm order\n"
                        << "7. View history\n"
                        << "8. Dish information\n"
                        << "0. Exit\n"
                        << Color::CYAN << "Choice: " << Color::RESET;
                    if (!(cin >> choice)) {
                        cin.clear(); consumeNewline();
                        cout << Color::RED << "Please enter a number!" << Color::RESET << endl;
                        continue;
                    }

                    try {
                        if (choice == 1) admin.showMenu();
                        else if (choice == 2) {
                            string dishName; cout << Color::CYAN << "Dish name: " << Color::RESET;
                            consumeNewline(); getline(cin, dishName);
                            vector<Dish>& menuRef = admin.getMenu(); bool found = false;
                            for (size_t k = 0; k < menuRef.size(); k++)
                                if (equalsIgnoreCaseASCII(menuRef[k].getName(), dishName)) {
                                    cart.addDish(menuRef[k]); found = true; break;
                                }
                            if (!found) cout << Color::RED << "Not found!" << Color::RESET << endl;
                        }
                        else if (choice == 3) cart.showCart();
                        else if (choice == 4) {
                            string name; cout << Color::CYAN << "To remove: " << Color::RESET;
                            consumeNewline(); getline(cin, name);
                            cart.removeDish(name);
                        }
                        else if (choice == 5) {
                            string dish, ingredient; double newQty;
                            cout << Color::CYAN << "Dish name: " << Color::RESET;
                            consumeNewline(); getline(cin, dish);
                            cout << Color::CYAN << "Ingredient: " << Color::RESET;
                            getline(cin, ingredient);

                            
                            newQty = getValidatedDouble(Color::CYAN + "New quantity (kg): " + Color::RESET);

                            cart.modifyIngredient(dish, ingredient, newQty);
                        }
                        else if (choice == 6) cart.confirmOrder(admin, users[i].getUsername());
                        else if (choice == 7) history.showUserHistory(users[i].getUsername());
                        else if (choice == 8) {
                            string name;
                            cout << Color::CYAN << "Dish name: " << Color::RESET;
                            consumeNewline();
                            getline(cin, name);
                            vector<Dish>& menuRef = admin.getMenu();
                            bool found = false;
                            for (size_t i2 = 0; i2 < menuRef.size(); i2++) {
                                if (equalsIgnoreCaseASCII(menuRef[i2].getName(), name)) {
                                    menuRef[i2].show();
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) cout << Color::RED << "No such dish in menu!" << Color::RESET << endl;
                        }
                        else if (choice == 0) break;
                        else cout << Color::RED << "Wrong choice!" << Color::RESET << endl;
                    }
                    catch (string e) { cout << e << endl; }
                }
                return;
            }
        }
        cout << Color::RED << "Login information is incorrect!" << Color::RESET << endl;
    }
};

// ==================== MAIN ====================
int main() {
    UserManager um; um.loadUsersFromFile();
    int choice;
    do {
        cout << Color::BOLD << Color::MAGENTA << "\n=== LOGIN PANEL ===" << Color::RESET << endl
            << "1. Register\n"
            << "2. Login\n"
            << "0. Exit\n"
            << Color::CYAN << "Choice: " << Color::RESET;
        if (!(cin >> choice)) {
            cin.clear(); consumeNewline();
            cout << Color::RED << "Please enter a number!" << Color::RESET << endl;
            continue;
        }
        try {
            if (choice == 1) um.signUp();
            else if (choice == 2) um.signIn();
        }
        catch (string e) { cout << e << endl; }
    } while (choice != 0);
    cout << Color::GREEN << "\nProgram ended." << Color::RESET << endl;
    return 0;
}