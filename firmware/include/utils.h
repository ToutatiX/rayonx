/* ***********************************************************************
 * Some usefull functions which are used in this programm
*/

/**
 * @brief Shift all elements of an array to the right and insert the element
 * The function does not return the array and simply modify it
*/
void insertLeftAndPush(int arr[], int n, int x)
{
    for (int i = n - 1; i > 0; i--) {
        arr[i] = arr[i - 1];
    }
    arr[0] = x;
}

/**
 * @brief Check if 2 arrays  are equals
 * @return Return true or false
*/
template <typename T, int m, int n>
bool isEqual(T (&first)[m], T (&second)[n]) {
    if (m != n) {
        return false;
    }

    for (int i = 0; i < m; i++){
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;
}