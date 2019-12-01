#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//-----ESTRUCTURA-----
struct dogType
{
    unsigned long int id;
    char nombre[32];
    char tipo[32];
    int edad;
    char raza[16];
    int estatura;
    float peso;
    char sexo[3];
};

//apuntadores de la estrucura
struct dogType *data;
struct dogType *dato;

unsigned long int NumeroRegistros(){ //Devuelve el número de registros que hay en el archivo.dat
    FILE *f; 
    f = fopen("dataDogs.dat", "r"); //Abre el archivo dataDogs

    fseek(f, 0, SEEK_END); //Salta a la ultima línea del fichero
    unsigned long int id = ftell(f); //Indica la posición actual en un fichero
    
    id = id/100;

    return id; //Devuelve el numero de registros
}

//-----INGRESAR-----
void cargar(void *ap)
{

    dato = ap; //Asigna dato a la posicion de memoria de ap
    printf("Nombre: ");
    scanf(" %s", dato->nombre);
    printf("Tipo: ");
    scanf(" %s", dato->tipo);
    printf("Edad: ");
    scanf(" %u", &dato->edad);
    printf("Raza: ");
    scanf(" %s", dato->raza);
    printf("Estatura (cm): ");
    scanf(" %u", &dato->estatura);
    printf("Peso (Kg): ");
    scanf(" %u", &dato->peso);
    printf("Sexo (H/M): ");
    scanf(" %s", dato->sexo);

    return 0;
}

int guardar(void *ap) //Guarda la informacion ingresada por consola en el archivo dataDogs.dat
{
    dato = ap;
    unsigned long int ID = NumeroRegistros()+1;  //Variable entera que guarda el id correspondiente 

    printf("ID: %d\n", ID); //Muestra el id de cada registro a guardar
    dato->id = ID; //Asigna el Id a la estructura

    FILE *f;
    f = fopen("dataDogs.dat", "a+"); //Abre dataDogs
    if (f == NULL)
    {
        perror("error abriendo");
        exit(-1);
    }
    int r;
    r = fwrite(dato, sizeof(struct dogType), 1, f); //Escribe cada estructura en una linea del archivo

    if (r == 0)
    {
        perror("error escribiendo");
        exit(-1);
    }

    fclose(f); //Cierra Datadogs
    return 0;
}

void Ingresar() //Guarda la informacion en memoria
{
    char conf;

    data = malloc(sizeof(struct dogType)); // Reserva una cierta cantidad de memoria del tamaño de la estructura

    if (data == NULL)
    {
        perror("error en malloc");
        exit(-1);
    }

    cargar(data); //Carga los datos 

    printf("Confirmar información (S/N): ");
    scanf(" %c", &conf);

    if (conf == 'S' || conf == 's')
    {
        guardar(data);
        printf("Guardo.\n");
    }else
    {
        printf("No guardó.\n");
    }

    free(data); //Libera un bloque de memoria que se haya reservado previamente con "malloc"
}


//-----VER-----

void Ver() //Muestra el numero de registros en el archivo
{
    printf("Número de registros disponibles: %d \n", NumeroRegistros());
    
    struct dogType dat;
    char idsearch[30]; //Variable para buscar
    int existe = 0;
    char conf;

    printf("Ingrese el id a buscar: \n"); //Solicita un  id para buscarlo en el archivo
    scanf("%s", idsearch);

    FILE *f;
    f = fopen("dataDogs.dat", "r"); //Abre dataDogs

    if (f == NULL)
    {
        perror("Error abriendo");
        exit(-1);
    }
    dato = &dat;

    unsigned long int ID = StringaEntero(idsearch); //Convierte idsearch en una variable de tipo entero

    while (!feof(f)) //Ejecuta mientras encuentra el final del archivo
    {
        if(ID == dat.id) //Compara el id a buscar con los ids de las estructuras
        {  
            existe = 1; 
            printf("Nombre = %s\n Tipo = %s\n Edad = %i\n Raza = %s\n Estatura (cm) = %i\n Peso (kg) = %.2f\n Sexo = %s\n ID = %d\n", 
            dat.nombre, dat.tipo, dat.edad, dat.raza, dat.estatura, dat.peso, dat.sexo, dat.id); //Muestra la informacion ligada al id buscado
        }

        fread(&dat, sizeof(struct dogType), 1, f); //Lee el archivo linea por linea   
    }

    fclose(f); //cierra el archivo

    if (existe != 0) //Verifica que exista el id buscado dentro del archivo
    {
        printf("Confirmar información (S/N): ");
        scanf(" %c", &conf);

        if (conf == 'S' || conf == 's')
        {
            //Abre un documento de texto con la historia del animal asociado al ID buscado
            char comandotouch[30]="touch Historias/";
            char comandognome[30]="gnome-open Historias/"; //sudo apt install libgnome2-bin
            char tipo[5] = ".txt";
            strcat(idsearch, tipo);
            strcat(comandotouch, idsearch);
            strcat(comandognome, idsearch);
            system(comandotouch);
            system(comandognome);   
        }

    }else //El id buscado no existe
    {
        printf("No existen registros con el ID digitado");
    }
}

void Ver2() //Muestra todos los registros 
{
    struct dogType dat;
    FILE *f;
    f = fopen("dataDogs.dat", "r"); //Abre dataDogs 
    if (f == NULL)
    {
        perror("Error abriendo");
        exit(-1);
    }
    fread(&dat, sizeof(struct dogType), 1, f); //Lee dataDogs linea a linea 
    dato = &dat;

    while (!feof(f))
    {
        printf("Nombre = %s\n Tipo = %s\n Edad = %i\n Raza = %s\n Estatura (cm) = %i\n Peso (kg) = %.2f\n Sexo = %s\n ID = %u\n\n", 
        dat.nombre, dat.tipo, dat.edad, dat.raza, dat.estatura, dat.peso, dat.sexo, dat.id); //Imprime en consola la informacion 
        fread(&dat, sizeof(struct dogType), 1, f); //lee la linea siguiente
    }

    fclose(f);   
}

//------BUSCAR--------

int Buscar() //Busca los registros que tengan un nombre indicado por el usuario
{
    struct dogType dat;
    char search [32] = "";

    printf("Ingrese el nombre a buscar: \n"); //Solicita el nombre que se desea buscar
    scanf(" %s", search);
    ConvertiraMinusculas(search); //Pasa el nombre ingresado a minusculas

    FILE *f;
    f = fopen("dataDogs.dat", "r"); //Abre dataDogs
    if (f == NULL)
    {
        perror("Error abriendo");
        exit(-1);
    }
    fread(&dat, sizeof(struct dogType), 1, f);
    dato = &dat;

    while (!feof(f)) //Ejecuta hasta que encuentra el final del archivo
    {
        ConvertiraMinusculas(dat.nombre); //Pasa el nombre almacenado en las estructuras a minusculas

        if(strcmp(search,dat.nombre) == 0) //Compara el nombre ingresado con los nombres registrados en dataDogs
        {  
            printf("Nombre = %s\n Tipo = %s\n Edad = %i\n Raza = %s\n Estatura (cm) = %i\n Peso (kg) = %.2f\n Sexo = %s\n ID = %d\n\n", 
            dat.nombre, dat.tipo, dat.edad, dat.raza, dat.estatura, dat.peso, dat.sexo, dat.id); //Muestra en la consola la informacion asociada a los nombres 
        }

        fread(&dat, sizeof(struct dogType), 1, f); //Lee el archivo linea por linea
    }

    fclose(f);

    return 0;
}

//-----BORRAR-----

int Borrar() //Borra el registro identificado con un ID proporcionado por el usuario
{
    struct dogType dat;
    int iddelete; //Variable entera para almacenar el id a borrar

    printf("Ingrese el id a borrar: \n"); //Solicita al usuario el Id que desea borrar
    scanf("%i",&iddelete);

    FILE *f, *aux; 
    f = fopen("dataDogs.dat", "rb+"); //Abre el archivo dataDogs
    aux = fopen("auxiliar.dat", "wb+"); //Abre un archivo auxiliar 

    if (f == NULL)
    {
        perror("Error abriendo");
        exit(-1);
    }
    
    fread(&dat, sizeof(struct dogType), 1, f); //Lee dataDogs
    dato = &dat;

    while (!feof(f)) //Ejecuta hasta encontrar el final del archivo
    {
        fread(&dat, sizeof(struct dogType), 1, f); //Lee dataDogs linea por linea

        if (dat.id != iddelete) //Verifica que el id de la estructura sea distinto al ingresado por el usuario para eliminar
            fwrite(&dat, sizeof(struct dogType),1, aux); //Escribe el registro en el archivo auxiliar
    }

    fclose(f); //Cierra dataDogs
    fclose(aux); //Cierra el auxiliar
    rename("dataDogs.dat", "a.dat"); //Cambia el nombre de datadogs
    rename("auxiliar.dat", "dataDogs.dat"); //Cambia el nombre de auxiliar a dataDogs con los registros excepto el que se deseaba eliminar
    remove("a.dat"); //Elimina el archivo que antes era dataDogs

}

//-----ADICIONAL---------

int longitudString(char *cadena) //Calcula la longitud de un string 
{
    int valor = 0;
    int i;

    for(i=0; cadena[ i ]!='\0'; i++) //recorre el string caracter por caracter 
        valor++; //Suma uno al contador por cada caracter

    return valor; //Devuelve la longitud de la cadena
}

int StringaEntero(char *cadena) //Convierte un string en entero
{
    int potencias[5]={1,10,100,1000,10000};
    int i;
    int valor = 0;
    int lon = longitudString(cadena);

    for(i=lon-1; i>=0; i--)
        valor += (cadena[ i ]-'0') * potencias[lon-i-1];

    return valor;
}

void ConvertiraMinusculas(char *cadena) //Convierte todos los caracteres de una cadena en minusculas
{
    for (int i = 0; cadena[i] != '\0'; i++) //recorre el string caracter por caracter
    {
        cadena[i] = tolower(cadena[i]); //transforma cada caracter en minuscula
    }
}
