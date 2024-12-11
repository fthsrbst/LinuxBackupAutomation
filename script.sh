#!/bin/bash

while true; do
clear
echo "--------------------------------------------------------"
printf "Merhaba Otomatik Yedekleme Programına Hoşgeldiniz!\n"
echo "--------------------------------------------------------"

    echo "---------------------"
    echo "|  Menü Seçenekleri |"
    echo "---------------------"
    echo "1. Yedeklemeyi başlat"
    echo "2. Dizini değiştir"
    echo "3. Yedekleme dizinini göster"
    echo "4. Çıkış"
    echo
    read -p "Bir seçenek giriniz [1-4]: " choice

    case $choice in
        1)
            echo "Dosya yedekleme işlemi başlatılıyor..."
            gcc backup.c -o backup
            if [ $? -eq 0 ]; then
                ./backup
            else
                echo "Derleme sırasında hata oluştu."
            fi
            
            ;;
        2)
            echo "Dizin değiştiriliyor..."
            gcc changeDir.c -o changeDir
            if [ $? -eq 0 ]; then
                ./changeDir
            else
                echo "Derleme sırasında hata oluştu."
            fi
            ;;
        3)
            ls -l
            ;;
        4)
            echo "Programdan çıkılıyor..."
            break
            ;;
        *)
            echo "Geçersiz seçim. Lütfen 1-4 arasında bir değer giriniz."
            ;;
    esac
    echo
done