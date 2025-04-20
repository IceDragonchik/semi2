#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Класс книги
class Book {
public:
    int year;
    string title;
    string author;
    
    Book(int y, string t, string a) {
        year = y;
        title = t;
        author = a;
    }
    
    // Сравнение всех полей
    bool equals(Book other) {
        return year == other.year && title == other.title && author == other.author;
    }
};

// Глобальная функция для хеширования
int calculateHash(Book book, int tableSize) {
    int hash = 0;
    // Складываем все символы названия
    for (char c : book.title) {
        hash += (int)c;
    }
    // Добавляем год издания
    hash += book.year;
    // Делим с остатком на размер таблицы
    return hash % tableSize;
}

template<typename K, typename V>
class HashTable {
private:
    // Структура для хранения данных
    struct TableEntry {
        K key;
        V value;
        bool isFilled;
        bool isDeleted;
        
        TableEntry() {
            isFilled = false;
            isDeleted = false;
        }
    };
    
    vector<TableEntry> entries; // Вектор для хранения
    int tableSize; // Текущий размер
    int count; // Количество элементов
    
public:
    // Конструктор с фиксированным размером
    HashTable() {
        tableSize = 10; // Просто взяли десяточку
        count = 0;
        entries.resize(tableSize);
    }
    
    // Метод добавления
    void add(K key, V value) {
        // Если таблица почти заполнена, увеличиваем размер
        if (count > tableSize * 0.7) {
            resizeTable();
        }
        
        int index = calculateHash(key, tableSize);
        
        // Ищем место для вставки
        while (entries[index].isFilled) {
            // Если ключ уже есть, обновляем значение
            if (entries[index].key.equals(key)) {
                entries[index].value = value;
                return;
            }
            index = (index + 1) % tableSize; // Линейное пробирование
        }
        
        // Вставляем новый элемент
        entries[index].key = key;
        entries[index].value = value;
        entries[index].isFilled = true;
        entries[index].isDeleted = false;
        count++;
    }
    
    // Метод поиска
    V* find(K key) {
        int index = calculateHash(key, tableSize);
        int startIndex = index;
        
        do {
            // Если нашли заполненную и неудаленную запись с нужным ключом
            if (entries[index].isFilled && !entries[index].isDeleted 
                && entries[index].key.equals(key)) {
                return &entries[index].value;
            }
            // Если нашли пустую и неудаленную - значит элемента нет
            if (!entries[index].isFilled && !entries[index].isDeleted) {
                break;
            }
            index = (index + 1) % tableSize;
        } while (index != startIndex);
        
        return nullptr; // Не нашли :(
    }
    
    // Метод удаления
    bool remove(K key) {
        int index = calculateHash(key, tableSize);
        int startIndex = index;
        
        do {
            // Если нашли нужный ключ
            if (entries[index].isFilled && !entries[index].isDeleted 
                && entries[index].key.equals(key)) {
                entries[index].isDeleted = true;
                count--;
                return true;
            }
            // Если дошли до пустой и неудаленной ячейки
            if (!entries[index].isFilled && !entries[index].isDeleted) {
                break;
            }
            index = (index + 1) % tableSize;
        } while (index != startIndex);
        
        return false; // Не нашли :(
    }
    
    // Вывод таблицы
    void print() {
        cout << "Hash Table (size: " << tableSize << ", elements: " << count << "):" << endl;
        for (int i = 0; i < tableSize; i++) {
            cout << "[" << i << "]: ";
            if (entries[i].isFilled && !entries[i].isDeleted) {
                cout << entries[i].key.title << " (" << entries[i].key.year << ")";
            } else if (entries[i].isDeleted) {
                cout << "<deleted>";
            } else {
                cout << "<empty>";
            }
            cout << endl;
        }
    }
    
private:
    // Увеличение размера таблицы
    void resizeTable() {
        vector<TableEntry> oldEntries = entries;
        tableSize *= 2; // Удваиваем размер
        entries.clear();
        entries.resize(tableSize);
        count = 0;
        
        // Перехешируем все элементы
        for (TableEntry entry : oldEntries) {
            if (entry.isFilled && !entry.isDeleted) {
                add(entry.key, entry.value);
            }
        }
    }
};