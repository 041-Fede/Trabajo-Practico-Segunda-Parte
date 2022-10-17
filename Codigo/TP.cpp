#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

struct Cupos
{
    int CuposDisponibles,CuposOcupados,DniDelDocente;
};

struct CursoDelArchivo
{
    int CodDeCurso,Nivel,Cupo,Dni;
    char Nombre[45],Idioma[45];
};

struct Alumnos
{
    int Dni;
    char Nombre[30];
};

struct NodoAlumno
{
    Alumnos Info;
    NodoAlumno *Sgte;
};

struct Cursos
{
    int CodDeCurso,Nivel;
    string Idioma;
    NodoAlumno *ListaDeAlumnos;
};

struct NodoCursos
{
    Cursos Info;
    NodoCursos *Sgte;
};

struct NodoColaDeEspera
{
    Alumnos Info;
    NodoColaDeEspera *Sgte;
};

struct Docentes
{
    int Dni,CantTotalDeAlumnos;
    string NombreDelDocente;
};

struct NodoArbol
{
    Docentes Info;
    NodoArbol *Izq;
    NodoArbol *Der;
};

void InicializarMatrizDeEspera(NodoColaDeEspera *MatrizDeEsperaPrimero[][8],NodoColaDeEspera *MatrizDeEsperaUltimo[][8]);
void InicializarMatriz(Cupos Matriz[][8]);
void CargaDeCursos(NodoCursos *&ListaDeCursos,NodoArbol *&Raiz,Cupos MatrizDeCursos[][8]);
void InsertarCurso(NodoCursos *&ListaDeCursos,Cursos Dato);
void InsertarDocente(NodoArbol *&Raiz,Docentes Dato);
void MostrarOperaciones(int &Nro);
void CargaDeAlumnos(NodoCursos *&ListaDeCursos,NodoColaDeEspera *MatrizDeEsperaPrimero[][8],NodoColaDeEspera *MatrizDeEsperaUltimo[][8],Cupos MatrizDeCursos[][8]);
void InsertarAlumno(NodoAlumno *&ListaDeAlumnos,Alumnos Dato);
void InsertarEnColaDeEspera(NodoColaDeEspera *&Primero,NodoColaDeEspera *&Ultimo,Alumnos Dato);
void DarDeBajaUnAlumno(NodoCursos *&ListaDeCursos,NodoColaDeEspera *MatrizDeEsperaPrimero[][8],NodoColaDeEspera *MatrizDeEsperaUltimo[][8],Cupos MatrizDeCursos[][8]);
void Desencolar(NodoColaDeEspera *&Primero,NodoColaDeEspera *&Ultimo,Alumnos &Dato);
void MostrarListado(NodoCursos *ListaDeCursos,Cupos MatrizDeCursos[][8]);
void MostrarListadoDeRechazados(NodoCursos *&ListaDeCursos,NodoColaDeEspera *MatrizDeEsperaPrimero[][8],NodoColaDeEspera *MatrizDeEsperaUltimo[][8]);
void ListarInOrden(NodoCursos *ListaDeCursos,NodoArbol *Raiz,Cupos MatrizDeCursos[][8]);
int NumeroDeIdioma(string Idioma);
bool EliminarNodo(NodoAlumno *&ListaDeAlumnos,int Dni);
FILE *CambioDeArchivo(int Nro);
NodoArbol *BusquedaDeDocente(NodoArbol *Raiz,int Dni);
NodoCursos *BusquedaDeCurso(NodoCursos *ListaDeCursos,int CodDeCurso);
NodoAlumno *BusquedaDeAlumno(NodoAlumno *ListaDeAlumnos,int Dni);

int main()
{
    int Nro;
    NodoArbol *Raiz = NULL;
    NodoCursos *ListaDeCursos = NULL;
    NodoColaDeEspera *MatrizDeEsperaPrimero[6][8]; // Hay que preguntarle a la profe!
    NodoColaDeEspera *MatrizDeEsperaUltimo[6][8];
    Cupos MatrizDeCursos[6][8];

    InicializarMatriz(MatrizDeCursos);

    InicializarMatrizDeEspera(MatrizDeEsperaPrimero,MatrizDeEsperaUltimo);

    CargaDeCursos(ListaDeCursos,Raiz,MatrizDeCursos);

    MostrarOperaciones(Nro);

    while(Nro != 4)
    {
        switch(Nro)
        {
            case 1:
                {
                    CargaDeAlumnos(ListaDeCursos,MatrizDeEsperaPrimero,MatrizDeEsperaUltimo,MatrizDeCursos);
                    break;
                }
            case 2:
                {
                    DarDeBajaUnAlumno(ListaDeCursos,MatrizDeEsperaPrimero,MatrizDeEsperaUltimo,MatrizDeCursos);
                    break;
                }
            case 3:
                {
                    MostrarListado(ListaDeCursos,MatrizDeCursos);
                    break;
                }
            default:
                {
                    cout << "OPERACION INEXISTENTE!" << endl;
                }
        }

        cout << "Ingrese una opcion: ";
        cin >> Nro;
    }

    cout << "-----------------" << endl;
    cout << "Listado por codigo de curso de los alumnos que no lograron obtener vacante: " << endl;

    MostrarListadoDeRechazados(ListaDeCursos,MatrizDeEsperaPrimero,MatrizDeEsperaUltimo);

    cout << "Listado de los docentes que dictaran cursos ordenado de menor a mayor por numero de documento: " << endl;

    ListarInOrden(ListaDeCursos,Raiz,MatrizDeCursos);

    cout << "-----------------" << endl;

    return 0;
}

void InicializarMatrizDeEspera(NodoColaDeEspera *MatrizDeEsperaPrimero[][8],NodoColaDeEspera *MatrizDeEsperaUltimo[][8])
{
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            MatrizDeEsperaPrimero[i][j] = NULL;
            MatrizDeEsperaUltimo[i][j] = NULL;
        }
    }
}

void InicializarMatriz(Cupos MatrizDeCursos[][8])
{
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            MatrizDeCursos[i][j].CuposDisponibles = 0;
            MatrizDeCursos[i][j].CuposOcupados = 0;
            MatrizDeCursos[i][j].DniDelDocente = 0;
        }
    }
}

void CargaDeCursos(NodoCursos *&ListaDeCursos,NodoArbol *&Raiz,Cupos MatrizDeCursos[][8])
{
    int Nro;
    NodoArbol *Aux;
    Cursos AuxC;
    CursoDelArchivo C;
    Docentes D;

    for(int i = 0; i < 6; i++)
    {
        FILE *Archivo = CambioDeArchivo(i);

        if(Archivo != NULL)
        {
            fread(&C,sizeof(CursoDelArchivo),1,Archivo);

            while(!feof(Archivo))
            {
                AuxC.Idioma = C.Idioma;
                Nro = NumeroDeIdioma(AuxC.Idioma);
                AuxC.Nivel = C.Nivel;
                AuxC.CodDeCurso = C.CodDeCurso;
                AuxC.ListaDeAlumnos = NULL;

                InsertarCurso(ListaDeCursos,AuxC);

                Aux = BusquedaDeDocente(Raiz,C.Dni);

                if(Aux == NULL)
                {
                    D.NombreDelDocente = C.Nombre;
                    D.Dni = C.Dni;

                    InsertarDocente(Raiz,D);

                    MatrizDeCursos[Nro][AuxC.Nivel - 1].CuposDisponibles = C.Cupo;
                    MatrizDeCursos[Nro][AuxC.Nivel - 1].DniDelDocente = C.Dni;
                }

                fread(&C,sizeof(CursoDelArchivo),1,Archivo);
            }

            fclose(Archivo);
        }
    }
}

void InsertarCurso(NodoCursos *&ListaDeCursos,Cursos Dato)
{
    NodoCursos *Nuevo,*Antecesor,*Aux;
    Nuevo = new NodoCursos;
    Nuevo->Info = Dato;
    Aux = ListaDeCursos;

    while(Aux != NULL && Aux->Info.CodDeCurso < Dato.CodDeCurso)
    {
        Antecesor = Aux;
        Aux = Aux->Sgte;
    }

    Nuevo->Sgte = Aux;

    if(Aux != ListaDeCursos)
    {
        Antecesor->Sgte = Nuevo;
    }
    else
    {
        ListaDeCursos = Nuevo;
    }
}

void InsertarDocente(NodoArbol *&Raiz,Docentes Dato)
{
    NodoArbol *Nuevo = new NodoArbol;
    Nuevo->Info = Dato;
    Nuevo->Der = Nuevo->Izq = NULL;

    if(Raiz == NULL)
    {
        Raiz = Nuevo;
    }
    else
    {
        NodoArbol *Antecesor,*Aux = Raiz;

        while(Aux != NULL)
        {
            Antecesor = Aux;

            if(Dato.Dni < Aux->Info.Dni)
            {
                Aux = Aux->Izq;
            }
            else
            {
                Aux = Aux->Der;
            }
        }
        if(Dato.Dni < Antecesor->Info.Dni)
        {
            Antecesor->Izq = Nuevo;
        }
        else
        {
            Antecesor->Der = Nuevo;
        }
    }
}

void MostrarOperaciones(int &Nro)
{
    int CodDeOperacion;

    cout << "Menu de Opciones" << endl;
    cout << "-----------------" << endl;
    cout << "1. Inscripcion de estudiante" << endl;
    cout << "2. Baja de estudiante" << endl;
    cout << "3. Mostrar" << endl;
    cout << "4. Salir" << endl;
    cout << "-----------------" << endl;
    cout << "Ingrese una opcion: ";
    cin >> CodDeOperacion;

    Nro = CodDeOperacion;
}

void CargaDeAlumnos(NodoCursos *&ListaDeCursos,NodoColaDeEspera *MatrizDeEsperaPrimero[][8],NodoColaDeEspera *MatrizDeEsperaUltimo[][8],Cupos MatrizDeCursos[][8])// Carga de Alumnos:
{
    int CodDeCurso,Nro;
    NodoCursos *Aux;
    NodoAlumno *AuxSub;
    Alumnos A;

    cout << "Informe codigo de curso a inscribirse: ";
    cin >> CodDeCurso;

    Aux = BusquedaDeCurso(ListaDeCursos,CodDeCurso);

    if(Aux != NULL)
    {

        cout << "Informe nombre del estudiante a inscribir: ";
        cin >> A.Nombre;

        cout << "Informe dni del estudiante a inscribir: ";
        cin >> A.Dni;

        AuxSub = BusquedaDeAlumno(Aux->Info.ListaDeAlumnos,A.Dni);

        if(AuxSub == NULL)
        {
            Nro = NumeroDeIdioma(Aux->Info.Idioma);

            if(MatrizDeCursos[Nro][Aux->Info.Nivel - 1].CuposDisponibles > 0)
            {
                InsertarAlumno(Aux->Info.ListaDeAlumnos,A);

                MatrizDeCursos[Nro][Aux->Info.Nivel - 1].CuposDisponibles -= 1;
                MatrizDeCursos[Nro][Aux->Info.Nivel - 1].CuposOcupados += 1;
            }
            else
            {
                cout << "INSCRIPCION RECHAZADA POR FALTA DE CUPO!" << endl;

                InsertarEnColaDeEspera(MatrizDeEsperaPrimero[Nro][Aux->Info.Nivel - 1],MatrizDeEsperaUltimo[Nro][Aux->Info.Nivel - 1],A);
            }
        }
        else
        {
            cout << "INSCRIPCION RECHAZADA, EL ALUMNO YA SE ENCUENTRA INSCRIPTO EN ESTE CURSO!" << endl;
        }
    }
    else
    {
        cout << "NO SE ENCONTRO EL CURSO SOLICITADO!" << endl;
    }

    cout << "-----------------" << endl;
}

void InsertarAlumno(NodoAlumno *&ListaDeAlumnos,Alumnos Dato)
{
    NodoAlumno *Nuevo,*Antecesor,*Aux;
    Nuevo = new NodoAlumno;
    Nuevo->Info = Dato;
    Aux = ListaDeAlumnos;

    while(Aux != NULL && Aux->Info.Dni < Dato.Dni)
    {
        Antecesor = Aux;
        Aux = Aux->Sgte;
    }

    Nuevo->Sgte = Aux;

    if(Aux != ListaDeAlumnos)
    {
        Antecesor->Sgte = Nuevo;
    }
    else
    {
        ListaDeAlumnos = Nuevo;
    }
}

void InsertarEnColaDeEspera(NodoColaDeEspera *&Primero,NodoColaDeEspera *&Ultimo,Alumnos Dato)
{
    NodoColaDeEspera *Nuevo= new NodoColaDeEspera;
    Nuevo->Info = Dato;
    Nuevo->Sgte = NULL;

    if(Ultimo != NULL)
    {
        Ultimo->Sgte = Nuevo;
    }
    else
    {
        Primero = Nuevo;
    }

    Ultimo = Nuevo;
}

void DarDeBajaUnAlumno(NodoCursos *&ListaDeCursos,NodoColaDeEspera *MatrizDeEsperaPrimero[][8],NodoColaDeEspera *MatrizDeEsperaUltimo[][8],Cupos MatrizDeCursos[][8])
{
    bool Verificador;
    int CodDeCurso,Dni,Nro;
    NodoCursos *Aux;
    Alumnos A;

    cout << "Informe codigo de curso: ";
    cin >> CodDeCurso;

    Aux = BusquedaDeCurso(ListaDeCursos,CodDeCurso);

    if(Aux != NULL)
    {
        cout << "Informe dni del alumno a dar a baja: ";
        cin >> Dni;

        Nro = NumeroDeIdioma(Aux->Info.Idioma);
        Verificador = EliminarNodo(Aux->Info.ListaDeAlumnos,Dni);

        if(Verificador)
        {
            if(MatrizDeEsperaPrimero[Nro][Aux->Info.Nivel - 1] != NULL)
            {
                Desencolar(MatrizDeEsperaPrimero[Nro][Aux->Info.Nivel - 1],MatrizDeEsperaUltimo[Nro][Aux->Info.Nivel - 1],A);

                InsertarAlumno(Aux->Info.ListaDeAlumnos,A);
            }
            else
            {
                MatrizDeCursos[Nro][Aux->Info.Nivel - 1].CuposDisponibles += 1;
                MatrizDeCursos[Nro][Aux->Info.Nivel - 1].CuposOcupados -= 1;
            }

            cout << "OPERACION REALIZADA CON EXITO!" << endl;
        }
        else
        {
            cout << "NO SE ENCONTRO EL DNI A DAR A BAJA" << endl;
        }
    }
}

void Desencolar(NodoColaDeEspera *&Primero,NodoColaDeEspera *&Ultimo,Alumnos &Dato)
{
    NodoColaDeEspera *Aux = Primero;
    Dato = Aux->Info;
    Primero = Aux->Sgte;

    delete Aux;

    if(Primero == NULL)
    {
        Ultimo = NULL;
    }
}

void MostrarListado(NodoCursos *ListaDeCursos,Cupos MatrizDeCursos[][8])
{
    int Nro;
    NodoCursos *Aux = ListaDeCursos;
    NodoAlumno *AuxAlumno;

    while(Aux != NULL)
    {
        cout << "Codigo de Curso: " << Aux->Info.CodDeCurso << endl;

        Nro = NumeroDeIdioma(Aux->Info.Idioma);

        cout << "Cantidad de vacantes disponibles: " << MatrizDeCursos[Nro][Aux->Info.Nivel - 1].CuposDisponibles << endl;
        cout << "Cantidad de vacantes ocupadas: " << MatrizDeCursos[Nro][Aux->Info.Nivel - 1].CuposOcupados << endl;
        cout << "Listado de alumnos inscriptos: " << endl;

        AuxAlumno = Aux->Info.ListaDeAlumnos;

        while(AuxAlumno != NULL)
        {
            cout << "Alumno: " << AuxAlumno->Info.Nombre << " - " << "Dni: " << AuxAlumno->Info.Dni << endl;

            AuxAlumno = AuxAlumno->Sgte;
        }

        cout << "-----------------" << endl;

        Aux = Aux->Sgte;
    }
}

void ListarInOrden(NodoCursos *ListaDeCursos,NodoArbol *Raiz,Cupos MatrizDeCursos[][8])
{
    int Nro,CantTotalDeAlumnos = 0;
    NodoCursos *Aux = ListaDeCursos;

    if(Raiz != NULL)
    {
        ListarInOrden(ListaDeCursos,Raiz->Izq,MatrizDeCursos);

        while(Aux != NULL)
        {
            Nro = NumeroDeIdioma(Aux->Info.Idioma);

            if(Raiz->Info.Dni == MatrizDeCursos[Nro][Aux->Info.Nivel - 1].DniDelDocente)
            {
                CantTotalDeAlumnos += MatrizDeCursos[Nro][Aux->Info.Nivel - 1].CuposOcupados;
            }

            Aux = Aux->Sgte;
        }

        cout << "Nombre del Docente: " << Raiz->Info.NombreDelDocente << " - " << "Dni del docente: " << Raiz->Info.Dni << " - " << "Cantidad total de alumnos a cargo: " << CantTotalDeAlumnos << endl;

        ListarInOrden(ListaDeCursos,Raiz->Der,MatrizDeCursos);
    }
}

void MostrarListadoDeRechazados(NodoCursos *&ListaDeCursos,NodoColaDeEspera *MatrizDeEsperaPrimero[][8],NodoColaDeEspera *MatrizDeEsperaUltimo[][8])
{
    int Nro;
    NodoCursos *Aux = ListaDeCursos;
    Alumnos A;

    while(Aux != NULL)
    {
        cout << "Codigo de curso: " << Aux->Info.CodDeCurso << endl;

        Nro = NumeroDeIdioma(Aux->Info.Idioma);

        while(MatrizDeEsperaPrimero[Nro][Aux->Info.Nivel - 1] != NULL)
        {
            Desencolar(MatrizDeEsperaPrimero[Nro][Aux->Info.Nivel - 1],MatrizDeEsperaUltimo[Nro][Aux->Info.Nivel - 1],A);

            cout << "Nombre del alumno: " << A.Nombre << " - " << "Dni del alumno: " << A.Dni << endl;
        }

        cout << "-----------------" << endl;
        Aux = Aux->Sgte;
    }
}

int NumeroDeIdioma(string Idioma)
{
    int Nro;

    if(Idioma == "Ingles")
    {
        Nro = 0;
    }
    else if(Idioma == "Frances")
    {
        Nro = 1;
    }
    else if(Idioma == "Portugues")
    {
        Nro = 2;
    }
    else if(Idioma == "Italiano")
    {
        Nro = 3;
    }
    else if (Idioma == "Aleman")
    {
        Nro = 4;
    }
    else if(Idioma == "Chino")
    {
        Nro = 5;
    }

    return Nro;
}

bool EliminarNodo(NodoAlumno *&ListaDeAlumnos,int Dni)
{
    bool Verificador = false;
    NodoAlumno *Aux;
    NodoAlumno *Aux2;
    Aux = ListaDeAlumnos;

    while(Aux != NULL)
    {
        if(Aux->Info.Dni == Dni)
        {
            if(Aux == ListaDeAlumnos)
            {
                ListaDeAlumnos = Aux->Sgte;
            }
            else
            {
                Aux2->Sgte = Aux->Sgte;
            }

            delete Aux;

            Verificador = true;
        }

        Aux2 = Aux;
        Aux = Aux->Sgte;
    }

    return Verificador;
}

FILE *CambioDeArchivo(int Nro)
{
    switch(Nro)
    {
        case 0:
            {
                FILE *ArchivoA = fopen("Ingles.dat","rb");
                return ArchivoA;
                break;
            }
        case 1:
            {
                FILE *ArchivoB = fopen("Frances.dat","rb");
                return ArchivoB;
                break;
            }
        case 2:
            {
                FILE *ArchivoC = fopen("Aleman.dat","rb");
                return ArchivoC;
                break;
            }
        case 3:
            {
                FILE *ArchivoD = fopen("Chino.dat","rb");
                return ArchivoD;
                break;
            }
        case 4:
            {
                FILE *ArchivoE = fopen("Italiano.dat","rb");
                return ArchivoE;
                break;
            }
        case 5:
            {
                FILE *ArchivoF = fopen("Portugues.dat","rb");
                return ArchivoF;
                break;
            }
        default:
            {
                return NULL;
            }
    }
}

NodoArbol *BusquedaDeDocente(NodoArbol *Raiz,int Dni)
{
   NodoArbol *Aux = Raiz;

   while(Aux != NULL && Aux->Info.Dni != Dni)
   {
       if(Dni < Aux->Info.Dni)
       {
            Aux = Aux->Izq;
       }
       else
       {
            Aux = Aux->Der;
       }
   }

   return Aux;
}

NodoCursos *BusquedaDeCurso(NodoCursos *ListaDeCursos,int CodDeCurso)
{
    NodoCursos *Aux;
    Aux = ListaDeCursos;

    while(Aux != NULL && Aux->Info.CodDeCurso < CodDeCurso)
    {
        Aux = Aux->Sgte;
    }

    if(Aux == NULL || Aux->Info.CodDeCurso != CodDeCurso)
    {
        return NULL;
    }
    else
    {
        return Aux;
    }
}

NodoAlumno *BusquedaDeAlumno(NodoAlumno *ListaDeAlumnos,int Dni)
{
    NodoAlumno *Aux;
    Aux = ListaDeAlumnos;

    while(Aux != NULL && Aux->Info.Dni < Dni)
    {
        Aux = Aux->Sgte;
    }

    if(Aux == NULL || Aux->Info.Dni != Dni)
    {
        return NULL;
    }
    else
    {
        return Aux;
    }
}
