#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_LEN 50
#define MAX_PRODUCTS 100
#define MIN_PRODUCTS 1
#define MIN_ID 1
#define MAX_ID 10000
#define MIN_PRICE 0
#define MAX_PRICE 100000
#define MIN_QUANTITY 0
#define MAX_QUANTITY 1000000

const char UPPER_A = 'A';
const char UPPER_Z = 'Z';
const int CASE_DIFF = 'a' - 'A';

typedef struct {
    int id;
    char name[NAME_LEN];
    float price;
    int quantity;
} Product;


void runInventoryManagement();
void inputInitialProducts(Product *products, int numOfProduct);
void inputProduct(Product *products, int index, int totalProducts);
void addProduct(Product **products, int *numOfProduct);
void viewAllProducts(Product *products, int numOfProduct);
void updateQuantity(Product *products, int numOfProduct);
void searchByID(Product *products, int numOfProduct);
void searchByName(Product *products, int numOfProduct);
void searchByPriceRange(Product *products, int numOfProduct);
void deleteProduct(Product **products, int *numOfProduct);
void inventoryMenu(Product **products, int *numOfProduct);
void clearMemory(Product *products);
bool isValidRangeInt(int val, int min, int max);
bool isValidRangeFloat(float val, float min, float max);
bool isDuplicateId(Product *products, int numOfProduct, int id);
bool isDuplicateName(Product *products, int numOfProduct, const char *name);
bool containsSubstring(const char *str, const char *substr);
void removeNewline(char *str);


int main() {
    runInventoryManagement();
    return 0;
}


void runInventoryManagement() {
    int numOfProducts;
    Product *products = NULL;

    while (1) {
        printf("Enter initial number of products: ");
        scanf("%d", &numOfProducts);
        if (!isValidRangeInt(numOfProducts, MIN_PRODUCTS, MAX_PRODUCTS)) {
            printf("Invalid number of products. Must be between %d and %d.\n", MIN_PRODUCTS, MAX_PRODUCTS);
        } else {
            break;
        }
    }

    products = (Product *)calloc(numOfProducts, sizeof(Product));
    if (products == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    inputInitialProducts(products, numOfProducts);
    inventoryMenu(&products, &numOfProducts);
    clearMemory(products);
    printf("Memory released successfully. Exiting program...\n");
}


void inputInitialProducts(Product *products, int numOfProduct) {
    for (int i = 0; i < numOfProduct; i++) {
        inputProduct(products, i, numOfProduct);
    }
}

void inputProduct(Product *products, int index, int totalProducts) {
    printf("\nEnter details for product %d:\n", index + 1);

    do {
        printf("Product ID: ");
        scanf("%d", &products[index].id);

        if (!isValidRangeInt(products[index].id, MIN_ID, MAX_ID)) {
            printf("Invalid product ID. Must be between %d and %d.\n", MIN_ID, MAX_ID);
        } else if (isDuplicateId(products, index, products[index].id)) {
            printf("Duplicate ID! Please enter a unique product ID.\n");
        }
    } while (!isValidRangeInt(products[index].id, MIN_ID, MAX_ID) ||
             isDuplicateId(products, index, products[index].id));

    while (getchar() != '\n');
    printf("Product Name: ");
    fgets(products[index].name, NAME_LEN, stdin);
    removeNewline(products[index].name);

    if (isDuplicateName(products, index, products[index].name)) {
        printf("Duplicate name found! Please enter a unique product name.\n");
        index--;
        return;
    }

    do {
        printf("Product Price: ");
        scanf("%f", &products[index].price);
        if (!isValidRangeFloat(products[index].price, MIN_PRICE, MAX_PRICE)) {
            printf("Invalid product price. Must be between %.2f and %.2f.\n", MIN_PRICE, MAX_PRICE);
        }
    } while (!isValidRangeFloat(products[index].price, MIN_PRICE, MAX_PRICE));

    do {
        printf("Product Quantity: ");
        scanf("%d", &products[index].quantity);
        if (!isValidRangeInt(products[index].quantity, MIN_QUANTITY, MAX_QUANTITY)) {
            printf("Invalid product quantity. Must be between %d and %d.\n", MIN_QUANTITY, MAX_QUANTITY);
        }
    } while (!isValidRangeInt(products[index].quantity, MIN_QUANTITY, MAX_QUANTITY));
}

void inventoryMenu(Product **products, int *numOfProduct) {
    int choice;

    do {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addProduct(products, numOfProduct); break;
            case 2: viewAllProducts(*products, *numOfProduct); break;
            case 3: updateQuantity(*products, *numOfProduct); break;
            case 4: searchByID(*products, *numOfProduct); break;
            case 5: searchByName(*products, *numOfProduct); break;
            case 6: searchByPriceRange(*products, *numOfProduct); break;
            case 7: deleteProduct(products, numOfProduct); break;
            case 8: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 8);
}

bool isDuplicateId(Product *products, int numOfProduct, int id) {
    for (int i = 0; i < numOfProduct; i++) {
        if (products[i].id == id) {
            return true;
        }
    }
    return false;
}

void addProduct(Product **products, int *numOfProduct) {
    if (*numOfProduct >= MAX_PRODUCTS) {
        printf("Cannot add more products. Maximum limit (%d) reached.\n", MAX_PRODUCTS);
        return;
    }

    *products = (Product *)realloc(*products, (*numOfProduct + 1) * sizeof(Product));
    if (*products == NULL) {
        printf("Memory reallocation failed!\n");
        return;
    }

    Product *newProduct = &((*products)[*numOfProduct]);

    printf("Enter new product details:\n");

    do {
        printf("Product ID: ");
        scanf("%d", &newProduct->id);
        if (!isValidRangeInt(newProduct->id, MIN_ID, MAX_ID)) {
            printf("Invalid product ID. Must be between %d and %d.\n", MIN_ID, MAX_ID);
        } else if (isDuplicateId(*products, *numOfProduct, newProduct->id)) {
            printf("Duplicate ID! Please enter a unique product ID.\n");
        }
    } while (!isValidRangeInt(newProduct->id, MIN_ID, MAX_ID) ||
             isDuplicateId(*products, *numOfProduct, newProduct->id));

    while (getchar() != '\n');
    printf("Product Name: ");
    fgets(newProduct->name, NAME_LEN, stdin);
    removeNewline(newProduct->name);

    if (isDuplicateName(*products, *numOfProduct, newProduct->name)) {
        printf("Duplicate name found! Product not added.\n");
        return;
    }

    do {
        printf("Product Price: ");
        scanf("%f", &newProduct->price);
        if (!isValidRangeFloat(newProduct->price, MIN_PRICE, MAX_PRICE)) {
            printf("Invalid product price. Must be between %.2f and %.2f.\n", MIN_PRICE, MAX_PRICE);
        }
    } while (!isValidRangeFloat(newProduct->price, MIN_PRICE, MAX_PRICE));

    do {
        printf("Product Quantity: ");
        scanf("%d", &newProduct->quantity);
        if (!isValidRangeInt(newProduct->quantity, MIN_QUANTITY, MAX_QUANTITY)) {
            printf("Invalid product quantity. Must be between %d and %d.\n", MIN_QUANTITY, MAX_QUANTITY);
        }
    } while (!isValidRangeInt(newProduct->quantity, MIN_QUANTITY, MAX_QUANTITY));

    (*numOfProduct)++;
    printf("Product added successfully!\n");
}

void viewAllProducts(Product *products, int numOfProduct) {
    if (numOfProduct == 0) {
        printf("No products available.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int i = 0; i < numOfProduct; i++) {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               products[i].id, products[i].name, products[i].price, products[i].quantity);
    }
}

void updateQuantity(Product *products, int numOfProduct) {
    int id, newQty, isFound = false;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);

    for (int i = 0; i < numOfProduct; i++) {
        if (products[i].id == id) {
            do {
                printf("Enter new Quantity: ");
                scanf("%d", &newQty);
                if (!isValidRangeInt(newQty, MIN_QUANTITY, MAX_QUANTITY)) {
                    printf("Invalid quantity. Must be between %d and %d.\n", MIN_QUANTITY, MAX_QUANTITY);
                }
            } while (!isValidRangeInt(newQty, MIN_QUANTITY, MAX_QUANTITY));
            products[i].quantity = newQty;
            printf("Quantity updated successfully!\n");
            isFound = true;
            break;
        }
    }

    if (!isFound) {
        printf("Product not found.\n");
    }
}

void searchByID(Product *products, int numOfProduct) {
    int id, isFound = false;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < numOfProduct; i++) {
        if (products[i].id == id) {
            printf("Product Found: ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            isFound = true;
            break;
        }
    }

    if (!isFound) {
        printf("Product not found.\n");
    }
}

void searchByName(Product *products, int numOfProduct) {
    char keyword[NAME_LEN];
    int isFound = false;
    while (getchar() != '\n');
    printf("Enter name to search (partial allowed): ");
    fgets(keyword, NAME_LEN, stdin);
    removeNewline(keyword);

    printf("Products Found:\n");
    for (int i = 0; i < numOfProduct; i++) {
        if (containsSubstring(products[i].name, keyword)) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            isFound = true;
        }
    }
    if (!isFound) {
        printf("No matching products found.\n");
    }
}

void searchByPriceRange(Product *products, int numOfProduct) {
    float minPrice, maxPrice;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);

    printf("Products in price range:\n");
    int isFound = false;
    for (int i = 0; i < numOfProduct; i++) {
        if (products[i].price >= minPrice && products[i].price <= maxPrice) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            isFound = true;
        }
    }
    if (!isFound) {
        printf("No products found in this range.\n");
    }
}

void deleteProduct(Product **products, int *numOfProduct) {
    int id, index = -1;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *numOfProduct; i++) {
        if ((*products)[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Product not found.\n");
        return;
    }

    for (int i = index; i < *numOfProduct - 1; i++) {
        (*products)[i] = (*products)[i + 1];
    }

    (*numOfProduct)--;
    *products = (Product *)realloc(*products, (*numOfProduct) * sizeof(Product));
    printf("Product deleted successfully!\n");
}


void clearMemory(Product *products) {
    free(products);
}

bool isValidRangeInt(int val, int min, int max) {
    return val >= min && val <= max;
}

bool isValidRangeFloat(float val, float min, float max) {
    return val >= min && val <= max;
}

void removeNewline(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
        i++;
    }
}

bool containsSubstring(const char *str, const char *substr) {
    if (!*substr) return true;

    for (int i = 0; str[i] != '\0'; i++) {
        int j = 0;
        while (str[i + j] != '\0' && substr[j] != '\0') {
            char c1 = str[i + j];
            char c2 = substr[j];
            if (c1 >= UPPER_A && c1 <= UPPER_Z) {
                c1 += CASE_DIFF;
            }
            if (c2 >= UPPER_A && c2 <= UPPER_Z) {
                c2 += CASE_DIFF;
            }
            if (c1 != c2) {
                break;
            }
            j++;
        }
        if (substr[j] == '\0') {
            return true;
        }
    }
    return false;
}

bool isDuplicateName(Product *products, int numOfProduct, const char *name) {
    for (int i = 0; i < numOfProduct; i++) {
        int j = 0;
        while (products[i].name[j] != '\0' && name[j] != '\0') {
            char c1 = products[i].name[j];
            char c2 = name[j];
            if (c1 >= UPPER_A && c1 <= UPPER_Z){
                c1 += CASE_DIFF;
            }
            if (c2 >= UPPER_A && c2 <= UPPER_Z) {
                c2 += CASE_DIFF;
            }
            if (c1 != c2) {
                break;
            }
            j++;
        }
        if (products[i].name[j] == '\0' && name[j] == '\0') return true;
    }
    return false;
}
