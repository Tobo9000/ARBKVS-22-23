//
// Created by tschn on 21.12.2022.
//

#ifndef P5_MULTITASKING_SEMAPHOR_H
#define P5_MULTITASKING_SEMAPHOR_H

#include <mutex>
#include <condition_variable>

/**
 * Unterschied mutex / Semaphor:
 * - Mutex kontrolliert den Zugriff von mehreren Threads auf eine gemeinsame
 *   Ressource und hat die zwei Zustände "locked" und "unlocked". Ein Mutex
 *   erlaubt nur einem Thread den Zugriff auf die Ressource.
 * - Semaphor kontrolliert ebenfalls den Zugriff auf eine gemeinsame Ressource,
 *   kann jedoch mehreren Threads den Zugriff bis zu einer bestimmten Grenze, die
 *   durch den Count des Semaphors festgelegt ist, erlauben.
 */

/**
 * Semaphor-Klasse mit mutex und condition variable.
 * Synchronisiert den Zugriff von mehreren Threads auf eine
 * gemeinsam genutzte Ressource (kritischer Abschnitt), und
 * verhindert so Race Conditions.
 */
class Semaphore {
public:
	explicit Semaphore(int count = 0);

    /**
     * Die wait()-Methode erzeugt eine Sperre auf dem mutex und
     * dekrementiert den Zähler count. Wenn der Zähler kleiner als 0 ist,
     * bedeutet das, dass es mehr Threads gibt, die auf den Semaphor warten,
     * als Ressourcen verfügbar sind. In diesem Fall wird der Thread
     * in die Warteschlange der condition variable eingereiht und wartet
     * dort auf das Signal, dass er weitermachen darf.
     * Die Sperre bleibt dabei bestehen.
     */
	void wait();

    /**
     * Die signal()-Methode erzeugt eine Sperre auf dem mutex und
     * inkrementiert den Zähler count. Wenn der Zähler kleiner oder gleich 0 ist,
     * bedeutet das, dass es Threads gibt, die auf den Semaphor warten.
     * In diesem Fall wird ein Thread aus der Warteschlange der condition
     * variable ausgewählt und aufgeweckt. Der Thread kann dann weitermachen.
     * Die Sperre bleibt dabei bestehen.
     */
	void signal();

private:
	std::mutex mutex;
	std::condition_variable condition;
	int count;
};


#endif //P5_MULTITASKING_SEMAPHOR_H
