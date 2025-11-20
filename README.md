![C++](https://img.shields.io/badge/C++-Pointer-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-yellow)
![Learning](https://img.shields.io/badge/Learning-C++-orange)

# Exercise pointers: book catalogue

Programma con menu testuale per la gestione di un catalogo di libri con puntatori.

## Scopo Didattico
Esercitazione pratica sui concetti fondamentali dei puntatori in C++:
- Gestione memoria dinamica con `new`/`delete`
- Array di puntatori a strutture
- Puntatori a funzioni
- Passaggio parametri per indirizzo

## Tecnologie uttilizzate
- **Linguaggio**: C++
- **Strutture dati**: srtuct Libro, struct Risultato, struct Parametri.
- **Memorizzazione**: array statico di puntatori a entità Libro
- **Persistenza**: file binario per il salvataggio dei dati.
- **Gestione errori**: sistema centralizzato con puntatori a funzioni.

## Sistema di salvataggio
Il file libri.dat contiene:
- Contatore dei libri del catalogo
- Record sequenziali dei libri in formato binario

Il salvataggio viene effettuato:
- Manualmente - con la selezione della voce "Salva" del menù.
- All'uscita del programma con scelta dell'utente.

## Sistema di centralizzato di gestione degli errori
Ogni funzione del programma viene passata alla funzione gestore_errori() tramite puntatore,
viene poi chiamata e ne viene gestito il ritorno.

## Funzionalità:
Il programma è provvisto di un menu testuale che gestisce le operazioni che si possono eseguire
sul catalogo:

Menù Principale

- **1** - Nuovo libro (inserimento record)
- **2** - Cancella libro (rimozione record)
- **3** - Modifica libro (aggiornamento dati)
- **4** - Visualizza libro singolo
- **5** - Visualizza catalogo completo
- **6** - Pulizia catalogo (ottimizzazione memoria)
- **7** - Salva dati
- **9** - Esci dal programma

Sottomenù Modifica libro

- **1** - modifica titolo
- **2** - modifica autore
- **3** - modifica anno
- **4** - modifica prezzo
- **9** - torna al menu principale

## Come usare il programma
1. Scarica la repository.
2. Apri `main.cpp` in Code::Blocks
3. Compila con `Build → Build and Run`

## Struttura file
```
biblioteca/
├── main.cpp              	# Codice sorgente principale
├── libri.dat			  	# Database libri (auto-generato)
└── README.md				# Questo file
```

## Licenza
Questo progetto è rilasciato sotto licenza [MIT](https://opensource.org/licenses/MIT).

## Note:
All'avvio il programma crea automaticamente il file di database se non presente e carica i dati nel catalogo.
