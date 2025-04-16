Pentru rezolvarea temei am implementat pentru fiecare functie ceruta cate o functie in C.

Citesc si introduc intr-un vector pages paginile din .in. Initializez browserul (sentinel) si introduc tabul cu pagina acs in el.

Functia NEW_TAB:
    - initializez noul tab si introduc la finalul sentinelei tabul acs

Functia CLOSE:
    - verific daca tabul are ID 0 afisam Forbidden 403
    - parcurgem taburile pana cand ajungem in tabul curent
    - eliminam toate elementele din backwardStack si din forwardStack, iar mai apoi eliminam din lista tabul curent

Functia OPEN ID:
    - parcurgem browserul pana ajungem in dreptul tabului cu ID-ul cerut si il punem ca tabul curent
    - daca nu-l gasim afisam Forbidden 403

Functia NEXT:
    - parcurgem browserul pana ajungem in dreptul tabului curent si il setam pe urmatorul ca tab curent
    - in cazul in care urmatorul tab este sentinela punem ca tab curent urmatorul dupa sentinel

Functia PREV:
    - exact la fel ca next doar ca punem tabul dinainte

Functia PAGE ID:
    - cautam in vectorul de pagini, pagina cu ID-ul cerut si retinem indexul
    - daca nu gasim afisam 403 Forbidden si eliminam memoria comenzii (dam continue ca sa nu facem urmatoarele instructiuni)
    - daca gasim dam push la tabul curent in stiva backward a tabului curent si dam pop la toata stiva forward
    - punem apoi ca tab curent tabul dorit

Functia PRINT_HISTORY ID:
    - parcurgem browserul pana ajungem in dreptul tabului cu ID-ul cerut
    - afisam recursiv backwardstack, apoi afisam pagina curenta, apoi afisam recursiv forwardstack
    - am facut functii diferite de afisare backwardstack si respectiv forwardstack pentru a-mi facilita munca
    - daca nu gasim asemenea tab cu asemenea ID afisam 403 Forbidden

Functia BACKWARD:
    - daca in backwardstack nu se afla elemente afisam 403 Forbidden
    - altfel, punem pagina curecta in forwardstack, inlocuim pagina curenta cu elementul de top din backwardstack si il eliminam din acesta

Functia FORWARD:
    - similar cu BACKWARD doar ca inversam stivele

Functia PRINT:
    - parcurgem browserul pana ajungem in tabul curent apoi pornim afisarea din acel moment
    - la final afisam si descrierea paginii curente a tabului curent

In final eliberam vectorul de pagini, descrierea paginilor, browserul si stivele.