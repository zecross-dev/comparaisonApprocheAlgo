/**
 * @file functions.cpp
 * @brief Implementation of the project functions
 * @author Ronan Champagnat - IUT Informatique La Rochelle
 * @date 10/11/2023
 */

#include "typeDef.h"

#include <iostream>
#include <fstream>
#include <functions.h>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

using namespace std;


/*
 * Chronographe functions
 */

/**
 * @brief Retourne l'heure courante avec une grande résolution
 */
chrono::time_point<std::chrono::high_resolution_clock> getTime()
{
    return chrono::high_resolution_clock::now();
}

/**
 * @brief Retourne la différence entre deux heures données et convertit en secondes.
 */
double calculateDuration(chrono::time_point<std::chrono::high_resolution_clock> start, chrono::time_point<std::chrono::high_resolution_clock> end)
{
    //calcule de la différence entre end et start
    std::chrono::duration<double> elapsed = end - start;
    //retourne le résultat sous forme de nombre
    return elapsed.count();
}


/*
 * Utility functions
 */

/**
 * @brief Ouvre le fichier passé en paramètres et le parcours intégralement en comptant le nombre
 * de lignes. Le fichier est fermé avant de retourner le nombre de lignes trouvées.
 */
int nbOfLines(string aFileName)
{
    ifstream Ofile(aFileName);
    int count =0;
    string line;
    if (Ofile.is_open()){
        while(getline(Ofile,line)){
            count++;
        }
    }
    else{
        cerr<< "Erreur impossible d'ouvrir le fichier";
        return -1;
    }
    return count;
}

/**

@brief Affiche une barre de progression au format :
[############                  ] 42%
Utilisation de \r (voir premier cours de Dév)
le nombre de # est proportionnel nb (nombre d'itérations) par rapport à max
le pourcentage est la proportion de nb sur max*/
void printProgressBar(int nb, int max)
{
    // "static" permet à la variable de garder sa valeur entre deux appels
    static int dernierPourcent = -1;

    if (max <= 0) return;

    // Le test attend une largeur de 30 caractères
    int largeurBarre = 30;
    float pourcent = (float)nb / max * 100.0;
    int pourcentActuel = (int)pourcent;

    // Calcul de la position des '#'
    int position = (nb * largeurBarre) / max;

    // Le test compare chaque étape (de 0 à 14), on ne filtre donc pas l'affichage
    cout << "\r[";

    for(int i = 0; i < largeurBarre; i++ )
    {
        if(i < position)
            cout << "#";
        else
            cout << " ";
    }

    cout << "] " << pourcentActuel << "%";

    // Le test attend un '\n' uniquement quand on arrive à 100%
    if(nb == max)
    {
        cout << endl;
        dernierPourcent = -1; // Réinitialisation
    }

    cout.flush();
}
/*
 * Utility functions for data structure
 */

/**
 * @brief Parcours une liste de type Process afin de libérer la mémoire de toutes les activités
 * puis libère la mémoire de la liste de type Process
 * Chaque élément libéré est mis à pointer sur nullpointer
 */
void clear(Process * aProcess)
{
    if (aProcess == nullptr) return;

    // 1. Libération des activités
    Activity* currentA = aProcess->firstActivity;
    while (currentA != nullptr) {
        Activity* nextA = currentA->nextActivity;
        delete currentA;
        currentA = nextA;
    }
    // Sécurité : on met à nul avant de supprimer le container
    aProcess->firstActivity = nullptr;
    aProcess->lastActivity = nullptr;

    delete aProcess;
}
/**
 * @brief Parcours tous les processus d'une liste et applique clear sur chaque processus
 * puis libère la mémoire de la liste et fait pointer la liste sur nullptr
 */
void clear(ProcessList * aList)
{
    if (aList == nullptr || aList->firstProcess == nullptr) return;

    Process * currentP = aList->firstProcess;
    while (currentP != nullptr) {
        Process * nextP = currentP->nextProcess;
        clear(currentP); // Appelle la version ci-dessus
        currentP = nextP;
    }

    // CRUCIAL : Remettre la tête de liste à NULL pour éviter un double delete
    aList->firstProcess = nullptr;
    aList->size = 0;
}
/**
 * @brief Affiche le nombre d'activités contenues dans le process
 * puis parcours toutes les activités pour les afficher (nom uniquement)
 * comme suit : Nb activités : 3: a b c \n
 */
void displayActivitiesList(Process * aProcess)
{
    if (aProcess == nullptr) {
        cerr << "processus invalide.";
        return;
    }
    //affichage de l'entete du processus
    cout << "Nb activités : " << aProcess->nbActivities << ": ";
    //initialisation du pointeur de parcours sur la tête de la liste des activités
    Activity * currentActivity = aProcess->firstActivity;
    //parcours de la liste chaînée
    while (currentActivity != nullptr) {
        cout << currentActivity->name << " ";

        //maj pointeur
        currentActivity = currentActivity->nextActivity;
    }
    cout << endl;
}
/**
 * @brief Affiche le nombre de processus contenus dans une liste puis parcours la liste
 * pour afficher l'id de chaque processus et ses activités (utilise displayActivitiesList)
 * comme suit : Nombre de processus : 3\n123:\tNb activités : 3: a b c \n456:\tNb activités : 1: b \n789:\tNb activités : 2: a b \n\n
 */
void displayProcessesList(ProcessList * aList){
    if (aList == nullptr) {
        cerr << "Liste de processus invalide.";
        return;
    }
    //affichage de l'entete
    cout << "Nombre de processus : " << aList->size << endl;

    Process * currentProcess = aList->firstProcess;
    while (currentProcess != nullptr) {
        //affiche l'ID
        cout << currentProcess->id << ":\t";
        displayActivitiesList(currentProcess);
        //passage au processus suivant
        currentProcess = currentProcess->nextProcess;
    }

    // Saut de ligne final supplémentaire requis par la chaîne de test
    cout << endl;
}
/**
 * @brief Si la liste de processus ne contient aucune activité, l'activité est ajoutée en tête
 * sinon la liste des activités est parcourue et l'activité est ajoutée en queue.
 * Le nombre d'activités du processus est incrémenté de 1
 */
void push_back(Process * aProcess, Activity * anActivity) {
    if (aProcess->firstActivity == nullptr) {
        aProcess->firstActivity = anActivity;
    } else {
        aProcess->lastActivity->nextActivity = anActivity;
    }
    aProcess->lastActivity = anActivity; // On met à jour la fin instantanément
    aProcess->nbActivities++;
}
/**
 * @brief Construit un pointeur de type Activity et l'initialise aux valeurs données
 * puis utilise push_back pour ajouter l'activité à la fin de la liste (au processus)
 */
void addActivity(Process * aProcess, const string& name, const string& time) {
    Activity* newA = new Activity;
    newA->name = name;
    newA->time = time;
    newA->nextActivity = nullptr;

    if (aProcess->firstActivity == nullptr) {
        aProcess->firstActivity = newA;
    } else {
        // On utilise le pointeur direct vers la fin
        aProcess->lastActivity->nextActivity = newA;
    }
    // On met à jour le nouveau dernier élément
    aProcess->lastActivity = newA;
    aProcess->nbActivities++;
}
/**
 * @brief Ajoute un processus (Process) en tête de liste ProcessList
 */
void push_front(ProcessList * aList, Process * aProcess)
{
    if (aList == nullptr || aProcess == nullptr) {
        cerr <<"erreur processus invalide";
        return;
    }
    aProcess->nextProcess = aList->firstProcess; //met le pointeur du processus a ajouter sur le processus de tête actuel
    aList->firstProcess = aProcess; //change la tête de la liste
    aList->size++;
}

/**
 * @brief Construit un pointeur de type Process, positionne le champ id à la valeur donnée
 * puis utilise addActivity pour ajouter l'activité passée en paramètre au processus créé
 * puis utilise push_front pour ajouter le processus à la liste de processus
 */
void addProcess(ProcessList * aList, int aProcessId, string anActivityName, string aTime)
{
    if (aList == nullptr) {
        cerr << "liste de processus invalide";
        return;
    }
    Process * current = aList->firstProcess;
    Process * targetProcess = nullptr; //initialise le processus cible a nullptr
    while (current != nullptr) {
        if (current->id == aProcessId) {//test si l'id du processus est
            targetProcess = current;
            break; //casse la boucle (innutile de continuer)
        }
        current = current->nextProcess;
    }
    if (targetProcess == nullptr) {
        targetProcess = new Process; //créé un nouveau processus
        targetProcess->id = aProcessId; //initialise l'id
        targetProcess->nbActivities = 0;
        targetProcess->firstActivity = nullptr;
        targetProcess->lastActivity = nullptr;
        targetProcess->nextProcess = nullptr;

        push_front(aList, targetProcess);
    }

    addActivity(targetProcess, anActivityName, aTime);
}

/**
 * @brief Parcours la liste des processus
 * quand le processus est trouvé (comparaison des id à aProcessID) on utilise addActivity pour ajouter
 * l'activité donnée au processus trouvé
 */
void insertProcessActivity(ProcessList * aList, int aProcessId, string anActivityName, string aTime)
{
    if (aList == nullptr) {
        cerr << "erreur : liste de processus invalide.";
        return;
    }

    Process* p = processExists(aList, aProcessId);
    //si le processus n'existe pas on le créé et on l'ajoute a la liste
    if (p == nullptr) {
        p = new Process;
        p->id = aProcessId;
        p->nbActivities = 0;
        p->firstActivity = nullptr;
        p->lastActivity = nullptr;
        p->nextProcess = nullptr;
        push_front(aList, p);
    }

    addActivity(p, anActivityName, aTime);
}

/**
 * @brief Parcours la liste des processus
 * Si un processus correspond au process Id donné retourner un pointeur vers ce processus
 * retourne nullptr sinon
 */
Process * processExists(ProcessList * aList, int aProcessId)
{
    if (aList == nullptr) return nullptr;

    Process * current = aList->firstProcess;

    while (current != nullptr) {
        if (current->id == aProcessId) {
            return current;
        }
        current = current->nextProcess;
    }
    return nullptr;
}


/*
 * Processes Functions
 */

/**
 * @brief Affiche la taille du fichier à analyser (en utilisant nbOfLines)
 * Puis parcours le fichier
 * met à jour la barre de progression en utilisant pringProgressBar
 * utilise un flux de fichier entrant pour récupérer l'identifiant du processus,
 * le nom de l'activité et la date de l'activité
 * si le processus existe (id déjà présent- pour cela on utilise processExist)
 * l'activité est ajoutée au processus trouvé en utilisant insertProcessActivity
 * ajoute le processus à la liste des processus sinon en utilisant addProcess
 * Ne pas oublier de fermer le fichier
 */
void extractProcesses(ProcessList* aList, string aFileName) {
    ifstream file(aFileName, ios::binary | ios::ate);
    if (!file.is_open()) return;

    streamsize totalSize = file.tellg();
    file.seekg(0, ios::beg);

    // OPTIMISATION : Utiliser l'itérateur pour éviter la double recherche
    unordered_map<int, Process*> cache;
    int id;
    string name, time;
    int count = 0;

    while (file >> id >> name >> time) {
        // OPTIMISATION : On affiche seulement toutes les 20 000 lignes
        if (++count % 20000 == 0) {
            printProgressBar((int)file.tellg(), (int)totalSize);
        }

        auto it = cache.find(id); //Recherche l'ID : O(1) grâce au hachage
        if (it == cache.end()) { //si l'id n'existe pas
            Process* newP = new Process;
            newP->id = id;
            newP->nbActivities = 0;
            newP->firstActivity = nullptr;
            newP->lastActivity = nullptr;
            newP->nextProcess = nullptr;
            push_front(aList, newP);
            cache[id] = newP;
            addActivity(newP, name, time);
        }
        else {
            // Accès direct via l'itérateur pour passer de complexité O(n) à O(1)
            addActivity(it->second, name, time);
        }
    }
    printProgressBar(100, 100);
}
/**
 * @brief Génère une empreinte textuelle (signature) unique d'un processus
 * Parcours la liste des activités du processus et concatène leurs noms
 * en utilisant un séparateur '|'.
 * Cette signature permet une comparaison rapide entre processus (O(1) via Hash)
 * au lieu d'un parcours de liste chaînée (O(n)).
 * @param p : pointeur vers le processus à signer
 * @return string : la chaîne de caractères représentant la séquence d'activités
 */
string getProcessSignature(Process* p) {
    string signature;
    signature.reserve(p->nbActivities * 12); //optimisation : alloue la mémoire en une fois pour éviter les copies coûteuses.
    Activity* currentP = p->firstActivity;
    while (currentP) {
        signature += currentP->name;
        signature += "|";
        currentP = currentP->nextActivity;
    }
    return signature;
}

/**
 * @brief Initialise une variable somme
 * Parcours la liste des processus puis ajoute le nombre d'activités de chaque processus
 * à la variable somme
 * retourne la moyenne somme/nombre de processus en réel
 */
double averageProcessLength(ProcessList * aList)
{
    //vérification pour éviter la division par zéro
    if (aList == nullptr || aList->size == 0 || aList->firstProcess == nullptr) {
        return 0.0;
    }
    int totalActivities = 0;
    Process * current = aList->firstProcess;
    // Parcours de la liste des processus pour sommer les activités
    while (current != nullptr) {
        totalActivities += current->nbActivities;
        current = current->nextProcess;
    }
    // Calcul de la moyenne en convertissant en double pour un résultat réel
    return static_cast<double>(totalActivities) / aList->size;
}

/**
 * @brief Insère une activité dans un processus en respectant l'ordre croissant sur le nom des activités
 * et à condition que le nom de l'activité n'existe pas déjà
 */
void insertActivity(Process * aProcess, Activity* anActivity)
{
    if (aProcess == nullptr || anActivity == nullptr) return;
    //vérif doublons et recherche position d'insertion
    Activity * current = aProcess->firstActivity;
    Activity * prev = nullptr;
    while (current != nullptr && current->name < anActivity->name) {
        prev = current;
        current = current->nextActivity;
    }
    //si on trouve un doublon on insére pas
    if (current != nullptr && current->name == anActivity->name) {
        delete anActivity; // Libération de l'activité qui ne sera pas insérée
        return;
    }
    //insertion au début si la liste est vide ou si l'élément va au debut
    if (prev == nullptr) {
        anActivity->nextActivity = aProcess->firstActivity;
        aProcess->firstActivity = anActivity;
    }
    //insertion milieu/fin
    else {
        anActivity->nextActivity = current;
        prev->nextActivity = anActivity;
    }
    aProcess->nbActivities++;
}

/**
 * @brief Parcours la liste des processus
 * ajoute la première activité de chaque processus en utilisant
 * insertActivity (pas de doublons)
 */
void startActivities(ProcessList * aProcessList, Process * anActivityList)
{
    if (aProcessList == nullptr || anActivityList == nullptr) return;
    //parcourt la liste des processus
    Process * currentP = aProcessList->firstProcess;
    while (currentP != nullptr) {
        //vérifie si le processus actuel possède une activité
        if (currentP->firstActivity != nullptr) {
            //copie de la première activité pour ne pas modifier l'originale
            Activity * copy = new Activity;
            copy->name = currentP->firstActivity->name;
            copy->time = currentP->firstActivity->time;
            copy->nextActivity = nullptr;
            // Tentative d'insertion
            insertActivity(anActivityList, copy);
        }
        currentP = currentP->nextProcess;
    }
}

void endActivities(ProcessList * aProcessList, Process * anActivityList)
{
    if (aProcessList == nullptr || anActivityList == nullptr) {
        cerr<<"erreur : liste de processus invalide";
        return;
    }
    //parcourt la liste principale des processus
    Process * currentP = aProcessList->firstProcess;
    while (currentP != nullptr) {
        //vérifie que le processus possède une activité
        if (currentP->firstActivity != nullptr) {
            //recherche la dernière activité de ce processus
            Activity * lastA = currentP->firstActivity;
            while (lastA->nextActivity != nullptr) {
                lastA = lastA->nextActivity;
            }
            //création copie de la dernière activité
            Activity * copy = new Activity;
            copy->name = lastA->name;
            copy->time = lastA->time;
            copy->nextActivity = nullptr;
            //insertion liste globale
            insertActivity(anActivityList, copy);
        }
        currentP = currentP->nextProcess;
    }
}

/**
 * @brief Parcours la liste des processus
 * pour chaque processus parcours la liste des activités et la compare avec la 
 * liste des activités du processus donné
 * retourne vrai si un processus identique est trouvé, faux sinon
 */
bool processAlreadyExists(ProcessList * aProcessList, Process * aProcess)
{
    if (aProcessList == nullptr || aProcess == nullptr) return false;
    //parcourt chaque processus de la liste
    Process * currentP = aProcessList->firstProcess;
    while (currentP != nullptr) {
        //optimisation : si le nombre d'activités diffère, ils ne sont pas identiques
        if (currentP->nbActivities == aProcess->nbActivities) {
            Activity * act1 = currentP->firstActivity;
            Activity * act2 = aProcess->firstActivity;
            bool identical = true;
            //comparaison de la séquence d'activités (nom par nom)
            while (act1 != nullptr && act2 != nullptr) {
                if (act1->name != act2->name) {
                    identical = false;
                    break;
                }
                act1 = act1->nextActivity;
                act2 = act2->nextActivity;
            }

            //si on a parcouru les deux listes sans trouver de différence
            if (identical && act1 == nullptr && act2 == nullptr) {
                return true;
            }
        }
        currentP = currentP->nextProcess;
    }

    return false;
}
/**
 * @brief Parcours la liste des processus
 * affiche la barre de progression en utilisant pringProgressBar
 * pour chaque processus vérifie qu'il n'est pas déjà dans la lite des variants
 * (utilise processAlreadyExists)
 * si c'est un nouveau variant créer un nouveau processus
 * y ajouter toutes les activités en utilisant addActivity
 * puis utiliser push_front pour ajouter le processus à la liste des variants
 */
void variants(ProcessList * aProcessList, ProcessList * aVariant) {
    std::unordered_set<std::string> seenSignatures;
    seenSignatures.reserve(aProcessList->size);

    Process* currP = aProcessList->firstProcess;
    int count = 0;
    while (currP) {
        //optimisation de l'affichage de la barre de progression pour limiter son appel
        if (++count % 1000 == 0) printProgressBar(count, aProcessList->size);

        string sig = getProcessSignature(currP);

        if (seenSignatures.insert(sig).second) {
            //initialisation explicite de tous les pointeurs à nullptr
            Process* newV = new Process;
            newV->id = currP->id;
            newV->nbActivities = 0;
            newV->firstActivity = nullptr;
            newV->lastActivity = nullptr;
            newV->nextProcess = nullptr;

            Activity* currA = currP->firstActivity;
            while (currA) {
                addActivity(newV, currA->name, currA->time);
                currA = currA->nextActivity;
            }
            push_front(aVariant, newV);
        }
        currP = currP->nextProcess;
    }
    printProgressBar(aProcessList->size, aProcessList->size);
}
