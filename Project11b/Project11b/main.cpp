#include "CGApplication.h"
#include <iostream>
#include <stdexcept>

//
//// PROYECTO: Project11b
//// DESCRIPCIÓN: Aplicación gráfica que incluye el cálculo de sombras
////              con la técnica de percentage-closer filtering (PCF).
//
int main()
{
    CGApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}