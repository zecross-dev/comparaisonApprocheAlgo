/**
 * @file main.cpp
 * @brief
 * @author Ronan Champagnat - IUT Informatique
 * @date 10/11/2023
 */


#include <iostream>

using namespace std;

#include "typeDef.h"
#include "functions.h"
#include "tests.h"
#include <fstream>


/**
* @brief Function to execute the analysis of the log file.
**/
void launchProcessAnalysis()
{
    // On définit le fichier à analyser (smallDataset.txt ou largeDataset.txt)
    string fileName = "largeDataset.txt";

    // 1. Initialisation de la liste principale
    ProcessList * mainList = new ProcessList;
    mainList->size = 0;
    mainList->firstProcess = nullptr;

    cout << "--- Debut de l'analyse du fichier : " << fileName << " ---" << endl;

    // 2. Extraction des processus
    auto startExtract = getTime();
    extractProcesses(mainList, fileName);
    auto endExtract = getTime();

    // 3. Affichage des statistiques de base
    cout << "Nombre de processus : " << mainList->size << endl;
    cout << "Longueur moyenne des processus : " << averageProcessLength(mainList) << endl;
    cout << "Duree d'extraction : " << calculateDuration(startExtract, endExtract) << "s" << endl;

    // 4. Recherche des activités de début
    cout << "Debut de la recherche des activites de debut" << endl;
    Process * startActs = new Process;
    startActivities(mainList, startActs);
    displayActivitiesList(startActs);

    // 5. Recherche des activités de fin
    cout << "Debut de la recherche des activites de fin" << endl;
    Process * endActs = new Process;
    endActivities(mainList, endActs);
    displayActivitiesList(endActs);

    // 6. Recherche des variants (la partie la plus gourmande en calcul)
    cout << "Debut de la recherche des variants" << endl;
    ProcessList * variantList = new ProcessList;
    variantList->size = 0;
    variantList->firstProcess = nullptr;

    auto startVar = getTime();
    variants(mainList, variantList);
    auto endVar = getTime();

    cout << "Duree de recherche des variants : " << calculateDuration(startVar, endVar) << "s" << endl;
    displayProcessesList(variantList);

    clear(mainList);    // Libère tous les processus
    clear(variantList); // Libère tous les variants

    // Pour les objets simples
    clear(startActs);
    clear(endActs);

    // Suppression des conteneurs
    delete mainList;
    delete variantList;

    cout << "--- Fin de l'analyse ---" << endl;
}

/**
* @brief Launches the testing suite.
* This function runs a suite of tests to validate the functions.
*/
void launchTests()
{
    cout << endl << "********* Start testing *********" << endl << endl;
    // Here you can use tests functions
    test_nbOfLines();
    test_printProgressBar();
    test_displayActivitiesList();
    test_displayProcessesList();
    test_push_back();
    test_addActivity();
    test_push_front();
    test_addProcess();
    test_insertProcessActivity();
    test_processExists();
    test_extractProcesses();
    test_averageProcessLength();
    test_insertActivity();
    test_startActivities();
    test_endActivities();
    test_processAlreadyExists();
    test_variants();
    cout << endl << "********** End testing **********" << endl << endl;
}

/**
* @brief Main function of the program.
* Entry point to process analysis. It can be used to start the analysis or run the tests.
* @return 0 for successful execution.
*/
int main()
{
    cout << "Prenom Nom groupeTP" << endl;

    // Uncomment the line below to run tests
    //launchTests();
    // Start the process analysis
    launchProcessAnalysis();

    return 0;
}
