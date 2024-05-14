# Tietorakenteet ja algoritmit, syksy 2021
# Projektityö vaihe 2

### Projektissa tekemieni ratkaisujen taustaa

Yritin koko koodiprojektin ajan hyödyntää jo olemassa olevia ja hyväksi havaittuja
STL:n algoritmeja ja tietorakenteita. Ne ovat epäilemättä optimoitu tekemään työnsä
tehokkaimmalla mahdollisella tavalla.

Tietorakenteet:
Valitsin kaupunkien välisten teiden tallentamiseen tietorakenteeksi unordered_mapin.
Sitä on tehokasta indeksoida ja käydä läpi. Jokaisella kaupungilla on vaiheessa 1
toteutetussa kaupunkimapissa oma roads_-jäsenmuuttuja, joka on siis tämä umap. Sen keynä on
aina kaupungista lähtevän toinen pää (TownID) ja valuena tien pituus.
Lisäksi loin ykkösvaiheen Town-structiin joitakin muita uusia muuttujia, jotka auttavat tiegraafin
läpikäymisessä eri metodeissa.


Metodit:
Toteutin julkiset metodit hyödyntäen kurssilla esillä olleita graafialgoritmeja, kuten
DFS, BFS ja Djikstra. Ne ovat tehokkuuden kannalta hyviä algoritmeja.
Datastructures-luokan julkisten metodien lisäksi toteutin yhden uuden oman yksityisen
apumetodin hasCycle, joka on rekursiivinen metodi ja auttaa graafissa olevan silmukan
löytämisessä.
