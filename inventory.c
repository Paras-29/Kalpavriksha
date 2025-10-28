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

typedef struct {
    int id;
    char name[NAME_LEN];
    float price;
    int quantity;
} Product;

void runInventoryManagement();
void inputInitialProducts(Product *products ,int numOfProduct);
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


int main() {

    runInventoryManagement();

    return 0;
}


void runInventoryManagement(){

    int numOfProducts;
    Product *products = NULL;

    do{
        printf("Enter initial number of products: ");
        scanf("%d", &numOfProducts);

        if(!isValidRangeInt(numOfProducts, MIN_PRODUCTS, MAX_PRODUCTS)) {
            printf("Invalid number of products. Must be between %d and %d.\n", MIN_PRODUCTS, MAX_PRODUCTS);
        }

    }while((!isValidRangeInt(numOfProducts, MIN_PRODUCTS, MAX_PRODUCTS)));


    products = (Product *)calloc(numOfProducts, sizeof(Product));

    if (products == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    inputInitialProducts(products ,numOfProducts);

    inventoryMenu(&products, &numOfProducts);

    clearMemory(products);

    printf("Memory released successfully. Exiting program...\n");
}


void inputInitialProducts(Product *products , int numOfProduct){

       for (int i = 0; i < numOfProduct; i++) {
            printf("\nEnter details for product %d:\n", i + 1);

        do {
            printf("Product ID: ");
            scanf("%d", &products[i].id);


            if((!isValidRangeInt(products[i].id, MIN_ID, MAX_ID))){
                printf("Invalid product id. Must be between %d and %d.\n", MIN_ID, MAX_ID);
            }
            else if (isDuplicateId(products, i, products[i].id)) {
            printf("Duplicate ID! Please enter a unique product ID.\n");
            }

        } while (!isValidRangeInt(products[i].id, MIN_ID, MAX_ID)||
         isDuplicateId(products, i, products[i].id));

        while (getchar() != '\n');
        printf("Product Name: ");
        fgets(products[i].name, NAME_LEN, stdin);
        products[i].name[strcspn(products[i].name, "\n")] = '\0';


        do {
            printf("Product Price: ");
            scanf("%f", &products[i].price);

            if((!isValidRangeFloat(products[i].price, MIN_PRICE, MAX_PRICE))){
                printf("Invalid product price. Must be between %.2f and %.2f.\n", MIN_PRICE, MAX_PRICE);
            }

        } while (!isValidRangeFloat(products[i].price, MIN_PRICE, MAX_PRICE));


        do {
            printf("Product Quantity: ");
            scanf("%d", &products[i].quantity);

            if((!isValidRangeInt(products[i].quantity, MIN_QUANTITY, MAX_QUANTITY))){
                printf("Invalid product quantity. Must be between %d and %d.\n", MIN_QUANTITY, MAX_QUANTITY);
            }

        } while (!isValidRangeInt(products[i].quantity, MIN_QUANTITY, MAX_QUANTITY));
    }
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
            case 1: 
                addProduct(products, numOfProduct); 
                break;
            case 2: 
                viewAllProducts(*products, *numOfProduct); 
                break;
            case 3: 
                updateQuantity(*products, *numOfProduct); 
                break;
            case 4: 
                searchByID(*products, *numOfProduct); 
                break;
            case 5:     
                searchByName(*products, *numOfProduct); 
                break;
            case 6: 
                searchByPriceRange(*products, *numOfProduct); 
                break;
            case 7: 
                deleteProduct(products, numOfProduct); 
                break;
            case 8: 
                break;
            default: 
                printf("Invalid choice. Try again.\n");
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
        printf("Cannot add more products. Maximum limit reached.\n");
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

        if((!isValidRangeInt(newProduct->id, MIN_ID, MAX_ID)) ){
                printf("Invalid product id. Must be between %d and %d.\n", MIN_ID, MAX_ID);
        }
        else if (isDuplicateId(*products, *numOfProduct, newProduct->id)) {
        printf("Duplicate ID! Please enter a unique product ID.\n");
        }
            
    } while (!isValidRangeInt(newProduct->id, MIN_ID, MAX_ID) ||
         isDuplicateId(*products, *numOfProduct, newProduct->id));

    while (getchar() != '\n');     
    printf("Product Name: ");
    fgets(newProduct->name, NAME_LEN, stdin);
    newProduct->name[strcspn(newProduct->name, "\n")] = '\0';

    do {
        printf("Product Price: ");
        scanf("%f", &newProduct->price);

        if((!isValidRangeFloat(newProduct->price, MIN_PRICE, MAX_PRICE))){
                printf("Invalid product price. Must be between %.2f and %.2f.\n", MIN_PRICE, MAX_PRICE);
        }

    } while (!isValidRangeFloat(newProduct->price, MIN_PRICE, MAX_PRICE));

    do {
        printf("Product Quantity: ");
        scanf("%d", &newProduct->quantity);

        if((!isValidRangeInt(newProduct->quantity, MIN_QUANTITY, MAX_QUANTITY))){
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

                if((!isValidRangeInt(newQty, MIN_QUANTITY, MAX_QUANTITY))){
                printf("Invalid product quantity. Must be between %d and %d.\n", MIN_QUANTITY, MAX_QUANTITY);
            }

            } while (!isValidRangeInt(newQty, MIN_QUANTITY, MAX_QUANTITY));

            products[i].quantity = newQty;
            printf("Quantity updated successfully!\n");
            isFound= true;
            break;
        }
    }

    if (!isFound){
        printf("Product not found.\n");
    }
}

void searchByID(Product *products, int numOfProduct) {
    int id, isFound = false;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < numOfProduct; i++) {
        if (products[i].id == id) {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            isFound= true;
            break;
        }
    }

    if (!isFound){
        printf("Product not found.\n");
    }
}

void searchByName(Product *products, int numOfProduct) {
    char keyword[NAME_LEN];
    int isFound = false;

    while (getchar() != '\n');
    printf("Enter name to search (partial allowed): ");
    fgets(keyword, NAME_LEN, stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("Products Found:\n");
    for (int i = 0; i < numOfProduct; i++) {
        if (strstr(products[i].name, keyword) != NULL) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            isFound = true;
        }
    }

    if (!isFound){
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

    if (!isFound){
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