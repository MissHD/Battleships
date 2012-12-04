//
//  main.c
//  Bataille Navale
//
//  Created by Héloïse Defize on 6/11/12.
//  Copyright (c) 2012 Héloïse Defize. All rights reserved.
//
//  Version : 2.2
//
//  -- Notes de version --
//  * 2.2 : Permettre de rejouer de suite si on vient d'entrer des coordonnées déjà jouée auparavant
//  * 2.1 : Amélioration résultat et affichage
//  * 2.0 : Quitter ou rejouer
//  * 1.5 : Optimisation
//  * 1.4 : Choix : joueur contre ordinateur / joueur contre joueur
//  * 1.3 : Alterner joueur/ordinateur et déterminer gagnant
//  * 1.2 : Pouvoir remplir aléatoirement sa grille
//  * 1.1 : Ordinateur joue contre joueur
//  * 1.0 : Joueur crée sa grille
//  * 0.3 : Joueur détermine le plateau de Ordinateur
//  * 0.2 : Optimisation avec fonction du point précédent
//  * 0.1 : Création de la grille de l'ordinateur aléatoirement
//
//  -- A faire --
//  * Interface graphique :)
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 10
#define NBRBAT 6

int jeu(int iNumJoueur, int iNature[2], int iGrilleJeuJ1[MAX][MAX], int iGrilleJ2[MAX][MAX], int iScore[2][NBRBAT+1], int iBateau[NBRBAT]);
int verifScore (int iNumJoueur, int iScore[2][NBRBAT+1], int iBateau[NBRBAT]);
int verifDirectionBateau( int iDirection, int iTaille, int iL, int iC);
int directionBateau (int iTaille, int iL, int iC);
int verifPlacementTouche (int iDirection, int iTaille, int iTabOrdi[MAX][MAX], int iL, int iC);
void placementBateau (int iTabOrdi[MAX][MAX],int iL, int iC, int iTaille, int iDirection, int iNumBat);
void placementBateauAuto (int iTabOrdi[MAX][MAX], int iTaille, int iNumBat);
void placementBateauJoueur (int iGrilleJ1[MAX][MAX], int iBateau[NBRBAT]);
void afficherGrille (int iGrille[MAX][MAX], int iCacher);
void toucheCouleRate (int iNumJoueur, int iScore[2][NBRBAT+1], int iBateau[NBRBAT], int iGrilleJ1[MAX][MAX], int iGrilleJeuJ2[MAX][MAX], int iL, int iC);






int main(void) //int argc, const char * argv[]
{
    int iGrilleJ1 [MAX][MAX] = {0}; // Grille Joueur 1
    int iGrilleJ2 [MAX][MAX] = {0}; // Grille ordinateur ou joueur 2
    int iGrilleJeuJ1 [MAX][MAX] = {0}; // Grille de jeu vide du joueur 2 ou de l'Ordinateur
    int iGrilleJeuJ2 [MAX][MAX] = {0}; // Grille de jeu vide du joueur 1
    
    int iBateau [NBRBAT] = {4,3,3,2,2,2};
    /*
     - 1 cuirassé : 4 cases
     - 2 croiseurs : 3 cases
     - 3 torpilleurs : 2 cases
    */
    
    int iScore [2][NBRBAT+1] = {0};
    /*
     ligne 0 : score du joueur 1
     ligne 1 : score du joueur 2 / ordinateur
    */
    
    int iNature [2]; // joueur ou ordinateur ? indice numéro du joueur
    
    int iI, iFin = 0;
    
    system("clear"); //effacer l'ecran
    printf("Bienvenue dans Bataille Navale \n\n");
    
	srand(time(NULL)); // initialisation de rand
    
    // initialise les messages (touché coulé raté) à vide
    iScore[0][NBRBAT] = -1;
    iScore[1][NBRBAT] = -1;
    
    // Déterminer les joueurs et les ordinateurs
    for (iI=0; iI<2; iI++) {
        printf("Joueur %d", iI+1);
        printf("\n 1. Humain");
        printf("\n 2. Ordinateur");
        do {
            printf("\nChoix : ");
            scanf("%d", &iNature[iI]);
        } while (iNature[iI]<1 || iNature[iI]>2);
        
        printf("\n");
    }
    
    // Création des grilles...
    
    // - Joueur 1
    
    if (iNature[0] == 2 ) { // ordinateur
        // Placer les bateaux de l'ordinateur
        for (iI=0; iI<NBRBAT; iI++) {
            placementBateauAuto(iGrilleJ1, iBateau[iI], iI+1);
        }
    } else { // humain
        system("clear"); //effacer l'ecran
        printf("Placement des bateaux du Joueur 1");
        placementBateauJoueur(iGrilleJ1, iBateau);
    }
    
    // - Joueur 2
    
    if (iNature[1] == 2 ) { // ordinateur
        // Placer les bateaux de l'ordinateur
        for (iI=0; iI<NBRBAT; iI++) {
            placementBateauAuto(iGrilleJ2, iBateau[iI], iI+1);
        }
    } else { // humain
        system("clear"); //effacer l'ecran
        printf("Placement des bateaux du Joueur 2");
        placementBateauJoueur(iGrilleJ2, iBateau);
    }
    
    printf("\n\n -- Que le jeu commence ! -- \n");
    
    while (iFin == 0) {
        
        // *** Joueur 1
        do {
            system("clear"); //effacer l'ecran
            iFin = jeu(0, iNature, iGrilleJeuJ1, iGrilleJ2, iScore, iBateau);
        } while (iScore[0][NBRBAT]==3);
        
        
        // *** joueur 2
        if (iFin==0) {
            do {
                system("clear"); //effacer l'ecran
                iFin = jeu(1, iNature, iGrilleJeuJ2, iGrilleJ1, iScore, iBateau);
            } while (iScore[1][NBRBAT]==3);
        }
        
    }
    
    printf("\nFin du jeu\n\n");
    
    printf("Recommencer ? 1. oui");
    scanf("%d",&iI);
    
    if (iI) {
        main();
    }
    
    return 0;
}






int jeu(int iNumJoueur, int iNature[2], int iGrilleJeuJ1[MAX][MAX], int iGrilleJ2[MAX][MAX], int iScore[2][NBRBAT+1], int iBateau[NBRBAT]) {

    int iL, iC, iFin;
    
    printf("\n -- Joueur %d -- ", iNumJoueur+1);
    
    // précédent résultat
    switch (iScore[iNumJoueur][NBRBAT]) {
        case -1:
            printf("\n\n\n");
            break;
        
        case 0:
            printf("\n\nPrécédent résultat : Raté !\n");
            break;
            
        case 1:
            printf("\n\nPrécédent résultat : Touché !\n");
            break;
            
        case 2:
            printf("\n\nPrécédent résultat : Touché-coulé !\n");
            break;
            
        case 3:
            printf("\n\nPrécédent résultat : Déjà joué...\n");
            break;
            
        default:
            break;
    }
    
    afficherGrille(iGrilleJeuJ1, 1);
    
    if (iNature[iNumJoueur] == 2 ) { // ordinateur
        
        // ordinateur - coordonnée aléatoire
        do {
            iL = rand()%MAX;
            iC = rand()%MAX;
        } while (iGrilleJeuJ1[iL][iC] != 0);
        
    } else { // Humain
        
        // Entrées de l'utilisateur avec vérification qu'elles sont entre 1 et MAX compris
        do {
            printf("  Sélectionnez une ligne : ");
            scanf("%d", &iL);
        } while (iL>MAX || iL<1);
        do {
            printf("Sélectionnez une colonne : ");
            scanf("%d", &iC);
        } while (iC>MAX || iC<1);
        
        iL--;
        iC--;
    }
    
    // vérification du touché raté coulé
    toucheCouleRate(iNumJoueur,iScore, iBateau, iGrilleJ2, iGrilleJeuJ1, iL, iC);
    
    // vérification des points du joueur 1
    iFin = verifScore(iNumJoueur, iScore, iBateau);
    
    if (iFin) {
        printf("\nJoueur %d gagne la partie.", iNumJoueur+1);
    }
    
    return iFin;

}

int verifScore (int iNumJoueur, int iScore[2][NBRBAT+1], int iBateau[NBRBAT]) {

    int iI, iFin = 0;
    
    // vérifier Score
    for (iI=0; iI<NBRBAT; iI++) {
        if (iScore[iNumJoueur][iI]==iBateau[iI]) {
            iFin = 1;
        } else {
            iFin = 0;
            iI = NBRBAT;
        }
    }
    
    return iFin;
}

void afficherGrille (int iGrille[MAX][MAX], int iCacher)
{
    int iL, iC;
    
    printf("\n");
    printf("    1 2 3 4 5 6 7 8 9 10\n");
    
    for (iL=0; iL<MAX; iL++) {
        
        //numéro de ligne
        if (iL<9) {
            printf(" %d  ",iL+1);
        } else {
            printf("%d  ",iL+1);
        }

        //grille de jeu
        for (iC=0; iC<MAX; iC++) {
            if (iGrille[iL][iC]==0) { // eau
                printf("~ ");
            } else if (iGrille[iL][iC]==-1) { // plouf
                printf("x "); // ✖
            } else {
                if (iCacher == 1) { // touché
                    printf("■ ");
                } else {
                    printf("%d ",iGrille[iL][iC]);
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

void toucheCouleRate (int iNumJoueur, int iScore[2][NBRBAT+1], int iBateau[NBRBAT], int iGrilleJ1[MAX][MAX], int iGrilleJeuJ2[MAX][MAX], int iL, int iC)
{
    // vérification du touché raté coulé
    if (iGrilleJeuJ2[iL][iC] == 0) {
        if (iGrilleJ1[iL][iC] == 0) {
            iScore[iNumJoueur][NBRBAT] = 0;
            //printf("\nRaté !\n");
            iGrilleJeuJ2[iL][iC] = -1;
        } else {
            
            iScore[iNumJoueur][NBRBAT] = 1;
            //printf("\nTouché");
            
            iScore[iNumJoueur][iGrilleJ1[iL][iC]-1]++; // à la position qui correspond au numéro du bateau touché
            iGrilleJeuJ2[iL][iC] = iGrilleJ1[iL][iC];
            
            if (iScore[iNumJoueur][iGrilleJ1[iL][iC]-1] == iBateau[iGrilleJ1[iL][iC]-1]) {
                iScore[iNumJoueur][NBRBAT] = 2;
                //printf("-coulé");
            }
            
            //printf(" !\n");
        }
    } else {
        iScore[iNumJoueur][NBRBAT] = 4;
        //printf("\nDéjà joué...\n");
    }
}

int directionBateau( int iTaille, int iL, int iC)
{
    int iDirection;
    
    iDirection = rand()%4;
    
    //vérifier que le sens permet de mettre le bateau en entier dans le tableau
    while (verifDirectionBateau(iDirection, iTaille, iL, iC) == 0) {
        iDirection = rand()%4;
    }
    
    return iDirection;
    
}

int verifDirectionBateau( int iDirection, int iTaille, int iL, int iC)
{
    /*
     0 : Haut
     1 : Droite
     2 : Bas
     3 : Gauche
     */
    
    iTaille--;
    
    //vérifier que le sens permet de mettre le bateau en entier dans le tableau
    if ((iDirection==0 && iL<iTaille) || (iDirection==1 && iC>MAX-1-iTaille) || (iDirection==2 && iL>MAX-1-iTaille) || (iDirection==3 && iC<iTaille)) {
        return 0; // Bof
    } else  {
        return 1; // OK
    }
}

int verifPlacementTouche (int iDirection, int iTaille, int iTab[MAX][MAX], int iL, int iC)
{
    // -- Vérification pas toucher un autre bateau
    
    int iI, iOk;
    iOk = 0;
    
    switch (iDirection) {
        case 0:
            for (iI=0; iI<iTaille; iI++) {
                if (iTab[iL-iI][iC]!=0) {
                    iI = MAX;
                    iOk = 0;
                } else {
                    iOk = 1;
                }
            }
            break;
            
        case 1:
            for (iI=0; iI<iTaille; iI++) {
                if (iTab[iL][iC+iI]!=0) {
                    iI = MAX;
                    iOk = 0;
                } else {
                    iOk = 1;
                }
            }
            break;
            
        case 2:
            for (iI=0; iI<iTaille; iI++) {
                if (iTab[iL+iI][iC]!=0) {
                    iI = MAX;
                    iOk = 0;
                } else {
                    iOk = 1;
                }
            }
            break;
            
        case 3:
            for (iI=0; iI<iTaille; iI++) {
                if (iTab[iL][iC-iI]!=0) {
                    iI = MAX;
                    iOk = 0;
                } else {
                    iOk = 1;
                }
            }
            break;
            
        default:
            break;
    }
    
    return iOk;
}

void placementBateauJoueur (int iGrilleJ1[MAX][MAX], int iBateau[NBRBAT])
{
    int iI;
    int iL, iC, iDir;
    
    // Placer les bateaux du joueur 1
    printf("\n\nPlacez vos bateaux.");
    printf("\n 1. Aléatoirement");
    printf("\n 2. Manuellement");
    
    do {
        printf("\nChoix : ");
        scanf("%d", &iI);
    } while (iI<1 || iI>2);
    
    if (iI == 1) {
        
        for (iI=0; iI<NBRBAT; iI++) {
            placementBateauAuto(iGrilleJ1, iBateau[iI], iI+1);
        }
        
    } else {
        
        for (iI=0; iI<NBRBAT; iI++) {
            system("clear"); //effacer l'ecran
            afficherGrille(iGrilleJ1, 0);
            printf("Placez le bateau à %d cases.", iBateau[iI]);
            
            do {
                printf("\n\nCoordonnées de début.");
                do {
                    printf("\n -   Ligne : ");
                    scanf("%d", &iL);
                } while (iL>MAX || iL<1);
                do {
                    printf(" - Colonne : ");
                    scanf("%d", &iC);
                } while (iC>MAX || iC<1);
                iL--;
                iC--;
                
            } while ((verifDirectionBateau(0, iBateau[iI], iL, iC) == 0 &&
                      verifDirectionBateau(1, iBateau[iI], iL, iC) == 0 &&
                      verifDirectionBateau(2, iBateau[iI], iL, iC) == 0 &&
                      verifDirectionBateau(3, iBateau[iI], iL, iC) == 0) ||
                     (verifPlacementTouche(0, iBateau[iI], iGrilleJ1, iL, iC) == 0 &&
                      verifPlacementTouche(1, iBateau[iI], iGrilleJ1, iL, iC) == 0 &&
                      verifPlacementTouche(2, iBateau[iI], iGrilleJ1, iL, iC) == 0 &&
                      verifPlacementTouche(3, iBateau[iI], iGrilleJ1, iL, iC) == 0));
            
            printf("\nDirection");
            if (verifDirectionBateau(0, iBateau[iI], iL, iC) && verifPlacementTouche(0, iBateau[iI], iGrilleJ1, iL, iC)) {
                printf("\n 1. Haut");
            }
            if (verifDirectionBateau(1, iBateau[iI], iL, iC) && verifPlacementTouche(1, iBateau[iI], iGrilleJ1, iL, iC)) {
                printf("\n 2. Droite");
            }
            if (verifDirectionBateau(2, iBateau[iI], iL, iC) && verifPlacementTouche(2, iBateau[iI], iGrilleJ1, iL, iC)) {
                printf("\n 3. Bas");
            }
            if (verifDirectionBateau(3, iBateau[iI], iL, iC) && verifPlacementTouche(3, iBateau[iI], iGrilleJ1, iL, iC)) {
                printf("\n 4. Gauche");
            }
            do {
                printf("\nChoix : ");
                scanf("%d", &iDir);
            } while (iDir<1 || iDir>4 || verifDirectionBateau(iDir-1, iBateau[iI], iL, iC) == 0 || verifPlacementTouche(iDir-1, iBateau[iI], iGrilleJ1, iL, iC) == 0);
            
            // placement...
            placementBateau(iGrilleJ1, iL, iC, iBateau[iI], iDir-1, iI+1);
            
        }
        
    }
    
    // Afficher tableau joueur 1 final
    afficherGrille(iGrilleJ1, 1);
}

void placementBateau (int iTabOrdi[MAX][MAX],int iL, int iC, int iTaille, int iDirection, int iNumBat)
{
    int iI;
    
    for (iI=0; iI<iTaille; iI++) {
        switch (iDirection) {
            case 0: //Haut
                iTabOrdi[iL-iI][iC] = iNumBat;
                break;
                
            case 1: //Droite
                iTabOrdi[iL][iC+iI] = iNumBat;
                break;
                
            case 2: //Bas
                iTabOrdi[iL+iI][iC] = iNumBat;
                break;
                
            case 3: //Gauche
                iTabOrdi[iL][iC-iI] = iNumBat;
                break;
                
            default:
                break;
        }
    }
}

void placementBateauAuto (int iTabOrdi[MAX][MAX], int iTaille, int iNumBat)
{
    int iDirection;
    int iC, iL;
    
    do {
        // -- Coordonnées de départ
        iL = rand()%MAX;
        iC = rand()%MAX;
        
        iDirection = directionBateau(iTaille,iL,iC);
    } while (verifPlacementTouche(iDirection, iTaille, iTabOrdi, iL, iC) == 0);
    
    // -- Placement
    placementBateau(iTabOrdi, iL, iC, iTaille, iDirection, iNumBat);
}