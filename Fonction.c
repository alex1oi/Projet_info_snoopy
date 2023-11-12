#include <stdio.h>
#include "Fonction.h"
//---Exercice 1---

//IN : - /
//OUT: - Structure saisie par l'utilisateur
struct NombreRationel saisie(){
    struct NombreRationel nbr;
    printf("Veuillez saisir le numerateur :");
    scanf("%d",&nbr.numerateur);
    printf("\n Veuillez saisir le dénominateur :");
    scanf("%d",&nbr.denominateur);
    return nbr;
}
//IN : - Structure à afficher
//OUT: - Affichage de la structure sous forme d'une fraction
void affiche(struct NombreRationel var){
    printf("%d / %d\n",var.numerateur,var.denominateur);
}


//IN : - Les deux structures que l'on veut multiplier
//OUT: - La structure qui résulte de la multiplication
struct NombreRationel Muliplication(struct NombreRationel nbr1,struct NombreRationel nbr2){
    struct NombreRationel result;
    result.numerateur = nbr1.numerateur*nbr2.numerateur;
    result.denominateur = nbr1.denominateur*nbr2.denominateur;
    return result;
}


//IN : - Les deux structures que l'on veut additioner
//OUT: - La structure qui résulte de l'addition
struct NombreRationel Addition(struct NombreRationel nbr1,struct NombreRationel nbr2){
    struct NombreRationel result;
    result.numerateur = nbr1.numerateur*nbr2.denominateur + nbr2.numerateur*nbr1.denominateur;
    result.denominateur = nbr1.denominateur*nbr2.denominateur;
    return result;
}