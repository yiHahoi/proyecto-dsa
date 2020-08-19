#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


class CITY {
    public:
        int population;
        double geoPointX;
        double geoPointY;
};


class NODE {
    public:
        CITY* data      = NULL;
        char color      = 'w';
        NODE* father    = NULL;
        NODE* first     = NULL;
        NODE* second    = NULL;
        NODE* third     = NULL;
        NODE* fourth    = NULL;
        double x        = 0.0;
        double y        = 0.0;
        double w        = 0.0;
        int depth       = 0;

        int count_colors(char);

};


class PR_QUADTREE {

    public:

        NODE* _root         = NULL;
        double _x           = -181.0;
        double _y           = -181.0;
        double _w           = 362.0;
        int _totalPoints    = 0;
        int _maxDepth       = 0;

        PR_QUADTREE();
        ~PR_QUADTREE();
        void insert(double, double, CITY*);
        int remove(double, double);
        int total_population(double, double);
        int total_cities_region(double, double, int);
        int total_population_region(double, double, int);
        NODE* search_node(double, double);
        NODE* search_city(double, double);

};

PR_QUADTREE::PR_QUADTREE(){
    _root = new NODE;
    _root->x = _x;
    _root->y = _y;
    _root->w = _w;
}


PR_QUADTREE::~PR_QUADTREE(){}

/////////////////////////////////////////////////////////////////////////////////////////


// devuelve un puntero a nodo con el cuadrante mas pequeño que incluye la posicion (x,y)
NODE* PR_QUADTREE::search_node(double x, double y){

    NODE* node = _root;

    while(1){

        // punto pertenece al primer cuadrante?
        if(node->x <= x && x < node->x + node->w/2 &&
           node->y + node->w/2 <= y && y < node->y + node->w){

                if(node->color != 'g')
                    break;
                else
                    node = node->first;

        // punto pertenece al segundo cuadrante?
        } else if(node->x + node->w/2 <= x && x < node->x + node->w &&
           node->y + node->w/2 <= y && y < node->y + node->w){

                if(node->color != 'g')
                    break;
                else
                    node = node->second;

        // punto pertenece al tercer cuadrante?
        } else if(node->x <= x && x < node->x + node->w/2 &&
           node->y <= y && y < node->y + node->w/2){

                if(node->color != 'g')
                    break;
                else
                    node = node->third;

        // punto pertenece al cuarto cuadrante?
        } else if(node->x + node->w/2 <= x && x < node->x + node->w &&
           node->y <= y && y < node->y + node->w/2){

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

    double h = _w;

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

                // se comprueba en que subcuadrante se encuentra cada punto

                // punto1 pertenece al primer subcuadrante?
                if(temp->x <= x && x < temp->x + temp->w/2 &&
                   temp->y + temp->w/2 <= y && y < temp->y + temp->w){

                        cuadrante1 = 1;

                // punto1 pertenece al segundo subcuadrante?
                } else if(temp->x + temp->w/2 <= x && x < temp->x + temp->w &&
                   temp->y + temp->w/2 <= y && y < temp->y + temp->w){

                        cuadrante1 = 2;

                // punto1 pertenece al tercer subcuadrante?
                } else if(temp->x <= x && x < temp->x + temp->w/2 &&
                   temp->y <= y && y < temp->y + temp->w/2){

                        cuadrante1 = 3;

                // punto1 pertenece al cuarto subcuadrante?
                } else if(temp->x + temp->w/2 <= x && x < temp->x + temp->w &&
                   temp->y <= y && y < temp->y + temp->w/2){

                        cuadrante1 = 4;

                // punto2 pertenece al primer subcuadrante?
                } if(temp->x <= oldX && oldX < temp->x + temp->w/2 &&
                   temp->y + temp->w/2 <= oldY && oldY < temp->y + temp->w){

                        cuadrante2 = 1;

                // punto2 pertenece al segundo subcuadrante?
                } else if(temp->x + temp->w/2 <= oldX && oldX < temp->x + temp->w &&
                   temp->y + temp->w/2 <= oldY && oldY < temp->y + temp->w){

                        cuadrante2 = 2;

                // punto2 pertenece al tercer subcuadrante?
                } else if(temp->x <= oldX && oldX < temp->x + temp->w/2 &&
                   temp->y <= oldY && oldY < temp->y + temp->w/2){

                        cuadrante2 = 3;

                // punto2 pertenece al cuarto subcuadrante?
                } else if(temp->x + temp->w/2 <= oldX && oldX < temp->x + temp->w &&
                   temp->y <= oldY && oldY < temp->y + temp->w/2){

                        cuadrante2 = 4;

                }

                // se crean los 4 nodos hijo
                temp->first = new NODE;
                temp->first->father = node;
                temp->first->depth = node->depth + 1;
                temp->first->color = 'w';
                temp->first->x = temp->x;
                temp->first->y = temp->y + temp->w/2;
                temp->first->w = temp->w/2;
                temp->first->w = temp->w/2;

                temp->second = new NODE;
                temp->second->father = node;
                temp->second->depth = node->depth + 1;
                temp->second->color = 'w';
                temp->second->x = temp->x + temp->w/2;
                temp->second->y = temp->y + temp->w/2;
                temp->second->w = temp->w/2;
                temp->second->w = temp->w/2;

                temp->third = new NODE;
                temp->third->father = node;
                temp->third->depth = node->depth + 1;
                temp->third->color = 'w';
                temp->third->x = temp->x;
                temp->third->y = temp->y;
                temp->third->w = temp->w/2;
                temp->third->w = temp->w/2;

                temp->fourth = new NODE;
                temp->fourth->father = node;
                temp->fourth->depth = node->depth + 1;
                temp->fourth->color = 'w';
                temp->fourth->x = temp->x + temp->w/2;
                temp->fourth->y = temp->y;
                temp->fourth->w = temp->w/2;
                temp->fourth->w = temp->w/2;

                // se actualiza el contador de maxima profundidad del quadtree :p
                if(_maxDepth < node->depth + 1)
                    _maxDepth = node->depth + 1;

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

        // se aumenta el contador de puntos del quadtree
        _totalPoints++;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

// devuelve el total de nodos hijo del color especificado
int NODE::count_colors(char x){
    int ctr = 0;
    if(color == 'g'){
        if(first->color     == x) ctr++;
        if(second->color    == x) ctr++;
        if(third->color     == x) ctr++;
        if(fourth->color    == x) ctr++;
    }
    return(ctr);
}


int PR_QUADTREE::remove(double x, double y){

    NODE* node = search_city(x,y);
    NODE* father = node->father;

    // se comprueba la existencia de la ciudad
    if(node == NULL)
        return(-1);

    // se borran los datos del nodo y se reinicia
    delete node->data;
    node->data = NULL;
    node->color = 'w';
    _totalPoints -= 1;

    // si solo existia el nodo root se retorna
    if(node == _root)
        return(0);

    // se COMPACTA el arbol de forma iterativa
    while(father != NULL) {

        // es necesaria una referencia directa al padre para evitar casos problematicos
        // en que el nodo elimine referencias asi mismo usando node->father->{first,second,...}
        father = node->father;

        // se cuentan los colores de los nodos hijos
        int g, w, b;
        g = father->count_colors('g');
        w = father->count_colors('w');
        b = 4 - g - w;

        // si solo quedan nodos blancos se eliminan y nodo padre se blanquea
        if(w == 4){

            delete father->first;
            delete father->second;
            delete father->third;
            delete father->fourth;
            father->first   = NULL;
            father->second  = NULL;
            father->third   = NULL;
            father->fourth  = NULL;
            father->color = 'w';
            node = father; // para siguiente iteracion

        // si solo queda un nodo negro entonces reemplaza al nodo padre y se eliminan los hijos
        } else if (b == 1 && w == 3) {

            father->color = 'b';
            father->data = node->data;
            delete father->first;
            delete father->second;
            delete father->third;
            delete father->fourth;
            father->first   = NULL;
            father->second  = NULL;
            father->third   = NULL;
            father->fourth  = NULL;
            node = father; // para siguiente iteracion

        // condicion de termino de compactacion (g >= 1 || b >= 2)
        } else {
            return(0);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

// retorna la poblacion estimada de una ciudad (si ciudad no existe se considera poblacion estimada = 0)
int PR_QUADTREE::total_population(double x, double y){

    NODE* node = search_city(x,y);
    if(node == NULL)
        return(0);
    else
        return(node->data->population);

}

/////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////

int PR_QUADTREE::total_cities_region(double x, double y, int radius){

    return(0);
}

/////////////////////////////////////////////////////////////////////////////////////////

int PR_QUADTREE::total_population_region(double x, double y, int radius){

    return(0);
}

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

    // lectura de datos
    ifstream file;
    file.open("worldcitiespop_fixed.csv");
    //file.open("mini.csv");
    string line;
    string word;
    string temp;
    int ctr = 0;

    PR_QUADTREE cities;

    getline(file,line);
    while(getline(file,line) && ctr < 3173647){

        stringstream ss(line);

        CITY* city = NULL;
        city = new CITY;

        /*  Country;City;AccentCity;Region;
        Population;Latitude;Longitude;geopoint */

        getline(ss,temp,';');
        //city->country = temp;

        getline(ss,temp,';');
        //city->city = temp;

        getline(ss,temp,';');
        //city->accentCity = temp;

        getline(ss,temp,';');
        //city->region = temp;

        getline(ss,word,';');
        city->population = stod(word);

        getline(ss,word,';');
        //city->latitude = stold(word);

        getline(ss,word,';');
        //city->longitude = stold(word);

        getline(ss,word,',');
        city->geoPointX = stold(word);

        getline(ss,word,',');
        city->geoPointY = stold(word);

        // se insertan los datos al quadtree
        cities.insert(city->geoPointX, city->geoPointY, city);

        ctr++;

    }

    file.close();

    // pruebas!!!
    NODE* x = cities.search_node((double)8.3766667,(double)-78.9591667);
    if(x != NULL)
        cout << "existe la ciudad <8.3766667,-78.9591667>" << endl;
    cout << "poblacion ciudad <8.3766667,-78.9591667>: " << x->data->population << endl;
    cout << "profundidad nodo de ciudad <8.3766667,-78.9591667>: " << x->depth << endl;
    cout << "total de ciudades en quadtree: " << cities._totalPoints << endl;
    cout << "maxima profundidad de nodo en quadtree: " << cities._maxDepth << endl;
    if(cities.remove((double)8.3766667,(double)-78.9591667) == 0)
        cout << "ciudad <8.3766667,-78.9591667> removida" << endl;
    x = cities.search_city((double)8.3766667,(double)-78.9591667);
    if(x == NULL)
        cout << "ya no existe la ciudad <8.3766667,-78.9591667> :C" << endl;
    //while(1){}

    return(0);

}

/////////////////////////////////////////////////////////////////////////////////////////
