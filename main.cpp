
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip> // std::setw
#include <cstring>
#include <cmath>
#include <utility>
#include <stdio.h>
#include <iostream>
#include <utility>
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::make_pair;
using std::string;
using std::stringstream;
using std::vector;
string aMayuscula(string cadena)
{
  for (int i = 0; i < cadena.length(); i++)
    cadena[i] = toupper(cadena[i]);
  return cadena;
}

const vector<string> stopwords = {"shouldnt", "wasn", "wasnt", "weren", "werent", "ourselves", "because", "as", "until", "while",
                                  "you", "you're", "you've", "you'll", "you'd", "your", "yours", "yourself", "yourselves",
                                  "he", "him", "his", "himself", "she", "she's", "her", "hers", "herself", "it", "it's",
                                  "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom",
                                  "this", "that", "thatll", "these", "those", "am", "is", "are", "was", "were", "be", "been",
                                  "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and",
                                  "but", "if", "or", "my", "myself", "of", "at", "by", "for", "with", "about", "against", "more", "most", "other",
                                  "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in",
                                  "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where",
                                  "why", "how", "all", "any", "both", "each", "few", "some", "such", "no", "nor", "not",
                                  "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "dont", "should",
                                  "shouldve", "now", "d", "ll", "m", "o", "re", "ve", "y", "ain", "aren", "arent", "couldn", "couldnt",
                                  "didn", "didnt", "doesn", "doesnt", "hadn", "hadnt", "hasn", "hasnt", "haven", "havent", "isn",
                                  "isnt", "ma", "mightn", "mightnt", "mustn", "mustnt", "needn", "neednt", "shan", "shant", "shouldn",
                                  "i", "me", "we", "our", "ours", "won", "wont", "wouldn", "wouldnt"};

void imprimirDocumentos(vector<vector<std::pair<std::pair<string, int>, vector<float>>>> documentos)
{
  for (int k = 0; k < documentos.size(); k++)
  {
    cout << endl;
    cout << "Documento " << k << endl;

    cout << std::setw(15) << "term";
    cout << std::setw(15) << "indice";
    cout << std::setw(15) << "tdf";
    cout << std::setw(15) << "idf";
    cout << std::setw(15) << "tfidf" << endl;

    for (int i = 0; i < documentos[k].size(); i++)
    {
      cout << endl;
      cout << std::setw(15) << documentos[k][i].first.first;
      cout << std::setw(15) << documentos[k][i].first.second;
      for (int j = 0; j < documentos[k][i].second.size(); j++)
      {
        cout << std::setw(15) << documentos[k][i].second[j];
      }
    }
  }
  cout << endl;
}
// Funcion para calcular la similitud entre documentos
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
void imprimirCorrelaciones(vector<vector<std::pair<string, float>>> matrizNormalizada)
{
  for (int j = 0; j < matrizNormalizada.size(); j++)
  {
    cout << std::setw(10) << "documento ";
    cout << std::setw(1) << j;
    cout << std::setw(2) << " ";
  }
  for (int i = 0; i < matrizNormalizada.size(); i++)
  {
    cout << endl;
    cout << std::setw(10) << "documento ";
    cout << std::setw(1) << i;
    cout << std::setw(2) << " ";

    for (int j = 0; j < matrizNormalizada.size(); j++)
    {
      float sim = cos(matrizNormalizada[i], matrizNormalizada[j]);
      cout << std::setw(11) << sim;
      cout << std::setw(2) << " ";
    }
  }
  cout << " " << endl;
}

//funcion para calcular la distancia del vector
float calculo(vector<std::pair<std::pair<string, int>, vector<float>>> filaDatosTerminos)
{
  float resultado = 0.0;
  for (int i = 0; i < filaDatosTerminos.size(); i++)
    resultado += pow(filaDatosTerminos[i].second[0], 2);
  float distancia = sqrt(resultado);
  return distancia;
}
// funcion para normalizar los valores del vector
vector<vector<std::pair<string, float>>> normalizar(vector<vector<std::pair<std::pair<string, int>, vector<float>>>> documentos)
{
  vector<std::pair<string, float>> fila;
  vector<vector<std::pair<string, float>>> matrix;

  for (int i = 0; i < documentos.size(); i++)
  {
    fila.clear();
    for (int j = 0; j < documentos[i].size(); j++)
    {
      // Divido el TF entre la la longitud del vector
      float distancia = calculo(documentos[i]);
      // cout << distancia << endl;
      float tfnorm = documentos[i][j].second[0] / distancia;
      // Creo un objeto con los datos recogidos
      //fila.push({"termino": documentos[i][j].termino, "tfnorm": tfnorm});
      fila.push_back({documentos[i][j].first.first, tfnorm});
    }
    matrix.push_back(fila);
  }
  return matrix;
}
// Devuelve cuantas veces aparece un termino en un documento
int frecuencia(vector<string> valoraciones, string valoracion)
{
  int contador = 0;
  for (int i = 0; i < valoraciones.size(); i++)
    if (aMayuscula(valoraciones[i]) == aMayuscula(valoracion))
    {
      contador++;
    }
  return contador;
}
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
vector<string> metodo(string texto)
{
  char space_char = ' ';
  vector<string> words{};
  stringstream sstream(texto);
  string word;
  while (std::getline(sstream, word, space_char))
  {
    word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
    words.push_back(word);
  }
  return words;
}

int main(int argc, char *argv[])
{

  string filename(argv[1]);
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

  for (int i = 0; i < valoraciones.size(); i++)
  {
    // Creo un vector con los terminos visitados
    vector<string> visitados;
    // Creo un vector cpara almacenar los calculos
    vector<std::pair<std::pair<string, int>, vector<float>>> filaDatosTerminos;
    for (int j = 0; j < valoraciones[i].size(); j++)
    {
      // Compruebo si la palabra no ha sido visitada y ademas no es una 'stopword'
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
  }
  imprimirDocumentos(documentos);

  vector<vector<std::pair<string, float>>> matrizNormalizada = normalizar(documentos);

  cout << std::setw(13) << "  ";

  ////IMPRIMIR CORRELACIONES ENTRE DOCUMENTOS
  imprimirCorrelaciones(matrizNormalizada);
  input_file.close();
  return EXIT_SUCCESS;
}