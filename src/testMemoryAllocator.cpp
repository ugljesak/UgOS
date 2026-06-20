// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include "../h/MemoryAllocator.hpp"
//
// // Ponovo definisemo lažni RAM za potrebe lokalnog testa
// static char fake_ram[50 * 1024 * 1024]; // 50 MB prostora za mučenje alokatora
// const void* HEAP_START_ADDR = (const void*)fake_ram;
// const void* HEAP_END_ADDR = (const void*)((char*)fake_ram + sizeof(fake_ram));
//
// // Funkcija koja ispisuje trenutni broj slobodnih blokova (za debagovanje)
// // Možeš je zakomentarisati ako nemaš pristup freeMemHead-u direktno iz testa
// void print_free_list_stats() {
//     // Ako želiš da vidiš koliko parčića imaš u memoriji, možeš privremeno staviti
//     // freeMemHead kao public u klasu, ili dodati neku metodu za ispis.
// }
//
// int main() {
//     std::cout << "========= KREĆE PAKLENI TEST ZA UgOS ALOKATOR =========" << std::endl;
//
//     // Inicijalizacija i postavljanje seed-a za nasumične vrednosti
//     //MemoryAllocator::init();
//     std::srand(42); // Fiksiran seed da test uvek bude ponovljiv
//
//     const int FAZA_1_ITERACIJA = 1000;
//     const int MAX_ALOKACIJA = 500;
//     void* adrese[MAX_ALOKACIJA] = {nullptr};
//     size_t velicine[MAX_ALOKACIJA] = {0};
//
//     std::cout << "[STAZA 1] Masovna nasumična alokacija i oslobađanje..." << std::endl;
//
//     for (int i = 0; i < FAZA_1_ITERACIJA; ++i) {
//         int indeks = std::rand() % MAX_ALOKACIJA;
//
//         if (adrese[indeks] == nullptr) {
//             // Blok je prazan, hajdemo da alociramo nasumičnu veličinu (od 1 do 5000 bajtova)
//             size_t trazeni_bajtovi = (std::rand() % 5000) + 1;
//             adrese[indeks] = MemoryAllocator::mem_alloc(trazeni_bajtovi);
//             velicine[indeks] = trazeni_bajtovi;
//
//             if (adrese[indeks] == nullptr) {
//                 // Ako vrati nullptr, proveravamo da li smo stvarno ostali bez memorije
//                 // (Za 50MB ne bi trebalo da ostanemo osim ako curi memorija)
//                 std::cout << "⚠️ Alokacija neuspešna na iteraciji: " << i << " za veličinu: " << trazeni_bajtovi << std::endl;
//             }
//         } else {
//             // Blok je zauzet, oslobađamo ga i proveravamo da li radi stabilno
//             if (MemoryAllocator::mem_free(adrese[indeks]) != 0) {
//                 std::cout << "❌ KATASTROFA: mem_free vratio grešku za važeću adresu! Iteracija: " << i << std::endl;
//                 return -1;
//             }
//             adrese[indeks] = nullptr;
//             velicine[indeks] = 0;
//         }
//     }
//
//     std::cout << "✅ Staza 1 završena bez rušenja sistema." << std::endl;
//
//     std::cout << "[STAZA 2] Čišćenje preostale memorije..." << std::endl;
//     for (int i = 0; i < MAX_ALOKACIJA; ++i) {
//         if (adrese[i] != nullptr) {
//             if (MemoryAllocator::mem_free(adrese[i]) != 0) {
//                 std::cout << "❌ Greška pri finalnom čišćenju na indeksu: " << i << std::endl;
//                 return -1;
//             }
//             adrese[i] = nullptr;
//         }
//     }
//     std::cout << "✅ Staza 2 završena. Sva memorija oslobođena." << std::endl;
//
//     std::cout << "[STAZA 3] ULTIMATIVNI TEST - Provera Defragmentacije (Coalescing)..." << std::endl;
//     // Ako je tvoj tryCoalesce odradio posao 100% tačno, celokupan RAM je sada ponovo
//     // jedan jedini veliki prazan blok. To znači da sada možemo odjednom alocirati
//     // skoro celu memoriju u jednom komadu!
//
//     size_t skoro_ceo_heap = 45 * 1024 * 1024; // 45 MB u jednom bloku
//     void* džinovski_blok = MemoryAllocator::mem_alloc(skoro_ceo_heap);
//
//     if (džinovski_blok == nullptr) {
//         std::cout << "❌ TEST PAO: Memorija je fragmentisana! tryCoalesce nije spojio blokove kako treba." << std::endl;
//         std::cout << "Iako je sve oslobođeno, alokator ne može da nađe kontinualan prostor od 45MB!" << std::endl;
//         return -1;
//     } else {
//         std::cout << "🚀🚀🚀 BRAVO MAJSTORE! Ultimativni test PROŠAO!" << std::endl;
//         std::cout << "Alokator je uspešno spojio sve sitne fragmente nazad u jedno veliko jezgro!" << std::endl;
//         MemoryAllocator::mem_free(džinovski_blok);
//     }
//
//     std::cout << "=======================================================" << std::endl;
//     return 0;
// }

#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"

void memoryAllocatorTest() {
    __putc('S');
    void* a=MemoryAllocator::mem_alloc(64);
    if(a) {
        __putc('1');
    }
    void* b=MemoryAllocator::mem_alloc(64);
    if(b) {
        __putc('2');
    }
    MemoryAllocator::mem_free(a);
    __putc('F');
    void* p=MemoryAllocator::mem_alloc(64);
    if (p==a) {
        __putc('M');
    }
    __putc('E');
    MemoryAllocator::mem_free(b);
    MemoryAllocator::mem_free(p);
}