#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib.h"
#include <stdbool.h>

/*************************************|
|                                     |
|         FICHIER TYPE TOVnC          |
|                                     |
|*************************************/
/*************************************************************|
|                                                             |
| Ouvrir fichier nom_fichier avec le mode correspondant TOVnC |
|                                                             |
|*************************************************************/
void Ouvrir_TOVnC(fichier_TOVnC *f, char nom_fichier[], char mode_ouverture)
{
    /// si on ouvre en mode nouveau
    if (tolower(mode_ouverture) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        if (f->fichier != NULL)
        {
            /// initialisation de l'entête :
            f->entete.nombre_bloc = 0;
            f->entete.nbr_caract_insert = 0;
            f->entete.nbr_caract_supp = 0;

            rewind(f->fichier); /// positionnement au debut du fichier
            fwrite(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    /// si on ouvre en mode ancien
    else if (tolower(mode_ouverture) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier != NULL)
        {
            rewind(f->fichier);
            fread(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/*************************************|
|                                     |
|     Fermer le fichier TOVnC         |
|                                     |
|*************************************/
void Fermer_TOVnC(fichier_TOVnC *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
    fclose(f->fichier);
}

/********************************************|
|                                            |
|    Lire le i eme bloc dans buf TOVnC       |
|                                            |
|********************************************/
void LireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC *buf)
{
    /// on se jitionne au debut du i ème bloc puis on le lit dans buf
    fseek(f->fichier, sizeof(entete_TOVnC) + (sizeof(Tbloc_TOVnC) * (i - 1)), SEEK_SET);
    fread(buf, sizeof(Tbloc_TOVnC), 1, f->fichier);
}

/**********************************************|
|                                              |
|  Retoure la i ème valeur del'entete  TOVnC   |
|                                              |
|**********************************************/
int Entete_TOVnC(fichier_TOVnC *f, int i)
{
    if (i == 1)
        return (f->entete.nombre_bloc);
    else if (i == 2)
        return (f->entete.nbr_caract_insert);
    else if (i == 3)
        return (f->entete.nbr_caract_supp);
    else
    {
        printf("Parametre inexistant dans l'entete\n");
        return -1;
    }
}

/*********************************************|
|                                             |
|    Ecrire buf dans le i eme bloc TOVnC      |
|                                             |
|*********************************************/
void EcrireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC buf)
{
    /// on se positionne au debut du i ème bloc puis on ecrit dans fichier->f
    fseek(f->fichier, sizeof(entete_TOVnC) + (sizeof(Tbloc_TOVnC) * (i - 1)), SEEK_SET);
    fwrite(&buf, sizeof(Tbloc_TOVnC), 1, f->fichier);
}

/***********************************************|
|                                               |
|  modifie la i ème valeur de l'entete  TOVnC   |
|                                               |
|***********************************************/
void Aff_Entete_TOVnC(fichier_TOVnC *f, int i, int val)
{
    if (i == 1)
        f->entete.nombre_bloc = val;
    else if (i == 2)
        f->entete.nbr_caract_insert = val;
    else if (i == 3)
        f->entete.nbr_caract_supp = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/**********************************************|
|                                              |
|   Retourne le numero du nouveau bloc TOVnC   |
|                                              |
|**********************************************/
int Alloc_bloc_TOVnC(fichier_TOVnC *f)
{
    int i = Entete_TOVnC(f, 1);    // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOVnC(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return Entete_TOVnC(f, 1);     // le nombre anciens de bloc = numero du dernier bloc
}

/**************************************************|
|                                                  |
|    afficher les caracteristqiues (entete) d'un   |
|       fichier "nom fichier" de type TOVnC        |
|                                                  |
|**************************************************/
void affichage_entete_TOVnC(char nom_fichier[])
{
    fichier_TOVnC f;
    Ouvrir_TOVnC(&f, nom_fichier, 'N');
    printf("\n\n\n*************************************************\n");
    printf("*                                               *\n");
    printf("*       caracteristiques du fichier             *\n");
    printf("*                                               *\n");
    printf("*************************************************\n\n\n");
    printf(" -> Nombre de Blocs : %d\n", Entete_TOVnC(&f, 1));
    printf(" -> Nombre caracteres inseres : %d\n", Entete_TOVnC(&f, 2));
    printf(" -> Nombre caracteres supprimes : %d\n", Entete_TOVnC(&f, 3));
}

/**************************************************|
|                                                  |
|    extraire une chaine de taille "taille" dans   |
|      la chaine destination a partir pos j        |
|                                                  |
|**************************************************/
void extraire_chaine_TOVnC(char destination[], int *j, int taille, Tampon_TOVnC *Buf)
{
    int k = 0;
    sprintf(destination, "%s", "");
    for (k = 0; k < taille; k++)             // boucle de taille iterations correspondant a la longueur de la chaine
    {                                        // indice de parcours de la chaine resultata et j celui du tableau                                             // le caractere 99 correspond a la fin de la chaine dans le tableau
        destination[k] = Buf->tableau[(*j)]; // recuperation du caractere dans la position k de la chaine
        (*j)++;                              // deplacement d'une position dans le buffer
    }                                        // fin de boucle
    destination[k] = '\0';                   // fin de la chaine obtenue
}

/**********************************************|
|                                              |
|       affichier le contenu d'un fichier      |
|                de type TOVnC                 |
|                                              |
|**********************************************/
void afficher_fichier_TOVnC(char nom_fichier[])
{
    fichier_TOVnC f;
    Ouvrir_TOVnC(&f, nom_fichier, 'A');
    int i = 1,                                   // parcours bloc par bloc
        j = 0,                                   // parcours de position dans le bloc
        counter = 0;                             // numero de l'enregistrement dans le bloc
    Tampon_TOVnC Buf;                            // contenu d'un bloc dans un buffer
    char Identifiant[TAILLE_IDENTIFIANT + 1],    // numero d'identifiant(cle)
        Supprime[TAILLE_SUPPRIMER + 1],          // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = "f", le materiel marche, fonctionne = "n" sinon
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel

    /**************************************************************************************************************|
    | Identifiant | champs supprime | Type materiel | fonctionne |    Prix   |   taille   | Description (variable) |
    |  (5 bytes)  |   (1 bytes)     |  (12 bytes)   |  (1 bytes) | (6 bytes) |  (3 bytes) |  (max sur 272 bytes)   |
    |**************************************************************************************************************/
    while (i <= Entete_TOVnC(&f, 1))
    {
        printf("\n\n\n*************************************************\n");
        printf("*                                               *\n");
        printf("*            Le bloc numero : %i                 *\n", i);
        printf("*                                               *\n");
        printf("*************************************************\n");

        LireDir_TOVnC(&f, i, &Buf);
        while (j < Buf.nb)
        {
            extraire_chaine_TOVnC(Identifiant, &j, TAILLE_IDENTIFIANT, &Buf);
            extraire_chaine_TOVnC(Supprime, &j, TAILLE_SUPPRIMER, &Buf);
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL - 1, &Buf);
            extraire_chaine_TOVnC(Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf);
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);

            if (strcmp(Supprime, "f") == 0)
            {
                printf("\n\n.........................\n");
                printf(".                       .\n");
                printf(".  Materiel numero : %i  .\n", counter);
                printf(".                       .\n");
                printf(".........................\n");
                printf("identifiant: %.5s\n", Identifiant);
                printf("materiel: %.11s\n", Materiel);
                if (strcmp(Fonctionne, "f") == 0)
                    printf("Fonctionne? : OUI\n");
                else
                    printf("Fonctionne? : NON\n");
                printf("prix: %.6s\n", Prix);
                printf("taille de description: %.3s\n", Taille);
                printf("description: %s\n", Description);
                counter++;
            }
        }
        j = 0;
        i++;
    }
}

/*














*/
/*************************************|
|                                     |
|           FICHIER TYPE TOF          |
|                                     |
|*************************************/
/************************************************************|
|                                                            |
|  Ouvrir fichier nom_fichier avec le mode correspondant TOF |
|                                                            |
|************************************************************/
void Ouvrir_TOF(fichier_TOF *f, char nom_fichier[], char mode)
{
    if (tolower(mode) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier == NULL)
            printf("Erreur lors de l'ouverture du fichier... verifier le nom du fichier");
        else
            fread(&(f->entete), sizeof(entete_TOF), 1, f->fichier);
    }
    else if (tolower(mode) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        Aff_Entete_TOF(f, 1, 0); // mettre le nombre de blocs à 0
        Aff_Entete_TOF(f, 2, 0); // mettre le nombre d'enregistrements inseres à 0
        Aff_Entete_TOF(f, 3, 0); // mettre le nombre d'enregistrements supprimes à 0
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/*************************************|
|                                     |
|       Fermer le fichier TOF         |
|                                     |
|*************************************/
void Fermer_TOF(fichier_TOF f)
{
    fseek(f.fichier, 0, SEEK_SET);
    fwrite(&(f.entete), sizeof(entete_TOF), 1, f.fichier);
    fclose(f.fichier);
}

/********************************************|
|                                            |
|      Lire le i eme bloc dans buf TOF       |
|                                            |
|********************************************/
void LireDir_TOF(fichier_TOF f, int i, Tampon_TOF *buf, int *cpt_lect)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(fichier_TOF) + (i) * sizeof(Tampon_TOF), SEEK_SET);
    fread(buf, sizeof(Tampon_TOF), 1, f.fichier);
    (*cpt_lect) = (*cpt_lect) + 1;
}

/*********************************************|
|                                             |
|      Ecrire buf dans le i eme bloc TOF      |
|                                             |
|*********************************************/
void EcrireDir_TOF(fichier_TOF f, int i, Tampon_TOF *buf, int *cpt_ecr)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(entete_TOF) + (i) * sizeof(Tampon_TOF), SEEK_SET);
    fwrite(buf, sizeof(Tampon_TOF), 1, f.fichier);
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

/********************************************|
|                                            |
|  Retoure la i ème valeur del'entete  TOF   |
|                                            |
|********************************************/
int Entete_TOF(fichier_TOF f, int i)
{
    if (i == 1) // nombre de blocs total
        return f.entete.blocs_total;
    else if (i == 2) // nombre d'enregistrements inseres
        return f.entete.enreg_inseres;
    else if (i == 3) // nombre d'enregistrements supprimes
        return f.entete.enreg_supprimes;
    else
        return -1;
}

/*********************************************|
|                                             |
|  modifie la i ème valeur de l'entete  TOF   |
|                                             |
|*********************************************/
void Aff_Entete_TOF(fichier_TOF *f, int i, int val)
{
    if (i == 1) // nombre de blocs total
        f->entete.blocs_total = val;
    else if (i == 2) // nombre d'enregistrements inseres
        f->entete.enreg_inseres = val;
    else if (i == 3) // nombre d'enregistrements supprimes
        f->entete.enreg_supprimes = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/********************************************|
|                                            |
|   Retourne le numero du nouveau bloc TOF   |
|                                            |
|********************************************/
int Alloc_bloc_TOF(fichier_TOF *f)
{
    int i = Entete_TOF(*f, 1);   // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOF(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return Entete_TOF(*f, 1);    // le nombre anciens de bloc = numero du dernier bloc
}

/*














*/
/*************************************|
|                                     |
|           FICHIER TYPE TOVC         |
|                                     |
|*************************************/
/*************************************************************|
|                                                             |
|  Ouvrir fichier nom_fichier avec le mode correspondant TOVC |
|                                                             |
|*************************************************************/
void Ouvrir_TOVC(fichier_TOVC *f, char nom_fichier[], char mode_ouverture)
{
    /// si on ouvre en mode nouveau
    if (tolower(mode_ouverture) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        if (f->fichier != NULL)
        {
            /// initialisation de l'entête :
            f->entete.adr_dernier_bloc = 0;
            f->entete.pos_libre_dernier_bloc = 0;
            f->entete.nbr_caract_insert = 0;
            f->entete.nbr_caract_supp = 0;

            rewind(f->fichier); /// positionnement au debut du fichier
            fwrite(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    /// si on ouvre en mode ancien
    else if (tolower(mode_ouverture) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier != NULL)
        {
            rewind(f->fichier);
            fread(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/*************************************|
|                                     |
|      Fermer le fichier TOVC         |
|                                     |
|*************************************/
void Fermer_TOVC(fichier_TOVC *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}

/********************************************|
|                                            |
|     Lire le i eme bloc dans buf TOVC       |
|                                            |
|********************************************/
void LireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC *buf)
{
    /// on se jitionne au debut du i ème bloc puis on le lit dans buf
    fseek(f->fichier, sizeof(entete_TOVC) + (sizeof(Tbloc_TOVC) * (i - 1)), SEEK_SET);
    fread(buf, sizeof(Tbloc_TOVC), 1, f->fichier);
}

/********************************************|
|                                            |
| Retoure la i ème valeur del'entete  TOVC   |
|                                            |
|********************************************/
int Entete_TOVC(fichier_TOVC *f, int i)
{
    if (i == 1)
        return (f->entete.adr_dernier_bloc);
    else if (i == 2)
        return (f->entete.pos_libre_dernier_bloc);
    else if (i == 3)
        return (f->entete.nbr_caract_insert);
    else if (i == 4)
        return (f->entete.nbr_caract_supp);
    else
        printf("Parametre inexistant dans l'entete\n");
}

/*********************************************|
|                                             |
|     Ecrire buf dans le i eme bloc TOVC      |
|                                             |
|*********************************************/
void EcrireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC buf)
{
    /// on se positionne au debut du i ème bloc puis on ecrit dans fichier->f
    fseek(f->fichier, sizeof(entete_TOVC) + (sizeof(Tbloc_TOVC) * (i - 1)), SEEK_SET);
    fwrite(&buf, sizeof(Tbloc_TOVC), 1, f->fichier);
}

/*********************************************|
|                                             |
| modifie la i ème valeur de l'entete  TOVC   |
|                                             |
|*********************************************/
void Aff_Entete_TOVC(fichier_TOVC *f, int i, int val)
{
    if (i == 1)
        f->entete.adr_dernier_bloc = val;
    else if (i == 2)
        f->entete.pos_libre_dernier_bloc = val;
    else if (i == 3)
        f->entete.nbr_caract_insert = val;
    else if (i == 4)
        f->entete.nbr_caract_supp = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/********************************************|
|                                            |
|  Retourne le numero du nouveau bloc TOVC   |
|                                            |
|********************************************/
int Alloc_bloc_TOVC(fichier_TOVC *f)
{
    int i = Entete_TOVC(f, 1);    // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOVC(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return Entete_TOVC(f, 1);     // le nombre anciens de bloc = numero du dernier bloc
}

/*














*/
/*************************************|
|                                     |
|           FICHIER TYPE LOVC         |
|                                     |
|*************************************/
/*************************************************************|
|                                                             |
|  Ouvrir fichier nom_fichier avec le mode correspondant LOVC |
|                                                           , |
|*************************************************************/
void Ouvrir_LOVC(fichier_LOVC *f, const char nomFichier[], char mode)
{
    f = malloc(sizeof(fichier_LOVC)); // allocation  de la structure

    if (tolower(mode) == 'a') // mode ancien
    {
        f->fichier = fopen(nomFichier, "rb+");
        if (f->fichier == NULL)
            printf("error interrupted the program ... check file's name");
        else
            fread(&(f->entete), sizeof(Entete_LOVC), 1, f->fichier); // chargement de l'entete
    }

    else if (tolower(mode) == 'n')
    {
        f->fichier = fopen(nomFichier, "wb+");
        aff_entete_LOVC(f, 1, -1); // mise a NIL de l'adresse du premier bloc tant qu'il est vide
        aff_entete_LOVC(f, 2, -1); // metter l'adresse du dernier bloc a NIL tant que le fichier est vide
        aff_entete_LOVC(f, 3, 0);  // initialiser le nombre de caractères inseres a 0
        aff_entete_LOVC(f, 4, 0);  // initialiser le nombre de caractères supprimes a 0
    }

    else // mode d'ouverture incorrecte
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/**************************************|
|                                      |
|       Fermer le fichier LOVC         |
|                                      |
|**************************************/
void fermer_LOVC(fichier_LOVC *f)
{
    rewind(f->fichier);                                       // repositionnement du curseur en debut de fichier
    fwrite(&(f->entete), sizeof(Entete_LOVC), 1, f->fichier); // sauvegarde l'entete en debut du fichier
    fclose(f->fichier);                                       // fermeture du fichier
    free(f);                                                  // libere l'espace memoire occupe par la structure
}

/*********************************************|
|                                             |
|      Lire le i eme bloc dans buf LOVC       |
|                                         ,   |
|*********************************************/
void LireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf, int *cpt_lect)
{
    // positionnement au debut du bloc numero i
    fseek(f->fichier, (sizeof(Entete_LOVC) + sizeof(Tbloc_LOVC) * (i - 1)), SEEK_SET);
    // lecture d'un bloc de caractere correspondant a la taille du bloc dans le buffer
    fread(buf, sizeof(Tampon_LOVC), 1, f->fichier);
    (*cpt_lect) = (*cpt_lect) + 1;
}

/**********************************************|
|                                              |
|      Ecrire buf dans le i eme bloc LOVC      |
|                                              |
|*********************************************/
void ecrireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf, int *cpt_ecr)
{

    // positionnement au debut du bloc numero i
    fseek(f->fichier, sizeof(Entete_LOVC) + sizeof(Tbloc_LOVC) * (i - 1), SEEK_SET);
    // ecriture du contenu du buffer dans le bloc numero i du fichier
    fwrite(buf, sizeof(Tampon_LOVC), 1, f->fichier);
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

/*********************************************|
|                                             |
|  Retoure la i ème valeur del'entete  LOVC   |
|                                             |
|*********************************************/
int entete_LOVC(fichier_LOVC *f, int i)
{
    if (i == 1)
        return (f->entete.Num_premier_bloc); // l'adresse du premier bloc
    else if (i == 2)
        return (f->entete.Num_dernier_bloc);
    else if (i == 3)
        return (f->entete.pos_libre_dernier_bloc); // la position libre dans le dernier bloc
    else if (i == 4)
        return (f->entete.nb_chars_inseres); // numero du bloc representatnt la tete du fichier
    else if (i == 5)
        return (f->entete.nb_chars_supprimes); // nombre de caracteres inseres (effaces non inclus)
    else
    {
        printf("Parametre inexistant dans l'entete\n");
        return -1;
    }
}

/**********************************************|
|                                              |
|  modifie la i ème valeur de l'entete  LOVC   |
|                                              |
|**********************************************/
void aff_entete_LOVC(fichier_LOVC *f, int i, int val)
{
    if (i == 1)
        f->entete.Num_premier_bloc = val; // le numero du premier bloc
    else if (i == 2)
        f->entete.Num_dernier_bloc = val; // le numero du dernier bloc
    else if (i == 3)
        f->entete.pos_libre_dernier_bloc = val; // la position libre dans le denier bloc
    else if (i == 4)
        f->entete.nb_chars_inseres = val; // nombre de caractères inseres dans le fichier
    else if (i == 5)
        f->entete.nb_chars_supprimes = val; // nombre de caractères supprimes dans le fichier
    else
        printf("Parametre inexistant dans l'entete\n");
}

/*********************************************|
|                                             |
|   Retourne le numero du nouveau bloc LOVC   |
|                                             |
|*********************************************/
int alloc_bloc_LOVC(fichier_LOVC *fichier, int *cpt_lect, int *cpt_ecr, Tampon_LOVC *buf)
{
    LireDir_LOVC(fichier, entete_LOVC(fichier, 2), buf, cpt_lect);  // lecture du bloc correspondant a la queue
    buf->suivant = entete_LOVC(fichier, 1) + 1;                     // mise a jour dui suivant de la queue au bloc correspondant a la nouvelle queue
    ecrireDir_LOVC(fichier, entete_LOVC(fichier, 2), buf, cpt_ecr); // ecriture du bloc de queue dans le fichier
    aff_entete_LOVC(fichier, 2, entete_LOVC(fichier, 1) + 1);       // mise a jour du numero du bloc correspondant a la nouvelle queue dan sl'entete
    buf->suivant = -1;                                              // mise a jour du suivant a nill
    sprintf(buf->tab, "%s", "");                                    // vider la chaine du buffer
    return entete_LOVC(fichier, 2);
}
/*
























/**********************************************|
|                                              |
|            FUNCTIONS USED IN TP              |
|                                              |
|**********************************************/
/**********************************************|
|                                              |
|     Generer un nombre aleatoire qui se       |
|   trouve entre les bornes [lower, upper]     |
|                                              |
|**********************************************/
int Random_Number(int lower, int upper)
{
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

/**********************************************|
|                                              |
|     Generer une chaine de longueur length    |
|         a partir d'un entier number          |
|                                              |
|**********************************************/
void Generer_Chaine(char chaine[], int length, int number)
{
    int i;
    for (i = length - 1; i >= 0; i--)
    {
        chaine[i] = number % 10 + '0';
        number = number / 10;
    }
    chaine[length] = '\0';
}

/**********************************************|
|                                              |
|      Concatener tous les champs dans         |
|       une seule chaine destination           |
|                                              |
|**********************************************/
void concatenate(char *destination, char *identifiant, char *supprime, char *materiel, char *fonctionne, char *prix, char *taille, char *description)
{
    /**************************************************************************************************************|
    | Identifiant | champs supprime | Type materiel | fonctionne |    Prix   |   taille   | Description (variable) |
    |  (5 bytes)  |   (1 bytes)     |  (12 bytes)   |  (1 bytes) | (6 bytes) |  (3 bytes) |  (max sur 272 bytes)   |
    |**************************************************************************************************************/
    sprintf(destination, "%s", "");                          // vider le tableau des caractères
    strncat(destination, identifiant, TAILLE_IDENTIFIANT);   // destination+=identifiant
    strncat(destination, supprime, TAILLE_SUPPRIMER);        // destination+=supprime
    strncat(destination, materiel, TAILLE_MATERIEL);         // destination+=materiel
    strncat(destination, fonctionne, TAILLE_FONCTIONNEMENT); // destination+=fonctionne
    strncat(destination, prix, TAILLE_PRIX);                 // destination+=prix
    strncat(destination, taille, TAILLE_TAILLE);             // destination+=taille
    strcat(destination, description);                        // destination+=description
}

/**********************************************|
|                                              |
| Ecrire la chaine d'identifiant cle a partir  |
|       du bloc i a la pos j dans Buf          |
|                                              |
|**********************************************/
void Ecrire_chaine_TOVnC(fichier_TOVnC *F, char chaine[], char cle[], int *i, int *j, Tampon_TOVnC *Buf)
{
    if (*j + strlen(chaine) > B) // inserer le nouvel element dans un nouveau bloc
    {
        EcrireDir_TOVnC(F, *i, *Buf);                     // Ecrire le bloc courant pour passer au prochain
        printf("\nNouveau bloc a ete cree");              // la creation d'un nouveau bloc
        *i = Alloc_bloc_TOVnC(F);                         // passer au nouveau bloc
        *j = 0;                                           // revenir a la premiere position dans le nouveau bloc
        memset(Buf->tableau, '\0', sizeof(Buf->tableau)); // vider la chaine de caracteres
    }
    strcat(Buf->tableau, chaine);                                // mise a jour du bloc: inserer la chaine de caractere
    Aff_Entete_TOVnC(F, 2, Entete_TOVnC(F, 2) + strlen(chaine)); // mise a jour de l'entere: nombre de caracteres inseres
    *j = *j + strlen(chaine);                                    // mise a jour du deplacement libre du buffer
    Buf->nb = *j;                                                // mise a jour du bloc: la permiere pos libre
    strcpy(Buf->cleMax, cle);                                    // mise a jour du bloc: la cle max
}

/*








*/
/**********************************************|
|                                              |
|       initialiser fichier nom_fichier        |
|        de type TOVnC avec n valeurs          |
|                                              |
|**********************************************/
void Chargement_initial_TOVnC(char nom_fichier[], int n)
{
    fichier_TOVnC *F;
    Ouvrir_TOVnC(F, nom_fichier, 'N');
    Tampon_TOVnC buf;
    memset(buf.tableau, '\0', sizeof(buf.tableau));  // vider le tableau des caractères
    int i = Alloc_bloc_TOVnC(F),                     // le numero du bloc pour le parcours entre bloc
        j = 0,                                       // la position dans le bloc pour le parcours interbloc
        k,                                           // le numero de l'element insere de 1 a n
        l;                                           // la longueur total de l'enregistrement ajoute
    char Identifiant[TAILLE_IDENTIFIANT + 1],        // numero d'identifiant(cle)
        Supprime[TAILLE_SUPPRIMER + 1] = "f",        // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL + 1],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1] = "f", // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX + 1],                       // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],                   // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1];     // la description (caracteristiques) du materiel

    for (k = 0; k < n; k++)
    {                                                                            // l'identifiant= multiple de 10 pour garder l'ordre et possibilite d'insertion
        Generer_Chaine(Identifiant, TAILLE_IDENTIFIANT, 10 * k);                 // generer l'identifiant sous forme de chaine sur 5 positions
        strcpy(Materiel, MATERIAL_LIST[Random_Number(0, NB_TYPE_MATERIEL - 1)]); // tirer un materiel de la liste  des materiels selon index genere aleartoirement
        Generer_Chaine(Prix, TAILLE_PRIX, Random_Number(0, PRIX_MAX));           // generer le prix du materiel aleartoirement

        printf("\n\n.........................\n");
        printf(".                       .\n");
        printf(". element numero : %i    .\n", k);
        printf(".                       .\n");
        printf(".........................\n");
        printf("identifiant: %.5s\n", Identifiant);
        printf("materiel: %.11s\n", Materiel);
        printf("prix: %.6s\n", Prix);

        printf("Description de votre materiel: ");                  // demander la description du materiel de l'utilisateur
        scanf(" %[^\n]", Description);                              // Lire la description de l'utilisateur
        Generer_Chaine(Taille, TAILLE_TAILLE, strlen(Description)); // taille du champs de la description
        printf("taille de description: %.3s\n", Taille);
        printf("description: %s\n", Description);

        l = TAILLE_IDENTIFIANT + TAILLE_SUPPRIMER + TAILLE_MATERIEL + TAILLE_FONCTIONNEMENT + TAILLE_PRIX + TAILLE_TAILLE + strlen(Description);
        char Enreg[l];
        concatenate(Enreg, Identifiant, Supprime, Materiel, Fonctionne, Prix, Taille, Description);
        printf("l'element sera insere sous cette forme: %s\n", Enreg);
        Ecrire_chaine_TOVnC(F, Enreg, Identifiant, &i, &j, &buf);
        if (strcmp(Fonctionne, "f") == 0)
            strcpy(Fonctionne, "n");
        else
            strcpy(Fonctionne, "f");
    }
    EcrireDir_TOVnC(F, i, buf);
    Fermer_TOVnC(F);
}

/****************************************************|
|                                                    |
|   Recherche dun materiel selon son identifiant     |
|                                                    |
|****************************************************/
void Recheche_TOVnC(char nom_fichier[], char Identifiant_Recherche[], int *trouv, int *i, int *j)
{
    fichier_TOVnC f;
    Ouvrir_TOVnC(&f, nom_fichier, 'A');
    int binf = 1,                                // le premier bloc
        bsup = Entete_TOVnC(&f, 1),              // le dernier bloc
        temp_j,                                  // sauvegarder la pos dans le bloc avant de se deplacer
        stop = 0;                                // booleen pour arreter la recherche si on arrive a une cle superieure a la cle recherche
    char Cle_Min[TAILLE_IDENTIFIANT + 1],        // la plus petite cle dans un bloc (plus petit identifiant)
        Cle_Max[TAILLE_IDENTIFIANT + 1],         // la plus grande cle dans un bloc (plus grand identifiant)
        Cle_Courrante[TAILLE_IDENTIFIANT + 1],   // la cle courrant dont on s'est arrete dans le parcours
        Identifiant[TAILLE_IDENTIFIANT + 1],     // numero d'identifiant(cle)
        Supprime[TAILLE_SUPPRIMER + 1],          // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel
    Tbloc_TOVnC Buf;                             // un buffer pour charger un bloc de MS vers MC
    *trouv = 0;                                  // initialiser trouv vers faux
    while (!(*trouv) && binf <= bsup && !stop)   // enregistrement non trouve et recherche possible
    {
        *i = (binf + bsup) / 2;                                                                           // numero de bloc a parcourir
        *j = 0;                                                                                           // la premiere position dans le bloc
        LireDir_TOVnC(&f, *i, &Buf);                                                                      // lire le buffer
        temp_j = *j;                                                                                      // sauvegarder j avant de se deplacer
        extraire_chaine_TOVnC(Cle_Min, j, TAILLE_IDENTIFIANT, &Buf);                                      // extraire la plus petite cle (premiere cle) de du bloc i
        strcpy(Cle_Courrante, Cle_Min);                                                                   // mettre a jour la cle courrante
        strcpy(Cle_Max, Buf.cleMax);                                                                      // lire la cle max qui est dans le tableau
        if (atoi(Identifiant_Recherche) >= atoi(Cle_Min) && atoi(Identifiant_Recherche) <= atoi(Cle_Max)) // si la cle à recherchee est entre Cle_Min et Cle_Max du bloc on fait le recherche sequentielle dans le bloc
        {
            while (!(*trouv) && *j < Buf.nb && !stop)
            {
                extraire_chaine_TOVnC(Supprime, j, TAILLE_SUPPRIMER, &Buf);                          // recuperer un le champs de suppression
                if (strcmp(Identifiant_Recherche, Cle_Courrante) == 0 && strcmp(Supprime, "f") == 0) // la cle st donc trouveee dans le bloc i
                {
                    extraire_chaine_TOVnC(Materiel, j, TAILLE_MATERIEL - 1, &Buf);
                    extraire_chaine_TOVnC(Fonctionne, j, TAILLE_FONCTIONNEMENT, &Buf);
                    extraire_chaine_TOVnC(Prix, j, TAILLE_PRIX, &Buf);
                    extraire_chaine_TOVnC(Taille, j, TAILLE_TAILLE, &Buf);
                    extraire_chaine_TOVnC(Description, j, atoi(Taille), &Buf);
                    *trouv = 1;
                    *j = temp_j;
                    printf("\n\n---------------- L'enregistrement trouve dans le bloc %d a la position %d ------------------\n", *i, *j);
                    printf("|    -> L'identifiant : %s\n", Cle_Courrante);
                    printf("|    -> Le type materiel : %s\n", Materiel);
                    if (strcmp(Fonctionne, "f") == 0)
                        printf("|    -> Fonctionnement du materiel: En marche\n", Fonctionne);
                    else
                        printf("|    -> Fonctionnement du materiel: En panne\n", Fonctionne);
                    printf("|    -> Le prix d'achat du materiel : %s\n", Prix);
                    printf("|    -> La Description : %s\n", Description);
                    printf("---------------------------------------------------------------------------------------------\n\n");
                }
                else
                {
                    if (atoi(Identifiant_Recherche) < atoi(Cle_Courrante))
                    {
                        stop = 1;
                        *j = temp_j;
                        printf("\n\n---------------- L'enregistrement n'exsite pas, il devait exister: ------------------\n", *i, *j);
                        printf("|    -> Dans le bloc numero : %i\n", *i);
                        printf("|    -> Dans la position : %i\n", *j);
                        printf("-------------------------------------------------------------------------------------\n\n");
                    }
                    else
                    {
                        *j = *j + TAILLE_MATERIEL - 1 + TAILLE_FONCTIONNEMENT + TAILLE_PRIX;
                        extraire_chaine_TOVnC(Taille, j, TAILLE_TAILLE, &Buf); // recuperer un tableau de 3 caractères, la taille d'enregistrement
                        *j = *j + atoi(Taille);
                        temp_j = *j;
                        extraire_chaine_TOVnC(Cle_Courrante, j, TAILLE_IDENTIFIANT, &Buf);
                    }
                }
            }
        }
        else
        {
            if (atoi(Identifiant_Recherche) < atoi(Cle_Min)) // la cle n'est pas dans ce bloc
                bsup = *i - 1;                               // la cle doit être avant le bloc courant
            else
                binf = *i + 1; /// cle  doit être après le bloc courant
        }
    }
    if (binf > bsup)
    {
        *i = binf;
        *j = temp_j;
        printf("\n\n!---------------- L'enregistrement n'exsite pas, il devait exister: ------------------\n", *i, *j);
        printf("|    -> Dans le bloc numero : %i\n", *i);
        printf("|    -> Dans la position : %i\n", *j);
        printf("-------------------------------------------------------------------------------------\n\n");
    }
    Fermer_TOVnC(&f);
}

/*








*/
/**********************************************|
|                                              |
|       reorganiser un fichier TOVnC selon     |
|          le champs de fonctionnement         |
|                                              |
|**********************************************/
void Reorganisation_TOVnC(char nom_fichier[], char nom_fichier1[], char nom_fichier2[])
{
    fichier_TOVnC f;                                  // le fichier original qu'on doit reorganiser "Materiel_informatique_TOVnC.bin"
    Ouvrir_TOVnC(&f, nom_fichier, 'A');               // ouvrir le fichier TOVnC
    fichier_TOVC f1;                                  // le fichier qui contient le materiel informatique ne marche
    Ouvrir_TOVC(&f1, nom_fichier1, 'N');              // ouvrir le fichier TOVC
    fichier_LOVC f2;                                  // le fichier qui contient le materiel informatique en panne
    Ouvrir_LOVC(&f2, nom_fichier2, 'N');              // ouvirir le fichier LOVC
    Tampon_TOVnC Buf;                                 // un buffer en MC pour le fichier TOVnC
    Tampon_TOVC Buf1;                                 // un buffer en MC pour le fichier TOVC
    Tampon_LOVC Buf2;                                 // un Buffer en MC pour le fichier LOVC
    memset(Buf1.tableau, '\0', sizeof(Buf1.tableau)); // vider le tableau BUf1
    memset(Buf2.tab, '\0', sizeof(Buf2.tab));         // vider le tableau Buf2

    int i = 1,                                   // l e parcours des blocs du fichier
        j = 0;                                   // le parcours de position du fichier
    char Identifiant[TAILLE_IDENTIFIANT + 1],    // numero d'identifiant(cle)
        Supprime[TAILLE_SUPPRIMER + 1],          // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1], // la description (caracteristiques) du materiel
        Destination[B];                          // la chaine ou on fera la concatenation des champs pour l'ecrire ensuite dans le Buf

    while (i < Entete_TOVnC(&f, 1))
    {
        LireDir_TOVnC(&f, i, &Buf);
        while (j < Buf.nb)
        {
            extraire_chaine_TOVnC(Identifiant, &j, TAILLE_IDENTIFIANT, &Buf);
            extraire_chaine_TOVnC(Supprime, &j, TAILLE_SUPPRIMER, &Buf);
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL - 1, &Buf);
            extraire_chaine_TOVnC(Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf);
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);

            if (strcmp(Supprime, "f") == 0)
            {
                concatenate(Destination, Identifiant, "", Materiel, "", Prix, Taille, Description);
                if (strcmp(Fonctionne, "f") == 0)
                {
                    printf("\n\nTOVC file");
                    printf("\n %s", Destination);
                }
                else
                {
                    printf("\n\nin the LOVC file");
                    printf("\n %s", Destination);
                }
            }
        }
        j = 0;
        i++;
    }
}

int main(void)
{
    printf("a printing is needed");
    // Reorganisation_TOVnC(FICHIER_ORIGINAL, FICHIER_MATERIEL_FONCTIONNE, FICHIER_MATERIEL_NON_FONCTIONNE);
    int trouv, i, j;
    Recheche_TOVnC(FICHIER_ORIGINAL, "00050", &trouv, &i, &j);
    Recheche_TOVnC(FICHIER_ORIGINAL, "00055", &trouv, &i, &j);
    Recheche_TOVnC(FICHIER_ORIGINAL, "00060", &trouv, &i, &j);
}