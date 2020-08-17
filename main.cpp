#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


class CITY {
    public:
        string country;
        string city;
        string accentCity;
        string region;
        int population;
        double latitude;
        double longitude;
        double geoPointX;
        double geoPointY;
};

class NODE {
    public:
        CITY* data      = NULL;
        char color      = 'w';
        //NODE* father    = NULL;
        NODE* first     = NULL;
        NODE* second    = NULL;
        NODE* third     = NULL;
        NODE* fourth    = NULL;
        double x        = 0.0;
        double y        = 0.0;
        double w        = 0.0;
        double h        = 0.0;
        //int depth       = 0;

};


class PR_QUADTREE {

    public:

        NODE* _root     = NULL;
        double _x       = -181.0;
        double _y       = -181.0;
        double _w       = 362.0;
        double _h       = 362.0;

        PR_QUADTREE();
        ~PR_QUADTREE();
        void insert(double, double, CITY*);
        void remove(double, double);
        int total_cities(double, double, int);
        void total_population(double, double);
        void total_population_region(double, double, int);
        NODE* search_node(double, double);
        NODE* search_city(double, double);

};

PR_QUADTREE::PR_QUADTREE(){
    _root = new NODE;
    _root->x = _x;
    _root->y = _y;
    _root->w = _w;
    _root->h = _h;
}


PR_QUADTREE::~PR_QUADTREE(){}

/////////////////////////////////////////////////////////////////////////////////////////

// devuelve un puntero a nodo con el cuadrante mas pequeño que incluye la posicion (x,y)
NODE* PR_QUADTREE::search_node(double x, double y){

    NODE* node = _root;

    while(1){

        // punto pertenece al primer cuadrante?
        if(node->x <= x && x < node->x + node->w/2 &&
           node->y + node->h/2 <= y && y < node->y + node->h){

                if(node->color != 'g')
                    break;
                else
                    node = node->first;

        // punto pertenece al segundo cuadrante?
        } else if(node->x + node->w/2 <= x && x < node->x + node->w &&
           node->y + node->h/2 <= y && y < node->y + node->h){

                if(node->color != 'g')
                    break;
                else
                    node = node->second;

        // punto pertenece al tercer cuadrante?
        } else if(node->x <= x && x < node->x + node->w/2 &&
           node->y <= y && y < node->y + node->h/2){

                if(node->color != 'g')
                    break;
                else
                    node = node->third;

        // punto pertenece al cuarto cuadrante?
        } else if(node->x + node->w/2 <= x && x < node->x + node->w &&
           node->y <= y && y < node->y + node->h/2){

                if(node->color != 'g')
                    break;
                else
                    node = node->fourth;

        }
    }

    // devuelve el nodo blanco o negro donde tendría que estar el punto
    // en teoria hasta este punto node solo puede ser blanco o negro, nunca NULL
    return(node);

}

/////////////////////////////////////////////////////////////////////////////////////////

// devuelve un puntero al nodo que contiene la ciudad o NULL si no existe
NODE* PR_QUADTREE::search_city(double x, double y){

    // se busca el nodo blanco o negro donde tendría que estar el punto
    NODE* node = search_node(x,y);

    // se devuelve el nodo que contiene la ciudad en x,y si existe o NULL si no existe
    if( node->color == 'b' && x == node->data->geoPointX && y == node->data->geoPointY)
        return(node);
    else
        return(NULL);

}

/////////////////////////////////////////////////////////////////////////////////////////

void PR_QUADTREE::insert(double x, double y, CITY* city){

    // se prueba la preexistencia de una ciudad en el mismo punto (en ese caso se ignora)
    NODE* node = search_node(x,y);

    // si existe una ciudad en la misma posicion se evita el insert
    if( node->color == 'b' && x == node->data->geoPointX && y == node->data->geoPointY){

        // si ya existe una ciudad con esta posicion se ignora
        // la nueva ciudad y se aprovecha de liberar su memoria
        delete city;

    } else {

        // el cuadrante ya contiene un punto? es decir hay colision en el nodo?

        // NO hay colision
        if(node->color == 'w'){
            node->data = city;
            node->color = 'b';

        // SI hay colision
        } else if (node->color == 'b'){

            // se debe mover el punto antiguo a un nuevo nodo hijo
            CITY* oldCity = node->data;
            double oldX = oldCity->geoPointX;
            double oldY = oldCity->geoPointY;
            node->data = NULL;
            node->color = 'g';

            int cuadrante1 = 1;
            int cuadrante2 = 1;

            // se crean subramas hasta que el punto antiguo y el nuevo
            // se encuentren en cuadrantes diferentes

            NODE* temp = node;

            while(1){

                // se marca como gris
                temp->color = 'g';

                // se crean los 4 nodos hijo
                temp->first = new NODE;
                temp->first->color = 'w';
                temp->first->x = temp->x;
                temp->first->y = temp->y + temp->h/2;
                temp->first->w = temp->w/2;
                temp->first->h = temp->h/2;

                temp->second = new NODE;
                temp->second->color = 'w';
                temp->second->x = temp->x + temp->w/2;
                temp->second->y = temp->y + temp->h/2;
                temp->second->w = temp->w/2;
                temp->second->h = temp->h/2;

                temp->third = new NODE;
                temp->third->color = 'w';
                temp->third->x = temp->x;
                temp->third->y = temp->y;
                temp->third->w = temp->w/2;
                temp->third->h = temp->h/2;

                temp->fourth = new NODE;
                temp->fourth->color = 'w';
                temp->fourth->x = temp->x + temp->w/2;
                temp->fourth->y = temp->y;
                temp->fourth->w = temp->w/2;
                temp->fourth->h = temp->h/2;

                // se comprueba en que cuadrante se encuentra cada punto

                // punto1 pertenece al primer cuadrante?
                if(temp->x <= x && x < temp->x + temp->w/2 &&
                   temp->y + temp->h/2 <= y && y < temp->y + temp->h){

                        cuadrante1 = 1;

                // punto1 pertenece al segundo cuadrante?
                } else if(temp->x + temp->w/2 <= x && x < temp->x + temp->w &&
                   temp->y + temp->h/2 <= y && y < temp->y + temp->h){

                        cuadrante1 = 2;

                // punto1 pertenece al tercer cuadrante?
                } else if(temp->x <= x && x < temp->x + temp->w/2 &&
                   temp->y <= y && y < temp->y + temp->h/2){

                        cuadrante1 = 3;

                // punto1 pertenece al cuarto cuadrante?
                } else if(temp->x + temp->w/2 <= x && x < temp->x + temp->w &&
                   temp->y <= y && y < temp->y + temp->h/2){

                        cuadrante1 = 4;

                }

                // punto2 pertenece al primer cuadrante?
                if(temp->x <= oldX && oldX < temp->x + temp->w/2 &&
                   temp->y + temp->h/2 <= oldY && oldY < temp->y + temp->h){

                        cuadrante2 = 1;

                // punto2 pertenece al segundo cuadrante?
                } else if(temp->x + temp->w/2 <= oldX && oldX < temp->x + temp->w &&
                   temp->y + temp->h/2 <= oldY && oldY < temp->y + temp->h){

                        cuadrante2 = 2;

                // punto2 pertenece al tercer cuadrante?
                } else if(temp->x <= oldX && oldX < temp->x + temp->w/2 &&
                   temp->y <= oldY && oldY < temp->y + temp->h/2){

                        cuadrante2 = 3;

                // punto2 pertenece al cuarto cuadrante?
                } else if(temp->x + temp->w/2 <= oldX && oldX < temp->x + temp->w &&
                   temp->y <= oldY && oldY < temp->y + temp->h/2){

                        cuadrante2 = 4;

                }

                // condicion de termino para creacion de subnodos
                if(cuadrante1 != cuadrante2){
                    break;
                } else if(cuadrante1 == 1){
                    temp = temp->first;
                } else if(cuadrante1 == 2) {
                    temp = temp->second;
                } else if(cuadrante1 == 3) {
                    temp = temp->third;
                } else if(cuadrante1 == 4) {
                    temp = temp->fourth;
                }

            }

            // luego se agregan las ciudades a los cuadrantes correspondientes
            if(cuadrante1 == 1){
                temp->first->color = 'b';
                temp->first->data = city;
            } else if(cuadrante1 == 2) {
                temp->second->color = 'b';
                temp->second->data = city;
            } else if(cuadrante1 == 3) {
                temp->third->color = 'b';
                temp->third->data = city;
            } else if(cuadrante1 == 4) {
                temp->fourth->color = 'b';
                temp->fourth->data = city;
            }

            if(cuadrante2 == 1){
                temp->first->color = 'b';
                temp->first->data = oldCity;
            } else if(cuadrante2 == 2) {
                temp->second->color = 'b';
                temp->second->data = oldCity;
            } else if(cuadrante2 == 3) {
                temp->third->color = 'b';
                temp->third->data = oldCity;
            } else if(cuadrante2 == 4) {
                temp->fourth->color = 'b';
                temp->fourth->data = oldCity;
            }

        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void PR_QUADTREE::remove(double x, double y){

}

/////////////////////////////////////////////////////////////////////////////////////////

int PR_QUADTREE::total_cities(double x, double y, int radius){

    return(0);
}

/////////////////////////////////////////////////////////////////////////////////////////

void PR_QUADTREE::total_population(double x, double y){

}

/////////////////////////////////////////////////////////////////////////////////////////

void PR_QUADTREE::total_population_region(double x, double y, int radius){

}

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

    // lectura de datos
    ifstream file;
    file.open("worldcitiespop_fixed.csv");
    //file.open("mini.csv");
    string line;
    string word;
    int ctr = 0;

    PR_QUADTREE cities;

    getline(file,line);
    while(getline(file,line) && ctr < 3173647){

        stringstream ss(line);

        CITY* city = NULL;
        city = new CITY;

        /*  Country;City;AccentCity;Region;
        Population;Latitude;Longitude;geopoint */

        getline(ss,city->country,';');
        getline(ss,city->city,';');
        getline(ss,city->accentCity,';');
        getline(ss,city->region,';');

        getline(ss,word,';');
        city->population = stod(word);

        getline(ss,word,';');
        city->latitude = stold(word);

        getline(ss,word,';');
        city->longitude = stold(word);

        getline(ss,word,',');
        city->geoPointX = stold(word);

        getline(ss,word,',');
        city->geoPointY = stold(word);

        // se insertan los datos al quadtree
        //cout << city->geoPointX << " " << city->geoPointY << endl;
        cities.insert(city->geoPointX, city->geoPointY, city);

        ctr++;

    }

    file.close();

    NODE* x = cities.search_node((double)8.3766667,(double)-78.9591667);
    cout << endl << x->data->city << endl;
    //while(1){}

    return(0);

}

/////////////////////////////////////////////////////////////////////////////////////////
