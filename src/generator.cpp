#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <pthread.h>
#include <unordered_map>

#include "generator.h"
#include "utils.h"
#include "smartword.h"

using namespace std;

const int Generator::DEFAULT_NUM_THREADS = 3;

const float Generator::MSG_TIME_MIN = 0.5;

const regex Generator::REGEX_COMMA_WHITESPACE = regex("[,\\s]");
const regex Generator::REGEX_NON_NUMERIC = regex("\\D");
const regex Generator::REGEX_NUMERIC = regex("\\d");
const regex Generator::REGEX_LOWERCASE = regex("[a-z]");
const regex Generator::REGEX_UPPERCASE = regex("[A-Z]");

Generator::Generator(shared_ptr<Options> options, shared_ptr<FileBuffer> fb){
    this->options = options;
    this->fb = fb;

	pthread_mutex_init(&combineMutex, NULL);
	pthread_mutex_init(&addLineMutex, NULL);
}
Generator::~Generator(){
    options = nullptr;
    fb = nullptr;
}

void Generator::generate(){
    generate(DEFAULT_NUM_THREADS);
}
void Generator::generate(const int numThreads){
    vector<string> words;
    ksMinOffset = 0;
    if (options->optUsePrepend){
        ksMinOffset++;
        prependSequencesLen = options->getPrependSequences(true).size(); //Build cache
    }
    if (options->optUseAppend){
        ksMinOffset++;
        appendSequencesLen = options->getAppendSequences(true).size(); //Build cache
    }
    totalWordCount = 0;
    wordCount = 0;
    beginTime = chrono::system_clock::now();
    lastFlush = chrono::system_clock::now();
    lastFlush -= chrono::milliseconds((int)(MSG_TIME_MIN * 1000)); //We always want to show at least one message

	int i;
    vector<string> names = Utils::split(options->dataNames, REGEX_COMMA_WHITESPACE);
    vector<string> keywords = Utils::split(options->dataKeywords, REGEX_COMMA_WHITESPACE);
    vector<string> dates = Utils::split(options->dataDates, REGEX_NON_NUMERIC);
    vector<string> numbers = Utils::split(options->dataNumbers, REGEX_NON_NUMERIC);

    //Smart dates
    //Create 2 digit year from 4
    //Strip out non-numerics
    //Create entry without leading 0s
    int datesLen = dates.size();
    for (i = 0; i < datesLen; i++){
        string snum = dates.at(i);
        if (snum.length() == 4){ //Create 2 digit year
            dates.push_back(snum.substr(2, 2));
        }
        int num = -1;
        try {
            num = stoi(snum);
        } catch (exception ex){
            dates.erase(dates.begin() + i);
            datesLen--;
            i--;
        }
        if (num == -1){
            continue;
        }
        string shortNum = to_string(num);
        if (shortNum != snum){ //Leading 0s
            dates.push_back(shortNum);
        }
    }

    Utils::concat(words, names);
    Utils::concat(words, keywords);
    Utils::concat(words, dates);
    Utils::concat(words, numbers);

    filter(words);
#if !_DEBUG
	Utils::randomize(words);
#endif
	cases(words);
    buildSmartWords(words);
    combine(numThreads);
    
    fb->flush();

    chrono::system_clock::time_point now = chrono::system_clock::now();
    chrono::duration<double> delta = now - beginTime;
    cout << endl << "Complete! Generated: " + Utils::formatCommas(to_string(totalWordCount)) + " in " + Utils::formatTime(delta.count()) << endl;
}

//Empty
//Too long
//Contains number if not specified
//Duplicates
void Generator::filter(vector<string>& words){
    int wordsLen = words.size();
    for (int i = 0; i < wordsLen; i++){
        bool shouldRemove = false;
        string word = words.at(i);
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        //Empty / Too long
        if (!word.size() || word.size() > options->ksMax){
            shouldRemove = true;
        }
        //Contains number if not specified
        if (!shouldRemove && !options->ksUseNum && regex_search(word, REGEX_NUMERIC)){
            shouldRemove = true;
        }
        //Duplicates
        if (!shouldRemove){
            for (int j = i + 1; j < wordsLen; j++){
                string word2 = words.at(j);
                transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
                if (word == word2){
                    shouldRemove = true;
                    break;
                }
            }
        }

        if (shouldRemove){
            words.erase(words.begin() + i);
            wordsLen--;
            i--;
        }
    }
}

//Build case variations
void Generator::cases(vector<string>& words){
    int wordsLen = words.size();
    for (int i = 0; i < wordsLen; i++){
        string word = words.at(i);

        //Check state of current word and remove if not supported
        bool shouldRemove = false;
        if (!options->ksUseLower && regex_search(word, REGEX_LOWERCASE)){
            shouldRemove = true;
        }
        if (!options->ksUseUpper && regex_search(word, REGEX_UPPERCASE)){
            shouldRemove = true;
        }
        if (shouldRemove){
            words.erase(words.begin() + i);
            wordsLen--;
            i--;
        }

        //Lowercase
        if (options->ksUseLower){
            string lowerCase = word;
            transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);
            if (lowerCase != word){
                words.insert(words.begin() + i + 1, lowerCase);
                i++;
                wordsLen++;
            }
        }

        //Uppercase
        if (options->ksUseUpper){
            string upperCase = word;
            transform(upperCase.begin(), upperCase.end(), upperCase.begin(), ::toupper);
            if (upperCase != word){
                words.insert(words.begin() + i + 1, upperCase);
                i++;
                wordsLen++;
            }
        }

        //Sentence case
        if (options->ksUseLower && options->ksUseUpper){
            string sentenceCase = word;
            transform(sentenceCase.begin(), sentenceCase.end(), sentenceCase.begin(), ::tolower);
            string firstLetter = sentenceCase.substr(0, 1);
            transform(firstLetter.begin(), firstLetter.end(), firstLetter.begin(), ::toupper);
            sentenceCase = firstLetter + sentenceCase.substr(1, sentenceCase.length() - 1);
            if (sentenceCase != word){
                words.insert(words.begin() + i + 1, sentenceCase);
                i++;
                wordsLen++;
            }
        }
    }
}

void Generator::buildSmartWords(vector<string>& words){
    smartWords.clear();
    int wordsLen = words.size();
    for (int i = 0; i < wordsLen; i++){
        string& word = words.at(i);
        string baseWord = word;
        transform(baseWord.begin(), baseWord.end(), baseWord.begin(), ::tolower);
        int isNumeric = Utils::isNumeric(word);
        smartWords.push_back(shared_ptr<SmartWord>(new SmartWord(word, baseWord, isNumeric)));
    }
}

//Threading
void* threadCombineEntry(void* obj){
    Generator* gen = (Generator*)obj;
    gen->threadCombine();
    return NULL;
}
void Generator::combine(const int numThreads){
    switch (numThreads){
        case 0: //No threading
            cout << "THREADING DISABLED" << endl << endl;
            combine();
            return;
        default:
            cout << "THREADING ENABLED | Using " << numThreads << " threads" << endl << endl;
            break;
    }
    threadWordIndex = 0;

    pthread_t threads[numThreads];
    int i;
    
    for (i = 0; i < numThreads; i++){
        int result = pthread_create(&threads[i], NULL, &threadCombineEntry, this);
        if (result){
            throw "Thread could not be created.";
        }
    }
    //Wait for threads to complete
    for (i = 0; i < numThreads; i++){
        pthread_join(threads[i], NULL);
    }
}
void Generator::threadCombine(){
    int smartWordsLen = smartWords.size();
    while (threadWordIndex < smartWordsLen){
        pthread_mutex_lock(&combineMutex);
            shared_ptr<SmartWord>& smartWord = smartWords.at(threadWordIndex);
            int currentIndex = threadWordIndex;
            threadWordIndex++;
        pthread_mutex_unlock(&combineMutex);
		addVariations(smartWord->word); //Try adding word
		unordered_map<string, unsigned char> wordCounts;
        combine(smartWord->word, wordCounts); //Start at one level past root
    }
}

//Combine words to fill keyspace
void Generator::combine(){
	unordered_map<string, unsigned char> wordCounts;
    combine("", wordCounts);
}
//baseCounts - Number of times each baseWord has been used
void Generator::combine(const string& currentWord, unordered_map<string, unsigned char> wordCounts){
    int currentWordLen = currentWord.length();
    int smartWordsLen = smartWords.size();
    for (int i = 0; i < smartWordsLen; i++){
        shared_ptr<SmartWord>& smartWord = smartWords.at(i);
		string& wordCountKey = smartWord->baseWord;
		
		//Enforce max same words
		if (wordCounts.find(wordCountKey) != wordCounts.end() && wordCounts[wordCountKey] + 1 >= options->optMaxSameWords){
			continue;
		}
		
        string newWord = currentWord + smartWord->word;
        int newWordLen = newWord.length();

        //Enforce max combined nums
        if (smartWord->isNumeric){
            string lastBit = newWord.substr(max(newWordLen - options->optMaxCombinedNums - 1, 0));
            if (lastBit.length() == options->optMaxCombinedNums + 1 && Utils::isNumeric(lastBit)){
                continue;
            }
        }

        addVariations(newWord, currentWordLen);

		if (newWordLen < options->ksMax){
			wordCounts[wordCountKey]++;

			combine(newWord, wordCounts); //Recurse!
			
			wordCounts[wordCountKey]--;
			if (wordCounts[wordCountKey] == 0){
				wordCounts.erase(wordCountKey);
			}
		}
    }
}

void Generator::addVariations(const string& word){
	addVariations(word, 0);
}
void Generator::addVariations(const string& word, const int splitIndex){
	int wordLen = word.length();
	//Check keyspace requirements
	//minOffset because variant prepend/append will add length
	if (wordLen < options->ksMin - ksMinOffset || wordLen > options->ksMax){
		return;
	}
    int i;
    int variationsLen;
    vector<string> variations;

	//Default
	if (wordLen >= options->ksMin){
    	variations.push_back(word);
	}
    
    //Leet
    if (options->optUseLeet){
        variationsLen = variations.size();
        for (i = 0; i < variationsLen; i++){
            leet(variations, variations.at(i));
        }
    }
    //Reduce Duplicate
	if (splitIndex){
		variationsLen = variations.size();
		for (i = 0; i < variationsLen; i++){
			string reduced = reduceDuplicate(variations.at(i), splitIndex);
			if (reduced.length() >= options->ksMin){
				variations.push_back(reduced);
			}
		}
	}
    //Prepend
    if (options->optUsePrepend){
        variationsLen = variations.size();
        for (i = 0; i < variationsLen; i++){
            prepend(variations, variations.at(i));
        }
    }
    //Append
    if (options->optUseAppend){
        variationsLen = variations.size();
        for (i = 0; i < variationsLen; i++){
            append(variations, variations.at(i));
        }
    }
	//Add to file
	addLines(variations);
}

//Leet substitution
void Generator::leet(vector<string>& words, const string& word){
    vector<string> leets;
    string lastWord = word;
    int optLeetsLen = options->optLeets.size();
    for (int i = 0; i < optLeetsLen; i++){
        shared_ptr<Leet> leet = options->optLeets.at(i);
        int fromLen = leet->from.size();
        int toLen = leet->to.size();
        for (int j = 0; j < fromLen; j++){
            string& from = leet->from.at(j);
            for (int k = 0; k < toLen; k++){
                string& to = leet->to.at(k);
                string newWord = lastWord;
                Utils::replaceAll(newWord, from, to);
                if (newWord != lastWord){
                    int newWordLen = newWord.length();
                    if (newWordLen >= options->ksMin && newWordLen <= options->ksMax){
                        leets.push_back(newWord);
                    }
                    lastWord = newWord;
                }
            }
        }
    }
	Utils::concat(words, leets);
}

//Reduce duplicates - "1337" + "70" = "133770" | "133770", 4 = "13370"
string Generator::reduceDuplicate(const string& word, const int splitIndex){
    int wordLen = word.length();
    if (wordLen - 1 >= options->ksMin && word[splitIndex - 1] == word[splitIndex]){
        return word.substr(0, splitIndex - 1) + word.substr(splitIndex, wordLen - splitIndex);
    }
    return "";
}

//Prepend sequences
void Generator::prepend(vector<string>& words, const string& word){
    vector<string> sequences;
    vector<string> prependSequences = options->getPrependSequences();
    for (int i = 0; i < prependSequencesLen; i++){
        string newWord = prependSequences.at(i) + word;
        if (newWord.length() <= options->ksMax){
            sequences.push_back(newWord);
        }
    }
	Utils::concat(words, sequences);
}

//Append sequences
void Generator::append(vector<string>& words, const string& word){
    vector<string> sequences;
    vector<string> appendSequences = options->getAppendSequences();
    for (int i = 0; i < appendSequencesLen; i++){
        string newWord = word + appendSequences.at(i);
        if (newWord.length() <= options->ksMax){
            sequences.push_back(newWord);
        }
    }
	Utils::concat(words, sequences);
}

void Generator::addLine(const string& line){
    pthread_mutex_lock(&addLineMutex);
		bool flushed = fb->addLine(line);
		if (flushed){
			chrono::system_clock::time_point now = chrono::system_clock::now();
			chrono::duration<double> delta = now - lastFlush;
			if (delta.count() >= MSG_TIME_MIN){
				int wps = wordCount / delta.count();
				cout << "Words-Per-Second: " + Utils::formatCommas(to_string(wps)) + " | Total Words: " + Utils::formatCommas(to_string(totalWordCount)) + " | Last Word: " + line << endl;
				wordCount = 0;
				lastFlush = now;
			}
		}
		wordCount++;
		totalWordCount++;
    pthread_mutex_unlock(&addLineMutex);
}

void Generator::addLines(const vector<string>& lines){
	int linesLen = lines.size();
    pthread_mutex_lock(&addLineMutex);
		bool flushed = fb->addLines(lines);
    pthread_mutex_unlock(&addLineMutex);
	if (flushed){
		chrono::system_clock::time_point now = chrono::system_clock::now();
		chrono::duration<double> delta = now - lastFlush;
		if (delta.count() >= MSG_TIME_MIN){
			int wps = wordCount / delta.count();
			cout << "Words-Per-Second: " + Utils::formatCommas(to_string(wps)) + " | Total Words: " + Utils::formatCommas(to_string(totalWordCount)) + " | Last Word: " + lines.at(linesLen - 1) << endl;
			pthread_mutex_lock(&addLineMutex);
				wordCount = 0;
				lastFlush = now;
			pthread_mutex_unlock(&addLineMutex);
		}
	}
    pthread_mutex_lock(&addLineMutex);
		wordCount += linesLen;
		totalWordCount += linesLen;
    pthread_mutex_unlock(&addLineMutex);
}
