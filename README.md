Bloțiu Mihnea-Andrei & Roșu Mihai-Cosmin 
313CA - Rope data structure - 03.06.2021

Pentru realizarea am completat funcțiile necesare din fișierul rope.c și
am adăugat câteva în plus pentru buna modularizare a codului.

makeRopeNode:
 - Am alocat memoria pentru un nod;
 - Am verificat dacă memoria a fost alocată cu succes;
 - Am inițializat copii nodului ca fiind NULL;
 - Greutatea egală cu lungimea șirului;
 - Am setat pointer-ul nodului spre string-ul primit;
 
makeRopeTree:
 - Am alocat memoria pentru un rope;
 - Am verificat dacă memoria a fost alocată cu succes;
 - Am inițializat rădăcina rope-ului ca fiind cea primită;

concat:
 - Pentru a crea noul rope ne-am folosit de funcția __concat,
 ce unește două noduri trimise ca parametru (rădăcinile rope-urilor
 primite);
 - Funcția __concat crează un nod ai cărui copii sunt cele două
 rădăcini primite și calculează greutatea sa.
 
indexRope:
 - Folosim funcția __indexRope, pornind de la rădăcina rope-ului
 și indexul primite.
 - Prin comparearea greutății nodului curent cu indexul căutat, mergem
 alternativ pe subarborele stâng sau drept, dacă acestea există.
 - În cazul în care am ajuns la poziția dorită, întoarcem caracterul de
 pe acea poziție.
 
search:
 - Pentru întoarcerea șirului dintre cele două poziții primite, alocăm
 memorie pentru acesta și îl completăm caracter cu caracter folosind
 funcția indexRope.
 
split:
 - Pentru împărțirea arborelui inițial este necesară o clonare a acestuia
 pentru a nu îl pierde la următorul apel. Acest lucru se realizează prin
 funcția __cloneNode care duplică arborele pornind de la rădăcină.
 - Cei doi arbori rezultați după împărțire se vor reține într-o structură
 de tip SplitPair.
 - Verificăm unde anume trebuie făcută această împărțire și rezultă trei
 cazuri (la început, la mijloc și la final).
 - Pentru impărțirea la mijloc, nodul stâng al SplitPair-ului este rădăcina
 inițială, iar cel drept va fi nodul prev rezultat în urma apelării funcției
 __split.
 - Funcția __split parcurge arborele de la stânga la dreapta și caută poziția
 index, de unde trebuie să înceapă împărțirea.
 - Dacă această poziție se află în interiorul unei frunze, funcția crează încă
 două noduri care vor deveni copiii frunzei curente și vor reține string-ul
 inițial împărțit după poziția index.
 - Când am găsit index-ul începem împărțirea care se propagă de-a lungul
 arborelui, motiv pentru care concatenăm succesiv cu prev nodurile tăiate,
 iar apoi actualizăm valoarea lui prev pentru a fi rădăcina noului arbore.
 - Pentru împărțirea la început sau la final, practic nu se realizează
 nicio tăiere, deci tot arborele rămâne pe o singură parte a SplitPair-ului.
 
insert:
 - Pentru a insera un nou șir în rope pe poziția index, mai întâi cream un nou
 nod cu șirul precizat și împărțim rope-ul la poziția indicată.
 - Dacă adăugarea se află la mijlocul rope-ului, atunci sunt necesare două
 concatenări: una între partea stângă a SplitPair-ului și noul nod și alta
 între rope-ul nou format și partea dreaptă a SplitPair-ului.
 - Dacă adăugarea se află la început sau la final, facem o singură concatenare
 între noul nod și partea corespunzătoare a SplitPair-ului, pentru a păstra
 ordinea corectă a șirului.
 
__freeNode:
 - Eliberăm un subarbore întreg pornind de la un nod printr-o parcurgere
 în postordine.
 
delete:
 - Pentru a șterge o parte din șirul de caractere la început sau la final,
 facem o singură împărțire a rope-ului pe poziția len și respectiv, start,
 ștergem partea ce trebuie eliminată și facem un nou rope cu partea rămasă.
 - Pentru a șterge o parte din șirul de caractere la mijloc, sunt necesare
 două împărțiri: una pe poziția start a rope-ului inițial și una pe poziția
 len a rope-ului nou format cu partea dreaptă a SplitPair-ului creat după
 prima împărțire. Se eliberează părțile ce trebuie eliminate și se formează
 rope-ul final prin concatenarea părților rămase din cele două SplitPair-uri.
 
