
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void vypis_potrebnych_dajov(char potrebny_udaj[])
{
for(int j=0;potrebny_udaj[j]!='\n';j++) printf("%c",potrebny_udaj[j]);
printf(" ");
}

int vypis_celeho_suboru(FILE **smernik_na_subor) //argumentami funckie sú smerník na otvorený súbor, s ktorým ďalej následne pracujem a premenná, ktorá kontroluje, či som zavrel súbor
{
FILE *subor;
subor=fopen("autobazar.txt","r");
if(subor==NULL)
    {
    printf("Neotvoreny subor\n");
    return 0; //ukončí sa funkcia ak sa správne neotvoril súbor
    }

double cena;
int typ_auta,datum_predaja,testovac=0;
char meno[50],SPZ[10],c;
while((c=getc(subor))!=EOF)
    {
if(testovac==0) testovac=1; //písanie nový riadkov medzi záznamami
else printf("\n");
//výpis toho čo sa nachádza v súbore
ungetc(c,subor);
fgets(meno,50,subor);
printf("meno priezvisko: %s",meno);
fgets(SPZ,10,subor);
printf("SPZ: %s",SPZ);
fscanf(subor,"%d",&typ_auta);
printf("typ auta: %d",typ_auta);
fscanf(subor,"%lf",&cena);
printf("\ncena: %.2lf",cena);
fscanf(subor,"%d",&datum_predaja);
printf("\ndatum: %d\n",datum_predaja);

c=getc(subor); //toto načítavanie ta je kvôli koncom riadkov
getc(subor);
      }

*smernik_na_subor=subor;
return 1;
}


void odmena(int datum,FILE **smernik_na_subor)
{
rewind(*smernik_na_subor); //potrebujem smerník na súbor vrátiť na prvé písmeno súboru
double cena;
int typ_auta,datum_predaja;
char meno[50],SPZ[10],c;
while((c=getc(*smernik_na_subor))!=EOF) //načítavam všetky informácie z každého záznamu a naśledne ak je splnená podmienka tak ich vypíšem v požadovanom tvare
{

    ungetc(c,*smernik_na_subor);
    fgets(meno,50,*smernik_na_subor);
    fgets(SPZ,10,*smernik_na_subor);
    fscanf(*smernik_na_subor,"%d",&typ_auta);
    fscanf(*smernik_na_subor,"%lf",&cena);
    fscanf(*smernik_na_subor,"%d",&datum_predaja);
    c=getc(*smernik_na_subor);
    getc(*smernik_na_subor);
    if(datum-datum_predaja>10000) //požadovanom je rok 10000, čiže ak je to viac ako 10 000 vypíše sa odmena a kto ju má dostať
        {
        vypis_potrebnych_dajov(meno);
        vypis_potrebnych_dajov(SPZ);


        if(typ_auta==1) printf("%.2f\n",cena*0.023);
        else printf("%.2f\n",cena*0.051);

        }
    }
}

int vypis_SPZ(FILE **smernik_na_subor,char **pole)
{
rewind(*smernik_na_subor);
double cena;
int typ_auta,datum_predaja,pocitadlo=0;
char meno[50],SPZ[7],c,*nove_pole;
nove_pole=(char*)calloc(100,sizeof(char));
while((c=getc(*smernik_na_subor))!=EOF)
    {
ungetc(c,*smernik_na_subor);
fgets(meno,50,*smernik_na_subor);
fscanf(*smernik_na_subor,"%[^\n]s",SPZ);
fscanf(*smernik_na_subor,"%d",&typ_auta);
fscanf(*smernik_na_subor,"%lf",&cena);
fscanf(*smernik_na_subor,"%d",&datum_predaja);
c=getc(*smernik_na_subor);
getc(*smernik_na_subor);
strcat(nove_pole,SPZ);
pocitadlo++;
}
*pole=(char*)malloc(pocitadlo*8*sizeof(char)); //alokovanie pamäte v poli, do ktorého následne zapíšem požadované údaje, čiže ŠPZ, rovno ho vytvorí také veľké, ako má byť aby sa do neho zmestili všetky ŠPZ-tky
*pole=nove_pole; // prepis do poľa v maine
printf("\n");
return 1; // zmenenie stavu premenej, ktorá kontroluje vytvorenie poľa

}

void print_SPZ(char **pole)
{
int pocitadlo_znakov=0,prvy_interval=1;
char **smernik_na_prvu_poziciu_pola=*pole; //na konci aby som ďalej mohol pracovať s poľom, musím znova smerník na to pole nastaviť na prvú pozíciu
for(int i=0;**pole!='\0';i++)
    {
    pocitadlo_znakov++;
    if(pocitadlo_znakov%3==0 && prvy_interval==1){ //vypisovanie funguje na princípe 2 intervalov, jeden ráta medzere pred číslom a druhý za číslom a navzájom sa vypínajú a zapínajú
        printf(" ");
        if (pocitadlo_znakov==6) prvy_interval=0;
    }
    if(pocitadlo_znakov%8==0) {
            printf("\n");
            pocitadlo_znakov=1;
            prvy_interval=1;
    }
    printf("%c",(**pole)); //výpis daného znaku z poľa
    ++*pole;
    }
printf("\n");
*pole=smernik_na_prvu_poziciu_pola;
}

void histogram(char **pole)
{
int **smernik_na_prvu_poziciu_pola=*pole;
int histogram_p[26],max,index=0; //histogram mi ráta koľko sa tam aké písmeno obrajví preto je veĺkosti 26
for(int j=0;j<26;j++) //vynulovanie všetkých pozícii
    {
    histogram_p[j]=0;
    }
for(int i=0;**pole!='\0';i++)
    {
    if(**pole>='A' && **pole<='Z')
        {
        histogram_p[(**pole)-65]++; //na prvú pozíciu histogramu pôjde prvé písmeno abecedy a tak ďalej aź po z
        }
    ++*pole;
    }
max=histogram_p[0];
for(int k=0;k<26;k++) //for cyklus na výber najčastejšie sa objavujúcej hodnoty
    {
    if(histogram_p[k]>max)
        {
        max=histogram_p[k];
        index=k;
        }
    }
printf("%c %d",65+index,max);
printf("\n");
*pole=smernik_na_prvu_poziciu_pola;
}



void palindrom(char **pole)
{
int **smernik_na_prvu_poziciu_pola=*pole,index=1;
char SPZ[100];
for(int i=1;**pole!='\0';i++)
    {
    SPZ[index-1]=**pole;
    ++*pole;
    index++;
    if(i%7==0 && i!=0) //po každej 7-dmici, čiže SPZ skontroluje či je palindrom
        {
        index=1;
        int palindr=1;
        for(int j=0;j<6;j++) // prehľadáva pole od začiatku a od konca a porovnáva tie znaky, na začiatku predpokladám, že to palindrom je a ak je nejaký znak iný tak polindrom je 0
            {
            if(SPZ[j]!=SPZ[6-j]) palindr=0;
            }
        if(palindr) printf("%c%c\n",SPZ[0],SPZ[1]);
        // vyprazenie pola s SPZ
        for(int k=0;k<6;k++)
        {
        SPZ[k]=0;
        }
        }
    }
*pole=smernik_na_prvu_poziciu_pola;
}



int main()
{

int kontrola_otvoreneho_subor=0;
char *pole;
char vstup;
int datum;
int existencnik_pola=0;
FILE *smernik_na_subor=NULL;
while(1){ //bude to nacitavat stale a funkcia k to cele prerusi returnom 0
scanf("%c",&vstup); // zadávanie vstupu som vyriešil cez switch
switch(vstup)
{

case('v'):
    kontrola_otvoreneho_subor=vypis_celeho_suboru(&smernik_na_subor);
    break;

case ('o'):
    scanf("%d",&datum); // zadanie dátumu po príkaze o
    if(kontrola_otvoreneho_subor) odmena(datum,&smernik_na_subor);
    break;

case('n'):
    if(kontrola_otvoreneho_subor) existencnik_pola=vypis_SPZ(&smernik_na_subor,&pole);
    break;

case('s'):
    if(existencnik_pola==0) printf("Pole nie je vytvorene\n"); // ak pole nebolo vytvorené vypíše sa Pole nie je vytvorene a nesputí funkciu
    else print_SPZ(&pole);
    break;

case('m'):
    if(existencnik_pola==0) printf("Pole nie je vytvorene\n");
    else histogram(&pole);
    break;

case('p'):
    if(existencnik_pola==0) printf("Pole nie je vytvorene\n");
    else palindrom(&pole);
    break;

case('k'): // ak je otvorený súbor tak ho tento výber switchu zavrie a následne preruší progra
    if(kontrola_otvoreneho_subor==1) fclose(smernik_na_subor);
    return 0;
    break;

}
}
}
