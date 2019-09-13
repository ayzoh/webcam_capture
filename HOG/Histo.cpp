#include "Histo.h"
using namespace std;


//Constructeurs
Histo::Histo(int width, int height, int image[])
        :m_width(width), m_height(height), m_histo_width(width/CELL_SIZE), m_histo_height(height/CELL_SIZE), m_img(Image(m_width, m_height, image))
{
    // Supprimer le if pour la synthèse
    if (m_width%CELL_SIZE!=0 || m_height%CELL_SIZE!=0) {
        cout << "Taille non divisible par " << CELL_SIZE << endl;
        exit(1);
    }
    m_histo = new int[m_histo_width*m_histo_height];
    m_histo_norme = new int[m_histo_width*m_histo_height];
    m_histo_accurate = new double[m_histo_width*m_histo_height];
    m_histo_norme_accurate = new double[m_histo_width*m_histo_height];

    calcul_histo();
}

void Histo::affiche()
{
    int i, j;
    for(j = 0; j < m_histo_height; j++) {
        for(i = 0; i < m_histo_width; i++) {
            cout << m_histo[i+m_histo_width*j] << " ";
        }
        cout << endl;
    }
}

void Histo::affiche_ascii()
{
    int i, j;
    char c;
    for(j = 0; j < m_histo_height; j++) {
        for(i = 0; i < m_histo_width; i++) {
            switch(((m_histo[i+m_histo_width*j]+2)%16)/4) {
                case 1:
                    c = '\\';
                    break;
                case 2:
                    c = '-';
                    break;
                case 3:
                    c = '/';
                    break;
                default:
                    c = '|';
                    break;
            }

            cout << c << " ";
        }
        cout << endl;
    }
}

void Histo::affiche_graphique(string nom, int pattern[N_CLASSES][CELL_SIZE*CELL_SIZE])
{
    ofstream fichier_out(("../res/"+nom+"_histo.pgm").c_str(), ios::out);
    if(!fichier_out) {
        cerr << "ça marche pas" << endl;
        exit(1);
    }

    fichier_out << "P2" << endl << m_width * 3 << " " << m_height << endl << "255" << endl;

    int i, j, k;
    for(j = 0; j < m_histo_height; j++) {
        for(int occur = 0; occur < 4; occur++) {
            for(i = 0; i < m_histo_width; i++) {
                for(k = 0; k < CELL_SIZE; k++) {
                    fichier_out << (m_img.getImage())[i*CELL_SIZE+k + (j*CELL_SIZE+occur)*m_width] << " ";
                }
            }
            for(i = 0; i < m_histo_width; i++) {
                int val = m_histo_norme[i+m_histo_width*j];
                if (val > 1024)
                    val = 1024;

                int classe = m_histo[i+m_histo_width*j];

                for(k = 0; k < CELL_SIZE; k++) {
                    fichier_out << val*pattern[classe][k+occur*CELL_SIZE]/1024 << " ";
                }
            }
            for(i = 0; i < m_histo_width; i++) {
                double val = m_histo_norme_accurate[i+m_histo_width*j];
                if (val > 1024)
                    val = 1024;

                int classe = m_histo_accurate[i+m_histo_width*j];

                for(k = 0; k < CELL_SIZE; k++) {
                    fichier_out << (int)round(val*pattern[classe][k+occur*CELL_SIZE]/1024) << " ";
                }
            }
            fichier_out << endl;
        }
    }
    fichier_out.close();
}

void Histo::gradient_pixel(int* grad_h, int* grad_v, int image[], int x, int y)
{
    if(x!=0 && x!=m_width-1 && y!=0 && y!=m_height-1) { // Si on n'est pas au bord de l'image
        *grad_h = image[(x+1)+m_width*y] - image[(x-1)+m_width*y];
        *grad_v = - image[x+m_width*(y+1)] + image[x+m_width*(y-1)];
    } else {
        *grad_h = 0;
        *grad_v = 0;
    }
    if (*grad_v < -300) {
        cout << image[x+m_width*(y+1)] << " " << image[x+m_width*(y+1)-1] << endl;
    }
}

void Histo::calcul_norme_pixel(int* norme, int x, int y, double* norme_accurate)
{
    *norme = m_calcul.racine(x*x+y*y);
    *norme_accurate = sqrt(x*x+y*y);
}

void Histo::calcul_arg_pixel(int* arg, int x, int y, double* arg_accurate)
{
    if(x == 0) { // Evite la division par 0
        if (y == 0) {
            *arg = 0;
        } else {
            *arg = N_CLASSES/2;
        }
        *arg_accurate = *arg;
    } else {
        *arg = m_calcul.arctangeante(y, x);
        double val = round(atan((double)y/x) * 16 / M_PI);
        if (val < 0)
            val += 16;
        *arg_accurate = val;
    }
}

void Histo::calcul_histo()
{
    int i, j, k, m, n, x, y, classe, val;
    double val_accurate;
    int grad_h;
    int grad_v;

    int norme;
    int arg;
    int cell[N_CLASSES];
    double norme_accurate;
    double arg_accurate;
    double cell_accurate[N_CLASSES];

    // Parcours des cellules
    for(j = 0; j < m_histo_height; j++) {
        for(i = 0; i < m_histo_width; i++) {
            // Init cell
            for(k = 0; k < N_CLASSES; k++) {
                cell[k] = 0;
                cell_accurate[k] = 0;
            }
            classe = 0; // Gradient horizontal
            val = 0;
            val_accurate = 0;

            // Parcours des pixels de la cellule
            for(n = 0; n < CELL_SIZE; n++) {
                for(m = 0; m < CELL_SIZE; m++) {
                    x = i*CELL_SIZE+m;
                    y = j*CELL_SIZE+n;

                    // Calcul gradients
                    gradient_pixel(&grad_h, &grad_v, m_img.getImage(), x, y);

                    // Calcul norme et argument
                    if (grad_v < -300) {
                        cout << grad_h << " " << grad_v << endl;
                    }
                    calcul_norme_pixel(&norme, grad_h, grad_v, &norme_accurate);
                    calcul_arg_pixel(&arg, grad_h, grad_v, &arg_accurate);

                    // Vote
                    cell[arg] += norme;
                    cell_accurate[(int)floor(arg_accurate)] += norme_accurate;
                }
            }

            // Resultat du vote
            for(k = 0; k < N_CLASSES; k++) {
                if (cell[k] > val) {
                    val = cell[k];
                    classe = k;
                }
            }
            m_histo[i+m_histo_width*j] = classe;
            m_histo_norme[i+m_histo_width*j] = val;
            for(k = 0; k < N_CLASSES; k++) {
                if (cell_accurate[k] > val_accurate) {
                    val_accurate = cell_accurate[k];
                    classe = k;
                }
            }
            m_histo_accurate[i+m_histo_width*j] = classe;
            m_histo_norme_accurate[i+m_histo_width*j] = val_accurate;
        }
    }
}
