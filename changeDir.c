#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Bu satırı ekleyin
#include <unistd.h>
#include <time.h>

int main() {
    char path[200];
    FILE *file;

    const char *backupPath = "backupPath.txt";

    FILE *filevalid = fopen(backupPath, "r");

    if (filevalid) {
        char choice;
        printf("'%s' mevcut.\n", backupPath);
        printf("Dizini değiştirmek istiyor musunuz? (y/n): ");
        scanf(" %c", &choice); // Boşluk ile scanf tamponu temizler
        getchar(); // '\n' karakterini temizler

        if (choice == 'y') {
            printf("Lütfen yedeklemek istediğiniz dizini giriniz: ");
            fgets(path, sizeof(path), stdin);
            path[strcspn(path, "\n")] = '\0'; // '\n' karakterini kaldır
        }

        fclose(filevalid);
    } else {
        printf("'%s' mevcut değil.\n", backupPath);
        printf("Lütfen yedeklemek istediğiniz dizini giriniz: ");
        fgets(path, sizeof(path), stdin);
        path[strcspn(path, "\n")] = '\0'; // '\n' karakterini kaldır
        file = fopen("backupPath.txt", "w");
        if (file == NULL) {
            printf("Dosya oluşturulamadı.\n");
            return 1;
        }
        fprintf(file, "%s", path);
        fclose(file);
        printf("Dizin başarıyla kaydedildi.\n");
    }

    return 0;
}