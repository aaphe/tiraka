# Tietorakenteet ja algoritmit, syksy 2021
# Projektityö vaihe 1

### Projektissa tekemieni ratkaisujen taustaa

Yritin koko koodiprojektin ajan hyödyntää jo olemassa olevia ja hyväksi havaittuja
STL:n algoritmeja ja tietorakenteita. Ne ovat epäilemättä optimoitu tekemään työnsä
tehokkaimmalla mahdollisella tavalla.

Tietorakenteet:
Koko ohjelman toiminta perustuu datastructures-luokan jäsenmuuttujana olevaan towndata_
-tietorakenteeseen. Se on STL:n unordered_map-tietorakenne, jonka keynä ovat kaupunkien id:t,
ja valuena Town-structit. Town-struct on määritelty erikseen luokan hh-tiedostossa, ja siinä
on muuttuja jokaiselle yhden kaupungin kuvaamisen kannalta olennaiselle tiedolle. Tarvittaessa
tietoja voisi olla enemmänkin, mutta toteutuksessa muuttujia ovat kaupungin id, nimi,
koordinaatit, talonpojilta tms. ihmisiltä saatava vero, vasallikaupunkien id:t vektorissa ja
isäntäkaupungin id.
Unordered_map päätyi valinnakseni päätietorakenteeksi lähes ilmiselvästi. Kaupunkeja käsitellään
ohjelmassa lähinnä juuri id:n kautta, ja unordered_map on erittäin tehokas arvon etsinnässä
keyn avulla. Struct taas on kätevä ja tehokas tapa kuvata yksittäistä kaupunkia. Vaihtoehtoisia
tietorakenteita olisi tietysti ollut paljonkin, mutta esimerkiksi Ohjelmointi 2 -kurssilta tuttu
map on velipuoltaan huomattavasti epätehokkaampi, eikä kaupunkien tarvinnut olla tietorakenteessa
millään tavalla järjestyksessä.

Metodit:
Datastructures-luokan julkisten metodien lisäksi toteutin kolme omaa yksityistä apumetodia.
VassalsRecursive ja taxFromVassal ovat vastaavien julkisten metodien apuna toimivia
rekursiivisia funktioita. Niiden tuottama toiminnallisuus olisi mahdollisesti onnistunut
julkisessakin metodissa, mutta koin selkeäksi ratkaisuksi luoda rekursiot kokonaan omissa funktioissaan.
Dist-metodi on kaikessa yksinkertaisuudessaan etäisyyksien laskennassa käytetty funktio, joka laskee
euklidisen etäisyyden kahden annetun pisteen välillä.
