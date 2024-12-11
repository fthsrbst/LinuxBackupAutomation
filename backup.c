#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

volatile int keepRunning = 1;

void performBackup() {
    char backupPath[256];
    FILE *file = fopen("backupPath.txt", "r");

    if (file == NULL) {
        printf("backupPath.txt dosyası bulunamadı. changeDir.c çalıştırılıyor...\n");
        int result = system("gcc changeDir.c -o changeDir && ./changeDir");
        if (result != 0) {
            printf("changeDir programı çalıştırılamadı veya derleme hatası oluştu.\n");
            return;
        }

        // changeDir çalıştıktan sonra tekrar kontrol et
        file = fopen("backupPath.txt", "r");
        if (file == NULL) {
            printf("backupPath.txt yine bulunamadı. Program sonlanıyor...\n");
            exit(1);
        }
    }

    if (fgets(backupPath, sizeof(backupPath), file) == NULL) {
        perror("Dizin bilgisi okunamadı");
        fclose(file);
        return;
    }
    fclose(file);

    // Yolda ekstra boşluk veya yeni satır varsa temizle
    backupPath[strcspn(backupPath, "\n")] = '\0';
    printf("Dizin yolunun doğruluğunu kontrol edin: %s\n", backupPath);

    // Dizin değişikliği yapmadan önce geri dön
    char currentDir[256];
    getcwd(currentDir, sizeof(currentDir));  // Mevcut dizini al

    // backupPath dizinine git
    if (chdir(backupPath) != 0) {
        perror("Dizine girilemedi");
        printf("Belirtilen dizin yolu: %s\n", backupPath);
        return;
    }

    // Yedekleme dosyasını oluştur
    char tarCommand[512];
    snprintf(tarCommand, sizeof(tarCommand), "tar -cvf ../backup_$(date +%%Y%%m%%d_%%H%%M%%S).tar *");

    printf("[%ld] Sıkıştırma işlemi başlatılıyor...\n", time(NULL));
    int result = system(tarCommand);

    if (result != 0) {
        printf("Sıkıştırma işlemi başarısız oldu.\n");
    } else {
        printf("Sıkıştırma işlemi tamamlandı.\n");
    }

    // Eski yedeği sil
    char deleteCommand[512];
    snprintf(deleteCommand, sizeof(deleteCommand), "rm -f /path/to/destination/folder/backup_*.tar");
    result = system(deleteCommand);
    if (result != 0) {
        printf("Eski yedek silinemedi.\n");
    } else {
        printf("Eski yedek dosyası silindi.\n");
    }

    // Yedekleme dosyasını taşı
    char moveCommand[512];
    snprintf(moveCommand, sizeof(moveCommand), "mv ../backup_$(date +%%Y%%m%%d_%%H%%M%%S).tar /path/to/destination/folder");

    result = system(moveCommand);
    if (result != 0) {
        printf("Dosya taşıma işlemi başarısız oldu.\n");
    } else {
        printf("Yedekleme dosyası taşındı.\n");
    }

    // Mevcut dizine geri dön
    chdir(currentDir);
}

void *backupThread(void *arg) {
    while (keepRunning) {
        performBackup();
        for (int i = 60; i >= 0 && keepRunning; i--) {
            printf("\rBir sonraki yedekleme için %d saniye bekleniyor...", i);
            fflush(stdout);  // Ekranı hemen güncelle
            sleep(1);  // 1 saniye bekle
        }
        printf("\n");
    }
    return NULL;
}

void *inputThread(void *arg) {
    printf("Programdan çıkmak için herhangi bir tuşa basın...\n");
    getchar();
    keepRunning = 0;
    return NULL;
}

int main() {
    pthread_t backupThreadId, inputThreadId;

    if (pthread_create(&backupThreadId, NULL, backupThread, NULL) != 0) {
        perror("Yedekleme thread'i oluşturulamadı");
        return 1;
    }

    if (pthread_create(&inputThreadId, NULL, inputThread, NULL) != 0) {
        perror("Kullanıcı girişi thread'i oluşturulamadı");
        return 1;
    }

    pthread_join(backupThreadId, NULL);
    pthread_join(inputThreadId, NULL);

    printf("Programdan çıkılıyor...\n");
    return 0;
}