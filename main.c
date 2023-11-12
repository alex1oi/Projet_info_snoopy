#include <stdio.h>
#include "Fonction.h" //pour inclure les sous-programmes de fonction.c
#define TailleMax 100

int main() {
    //---Exercice 1---
    /*
    struct NombreRationel nbr1,nbr2;
    nbr1 = saisie();
    nbr2 = saisie();
    printf("La multiplication des deux est :");
    affiche(Muliplication(nbr1,nbr2));
    printf("\nL'addition des deux est :");
    affiche(Addition(nbr1,nbr2));
     */

//---Exercice 2---
/*
    int tableau[TailleMax];
    int n,validation = 0;

    while(validation==0){//verifie que n<100 et si n>100, redemande de saisir n
        printf("Saisir le nombres d'entiers à mettre dans le tableau (maximum 100) :\n");
        scanf("%d",&n);
        if(n<100)
            validation = 1;//met fin a la boucle
    }

    for(int i = 0;i<n;i++){//répète n fois le programme
        printf("Saisir un entier :\n");
        scanf("%d",&tableau[i]);//saisie la valeur de l'utilisateur a l'emplacement i du tableau
    }
    int a =0;
    for(int j = 0;j<n;j++){ //boucle qui se répète n fois
        if(tableau[a] < tableau[j]){ //compare de la valeur la plus grande trouvée jusqu'a maintenant avec la valeur de la case d'après du tableau
            a = j; //prends la position de la prochaine case si elle est plus grande
        }
    }
    printf("Le plus grand entier du tableau est %d",tableau[a]);
    */

//---Exercice 3---
    int tableau[3][4];
    int tableau2[12];
    for(int i=0;i<3;i++){
        for(int j = 0;j<4;j++){
            printf("Saisir un entier");
            scanf("%d",&tableau[i][j]);
            tableau2[4*i+j] = tableau[i][j];
        }
    }
    printf("Affichage du tableau en 2 dimensions :\n");
    for(int k = 0;k<3; k++){
        for(int l=0;l<4;l++){
            printf("%d ",tableau[k][l]);
        }
        printf("\n");
    }
    printf("Affichage du tableau sur une dimension :\n");
    for(int m = 0;m<12;m++)
        printf("%d ",tableau2[m]);
    return 0;
}