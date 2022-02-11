/*
 * Dan Musachio::
 * This file creates an output that can identify with pretty good accuracy the language that a given text is written in through
 * the use of trigrams
 * */

#include "RosettaStone.h"
#include "GUI/SimpleTest.h"
#include "error.h"
#include "math.h"
#include "priorityqueue.h"
using namespace std;

/*
 * given a string and a length,
 * will return a map of all possible kgrams of that length. Error is triggered if kGramLength is less than one.
*/
Map<string, double> kGramsIn(const string& str, int kGramLength) {
    Map<string, double> freqMap;
    if (kGramLength < 1) {
        error("k Gram Length must be at least one");
    }
    if (kGramLength > str.length()){
        return freqMap;
    } else {
        for (int i = 0; i < str.length(); i ++) {
            string kGram = str.substr(i, kGramLength);
            freqMap[kGram] ++;
        } return freqMap;
    }
}

/*
 * given a map of kGrams and the frequency they appear
 * this function will normalize that number between 1 and 0 and return a map of the kGrams and the new normalized number. If the map of
 * trigrams has no trigrams, and error is triggered.
*/
Map<string, double> normalize(const Map<string, double>& input) {
    double totalNumberOfTrigrams = 0;
    Map<string, double> normalized;

    for (string key: input) {
        totalNumberOfTrigrams = totalNumberOfTrigrams + pow(input[key], 2);
    } if (totalNumberOfTrigrams == 0) {
        error("can't divide by 0");
    } for (string key: input){
        normalized[key] = input[key] / sqrt(totalNumberOfTrigrams);
    } return normalized;
}

/*
 * given a map of normalized kGrams and a number "numToKeep
 * this function will keep "numToKeep" of the most frequently used kgrams and return them in a map along with their normalized number. If the numToKeep
 * is less than 0, an error is triggered.
*/
Map<string, double> topKGramsIn(const Map<string, double>& source, int numToKeep) {
    Map<string, double> topKGrams;
    PriorityQueue<string> pq;
    if (numToKeep < 0) { // you can't keep negative 5 k grams, has to keep at least 0
        error("Can't be negative");
    }
    if (numToKeep == 0) {
        return topKGrams;
    }
    if (numToKeep > source.size()) {
        return source;
    }
    for (string key: source) {
        pq.enqueue(key, source[key]);
    }
    while (pq.size() > numToKeep) {
        pq.dequeue();
    }
    while (!pq.isEmpty()) {
        string kGram = pq.dequeue();
        topKGrams[kGram] = source[kGram];
    }
    return topKGrams;
}

/*
 * given two maps, this function computes
 * their cosine similarity, or in other words, how similar their values are
 */
double cosineSimilarityOf(const Map<string, double>& lhs, const Map<string, double>& rhs) {
    double cosineSimilarity = 0;
    for (string key: lhs) {
        if (rhs[key] != 0) {
            cosineSimilarity = cosineSimilarity + (lhs[key] * rhs[key]);
        }
    } return cosineSimilarity;
}

/*
 * given a text profile and a set of corpora, this function
 * goes through all of the corpora and sees which one returns the highest cosineSimilarity. That is returned as the
 * "guessed language." If the corpora is empty, an error is sent out.
 */
string guessLanguageOf(const Map<string, double>& textProfile,
                       const Set<Corpus>& corpora) {
    double topCosine = 0;
    string topLanguage = "";
    for (Corpus structure: corpora){
        double cosineSimilarity = cosineSimilarityOf(textProfile, structure.profile);
        if (cosineSimilarity > topCosine){
            topLanguage = structure.name;
            topCosine = cosineSimilarity;
        }
    }
    if (topLanguage == ""){
        error("corpora can't be empty");
    } return topLanguage;
}

