#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <string.h>
#include <iomanip>
#include "Image.h"
#include "Histo.h"
#include "Affine.h"

using namespace std;

void hog(string nom_fichier, int pattern[N_CLASSES][CELL_SIZE*CELL_SIZE])
{
    int large, haut;
    string magic_number;
    int header;
    int i;

    // Lecture image
    ifstream fichier(("../img/"+nom_fichier+".pgm").c_str(), ios::in);
    if(!fichier) {
        cerr << "�a marche pas" << endl;
    }
    fichier >> magic_number >> large >> haut >> header;

    i = 0;
    int tab[large*haut];
    while(!fichier.eof() && i < large*haut) {
        fichier >> tab[i];
        if (tab[i] > 255)
            cout << i << " " << tab[i] << " " << large << " " << haut << endl;
        i++;
    }
    fichier.close();

    Histo histo(large,haut,tab);
    histo.affiche_graphique(nom_fichier, pattern);
}

void precision_arctangeante()
{
    for (int precision = 1; precision <= 2048; precision*=2) {
        {
            Affine calcul;
            calcul.writeArctangeante(precision);
        }
        Affine calcul;
        cout << "Coefficient = " << setw(4) << precision;
        for (int borne = 2; borne <= 256; borne*=2) {
            int compare = 0;
            int cpt = 0;
            for (int j = borne; j >= -1*borne; j--) {
                for (int i = -1*borne; i <= borne; i++) {
                    if (i != 0) {
                        double val = round(8.0*atan(j/(double)i)/(M_PI/2));
                        if (val < 0)
                            val += 16;
                        if (calcul.arctangeante(j, i) != val)
                            compare++;
                        cpt++;
                    }
                }
            }
            cout << ", borne = " << borne << " : " << setw(3) << 100 - (100*compare/cpt) << "%";
        }
        cout << endl;
    }
}

void precision_racine()
{
    {
        Affine calcul;
        calcul.writeRacine();
    }
    Affine calcul;
    for (int borne = 1; borne <= 131072; borne *= 2) {
        double compare = 0;
        int cpt = 0;
        for (int i = borne/2; i <= borne; i++) {
            if (calcul.racine(i) != round(sqrt(i)))
                compare += (round(sqrt(i)) - calcul.racine(i)) / (double)round(sqrt(i));
            cpt++;
        }
        cout << "Intervalle = [" << setw(5) << borne/2 << "," << setw(6) << borne << "] : " << setw(3) << 100 - (100*compare/cpt) << "%" << endl;
    }
}

void precision_inverse()
{
    Affine calcul;
    double compare = 0;
    int cpt = 0;
    for (int i = 1; i < 256; i++) {
        if ((double)calcul.inverse(i) / PRECISION_INVERSE != 1.0/i)
        compare += abs((double)calcul.inverse(i) / PRECISION_INVERSE - 1.0/i) / (1.0/i);
        cpt++;
    }
    cout << "Precision inverse : " << setw(3) << 100 - (100*compare/cpt) << "%" << endl;
}

int main() {

    //A d�commenter pour visualiser la comparaison des r�sultats de
    //l'approximation lin�aire :

    // precision_arctangeante();
    // precision_racine();
    // precision_inverse();

    // Lecture patterns histogramme
    string magic_number;
    int header;
    int pattern[N_CLASSES][CELL_SIZE*CELL_SIZE];
    for(int k = 0; k < N_CLASSES; k++) {
        stringstream ks;
        ks << k; 
        ifstream fichier(("histo/"+ks.str()+".pgm").c_str(), ios::in | ios::binary);
        if(!fichier)
            cerr << "�a marche pas" << endl;
        fichier >> magic_number >> header >> header >> header;
        int i = 0;
        unsigned char buffer;
        fichier.read((char *)&buffer, sizeof(unsigned char));
        while(i < CELL_SIZE*CELL_SIZE) {
            fichier.read((char *)&buffer, sizeof(unsigned char));
            pattern[k][i] = buffer;
            i++;
        }
        fichier.close();

    }

    // Initialisation fonctions approximations
    {
        Affine calcul;
        calcul.writeRacine();
        calcul.writeArctangeante(256);
        calcul.writeInverse();
    }

    //fichiers trait�s ajout�s ou supprim�s si necesaire (les fichiers doivent
    //etre contenus dans ../img
    DIR* dirFile = opendir("../img/");
    if (dirFile) {
        struct dirent *jfile;
        while ((jfile = readdir(dirFile)) != NULL) {
            if (!strcmp(jfile->d_name, ".") || !strcmp(jfile->d_name, ".."))
                continue;
            if (strstr(jfile->d_name, ".pgm")) {
                char *dot = strchr(jfile->d_name, '.');
                if (dot != NULL)
                    *dot = '\0';
                hog(jfile->d_name, pattern);
            }
        }
    }
    closedir(dirFile);
}