#include "Affine.h"

using namespace std;

Affine::Affine()
{
    readRacine();
    readArctangeante();
    readInverse();
}

int Affine::racine(int x)
{
    if (x < 0 || x > 131072) { // 256*256 *2 = 131 072
        cout << "Calcul de Racine impossible : " << x << endl;
        exit(1);
    }

    int i = 1;
    int incr = 1;
    if (x <= incr) {
        return m_racine[i-1] + (m_racine[i] - m_racine[i-1]) * x / incr;
    }

    for (i = 2; i <= PRECISION_RACINE; i++) {
        if (x <= incr * 2) {
            return m_racine[i-1] + (m_racine[i] - m_racine[i-1]) * (x - incr) / incr;
        }
        incr *= 2;
    }
    return 0;
}

// Calcule Arctan(num/denom)
int Affine::arctangeante(int num, int denom)
{
    int i, val;
    if (denom == 0) {
        cout << "Calcul de Arctangeante impossible" << endl;
        exit(1);
    }
    val = num * m_precision_atan * inverse(denom) / PRECISION_INVERSE;
    if (val > 0) {
        for (i = N_CLASSES/2; i > 0; i--) {
            if (val >= m_arctangeante[i-1])
                return i;
        }
    }
    if (val >= -m_arctangeante[0])
        return 0;
    for (i = -1; i > -N_CLASSES/2; i--) {
        if (val >= -m_arctangeante[-i])
            return i+N_CLASSES;
    }
    return N_CLASSES/2;
}

int Affine::inverse(int x)
{
    if (x > 0)
        return m_inverse[x - 1];
    return -m_inverse[1 - x];
}

void Affine::writeRacine()
{
    ofstream fichier_out("HOG/affine/racine.txt", ios::out);
    if(!fichier_out) {
        cerr << "�a marche pas" << endl;
        exit(1);
    }

    int incr = 0;
    fichier_out << round(sqrt(incr)) << " ";

    incr = 1;
    for (int i = 0; i < PRECISION_RACINE; i++) {
        fichier_out << round(sqrt(incr)) << " ";
        incr *= 2;
    }

    fichier_out.close();
}

void Affine::writeArctangeante(int precision)
{
    ofstream fichier_out("HOG/affine/arctangeante.txt", ios::out);
    if(!fichier_out) {
        cerr << "�a marche pas" << endl;
        exit(1);
    }
    double incr = M_PI/(N_CLASSES);
    double angle = incr/2;

    fichier_out << precision << " ";
    for (int i = 0; i < N_CLASSES/2; i++) {
        int val = round(tan(angle)*precision);
        fichier_out << val << " ";
        angle += incr;
    }

    fichier_out.close();
}

void Affine::writeInverse()
{
    ofstream fichier_out("HOG/affine/inverse.txt", ios::out);
    if(!fichier_out) {
        cerr << "�a marche pas" << endl;
        exit(1);
    }
    for (int i = 1; i < 256; i++) {
        fichier_out << round(PRECISION_INVERSE/i) << " ";
    }

    fichier_out.close();
}

void Affine::readRacine()
{
    ifstream fichier_in("HOG/affine/racine.txt", ios::in);
    if(!fichier_in) {
        cerr << "�a marche pas" << endl;
        exit(1);
    }

    int i;
    for (i = 0; i <= PRECISION_RACINE; i++) {
        fichier_in >> m_racine[i];
    }

    fichier_in.close();
}

void Affine::readArctangeante()
{
    ifstream fichier_in("HOG/affine/arctangeante.txt", ios::in);
    if(!fichier_in) {
        cerr << "�a marche pas" << endl;
        exit(1);
    }

    fichier_in >> m_precision_atan;
    for (int i = 0; i < N_CLASSES; i++) {
        fichier_in >> m_arctangeante[i];
    }

    fichier_in.close();
}

void Affine::readInverse()
{
    ifstream fichier_in("HOG/affine/inverse.txt", ios::in);
    if(!fichier_in) {
        cerr << "�a marche pas" << endl;
        exit(1);
    }

    for (int i = 1; i < 256; i++) {
        fichier_in >> m_inverse[i - 1];
    }

    fichier_in.close();
}
