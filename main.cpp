/*
 * Programma: Catalogo libri
 * Autore: Gabriele Henriet
 * Data: Novembre 2025
 * Scopo: Utilizzo puntatori e gestione dei dati di un catalogo libri
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <iomanip>

#define LUNG_TITOLO 50
#define LUNG_AUTORE 40
#define MAX_CATALOGO 10000

struct Libro{
    char titolo[LUNG_TITOLO];
    char autore[LUNG_AUTORE];
    int anno;
    float prezzo;
};

struct Risultato{
    int codice;                 // codice errore
    char messaggio[100];        // messaggio errore
    int indice;                 // indice catalogo
};

struct Parametri{
    int indice = 0;             // indice catalogo
};

// GESTORE ERRORI CENTRALIZZATO: gestisce i risultati di tutte le funzioni che chiama.
// Ritorna:
// Risultato
Risultato gestore_errori(Risultato (*chiama)(Libro* cat[], int* n, Parametri p), Libro* cat[], int* n, Parametri p);

// CRUD: inserisce un nuovo libro nel catalogo.
// Ritorna:
// Risultato
Risultato nuovo_libro(Libro* cat[], int* n, Parametri p);

// CRUD: rimuove un libro dal catalogo.
// Ritorna:
// Risultato
Risultato rimuovi_libro(Libro* cat[], int* n, Parametri p);

// CRUD: modifica i dati di un libro del catalogo.
// Ritorna:
// Risultato
Risultato modifica_libro(Libro* cat[], int* n, Parametri p);

// CRUD: Cerca libro tramite titolo.
// Ritorna:
// Risultato
Risultato cerca_libro(Libro* cat[], int* n, Parametri p);

// INTERFACCIA: visualizza menu e gestisce flusso.
// Risultato
Risultato menu_principale(Libro* cat[], int* n, Parametri p);

// INTERFACCIA: visualizza l'header della tabella libri.
// Ritorna:
// void
Risultato stampa_header(Libro* cat[], int* n, Parametri p);

// INTERFACCIA: visualizza i dati di un solo libro intabellati.
// Ritorna:
// Risultato
Risultato stampa_libro(Libro* cat[], int* n, Parametri p);

// INTERFACCIA: visualizza i dati di tutto il catalogo intabellati.
// Ritorna:
// void
Risultato stampa_catalogo(Libro* cat[], int* n, Parametri p);

// UTILITY: rimuove tutti i libri cancellati, liberando i posti.
// Ritorna:
// Risultato
Risultato pulizia_catalogo(Libro* cat[], int* n, Parametri p);

// UTILITY: crea il risultato della funzione.
// Ritorna:
// Risultato
Risultato crea_risultato(int cod, const char messaggio[], int ind);

// PERSISTENZA: salvataggio su file binario.
// Ritorna:
// Risultato
Risultato salva_dati(Libro* cat[], int* n, Parametri p);

// PERSISTENZA: carica i dati da file binario.
// Ritorna:
// Risultato
Risultato carica_dati(Libro* cat[], int* n, Parametri p);

// Inizializzazione catalogo e contatore di posizione.
// Caricamento dati del catalogo da file.
// Gestisce l'elaborazione con un ciclo infinito.
// Salvataggio dati in fase di uscita.
int main(){
    Parametri extra;
    Libro* catalogo[MAX_CATALOGO] = {nullptr};
    Risultato result;
    int* count = new int(0);
    char scelta;

    gestore_errori(carica_dati, catalogo, count, extra);

    while(true){
        result = gestore_errori(menu_principale, catalogo, count, extra);

        if(result.codice == 4)
            return 1;
        if(result.codice == - 1)
            break;
    }

    std::cout << "\tSalvare i dati (s/n): ";
    std::cin >> scelta;

    if(scelta == 's'){
        result = gestore_errori(salva_dati, catalogo, count, extra);
    }

    delete count;
    for(int i = 0; i < MAX_CATALOGO; i++){
        if(catalogo[i] != nullptr){
            delete catalogo[i];
            catalogo[i] = nullptr;
        }
    }

    return 0;
}

// Sistema centralizzato di gestione degli errori.
// Parametri:
// Puntatore alle altre funzioni.
// Il catalogo dei libri.
// Il primo posto libero nel catalogo.
// La struttura dei parametri extra.
// In base al puntatore con cui viene chiamata, chiama a sua volta la
// la funzione corrispondente e ne gestisce la struttura di ritorno.
Risultato gestore_errori(Risultato (*chiama)(Libro* cat[], int* n, Parametri p), Libro* cat[], int* n, Parametri p){

    // costanti di stato locali - gestione degli errori.
    const int SUCCESSO = 0;                 // successo.
    const int INFO = 1;                     // successo + stampa messaggio.
    const int WARNING = 2;                  // avviso ma continua programma.
    const int ERRORE = 3;                   // gestione errore della funzione chiamata.
    const int ERRORE_GRAVE = 4;             // errore del programma, uscita automatica.
    const int USCITA = -1;                  // uscita da programma e sotto menu.

    Risultato result = chiama(cat, n, p);

    switch(result.codice){
        case SUCCESSO:
            break;

        case INFO:
            std::cout << std::endl;
            if(result.indice < 0)
                std::cout << "\tInformazione " << result.codice << ": " << result.messaggio;
            else
                std::cout << "\tInformazione " << result.codice << ": " << result.messaggio << " " << result.indice;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "\t";
            system("PAUSE");
            break;

        case WARNING:
            std::cout << std::endl;
            std::cout << "\tAttenzione " << result.codice << ": " << result.messaggio;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "\t";
            system("PAUSE");
            break;

        case ERRORE:
            std::cout << std::endl;
            std::cout << "\tCodice errore " << result.codice << ": " << result.messaggio;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "\t";
            system("PAUSE");
            break;

        case ERRORE_GRAVE:
            std::cout << std::endl;
            std::cout << "\tCodice errore " << result.codice << ": " << result.messaggio;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "\t";
            system("PAUSE");
            result = crea_risultato(4, "", -1);
            break;

        case USCITA:
            std::cout << std::endl;
            std::cout << result.messaggio;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "\t";
            system("PAUSE");
            result = crea_risultato(-1, "", -1);
    }
    return result;
}

// Interfaccia principale del programma, gestisce anche il
// flusso di elaborazione.
// Ritorna struttura:
// codice 0: SUCCESSO
// codice -1: USCITA
Risultato menu_principale(Libro* cat[], int* n, Parametri p){
    int scelta;
    Risultato result = {0, "", -1};

    system("CLS");

    std::cout << std::endl;
    std::cout << "\tMenu principale" << std::endl;
    std::cout << std::endl;
    std::cout << "\tNuovo libro..............................1" << std::endl;
    std::cout << "\tCancella libro...........................2" << std::endl;
    std::cout << "\tModifica libro...........................3" << std::endl;
    std::cout << "\tVisualizza libro.........................4" << std::endl;
    std::cout << "\tVisualizza tutto il catalogo.............5" << std::endl;
    std::cout << "\tPulizia catalogo.........................6" << std::endl;
    std::cout << "\tSalva dati...............................7" << std::endl;
    std::cout << "\tEsci dal programma.......................9" << std::endl;
    std::cout << std::endl;
    std::cout << "\tInserisci la tua scelta: ";
    std::cin >> scelta;
    std::cout << std::endl;
    std::cin.ignore();

    switch(scelta){
        case 1:
            result = gestore_errori(nuovo_libro, cat, n, p);
            break;
        case 2:
            result = gestore_errori(rimuovi_libro, cat, n, p);
            break;
        case 3:
            result = gestore_errori(modifica_libro, cat, n, p);
            break;
        case 4:
            result = gestore_errori(stampa_libro, cat, n, p);
            break;
        case 5:
            result = gestore_errori(stampa_catalogo, cat, n, p);
            break;
        case 6:
            result = gestore_errori(pulizia_catalogo, cat, n, p);
            break;
        case 7:
            result = gestore_errori(salva_dati, cat, n, p);
            break;
        case 9:
            result = crea_risultato(-1, "\tBye Bye", -1);
            break;
    }
    return result;
}

// Inserisce un nuovo libro nel catalogo nella prima
// posizione libera e aggiorna il contatore delle posizioni.
// Richiede input dei dati.
// Ritorna struttura:
// codice 1: INFO
// codice 2: WARNING
Risultato nuovo_libro(Libro* cat[], int* n, Parametri p){
    Risultato result = {1, "nuovo libro inserito", -1};

    if(*n < MAX_CATALOGO){
        cat[*n] = new Libro;
        std::cout << "\tTitolo: ";
        std::cin.getline(cat[*n] -> titolo, LUNG_TITOLO);

        std::cout << "\tAutore: ";
        std::cin.getline(cat[*n] -> autore, LUNG_AUTORE);

        std::cout << "\tAnno publicazione: ";
        std::cin >> cat[*n] -> anno;

        std::cout << "\tPrezzo: ";
        std::cin >> cat[*n] -> prezzo;

        std::cin.ignore();

        (*n)++;
    }else{
        return crea_risultato(2, "catalogo pieno, fare pulizia", -1);
    }
    return result;
}

// Cerca il libro per titolo e resetta i suoi campi.
// Ritorna struttura:
// codice 1: INFO
// codice 3: ERRORE
Risultato rimuovi_libro(Libro* cat[], int* n, Parametri p){
    Risultato result;
    p.indice = 0;

    result = gestore_errori(cerca_libro, cat, n, p);

    if(result.codice == 0 || result.codice == 1){           // resetto i campi.
        strcpy(cat[result.indice] -> titolo, "");
        strcpy(cat[result.indice] -> autore, "");
        cat[result.indice] -> anno = 0;
        cat[result.indice] -> prezzo = 0;
        result = crea_risultato(1, "libro rimosso", -1);    // ritorno 1 se libro trovato
    }else{
        result = crea_risultato(3, "modifica fallita", -1); // ritorno 3 se libro non trovato.
    }
    return result;
}

// Cerca libro per titolo, visualizza sotto menu modifica,
// gestisce flusso di modifica dei dati del libro.
// Ritorna struttura:
// codice 1: INFO
// codice 3: ERRORE
Risultato modifica_libro(Libro* cat[], int* n, Parametri p){
    Risultato result;
    int ind;
    int scelta;

    result = gestore_errori(cerca_libro, cat, n, p);

    if(result.codice == 0 || result.codice == 1){       // se libro trovato procedo con modifica
        ind = result.indice;

        while(true){
            system("CLS");

            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "\tMenu modifica - Libro: " << cat[ind] -> titolo;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "\tTitolo...................................1" << std::endl;
            std::cout << std::endl;
            std::cout << "\tAutore...................................2" << std::endl;
            std::cout << std::endl;
            std::cout << "\tAnno publicazione........................3" << std::endl;
            std::cout << std::endl;
            std::cout << "\tPrezzo...................................4" << std::endl;
            std::cout << std::endl;
            std::cout << "\tEsci.....................................9" << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "\tInserisci la tua scelta: ";
            std::cin >> scelta;
            std::cin.ignore();
            std::cout << std::endl;

            switch(scelta){
                case 1:
                    std::cout << "\tInserisci nuovo titolo: ";
                    std::cin.getline(cat[ind] -> titolo, LUNG_TITOLO);
                    break;
                case 2:
                    std::cout << "\tInserisci nuovo autore: ";
                    std::cin.getline(cat[ind] -> autore, LUNG_AUTORE);
                    break;
                case 3:
                    std::cout << "\tInserisci nuovo anno di publicazione: ";
                    std::cin >> cat[ind] -> anno;
                    std::cin.ignore();
                    break;
                case 4:
                    std::cout << "\tInserisci nuovo prezzo: ";
                    std::cin >> cat[ind] -> prezzo;
                    std::cin.ignore();
                    break;
                case 9:
                    result = crea_risultato(1, "modifica effettuata", -1);
                    return result;
            }
        }
    }else{
        result = crea_risultato(3, "modifica fallita", -1);
    }
    return result;
}

// Richiede titolo del libro in input.
// Effettua ricerca case insensitive nel catalogo.
// Ritorna:
// codice 1: INFO + indice libro se trovato.
// codice 2: ERRORE se non trovato o input non valido
Risultato cerca_libro(Libro* cat[], int* n, Parametri p){
    Risultato result = {2, "libro non trovato", -1};
    char* titolo_cercato = new char [LUNG_TITOLO];          // Titolo cercato dall'utente.
    char* titolo_temp = new char [LUNG_TITOLO];             // Buffer per titolo memorizzato.

    std::cout << "\tInserisci il titolo del libro: ";       // Input utente.
    std::cin.getline(titolo_cercato, LUNG_TITOLO);
    if(titolo_cercato[0] == '\0') {
        return crea_risultato(2, "titolo non valido", -1);
    }

    for(int j = 0; *(titolo_cercato + j) != '\0'; j++){     // Trasformo la stringa in minuscolo.
        *(titolo_cercato + j) = tolower(*(titolo_cercato + j));
    }

    for(int i = 0; i < *n; i++){
        strcpy(titolo_temp, cat[i] -> titolo);
        for(int j = 0; *(titolo_temp + j) != '\0'; j++){    // Trasformo la stringa in minuscolo.
            *(titolo_temp + j) = tolower(*(titolo_temp + j));
        }

        if(strcmp(titolo_temp, titolo_cercato) == 0){       // Confronto le stringhe.
            p.indice = i;
            gestore_errori(stampa_libro, cat, n, p);
            result = crea_risultato(1, "libro trovato in posizione", i);
            break;
        }
    }

    delete [] titolo_temp;                                  // Libero la memoria
    delete [] titolo_cercato;

    return result;
}

// Scorre tutto il catalogo con una ricerca sequenziale.
// Guando trova un libro resettato effettua uno shift a sinistra.
// Aggiorna contatore di posizione posto libero.
// Ritorna:
// codice 1: INFO
Risultato pulizia_catalogo(Libro* cat[], int* n, Parametri p){
    Risultato result = {0, "", -1};
    for(int i = 0; i < *n; i++){
        if(cat[i] -> titolo[0] == '\0'){            // se libro resettato shifto a sinistra
            delete cat[i];
            for(int j = i; j < *n-1; j++){
                cat[j] = cat[j+1];
            }
            cat[*n - 1] = nullptr;
            (*n)--;
            i--;
        }
    }
    result = crea_risultato(1, "catalogo pulito", -1);
    return result;
}

// Inserisce i dati nella struttura Risultato.
// Ritorna struttura con campi aggiornati.
Risultato crea_risultato(int cod, const char* mes, int ind = 0){
    Risultato ris;
    ris.codice = cod;
    strcpy(ris.messaggio, mes);
    ris.indice = ind;
    return ris;
}

// Stampa a video l'header della tabella per visualizzazione dei dati.
// Ritorna:
// codice 0: SUCCESSO
Risultato stampa_header(Libro* cat[], int* n, Parametri p){
    Risultato result = {0, "", -1};

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "\tCatalogo libri";
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << std::left;
    std::cout << "\t";
    std::cout << std::setw(40);
    std::cout << "Titolo";
    std::cout << std::setw(20);
    std::cout << "Autore";
    std::cout << std::setw(30);
    std::cout << "Anno publicazione";
    std::cout << std::setw(10);
    std::cout << "Prezzo";
    std::cout << std::endl;

    return result;
}

// Stampa a video i dati formattati in forma tabellare di un solo libro.
// Ritorna:
// codice 0: SUCCESSO
Risultato stampa_libro(Libro* cat[], int* n, Parametri p){
    Risultato result = {0, "", -1};

    std::cout << std::left;
    std::cout << "\t";
    std::cout << std::setw(40);
    std::cout << cat[p.indice] -> titolo;
    std::cout << std::setw(20);
    std::cout << cat[p.indice] -> autore;
    std::cout << std::setw(30);
    std::cout << cat[p.indice] -> anno;
    std::cout << std::setw(10);
    std::cout << cat[p.indice] -> prezzo;
    std::cout << std::endl;

    return result;
}

// Effettua una scansione sequenziale del catalogo e stampa tutti
// i dati di tutti i libri.
// Ritorna:
// codice 1: INFO
Risultato stampa_catalogo(Libro* cat[], int* n, Parametri p){

    Risultato result = {1, "catalogo stampato", -1};
    gestore_errori(stampa_header, cat, n, p);

    std::cout << std::endl;
    std::cout << std::endl;

    for(p.indice = 0; p.indice < *n; p.indice++){
        gestore_errori(stampa_libro, cat, n, p);
    }
    return result;
}

// Salva tutti i dati del catalogo e il contatore di posizione in binario,
// sovrascrivendo il file precedente.
// Ritorna:
// codice 1: INFO
Risultato salva_dati(Libro* cat[], int* n, Parametri p){
    Risultato result = {1, "dati salvati", -1};
    FILE* p_file;

    fopen_s(&p_file, "libri.dat", "wb");

    fwrite(n, sizeof(int), 1, p_file);

    for(int i = 0; i < *n; i++){
        fwrite(cat[i], sizeof(Libro), 1, p_file);
    }
    fclose(p_file);

    return result;
}

// Crea file di salvataggio se non esiste.
// Carica il contatore di posizione in memoria.
// Carica i dati nel catatologo con una scansione sequenziale.
// Ritorna:
// codice 0: SUCCESSO
// codice 1: INFO file creato
// codice 3: ERRORE nella lettura del contatore
// codice 3: ERRORE file corrotto
// codice 4: ERRORE_GRAVE non riesce a creare il file
Risultato carica_dati(Libro* cat[], int* n, Parametri p){
    FILE* p_file;
    Libro buffer;
    Risultato result = {0, "caricamento dati eseguito", -1};

    if(fopen_s(&p_file, "libri.dat", "rb") != 0){           // il file non esiste, lo creo.
        if(fopen_s(&p_file, "libri.dat", "wb") == 0){
            *n = 0;
            fwrite(n, sizeof(int), 1, p_file);
            fclose(p_file);
            return crea_risultato(1, "file creato", -1);
        }else{
            return crea_risultato(4, "creazione file");
        }

    }else{                                                  // il file esiste.
        if(fread(n, sizeof(int), 1, p_file) != 1){          // memorizzo contatore di libri.
            return crea_risultato(3, "lettura contatore");
        }

        for(int i = 0; i < *n; i++){                        // memorizzo libri.
            cat[i] = new Libro;
            if(fread(cat[i], sizeof(Libro), 1, p_file) != 1){
                // se non legge dati e non sono alla fine del file, file corrotto.
                if(!feof(p_file)){
                    for(int j = 0; j <= i; j++){
                        delete cat[j];                      // dealloco memoria catalogo.
                        cat[j] = nullptr;                   // puntatori a nullptr.
                    }

                    *n = 0;                                 // resetto contatore.
                    fclose(p_file);                         // chiudo file.
                    return crea_risultato(3, "lettura libri, file corrotto");
                }
            }
        }
        fclose(p_file);
    }

    return result;
}
