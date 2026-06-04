#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#define spavaj(x) Sleep(x*1000)

typedef struct {
    char ime[50];
    char ekipa[50];
    long int cena;
    int ovr;
} Igrac;

typedef struct {
    char ime[50];
    char konf[10];
    char div[20];
    int pobede;
    int porazi;
} Ekipa;

typedef struct {
    char protivnik[50];
    int da_li_sam_domacin;
} Utakmica;

int ucitajTimove(Ekipa niz_ekipa[]) {
    FILE *fajl = fopen("timovi.txt", "r");
    if (fajl == NULL) {
        printf("Greska: Ne mogu da otvorim fajl 'timovi.txt'!\n");
        return -1;
    }
    int i = 0;
    while (i < 30 && fscanf(fajl, " %[^,],%[^,],%[^\n]\n",
                  niz_ekipa[i].ime,
                  niz_ekipa[i].konf,
                  niz_ekipa[i].div) == 3) {
        niz_ekipa[i].pobede = 0;
        niz_ekipa[i].porazi = 0;
        i++;
    }
    fclose(fajl);
    return i;
}

int izborKonf() {
    int broj;
    do {
        printf("\n==========================\n");
        printf("1. Istocna konferencija\n");
        printf("2. Zapadna konferencija\n");
        printf("Izaberi konferenciju (1 ili 2): ");
        scanf("%d", &broj);
        if (broj < 1 || broj > 2) {
            printf("Greska! Unesite 1 za Istok ili 2 za Zapad.\n");
        }
    } while(broj < 1 || broj > 2);
    printf("==========================\n");
    return broj;
}

int izbor() {
    int broj;
    do {
        printf("Unesi broj za izbor ekipe (1-15): ");
        scanf("%d", &broj);
        if (broj < 1 || broj > 15)
            printf("Greska! Molimo unesite broj izmedju 1 i 15.\n\n");
    } while(broj > 15 || broj < 1);
    return broj;
}

int ucitajIgrace(Igrac svi_igraci[], int max_kapacitet) {
    FILE *fajl = fopen("igraci.txt", "r");
    if (fajl == NULL) {
        printf("Greska: Ne mogu da otvorim fajl 'igraci.txt'!\n");
        return -1;
    }
    int brojac = 0;
    while (brojac < max_kapacitet && fscanf(fajl, " %[^,],%[^,],%ld,%d\n",
                  svi_igraci[brojac].ime,
                  svi_igraci[brojac].ekipa,
                  &svi_igraci[brojac].cena,
                  &svi_igraci[brojac].ovr) == 4) {
        brojac++;
    }
    fclose(fajl);
    return brojac;
}

float racunajProsecanOvr(Igrac svi_igraci[], int broj_igraca, char tim[]) {
    int suma = 0, br = 0;
    for (int i = 0; i < broj_igraca; i++) {
        if (strcmp(svi_igraci[i].ekipa, tim) == 0) {
            suma = suma + svi_igraci[i].ovr;
            br++;
        }
    }
    if (br == 0) return (float)0.0;
    return (float) suma / br;
}

void simulirajJednuUtakmicu(Igrac svi_igraci[], int broj_igraca, char moj_tim[], Utakmica u, Ekipa niz_ekipa[], int broj_timova, int *trenutni_mec) {
    system("cls");
    float moj_ovr = racunajProsecanOvr(svi_igraci, broj_igraca, moj_tim);
    float protivnik_ovr = racunajProsecanOvr(svi_igraci, broj_igraca, u.protivnik);
    printf("=== UTRAKMICA %d / 82 ===\n\n", *trenutni_mec + 1);
    if (u.da_li_sam_domacin) {
        printf("[KUCI] %s (OVR: %.1f)  vs  %s (OVR: %.1f)\n", moj_tim, moj_ovr, u.protivnik, protivnik_ovr);
    } else {
        printf("[GOSTI] %s (OVR: %.1f)  vs  %s (OVR: %.1f)\n", u.protivnik, protivnik_ovr, moj_tim, moj_ovr);
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
    printf("\nSansa za pobedu: %s (%.1f%%) vs %s (%.1f%%)\n", moj_tim, sansa_moj, u.protivnik, 100.0f - sansa_moj);
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
    for (int i = 0; i < broj_timova; i++) {
        if (strcmp(niz_ekipa[i].ime, moj_tim) == 0) {
            if (ja_pobedio) niz_ekipa[i].pobede++;
            else niz_ekipa[i].porazi++;
        }
        if (strcmp(niz_ekipa[i].ime, u.protivnik) == 0) {
            if (!ja_pobedio) niz_ekipa[i].pobede++;
            else niz_ekipa[i].porazi++;
        }
    }
    int ostali_timovi[30];
    int br_ostalih = 0;
    for (int i = 0; i < broj_timova; i++) {
        if (strcmp(niz_ekipa[i].ime, moj_tim) != 0 && strcmp(niz_ekipa[i].ime, u.protivnik) != 0) {
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
        float ovr1 = racunajProsecanOvr(svi_igraci, broj_igraca, niz_ekipa[idx1].ime);
        float ovr2 = racunajProsecanOvr(svi_igraci, broj_igraca, niz_ekipa[idx2].ime);
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
    printf(" %s  %d : %d  %s\n", moj_tim, moji_poeni, protivnik_poeni, u.protivnik);
    printf("=====================================\n");
    for (int i = 0; i < broj_timova; i++) {
        if (strcmp(niz_ekipa[i].ime, moj_tim) == 0) {
            printf("\nTrenutni skor: %d POBEDA - %d PORAZA\n", niz_ekipa[i].pobede, niz_ekipa[i].porazi);
            break;
        }
    }
    (*trenutni_mec)++;
    printf("\nPritisnite Enter za povratak u glavni meni...");
    getchar(); getchar();
}

void ispisiRoster(Igrac svi_igraci[], int broj_igraca, char trazeni_tim[])
{
    system("cls");

    Igrac roster[20];
    int br = 0;

    for(int i = 0; i < broj_igraca; i++)
    {
        if(strcmp(svi_igraci[i].ekipa, trazeni_tim) == 0)
        {
            roster[br] = svi_igraci[i];
            br++;
        }
    }

    for(int i = 0; i < br - 1; i++)
    {
        for(int j = 0; j < br - i - 1; j++)
        {
            if(roster[j].ovr < roster[j + 1].ovr)
            {
                Igrac temp = roster[j];
                roster[j] = roster[j + 1];
                roster[j + 1] = temp;
            }
        }
    }

    printf("\n==================================================\n");
    printf("           ROSTER TIMA: %s\n", trazeni_tim);
    printf("==================================================\n");

    for(int i = 0; i < br; i++)
    {
        printf("%-20s | %-5d | $%ld\n",
               roster[i].ime,
               roster[i].ovr,
               roster[i].cena);
    }
}

void ispisi_opcije() {
    printf("==================================================\n");
    printf("1. Odigraj utakmicu\n");
    printf("2. Pregled ekipe\n");
    printf("3. Prikaz tabele\n");
    printf("4. Transfer igraca\n");
    printf("5. Izlaz\n");
    printf("==================================================\n");
}

int izbor_meni() {
    int broj;
    printf("Unesi broj za izbor opcije: ");
    do {
        scanf("%d", &broj);
        if (broj < 1 || broj > 5)
            printf("Greska! Molimo unesite broj izmedju 1 i 5.\n\n");
    } while(broj < 1 || broj > 5);
    return broj;
}

void prikaziTabelu(Ekipa niz_ekipa[], int broj_timova) {
    system("cls");
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
                } else if (istok[j].porazi == istok[j + 1].porazi) {
                    if (strcmp(istok[j].ime, istok[j + 1].ime) > 0) {
                        treba_zamena = 1;
                    }
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
                } else if (zapad[j].porazi == zapad[j + 1].porazi) {
                    if (strcmp(zapad[j].ime, zapad[j + 1].ime) > 0) {
                        treba_zamena = 1;
                    }
                }
            }
            if (treba_zamena) {
                Ekipa temp = zapad[j];
                zapad[j] = zapad[j + 1];
                zapad[j + 1] = temp;
            }
        }
    }
    printf("\n=============================================\n");
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
void transferIgraca(Igrac svi_igraci[], int broj_igraca, char moj_tim[])
{
    int moji[20];
    int br_mojih = 0;

    system("cls");

    printf("=====================================\n");
    printf("          TRANSFER CENTAR\n");
    printf("=====================================\n\n");

    printf("Vasi igraci:\n\n");

    for(int i = 0; i < broj_igraca; i++)
    {
        if(strcmp(svi_igraci[i].ekipa, moj_tim) == 0)
        {
            printf("%d. %-20s OVR %d\n",
                   br_mojih + 1,
                   svi_igraci[i].ime,
                   svi_igraci[i].ovr);

            moji[br_mojih] = i;
            br_mojih++;
        }
    }

    int izbor;

    printf("\nIzaberite igraca za trade: ");
    scanf("%d",&izbor);

    izbor--;

    if(izbor < 0 || izbor >= br_mojih)
    {
        printf("Pogresan unos!\n");
        return;
    }

    int moj_idx = moji[izbor];

    int kandidati[200];
    int br_kandidata = 0;

    printf("\nMoguci tradeovi:\n\n");

    for(int i = 0; i < broj_igraca; i++)
    {
        if(strcmp(svi_igraci[i].ekipa, moj_tim) != 0)
        {
            if(abs(svi_igraci[i].ovr - svi_igraci[moj_idx].ovr) <= 2)
            {
                printf("%d. %-20s | %-25s | OVR %d\n",
                       br_kandidata + 1,
                       svi_igraci[i].ime,
                       svi_igraci[i].ekipa,
                       svi_igraci[i].ovr);

                kandidati[br_kandidata] = i;
                br_kandidata++;
            }
        }
    }

    if(br_kandidata == 0)
    {
        printf("Nema dostupnih tradeova!\n");
        return;
    }

    int izbor2;

    printf("\nIzaberi igraca za zamenu: ");
    scanf("%d",&izbor2);

    izbor2--;

    if(izbor2 < 0 || izbor2 >= br_kandidata)
    {
        printf("Pogresan unos!\n");
        return;
    }

    int drugi_idx = kandidati[izbor2];

    printf("\n=====================================\n");
    printf("PONUDA TRADEA\n");
    printf("=====================================\n");

    printf("%s (%d)\n",
           svi_igraci[moj_idx].ime,
           svi_igraci[moj_idx].ovr);

    printf("ZA\n");

    printf("%s (%d)\n",
           svi_igraci[drugi_idx].ime,
           svi_igraci[drugi_idx].ovr);

    int potvrda;

    printf("\n1 - Prihvati trade\n");
    printf("0 - Odbij trade\n");
    printf("Izbor: ");

    scanf("%d",&potvrda);

    if(potvrda == 1)
    {
        char tim1[50];
        char tim2[50];

        strcpy(tim1, svi_igraci[moj_idx].ekipa);
        strcpy(tim2, svi_igraci[drugi_idx].ekipa);

        strcpy(svi_igraci[moj_idx].ekipa, tim2);
        strcpy(svi_igraci[drugi_idx].ekipa, tim1);

        printf("\nTRADE USPESNO ZAVRSEN!\n");
    }
    else
    {
        printf("\nTrade odbijen.\n");
    }

    printf("\nPritisni Enter...");
    getchar();
    getchar();
}
void meni(Igrac svi_igraci[], int broj_igraca, char trazeni_tim[], Ekipa niz_ekipa[], int broj_timova, Utakmica raspored[], int *trenutni_mec) {
    int broj;
    do {
        system("cls");
        ispisi_opcije();
        broj = izbor_meni();
        switch(broj) {
        case 1:
            if (*trenutni_mec < 82) {
                simulirajJednuUtakmicu(svi_igraci, broj_igraca, trazeni_tim, raspored[*trenutni_mec], niz_ekipa, broj_timova, trenutni_mec);
            } else {
                printf("\nSezona je zavrsena! Odigrali ste sve 82 utakmice.\n");
                printf("Pritisnite Enter za povratak...");
                getchar(); getchar();
            }
            break;
        case 2:
            ispisiRoster(svi_igraci, broj_igraca, trazeni_tim);
            printf("\nPritisnite Enter za povratak u meni...");
            getchar(); getchar();
            break;
        case 3:
            prikaziTabelu(niz_ekipa, broj_timova);
            printf("\nPritisnite Enter za povratak u meni...");
            getchar(); getchar();
            break;
        case 4:
            transferIgraca(svi_igraci, broj_igraca, trazeni_tim);
            break;

        case 5:
            printf("Gasim igricu Hvala na igranju!\n");
            spavaj(2);
            exit(0);
        }
    } while(broj != 5);
}

int main() {
    srand(time(NULL));
    Igrac svi_igraci[1000];
    Ekipa niz_ekipa[30];
    Utakmica raspored[82];
    int trenutni_mec = 0;
    int broj_igraca = ucitajIgrace(svi_igraci, 1000);
    if (broj_igraca == -1) return 1;
    int broj_timova = ucitajTimove(niz_ekipa);
    if (broj_timova == -1) return 1;
    int konf = izborKonf();
    int start_index = (konf == 1) ? 15 : 0;
    printf("\n=== IZABERITE SVOJ TIM ===\n");
    for (int i = 0; i < 15; i++) {
        printf("%d. %s\n", i + 1, niz_ekipa[start_index + i].ime);
    }
    printf("==========================\n");
    int izabran_broj = izbor();
    int stvarni_indeks = start_index + (izabran_broj - 1);
    char* moj_tim = niz_ekipa[stvarni_indeks].ime;
    int brojac_maceva = 0;
    while(brojac_maceva < 82) {
        int nasumicni_indeks = rand() % broj_timova;
        if (strcmp(niz_ekipa[nasumicni_indeks].ime, moj_tim) != 0) {
            strcpy(raspored[brojac_maceva].protivnik, niz_ekipa[nasumicni_indeks].ime);
            raspored[brojac_maceva].da_li_sam_domacin = rand() % 2;
            brojac_maceva++;
        }
    }
    ispisiRoster(svi_igraci, broj_igraca, moj_tim);
    printf("Pritisni enter da pocnes: ");
    getchar(); getchar();
    meni(svi_igraci, broj_igraca, moj_tim, niz_ekipa, broj_timova, raspored, &trenutni_mec);
    return 0;
}
