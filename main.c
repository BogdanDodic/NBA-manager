#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define spavaj(x) Sleep(x*1000)

typedef struct {
    char ime[50];
    int id_ekipe;
    long int cena;
    int ovr;
} Igrac;

typedef struct {
    int id;
    char ime[50];
    char konf[10];
    char div[20];
    int pobede;
    int porazi;
} Ekipa;

typedef struct {
    int protivnik_id;
    int da_li_sam_domacin;
} Utakmica;

void pocetak_kordinata(int x, int y)
{
    COORD coord={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void boja(int tekst, int pozadina)
{
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,(pozadina<<4)| tekst);
}

void crtanje_okv(int x,int y,int sirina,int visina)
{
    int i;
    pocetak_kordinata(x,y);
    printf("%c",201);
    for(i=0;i<sirina;i++)
    {
       printf("%c",205);
    }
    printf("%c",187);
    for(i=1;i<=visina;i++)
    {
        pocetak_kordinata(x,y+i);
        printf("%c",186);
        pocetak_kordinata(x+sirina+1,y+i);
        printf("%c",186);
    }
    pocetak_kordinata(x,y+visina+1);
    printf("%c",200);
    for(i=0;i<sirina;i++)
    {
        printf("%c",205);
    }
    printf("%c",188);
}

void zaglavlje()
{
    boja(15,4);
    crtanje_okv(2,1,74,3);
    pocetak_kordinata(31,2);
    printf("NBA MANAGER 2026");
    boja(7,0);
}

void pocetni_ekran()
{
    system("cls");
    boja(14,0);
    crtanje_okv(12, 6, 52, 5);
    pocetak_kordinata(15,8);  printf("                                             ");
    pocetak_kordinata(15,9);  printf("         DOBRODOSLI U NBA MANAGER 2026       ");
    pocetak_kordinata(15,10); printf("                                             ");
    boja(7,0);
    pocetak_kordinata(22,14);
    printf("PRITISNI BILO KOJI TASTER ZA START");
    _getch();
}

int ucitajTimove(Ekipa niz_ekipa[]) {
    FILE *fajl = fopen("timovi.txt", "r");
    if (fajl == NULL) {
        printf("Greska: Ne mogu da otvorim fajl 'timovi.txt'!\n");
        return -1;
    }
    int i = 0;
    while (i < 30 && fscanf(fajl, "%d,%[^,],%[^,],%[^\n]\n",
                  &niz_ekipa[i].id,
                  niz_ekipa[i].ime,
                  niz_ekipa[i].konf,
                  niz_ekipa[i].div) == 4) {
        niz_ekipa[i].pobede = 0;
        niz_ekipa[i].porazi = 0;
        i++;
    }
    fclose(fajl);
    return i;
}

int ucitajIgrace(Igrac svi_igraci[], int max_kapacitet) {
    FILE *fajl = fopen("igraci.txt", "r");
    if (fajl == NULL) {
        printf("Greska: Ne mogu da otvorim fajl 'igraci.txt'!\n");
        return -1;
    }
    int brojac = 0;
    while (brojac < max_kapacitet && fscanf(fajl, " %[^,],%d,%ld,%d\n",
                  svi_igraci[brojac].ime,
                  &svi_igraci[brojac].id_ekipe,
                  &svi_igraci[brojac].cena,
                  &svi_igraci[brojac].ovr) == 4) {
        brojac++;
    }
    fclose(fajl);
    return brojac;
}

int izborKonf() {
    int broj;
    do {
        system("cls");
        zaglavlje();
        pocetak_kordinata(2, 6);
        printf("==========================\n");
        printf("1. Istocna konferencija\n");
        printf("2. Zapadna konferencija\n");
        printf("Izaberi konferenciju (1 ili 2): ");
        scanf("%d", &broj);
        if (broj < 1 || broj > 2) {
            printf("Greska! Unesite 1 za Istok ili 2 za Zapad.\n");
            spavaj(1);
        }
    } while(broj < 1 || broj > 2);
    return broj;
}

int izbor() {
    int broj;
    do {
        pocetak_kordinata(2, 24);
        printf("Unesi broj za izbor ekipe (1-15): ");
        scanf("%d", &broj);
        if (broj < 1 || broj > 15) {
            printf("Greska! Molimo unesite broj izmedju 1 i 15.\n");
            spavaj(1);
        }
    } while(broj > 15 || broj < 1);
    return broj;
}

float racunajProsecanOvr(Igrac svi_igraci[], int broj_igraca, int id_ekipe) {
    int suma = 0, br = 0;
    for (int i = 0; i < broj_igraca; i++) {
        if (svi_igraci[i].id_ekipe == id_ekipe) {
            suma = suma + svi_igraci[i].ovr;
            br++;
        }
    }
    if (br == 0) return (float)0.0;
    return (float) suma / br;
}

void generisiNBAraspored(Ekipa niz_ekipa[], int broj_timova, int stvarni_indeks, Utakmica raspored[]) {
    int moj_id = niz_ekipa[stvarni_indeks].id;
    char* moja_konf = niz_ekipa[stvarni_indeks].konf;
    char* moja_div = niz_ekipa[stvarni_indeks].div;
    int ostali_iz_konf[15];
    int br_iz_konf = 0;
    int brojac_maceva = 0;

    for (int i = 0; i < broj_timova; i++) {
        if (niz_ekipa[i].id == moj_id) continue;
        if (strcmp(niz_ekipa[i].konf, moja_konf) == 0) {
            if (strcmp(niz_ekipa[i].div, moja_div) == 0) {
                for (int j = 0; j < 4; j++) {
                    raspored[brojac_maceva].protivnik_id = niz_ekipa[i].id;
                    raspored[brojac_maceva].da_li_sam_domacin = (j % 2 == 0);
                    brojac_maceva++;
                }
            } else {
                ostali_iz_konf[br_iz_konf] = i;
                br_iz_konf++;
            }
        } else {
            for (int j = 0; j < 2; j++) {
                raspored[brojac_maceva].protivnik_id = niz_ekipa[i].id;
                raspored[brojac_maceva].da_li_sam_domacin = (j % 2 == 0);
                brojac_maceva++;
            }
        }
    }
    for (int i = 0; i < br_iz_konf; i++) {
        int r = i + rand() % (br_iz_konf - i);
        int temp = ostali_iz_konf[i];
        ostali_iz_konf[i] = ostali_iz_konf[r];
        ostali_iz_konf[r] = temp;
    }
    for (int i = 0; i < br_iz_konf; i++) {
        int idx = ostali_iz_konf[i];
        int broj_utakmica = (i < 6) ? 4 : 3;
        for (int j = 0; j < broj_utakmica; j++) {
            if (brojac_maceva < 82) {
                raspored[brojac_maceva].protivnik_id = niz_ekipa[idx].id;
                raspored[brojac_maceva].da_li_sam_domacin = (j % 2 == 0);
                brojac_maceva++;
            }
        }
    }
    for (int i = 0; i < brojac_maceva; i++) {
        int r = i + rand() % (brojac_maceva - i);
        Utakmica temp = raspored[i];
        raspored[i] = raspored[r];
        raspored[r] = temp;
    }
}

void simulirajJednuUtakmicu(Igrac svi_igraci[], int broj_igraca, int moj_tim_id, Utakmica u, Ekipa niz_ekipa[], int broj_timova, int *trenutni_mec) {
    system("cls");
    zaglavlje();

    float moj_ovr = racunajProsecanOvr(svi_igraci, broj_igraca, moj_tim_id);
    float protivnik_ovr = racunajProsecanOvr(svi_igraci, broj_igraca, u.protivnik_id);

    char* moj_tim_ime = niz_ekipa[moj_tim_id].ime;
    char* protivnik_ime = niz_ekipa[u.protivnik_id].ime;

    pocetak_kordinata(2, 6);
    printf("=== UTAKMICA %d / 82 ===\n\n", *trenutni_mec + 1);
    if (u.da_li_sam_domacin) {
        printf("[KUCI] %s (OVR: %.1f)  vs  %s (OVR: %.1f)\n", moj_tim_ime, moj_ovr, protivnik_ime, protivnik_ovr);
    } else {
        printf("[GOSTI] %s (OVR: %.1f)  vs  %s (OVR: %.1f)\n", protivnik_ime, protivnik_ovr, moj_tim_ime, moj_ovr);
    }

    float razlika = moj_ovr - protivnik_ovr;
    if (u.da_li_sam_domacin) {
        razlika += 3.0f;
    } else {
        razlika -= 3.0f;
    }

    float sansa_moj = 50.0f + (razlika * 4.0f);
    if (sansa_moj > 92.0f) sansa_moj = 92.0f;
    if (sansa_moj < 8.0f) sansa_moj = 8.0f;

    printf("\nSansa za pobedu: %s (%.1f%%) vs %s (%.1f%%)\n", moj_tim_ime, sansa_moj, protivnik_ime, 100.0f - sansa_moj);
    printf("\nSimuliranje utakmice u toku...");
    fflush(stdout);
    spavaj(2);

    int rng = rand() % 100;
    int ja_pobedio = (rng < sansa_moj);
    int poeni_pobednik = 90 + (rand() % 26);
    int poeni_gubitnik = poeni_pobednik - (rand() % 14 + 1);
    int moji_poeni, protivnik_poeni;

    if (ja_pobedio) {
        moji_poeni = poeni_pobednik;
        protivnik_poeni = poeni_gubitnik;
    } else {
        moji_poeni = poeni_gubitnik;
        protivnik_poeni = poeni_pobednik;
    }

    niz_ekipa[moj_tim_id].pobede += ja_pobedio;
    niz_ekipa[moj_tim_id].porazi += !ja_pobedio;
    niz_ekipa[u.protivnik_id].pobede += !ja_pobedio;
    niz_ekipa[u.protivnik_id].porazi += ja_pobedio;

    int ostali_timovi[30];
    int br_ostalih = 0;
    for (int i = 0; i < broj_timova; i++) {
        if (niz_ekipa[i].id != moj_tim_id && niz_ekipa[i].id != u.protivnik_id) {
            ostali_timovi[br_ostalih] = i;
            br_ostalih++;
        }
    }

    for (int i = 0; i < br_ostalih; i++) {
        int r = i + rand() % (br_ostalih - i);
        int temp = ostali_timovi[i];
        ostali_timovi[i] = ostali_timovi[r];
        ostali_timovi[r] = temp;
    }

    for (int i = 0; i < br_ostalih; i += 2) {
        int idx1 = ostali_timovi[i];
        int idx2 = ostali_timovi[i + 1];
        float ovr1 = racunajProsecanOvr(svi_igraci, broj_igraca, niz_ekipa[idx1].id);
        float ovr2 = racunajProsecanOvr(svi_igraci, broj_igraca, niz_ekipa[idx2].id);
        float s1 = 50.0f + ((ovr1 - ovr2) * 4.0f);
        if (s1 > 90.0f) s1 = 90.0f;
        if (s1 < 10.0f) s1 = 10.0f;
        if ((rand() % 100) < s1) {
            niz_ekipa[idx1].pobede++;
            niz_ekipa[idx2].porazi++;
        } else {
            niz_ekipa[idx2].pobede++;
            niz_ekipa[idx1].porazi++;
        }
    }

    printf("\n\n=====================================\n");
    if (ja_pobedio) printf(" REZULTAT: POBEDA!\n");
    else printf(" REZULTAT: PORAZ!\n");
    printf(" %s  %d : %d  %s\n", moj_tim_ime, moji_poeni, protivnik_poeni, protivnik_ime);
    printf("=====================================\n");

    printf("\nTrenutni skor: %d POBEDA - %d PORAZA\n", niz_ekipa[moj_tim_id].pobede, niz_ekipa[moj_tim_id].porazi);

    (*trenutni_mec)++;

    printf("\nPritisnite Enter za povratak u glavni meni...");
    getchar(); getchar();
}

void ispisiRoster(Igrac svi_igraci[], int broj_igraca, int trazeni_tim_id, Ekipa niz_ekipa[]) {
    Igrac roster[20];
    int br = 0;
    for(int i = 0; i < broj_igraca; i++) {
        if(svi_igraci[i].id_ekipe == trazeni_tim_id) {
            roster[br] = svi_igraci[i];
            br++;
        }
    }
    for(int i = 0; i < br - 1; i++) {
        for(int j = 0; j < br - i - 1; j++) {
            if(roster[j].ovr < roster[j + 1].ovr) {
                Igrac temp = roster[j];
                roster[j] = roster[j + 1];
                roster[j + 1] = temp;
            }
        }
    }

    pocetak_kordinata(2, 6);
    printf("==================================================\n");
    printf("           ROSTER TIMA: %s\n", niz_ekipa[trazeni_tim_id].ime);
    printf("==================================================\n");
    for(int i = 0; i < br; i++) {
        printf("%-24s | OVR: %-3d | $%ld\n", roster[i].ime, roster[i].ovr, roster[i].cena);
    }
}

void ispisi_opcije() {
    pocetak_kordinata(2, 6);
    printf("==================================================\n");
    printf("1. Odigraj utakmicu\n");
    printf("2. Simuliraj celu sezonu\n");
    printf("3. Pregled ekipe\n");
    printf("4. Prikaz tabele\n");
    printf("5. Transfer igraca\n");
    printf("6. Izlaz / Kraj sezone\n");
    printf("==================================================\n");
}

int izbor_meni() {
    int broj;
    printf("Unesi broj za izbor opcije: ");
    do {
        scanf("%d", &broj);
        if (broj < 1 || broj > 6)
            printf("Greska! Molimo unesite broj izmedju 1 i 5.\n\n");
    } while(broj < 1 || broj > 6);
    return broj;
}

void prikaziTabelu(Ekipa niz_ekipa[], int broj_timova) {
    system("cls");
    zaglavlje();

    Ekipa istok[15];
    Ekipa zapad[15];
    int br_istok = 0;
    int br_zapad = 0;
    for (int i = 0; i < broj_timova; i++) {
        if (strcmp(niz_ekipa[i].konf, "Istok") == 0) {
            istok[br_istok] = niz_ekipa[i];
            br_istok++;
        } else if (strcmp(niz_ekipa[i].konf, "Zapad") == 0) {
            zapad[br_zapad] = niz_ekipa[i];
            br_zapad++;
        }
    }

    for (int i = 0; i < br_istok - 1; i++) {
        for (int j = 0; j < br_istok - i - 1; j++) {
            int treba_zamena = 0;
            if (istok[j].pobede < istok[j + 1].pobede) {
                treba_zamena = 1;
            } else if (istok[j].pobede == istok[j + 1].pobede) {
                if (istok[j].porazi > istok[j + 1].porazi) {
                    treba_zamena = 1;
                }
            }
            if (treba_zamena) {
                Ekipa temp = istok[j];
                istok[j] = istok[j + 1];
                istok[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < br_zapad - 1; i++) {
        for (int j = 0; j < br_zapad - i - 1; j++) {
            int treba_zamena = 0;
            if (zapad[j].pobede < zapad[j + 1].pobede) {
                treba_zamena = 1;
            } else if (zapad[j].pobede == zapad[j + 1].pobede) {
                if (zapad[j].porazi > zapad[j + 1].porazi) {
                    treba_zamena = 1;
                }
            }
            if (treba_zamena) {
                Ekipa temp = zapad[j];
                zapad[j] = zapad[j + 1];
                zapad[j + 1] = temp;
            }
        }
    }

    pocetak_kordinata(2, 6);
    printf("=============================================\n");
    printf("               ISTOCNA KONFERENCIJA               \n");
    printf("=============================================\n");
    printf("%-4s | %-22s | %-4s | %-4s\n", "POZ", "TIM", "POB", "POR");
    printf("---------------------------------------------\n");
    for (int i = 0; i < br_istok; i++) {
        printf("%-4d | %-22s | %-4d | %-4d\n", i + 1, istok[i].ime, istok[i].pobede, istok[i].porazi);
    }

    printf("\n=============================================\n");
    printf("               ZAPADNA KONFERENCIJA               \n");
    printf("=============================================\n");
    printf("%-4s | %-22s | %-4s | %-4s\n", "POZ", "TIM", "POB", "POR");
    printf("---------------------------------------------\n");
    for (int i = 0; i < br_zapad; i++) {
        printf("%-4d | %-22s | %-4d | %-4d\n", i + 1, zapad[i].ime, zapad[i].pobede, zapad[i].porazi);
    }
    printf("=============================================\n");
}

void transferIgraca(Igrac svi_igraci[], int broj_igraca, int moj_tim_id, int *preostali_pokusaji, Ekipa niz_ekipa[]) {
    system("cls");
    zaglavlje();

    pocetak_kordinata(2, 6);
    if (*preostali_pokusaji <= 0) {
        printf("=====================================\n");
        printf("          TRANSFER CENTAR\n");
        printf("=====================================\n\n");
        printf("Greska: Iskoristili ste svih 5 pokusaja za trade u ovoj sezoni!\n");
        printf("\nPritisni Enter za povratak...");
        getchar(); getchar();
        return;
    }

    int moji[20];
    int br_mojih = 0;
    printf("=====================================\n");
    printf("          TRANSFER CENTAR\n");
    printf("=====================================\n\n");
    printf("Vasi igraci:\n\n");
    for(int i = 0; i < broj_igraca; i++) {
        if(svi_igraci[i].id_ekipe == moj_tim_id) {
            printf("%d. %-24s OVR %d\n", br_mojih + 1, svi_igraci[i].ime, svi_igraci[i].ovr);
            moji[br_mojih] = i;
            br_mojih++;
        }
    }

    int izbor;
    printf("\nIzaberite igraca za trade: ");
    scanf("%d",&izbor);
    izbor--;
    if(izbor < 0 || izbor >= br_mojih) {
        printf("Pogresan unos!\n");
        printf("\nPritisni Enter...");
        getchar(); getchar();
        return;
    }

    int moj_idx = moji[izbor];
    int kandidati[400];
    int br_kandidata = 0;
    printf("\nMoguci tradeovi:\n\n");
    for(int i = 0; i < broj_igraca; i++) {
        if(svi_igraci[i].id_ekipe != moj_tim_id) {
            if(abs(svi_igraci[i].ovr - svi_igraci[moj_idx].ovr) <= 2) {
                printf("%d. %-22s | %-22s | OVR %d\n", br_kandidata + 1, svi_igraci[i].ime, niz_ekipa[svi_igraci[i].id_ekipe].ime, svi_igraci[i].ovr);
                kandidati[br_kandidata] = i;
                br_kandidata++;
            }
        }
    }

    if(br_kandidata == 0) {
        printf("Nema dostupnih tradeova!\n");
        printf("\nPritisni Enter...");
        getchar(); getchar();
        return;
    }

    int izbor2;
    printf("\nIzaberi igraca za zamenu: ");
    scanf("%d",&izbor2);
    izbor2--;
    if(izbor2 < 0 || izbor2 >= br_kandidata) {
        printf("Pogresan unos!\n");
        printf("\nPritisni Enter...");
        getchar(); getchar();
        return;
    }

    int drugi_idx = kandidati[izbor2];
    printf("\n=====================================\n");
    printf("PONUDA TRADEA\n");
    printf("=====================================\n");
    printf("%s (%d)\n", svi_igraci[moj_idx].ime, svi_igraci[moj_idx].ovr);
    printf("ZA\n");
    printf("%s (%d)\n", svi_igraci[drugi_idx].ime, svi_igraci[drugi_idx].ovr);

    int potvrda;
    printf("\n1 - Prihvati trade\n");
    printf("0 - Odbij trade\n");
    printf("Izbor: ");
    scanf("%d",&potvrda);

    if(potvrda == 1) {
        (*preostali_pokusaji)--;
        int razlika_ovr = svi_igraci[moj_idx].ovr - svi_igraci[drugi_idx].ovr;
        int sansa_za_prihvatanje = 50;

        if (razlika_ovr >= 0) {
            sansa_za_prihvatanje += razlika_ovr * 20;
        } else {
            sansa_za_prihvatanje += razlika_ovr * 25;
        }

        if (sansa_za_prihvatanje > 95) sansa_za_prihvatanje = 95;
        if (sansa_za_prihvatanje < 5) sansa_za_prihvatanje = 5;

        printf("\nRazmatranje ponude od strane tima %s...\n", niz_ekipa[svi_igraci[drugi_idx].id_ekipe].ime);
        printf("(Preostalo pokusaja za trade u sezoni: %d)\n", *preostali_pokusaji);
        spavaj(2);

        int rng = rand() % 100;
        if (rng < sansa_za_prihvatanje) {
            int privremeni_id = svi_igraci[moj_idx].id_ekipe;
            svi_igraci[moj_idx].id_ekipe = svi_igraci[drugi_idx].id_ekipe;
            svi_igraci[drugi_idx].id_ekipe = privremeni_id;
            printf("\nTRADE PRIHVACEN!\n");
        }
        else {
            printf("\nTrade odbijen od strane uprave...\n");
        }
    }
    else {
        printf("\nTrade odbijen sa vase strane...\n");
    }
    printf("\nPritisni Enter...");
    getchar(); getchar();
}
void simulirajCeluSezonu(
    Igrac svi_igraci[],
    int broj_igraca,
    int moj_tim_id,
    Ekipa niz_ekipa[],
    int broj_timova,
    Utakmica raspored[],
    int *trenutni_mec)
{
    while(*trenutni_mec < 82)
    {
        Utakmica u = raspored[*trenutni_mec];

        float moj_ovr = racunajProsecanOvr(
            svi_igraci,
            broj_igraca,
            moj_tim_id);

        float protivnik_ovr = racunajProsecanOvr(
            svi_igraci,
            broj_igraca,
            u.protivnik_id);

        float razlika = moj_ovr - protivnik_ovr;

        if(u.da_li_sam_domacin)
            razlika += 3;
        else
            razlika -= 3;

        float sansa = 50 + razlika * 4;

        if(sansa > 92) sansa = 92;
        if(sansa < 8) sansa = 8;

        int ja_pobedio = (rand()%100 < sansa);

        niz_ekipa[moj_tim_id].pobede += ja_pobedio;
        niz_ekipa[moj_tim_id].porazi += !ja_pobedio;

        niz_ekipa[u.protivnik_id].pobede += !ja_pobedio;
        niz_ekipa[u.protivnik_id].porazi += ja_pobedio;

        int ostali_timovi[30];
        int br_ostalih = 0;

        for(int i=0;i<broj_timova;i++)
        {
            if(niz_ekipa[i].id != moj_tim_id &&
               niz_ekipa[i].id != u.protivnik_id)
            {
                ostali_timovi[br_ostalih++] = i;
            }
        }

        for(int i=0;i<br_ostalih;i++)
        {
            int r = i + rand()%(br_ostalih-i);

            int temp = ostali_timovi[i];
            ostali_timovi[i] = ostali_timovi[r];
            ostali_timovi[r] = temp;
        }

        for(int i=0;i<br_ostalih;i+=2)
        {
            int idx1 = ostali_timovi[i];
            int idx2 = ostali_timovi[i+1];

            float ovr1 =
                racunajProsecanOvr(
                    svi_igraci,
                    broj_igraca,
                    niz_ekipa[idx1].id);

            float ovr2 =
                racunajProsecanOvr(
                    svi_igraci,
                    broj_igraca,
                    niz_ekipa[idx2].id);

            float s1 = 50 + (ovr1-ovr2)*4;

            if(s1 > 90) s1 = 90;
            if(s1 < 10) s1 = 10;

            if(rand()%100 < s1)
            {
                niz_ekipa[idx1].pobede++;
                niz_ekipa[idx2].porazi++;
            }
            else
            {
                niz_ekipa[idx2].pobede++;
                niz_ekipa[idx1].porazi++;
            }
        }

        (*trenutni_mec)++;
    }

    printf("\nSezona simulirana!\n");
    printf("Pritisni Enter...");
    getchar();
    getchar();
}
void meni(Igrac svi_igraci[], int broj_igraca, int moj_tim_id, Ekipa niz_ekipa[], int broj_timova, Utakmica raspored[], int *trenutni_mec, int *preostali_pokusaji) {
    int broj;
    do {
        system("cls");
        zaglavlje();
        ispisi_opcije();
        broj = izbor_meni();
        switch(broj) {
        case 1:
            if (*trenutni_mec < 82) {
                simulirajJednuUtakmicu(svi_igraci, broj_igraca, moj_tim_id, raspored[*trenutni_mec], niz_ekipa, broj_timova, trenutni_mec);
            } else {
                system("cls");
                zaglavlje();
                pocetak_kordinata(2, 6);
                printf("\nSezona je zavrsena! Odigrali ste sve 82 utakmice.\n");
                printf("Izadjite iz menija na opciju 5 kako biste presli u sledecu sezonu!\n");
                printf("Pritisnite Enter za povratak...");
                getchar(); getchar();
            }
            break;
        case 2:
            simulirajCeluSezonu(
            svi_igraci,
            broj_igraca,
            moj_tim_id,
            niz_ekipa,
            broj_timova,
            raspored,
            trenutni_mec);
    break;
        case 3:
            system("cls");
            zaglavlje();
            ispisiRoster(svi_igraci, broj_igraca, moj_tim_id, niz_ekipa);
            printf("\nPritisnite Enter za povratak u meni...");
            getchar(); getchar();
            break;
        case 4:
            prikaziTabelu(niz_ekipa, broj_timova);
            printf("\nPritisnite Enter za povratak u meni...");
            getchar(); getchar();
            break;
        case 5:
            transferIgraca(svi_igraci, broj_igraca, moj_tim_id, preostali_pokusaji, niz_ekipa);
            break;
        case 6:
            break;
        }
    } while(broj != 6);
}

void sortirajKonferenciju(Ekipa konferencija[], int br)
{
    for(int i=0;i<br-1;i++)
    {
        for(int j=0;j<br-i-1;j++)
        {
            if(konferencija[j].pobede < konferencija[j+1].pobede)
            {
                Ekipa temp=konferencija[j];
                konferencija[j]=konferencija[j+1];
                konferencija[j+1]=temp;
            }
        }
    }
}
int simulirajSeriju(
    Igrac svi_igraci[],
    int broj_igraca,
    Ekipa tim1,
    Ekipa tim2)
{
    int pobede1=0;
    int pobede2=0;

    float ovr1=racunajProsecanOvr(
        svi_igraci,
        broj_igraca,
        tim1.id);

    float ovr2=racunajProsecanOvr(
        svi_igraci,
        broj_igraca,
        tim2.id);

    while(pobede1<4 && pobede2<4)
    {
        float sansa=50+(ovr1-ovr2)*4;

        if(sansa>90) sansa=90;
        if(sansa<10) sansa=10;

        if(rand()%100<sansa)
            pobede1++;
        else
            pobede2++;
    }

    printf("%s %d - %d %s\n",
           tim1.ime,
           pobede1,
           pobede2,
           tim2.ime);

    if(pobede1==4)
        return tim1.id;

    return tim2.id;
}
Ekipa nadjiTim(Ekipa niz_ekipa[], int broj_timova, int id)
{
    for(int i=0;i<broj_timova;i++)
    {
        if(niz_ekipa[i].id==id)
            return niz_ekipa[i];
    }

    return niz_ekipa[0];
}
void playoff(
    Igrac svi_igraci[],
    int broj_igraca,
    Ekipa niz_ekipa[],
    int broj_timova,
    int moj_tim_id)
{
    Ekipa konf[15];
    int br=0;

    char moja_konf[10];

    for(int i=0;i<broj_timova;i++)
    {
        if(niz_ekipa[i].id==moj_tim_id)
        {
            strcpy(moja_konf,niz_ekipa[i].konf);
            break;
        }
    }

    for(int i=0;i<broj_timova;i++)
    {
        if(strcmp(niz_ekipa[i].konf,moja_konf)==0)
        {
            konf[br++]=niz_ekipa[i];
        }
    }

    sortirajKonferenciju(konf,br);

    int usao=0;

    for(int i=0;i<8;i++)
    {
        if(konf[i].id==moj_tim_id)
            usao=1;
    }

    system("cls");

    printf("=================================\n");
    printf("          NBA PLAYOFF\n");
    printf("=================================\n\n");

    if(!usao)
    {
        printf("Niste usli u playoff!\n");
        return;
    }

    printf("CETVRTFINALE\n\n");

    int p1=simulirajSeriju(
        svi_igraci,
        broj_igraca,
        konf[0],
        konf[7]);

    int p2=simulirajSeriju(
        svi_igraci,
        broj_igraca,
        konf[3],
        konf[4]);

    int p3=simulirajSeriju(
        svi_igraci,
        broj_igraca,
        konf[1],
        konf[6]);

    int p4=simulirajSeriju(
        svi_igraci,
        broj_igraca,
        konf[2],
        konf[5]);

    printf("\nPOLUFINALE\n\n");

    Ekipa t1=nadjiTim(niz_ekipa,broj_timova,p1);
    Ekipa t2=nadjiTim(niz_ekipa,broj_timova,p2);

    Ekipa t3=nadjiTim(niz_ekipa,broj_timova,p3);
    Ekipa t4=nadjiTim(niz_ekipa,broj_timova,p4);

    int f1=simulirajSeriju(
        svi_igraci,
        broj_igraca,
        t1,
        t2);

    int f2=simulirajSeriju(
        svi_igraci,
        broj_igraca,
        t3,
        t4);

    printf("\nFINALE KONFERENCIJE\n\n");

    Ekipa fin1=nadjiTim(niz_ekipa,broj_timova,f1);
    Ekipa fin2=nadjiTim(niz_ekipa,broj_timova,f2);

    int sampion=simulirajSeriju(
        svi_igraci,
        broj_igraca,
        fin1,
        fin2);

    Ekipa pobednik=
        nadjiTim(niz_ekipa,broj_timova,sampion);

    printf("\n=================================\n");
    printf("SAMPION KONFERENCIJE:\n");
    printf("%s\n",pobednik.ime);
    printf("=================================\n");
}

void resetujSezonu(Ekipa niz_ekipa[], int broj_timova) {
    for (int i = 0; i < broj_timova; i++) {
        niz_ekipa[i].pobede = 0;
        niz_ekipa[i].porazi = 0;
    }
}


int main() {
    srand(time(NULL));
    Igrac svi_igraci[1000];
    Ekipa niz_ekipa[30];
    Utakmica raspored[82];

    int broj_timova = ucitajTimove(niz_ekipa);
    if (broj_timova == -1) return 1;

    int broj_igraca = ucitajIgrace(svi_igraci, 1000);
    if (broj_igraca == -1) return 1;

    pocetni_ekran();

    int konf = izborKonf();
    int start_index = (konf == 1) ? 0 : 15; // Istok pocinje od 0, Zapad od 15

    system("cls");
    zaglavlje();
    pocetak_kordinata(2, 6);
    printf("\n=== IZABERITE SVOJ TIM ===\n");
    for (int i = 0; i < 15; i++) {
        printf("%d. %s\n", i + 1, niz_ekipa[start_index + i].ime);
    }
    printf("==========================\n");
    int izabran_broj = izbor();
    int stvarni_indeks = start_index + (izabran_broj - 1);
    int moj_tim_id = niz_ekipa[stvarni_indeks].id;

    int sledeca_sezona = 1;
    int broj_sezone = 1;

    do {
        int trenutni_mec = 0;
        int preostali_pokusaji = 5;

        resetujSezonu(niz_ekipa, broj_timova);
        generisiNBAraspored(niz_ekipa, broj_timova, stvarni_indeks, raspored);

        system("cls");
        zaglavlje();
        pocetak_kordinata(2, 6);
        printf("=====================================\n");
        printf("         DOBRODOSLI U SEZONU %d       \n", broj_sezone);
        printf("=====================================\n\n");

        ispisiRoster(svi_igraci, broj_igraca, moj_tim_id, niz_ekipa);

        printf("\nPritisni enter da zapocnes sezonu %d: ", broj_sezone);
        getchar(); getchar();

        meni(svi_igraci, broj_igraca, moj_tim_id, niz_ekipa, broj_timova, raspored, &trenutni_mec, &preostali_pokusaji);

        system("cls");
        zaglavlje();
        pocetak_kordinata(2, 6);
        if (trenutni_mec >= 82) {
            playoff(
    svi_igraci,
    broj_igraca,
    niz_ekipa,
    broj_timova,
    moj_tim_id);

printf("\nPritisni Enter...");
getchar();
getchar();
            printf("=== KRAJ SEZONE %d ===\n\n", broj_sezone);
            printf("Zelite li da predjete u sledecu sezonu?\n");
            printf("1 - Da, igraj sledecu sezonu\n");
            printf("0 - Ne, ugasi igricu\n");
            printf("Vas izbor: ");
            scanf("%d", &sledeca_sezona);

            if (sledeca_sezona == 1) {
                broj_sezone++;
            }
        } else {
            printf("\nNapustili ste igru pre kraja sezone. Hvala na igranju!\n");
            sledeca_sezona = 0;
        }

    } while (sledeca_sezona == 1);

    printf("\nGasim igricu... Pozdrav!\n");
    spavaj(2);
    return 0;
}
