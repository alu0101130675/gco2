# Descripción de uso 
**g++ -std=c++11 main.cpp -o main**
para compilar el archivo main.cpp
**./main input.txt**
para ejecutar el programa. Si se tiene un txt de otro nombre pues se cambia input.txt por el que se tenga.
# Descripción del código desarrollado.

El codigo para esta practicase ha escrito en c++. En el fichero main.cpp encontramos lo siguiente:
~~~ c++
int main(int argc, char *argv[])
{

  string filename("input.txt");
  vector<vector<string>> valoraciones;
  string line;
  int filas = 0;

  ifstream input_file(filename);
  if (!input_file.is_open())
  {
    cerr << "Could not open the file - '"
         << filename << "'" << endl;
    return EXIT_FAILURE;
  }

  while (getline(input_file, line))
  {
    valoraciones.push_back(metodo(line)); //lineas sin comas ni puntos
  }
  vector<string> aux;
  vector<vector<std::pair<std::pair<string, int>, vector<float>>>> documentos;
  ~~~
  Estas lineas lo que hacen es leer linea por linea el input file y llama al "metodo" que lo que hace es eliminar caracter como las ´, y todos los que pueden dar problemas.
  ~~~ c++
  for (int i = 0; i < valoraciones.size(); i++)
  {
    vector<string> visitados;
    vector<std::pair<std::pair<string, int>, vector<float>>> filaDatosTerminos;
    for (int j = 0; j < valoraciones[i].size(); j++)
    {
      if ((std::find(visitados.begin(), visitados.end(), aMayuscula(valoraciones[i][j])) == visitados.end()) &&
          (std::find(stopwords.begin(), stopwords.end(), valoraciones[i][j]) == stopwords.end()))
      {
        float tf = frecuencia(valoraciones[i], valoraciones[i][j]);
        float idf = frecuenciaInversa(valoraciones, valoraciones[i][j]);
        float tf_idf = tf * idf;
        vector<float> aux = {tf, idf, tf_idf};
        filaDatosTerminos.push_back({{valoraciones[i][j], j}, aux});
        visitados.push_back(aMayuscula(valoraciones[i][j]));
      }
    }
    documentos.push_back(filaDatosTerminos);
  ~~~
En estas lineas vamos a calcualr la frecuencia, frecunecia inversa tf_idf. Para ello no podemos coger la misma palabra dos veces en un documento por lo que cada vez que visito una nueva palabra la guardo en el vector visitados y siempre se comprieba si la sigueinte palabra que visitemos esta en ese vector o en el stopwords que lo que contiene son palabras que normalmente no son importantes.
los calculos realizados los guardo en una fila que es un documento y despues esa fila la meto en una mariz de documentos para ser utilizada más tarde.
## Metodos que intervienen en la anterior parte del codigo:
### frecuencia
~~~c++
int frecuencia(vector<string> valoraciones, string valoracion)
  int contador = 0;
  for (int i = 0; i < valoraciones.size(); i++)
    if (aMayuscula(valoraciones[i]) == aMayuscula(valoracion))
    {
      contador++;
    }
  return contador;
}
}
~~~
Contamos cuantas palabras hay en un documento, es decir que le pasamos una fila y el valor a buscar
### frecuenciaInversa
~~~c++
float frecuenciaInversa(vector<vector<string>> valoraciones, string valoracion)
{
  int contador = 0;
  for (int i = 0; i < valoraciones.size(); i++)
    for (int j = 0; j < valoraciones[i].size(); j++)
    {
      if (aMayuscula(valoraciones[i][j]) == aMayuscula(valoracion))
      {
        contador++;
        j = valoraciones[i].size();
      }
    }
  float numerador = valoraciones.size();
  return log10(numerador / contador);
}
~~~
Aplicamos la formula vista en clase en el que el numerador va el numero de documentos y en el denominador el numero de documentos en los que aparece la valoracion. aMayuscula es un metodo para comparar las palabras sin que tenga que ver si estan en mayuscula o no.
El siguiente paso es normalizar la matriz:
~~~c++
vector<vector<std::pair<string, float>>> normalizar(vector<vector<std::pair<std::pair<string, int>, vector<float>>>> documentos)
{
  vector<std::pair<string, float>> fila;
  vector<vector<std::pair<string, float>>> matrix;

  for (int i = 0; i < documentos.size(); i++)
  {
    fila.clear();
    for (int j = 0; j < documentos[i].size(); j++)
    {
      float distancia = calculo(documentos[i]);
      float tfnorm = documentos[i][j].second[0] / distancia;
      fila.push_back({documentos[i][j].first.first, tfnorm});
    }
    matrix.push_back(fila);
  }
  return matrix;
}
~~~
Lo que hacemos es dividir la frecuencia en la que aparece un termino entre la distancia del vector que calulamos con un metodo. Una vez hecho esto lo guardamos en un vector y este en otro que contendrá todos los datos normalizados.
~~~ c++
//funcion para calcular la distancia del vector
float calculo(vector<std::pair<std::pair<string, int>, vector<float>>> filaDatosTerminos)
{
  float resultado = 0.0;
  for (int i = 0; i < filaDatosTerminos.size(); i++)
    resultado += pow(filaDatosTerminos[i].second[0], 2);
  float distancia = sqrt(resultado);
  return distancia;
}
~~~
Este es el metodo que se usa para la distanica en el que lo que se hace es la raiz cuadrada de la suma de todos los terminos del vector al cuadrado.
Por ultimo tenemos el metodo cos que es con el que calculamos la correlacion
~~~c++
float cos(vector<std::pair<string, float>> u, vector<std::pair<string, float>> v)
{
  float res = 0;
  for (int i = 0; i < u.size(); i++)
  {
    for (int j = 0; j < v.size(); j++)
    {
      if (u[i].first == v[j].first)
      {
        res += u[i].second * v[j].second;
      }
    }
  }
  return res;
}
~~~
Le pasamos dos documentos y si los terminos son iguales pues se hace la operacion y cuando acabe con todos retornamos el resultado.
Este metodo se llama cunado vamos a imprimir
