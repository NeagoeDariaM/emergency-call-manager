# emergency-call-manager

Pentru implementare am folosit urmatoarele structuri de date:

    - *unit* stocheaza id-ul echipajului de interventie, tipul si disponibilitatea acestuia
    - *incident* stocheaza id-ul incidentului, tipul de prioritate, o descriere si statusul acestuia
    - *intervention* stocheaza atat datele despre incident, cat si despre echipaj
    - *system* stocheaza toate structurile de mai sus, practic necesare unui sistem de urgenta

Cat si structuri pentru crearea structurilor de date necesare: lista dublu inlantuita, coada, stiva:

    - *dll_node_t* stocheaza informatii despre un nod din lista, adica data-informatiile din nod, un pointer catre nodul de dinainte si un pointer catre cel de dupa
    - *doubly_linked_list_t* stocheaza informatii despre lista - head, unde incepe lista, dimensiunea informatiilor stocate si dimensiunea totala a listei.

### Functii pentru lucrul cu liste(cozi, stiva)

 * `dll_create` subprogramul initializeaza o lista si un nod care este inceputul acesteia, face legaturile si initializeaza dimensiunea listei de la 0.

 * `q_enqueue` subprogramul initializeaza un nod cu informatiile ce trebuie adaugate in lista, apoi este luat un nod care este initializat ca fiind ultimul nod, initial, iar apoi in contiuarea acestuia este pus noul nod, se refac legaturile si creste dimensiune listei.

 * `q_add_first` subprogramul ia un nod nou unde adauga informatia ce trebuie pusa in lista si o adauga la inceput, sarind peste santinela, se refac legaturile si creste dimensiune listei.

 * `s_dequeue` subprogramul ia un nod care este initializat ca fiind ultimul si un nod ca fiind penultimul element, cel ce va fi ultimul pentru ca se va sari, se vor reface legaturile si se va elimina informatia de la ultimul nod din lista initiala.

 * `q_dequeue` subprogramul sterge primul element din lista luand un nod ce pointeaza la acesta si unul ce pointeaza la urmatorul, sarind cu el peste primul element refacand legaturile si eliberand memoria.

 * `santinela`, `int_santinela` subprogramele initializeaza primul nod al listei cu informatiile indicate, ocupand fiecare camp din structura incident, respectiv intervention si incident.


 ### Functii pentru implementarea sistemului de interventii

 * `add_incident` subprogramul creeaza un nod de tip incident unde pune toate datele-trimise ca parametru, apoi se ia un nou nod in care data acestuia este fix nodul incident si se adauga la final noul nod. In functie de gradul de prioritate al cazului este adaugat si in coada corespunzatoare.

 * `read_unit` subprogramul citeste numarul de echipaje si apoi pentru fiecare id-ul si tipul, iar dupa le pune in coada pentru echipajele disponibile.

 * `check_units_availability` subprogramul afiseaza in fisierul de iesire numarul de echipaje disponibile.

 * `dispatch` subprogramul ia fiecare coada in parte in functie de prioritate si verifica daca exista incidente ce pot fi rezolvate.
 Daca exista, se ia un nod de tip interventie si se pun in campurile structurii incidentul si echipajul.
 Se marcheaza statusul ca intervened si echipajul ca nu mai fiind disponibil, iar apoi se pun in interventii si in stiva pentru interventii demarate informatiile.

 * `undo_last_dispatch` subprogramul ia din stiva de interventii demarate si pune data in structura de interventie.
 Daca statusul incidentului este intervened, statusul se modifica in queued si este adaugat in coada corespunzatoare prioritatii.
 Echipajul este marcat ca disponibil si este adaugat in coada.
 Se parcurg interventii, iar la final se sterge nodul respectiv.

 * `solved_incident` subprogramul parcurge lista interventii si cauta incidentul in functie de id primit, apoi modifica statusul ca fiind solved, iar echipajul ca fiind disponibil.

 * `show_unit` subprogramul pune in sirul mes disponibilitatea echipajului: available/unavailable, iar apoi afiseaza in fisierul de iesire toate informatiile despre echipaje.

 * `show_incident` subprogramul afiseaza informatii despre incidente cautand id ul primit in lista, afisand: id, prioritate, descriere, status.

 * `show_interventions` subprogramul parcurge interventii si afiseaza toate datele din acesta: atat despre incident cat si despre echipaj.

 * `read_command` subpogramul citeste din fisier comanda, compara si citeste unde e cazul informatii suplimentare si apeleaza apoi functia corespunzatoare pentru rezolvare.

 * `free_q` subprogramul ia un nod cu care parcurge toata lista, ia unul caruia ii da free si trece la urmatorul.
 La final da free intregii cozi.

 * `free_all` subprogramul da free prima data listei pentru incidente, parcurgand-o, dand free la date si trecand la urmatorul.
 La final da free si santinelei.
 La fel se intampla si pentru interventii.
 Se da free si pentru toate cozile folosite, apeland functia anterioara.

 ### MAIN

 In main, se deschid fisierele de intrare si iesire, se creeaza listele necesare, se adauga santinelele, se creeaza cozile si stiva, apoi se apeleaza functiile pentru rezolvarea problemelor.
 La final se da free pentru intregul program si se inchid fisierele.
